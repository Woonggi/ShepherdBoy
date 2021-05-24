/******************************************************************************/
/*!
\file   ObjectManager.h
\author jangseok.han, woonggi.eun
\par    email: dmsdndre\@gmail.com
\par    email: p084111\@gmail.com
\date   2015/9/7

\description
Object Manager for Control all objects in game.
All content (C) 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include <map>
#include <unordered_map>
#include "../GameObject/Object.h"
#include "../System/SubSystem.h"
#include "../GameObject/ObjectType.h"
//#include "Message\MessageDispatcher.h"

typedef std::unordered_multimap<std::string, ObjectPtr> ObjectMap;
typedef std::map<std::string, ObjectPtr>    ArchMap;
typedef std::map<std::string, SubSystemPtr> SystemMap;
class AssetManager;
class StateController;

class ObjectManager
{
public:
  ObjectManager(void);
  ~ObjectManager(void);
  void AddObject(ObjectPtr obj, const std::string& name);
  void RemoveObject(const std::string& name);
  void RemoveObject(ObjectPtr pObj);
  void RemoveAllObjectAsSameName(const std::string& name);
  void ClearObject(void);

  bool AddArchetype(ObjectPtr arch, const std::string& name);
  bool RemoveArchetype(const std::string& name);
  ObjectPtr CloneArchetype(const std::string& name, const std::string& objectName);

  ObjectPtr GetGameObject(const std::string& name) const;
  ObjectPtr GetLastGameObjectList(const std::string& name) const;
  ObjectList GetGameObjectList(const std::string& name) const;
  int GetObjectCount(const std::string& name) const;
  int GetObjectCount(void) const;

  void AddSubsystem(SubSystemPtr system);
  void ClearSubsystem(void);
  ObjectMap GetObjMap(void) const;
  SystemMap GetSysMap(void) const;
  bool HasObject(const std::string& name) const;
  bool HasObject(ObjectPtr pObject) const;

  void LoadSystem(void);
  void InitSystem(void);
  void UpdateSystem(float dt);
  void ShutdownSystem(void);
  void UnLoadSystem(void);

  void SetAssetManager(AssetManager* pAssets);
  void SetStateManager(StateController* pSM);
  AssetManager* Assets(void);
  StateController* StateManager();

  template <class T>
  T* GetSubsystem(void)
  {
    auto found = m_SubSystem.find(typeid(T).name());

    if (found != m_SubSystem.end())
      return static_cast<T*>(found->second);

    return nullptr;
  }

private:

  AssetManager* m_Assets;
  StateController* m_pSM;
  ObjectMap m_obj;
  ArchMap   m_archetypes;
  SystemMap m_SubSystem;
};



#endif