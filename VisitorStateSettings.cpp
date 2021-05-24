/******************************************************************************/
/*!
file    VisitorStateSettings.cpp
\author Woonggi Eun
\par    email: dmsdndre\@gmail.com
\date   2016-03-14
\description
Logic component for VisitorStateSettings
All content (C) 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "VisitorStateSettings.h"
#include "../Menu/TypingAnimation.h"
#include "../Menu/FadeInOut.h"
#include "../../../Engine/BaseEngine.h"
#include "../../../Engine/System/PhysicsSystem/Collision/CollisionEvent.h"

/******************************************************************************/
/*!

\brief - Logic builder constructor

*/
/******************************************************************************/
VisitorStateSettingsBuilder::VisitorStateSettingsBuilder()
: ComponentBuilder()
{ }

/******************************************************************************/
/*!

\brief - Logic builder destructor

*/
/******************************************************************************/
VisitorStateSettingsBuilder::~VisitorStateSettingsBuilder()
{ }

/******************************************************************************/
/*!

\brief - Component building for logic

\param - Owner object of this logic

*/
/******************************************************************************/
Component* VisitorStateSettingsBuilder::create(ObjectPtr owner) const
{
	return new VisitorStateSettings(owner);
}

/******************************************************************************/
/*!

\brief - Constructor of logic

*/
/******************************************************************************/
VisitorStateSettings::VisitorStateSettings(ObjectPtr pObject)
:Logic(pObject), m_hired(false), m_timer(1.f), m_timerDup(m_timer),
m_choice(false), m_upDown(true), m_beforeHire(true)
{
	m_pSystem->AddLogic(this);
}

/******************************************************************************/
/*!

\brief - Destructor of logic

*/
/******************************************************************************/
VisitorStateSettings::~VisitorStateSettings(void)
{
	m_pSystem->DeleteLogic(this);
}

/******************************************************************************/
/*!


\brief - Load data for this logic in JSON file.

\return - return true if it is loaded succesfully. Otherwise, return false.

*/
/******************************************************************************/
bool VisitorStateSettings::Load(const rapidjson::Value& data)
{
	UNREFERENCED_PARAMETER(data);
	return true;
}

/******************************************************************************/
/*!

\brief - Initial setting for this logic

*/
/******************************************************************************/
void VisitorStateSettings::InitLogic(void)
{
	script = m_OBM->GetGameObject("Script");
	visitor = m_OBM->GetGameObject("Visitor");
	m_gameData = m_pSystem->GetGameData();

	//Visitor type random setting.
	if (m_pSystem->GetGameData()->day == 4)
		visitorType = thief;

	else
	{
		if (m_pSystem->GetGameData()->day % 3)
			visitorType = static_cast<Visitor>(Random::GIRand().RandInt(dog, flower));

		else
			visitorType = static_cast<Visitor>(Random::GIRand().RandInt(hunter, knight));
	}

	//If you already hired them, then state will be restarted
	if ((visitorType == thief && m_gameData->day < 4) ||
		(visitorType == dog && m_gameData->dog) ||
		(visitorType == compu && m_gameData->compu))
		m_OBM->StateManager()->SetRestart(true);

	InitialVisitorSet(visitorType);
}

/******************************************************************************/
/*!

\brief - Update logic.

\param - Value for time-based calculation.

*/
/******************************************************************************/
void VisitorStateSettings::UpdateLogic(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	m_textPostion = 10;
	m_textScale = 22.5;

	if (script->GetComponent<TypingAnimation>()->GetEnd())
	{
		m_OBM->GetGameObject("UI_YesNOBox")->GetComponent<Sprite>()->SetColor(1, 1, 1, 1);
		m_OBM->GetGameObject("UI_Yes")->GetComponent<Text>()->SetColor(1, 1, 1, 1);
		m_OBM->GetGameObject("UI_No")->GetComponent<Text>()->SetColor(1, 1, 1, 1);
		m_OBM->GetGameObject("Arrow")->GetComponent<Sprite>()->SetColor(1, 1, 1, 1);
	}

	if (m_choice && !m_hired && m_upDown)
	{
		m_hired = true;
		HireVisitors(visitorType);
	}

	else if (m_choice && !m_upDown)
		m_OBM->GetGameObject("FadeIO")->GetComponent<FadeInOut>()->FadeOut_SetNextStage(ST_RESULT);

	if (m_beforeHire)
	{
		KeyboardControl();
		if (script->GetComponent<TypingAnimation>()->GetEnd())
		{
			MouseControl("UI_Yes");
			MouseControl("UI_No");
		}

	}
	else
		AfterHired(dt);
}

/******************************************************************************/
/*!

\brief - Function that activated when this logic shutdown.


*/
/******************************************************************************/
void VisitorStateSettings::ShutLogic(void)
{

}
/******************************************************************************/
/*!

\brief - State settings for each random visitors. Each visitor has different 
         script and costs.

\param - Enum for visitor


*/
/******************************************************************************/
void VisitorStateSettings::InitialVisitorSet(Visitor v)
{
	switch (v)
	{
		//main
	case hunter:
		//Localization
		script->GetComponent<Text>()->SetInput("I am a hunter. Hire me!\nCost: %i / Money: %d", m_gameData->hunterCost, m_gameData->money);
		visitor->GetComponent<Transform>()->SetPosition(
			visitor->GetComponent<Transform>()->GetPosition() + Vector(5, 10));
		visitor->GetComponent<Transform>()->SetScale(Vector(45, 100));
		visitor->GetComponent<Sprite>()->SetTexture(m_OBM->Assets()->GetTexture("Hunter"));
		visitor->GetComponent<Sprite>()->SetAnimation(2, 1.f);
		break;

	case wizard:
		//Localization
		script->GetComponent<Text>()->SetInput("I am a wizard. Hire me!\nCost: %i / Money: %d", m_gameData->wizardCost, m_gameData->money);

		visitor->GetComponent<Transform>()->SetScale(Vector(70, 90));
		visitor->GetComponent<Sprite>()->SetTexture(m_OBM->Assets()->GetTexture("Wizard"));
		visitor->GetComponent<Sprite>()->SetAnimation(2, 1.f);
		break;

	case knight:
		//Localization
		script->GetComponent<Text>()->SetInput("I am a knight. Hire me!\nCost: %i / Money: %d", m_gameData->knightCost, m_gameData->money);

		visitor->GetComponent<Transform>()->SetScale(Vector(60, 110));
		visitor->GetComponent<Transform>()->SetPosition(
			visitor->GetComponent<Transform>()->GetPosition() + Vector(5, 20));
		visitor->GetComponent<Sprite>()->SetTexture(m_OBM->Assets()->GetTexture("Knight"));
		visitor->GetComponent<Sprite>()->SetAnimation(2, 1.f);
		break;

		//extra
	case compu:
		//Localization
		script->GetComponent<Text>()->SetInput("I am Hacompu. Hire me!\nCost: %i / Money: %d", m_gameData->compuCost, m_gameData->money);
		visitor->GetComponent<Sprite>()->SetTexture(m_OBM->Assets()->GetTexture("compu"));
		visitor->GetComponent<Sprite>()->SetAnimation(2, 1.5f);
		break;

	case flower:
		//Localization
		script->GetComponent<Text>()->SetInput("I am a flower. Hire me!\nCost: %i / Money: %d", m_gameData->flowerCost, m_gameData->money);

		visitor->GetComponent<Sprite>()->SetTexture(m_OBM->Assets()->GetTexture("flower"));
		visitor->GetComponent<Sprite>()->SetAnimation(2, 1.5f);
		break;

	case pigs:
		//Localization
		script->GetComponent<Text>()->SetInput("We are three little pigs. Hire us!\nCost: %i / Money: %d", m_gameData->pigsCost, m_gameData->money);

		visitor->GetComponent<Transform>()->SetScale(Vector(100, 80));
		visitor->GetComponent<Sprite>()->SetTexture(m_OBM->Assets()->GetTexture("pigs"));
		visitor->GetComponent<Sprite>()->SetAnimation(3, 2.f);
		break;

	case dog:
		//Localization
		script->GetComponent<Text>()->SetInput("Bow wow! Bow wow! Bow wow! \nCost: %i / Money: %d", m_gameData->dogCost, m_gameData->money);

		visitor->GetComponent<Sprite>()->SetTexture(m_OBM->Assets()->GetTexture("dog"));
		visitor->GetComponent<Sprite>()->SetAnimation(2, 1.5f);
		break;

	case thief:
		//Localization
		script->GetComponent<Text>()->SetInput("I am a thief. Hire me!\nCost: %i / Money: %d", m_gameData->thiefCost, m_gameData->money);
		visitor->GetComponent<Transform>()->SetPosition(
			visitor->GetComponent<Transform>()->GetPosition() + Vector(0, 20));
		visitor->GetComponent<Transform>()->SetScale(Vector(70, 100));
		visitor->GetComponent<Sprite>()->SetTexture(m_OBM->Assets()->GetTexture("thief"));
		visitor->GetComponent<Sprite>()->SetAnimation(2, 1.5f);
		break;

	}

	script->GetComponent<TypingAnimation>()->SetSpeed(70.f);
}

/******************************************************************************/
/*!

\brief - Function that hires visitor.

\param - Visitor type to hire.


*/
/******************************************************************************/
void VisitorStateSettings::HireVisitors(Visitor v)
{
	switch (v)
	{
	case hunter:
		if (m_gameData->hunterCost <= m_gameData->money)
		{
			script->GetComponent<TypingAnimation>()->InitSentence("You hired a hunter!");
			m_gameData->hunter++;
			m_gameData->money -= m_gameData->hunterCost;
			m_OBM->GetSubsystem<SoundSystem>()->PlaySE("SpendMoney");
			m_hired = true;
		}
		else
			script->GetComponent<TypingAnimation>()->InitSentence("Not Enough Money...");

		break;
	case wizard:
		if (m_gameData->wizardCost <= m_gameData->money)
		{
			script->GetComponent<TypingAnimation>()->InitSentence("You hired a wizard!");
			m_gameData->wizard++;
			m_gameData->money -= m_gameData->wizardCost;
			m_OBM->GetSubsystem<SoundSystem>()->PlaySE("SpendMoney");
			m_hired = true;
		}
		else
			script->GetComponent<TypingAnimation>()->InitSentence("Not Enough Money...");
		break;

	case knight:
		if (m_gameData->wizardCost <= m_gameData->money)
		{
			script->GetComponent<TypingAnimation>()->InitSentence("You hired a knight!");
			m_gameData->knight++;
			m_gameData->money -= m_gameData->knightCost;
			m_OBM->GetSubsystem<SoundSystem>()->PlaySE("SpendMoney");
			m_hired = true;
		}
		else
			script->GetComponent<TypingAnimation>()->InitSentence("Not Enough Money...");
		break;

	case compu:
		if (m_gameData->compuCost <= m_gameData->money)
		{
			script->GetComponent<TypingAnimation>()->InitSentence("You hired Hacompu!");
			m_gameData->compu = true;
			m_gameData->money -= m_gameData->compuCost;
			m_OBM->GetSubsystem<SoundSystem>()->PlaySE("SpendMoney");
			m_hired = true;
		}
		else
			script->GetComponent<TypingAnimation>()->InitSentence("Not Enough Money...");

		break;
	case flower:
		if (m_gameData->flowerCost <= m_gameData->money)
		{
			script->GetComponent<TypingAnimation>()->InitSentence("You hired a flower!");
			m_gameData->flower++;
			m_gameData->money -= m_gameData->flowerCost;
			m_OBM->GetSubsystem<SoundSystem>()->PlaySE("SpendMoney");
			m_hired = true;
		}
		else
			script->GetComponent<TypingAnimation>()->InitSentence("Not Enough Money...");
		break;

	case pigs:
		if (m_gameData->pigsCost <= m_gameData->money)
		{
			script->GetComponent<TypingAnimation>()->InitSentence("You've got 300 foods!");

			m_gameData->pigs++;
			m_gameData->food += 300;
			m_gameData->money -= m_gameData->pigsCost;
			m_OBM->GetSubsystem<SoundSystem>()->PlaySE("SpendMoney");
			m_hired = true;
		}
		else
			script->GetComponent<TypingAnimation>()->InitSentence("Not Enough Money...");

		break;
	case dog:
		if (m_gameData->dogCost <= m_gameData->money)
		{
			script->GetComponent<TypingAnimation>()->InitSentence("You hired a dog!");

			m_gameData->dog = true;
			m_gameData->money -= m_gameData->dogCost;
			m_OBM->GetSubsystem<SoundSystem>()->PlaySE("SpendMoney");
			m_hired = true;
		}
		else
			script->GetComponent<TypingAnimation>()->InitSentence("Not Enough Money...");

		break;
	case thief:
		if (m_gameData->thiefCost <= m_gameData->money)
		{
			int lost = Random::GIRand().RandInt(700, 1000);
			script->GetComponent<TypingAnimation>()->InitSentence("You lost %d golds!", lost);

			m_gameData->money -= lost;
			m_OBM->GetSubsystem<SoundSystem>()->PlaySE("SpendMoney");
			m_hired = true;
		}
		else
			script->GetComponent<TypingAnimation>()->InitSentence("Not Enough Money...");

		break;
	}
}

/******************************************************************************/
/*!

\brief - After hire the visitor, there is a time interval between states.

\param - Time interval to wait for.

*/
/******************************************************************************/
void VisitorStateSettings::AfterHired(float dt)
{
	if (m_hired && m_choice)
	{
		m_timer -= dt;

		if (m_timer <= 0)
		{
			m_OBM->GetGameObject("FadeIO")->GetComponent<FadeInOut>()->FadeOut_SetNextStage(ST_RESULT);
		}
	}
}

/******************************************************************************/
/*!

\brief - Keyboard controls to buy or not.


*/
/******************************************************************************/
void VisitorStateSettings::KeyboardControl(void)
{
	if (InputManager::GIInput().IsKeyTriggered(K_ARROW_DOWN))
	{
		m_upDown = false;
		m_OBM->GetGameObject("Arrow")->GetComponent<Transform>()->SetPosition(Vector(310, -255));
	}

	if (InputManager::GIInput().IsKeyTriggered(K_ARROW_UP))
	{
		m_upDown = true;
		m_OBM->GetGameObject("Arrow")->GetComponent<Transform>()->SetPosition(Vector(310, -200));
	}

	if (InputManager::GIInput().IsKeyTriggered(K_ENTER))
	{
		m_choice = true;
		m_beforeHire = false;
	}
}

/******************************************************************************/
/*!

\brief - Mouse control to buy or not.
 
\param - Name of object that intersect with mouse.

*/
/******************************************************************************/
void VisitorStateSettings::MouseControl(const std::string& name)
{
	if (m_OBM->GetGameObject(name))
	{
		bool mouse_on_obj =
			ColDetect::SquareToSquare(
			InputManager::GIInput().GetOrthoMouse(),
			Vector(0.1f, .1f, 0.f),
			0.f,
			m_OBM->GetGameObject(name)->GetComponent<Transform>()->GetPosition() + Vector(
			m_OBM->GetGameObject(name)->GetComponent<Text>()->GetSize() * m_textPostion, 18.5),
			m_OBM->GetGameObject(name)->GetComponent<Transform>()->GetScale() + Vector(
			m_OBM->GetGameObject(name)->GetComponent<Text>()->GetSize() * m_textScale, 40),
			0.f);

		m_OBM->GetGameObject(name)->GetComponent<Text>()->SetColor(1, 1, 1, 1);

		if (mouse_on_obj)
		{
			m_OBM->GetGameObject(name)->GetComponent<Text>()->SetColor(1, 1, 0, 1);

			if (InputManager::GIInput().IsKeyPressed(K_MOUSE_LEFT))
			{
				if (!strcmp(name.c_str(), "UI_Yes"))
				{
					m_upDown = true;
					m_OBM->GetGameObject("Arrow")->GetComponent<Transform>()->SetPosition(Vector(310, -200));
				}

				else if (!strcmp(name.c_str(), "UI_No"))
				{
					m_upDown = false;
					m_OBM->GetGameObject("Arrow")->GetComponent<Transform>()->SetPosition(Vector(310, -255));
				}

				m_choice = true;
				m_beforeHire = false;
			}
		}
	}
}