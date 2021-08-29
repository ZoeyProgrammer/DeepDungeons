#pragma once
#include "FighterComponent.h"
#include "Entity.h"

class CombatSystem
{
public:
	CombatSystem();

	void CombatStart();	//For init
	void CombatLoop();	//CombatLoop till all enemies are dead
	void SummonEnemy(bool isBoss);
	void Combat(int attacker, int defender, int cbtMod, int atkMod);
private:
	std::vector<Entity*> fighters; //Entities which have the FighterComponent
	int currentLevel;
};