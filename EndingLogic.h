/******************************************************************************/
/*!
file    EndingLogic.h
\author j.jeong
\par    email: p084111\@gmail.com
\date   2016-05-12
\description
Logic component for EndingLogic
All content (C) 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
 
#ifndef EndingLogic_H
#define EndingLogic_H
 
#include "../../Engine/Component/Component.h"
#include "../../Engine/Component/Logic/Logic.h"
 
class EndingLogicBuilder : public ComponentBuilder
{
public:
	EndingLogicBuilder();
	~EndingLogicBuilder();
 
	virtual Component* create(ObjectPtr owner) const;
};
 
class LogicSystem;
 
class EndingLogic : public Logic
{
public:
  EndingLogic(ObjectPtr pObject);
  ~EndingLogic(void);
 
  void InitLogic(void);
  void UpdateLogic(float dt);
  void ShutLogic(void);
  virtual bool Load(const rapidjson::Value& data);
 
  void LoseAnimation(float dt);
  void WinAnimation(float dt);

protected:
	float wolf_speed;
	float crow_speed;
	float opacity;
	bool  scene_changed;
	bool  goto_next;
};
 
#endif //EndingLogic
 
