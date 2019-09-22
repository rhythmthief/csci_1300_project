#include "Character.h"
#include <iostream>
using namespace std;

Character::Character()
{
	name = "";
	health = 0;
	fatigue = 0;
	disease = 0;
}
//Mutators
void Character::setName(string newName)
{
	name = newName;
}
void Character::setHealth(int i)
{
	health = i;
}
void Character::setFatigue(int i)
{
	fatigue = i;
}
void Character::setDisease(int i)
{
	disease = i; //A character can only have one disease at a time.
}

//Accessors
string Character::getName()
{
	return name;
}

int Character::getHealth()
{
	return health;
}
int Character::getFatigue()
{
	return fatigue;
}
int Character::getDisease()
{
	return disease;
}
