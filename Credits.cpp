/******************************************************************************/
/*!
file    Credits.cpp
\author jeong.juyong
\par    email: jeykop14\@gmail.com
\date   2016-01-18
\description
implementation of CreditsState in state class.
All content (C) 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
 
#include "Credits.h"
#include "../../../Engine/BaseEngine.h"
#include <fstream>
/******************************************************************************/
/*!
\brief Destructor for CreditsState
*/
/******************************************************************************/
CreditsState::~CreditsState(void)
{
  m_ObjM.ClearObject();
  m_ObjM.ShutdownSystem();
  m_TexM.UnloadAssets();
  m_ObjM.UnLoadSystem();
}
/******************************************************************************/
/*!
\brief Load json data
\param filename
*/
/******************************************************************************/
void CreditsState::Load(const std::string& filename)
{
  UNREFERENCED_PARAMETER(filename);
  m_ObjM.LoadSystem();
  m_ObjM.SetAssetManager(&m_TexM);
  m_ObjM.SetStateManager(m_pGM);
  LoadAssets(filename);
 
  //write your code here...
}
/******************************************************************************/
/*!
\brief Init state
\param gameData
\param filename
*/
/******************************************************************************/

void CreditsState::Init(GameData& gameData, const std::string& filename)
{
  UNREFERENCED_PARAMETER(gameData);
  UNREFERENCED_PARAMETER(filename);
  LoadObjects(filename);
  m_ObjM.GetSubsystem<LogicSystem>()->SetGameData(&gameData);
  m_ObjM.InitSystem();
 
  //write your code here...

}
/******************************************************************************/
/*!
\brief Update state
\param gameData
*/
/******************************************************************************/

void CreditsState::Update(GameData& gameData)
{
  //Setting camera first

	if (m_soundPause == true)
	{
		m_ObjM.GetSubsystem<SoundSystem>()->PausingSoundSystem(false);
		m_soundPause = false;
	}

	if (m_pGM->IsMinimize() == true)
	{
		m_ObjM.GetSubsystem<SoundSystem>()->PausingSoundSystem(true);
		m_soundPause = true;
	}

  m_ObjM.GetSubsystem<GraphicSystem>()->SetCamera(0, 0, 300, 0);
  UNREFERENCED_PARAMETER(gameData);
  m_ObjM.UpdateSystem(gameData.dt);
 
  //write your code here...
  float move = gameData.dt * 100.f;
  static float criteria_y = -450.f;

  //Move the credit text
  criteria_y += move;

  //Contents of credit
  m_ObjM.GetGameObject("Credit")->GetComponent<Text>()->SetInput(
	  "Credits\n"
	  "Game Title: A Shepherd Boy\n"
	  "Team Name: Recycle Bin\n"
	  "Class: GAM250KR\n"
	  "President : Claude Comair\n"
	  "Instructor : David Ly\n"
	  "Tech Lead: Han Jangseok\n"
	  "Design Lead: Brice Hernandez\n"
	  "Graphic Programmer: Jeong Juyong\n"
	  "Producer: Eun Woonggi\n"
	  );

  m_ObjM.GetGameObject("Credit_CPR_1")->GetComponent<Text>()->SetInput(
	  "Copyrights\n"
	  "All content (C) 2016 DigiPen (USA) Corporation,\n"
	  "all rights reserved.\n"
	  "rapidjson, Copyright (C) 2015 THL A29 Limited,\n"
	  "a Tencent company, and Milo Yip.\n"
	  "All rights reserved. Licensed under MIT License.\n"
	  );

  m_ObjM.GetGameObject("Credit_CPR_2")->GetComponent<Text>()->SetInput(
	  "freetype, Copyright 1996 - 2001, 2002 by David Turner,\n"
	  "Robert Wilhelm, and Werner Lemberg.\n"
	  "FMOD, copyright (C) Firelight Technologies Pty,\n"
	  "Ltd., 2012-2016.\n"
	  );

  //Show the text
  
  m_ObjM.GetGameObject("Credit_CPR_1")->GetComponent<Transform>()->SetPosition(Vector(-150, criteria_y, 1));
  m_ObjM.GetGameObject("Credit_CPR_2")->GetComponent<Transform>()->SetPosition(Vector(-150, criteria_y - 350, 1));
  m_ObjM.GetGameObject("Credit")->GetComponent<Transform>()->SetPosition(Vector(-150, criteria_y - 650, 1));

  //Refresh the position
  if (criteria_y > 1700)
	  criteria_y = -450.f;

  //Go back to menu
  if (InputManager::GIInput().IsKeyTriggered(K_ESC)||
	  InputManager::GIInput().IsKeyTriggered(K_ENTER)||
	  InputManager::GIInput().IsKeyTriggered(K_SPACE))
  {
	  criteria_y = -450.f;
	  m_pGM->Resume();
  }

}
/******************************************************************************/
/*!
\brief Shutdown state
\param gameData
*/
/******************************************************************************/

void CreditsState::Shutdown(GameData& gameData)
{
  UNREFERENCED_PARAMETER(gameData);
  m_ObjM.ClearObject();
  m_ObjM.ShutdownSystem();
}
/******************************************************************************/
/*!
\brief UnLoad state's data
\param gameData
*/
/******************************************************************************/

void CreditsState::UnLoad(GameData& gameData)
{
  UNREFERENCED_PARAMETER(gameData);
  m_TexM.UnloadAssets();
  m_ObjM.UnLoadSystem();
}
/******************************************************************************/
/*!
\brief Load Assets
\param filename
*/
/******************************************************************************/

bool CreditsState::LoadAssets(const std::string& filename)
{
  std::ifstream in(filename);
  std::string contents((std::istreambuf_iterator<char>(in)),
    std::istreambuf_iterator<char>());
 
  rapidjson::Document doc;
  doc.Parse(contents.c_str());
 
  if (doc.HasParseError())
  {
    std::cerr << "Error Application.cpp : Invalid JSON, error number " << doc.GetParseError() << std::endl;
    return false;
  }
 
  if (doc.HasMember("Assets"))
  {
    m_TexM.Load(doc["Assets"]);
  }
 
  return true;
}
/******************************************************************************/
/*!
\brief Load Objects
\param filename
*/
/******************************************************************************/

bool CreditsState::LoadObjects(const std::string& filename)
{
  std::ifstream in(filename);
  std::string contents((std::istreambuf_iterator<char>(in)),
    std::istreambuf_iterator<char>());
 
  rapidjson::Document doc;
  doc.Parse(contents.c_str());
 
  if (doc.HasParseError())
  {
    std::cerr << "Error Application.cpp : Invalid JSON, error number " << doc.GetParseError() << std::endl;
    return false;
  }
 
  if (doc.HasMember("Objects") && doc["Objects"].IsArray())
  {
    for (auto it = doc["Objects"].Begin(); it != doc["Objects"].End(); ++it)
    {
      if (it->HasMember("name") && (*it)["name"].IsString())
      {
        std::string name = (*it)["name"].GetString();
        
        Object* newObj = new Object();
        if (newObj)
        {
          m_ObjM.AddObject(newObj, name);
          newObj->Load(*it);
        }
      }
    }
  }
 
  return true;
}
 
