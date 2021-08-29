#include "FighterComponent.h"

FighterComponent::FighterComponent()
{
	name = "Luka";

	stats["Strength"] = 0;	//Ever heard of DAMAGE?
	stats["Dexterity"] = 0; //Flat Defense
	stats["Vitality"] = 0;	//Im only here for the food.
	stats["MaxHitpoints"] = 10;	//This needs to update on Level-Ups in Vitality.
	stats["Hitpoints"] = stats["MaxHitpoints"];	//This needs to update on Level-Ups in Vitality.
	stats["Combat"] = 0;	//This stat is dumb. (and overpowered - it's effectively the only stat that matters)
	stats["Initiative"] = 0;	//This stat is useless.

	isPlayer = false;

	combatManeuvers.push_back(CombatManeuvers::StandartAttack);
};

FighterComponent::FighterComponent(std::string name, int str, int dex, int vit, int cbt, int ini, Weapon* wpn)
{
	this->name = name;

	stats["Strength"] = str;
	stats["Dexterity"] = dex;
	stats["Vitality"] = vit;
	stats["MaxHitpoints"] = 10 + vit * 5;
	stats["Hitpoints"] = stats["MaxHitpoints"];
	stats["Combat"] = cbt;
	stats["Initiative"] = ini;

	isPlayer = false;

	weapon = wpn;

	combatManeuvers.push_back(CombatManeuvers::StandartAttack);
};