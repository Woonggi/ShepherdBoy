/******************************************************************************/
/*!
file    TutorialLogic.cpp
\author Woonggi Eun
\par    email: dmsdndre\@gmail.com
\date   2016-05-31
\description
Logic component for TutorialLogic
All content (C) 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
 
#include "TutorialLogic.h"
#include "../../../Engine/BaseEngine.h"
#include "../../../Engine/System/PhysicsSystem/Collision/CollisionEvent.h"

namespace
{
    //Max number of items
    const int ITEMS = 5;

    //Index of each elements;
    const int SHEEP = 0;
    const int MONEY = 1;
    const int FENCE = 2;
    const int FOOD  = 3;
	const int DAY   = 4;
}
 
TutorialLogicBuilder::TutorialLogicBuilder()
	: ComponentBuilder()
{ }
 
TutorialLogicBuilder::~TutorialLogicBuilder()
{ }
 
Component* TutorialLogicBuilder::create(ObjectPtr owner) const
{
  return new TutorialLogic(owner);
}
 
TutorialLogic::TutorialLogic(ObjectPtr pObject)
  :Logic(pObject)
{
  m_pSystem->AddLogic(this);
}
 
TutorialLogic::~TutorialLogic(void)
{
	//Delete data that allocated
    delete[] desc;
    delete[] descTxt;
    m_pSystem->DeleteLogic(this);
}
 
bool TutorialLogic::Load(const rapidjson::Value& data)
{
  UNREFERENCED_PARAMETER(data);
  return true;
}

/******************************************************************************/
/*!

\brief - Initialize logic when it is made by user.

*/
/******************************************************************************/
void TutorialLogic::InitLogic(void)
{
	signCheck   = false;
	dayCheck    = false;
	done        = false;
    desc        = new ObjectPtr[ITEMS]; 
	descTxt     = new ObjectPtr[ITEMS];

	//Set object pointer to certain objects.
    desc[SHEEP] = m_OBM->GetGameObject("SheepBox");
    desc[MONEY] = m_OBM->GetGameObject("MoneyBox");
    desc[FENCE] = m_OBM->GetGameObject("FenceBox");
    desc[FOOD]  = m_OBM->GetGameObject("FoodBox");
	desc[DAY]   = m_OBM->GetGameObject("DayBox");

	descTxt[SHEEP] = m_OBM->GetGameObject("SheepText");
	descTxt[MONEY] = m_OBM->GetGameObject("MoneyText");
	descTxt[FENCE] = m_OBM->GetGameObject("FenceText");
	descTxt[FOOD]  = m_OBM->GetGameObject("FoodText");
	descTxt[DAY]   = m_OBM->GetGameObject("DayText");

    mouse      = m_OBM->GetGameObject("Mouse_Cursor");
    sign       = m_OBM->GetGameObject("Sign");
    descWindow = m_OBM->GetGameObject("DescWindow");
    descScript = m_OBM->GetGameObject("DescScript");
}
/******************************************************************************/
/*!

\brief - Function that return true if the mouse is on the object getting from
         parameter

\param pObj - Target object to check is it intersect with mouse cursor.

*/
/******************************************************************************/
bool TutorialLogic::MouseEvent(ObjectPtr pObj)
{
	//Check intersection between pObj and mouse cursor.
    if (ColDetect::SquareToSquare(pObj->GetComponent<Transform>()->GetPosition(),
        pObj->GetComponent<Transform>()->GetScale(), 0,
        mouse->GetComponent<Transform>()->GetPosition(),
        mouse->GetComponent<Transform>()->GetScale(), 0))
    {
		//If the object is "sign"
		if (pObj->GetName() == "Sign")
		{
			//Show description about sign.
			descWindow->GetComponent<Transform>()->SetPosition(Vector(400, 90.f));
			descWindow->GetComponent<Sprite>()->SetColor(1.f, 1.f, 1.f, 1.f);
			descWindow->GetComponent<Transform>()->SetScale(Vector(300.f, 300.f));

			descScript->GetComponent<Transform>()->SetPosition(Vector(250.f, 150));
			descScript->GetComponent<Text>()->SetColor(1.f, 1.f, 0.f, 1.f);
			descScript->GetComponent<Text>()->SetFontSize(18); 
			descScript->GetComponent<Transform>()->SetScale(Vector(0.75f, 0.75f));
		}
		else
		{
			Vector mousePos = mouse->GetComponent<Transform>()->GetPosition();
			mousePos.x += 120.f;
			mousePos.y -= 40.f;

			descWindow->GetComponent<Transform>()->SetPosition(Vector(mousePos.x, mousePos.y, 0.9f));	
			descWindow->GetComponent<Sprite>()->SetColor(1.f, 1.f, 1.f, 1.f);

			mousePos.x -= 70.f;
			mousePos.y += 20.f;

			descScript->GetComponent<Transform>()->SetPosition(Vector(mousePos.x, mousePos.y, 1.f));
			descScript->GetComponent<Text>()->SetColor(1.f, 1.f, 0.f, 1.f);
			descScript->GetComponent<Text>()->SetFontSize(11);
			descScript->GetComponent<Transform>()->SetScale(Vector(0.5f, 0.5f));
		}

		//returns true if mouse cursor is intersecting with pObj
        return true;
    }
	//else, return false.
    else
        return false;

    return false;
}
/******************************************************************************/
/*!

\brief - Updating logic.

\param dt - Value for real-time calculation.

*/
/******************************************************************************/
void TutorialLogic::UpdateLogic(float dt)
{
	dt;
	//Value that determines alpha color of descriptions.
	static float c = 1.f;

	if (m_pSystem->GetGameData()->day == 1)
	{
		if (!dayCheck && !signCheck && !done)
		{
			static bool dayTrigger = false;
			descTxt[DAY]->GetComponent<Text>()->SetColor(c, 1.f, 1.f, c);

			if (c >= 1.f)
				dayTrigger = true;
			if (c <= 0.f)
				dayTrigger = false;

			if (dayTrigger)
				c -= 3 * dt;
			else
				c += 3 * dt;
		}
		
		if (dayCheck && !signCheck && !done)
		{
			//Boolean value to make the color change over and over.
			static bool trigger = false;

			//Alpha value transition.
			if (c >= 1.f)
				trigger = true;
			if (c <= 0.f)
				trigger = false;

			if (trigger)
				c -=  3 * dt;
			else
				c += 3 * dt;

			sign->GetComponent<Sprite>()->SetColor(1.f, 1.f, 1.f, c);
		}

		//If player touches sign, alpha value of properties are going
		//to be changed.
		if (dayCheck && signCheck && !done)
		{
			sign->GetComponent<Sprite>()->SetColor(1.f, 1.f, 1.f, 1.f);

			for (int i = 0; i < ITEMS - 1; ++i)
			{
				static bool eleTrigger = false;
				descTxt[i]->GetComponent<Text>()->SetColor(1.f, 1.f, 1.f, c);

				if (c >= 1.f)
					eleTrigger = true;
				if (c <= 0.f)
					eleTrigger = false;

				if (eleTrigger)
					c -= dt;
				else
					c += dt;
			}
		}

		//If player see all of the descriptions, properties should not
		//be twinkled.
		if (dayCheck && signCheck && done)
		{
			for (int i = 0; i < ITEMS - 1; ++i)
			{
				descTxt[i]->GetComponent<Text>()->SetColor(1.f, 1.f, 1.f, 1.f);
			}

			//In order to make this loop to activate only 
			//one time.
			signCheck = false;
		}
	}

	//Different descriptions for each properties.
	Vector mousePos = mouse->GetComponent<Transform>()->GetPosition();
    if (MouseEvent(sign))
    {
		if(dayCheck)
			signCheck = true;

		descScript->GetComponent<Text>()->SetInput("  Right Mouse Click\n\n    +    \n\n   Sheep   \n\n = Money");
    }
    else if (MouseEvent(desc[MONEY]))
    {
		if (signCheck)
			done = true;

		descWindow->GetComponent<Transform>()->SetScale(Vector(380.f, 100.f));
	
		mousePos.x += 230.f;
		mousePos.y -= 40.f;

		descWindow->GetComponent<Transform>()->SetPosition(mousePos);
        descScript->GetComponent<Text>()
			->SetInput("Money : You can buy sheep, upgrade,\n hire visitors. You can make money \n by shearing sheep");
    }
    else if (MouseEvent(desc[SHEEP]))
    {
		if (signCheck)
			done = true;

		descWindow->GetComponent<Transform>()->SetScale(Vector(200.f, 100.f));
		descScript->GetComponent<Text>()->SetInput("Number of sheep.\nMore sheep,\nmore money");
    }
    else if (MouseEvent(desc[FENCE]))
    {
		if (signCheck)
			done = true;
		
		descWindow->GetComponent<Transform>()->SetScale(Vector(200.f, 100.f));
		descScript->GetComponent<Text>()->SetInput("Level Of Fence.");
    }
    else if (MouseEvent(desc[FOOD]))
    {
		if (signCheck)
			done = true;

		descWindow->GetComponent<Transform>()->SetScale(Vector(380.f, 100.f));

		mousePos.x += 230.f;
		mousePos.y -= 40.f;

		descWindow->GetComponent<Transform>()->SetPosition(mousePos);
		descScript->GetComponent<Text>()->SetInput("Food / Food consumption per day\n\nYou have to take food to maintain\nvisitors");
    }
	else if (MouseEvent(desc[DAY]))
	{
		dayCheck = true;
		descTxt[DAY]->GetComponent<Text>()->SetColor(1.f, 1.f, 1.f, 1.f);

		mousePos.x += 200.f;
		mousePos.y -= 40.f;

		descWindow->GetComponent<Transform>()->SetPosition(mousePos);
		descWindow->GetComponent<Transform>()->SetScale(Vector(350.f, 100.f));
		descScript->GetComponent<Text>()->SetInput("Day %d/ Day 15\n\nYou have to save sheep\nuntil day 15.", m_pSystem->GetGameData()->day);
		descScript->GetComponent<Text>()->SetFontSize(9);

	}
	else
	{
		descWindow->GetComponent<Sprite>()->SetColor(1.f, 1.f, 1.f, 0.f);
		descScript->GetComponent<Text>()->SetColor(1.f, 1.f, 1.f, 0.f);
	}

}
/******************************************************************************/
/*!

\brief - It is actiavted when this logic shoould be shutdown.

*/
/******************************************************************************/
void TutorialLogic::ShutLogic(void)
{

}
/******************************************************************************/
/*!

\brief - Returns boolean value that specify has player done the tutorial.

\param result - Fmod_result data.

*/
/******************************************************************************/
bool TutorialLogic::getTutorialDone(void)
{
	return (done);
}
 
