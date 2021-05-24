/******************************************************************************/
/*!
file    MeteorLogic.cpp
\author Woonggi Eun
\par    email: dmsdndre\@gmail.com
\date   2016-04-01
\description
Logic component for MeteorLogic
All content (C) 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "MeteorLogic.h"
#include "SplashDamageLogic.h"
#include "SlingShotStoneAction.h"
#include "../../../Engine/BaseEngine.h"
#include "../../../Engine/System/PhysicsSystem/Collision/CollisionEvent.h"

/******************************************************************************/
/*!

\brief - Logic builder constructor

*/
/******************************************************************************/
MeteorLogicBuilder::MeteorLogicBuilder()
	: ComponentBuilder()
{ }

/******************************************************************************/
/*!

\brief - Logic builder destructor

*/
/******************************************************************************/
MeteorLogicBuilder::~MeteorLogicBuilder()
{ }

/******************************************************************************/
/*!

\brief - Component building for logic

\param - Owner object of this logic

*/
/******************************************************************************/
Component* MeteorLogicBuilder::create(ObjectPtr owner) const
{
	return new MeteorLogic(owner);
}

/******************************************************************************/
/*!

\brief - Constructor of logic

*/
/******************************************************************************/
MeteorLogic::MeteorLogic(ObjectPtr pObject)
	:Logic(pObject), m_camVib(0.f), m_shake(0.f), m_destroy(false)
	, m_use(true)
{
	m_splashRange = { 10.f, 1.f, 0.f };
	m_pSystem->AddLogic(this);
}

/******************************************************************************/
/*!

\brief - Destructor of logic

*/
/******************************************************************************/
MeteorLogic::~MeteorLogic(void)
{
	m_pSystem->DeleteLogic(this);
}

/******************************************************************************/
/*!


\brief - Load data for this logic in JSON file.

\return - return true if it is loaded succesfully. Otherwise, return false.

*/
/******************************************************************************/
bool MeteorLogic::Load(const rapidjson::Value& data)
{
	UNREFERENCED_PARAMETER(data);
	return true;
}

/******************************************************************************/
/*!

\brief - Initial setting for this logic

*/
/******************************************************************************/
void MeteorLogic::InitLogic(void)
{

}

/******************************************************************************/
/*!

\brief - Update logic.

\param - Value for time-based calculation.

*/
/******************************************************************************/
void MeteorLogic::UpdateLogic(float dt)
{
	//Conduct camera vibration.
	CameraVibration(dt);

	//If meteor is not destroyed, 
	if (m_destroy == false)
	{
		//And collided with ground, 
		if (m_pOwner->GetCollisionInfo().OtherObject("Ground") == CollisionStarted)
		{
			//Set camera vibration value to 2.5.
			m_camVib = 2.5f;

            //m_OBM->GetSubsystem<SoundSystem>()->PlaySE("WizardExplosion");
            
			m_pOwner->DeleteComponent<Sprite>();
			m_pOwner->DeleteComponent<RigidBody>();
			m_pOwner->DeleteComponent<EllipseCollider>();

			if (!m_use) m_pOwner->DeleteComponent<Emitter>();

			m_destroy = true;

            m_OBM->GetSubsystem<SoundSystem>()->PlaySE("Slingshot_Explosive");

			SplashDamage(Vector(
				m_pOwner->GetComponent<Transform>()->GetPosition().x, 0));
		}
	}
}

/******************************************************************************/
/*!

\brief - Activate camera vibration according to m_camVib.

\param - Value for time-based calculation.

*/
/******************************************************************************/
void MeteorLogic::CameraVibration(float dt)
{
	//Get camera position.
	Camera cam = m_OBM->GetSubsystem<GraphicSystem>()->GetCameraPosition();
	//Set random number to shake the camera.
	m_shake = Random::GIRand().RandFloat(-m_camVib, m_camVib);

	//Change camera position caused by m_shake(random number).
	m_OBM->GetSubsystem<GraphicSystem>()->SetCamera(cam.X + m_shake, cam.Y + m_shake, cam.Z, 0);

	//Decrease m_camVib to return back to original position
	m_camVib -= 17 * dt;

	if (m_camVib <= 0)
	{
		m_camVib = 0.f;
	}
}

/******************************************************************************/
/*!

\brief - Made a object that applies splash damage.

\param - Position to spawn splash damage.

*/
/******************************************************************************/
void MeteorLogic::SplashDamage(Vector pos)
{
	m_OBM->AddObject(new Object, "MeteorDamage");
	ObjectPtr damage = m_OBM->GetLastGameObjectList("MeteorDamage");

	damage->AddComponent(new Transform(damage));
	damage->AddComponent(new Sprite(damage, m_OBM->Assets()->GetTexture("Square")));
	damage->AddComponent(new RigidBody(damage));
	damage->AddComponent(new RectangleCollider(damage));
	damage->AddComponent(new SplashDamageLogic(damage));

	damage->GetComponent<Sprite>()->SetColor(1.f, 1.f, 0.f, 1.f);
	damage->GetComponent<Transform>()->SetPosition(pos);
	damage->GetComponent<Transform>()->SetScale(m_splashRange);
	damage->GetComponent<RigidBody>()->SetMotionType(Motion::STATIC);
	damage->GetComponent<RectangleCollider>()->SetGhost(true);
	damage->GetComponent<SplashDamageLogic>()->SetPushingDistance(2000);

}

/******************************************************************************/
/*!

\brief - Function that activated when this logic shutdown.

*/
/******************************************************************************/
void MeteorLogic::ShutLogic(void)
{

}

/******************************************************************************/
/*!

\brief - Set m_use for slingshot.

\param - Boolean to set m_use.

*/
/******************************************************************************/
void MeteorLogic::ForSlingshot(bool toggle)
{
	m_use = toggle;
}

/******************************************************************************/
/*!

\brief - Return m_use.

\return - m_use.

*/
/******************************************************************************/
bool MeteorLogic::IsForSlingshot(void) const
{
	return m_use;
}
