#pragma once
#include "IInputComponent.h"

class AIInputComponent : public IInputComponent
{
public:
	FighterComponent* Create(bool isBoss) override;
	CombatManeuvers ChooseManeuver(FighterComponent* fighter) override;
	int TargetDesignation(std::vector<FighterComponent*> fighters) override;

	AIInputComponent();

};