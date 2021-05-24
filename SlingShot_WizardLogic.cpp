/******************************************************************************/
/*!
file    SlingShot_WizardLogic.cpp
\author woonggi.eun
\par    email: \@gmail.com
\date   2016-03-31
\description
Logic component for SlingShot_WizardLogic
All content (C) 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
 
#include "SlingShot_WizardLogic.h"
#include "../../../Engine/BaseEngine.h"
#include "../../../Engine/System/PhysicsSystem/Collision/CollisionEvent.h"
#include "../../../Game/GameLogic/SlingShotGame/MeteorLogic.h"
 

/******************************************************************************/
/*!

\brief - Logic builder constructor

*/
/******************************************************************************/
SlingShot_WizardLogicBuilder::SlingShot_WizardLogicBuilder()
	: ComponentBuilder()
{ }
 
/******************************************************************************/
/*!

\brief - Logic builder destructor

*/
/******************************************************************************/
SlingShot_WizardLogicBuilder::~SlingShot_WizardLogicBuilder()
{ }
 
/******************************************************************************/
/*!

\brief - Component building for logic

\param - Owner object of this logic

*/
/******************************************************************************/
Component* SlingShot_WizardLogicBuilder::create(ObjectPtr owner) const
{
  return new SlingShot_WizardLogic(owner); 
}
 
/******************************************************************************/
/*!

\brief - Constructor of logic

*/
/******************************************************************************/
SlingShot_WizardLogic::SlingShot_WizardLogic(ObjectPtr pObject)
    :Logic(pObject), m_timer(0.f)
{
  m_pSystem->AddLogic(this);
}
 
/******************************************************************************/
/*!

\brief - Destructor of logic

*/
/******************************************************************************/
SlingShot_WizardLogic::~SlingShot_WizardLogic(void)
{
  m_pSystem->DeleteLogic(this);
}
 
/******************************************************************************/
/*!

\brief - Load data for this logic in JSON file.

\return - return true if it is loaded succesfully. Otherwise, return false.

*/
/******************************************************************************/
bool SlingShot_WizardLogic::Load(const rapidjson::Value& data)
{
  UNREFERENCED_PARAMETER(data);
  return true;
}

/******************************************************************************/
/*!

\brief - Initial setting for this logic

*/
/******************************************************************************/
void SlingShot_WizardLogic::InitLogic(void)
{
 
}
 
/******************************************************************************/
/*!

\brief - Update logic.

\param - Value for time-based calculation.

*/
/******************************************************************************/
void SlingShot_WizardLogic::UpdateLogic(float dt)
{
    if (m_timer >= m_rate)
    {
        Vector randPos;

        //Should be random
        randPos.x = Random::GIRand().RandFloat(-15, 15);
        randPos.y = 30;
         
        SpawnMeteor(randPos);

        m_timer = 0;
    }

    m_timer += dt;
}
 
/******************************************************************************/
/*!

\brief - Function that activated when this logic shutdown.


*/
/******************************************************************************/
void SlingShot_WizardLogic::ShutLogic(void)
{
    
}

/******************************************************************************/
/*!

\brief - Spawn meteor at position with initial speed

\param - Spawn position.

*/
/******************************************************************************/
void SlingShot_WizardLogic::SpawnMeteor(const Vector& position)
{
    std::string name = "WizardMeteor";
    m_OBM->AddObject(new Object, name);

    Object* meteor = m_OBM->GetLastGameObjectList(name);
    meteor->AddComponent(new Transform(meteor));
    meteor->AddComponent(new Sprite(meteor, m_OBM->Assets()->GetTexture("Square")));
    meteor->AddComponent(new RigidBody(meteor));
    meteor->AddComponent(new EllipseCollider(meteor));
    meteor->AddComponent(new MeteorLogic(meteor));
    meteor->AddComponent(new Emitter(meteor));
    
    meteor->GetComponent<Transform>()->SetPosition(position);
    meteor->GetComponent<Transform>()->SetScale(Vector(3.f, 3.f));
    meteor->GetComponent<RigidBody>()->SetMass(3.f);
    meteor->GetComponent<RigidBody>()->SetMotionType(Motion::DYNAMICS);
    meteor->GetComponent<RigidBody>()->SetGravity(0.f);
    meteor->GetComponent<EllipseCollider>()->SetGhost(true);

    meteor->GetComponent<Emitter>()->SetSlowdown(50);
    meteor->GetComponent<Emitter>()->SetParticleMount(50);
    meteor->GetComponent<Emitter>()->SetTexture(m_OBM->Assets()->GetTexture("Square"));

    Vector target = { 3000, -3000, 0 };
    meteor->GetComponent<Emitter>()->SetWindEffect(13.5f, -target);

    meteor->GetComponent<RigidBody>()->AddForce(target);
    meteor->GetComponent<RigidBody>()->AddTorque(Vector(0, 0, 800.f));
	meteor->GetComponent<MeteorLogic>()->ForSlingshot(false);
}
/******************************************************************************/
/*!

\brief - Set Shooting rate of wizard.

\param - Shooting rate to set.

*/
/******************************************************************************/
void SlingShot_WizardLogic::SetRate(float rate)
{
    m_rate = rate;
} 