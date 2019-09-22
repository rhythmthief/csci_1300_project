#include "Inventory.h"
#include <iostream>
#include <stdlib.h>
using namespace std;

/*
	Game inventory, self-explanatory
*/


Inventory::Inventory()
{
	wagon = 0;
	oxen = 0;
	food = rand()%20;
	bullets = rand()%20;
	money = rand()%100;
	medkits = 0;
	parts = 0;
}

//Mutators
void Inventory::setWagon(int i)
{
	wagon = i;
}

void Inventory::setOxen(int i)
{
	oxen = i;
}
void Inventory::setFood(int i)
{
	food = i;
}
void Inventory::setBullets(int i)
{
	bullets = i;
}
void Inventory::setMoney(float i)
{
	money = i;
}
void Inventory::setMedkits(int i)
{
	medkits = i;
}
void Inventory::setParts(int i)
{
	parts = i;
}

//Accessors
int Inventory::getWagon()
{
	return wagon;
}

int Inventory::getOxen()
{
	return oxen;
}
int Inventory::getFood()
{
	return food;
}
int Inventory::getBullets()
{
	return bullets;
}
float Inventory::getMoney()
{
	return money;
}
int Inventory::getMedkits()
{
	return medkits;
}
int Inventory::getParts()
{
	return parts;
}