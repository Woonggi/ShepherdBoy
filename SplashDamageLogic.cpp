/******************************************************************************/
/*!
file    SplashDamageLogic.cpp
\author Woonggi Eun
\par    email: dmsdndre\@gmail.com
\date   2016-04-07
\description
Logic component for SplashDamageLogic
All content (C) 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
 
#include "SplashDamageLogic.h"
#include "../../../Engine/BaseEngine.h"
#include "../../../Engine/System/PhysicsSystem/Collision/CollisionEvent.h"
 
/******************************************************************************/
/*!

\brief - Logic builder constructor

*/
/******************************************************************************/
SplashDamageLogicBuilder::SplashDamageLogicBuilder()
	: ComponentBuilder()
{ }
 
/******************************************************************************/
/*!

\brief - Logic builder destructor

*/
/******************************************************************************/
SplashDamageLogicBuilder::~SplashDamageLogicBuilder()
{ }
 
/******************************************************************************/
/*!

\brief - Component building for logic

\param - Owner object of this logic

*/
/******************************************************************************/
Component* SplashDamageLogicBuilder::create(ObjectPtr owner) const
{
  return new SplashDamageLogic(owner);
}
 
/******************************************************************************/
/*!

\brief - Constructor of logic

*/
/******************************************************************************/
SplashDamageLogic::SplashDamageLogic(ObjectPtr pObject)
	:Logic(pObject), m_timer(0.3f), m_distance(1500)
{
    m_pSystem->AddLogic(this);
}
 
/******************************************************************************/
/*!

\brief - Destructor of logic

*/
/******************************************************************************/
SplashDamageLogic::~SplashDamageLogic(void)
{
    m_pSystem->DeleteLogic(this);
}

/******************************************************************************/
/*!


\brief - Load data for this logic in JSON file.

\return - return true if it is loaded succesfully. Otherwise, return false.

*/
/******************************************************************************/
bool SplashDamageLogic::Load(const rapidjson::Value& data)
{
    UNREFERENCED_PARAMETER(data);
    return true;
}
 
/******************************************************************************/
/*!

\brief - Initial setting for this logic

*/
/******************************************************************************/
void SplashDamageLogic::InitLogic(void)
{
 
}
 
/******************************************************************************/
/*!

\brief - Update logic.

\param - Value for time-based calculation.

*/
/******************************************************************************/
void SplashDamageLogic::UpdateLogic(float dt)
{
	m_timer -= dt;

    if (m_timer <= 0)
		Logic::DeleteOwner(true);
}
 
/******************************************************************************/
/*!

\brief - Function that activated when this logic shutdown.


*/
/******************************************************************************/
void SplashDamageLogic::ShutLogic(void)
{
 
}
 
/******************************************************************************/
/*!

\brief - Set distance to push enemies.

\param - Distance setting value.

*/
/******************************************************************************/
void SplashDamageLogic::SetPushingDistance(float distance)
{
	m_distance = distance;
}

/******************************************************************************/
/*!


\brief - Returns m_distance.

\return - Distance to push


*/
/******************************************************************************/
float SplashDamageLogic::GetPushingDistance(void) const
{
	return m_distance;
}