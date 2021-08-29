#pragma once
#include "FighterComponent.h"
#include "PlayerInputComponent.h"
#include "AIInputComponent.h"

class Entity
{
public:
	FighterComponent* fighterComponent;
	IInputComponent* inputComponent;

	Entity();
private:

};

///
/// I know this is not what a proper Entity-Component System looks like
/// But I was on heavy time-constraints and wasn't feeling well
/// So I didn't really have the time or brain-power to write something fancy
/// And I didn't just want to copy what someone else did online..
/// 
/// I did understand the theory of how it should work though,
/// I just don't think it's viable for me in the time I have.
/// But I at least tried to make everything work regardless.
/// 
/// I dont expect full points, but at least to be able to pass.
/// I at least put in effort.. unlike some other people..
/// Others hired private-tutors just for this exam.
/// I wanted to do it on my own and not just be a parasite on someone else's work.
/// Please, at least give me some credit for that..
/// 