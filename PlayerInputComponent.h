#pragma once
#include "IInputComponent.h"

class PlayerInputComponent : public IInputComponent
{
public:
	FighterComponent* Create(bool isBoss);
	CombatManeuvers ChooseManeuver(FighterComponent* fighter) override;
	int TargetDesignation(std::vector<FighterComponent*> fighters) override;

	PlayerInputComponent();

};