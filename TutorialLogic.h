/******************************************************************************/
/*!
file    TutorialLogic.h
\author Woonggi Eun
\par    email: dmsdndre\@gmail.com
\date   2016-05-31
\description
Logic component for TutorialLogic
All content (C) 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
 
#ifndef TutorialLogic_H
#define TutorialLogic_H
 
#include "../../../Engine/Component/Component.h"
#include "../../../Engine/Component/Logic/Logic.h"
 
class TutorialLogicBuilder : public ComponentBuilder
{
public:
	TutorialLogicBuilder();
	~TutorialLogicBuilder();
 
	virtual Component* create(ObjectPtr owner) const;
};
 
class LogicSystem;
 
class TutorialLogic : public Logic
{
public:
  TutorialLogic(ObjectPtr pObject);
  ~TutorialLogic(void);
 
  void InitLogic(void);
  void UpdateLogic(float dt);
  void ShutLogic(void);

  void Sign(void);
  void Elements(void);
  bool MouseEvent(ObjectPtr pObj);
  bool getTutorialDone(void);

  virtual bool Load(const rapidjson::Value& data);
 
protected:
    ObjectPtr mouse;

    ObjectPtr sign;
    ObjectPtr* desc;
	ObjectPtr* descTxt;
    ObjectPtr descWindow;
    ObjectPtr descScript;

	bool dayCheck;
	bool signCheck;
	bool done;
 
};
 
#endif //TutorialLogic
 
