/******************************************************************************/
/*!
file    ShopLogic.h
\author Woonggi Eun
\par    email: dmsdndre\@gmail.com
\date   5/12/2016
\description
Logic component for ShopLogic
All content (C) 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
 
#ifndef ShopLogic_H
#define ShopLogic_H
 
#include "../../../Engine/Component/Component.h"
#include "../../../Engine/Component/Logic/Logic.h"
 
struct GameData;

typedef enum _Buy
{   buy_sheep, 
    fenceUpgrade, 
    farmUpgrade, 
    buy_food, 
    upgrade_cooldown, 
    upgrade_damage
};

class ShopLogicBuilder : public ComponentBuilder
{
public:
	ShopLogicBuilder();
	~ShopLogicBuilder();
 
	virtual Component* create(ObjectPtr owner) const;
};
 
class LogicSystem;
 
class ShopLogic : public Logic
{
public:
  ShopLogic(ObjectPtr pObject);
  ~ShopLogic(void);
 
  void InitLogic(void);
  void UpdateLogic(float dt);
  void ShutLogic(void);
  virtual bool Load(const rapidjson::Value& data);
 
  bool MouseEvent(ObjectPtr obj);
  void BuyThings( int* stuff, _Buy buy,  int amount,  int cost);

  void CostEquations(void); 
  void PrintProperties(void);
  void PrintScript(_Buy buy);
  void UpdateScript(float dt);

  void ClickNBuy(void);
  void PrintTextsBtLocal(void);

  void CalculateFoodConsumption(void);
  void UpdateFoodConsumption(void);

  void SetTextAlphaValue(float a);
  void ResetScript(void);

protected:
	ObjectPtr script;

	//Items(products)
	ObjectPtr sheep;
	ObjectPtr food;
	ObjectPtr fence;
    ObjectPtr cooldown;
    ObjectPtr damage;

	ObjectPtr mouse;

	//Costs
	ObjectPtr cost_sheep;
	ObjectPtr cost_food;
	ObjectPtr cost_fence;
    ObjectPtr cost_cooldown;
    ObjectPtr cost_damage;

	//UI
	ObjectPtr sheepText;
	ObjectPtr fenceText;
	ObjectPtr moneyText;
	ObjectPtr foodText;
	ObjectPtr dayText;

    ObjectPtr descWindow;
    ObjectPtr descScript;

	GameData* m_gameData;

	bool m_print;
	float m_scriptTimer;
	float m_timerDup;
    
	int sheepLimit;
    int fenceCost;
    int foodCost;
    int cooldownCost;
    int damageCost;
};
 
#endif //ShopLogic
 
