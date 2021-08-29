#include "PlayerInputComponent.h"
#include <iostream>
#include <string>
#include "Util.h"

PlayerInputComponent::PlayerInputComponent()
{

}

FighterComponent* PlayerInputComponent::Create(bool isBoss)
{
	//Our template we're going to fill
	FighterComponent* player = new FighterComponent();

	bool charFinished = false;
	do
	{
		//Name
		Util::Clear();
		std::cout << "= Welcome to Character Creation =\n"
			<< "Please choose your name: ";
		Util::FakePrompt();
		player->name = "Luka";
		std::cout << "That is a very nice name ^^\n";
		std::cout << "Anyways, the actual Hero's name is: " << player->name << "\n";

		bool acpt;
		acpt = Util::BoolPrompt("Accept? ");

		while (acpt == false)
		{
			Util::Clear();
			std::cout << "= Welcome to Character Creation =\n";
			std::string name = "";
			name = Util::StringPrompt("Choose a new name for the hero: ");
			player->name = name;
			std::cout << "The Hero's nickname is now " << player->name << "\n";
			acpt = Util::BoolPrompt("Accept? ");
		}

		//Skillpoints
		acpt = false;
		do {
			//Resetting for the loop
			player->stats["MaxHitpoints"] = 10;
			player->stats["Hitpoints"] = 10;

			Util::Clear();
			std::cout << "= Stat Distribution =\n";
			int statPoints = 30;
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
				player->stats[selected] = input;

				if (i == 2)	//If it's vitality, also update HP & maxHP
				{
					player->stats["MaxHitpoints"] += input * 5;
					player->stats["Hitpoints"] += input * 5;
				}
			}
			Util::Clear();
			std::cout << "= Stat Distribution finished = \n"
				<< "Your stats are now:\n"
				<< "STR:" << player->stats["Strength"] << "\n"
				<< "DEX:" << player->stats["Dexterity"] << "\n"
				<< "VIT:" << player->stats["Vitality"] << "\n"
				<< "MaxHP:" << player->stats["MaxHitpoints"] << "\n"
				<< "HP:" << player->stats["Hitpoints"] << "\n"
				<< "CBT:" << player->stats["Combat"] << "\n"
				<< "INI:" << player->stats["Initiative"] << "\n"
				<< "Undistributed:" << statPoints << "\n";

			acpt = Util::BoolPrompt("Accept? ");
		} while (acpt != true);

		//Weapon
		acpt = false;
		do
		{
			Weapon* ladle = new Weapon{ "Ladle", 1, 1, 1 };
			Weapon* poison = new Weapon{ "The Greatweapon", 2, 3, 3 };
			Weapon* hero = new Weapon{ "AngelHalo", 6, 6, 6 };

			Util::Clear();
			std::cout << "= Choose a Weapon for your journey =\n"
				<< "[0] " << ladle->name << " - Deal " << ladle->diceAmount << "d6+" << ladle->damageMod << "Dmg +" << ladle->combatMod << "CBT 'If you want a bit of a challenge' \n"
				<< "[1] " << poison->name << " - Deal " << poison->diceAmount << "d6+" << poison->damageMod << "Dmg +" << poison->combatMod << "CBT 'Great is the weapon, that cuts on it's own' \n"
				<< "[2] " << hero->name << " - Deal " << hero->diceAmount << "d6+" << hero->damageMod << "Dmg +" << hero->combatMod << "CBT 'Truely, a weapon fit for a hero' \n";
			int wpn = 0;
			wpn = Util::IntPrompt(2, "Choose a Weapon ");

			switch (wpn)
			{
			case 0:
				player->weapon = ladle;
				break;
			case 1:
				player->weapon = poison;
				break;
			case 2:
				player->weapon = hero;
				break;
			default:
				break;
			}
			std::cout << "You chose: " << player->weapon->name << "\n";

			acpt = Util::BoolPrompt("Accept? ");
		} while (acpt != true);

		//Confirmation
		Util::Clear();
		std::cout << "= Character Creation Finished = \n"
			<< "Name: " << player->name << "\n"
			<< "STR: " << player->stats["Strength"] << "\n"
			<< "DEX: " << player->stats["Dexterity"] << "\n"
			<< "VIT: " << player->stats["Vitality"] << "\n"
			<< "MaxHP: " << player->stats["MaxHitpoints"] << "\n"
			<< "HP: " << player->stats["Hitpoints"] << "\n"
			<< "CBT: " << player->stats["Combat"] << "\n"
			<< "INI: " << player->stats["Initiative"] << "\n"
			<< "Weapon: " << player->weapon->name << "\n";
		charFinished = Util::BoolPrompt("Accept and start your journey? ");
	} while (charFinished != true);

	//Add all Combat Maneuvers to the player, because he is a hero
	player->combatManeuvers.push_back(CombatManeuvers::PowerStrike);
	player->combatManeuvers.push_back(CombatManeuvers::SweepingStrike);
	player->isPlayer = true;

	return player;
};

CombatManeuvers PlayerInputComponent::ChooseManeuver(FighterComponent* fighter)
{
	//Choose from one of the 3 options
	std::cout << " Choose your move: \n";
	std::cout << " [0] Basic Attack \n"
		<< " [1] Power Strike \n"
		<< " [2] Sweeping Strike \n";
	int choice = Util::IntPrompt(2, "Choose ");
	return fighter->combatManeuvers[choice];
}

int PlayerInputComponent::TargetDesignation(std::vector<FighterComponent*> fighters)
{
	//Find all potential targets
	std::vector<int> potTargets;
	for (int i = 0; i < fighters.size(); i++)
	{
		if (fighters[i]->isPlayer == false)
		{
			potTargets.push_back(i);
		}
	}
	//Give the Player a prompt
	std::cout << " Choose an Enemy: \n";
	for (int i = 0; i < potTargets.size(); i++)
	{
		std::cout << " [" << i << "] " << fighters[potTargets[i]]->name << "\n";
	}
	int choice = Util::IntPrompt(potTargets.size() - 1, "Choose ");
	return potTargets[choice];
}