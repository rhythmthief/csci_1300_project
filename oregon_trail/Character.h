#ifndef CHARACTER_H
#define CHARACTER_H

#include <iostream>
using namespace std;

class Character
{
   public:
	Character();
	void setName(string);
	void setHealth(int);
	void setFatigue(int);
	void setDisease(int);

	string getName();
	int getHealth();
	int getFatigue();
	int getDisease();

   private:
	string name;
	int health;
	int fatigue;
	int disease;
};
#endif