/******************************************************************************/
/*!
file    StatusManager.h
\author Woonggi Eun
\par    email: dmsdndre\@gmail.com
\date   2016-03-08
\description
Logic component for StatusManager
All content (C) 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
 
#ifndef StatusManager_H
#define StatusManager_H
 
#include "../../../Engine/Component/Component.h"
#include "../../../Engine/Component/Logic/Logic.h"
 
typedef enum Buy{sheep, fence_upgrade, farm_upgrade, food};

class StatusManagerBuilder : public ComponentBuilder
{
public:
	StatusManagerBuilder();
	~StatusManagerBuilder();
 
	virtual Component* create(ObjectPtr owner) const;
};
 
class LogicSystem;
struct GameData;

class StatusManager : public Logic
{
public:
  StatusManager(ObjectPtr pObject);
  ~StatusManager(void);
 
  void InitLogic(void);
  void UpdateLogic(float dt);
  void ShutLogic(void);

  virtual bool Load(const rapidjson::Value& data);

  void Keys(void);
  void TextUpdater(void);
  
  void SetGameData(GameData* gameData) { m_gameData = gameData; }
  void PrintTextsBtLocal(void);

  void Sign(void);
 
protected: 
    GameData* m_gameData;
    ObjectPtr script;
    unsigned int initialSheepLimit;
    bool m_print;

    float m_scriptTimer;
    float m_timerDup;

};
 
#endif //StatusManager
 
