#ifndef TRAIL_H
#define TRAIL_H

/*
The class with all the game mechanics
*/

//events.cyb action layout: actionID*prompts*text
//actionID determines which direction the response will go
//i prompt stands for free input

#include "Calendar.h"
#include "Character.h"
#include "Event.h"
#include "Inventory.h"
#include "Reader.h"
#include "Trail.h"
#include <fstream>
#include <iostream>
using namespace std;

class Trail
{
   public:
	Trail();
	void renderScreen(); //Handles the visuals of the game
	void assignPrompts();
	void progressEvent(string);
	void checkStatus(); //Checks the party condition

	void actionNone();
	void actionMove(string); //Progressing forward
	bool verifyInput(string);
	void actionCreateCharacter(string);
	bool getExecutionStatus();
	void actionEditResource();
	void actionShop(string); //Shopping
	void actionSetDeparture(string);
	bool actionPuzzle();	   //Solving puzzles
	void actionRaiders(string); //Raiders
	void actionEnding();

   private:
	Inventory gameInventory;
	Calendar gameCalendar;
	Event gameEvents[20];	 //The size might change
	Character gameCharacter[6]; //The size might change
	int characterCount;
	bool characterStatus[6];
	int characterDead;
	int currentEventId;
	int restDuration; //I need to cache the rest duration to prevent intentional abuse.
	Reader fileReader;
	
	int milesTraveled;

	bool promptBoolean;
	bool actionComplete;
	bool actionStarted;
	int eventCount;

	int activePrompts[15]; //Cells in use contain current prompt IDs
	string inputPrompts[10];

	//string inputOptions[10];
	//string inputOptionsText[10]{"Press Enter to continue...", "Yes", "No"};

	string errorMessage;
	string renderMessage;

	int endingValue;

	string milestoneNames[8];
	int milestoneDistance[8];
	int milestoneId;


	bool renderBooleans[10]; //What's the actual size I need?

	float shopPurchases[10];
	float shopMultiplier;

	bool executionStatus;

	string graphics[3][7];

	string waypoints[10];

	int danger; // calculated based on the player's resources and time from the last punishment
};

#endif