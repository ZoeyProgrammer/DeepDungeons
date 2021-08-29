#include "CombatSystem.h"
#include <iostream>
#include "Util.h"
#include "Game.h"

CombatSystem::CombatSystem()
{
	currentLevel = 1;
}

void CombatSystem::CombatStart() //Initialize everything needed for combat
{
	if (fighters.empty())	//Character Creation happens here
	{
		//Creating the Player Entity
		Entity* player = new Entity();
		player->inputComponent = new PlayerInputComponent();
		player->fighterComponent = player->inputComponent->Create(false);
		fighters.push_back(player);
	}
	std::cout << "Initiating Combat..\n";

	//Check if this is a normal combat or boss combat
	if (currentLevel % 3 == 0)
	{
		//Summon a singular Boss enemy
		SummonEnemy(true);
	}
	else
	{
		//Summon 2-4 Enemies
		int enemyNumber = Util::RNG(2, 4);
		while (enemyNumber > 0)
		{
			SummonEnemy(false);
			enemyNumber -= 1;
		}
	}

	//Sort according to Inititative:
	std::vector<Entity*> toSort = fighters;
	std::vector<Entity*> iniOrder;
	while (!toSort.empty())
	{
		//Randomize the order, so it's effectively random which one is first, when 2 have the same Inititative
		//It's a bit of a outlandish way to do it, but it does work
		for (int i = 0; i < toSort.size(); i++)
		{
			Entity* temp = toSort[i];
			toSort.erase(toSort.begin() + i);
			toSort.insert(toSort.begin() + Util::RNG(0, toSort.size()), temp);
		}

		int highIni = 0;
		int iniEntity = 0;
		for (int i = 0; i < toSort.size(); i++)
		{//See which one has the highest Initiative
			if (toSort[i]->fighterComponent->stats["Initiative"] >= highIni)
			{
				highIni = toSort[i]->fighterComponent->stats["Initiative"];
				iniEntity = i;
			}
		}
		iniOrder.push_back(toSort[iniEntity]);
		toSort.erase(toSort.begin() + iniEntity);
	}

	fighters = iniOrder;

	//Check if their names are the same and add additonal Identifier
	//Just like in Dragon Quest
	for (Entity* fighter : fighters)
	{
		std::string name = fighter->fighterComponent->name;
		std::vector<Entity*> sameName;
		for (Entity* fighter : fighters)
		{
			if (fighter->fighterComponent->name == name)
			{
				sameName.push_back(fighter);
			}
		}
		if (sameName.size() > 1)
		{
			for (int i = 0; i < sameName.size(); i++)
			{
				sameName[i]->fighterComponent->name = name + " " + Util::intToAlphabet(i);
			}
		}
	}

}

void CombatSystem::CombatLoop()
{
	for (int i = 0; i < fighters.size(); i++)
	{
		//Start of Combat Interface
		Util::Clear();
		std::cout << "Level: " << currentLevel << "\n\n";
		std::cout << "== Combat Order ==\n";
		for (Entity* entity : fighters)
		{
			if (entity == fighters[i])
			{
				std::cout << ">";
			}
			std::cout << " [INI " << entity->fighterComponent->stats["Initiative"] << "] "
				<< "[" << entity->fighterComponent->stats["Hitpoints"] << "/" << entity->fighterComponent->stats["MaxHitpoints"] << "HP] " << entity->fighterComponent->name << "\n";
		}
		std::cout << "\n";

		std::cout << "\n[NextTurn] It is " << fighters[i]->fighterComponent->name << "'s turn.\n";
		//Maneuver Choosing
		CombatManeuvers action = fighters[i]->inputComponent->ChooseManeuver(fighters[i]->fighterComponent);
		
		//Target Designation Prep
		std::vector<FighterComponent*> fightersList;
		int target = 0;
		for (Entity* fighter : fighters)
		{
			fightersList.push_back(fighter->fighterComponent);
		}
		
		// Damage Calculation
		switch (action)
		{
		case CombatManeuvers::StandartAttack:
			target = fighters[i]->inputComponent->TargetDesignation(fightersList);
			std::cout << " " << fighters[i]->fighterComponent->name << " attacks " << fighters[target]->fighterComponent->name << " using their " << fighters[i]->fighterComponent->weapon->name << "\n";
			//Damage Calc
			Combat(i, target, 0, 0);
			break;
		case CombatManeuvers::PowerStrike:
			target = fighters[i]->inputComponent->TargetDesignation(fightersList);
			std::cout << " " << fighters[i]->fighterComponent->name << " flails wildly at " << fighters[target]->fighterComponent->name << " with their " << fighters[i]->fighterComponent->weapon->name << "\n";
			//Damage Calc | CBT -4 | ATK +4
			Combat(i, target, -4, +4);
			break;
		case CombatManeuvers::SweepingStrike:
			target = fighters[i]->inputComponent->TargetDesignation(fightersList);
			if (fightersList.size() <= 2) //If only 1 Enemy available
			{
				std::cout << " " << fighters[i]->fighterComponent->name << " makes a wide sweep, hitting " << fighters[target]->fighterComponent->name << " with their " << fighters[i]->fighterComponent->weapon->name << "\n";
				//Damage Calc | CBT -4
				Combat(i, target, -4, 0);
			}
			else //2nd Target Designation by RNG
			{
				int target2 = 0;
				do {
					target2 = Util::RNG(0, fighters.size() - 1);
				} while (target2 == i || target2 == target);
				std::cout << " " << fighters[i]->fighterComponent->name << " makes a wide sweep, hitting " << fighters[target]->fighterComponent->name << " and " << fighters[target2]->fighterComponent->name << " with their " << fighters[i]->fighterComponent->weapon->name << "\n";
				//Damage Calc | CBT -4
				Combat(i, target, -4, 0);
				Combat(i, target2, -4, 0);
			}
			break;
		default:
			break;
		}

		//Check for dead people to delete them
		for (int j = 0; j < fighters.size(); j++)
		{
			if (fighters[j]->fighterComponent->stats["Hitpoints"] <= 0)
			{
				fighters.erase(fighters.begin() + j);
				j--;
			}
		}

		//Lose-Condition - Check if the player is dead
		bool playerIsAlive = false;
		for (Entity* fighter : fighters) //Check all alive fighters
		{
			if (fighter->fighterComponent->isPlayer == true)
			{
				playerIsAlive = true;
			}
		}
		if (playerIsAlive == false)
		{
			std::cout << "== GAME OVER ==\n";
			Util::Pause();
			Game::Instance().state = GameState::Closing;
			break;
		}
		//Win-Condition - Check if all Enemies are dead
		if (fighters.size() <= 1)
		{
			std::cout << "== VICTORY ==\n";
			Util::Pause();
			//Rewards
			bool acpt = false;
			
			//Skillpoints
			//Could maybe be put in a "StatChange" function or so?
			// -> But would fill up IInputComponent more than needed -> Dont do that
			do
			{
				Util::Clear();
				std::vector<int> statUps;
				std::cout << "== Level Up ==\n";
				int statPoints = 5;
				for (int i = 0; i <= 4; i++)
				{
					std::string selected = "";
					switch (i)
					{
					case 0:
						selected = "Strength";
						break;
					case 1:
						selected = "Dexterity";
						break;
					case 2:
						selected = "Vitality";
						break;
					case 3:
						selected = "Combat";
						break;
					case 4:
						selected = "Initiative";
						break;
					}
					int input = 0;
					input = Util::IntPrompt(statPoints, "Spend points for " + selected);
					statPoints -= input;
					statUps.push_back(input);
				}
				Util::Clear();
				std::cout << "= Level Up finished = \n"
					<< "Your stats are now:\n"
					<< "STR:" << fighters[0]->fighterComponent->stats["Strength"] + statUps[0] << "\n"
					<< "DEX:" << fighters[0]->fighterComponent->stats["Dexterity"] + statUps[1] << "\n"
					<< "VIT:" << fighters[0]->fighterComponent->stats["Vitality"] + statUps[2] << "\n"
					<< "MaxHP:" << fighters[0]->fighterComponent->stats["MaxHitpoints"] + statUps[2]*5 << "\n"
					<< "HP:" << fighters[0]->fighterComponent->stats["Hitpoints"] + statUps[2]*5 << "\n"
					<< "CBT:" << fighters[0]->fighterComponent->stats["Combat"] + statUps[3] << "\n"
					<< "INI:" << fighters[0]->fighterComponent->stats["Initiative"] + statUps[4] << "\n"
					<< "Undistributed:" << statPoints << "\n";

				acpt = Util::BoolPrompt("Accept? ");
				//Only actually make the changes if they accept, as it's annoying to reset
				if (acpt == true)
				{
					fighters[0]->fighterComponent->stats["Strength"] += statUps[0];
					fighters[0]->fighterComponent->stats["Dexterity"] += statUps[1];
					fighters[0]->fighterComponent->stats["Vitality"] += statUps[2];
					fighters[0]->fighterComponent->stats["MaxHitpoints"] += statUps[2] * 5;
					fighters[0]->fighterComponent->stats["Hitpoints"] += statUps[2] * 5;
					fighters[0]->fighterComponent->stats["Combat"] += statUps[3];
					fighters[0]->fighterComponent->stats["Initiative"] += statUps[4];
				}
			} while (acpt != true);

			//Boss Weapon Upgrades - Right down the road!
			if (currentLevel % 3 == 0) //Only if it was a boss stage
			{
				Util::Clear();
				std::cout << "== A Mysterious Encounter == \n"
					<< fighters[0]->fighterComponent->name << " found a weaponsmith, right down the road (inside of the dungeon) \n";
				Util::Pause();
				std::cout << fighters[0]->fighterComponent->name << " couldn't really miss it.\n";
				Util::Pause();
				std::cout << "The weaponsmith offers " << fighters[0]->fighterComponent->name << " to upgrade " << fighters[0]->fighterComponent->weapon->name << "\n";
				Util::Pause();
				//Weapon Upgrade
				acpt == false;
				do
				{
					Util::Clear();
					std::cout << "== Weapon Upgrade ==\n";
					std::cout << "[0] Upgrade Combat Modifier + 1\n"
						<< "[1] Upgrade Damage Modifier + 2\n";
					int choice = Util::IntPrompt(1, "Choose ");
					if (choice == 0)
						std::cout << "You chose the Combat Modifier Upgrade \n";
					else
						std::cout << "You chose the Damage Modifier Upgrade \n";

					acpt = Util::BoolPrompt("Accept? ");
					//Only actually make the changes if they accept, as it's annoying to reset
					if (acpt == true)
					{
						if (choice == 0)
							fighters[0]->fighterComponent->weapon->combatMod += 1;
						else
							fighters[0]->fighterComponent->weapon->damageMod += 2;
					}
				} while (acpt != true);
			}

			//Potion Reward Chance
			if (Util::RNG(1, 5) == 1) //20% chance
			{
				Util::Clear();
				std::cout << "== Congratulations! == \n"
					<< fighters[0]->fighterComponent->name << " found a potion! \n";
				Util::Pause();
				std::cout << fighters[0]->fighterComponent->name << " drank the potion immediately without regarding consequences.\n";
				Util::Pause();
				fighters[0]->fighterComponent->stats["Hitpoints"] = fighters[0]->fighterComponent->stats["MaxHitpoints"]; //Actually set the HP to full too
				std::cout << fighters[0]->fighterComponent->name << " is back to full HP\n";
				Util::Pause();
				std::cout << fighters[0]->fighterComponent->name << " goes of into the next Level of the deep dungeon. \n";
				Util::Pause;
			}
			else
			{
				Util::Clear();
				std::cout << "== Dissapointing! == \n"
					<< fighters[0]->fighterComponent->name << " found no loot or magic swag. \n";
				Util::Pause;
				std::cout << fighters[0]->fighterComponent->name << " goes of into the next Level of the deep dungeon. \n";
				Util::Pause;
			}

			Util::Pause();
			//Set everything up for the next round
			currentLevel += 1;
			Game::Instance().state = GameState::CombatSetup;
			break;
		}

		Util::Pause();
	}
	

}

void CombatSystem::Combat(int attacker, int defender, int cbtMod, int atkMod)
{
	//DamageCalc Prep
	int atkCBT;
	int atkDMG;
	int defCBT;
	int defDEX;
	int diceCounter;
	int resultingDmg;

	//Damage Calc
	atkCBT = Util::RNG(1, 20) + fighters[attacker]->fighterComponent->stats["Combat"] + fighters[attacker]->fighterComponent->weapon->combatMod + cbtMod;
	atkDMG = fighters[attacker]->fighterComponent->stats["Strength"] + fighters[attacker]->fighterComponent->weapon->damageMod + atkMod;
	diceCounter = fighters[attacker]->fighterComponent->weapon->diceAmount;
	while (diceCounter > 0)
	{
		atkDMG += Util::RNG(1, 6);
		diceCounter--;
	};
	defCBT = Util::RNG(1, 20) + fighters[defender]->fighterComponent->stats["Combat"] + fighters[defender]->fighterComponent->weapon->combatMod;
	defDEX = fighters[defender]->fighterComponent->stats["Dexterity"];
	resultingDmg = atkDMG - defDEX;
	//Making sure you cant heal through the sheer power of masochism
	if (resultingDmg < 0)
		resultingDmg = 0;
	
	//Output
	std::cout << " [Attack] " << fighters[attacker]->fighterComponent->name + "[" << atkCBT << "] -> [" << defCBT << "]" << fighters[defender]->fighterComponent->name << " Result: ";
	if (atkCBT > defCBT)
	{
		std::cout << fighters[attacker]->fighterComponent->name << " hits \n";
		std::cout << " [Damage] " << fighters[attacker]->fighterComponent->name + "[" << atkDMG << "] - [" << defDEX << "]" << fighters[defender]->fighterComponent->name << " Result: ";

		std::cout << fighters[attacker]->fighterComponent->name << " deals " << resultingDmg << "DMG \n";
		fighters[defender]->fighterComponent->stats["Hitpoints"] -= resultingDmg;
	}
	else
	{
		std::cout << fighters[defender]->fighterComponent->name << " dodges the attack \n";
	}

	//Check for Dead People
	if (fighters[defender]->fighterComponent->stats["Hitpoints"] <= 0)
	{
		std::cout << " " << fighters[defender]->fighterComponent->name << " died. \n";
	}
}

void CombatSystem::SummonEnemy(bool isBoss)
{
	Entity* enemy = new Entity();
	enemy->inputComponent = new AIInputComponent();
	enemy->fighterComponent = enemy->inputComponent->Create(isBoss);
	fighters.push_back(enemy);
}