/******************************************************************************/
/*!
file    HTPLogic.h
\author Woonggi Eun
\par    email: dmsdndre\@gmail.com
\date   2016-06-06
\description
Logic component for HTPLogic
All content (C) 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
 
#ifndef HTPLogic_H
#define HTPLogic_H
 
#include "../../Engine/Component/Component.h"
#include "../../Engine/Component/Logic/Logic.h"
 
class HTPLogicBuilder : public ComponentBuilder
{
public:
	HTPLogicBuilder();
	~HTPLogicBuilder();
 
	virtual Component* create(ObjectPtr owner) const;
};
 
class LogicSystem;
 
class HTPLogic : public Logic
{
public:
  HTPLogic(ObjectPtr pObject);
  ~HTPLogic(void);
 
  void InitLogic(void);
  void UpdateLogic(float dt);
  void ShutLogic(void);

  bool MouseEvent(ObjectPtr pObj);

  void Descriptions(void);
  void FlickeringText(float dt);

  virtual bool Load(const rapidjson::Value& data);
 
protected:
    ObjectPtr mouse;

    ObjectPtr managementTxt;
    ObjectPtr slingshotTxt;
    ObjectPtr visitorTxt;

    ObjectPtr slingshot;
    ObjectPtr wizard;
    ObjectPtr sheep;
    ObjectPtr fence;
    ObjectPtr wolf;
    ObjectPtr werewolf;
    ObjectPtr armedWerewolf;

    ObjectPtr descWindow;
    ObjectPtr descTxt;

    bool managementTrigger;
    bool slingshotTrigger;
    bool visitorTrigger;
 
};
 
#endif //HTPLogic
 
