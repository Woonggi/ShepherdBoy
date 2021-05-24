/******************************************************************************/
/*!
file    EndingLogic.cpp
\author j.jeong
\par    email: p084111\@gmail.com
\date   2016-05-12
\description
Logic component for EndingLogic
All content (C) 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
 
#include "EndingLogic.h"
#include "../GameLogic/Menu/TypingAnimation.h"
#include "../GameLogic/Menu/FadeInOut.h"
#include "../../Engine/BaseEngine.h"
#include "../../Engine/System/PhysicsSystem/Collision/CollisionEvent.h"
/******************************************************************************/
/*!
\brief Constructor for EndingLogic Builder
*/
/******************************************************************************/
EndingLogicBuilder::EndingLogicBuilder()
	: ComponentBuilder()
{ }
/******************************************************************************/
/*!
\brief Destructor for EndingLogic Builder
*/
/******************************************************************************/
EndingLogicBuilder::~EndingLogicBuilder()
{ }
/******************************************************************************/
/*!
\brief Create for EndingLogic
*/
/******************************************************************************/
Component* EndingLogicBuilder::create(ObjectPtr owner) const
{
  return new EndingLogic(owner);
}
/******************************************************************************/
/*!
\brief Constructor for EndingLogic
\param pObject
*/
/******************************************************************************/
EndingLogic::EndingLogic(ObjectPtr pObject)
:Logic(pObject), wolf_speed(200), crow_speed(400),
scene_changed(true), goto_next(false), opacity(0)
{
  m_pSystem->AddLogic(this);
}
/******************************************************************************/
/*!
\brief Destructor for EndingLogic
*/
/******************************************************************************/
EndingLogic::~EndingLogic(void)
{
	m_pSystem->DeleteLogic(this);
}
/******************************************************************************/
/*!
\brief Load json data
\param data
\return true
*/
/******************************************************************************/
bool EndingLogic::Load(const rapidjson::Value& data)
{
  UNREFERENCED_PARAMETER(data);
  return true;
}
/******************************************************************************/
/*!
\brief Init EndingLogic 
*/
/******************************************************************************/
void EndingLogic::InitLogic(void)
{
	//Set sprites' starting info
	m_OBM->GetGameObject("FadeIO")->GetComponent<FadeInOut>()->SetSpeed(0.5);
	if (m_pSystem->GetGameData()->game_condition)
	{
		m_OBM->GetGameObject("Crow1")->
			GetComponent<Sprite>()->SetTexture(m_OBM->Assets()->GetTexture("Win_Crow"));
		m_OBM->GetGameObject("Crow2")->
			GetComponent<Sprite>()->SetTexture(m_OBM->Assets()->GetTexture("Win_Crow"));
		m_OBM->GetGameObject("Crow3")->
			GetComponent<Sprite>()->SetTexture(m_OBM->Assets()->GetTexture("Win_Crow"));
		m_OBM->GetGameObject("Crow1")->
			GetComponent<Transform>()->SetScale(Vector(100, 90));
		m_OBM->GetGameObject("Crow2")->
			GetComponent<Transform>()->SetScale(Vector(100, 90));
		m_OBM->GetGameObject("Crow3")->
			GetComponent<Transform>()->SetScale(Vector(100, 90));
		m_OBM->GetGameObject("Wolf1")->
			GetComponent<Sprite>()->SetTexture(m_OBM->Assets()->GetTexture("Win_Wolf"));
		m_OBM->GetGameObject("Wolf2")->
			GetComponent<Sprite>()->SetTexture(m_OBM->Assets()->GetTexture("Win_Wolf"));
		m_OBM->GetGameObject("Wolf3")->
			GetComponent<Sprite>()->SetTexture(m_OBM->Assets()->GetTexture("Win_Wolf"));
		m_OBM->GetGameObject("Wolf1")->
			GetComponent<Sprite>()->SetAnimation(3, 6);
		m_OBM->GetGameObject("Wolf2")->
			GetComponent<Sprite>()->SetAnimation(3, 6);
		m_OBM->GetGameObject("Wolf3")->
			GetComponent<Sprite>()->SetAnimation(3, 6);
	}
}
/******************************************************************************/
/*!
\brief Update EndingLogic 
\param dt
*/
/******************************************************************************/
void EndingLogic::UpdateLogic(float dt)
{
  UNREFERENCED_PARAMETER(dt);
  
  //Work win case animation
  if (m_pSystem->GetGameData()->game_condition)
	WinAnimation(dt);
  
  //lose animation
  else
	LoseAnimation(dt); 
}
/******************************************************************************/
/*!
\brief Shut EndingLogic
*/
/******************************************************************************/
void EndingLogic::ShutLogic(void)
{
 
}
 
/******************************************************************************/
/*!
\brief Win animation
\param dt
*/
/******************************************************************************/
void EndingLogic::WinAnimation(float dt)
{
	//Start to move
	m_OBM->GetGameObject("Crow1")->
		GetComponent<Transform>()->
		SetPosition(Vector(
		m_OBM->GetGameObject("Crow1")->
		GetComponent<Transform>()->
		GetPosition().x - crow_speed * dt,
		m_OBM->GetGameObject("Crow1")->
		GetComponent<Transform>()->
		GetPosition().y + dt * 10,
		m_OBM->GetGameObject("Crow1")->
		GetComponent<Transform>()->
		GetPosition().z));

	m_OBM->GetGameObject("Crow2")->
		GetComponent<Transform>()->
		SetPosition(Vector(
		m_OBM->GetGameObject("Crow2")->
		GetComponent<Transform>()->
		GetPosition().x - crow_speed * dt,
		m_OBM->GetGameObject("Crow2")->
		GetComponent<Transform>()->
		GetPosition().y,
		m_OBM->GetGameObject("Crow2")->
		GetComponent<Transform>()->
		GetPosition().z));

	m_OBM->GetGameObject("Crow3")->
		GetComponent<Transform>()->
		SetPosition(Vector(
		m_OBM->GetGameObject("Crow3")->
		GetComponent<Transform>()->
		GetPosition().x - crow_speed * dt,
		m_OBM->GetGameObject("Crow3")->
		GetComponent<Transform>()->
		GetPosition().y - dt * 10,
		m_OBM->GetGameObject("Crow3")->
		GetComponent<Transform>()->
		GetPosition().z));

	m_OBM->GetGameObject("Wolf1")->
		GetComponent<Transform>()->
		SetPosition(Vector(
		m_OBM->GetGameObject("Wolf1")->
		GetComponent<Transform>()->
		GetPosition().x - wolf_speed * dt,
		m_OBM->GetGameObject("Wolf1")->
		GetComponent<Transform>()->
		GetPosition().y,
		m_OBM->GetGameObject("Wolf1")->
		GetComponent<Transform>()->
		GetPosition().z));

	m_OBM->GetGameObject("Wolf2")->
		GetComponent<Transform>()->
		SetPosition(Vector(
		m_OBM->GetGameObject("Wolf2")->
		GetComponent<Transform>()->
		GetPosition().x - wolf_speed * dt,
		m_OBM->GetGameObject("Wolf2")->
		GetComponent<Transform>()->
		GetPosition().y,
		m_OBM->GetGameObject("Wolf2")->
		GetComponent<Transform>()->
		GetPosition().z));

	m_OBM->GetGameObject("Wolf3")->
		GetComponent<Transform>()->
		SetPosition(Vector(
		m_OBM->GetGameObject("Wolf3")->
		GetComponent<Transform>()->
		GetPosition().x - wolf_speed * dt,
		m_OBM->GetGameObject("Wolf3")->
		GetComponent<Transform>()->
		GetPosition().y,
		m_OBM->GetGameObject("Wolf3")->
		GetComponent<Transform>()->
		GetPosition().z));

	//Stop middle wolf and he says
	if (m_OBM->GetGameObject("Wolf2")->
		GetComponent<Transform>()->
		GetPosition().x <= 0)
	{
		m_OBM->GetGameObject("Wolf2")->
			GetComponent<Transform>()->
			SetPosition(Vector(0,
			m_OBM->GetGameObject("Wolf2")->
			GetComponent<Transform>()->
			GetPosition().y,
			m_OBM->GetGameObject("Wolf2")->
			GetComponent<Transform>()->
			GetPosition().z));

		if (scene_changed)
		{
			m_OBM->GetGameObject("Wolf2")->
				GetComponent<Sprite>()->FixAnimation(1, 3);
			m_OBM->GetGameObject("Script")->GetComponent<TypingAnimation>()->SetSpeed(35);
			m_OBM->GetGameObject("Script")->GetComponent<TypingAnimation>()->InitSentence("You....you...Krrrrr..\nYou won.....!!!");
			scene_changed = false;
		}
	}

	//Move to the main menu
	if (m_OBM->GetGameObject("Wolf3")->
		GetComponent<Transform>()->
		GetPosition().x <= -850)
	{
		m_OBM->GetGameObject("FadeIO")->GetComponent<FadeInOut>()->FadeOut_SetNextStage(ST_NEWS);
	}

	//Show text
	if (!scene_changed)
	{
		opacity += 0.005f;
		m_OBM->GetGameObject("Spotlight")->
			GetComponent<Transform>()->SetScale(Vector(750, 100));
		m_OBM->GetGameObject("Spotlight")->
			GetComponent<Sprite>()->SetColor(1, 1, 1, opacity);

		if (opacity >= 1)
		{
			opacity = 1;
            static bool play_1 = false;
            if (!play_1)
            {
                m_OBM->GetSubsystem<SoundSystem>()->PlaySE("Win");
                play_1 = true;
            }
			m_OBM->GetGameObject("Condition")->GetComponent<Text>()->SetInput("You Win");
			m_OBM->GetGameObject("Condition")->GetComponent<Text>()->SetColor(1, 1, 1, 1);
		}
	}
}

/******************************************************************************/
/*!
\brief Lose animation
\param dt
*/
/******************************************************************************/
void EndingLogic::LoseAnimation(float dt)
{
	//Start to move
	m_OBM->GetGameObject("Crow1")->
		GetComponent<Transform>()->
		SetPosition(Vector(
		m_OBM->GetGameObject("Crow1")->
		GetComponent<Transform>()->
		GetPosition().x - crow_speed * dt,
		m_OBM->GetGameObject("Crow1")->
		GetComponent<Transform>()->
		GetPosition().y + dt * 10,
		m_OBM->GetGameObject("Crow1")->
		GetComponent<Transform>()->
		GetPosition().z));

	m_OBM->GetGameObject("Crow2")->
		GetComponent<Transform>()->
		SetPosition(Vector(
		m_OBM->GetGameObject("Crow2")->
		GetComponent<Transform>()->
		GetPosition().x - crow_speed * dt,
		m_OBM->GetGameObject("Crow2")->
		GetComponent<Transform>()->
		GetPosition().y,
		m_OBM->GetGameObject("Crow2")->
		GetComponent<Transform>()->
		GetPosition().z));

	m_OBM->GetGameObject("Crow3")->
		GetComponent<Transform>()->
		SetPosition(Vector(
		m_OBM->GetGameObject("Crow3")->
		GetComponent<Transform>()->
		GetPosition().x - crow_speed * dt,
		m_OBM->GetGameObject("Crow3")->
		GetComponent<Transform>()->
		GetPosition().y - dt * 10,
		m_OBM->GetGameObject("Crow3")->
		GetComponent<Transform>()->
		GetPosition().z));

	m_OBM->GetGameObject("Wolf1")->
		GetComponent<Transform>()->
		SetPosition(Vector(
		m_OBM->GetGameObject("Wolf1")->
		GetComponent<Transform>()->
		GetPosition().x - wolf_speed * dt,
		m_OBM->GetGameObject("Wolf1")->
		GetComponent<Transform>()->
		GetPosition().y,
		m_OBM->GetGameObject("Wolf1")->
		GetComponent<Transform>()->
		GetPosition().z));

	m_OBM->GetGameObject("Wolf2")->
		GetComponent<Transform>()->
		SetPosition(Vector(
		m_OBM->GetGameObject("Wolf2")->
		GetComponent<Transform>()->
		GetPosition().x - wolf_speed * dt,
		m_OBM->GetGameObject("Wolf2")->
		GetComponent<Transform>()->
		GetPosition().y,
		m_OBM->GetGameObject("Wolf2")->
		GetComponent<Transform>()->
		GetPosition().z));

	m_OBM->GetGameObject("Wolf3")->
		GetComponent<Transform>()->
		SetPosition(Vector(
		m_OBM->GetGameObject("Wolf3")->
		GetComponent<Transform>()->
		GetPosition().x - wolf_speed * dt,
		m_OBM->GetGameObject("Wolf3")->
		GetComponent<Transform>()->
		GetPosition().y,
		m_OBM->GetGameObject("Wolf3")->
		GetComponent<Transform>()->
		GetPosition().z));

	//Stop middle wolf and he say
	if (m_OBM->GetGameObject("Wolf2")->
		GetComponent<Transform>()->
		GetPosition().x <= 0)
	{
		m_OBM->GetGameObject("Wolf2")->
			GetComponent<Transform>()->
			SetPosition(Vector(0,
			m_OBM->GetGameObject("Wolf2")->
			GetComponent<Transform>()->
			GetPosition().y,
			m_OBM->GetGameObject("Wolf2")->
			GetComponent<Transform>()->
			GetPosition().z));

		if (scene_changed)
		{
			m_OBM->GetGameObject("Wolf2")->
				GetComponent<Sprite>()->FixAnimation(1, 5);
			m_OBM->GetGameObject("Script")->GetComponent<TypingAnimation>()->SetSpeed(70);
			m_OBM->GetGameObject("Script")->GetComponent<TypingAnimation>()->InitSentence("Ah hahahahahahahaha!\nYou L.O.S.E.R!!");
			scene_changed = false;
		}

		m_OBM->GetGameObject("Wolf2")->
			GetComponent<Sprite>()->SetAnimation(2, 2);
		m_OBM->GetGameObject("Wolf2")->
			GetComponent<Sprite>()->SetTexture(m_OBM->Assets()->GetTexture("Lose_Wolf2"));
	}

	//Move to the main menu
	if (m_OBM->GetGameObject("Wolf3")->
		GetComponent<Transform>()->
		GetPosition().x <= -850)
	{
		m_OBM->GetGameObject("FadeIO")->GetComponent<FadeInOut>()->FadeOut_SetNextStage(ST_MENU);
	}

	//Show texts
	if (!scene_changed)
	{
		opacity += 0.005f;
		m_OBM->GetGameObject("Spotlight")->
			GetComponent<Transform>()->SetScale(Vector(750, 100));
		m_OBM->GetGameObject("Spotlight")->
			GetComponent<Sprite>()->SetColor(1, 1, 1, opacity);

		if (opacity >= 1)
		{
			opacity = 1;
            static bool play_2 = false;
            if (!play_2)
            {
                m_OBM->GetSubsystem<SoundSystem>()->PlaySE("Lose");
                play_2 = true;
            }
			m_OBM->GetGameObject("Condition")->GetComponent<Text>()->SetInput("You Lose");
			m_OBM->GetGameObject("Condition")->GetComponent<Text>()->SetColor(1, 1, 1, 1);
		}
	}
}