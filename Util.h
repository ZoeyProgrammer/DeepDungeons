#pragma once
#include <iostream>
#include <string>

class Util
{
public:
	static int IntPrompt(int maxInt, std::string prompt)
	{
		{
			int input = 0;
			do
			{
				std::cout << prompt << "[0-" << maxInt << "]: ";
				std::cin >> input;

				if (std::cin.fail() || input < 0 || input > maxInt)
				{
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				}
				else
				{
					break;
				}
			} while (true);

			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			return input;
		}
	};

	static std::string StringPrompt(std::string prompt)
	{
		{
			std::string input = "";
			do
			{
				std::cout << prompt;
				std::cin >> input;

				if (std::cin.fail() || input == "")
				{
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				}
				else
				{
					break;
				}
			} while (true);

			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			return input;
		}
	};

	static bool BoolPrompt(std::string prompt)
	{
		{
			char input = NULL;
			do
			{
				std::cout << prompt << "[Y/N]: ";
				std::cin >> input;

				if (std::cin.fail() || input != 'Y' && input != 'N')
				{
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				}
				else
				{
					break;
				}
			} while (true);

			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			if (input == 'N')
				return false;
			else 
				return true;
		}
	};

	static void FakePrompt()
	{
		std::string input = "";
		do
		{
			std::cin >> input;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			break;
		} while (true);
	}

	static void Clear()
	{
		system("CLS");
	};

	static void Pause()
	{
		system("pause");
	};

	static int RNG(int min, int max)
	{
		int dice = 0;
		dice = rand() % (max + 1 - min) + min;
		return dice;
	}

	//This is the only thing blatantly stolen from StackOverflow
	//It's for a feature I thought was neat, so cut me some slack
	static char intToAlphabet(int i)
	{
		static char const remap[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		return i >= 0 && i < sizeof(remap) - 1
			? remap[i]
			: '?';   // or throw, or ...
	}

private:
};