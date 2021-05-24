/******************************************************************************/
/*!
file    ShopLogic.cpp
\author Woonggi Eun
\par    email: dmsdndre\@gmail.com
\date   5/12/2016
\description
Logic component for ShopLogic
All content (C) 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
 
#include "ShopLogic.h"
#include "../../../Engine/BaseEngine.h"
#include "../../../Engine/System/PhysicsSystem/Collision/CollisionEvent.h"
#include "../../GameLogic/Menu/TypingAnimation.h"
 
namespace
{
	const int MAX_COOLDOWN = 10;
	const int MAX_DAMAGE   = 15;
}

ShopLogicBuilder::ShopLogicBuilder()
	: ComponentBuilder()
{ }
 
ShopLogicBuilder::~ShopLogicBuilder()
{ }
 
Component* ShopLogicBuilder::create(ObjectPtr owner) const
{
  return new ShopLogic(owner);
}
 
ShopLogic::ShopLogic(ObjectPtr pObject)
  :Logic(pObject)
{
  m_pSystem->AddLogic(this);
}
 
ShopLogic::~ShopLogic(void)
{
  m_pSystem->DeleteLogic(this);
}
 
bool ShopLogic::Load(const rapidjson::Value& data)
{
  UNREFERENCED_PARAMETER(data);
  return true;
}
/******************************************************************************/
/*!

\brief - Initialize data.

*/
/******************************************************************************/
void ShopLogic::InitLogic(void)
{
	//Set object pointers to certain objects
	sheep    = m_OBM->GetGameObject("Sheep");
	food     = m_OBM->GetGameObject("Food");
	fence    = m_OBM->GetGameObject("Fence");
    cooldown = m_OBM->GetGameObject("CooldownUpgrade");
    damage   = m_OBM->GetGameObject("DamageUpgrade");
	mouse    = m_OBM->GetGameObject("Mouse_Cursor");
	script   = m_OBM->GetGameObject("Script");

	cost_sheep    = m_OBM->GetGameObject("SheepCost");
	cost_food     = m_OBM->GetGameObject("FoodCost");
	cost_fence    = m_OBM->GetGameObject("FenceCost");
    cost_cooldown = m_OBM->GetGameObject("CooldownCost");
    cost_damage   = m_OBM->GetGameObject("DamageCost");

	sheepText = m_OBM->GetGameObject("SheepText");
	fenceText = m_OBM->GetGameObject("FenceText");
	moneyText = m_OBM->GetGameObject("MoneyText2");
	foodText  = m_OBM->GetGameObject("FoodText");
	dayText   = m_OBM->GetGameObject("DayText");

    descWindow = m_OBM->GetGameObject("DescWindow");
    descScript = m_OBM->GetGameObject("DescScript");

	m_print       = false;
	m_scriptTimer = 1.75f;
	m_timerDup    = m_scriptTimer;

	m_gameData = m_pSystem->GetGameData();
	CalculateFoodConsumption();
}
 
/******************************************************************************/
/*!

\brief - Updating this logic.

\param dt - value for real-time calculation

*/
/******************************************************************************/
void ShopLogic::UpdateLogic(float dt)
{
	dt;
	//Function that buy things.
	ClickNBuy();
	UpdateScript(dt);

	//Cost changes.
	CostEquations();
	//Show properties on screen.
	PrintProperties();
	PrintTextsBtLocal();
	UpdateFoodConsumption();
}
 
void ShopLogic::ShutLogic(void)
{
 
}

/******************************************************************************/
/*!

\brief - Function that return true if the mouse is on the object getting from
         parameter

\param pObj - Target object to check is it intersect with mouse cursor.

*/
/******************************************************************************/
bool ShopLogic::MouseEvent(ObjectPtr obj)
{
	//Check if mouse cursor and obj is intersecting.
    if (ColDetect::SquareToSquare(obj->GetComponent<Transform>()->GetPosition(),
        obj->GetComponent<Transform>()->GetScale(), 0,
        mouse->GetComponent<Transform>()->GetPosition(),
        mouse->GetComponent<Transform>()->GetScale(), 0))
    {
		//If so, show description window and description.
        Vector mousePos = mouse->GetComponent<Transform>()->GetPosition();
        mousePos.x += 70.f;
        mousePos.y -= 40.f;

        descWindow->GetComponent<Transform>()->SetPosition(mousePos);
        descWindow->GetComponent<Sprite>()->SetColor(1.f, 1.f, 1.f, 1.f);

        mousePos.x -= 70.f;
        mousePos.y += 20.f;

        descScript->GetComponent<Transform>()->SetPosition(mousePos);
        descScript->GetComponent<Text>()->SetColor(1.f, 1.f, 1.f, 1.f);
        descScript->GetComponent<Text>()->SetFontSize(9);
       
        obj->GetComponent<Sprite>()->SetColor(1.f, 1.f, 0.f, 1.f);

        return true;
    }
    else
        return false;

    return false;
}

/******************************************************************************/
/*!

\brief - Showing scripts such as "You bought sheep".

*/
/******************************************************************************/
void ShopLogic::UpdateScript(float dt)
{
	if (m_print)
	{
		m_scriptTimer -= dt;

		script->GetComponent<Text>()->SetColor(1.f, 1.f, 1.f, 1.f);

		SetTextAlphaValue(0.f);
	}

	if (m_scriptTimer <= 0)
	{
		m_scriptTimer = m_timerDup;
		m_print       = false;
		script->GetComponent<Text>()->SetColor(1.f, 1.f, 1.f, 0.f);

		SetTextAlphaValue(1.f);
	}
}

/******************************************************************************/
/*!

\brief - Buy items by mouse clicking.

\param stuff - Value that player wants to buy.

\param buy    - enumeration that type of buying.

\param amount - How much player will buy.

\param cost   - Cost of item.

*/
/******************************************************************************/

void ShopLogic::BuyThings( int* stuff, _Buy buy, int amount, int cost)
{
	//If player has money more that cost of item, 
	if (m_gameData->money >= cost)
	{
		//Success to buy.
		m_gameData->money -= cost;
		*stuff += amount;
		PrintScript(buy);
		m_OBM->GetSubsystem<SoundSystem>()->PlaySE("SpendMoney");
	}
	//If not...
	else if (m_gameData->money < cost)
	{
		//script print.
		script->GetComponent<TypingAnimation>()->InitSentence("Not Enough Money...");
	
		ResetScript();
	}
}
/******************************************************************************/
/*!

\brief - Calcalate costs base on equations.

*/
/******************************************************************************/
void ShopLogic::CostEquations(void)
{
    //Setting costs of elements.
	fenceCost = m_gameData->fenceLevel * 125;
    cooldownCost = m_gameData->slingshot_cooldown_level * 200;
    damageCost   = m_gameData->slingshot_damage_level * 250;
    foodCost     = 200;
}

/******************************************************************************/
/*!

\brief - Print out costs based on cost data.

*/
/******************************************************************************/
void ShopLogic::PrintProperties(void)
{
	cost_sheep->GetComponent<Text>()->SetInput("%d", m_gameData->sheepCost);

    cost_food->GetComponent<Text>()->SetInput("%d", foodCost);

	cost_fence->GetComponent<Text>()->SetInput("%d", fenceCost);

	if (m_gameData->slingshot_cooldown_level < MAX_COOLDOWN)
		cost_cooldown->GetComponent<Text>()->SetInput("%d", cooldownCost);
	else
		cost_cooldown->GetComponent<Text>()->SetInput("Full Upgrade");

	if (m_gameData->slingshot_damage_level < MAX_DAMAGE)
		cost_damage->GetComponent<Text>()->SetInput("%d", damageCost);
	else
		cost_damage->GetComponent<Text>()->SetInput("Full Upgrade");
}

/******************************************************************************/
/*!

\brief - Prints different scripts according to paramter.

*/
/******************************************************************************/
void ShopLogic::PrintScript(_Buy buy)
{
	ResetScript();

	switch (buy)
	{
	case buy_sheep:
		script->GetComponent<TypingAnimation>()->InitSentence("You Bought Sheep.");
		break;
	case fenceUpgrade:
		script->GetComponent<TypingAnimation>()->InitSentence("Upgrade Fence!");
		break;
	case farmUpgrade:
		script->GetComponent<TypingAnimation>()->InitSentence("Upgrade Farm!");
		break;
	case buy_food:
		script->GetComponent<TypingAnimation>()->InitSentence("You Bought 50 Food.");
		break;
    case upgrade_cooldown:
        script->GetComponent<TypingAnimation>()->InitSentence("Upgrade Slingshot Cooldown!");
        break;
    case upgrade_damage:
        script->GetComponent<TypingAnimation>()->InitSentence("Upgrade Slingshot Damage!");
        break;
	}

	script->GetComponent<TypingAnimation>()->SetSpeed(70.f);
}

/******************************************************************************/
/*!

\brief - Buy items by clicking mouse.

*/
/******************************************************************************/
void ShopLogic::ClickNBuy(void)
{
	//Buying Sheep
	if (MouseEvent(sheep))
	{
		descScript->GetComponent<Text>()->SetInput("Sheep\n\nIncrease number of \nsheep");

		if (InputManager::GIInput().IsKeyTriggered(K_MOUSE_LEFT))
		{
			if (m_gameData->sheep < m_gameData->sheepLimit)
				BuyThings(&m_gameData->sheep, buy_sheep, 1, m_gameData->sheepCost);
			else
			{
				ResetScript();
				script->GetComponent<TypingAnimation>()->InitSentence("Farm is full...");	
			}
		}
	}
   	//Fence Upgrade
	else if (MouseEvent(fence))
	{
        descScript->GetComponent<Text>()->SetInput("Fence\n\nUpgrade Fence Hp");

		if (InputManager::GIInput().IsKeyTriggered(K_MOUSE_LEFT))
			BuyThings(&m_gameData->fenceLevel, fenceUpgrade, 1, fenceCost);
	}
	//Buying Food
    else if (MouseEvent(food))
    {
        descScript->GetComponent<Text>()->SetInput("Food\n\nFood Increases 50");

        if (InputManager::GIInput().IsKeyTriggered(K_MOUSE_LEFT))
            BuyThings(&m_gameData->food, buy_food, 50, foodCost);
    }    
    //cooldown upgrade
    else if (MouseEvent(cooldown))
    {
        float cooldown_ = 2.f;
        int cooldown_lev = m_gameData->slingshot_cooldown_level;
        cooldown_ = cooldown_ - static_cast<float>((cooldown_lev - 1) * 0.133333f);

        descScript->GetComponent<Text>()->SetInput("Cooldown Upgrade\n\nReduce Reloading time\n\n\nCooldown: %.3f", cooldown_);

		if (InputManager::GIInput().IsKeyTriggered(K_MOUSE_LEFT))
		{
			if (m_gameData->slingshot_cooldown_level < MAX_COOLDOWN)
				BuyThings(&m_gameData->slingshot_cooldown_level, upgrade_cooldown, 1, cooldownCost);
			else
			{
				ResetScript();
				script->GetComponent<TypingAnimation>()->InitSentence("No more upgrade is available");
			}
		}
            
    }
    //damage upgrade
    else if (MouseEvent(damage))
    {
		int dmg_scale = m_pSystem->GetGameData()->slingshot_damage_level;
		const float baseDmg = 70.f + static_cast<float>((dmg_scale - 1) * 3.5f);
		
        descScript->GetComponent<Text>()->SetInput("Damage Enhancement\n\nGain More Power For\nSlingshot\n\nBase Damage : %.3f", baseDmg);

		if (InputManager::GIInput().IsKeyTriggered(K_MOUSE_LEFT))
		{
			if (m_gameData->slingshot_damage_level < MAX_DAMAGE)
				BuyThings(&m_gameData->slingshot_damage_level, upgrade_damage, 1, damageCost);
			else
			{
				ResetScript();
				script->GetComponent<TypingAnimation>()->InitSentence("No more upgrade is available");
			}
		}
           
    }
    else
    {
        fence->GetComponent<Sprite>()->SetColor(1.f, 1.f, 1.f, 1.f);
        sheep->GetComponent<Sprite>()->SetColor(1.f, 1.f, 1.f, 1.f);
        cooldown->GetComponent<Sprite>()->SetColor(1.f, 1.f, 1.f, 1.f);
        damage->GetComponent<Sprite>()->SetColor(1.f, 1.f, 1.f, 1.f);
        food->GetComponent<Sprite>()->SetColor(1.f, 1.f, 1.f, 1.f);

        descScript->GetComponent<Text>()->SetColor(1.f, 1.f, 1.f, 0.f);
        descWindow->GetComponent<Sprite>()->SetColor(1.f, 1.f, 1.f, 0.f);
    }

}

/******************************************************************************/
/*!

\brief - Prints properties in description window.

*/
/******************************************************************************/
void ShopLogic::PrintTextsBtLocal(void)
{	
	sheepText->GetComponent<Text>()->SetInput("Sheep : %i / %i", m_pSystem->GetGameData()->sheep, m_pSystem->GetGameData()->sheepLimit);
	fenceText->GetComponent<Text>()->SetInput("Fence : Level %i", m_pSystem->GetGameData()->fenceLevel);
	moneyText->GetComponent<Text>()->SetInput("Money : %i", m_pSystem->GetGameData()->money);
	foodText->GetComponent<Text>()->SetInput("Food : %i / %i", m_pSystem->GetGameData()->food, m_pSystem->GetGameData()->foodConsumption);
	dayText->GetComponent<Text>()->SetInput("Day %i", m_pSystem->GetGameData()->day);	
}

/******************************************************************************/
/*!

\brief - Calcalate food consumption.

*/
/******************************************************************************/
void ShopLogic::CalculateFoodConsumption(void)
{
	m_pSystem->GetGameData()->foodConsumption = 0;

	if (m_pSystem->GetGameData()->knight)
		m_pSystem->GetGameData()->foodConsumption
		+= m_pSystem->GetGameData()->knightCost / 3;

	if (m_pSystem->GetGameData()->wizard)
		m_pSystem->GetGameData()->foodConsumption
		+= m_pSystem->GetGameData()->wizardCost / 3;

	if (m_pSystem->GetGameData()->hunter)
		m_pSystem->GetGameData()->foodConsumption
		+= m_pSystem->GetGameData()->hunterCost / 3;

	if (m_pSystem->GetGameData()->compu)
		m_pSystem->GetGameData()->foodConsumption
		+= m_pSystem->GetGameData()->compuCost / 3;

	if (m_pSystem->GetGameData()->flower)
		m_pSystem->GetGameData()->foodConsumption
		+= m_pSystem->GetGameData()->flowerCost / 3;

	if (m_pSystem->GetGameData()->dog)
		m_pSystem->GetGameData()->foodConsumption
		+= m_pSystem->GetGameData()->dogCost / 3;

	if (m_pSystem->GetGameData()->food < m_pSystem->GetGameData()->foodConsumption)
	{
		m_OBM->GetGameObject("FoodText")->GetComponent<Text>()->SetColor(1, 0, 0, 1);
		m_pSystem->GetGameData()->visitorState = false;
	}
}

/******************************************************************************/
/*!

\brief - Update food consumption.

*/
/******************************************************************************/
void ShopLogic::UpdateFoodConsumption(void)
{
	if (m_pSystem->GetGameData()->food >= m_pSystem->GetGameData()->foodConsumption)
	{
		if (m_print)
			m_OBM->GetGameObject("FoodText")->GetComponent<Text>()->SetColor(1, 1, 1, 0.f);
		else 
			m_OBM->GetGameObject("FoodText")->GetComponent<Text>()->SetColor(1, 1, 1, 1.f);

		m_pSystem->GetGameData()->visitorState = true;
	}
}

/******************************************************************************/
/*!

\brief - Set alpha value of texts that specify elements.

\param a - Input alpha value to set.

*/
/******************************************************************************/
void ShopLogic::SetTextAlphaValue(float a)
{
	sheepText->GetComponent<Text>()->SetColor(1.f, 1.f, 1.f, a);
	fenceText->GetComponent<Text>()->SetColor(1.f, 1.f, 1.f, a);
	moneyText->GetComponent<Text>()->SetColor(1.f, 1.f, 1.f, a);
	foodText->GetComponent<Text>()->SetColor(1.f, 1.f, 1.f, 0.f);
}

/******************************************************************************/
/*!


\brief - Resetting data that related into script printing.


*/
/******************************************************************************/

void ShopLogic::ResetScript(void)
{
	m_print = true;
	m_scriptTimer = m_timerDup;
}