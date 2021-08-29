#pragma once
#include <unordered_map>
#include <string>
#include <vector>

struct Weapon
{
	std::string name;
	int diceAmount;
	int damageMod;
	int combatMod;
};

enum class CombatManeuvers
{
	StandartAttack,
	SweepingStrike,
	PowerStrike
};

class FighterComponent
{
public:
	std::string name;
	std::unordered_map<std::string, int> stats;
	Weapon* weapon;
	std::vector<CombatManeuvers> combatManeuvers;

	bool isPlayer;

	FighterComponent();
	FighterComponent(std::string name, int str, int dex, int vit, int cbt, int ini, Weapon* wpn);
};

