#pragma once
#include "FighterComponent.h"

class IInputComponent
{
public:
	virtual FighterComponent* Create(bool isBoss);
	virtual CombatManeuvers ChooseManeuver(FighterComponent* fighter);
	virtual int TargetDesignation(std::vector<FighterComponent*> fighters);
};