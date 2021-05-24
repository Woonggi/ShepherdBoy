/******************************************************************************/
/*!
file    UIConditions.h
\author Woonggi Eun
\par    email: dmsdndre\@gmail.com
\date   2016-04-28
\description
Logic component for UIConditions
All content (C) 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
 
#ifndef UIConditions_H
#define UIConditions_H
 
#include "../../../../Engine/Component/Component.h"
#include "../../../../Engine/Component/Logic/Logic.h"
#include "../../../../Engine/Tool/Math/Math.h"
#include <vector>
 
class Texture;

class UIConditionsBuilder : public ComponentBuilder
{
public:
	UIConditionsBuilder();
	~UIConditionsBuilder();
 
	virtual Component* create(ObjectPtr owner) const;
};
 
class LogicSystem;
 
class UIConditions : public Logic
{
public:
  UIConditions(ObjectPtr pObject);
  ~UIConditions(void);
 
  void InitLogic(void);
  void UpdateLogic(float dt);
  void ShutLogic(void);
  virtual bool Load(const rapidjson::Value& data);

  void UI_Greater15(void);

  void UI_Transparency(void);

  void UI_ShowSheepRemain(void);
  void UI_ShowWolfRemain(void);
  void UI_ShowXMark(void);

  void UI_Win(void);
  void UI_Lose(void);
 
protected:
    ObjectList m_wolfList;
    Vector m_wolfStartingPos;
    Vector m_sheepStartingPos;

    int m_numberOfSheep;
    int m_numberOfWolves; 
	int m_wolvesUpdate;

    bool oneTime_sheep;
    bool oneTime_wolf;

	Texture* m_xMarkTexture;

	std::vector<ObjectPtr> m_sheepUiList;
	std::vector<ObjectPtr> m_wolvesUiList;

	std::list<ObjectPtr>::iterator m_sheepUiIt;
	std::list<ObjectPtr>::iterator m_wolvesUiIt;

	ObjectPtr m_winLoseText;

    ObjectPtr m_sheepHead;
    ObjectPtr m_sheepX15;

    ObjectPtr m_wolfHead;
    ObjectPtr m_wolfX15;

    int m_sheepIndex;
    int m_wolfIndex;
};
 
#endif //UIConditions
 
