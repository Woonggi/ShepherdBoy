/******************************************************************************/
/*!
\file   ObjectManager.cpp
\author  jangseok.han, woonggi.eun
\par    email: dmsdndre\@gmail.com
\par    email: p084111\@gmail.com
\date   2015/9/15

\description
implementation of ObjectManager.
All content (C) 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "ObjectManager.h"
#include "../System/SubSystem.h"
#include "../Tool/Debug/Debug.h"


#include "../../Engine/System/GraphicSystem/GraphicSystem.h"
#include "../../Engine/System/LogicSystem/LogicSystem.h"
#include "../../Engine/System/PhysicsSystem/PhysicsSystem.h"
#include "../../Engine/System/SoundSystem/SoundSystem.h"
#include "../../Engine/Tool/Asset/AssetManager.h"
#include "../../Engine/System/StateSystem/StateController.h"


/******************************************************************************/
/*!
\brief - constructor of Object Manager

*/
/******************************************************************************/
ObjectManager::ObjectManager(void)
{
	AddSubsystem(new LogicSystem(this));
	AddSubsystem(new PhysicsSystem(this));
	AddSubsystem(new GraphicSystem(this));
	AddSubsystem(new SoundSystem(this));
}

/******************************************************************************/
/*!
\brief - destructor of Object Manager

*/
/******************************************************************************/
ObjectManager::~ObjectManager(void)
{
	ClearObject();
	ClearSubsystem();
}


/******************************************************************************/
/*!
\brief - Add a Object in Object Map.

\param obj - Object pointer

\param name - Object's name

*/
/******************************************************************************/
void ObjectManager::AddObject(ObjectPtr obj, const std::string& name)
{
	//Map key is like an index. <key(index), value>
	m_obj.insert(std::pair<std::string, Object*>(name, obj));
	obj->SetName(name);
	obj->SetManager(this);
}


/******************************************************************************/
/*!
\brief - remove Object in Object Map.

\param name - Object's name

*/
/******************************************************************************/
void ObjectManager::RemoveObject(const std::string& name)
{
	ObjectMap::iterator results = m_obj.find(name);

	if (results != m_obj.end())
	{
		results->second->RemoveAllComponent();
		delete results->second;
		m_obj.erase(results);
	}
}


/******************************************************************************/
/*!
\brief - remove Object in Object Map.

\param pObj - pointer to Object

*/
/******************************************************************************/

void ObjectManager::RemoveObject(ObjectPtr pObj)
{
	for (auto it = m_obj.begin(); it != m_obj.end(); ++it)
	{
		if (it->second == pObj)
		{
			it->second->RemoveAllComponent();
			delete it->second;
			m_obj.erase(it);
			break;
		}
	}


}

void ObjectManager::RemoveAllObjectAsSameName(const std::string& name)
{
	auto ret = m_obj.equal_range(name);

	for (auto it = ret.first; it != ret.second;)
	{
		it->second->RemoveAllComponent();
		delete it->second;
		m_obj.erase(it++);
	}
}


/******************************************************************************/
/*!
\brief - Get Game Object(first created) using name.

\param name - Object's name

\return ObjectPtr - pointer to Object

*/
/******************************************************************************/
ObjectPtr ObjectManager::GetGameObject(const std::string& name) const
{
	auto results = m_obj.lower_bound(name);

	if (results == m_obj.end())
	{
		return NULL;
	}

	return results->second;
}

/******************************************************************************/
/*!
\brief - Get Game Object(last created) using name.

\param name - Object's name

\return ObjectPtr - pointer to Object

*/
/******************************************************************************/
ObjectPtr ObjectManager::GetLastGameObjectList(const std::string& name) const
{
	ObjectList objList = GetGameObjectList(name);

	std::list<ObjectPtr>::reverse_iterator result = objList.rbegin();

	return *result;
}

/******************************************************************************/
/*!
\brief -  Get Game Object List using name.

\param name - Object's name

\return ObjectList - List of Pointer to Object

*/
/******************************************************************************/
ObjectList ObjectManager::GetGameObjectList(const std::string& name) const
{
	ObjectList result;

	auto ret = m_obj.equal_range(name);

	for (auto it = ret.first; it != ret.second; ++it)
	{
		result.push_back(it->second);
	}

	return result;
}

/******************************************************************************/
/*!
\brief - get number of object using same name.

\param name - Object's name

\return int - number of object using same name.

*/
/******************************************************************************/
int ObjectManager::GetObjectCount(const std::string& name) const
{
	return m_obj.count(name);
}


/******************************************************************************/
/*!
\brief - get number of object

\return int - number of object

*/
/******************************************************************************/
int ObjectManager::GetObjectCount(void) const
{
	return m_obj.size();
}


/******************************************************************************/
/*!
\brief - clear the all object in the map

*/
/******************************************************************************/
void ObjectManager::ClearObject(void)
{
	ObjectMap::iterator begin = m_obj.begin();
	ObjectMap::iterator end = m_obj.end();

	for (ObjectMap::iterator it = begin; it != end; ++it)
	{
		it->second->RemoveAllComponent();
		delete it->second;
	}
	m_obj.clear();
}


/******************************************************************************/
/*!
\brief - Add a Subsystem into a Object Manager.

\param system - subsystem pointer

*/
/******************************************************************************/
void ObjectManager::AddSubsystem(SubSystemPtr system)
{
	std::string systemName = typeid(*system).name();

	SystemMap::iterator found = m_SubSystem.find(systemName);

	DEBUG_ASSERT(found == m_SubSystem.end(),
		"Error: Trying to duplicate same System! \n");

	m_SubSystem[systemName] = system;

}


/******************************************************************************/
/*!
\brief - Clear the Subsystem

*/
/******************************************************************************/
void ObjectManager::ClearSubsystem(void)
{

	SystemMap::iterator begin = m_SubSystem.begin();
	SystemMap::iterator end = m_SubSystem.end();

	for (SystemMap::iterator it = begin; it != end; ++it)
	{
		it->second->Shutdown();
		delete it->second;
	}
	m_SubSystem.clear();
}

/******************************************************************************/
/*!
\brief - Get Object Map from manager

\return ObjectMap

*/
/******************************************************************************/
ObjectMap ObjectManager::GetObjMap(void) const
{
	return m_obj;
}

/******************************************************************************/
/*!
\brief - Get Subsystem Map from manager

\return SystemMap

*/
/******************************************************************************/
SystemMap ObjectManager::GetSysMap(void) const
{
	return m_SubSystem;
}

/******************************************************************************/
/*!
\brief - flag of object.

\param name - Object's name

\return boolean

*/
/******************************************************************************/
bool ObjectManager::HasObject(const std::string& name)const
{
	auto results = m_obj.find(name);

	if (results != m_obj.end())
	{
		return true;
	}

	return false;
}

bool ObjectManager::HasObject(ObjectPtr pObject) const
{
	for (auto it = m_obj.begin(); it != m_obj.end(); ++it)
	{
		if (it->second == pObject)
			return true;
	}

	return false;
}


/******************************************************************************/
/*!
\brief - load System

*/
/******************************************************************************/
void ObjectManager::LoadSystem(void)
{

}


/******************************************************************************/
/*!
\brief - Initialize System

*/
/******************************************************************************/
void ObjectManager::InitSystem(void)
{
	SystemMap::iterator begin = m_SubSystem.begin();
	SystemMap::iterator end = m_SubSystem.end();

	for (SystemMap::iterator it = begin; it != end; ++it)
	{
		it->second->Initialize();
	}
}


/******************************************************************************/
/*!
\brief - update System

\param dt - time interval of engine.

*/
/******************************************************************************/
void ObjectManager::UpdateSystem(float dt)
{
	GetSubsystem<LogicSystem>()->Update(dt);
	GetSubsystem<PhysicsSystem>()->Update(dt);
	GetSubsystem<GraphicSystem>()->Update(dt);
	GetSubsystem<SoundSystem>()->Update(dt);
}


/******************************************************************************/
/*!
\brief - shut down system.

*/
/******************************************************************************/
void ObjectManager::ShutdownSystem(void)
{
	SystemMap::iterator begin = m_SubSystem.begin();
	SystemMap::iterator end = m_SubSystem.end();

	for (SystemMap::iterator it = begin; it != end; ++it)
	{
		it->second->Shutdown();
	}
}


/******************************************************************************/
/*!
\brief - Unload data from system.

*/
/******************************************************************************/
void ObjectManager::UnLoadSystem(void)
{
	SystemMap::iterator begin = m_SubSystem.begin();
	SystemMap::iterator end = m_SubSystem.end();

	for (SystemMap::iterator it = begin; it != end; ++it)
	{
		delete it->second;
	}
	m_SubSystem.clear();
}


/******************************************************************************/
/*!
\brief - set asset manager from engine.

\param pAssets - pointer to AssetManager

*/
/******************************************************************************/
void ObjectManager::SetAssetManager(AssetManager* pAssets)
{
	m_Assets = pAssets;
}


/******************************************************************************/
/*!
\brief - get Asset manager from manager.

\return AssetManager* - pointer to Asset Manager

*/
/******************************************************************************/
AssetManager* ObjectManager::Assets(void)
{
	return m_Assets;
}


/******************************************************************************/
/*!
\brief - set state manager from engine.

\param pSM - pointer to StateController

*/
/******************************************************************************/
void ObjectManager::SetStateManager(StateController* pSM)
{
	m_pSM = pSM;
}


/******************************************************************************/
/*!
\brief - get State manager from manager.

\return StateController* - pointer to StateController

*/
/******************************************************************************/
StateController* ObjectManager::StateManager()
{
	return m_pSM;
}

/******************************************************************************/
/*!
\brief - add an archetype to the list

\return bool - success or failure

*/
/******************************************************************************/
bool ObjectManager::AddArchetype(ObjectPtr arch, const std::string& name)
{
    if (m_archetypes.find(name) == m_archetypes.end())
    {
        m_archetypes[name] = arch;
        return true;
    }

    return false;
}

/******************************************************************************/
/*!
\brief - remove an archetype from the list

\return bool - success or failure

*/
/******************************************************************************/
bool ObjectManager::RemoveArchetype(const std::string& name)
{
    if (m_archetypes.find(name) != m_archetypes.end())
    {
        m_archetypes.erase(name);
        return true;
    }

    return false;
}

/******************************************************************************/
/*!
\brief - clone Archetype and adds it to object list

\return OjectPtr - pointer to Object

*/
/******************************************************************************/
ObjectPtr ObjectManager::CloneArchetype(const std::string& name, const std::string& objectName)
{
	objectName;
    return m_archetypes[name]->Clone();
}