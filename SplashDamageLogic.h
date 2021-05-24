/******************************************************************************/
/*!
file    SplashDamageLogic.h
\author Woonggi Eun
\par    email: dmsdndre\@gmail.com
\date   2016-04-07
\description
Logic component for SplashDamageLogic
All content (C) 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
 
#ifndef SplashDamageLogic_H
#define SplashDamageLogic_H
 
#include "../../../Engine/Component/Component.h"
#include "../../../Engine/Component/Logic/Logic.h"
 
class SplashDamageLogicBuilder : public ComponentBuilder
{
public:
	SplashDamageLogicBuilder();
	~SplashDamageLogicBuilder();
 
	virtual Component* create(ObjectPtr owner) const;
};
 
class LogicSystem;
 
class SplashDamageLogic : public Logic
{
public:
  SplashDamageLogic(ObjectPtr pObject);
  ~SplashDamageLogic(void);
 
  void InitLogic(void);
  void UpdateLogic(float dt);
  void ShutLogic(void);
  virtual bool Load(const rapidjson::Value& data);

	void SetPushingDistance(float distance);
	float GetPushingDistance(void) const;
 
protected:
	float m_timer;
	float m_distance;
 
};
 
#endif //SplashDamageLogic
 
