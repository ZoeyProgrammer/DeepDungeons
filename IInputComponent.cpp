#include "IInputComponent.h"

FighterComponent* IInputComponent::Create(bool isBoss)
{
	return nullptr;
};

CombatManeuvers IInputComponent::ChooseManeuver(FighterComponent* fighter)
{
	return {};
};

int IInputComponent::TargetDesignation(std::vector<FighterComponent*> fighters)
{
	return NULL;
};
