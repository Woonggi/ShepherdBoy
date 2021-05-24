/******************************************************************************/
/*!
file    Credits.h
\author jeong.juyong
\par    email: jeykop14\@gmail.com
\date   2016-01-18
\description
prototype of Drive class for CreditsState in stage class.
All content (C) 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
 
#ifndef Credits_H
#define Credits_H
 
#include "../../../Engine/System/StateSystem/VirtualState.h"
#include "../../../Engine/Base/ObjectManager.h"
#include "../../../Engine/Tool/Asset/AssetManager.h"
 
/******************************************************************************/
/*!
\class CreditsState
\brief
  CreditsState is a Drive Class in stage hierarchy
 
Operations include:
- Constructor
- Initialize
- Update
- Shutdown
 
*/
/******************************************************************************/
 
class CreditsState : public VirtualState
{
public:
  CreditsState(StateController* pGM) 
	  : VirtualState(pGM), m_soundPause(false) {}
  ~CreditsState(void);
 
  virtual void Init(GameData& gameData, const std::string& filename);
  virtual void Update(GameData& gameData);
  virtual void Shutdown(GameData& gameData);
  virtual void Load(const std::string& filename);
  virtual void UnLoad(GameData& gameData);
  virtual bool LoadObjects(const std::string& filename);
  virtual bool LoadAssets(const std::string& filename);
 
  //write your method functions here...
 
private:
  ObjectManager m_ObjM;
  AssetManager m_TexM;

  std::string credits;
  //write your variables here...
  bool m_soundPause;
};
 
#endif //Credits_H
 
