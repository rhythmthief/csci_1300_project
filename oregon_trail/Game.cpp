// CSCI1300 Fall 2018
// Author: Dennis Windham
// Recitation: 108 -- I. Huang
// Cloud9 Workspace Editor Link: https://ide.c9.io/rhythmthief/csci1300
// Project III

#include "Trail.h"
#include <iostream>
using namespace std;

/*
The core design is rather simple:
 The driver function will keep the game in a loop and prompt for player input, which gets interpreted by a separate function. At any given point the player is either in the overworld section or going through an event
*/

int main()
{
	Trail oregonTrail;
	string input;

	oregonTrail.progressEvent(input); //Showing the title screen at startup.

	while (oregonTrail.getExecutionStatus())
	{

		getline(cin, input);

		oregonTrail.progressEvent(input);
	}

	return 0;
}