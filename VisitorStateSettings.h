/******************************************************************************/
/*!
file    VisitorStateSettings.h
\author Woonggi Eun
\par    email: dmsdndre\@gmail.com
\date   2016-03-14
\description
Logic component for VisitorStateSettings
All content (C) 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef VisitorStateSettings_H
#define VisitorStateSettings_H

#include "../../../Engine/Component/Component.h"
#include "../../../Engine/Component/Logic/Logic.h"

typedef enum Visitor { dog = -5, compu, pigs, thief, flower, hunter, wizard, knight };

class VisitorStateSettingsBuilder : public ComponentBuilder
{
public:
	VisitorStateSettingsBuilder();
	~VisitorStateSettingsBuilder();

	virtual Component* create(ObjectPtr owner) const;
};

class LogicSystem;
struct GameData;

class VisitorStateSettings : public Logic
{
public:
	VisitorStateSettings(ObjectPtr pObject);
	~VisitorStateSettings(void);

	void InitLogic(void);
	void UpdateLogic(float dt);
	void ShutLogic(void);

	void InitialVisitorSet(Visitor v);
	void HireVisitors(Visitor v);

	void AfterHired(float dt);
	void KeyboardControl(void);
	void MouseControl(const std::string& name);

	virtual bool Load(const rapidjson::Value& data);

protected:
	ObjectPtr script;
	ObjectPtr visitor;
	Visitor visitorType;
	GameData* m_gameData;

	float m_timer;
	float m_timerDup;

	bool m_hired;
	bool m_choice;
	bool m_upDown;
	bool m_beforeHire;
	float m_textPostion, m_textScale;

};

#endif //VisitorStateSettings

