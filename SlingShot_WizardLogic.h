/******************************************************************************/
/*!
file    SlingShot_WizardLogic.h
\author 
\par    email: \@gmail.com
\date   2016-03-31
\description
Logic component for SlingShot_WizardLogic
All content (C) 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
 
#ifndef SlingShot_WizardLogic_H
#define SlingShot_WizardLogic_H
 
#include "../../../Engine/Component/Component.h"
#include "../../../Engine/Component/Logic/Logic.h"

struct Vector;

class SlingShot_WizardLogicBuilder : public ComponentBuilder
{
public:
	SlingShot_WizardLogicBuilder();
	~SlingShot_WizardLogicBuilder();
 
	virtual Component* create(ObjectPtr owner) const;
};
 
class LogicSystem;
 
class SlingShot_WizardLogic : public Logic
{
public:
  SlingShot_WizardLogic(ObjectPtr pObject);
  ~SlingShot_WizardLogic(void);
 
  void InitLogic(void);
  void UpdateLogic(float dt);
  void ShutLogic(void);
  virtual bool Load(const rapidjson::Value& data);

  void SpawnMeteor(const Vector& position);
 
  void SetRate(float rate);
 
protected:
    float		 m_rate;
    float		 m_timer;
	
};
 
#endif //SlingShot_WizardLogic
 
