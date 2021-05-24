/******************************************************************************/
/*!
file    MeteorLogic.h
\author Woonggi Eun
\par    email: dmsdndre\@gmail.com
\date   2016-04-01
\description
Logic component for MeteorLogic
All content (C) 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef MeteorLogic_H
#define MeteorLogic_H

#include "../../../Engine/Component/Component.h"
#include "../../../Engine/Component/Logic/Logic.h"
#include "../../../Engine/Tool/Math/Math.h"

class MeteorLogicBuilder : public ComponentBuilder
{
public:
	MeteorLogicBuilder();
	~MeteorLogicBuilder();

	virtual Component* create(ObjectPtr owner) const;
};

class LogicSystem;

class MeteorLogic : public Logic
{
public:
	MeteorLogic(ObjectPtr pObject);
	~MeteorLogic(void);

	void InitLogic(void);
	void UpdateLogic(float dt);
	void ShutLogic(void);
	virtual bool Load(const rapidjson::Value& data);

	void CameraVibration(float dt);
	void SplashDamage(Vector pos);

	void ForSlingshot(bool toggle);
	bool IsForSlingshot(void) const;

protected:
	float m_camVib;
	float m_shake;

	Vector m_splashPos;
	Vector m_splashRange;

	bool m_destroy;
	bool m_use;

};

#endif //MeteorLogic

