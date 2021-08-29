#pragma once


enum class GameState
{
	Setup,
	CharCreation,
	CombatSetup,
	Combat,
	Closing
};

class Game
{
public:
	GameState state;

	static Game& Instance()
	{
		static Game instance;
		return instance;
	}
private:
	Game()//Making sure it always starts in Setup-state
	{
		this->state = GameState::Setup;
	}
};
