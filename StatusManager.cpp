/******************************************************************************/
/*!
file    StatusManager.cpp
\author Woonggi Eun
\par    email: dmsdndre\@gmail.com
\date   2016-03-08
\description
Logic component for StatusManager
All content (C) 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
 
#include "StatusManager.h"
#include "../../../Engine/BaseEngine.h"
#include "../../../Game/GameLogic/Menu/TypingAnimation.h"
#include "../../../Engine/System/PhysicsSystem/Collision/CollisionEvent.h"

 
StatusManagerBuilder::StatusManagerBuilder()
	: ComponentBuilder()
{ }
 
StatusManagerBuilder::~StatusManagerBuilder()
{ }
 
Component* StatusManagerBuilder::create(ObjectPtr owner) const
{
  return new StatusManager(owner);
}
 
StatusManager::StatusManager(ObjectPtr pObject)
    :Logic(pObject), m_print(false), m_scriptTimer(1.5f)
    , m_timerDup(m_scriptTimer)
{ 
  m_pSystem->AddLogic(this); 
}
 
StatusManager::~StatusManager(void)
{
  m_pSystem->DeleteLogic(this);
}
 
bool StatusManager::Load(const rapidjson::Value& data)
{
  UNREFERENCED_PARAMETER(data);
  return true;
}
 
void StatusManager::InitLogic(void)
{
    m_gameData = m_pSystem->GetGameData();

    initialSheepLimit = m_gameData->sheepLimit;
    script = m_OBM->GetGameObject("Script");

}
 
void StatusManager::UpdateLogic(float dt)
{
    if (m_gameData->money <= 0)
        m_gameData->money = 0;

	Keys();
	TextUpdater();

    UNREFERENCED_PARAMETER(dt);
}
 
void StatusManager::ShutLogic(void)
{
 
}

/******************************************************************************/
/*!

\brief - This function activate keys for buy stuff.

NONE.
*/
/******************************************************************************/
//Helper Functions.
void StatusManager::Keys(void)
{
    //Cheat
    if (InputManager::GIInput().IsKeyPressed(K_M))
    {
        m_gameData->money += 100;
    }

}
/******************************************************************************/
/*!

\brief - This function updates texts contains status info.

NONE.
*/
/******************************************************************************/
void StatusManager::TextUpdater(void)
{
	PrintTextsBtLocal();
}

/******************************************************************************/
/*!

\brief - This function activate keys for buy stuff.

NONE.
*/
/******************************************************************************/
void StatusManager::PrintTextsBtLocal(void)
{
	m_OBM->GetGameObject("SheepText")->GetComponent<Text>()->SetInput("Sheep : %i / %i", m_gameData->sheep, m_gameData->sheepLimit);
	m_OBM->GetGameObject("FenceText")->GetComponent<Text>()->SetInput("Fence : Level %i", m_gameData->fenceLevel);
	m_OBM->GetGameObject("MoneyText")->GetComponent<Text>()->SetInput("Money : %i", m_gameData->money);

	m_OBM->GetGameObject("FoodText")->GetComponent<Text>()->SetInput("Food : %i", m_gameData->food);
	m_OBM->GetGameObject("FoodConsumeText")->GetComponent<Text>()->SetInput("%i Food / Day", m_gameData->foodConsumption);
	m_OBM->GetGameObject("DayText")->GetComponent<Text>()->SetInput("Day %i / 15", m_gameData->day);
	
}
