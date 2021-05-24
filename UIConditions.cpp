/******************************************************************************/
/*!
file    UIConditions.cpp
\author Woonggi Eun
\par    email: dmsdndre\@gmail.com
\date   2016-04-28
\description
Logic component for UIConditions
All content (C) 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
 
#include "UIConditions.h"
#include "../SlingshotManager.h"
#include "../../../../Engine/BaseEngine.h"
#include "../../../../Engine/System/PhysicsSystem/Collision/CollisionEvent.h"
 
//To avoid magic numbers.
namespace
{
	//Max number of UI in a same row.
	const int MAX_ROW    = 7;
	//Scale of each UI.
	const float UI_SCALE = 50.f;
	//Position gaps for each UI.
	const float UI_GAP   = UI_SCALE + 5.f;
	//Max UI that is shown to player. 
	//If number of UI is greater than this,
	//UI pictures turns into numbers.
    const int UI_MAX     = 15;
}

/******************************************************************************/
/*!

\brief - Logic builder constructor

*/
/******************************************************************************/
UIConditionsBuilder::UIConditionsBuilder()
	: ComponentBuilder()
{ }
 
/******************************************************************************/
/*!

\brief - Logic builder destructor

*/
/******************************************************************************/
UIConditionsBuilder::~UIConditionsBuilder()
{ }
 
/******************************************************************************/
/*!

\brief - Component building for logic

\param - Owner object of this logic

*/
/******************************************************************************/
Component* UIConditionsBuilder::create(ObjectPtr owner) const
{
  return new UIConditions(owner);
}

/******************************************************************************/
/*!

\brief - Constructor of logic

*/
/******************************************************************************/
UIConditions::UIConditions(ObjectPtr pObject)
  :Logic(pObject)
{
    m_pSystem->AddLogic(this);
}
 
/******************************************************************************/
/*!

\brief - Destructor of logic

*/
/******************************************************************************/
UIConditions::~UIConditions(void)
{
    m_pSystem->DeleteLogic(this);
}
 
/******************************************************************************/
/*!


\brief - Load data for this logic in JSON file.

\return - return true if it is loaded succesfully. Otherwise, return false.

*/
/******************************************************************************/
bool UIConditions::Load(const rapidjson::Value& data)
{
  UNREFERENCED_PARAMETER(data);
  return true;
}
 
/******************************************************************************/
/*!

\brief - Initial setting for this logic

*/
/******************************************************************************/
void UIConditions::InitLogic(void)
{
    //Draw  to UI window. 
    m_wolfList    = m_OBM->GetGameObject("Wolf_Manager")->GetComponent<SlingshotManager>()->WolfList();
	m_winLoseText = m_OBM->GetGameObject("WinLoseText");

    m_numberOfWolves = m_OBM->GetGameObject("Wolf_Manager")->GetComponent<SlingshotManager>()->getMaxWolfCount();
	m_numberOfSheep	   = m_pSystem->GetGameData()->sheep;
	m_xMarkTexture     = m_OBM->Assets()->GetTexture("UI_XMark");

    m_sheepStartingPos = {-450.f, -170.f};
    m_wolfStartingPos  = {175.f, -170.f};

    m_wolfHead = m_OBM->GetGameObject("UI_WolfHead");
    m_sheepHead = m_OBM->GetGameObject("UI_SheepHead");
    m_wolfX15 = m_OBM->GetGameObject("UI_WolfX15");
    m_sheepX15 = m_OBM->GetGameObject("UI_SheepX15");

    oneTime_sheep = false;
    oneTime_wolf = false;

    m_wolfIndex = 0;
    m_sheepIndex = 0;

    UI_Greater15();

    UI_ShowSheepRemain();
    UI_ShowWolfRemain();

}
 
/******************************************************************************/
/*!

\brief - Update logic.

\param - Value for time-based calculation.

*/
/******************************************************************************/
void UIConditions::UpdateLogic(float dt)
{
    dt;

    if (InputManager::GIInput().IsKeyTriggered(K_C))
    {
        m_pSystem->GetGameData()->sheep--;
    }


	UI_Transparency();

	UI_ShowXMark();
	UI_Win();
	UI_Lose();
}
 
/******************************************************************************/
/*!

\brief - Function that activated when this logic shutdown.


*/
/******************************************************************************/
void UIConditions::ShutLogic(void)
{
 
}
    
/******************************************************************************/
/*!

\brief - Deal with transparency of UI. If number of sheep is greater than
         UI_MAX, it is transparent otherwise, not tranparent.

*/
/******************************************************************************/
void UIConditions::UI_Transparency(void)
{
    if (m_numberOfSheep < UI_MAX && !oneTime_sheep)
    {
        for (int i = 0; i < UI_MAX-1; ++i)
        {
            if (i >= m_numberOfSheep)
                break;

            m_sheepUiList[i]->GetComponent<Sprite>()->SetColor(1.f, 1.f, 1.f, 1.f);
        }
        
        m_sheepHead->GetComponent<Sprite>()->SetColor(1.f, 1.f, 1.f, 0.f);
        m_sheepX15->GetComponent<Text>()->SetColor(1.f, 1.f, 1.f, 0.f);

        oneTime_sheep = true;
    }

    if (m_numberOfWolves < UI_MAX && !oneTime_wolf)
    {
        for (int i = 0; i < UI_MAX-1; ++i)
        { 
            if (i >= m_numberOfWolves)
                break;

            m_wolvesUiList[i]->GetComponent<Sprite>()->SetColor(1.f, 1.f, 1.f, 1.f);
        }

        m_wolfHead->GetComponent<Sprite>()->SetColor(1.f, 1.f, 1.f, 0.f);
        m_wolfX15->GetComponent<Text>()->SetColor(1.f, 1.f, 1.f, 0.f);

        oneTime_wolf = true;
    }
}

/******************************************************************************/
/*!

\brief - When Initial number of sheep or wolf is grater than UI_MAX, set UI
         as a number.

*/
/******************************************************************************/
void UIConditions::UI_Greater15(void)
{
    if (m_numberOfSheep >= UI_MAX)
    {
        //Add head of sheep
        m_sheepHead->GetComponent<Sprite>()->SetColor(1.f, 1.f, 1.f, 1.f);

        m_sheepX15 = m_OBM->GetGameObject("UI_SheepX15");
        m_sheepX15->GetComponent<Text>()->SetColor(1.f, 1.f, 1.f, 1.f);
        m_sheepX15->GetComponent<Text>()->SetInput("X %d", m_numberOfSheep);
    }

    if (m_numberOfWolves >= UI_MAX)
    {
        //Add head of wolves
        m_wolfHead->GetComponent<Sprite>()->SetColor(1.f, 1.f, 1.f, 1.f);

        m_wolfX15 = m_OBM->GetGameObject("UI_WolfX15");
        m_wolfX15->GetComponent<Text>()->SetColor(1.f, 1.f, 1.f, 1.f);
        m_wolfX15->GetComponent<Text>()->SetInput("X %d", m_numberOfWolves);
    }
}

/******************************************************************************/
/*!

\brief - Show how much sheep is remaining.

*/
/******************************************************************************/
void UIConditions::UI_ShowSheepRemain(void)
{
    Vector startPos = m_sheepStartingPos;

    //Draw sheep in script window
    for (int i = 1; i <= m_numberOfSheep; ++i)
    {		
        if (i >= UI_MAX)
            break;

        m_OBM->AddObject(new Object, "UI_Sheep");	

        ObjectPtr sheepUI = m_OBM->GetLastGameObjectList("UI_Sheep");

        sheepUI->AddComponent(new Transform(sheepUI));
        sheepUI->AddComponent(new Sprite(sheepUI, m_OBM->Assets()->GetTexture("UI_Sheep")));

        sheepUI->GetComponent<Transform>()->SetPosition(startPos);
        sheepUI->GetComponent<Transform>()->SetScale(Vector(UI_SCALE, UI_SCALE));
        
        if (m_numberOfSheep < UI_MAX)
            sheepUI->GetComponent<Sprite>()->SetColor(1.f, 1.f, 1.f, 1.f);
        else
            sheepUI->GetComponent<Sprite>()->SetColor(1.f, 1.f, 1.f, 0.f);

        sheepUI->GetComponent<Sprite>()->SetOrtho(true);        

		m_sheepUiList.push_back(sheepUI);

        startPos.x += UI_GAP;
			
        if (i % MAX_ROW == 0)
        {
            startPos.x = m_sheepStartingPos.x;
            startPos.y = m_sheepStartingPos.y - UI_GAP;
        }
    }
}

/******************************************************************************/
/*!

\brief - Show how many wolves are remaining.

*/
/******************************************************************************/
void UIConditions::UI_ShowWolfRemain(void)
{
    //Draw wolves in script window
    Vector startPos = m_wolfStartingPos;

    for ( int i = 1; i <= m_numberOfWolves; ++i)
    {		
        if (i >= UI_MAX)
			break;

        m_OBM->AddObject(new Object, "UI_Wolf");
        ObjectPtr wolfUI = m_OBM->GetLastGameObjectList("UI_Wolf");

        wolfUI->AddComponent(new Transform(wolfUI));
        wolfUI->AddComponent(new Sprite(wolfUI, m_OBM->Assets()->GetTexture("UI_Wolf")));

        wolfUI->GetComponent<Transform>()->SetPosition(startPos);
        wolfUI->GetComponent<Transform>()->SetScale(Vector(UI_SCALE, UI_SCALE));
    
        if (m_numberOfWolves < UI_MAX)
            wolfUI->GetComponent<Sprite>()->SetColor(1.f, 1.f, 1.f, 1.f);
        else
            wolfUI->GetComponent<Sprite>()->SetColor(1.f, 1.f, 1.f, 0.f);

        wolfUI->GetComponent<Sprite>()->SetOrtho(true);

		m_wolvesUiList.push_back(wolfUI);

        startPos.x += UI_GAP;

        if (i % MAX_ROW == 0)
        {
            startPos.x = m_wolfStartingPos.x;
            startPos.y = m_wolfStartingPos.y - UI_GAP;
        }
    }
}

/******************************************************************************/
/*!

\brief - Show number of UI as a number.

*/
/******************************************************************************/
void UIConditions::UI_ShowXMark(void)
{

    if (m_pSystem->GetGameData()->sheep != m_numberOfSheep)
    {   
        if (m_numberOfSheep >= UI_MAX)
        {
            m_sheepX15->GetComponent<Text>()->SetInput("X %d", m_numberOfSheep);
        }

        else if (m_numberOfSheep < UI_MAX && m_numberOfSheep > 0)
        {
            m_sheepUiList[m_sheepIndex]->GetComponent<Sprite>()->SetColor(1.f, 1.f, 1.f, 0.3f);
            ++m_sheepIndex;
        }

        m_numberOfSheep--;

    }

    m_wolvesUpdate = m_OBM->GetGameObject("Wolf_Manager")->GetComponent<SlingshotManager>()->getWolvesRemaining();

    if (m_wolvesUpdate != m_numberOfWolves && m_numberOfWolves > 0)
    {
        if (m_numberOfWolves >= UI_MAX)
        {
            m_wolfX15->GetComponent<Text>()->SetInput("X %d", m_wolvesUpdate);
        }

        else if (m_numberOfWolves < UI_MAX && m_numberOfWolves > 0)
        {
            m_wolvesUiList[m_wolfIndex]->GetComponent<Sprite>()->SetColor(1.f, 1.f, 1.f, 0.3f);
            ++m_wolfIndex;
        }

        m_numberOfWolves--;
    }
}

/******************************************************************************/
/*!

\brief - Show you win text on the UI.

*/
/******************************************************************************/
void UIConditions::UI_Win(void)
{
	//If number of wolves is 0, Erase wolves' UI and print You win.
	// 150, -240
	if (m_wolvesUpdate == 0)
	{
		m_winLoseText->GetComponent<Text>()->SetColor(1.f, 1.f, 1.f, 1.f);
		m_winLoseText->GetComponent<Transform>()->SetPosition(Vector(150, -240));
		m_winLoseText->GetComponent<Text>()->SetInput("You Win");
	}
}

/******************************************************************************/
/*!

\brief - Show you lose text on the UI.

*/
/******************************************************************************/
void UIConditions::UI_Lose(void)
{
	//Sheep == 0
	//-450, -240
	if (m_pSystem->GetGameData()->sheep == 0)
	{
		m_winLoseText->GetComponent<Text>()->SetColor(1.f, 1.f, 1.f, 1.f);
		m_winLoseText->GetComponent<Transform>()->SetPosition(Vector(-450, -240));
		m_winLoseText->GetComponent<Text>()->SetInput("You Lose");
	}
}


