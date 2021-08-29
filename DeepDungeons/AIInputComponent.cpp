#include "AIInputComponent.h"
#include <iostream>
#include <string>
#include "Util.h"

AIInputComponent::AIInputComponent()
{

}

FighterComponent* AIInputComponent::Create(bool isBoss)
{
	//Our template we're going to fill
	FighterComponent* enemy = new FighterComponent();


	if (isBoss)
	{//Bosses (Featuring Klaus from the Klaus Saga, see more on schandenserver.de)
		std::vector<FighterComponent*> bosses;
		bosses.push_back(new FighterComponent("Landherr Klaus", 3, 9, 10, 4, 5, new Weapon{ "Stink. Papier", 1, 2, 3 }));
		bosses.push_back(new FighterComponent("Grottenherr Klaus", 4, 9, 10, 2, 3, new Weapon{ "Tot. Fogl", 1, 2, 0 }));
		bosses.push_back(new FighterComponent("Waldbeschuetzer Klaus", 5, 5, 10, 5, 8, new Weapon{ "Stum. Floete", 1, 0, 0 }));
		bosses.push_back(new FighterComponent("Meister des Landschlosses Klaus", 1, 2, 8, 6, 7, new Weapon{ "Dreck. Haarband", 2, 0, 3 }));
		bosses.push_back(new FighterComponent("Meister des Seeschlosses Klaus", 5, 5, 10, 5, 5, new Weapon{ "Fab. Muschel", 1, 3, 2 }));
		bosses.push_back(new FighterComponent("Herzog Koenig Lord Klaus", 6, 9, 1, 15, 20, new Weapon{ "U. Powerlocke", 2, 1, 3 })); //This enemy is insanely strong because high combat
		enemy = bosses[Util::RNG(0, bosses.size() - 1)];
		enemy->combatManeuvers.push_back(CombatManeuvers::PowerStrike);
	}
	else
	{//Normal Enemies	(~15Points each)
		std::vector<FighterComponent*> enemies;
		enemies.push_back(new FighterComponent("Slime Girl", 2, 5, 0, 3, 5, new Weapon{ "Slime", 1, 1, 0 }));
		enemies.push_back(new FighterComponent("Little Imp", 3, 0, 2, 5, 4, new Weapon{ "Fang", 1, 0, 1 }));
		enemies.push_back(new FighterComponent("Zombie Swordsman", 3, 2, 5, 5, 0, new Weapon{ "Sword", 1, 1, 0 }));
		enemies.push_back(new FighterComponent("Devil Fighter", 5, 4, 3, 3, 0, new Weapon{ "Fist", 1, 0, 1 }));
		enemies.push_back(new FighterComponent("Iron Maiden", 3, 8, 4, 0, 0, new Weapon{ "Body", 1, 1, 0 }));
		enemies.push_back(new FighterComponent("High Minotaurus", 8, 0, 4, 3, 0, new Weapon{ "Axe", 2, 0, 0 }));
		enemies.push_back(new FighterComponent("Four Tailed Samurai", 2, 4, 4, 3, 2, new Weapon{ "Katana", 1, 0, 1 }));
		enemies.push_back(new FighterComponent("Demon Monk", 4, 0, 4, 6, 1, new Weapon{ "Fist", 1, 0, 1 }));
		enemies.push_back(new FighterComponent("Experimental Organism S-2", 3, 3, 3, 3, 3, new Weapon{ "Substance", 1, 1, 1 }));
		enemy = enemies[Util::RNG(0, enemies.size() - 1)];
	}

	return enemy;
};

CombatManeuvers AIInputComponent::ChooseManeuver(FighterComponent* fighter)
{
	//Randomize from all options, standard needs to be 8 times as likely
	if (fighter->combatManeuvers.size() < 2)
	{
		return CombatManeuvers::StandartAttack;
	}
	else
	{
		int option = Util::RNG(0, fighter->combatManeuvers.size() + 6); //Only +6 because it is already once in the options and since 0 is a thing
		if (option <= fighter->combatManeuvers.size() -1)
		{
			return fighter->combatManeuvers[option];
		}
		else
		{
			return CombatManeuvers::StandartAttack;
		}
	}
}

int AIInputComponent::TargetDesignation(std::vector<FighterComponent*> fighters)
{
	for (int i = 0; i <= fighters.size(); i++)
	{
		if (fighters[i]->isPlayer == true)
		{
			return i;
		}
	}
	return 0;
}