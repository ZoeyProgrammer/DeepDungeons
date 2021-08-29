// DeepDungeons.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Entity.h"
#include "CombatSystem.h"
#include "Game.h"

int main()
{
  //Pre-Setup
  
  //Creating the GameMaster Entity
  CombatSystem* gameMaster = new CombatSystem();

  //Main Loop
  while (Game::Instance().state != GameState::Closing)
  {
    switch (Game::Instance().state)
    {
    case GameState::Setup:  //In case something needs setup
      srand(time(NULL));
      Game::Instance().state = GameState::CombatSetup;
      break;
    case GameState::CombatSetup:  //Setting everything up for the fight
      gameMaster->CombatStart();
      Game::Instance().state = GameState::Combat;
      break;
    case GameState::Combat: //The main CombatSys Loop
      gameMaster->CombatLoop();
      break;
    default:  //This state should not happen
      break;
    }
  }
  
  //Everything cleans itself up
  delete gameMaster;
  
  return 0;
}
