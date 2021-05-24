/******************************************************************************/
/*!
file    HTPLogic.cpp
\author Woonggi Eun
\par    email: dmsdndre\@gmail.com
\date   2016-06-06
\description
Logic component for HTPLogic
All content (C) 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
 
#include "HTPLogic.h"
#include "../../Engine/BaseEngine.h"
#include "../../Engine/System/PhysicsSystem/Collision/CollisionEvent.h"
#include "../GameLogic/Menu/TypingAnimation.h"
 
/******************************************************************************/
/*!

\brief - Logic Builder Constructor.

*/
/******************************************************************************/
HTPLogicBuilder::HTPLogicBuilder()
	: ComponentBuilder()
{ }
 
/******************************************************************************/
/*!

\brief - Logic Builder Destructor.

*/
/******************************************************************************/ 
HTPLogicBuilder::~HTPLogicBuilder()
{ }
 
/******************************************************************************/
/*!

\brief - Create logic and set owner.

*/
/******************************************************************************/
Component* HTPLogicBuilder::create(ObjectPtr owner) const
{
  return new HTPLogic(owner);
}
 
/******************************************************************************/
/*!

\brief - Logic constructor.

*/
/******************************************************************************/
HTPLogic::HTPLogic(ObjectPtr pObject)
  :Logic(pObject)
{
  m_pSystem->AddLogic(this);
}
 
/******************************************************************************/
/*!

\brief - Logic destructor.

*/
/******************************************************************************/
HTPLogic::~HTPLogic(void)
{
  m_pSystem->DeleteLogic(this);
}
 
/******************************************************************************/
/*!

\brief - Logic load from JSON file.

*/
/******************************************************************************/
bool HTPLogic::Load(const rapidjson::Value& data)
{
  UNREFERENCED_PARAMETER(data);
  return true;
}
 
/******************************************************************************/
/*!

\brief - Initial settings of logic.

*/
/******************************************************************************/
void HTPLogic::InitLogic(void)
{
    //Set object pointers to certain object.
    mouse = m_OBM->GetGameObject("Mouse_Cursor");

    managementTxt = m_OBM->GetGameObject("ManagementText");
    slingshotTxt  = m_OBM->GetGameObject("SlingshotText");
    visitorTxt    = m_OBM->GetGameObject("VisitorText");

    slingshot      = m_OBM->GetGameObject("Slingshot");
    wizard         = m_OBM->GetGameObject("Wizard");
    sheep          = m_OBM->GetGameObject("Sheep");
    fence          = m_OBM->GetGameObject("Fence");
    wolf           = m_OBM->GetGameObject("Wolf");
    werewolf       = m_OBM->GetGameObject("Werewolf");
    armedWerewolf  = m_OBM->GetGameObject("ArmedWerewolf");

    descWindow  = m_OBM->GetGameObject("DescWindow");
    descTxt     = m_OBM->GetGameObject("Script");

    managementTrigger = false;
    slingshotTrigger  = false;
    visitorTrigger    = false;
}
 
/******************************************************************************/
/*!

\brief - Updating logic codes.

*/
/******************************************************************************/
void HTPLogic::UpdateLogic(float dt)
{
    dt;

    Descriptions();
    FlickeringText(dt);
}

/******************************************************************************/
/*!

\brief - Activate when it is shutdown.

*/
/******************************************************************************/
void HTPLogic::ShutLogic(void)
{
 
}

/******************************************************************************/
/*!

\brief - Check is mouse having intersection with parameter.

\param pObj - Pointer to an object check 

\return - return true if mouse is intersecting with pObj.

*/
/******************************************************************************/
bool HTPLogic::MouseEvent(ObjectPtr pObj)
{
    //Check intersection between pObj and mouse cursor.
    if (ColDetect::SquareToSquare(pObj->GetComponent<Transform>()->GetPosition(),
        pObj->GetComponent<Transform>()->GetScale(), 0,
        mouse->GetComponent<Transform>()->GetPosition(),
        mouse->GetComponent<Transform>()->GetScale(), 0))
    {
        pObj->GetComponent<Sprite>()->SetColor(1.f, 1.f, 0.f, 1.f);
        descTxt->GetComponent<Text>()->SetColor(1.f, 1.f, 1.f, 1.f);
        
        //returns true if mouse cursor is intersecting with pObj
        return true;
    }
    //else, return false.
    else
    {
        pObj->GetComponent<Sprite>()->SetColor(1.f, 1.f, 1.f, 1.f);
        descTxt->GetComponent<Text>()->SetColor(1.f, 1.f, 1.f, 0.f);

        managementTrigger = false;
        slingshotTrigger  = false;
        visitorTrigger    = false;
        return false;
    }

    return false;
}

/******************************************************************************/
/*!

\brief - Showing corresponding descriptions to each object in how to play.

*/
/******************************************************************************/
void HTPLogic::Descriptions(void)
{
    if (MouseEvent(slingshot))
    {
        //Description showing.
        descTxt->GetComponent<Text>()->SetInput("Slingshot\nIn Management : Upgrade damage, cooldown\nIn Slingshot : Beat wolves by drag and drop");

        managementTrigger = true;
        slingshotTrigger = true;
        visitorTrigger = false;
    }
    else if (MouseEvent(wizard))
    {
        //Description showing.
        descTxt->GetComponent<Text>()->SetInput("Visitor\nIn Visitor State : You can hire visitors everyday\nby spending money.");

        managementTrigger = false;
        slingshotTrigger = true;
        visitorTrigger = true;
    }
    else if (MouseEvent(sheep))
    {
        descTxt->GetComponent<Text>()->SetInput("Sheep\nIn Management : You can buy and shear sheep.\nIn Slingshot : Protect sheep from wolves");
        //Description showing.
        managementTrigger = true;
        slingshotTrigger = true;
        visitorTrigger = false;
    }
    else if (MouseEvent(fence))
    {
        descTxt->GetComponent<Text>()->SetInput("Fence\nIn Management : Upgrade Fence HP.\nIn Slingshot : It protects sheep from wolves");
        //Description showing.
        managementTrigger = true;
        slingshotTrigger = true;
        visitorTrigger = false;
    }
    else if (MouseEvent(wolf))
    {
        descTxt->GetComponent<Text>()->SetInput("Normal wolf\nThe most basic wolf.\nHP : 100 / Speed : Fast");
        //Description showing.
        managementTrigger = false;
        slingshotTrigger = true;
        visitorTrigger = false;
    }
    else if (MouseEvent(werewolf))
    {
        descTxt->GetComponent<Text>()->SetInput("Werewolf\nStronger wolf.We cannot ignore this type of wolf\nHP : 500 / Speed : Normal");
        //Description showing.
        managementTrigger = false;
        slingshotTrigger = true;
        visitorTrigger = false;
    }
    else if (MouseEvent(armedWerewolf))
    {
        descTxt->GetComponent<Text>()->SetInput("ArmedWerewolf\nStrongest but slow. We can't push this wolf\nHP : 1000 / Speed : Slow");
        //Description showing.
        managementTrigger = false;
        slingshotTrigger = true;
        visitorTrigger = false;
    }
}

/******************************************************************************/
/*!

\brief - Text flickering.

*/
/******************************************************************************/
void HTPLogic::FlickeringText(float dt)
{
    static float c = 1.f;
    static bool trigger = false;

    if (managementTrigger)
    {
        if (c >= 1.f)
            trigger = true;
        else if (c <= 0.f)
            trigger = false;

        if (trigger)
            c -= 2 * dt;
        else
            c += 2 * dt;

        managementTxt->GetComponent<Text>()->SetColor(1.f, 1.f, 1.f, c);
    }
    else
        managementTxt->GetComponent<Text>()->SetColor(1.f, 1.f, 1.f, 1.f);

    if (slingshotTrigger)
    {
        if (c >= 1.f)
            trigger = true;
        else if (c <= 0.f)
            trigger = false;

        if (trigger)
            c -= 2 * dt;
        else
            c += 2 * dt;

        slingshotTxt->GetComponent<Text>()->SetColor(1.f, 1.f, 1.f, c);
    }
    else
       slingshotTxt->GetComponent<Text>()->SetColor(1.f, 1.f, 1.f, 1.f);

    if (visitorTrigger)
    {
        if (c >= 1.f)
            trigger = true;
        else if (c <= 0.f)
            trigger = false;

        if (trigger)
            c -= 2 * dt;
        else
            c += 2 * dt;

        visitorTxt->GetComponent<Text>()->SetColor(1.f, 1.f, 1.f, c);
    }
    else
        visitorTxt->GetComponent<Text>()->SetColor(1.f, 1.f, 1.f, 1.f);
}

