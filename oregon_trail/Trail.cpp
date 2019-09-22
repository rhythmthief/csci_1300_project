#include "Trail.h"
#include "Calendar.h"
#include "Character.h"
#include "Event.h"
#include "Inventory.h"
#include "Reader.h"
#include <cmath>
#include <iomanip>
#include <iostream>
using namespace std;

//Making sure an in-action input is a number
bool validateActionInput(string input)
{
	bool validInput = true;
	if (input != "")
	{
		for (int i = 0; i < input.size(); i++)
		{
			if (!isdigit(input[i]))
			{
				validInput = false;
				cout << "Invalid input. Enter a positive integer." << endl;
				break;
			}
		}
	}
	else
	{
		validInput = false;
	}
	return validInput;
}

//Default constructor
Trail::Trail()
{
	for (int i = 0; i < 15; i++)
	{
		activePrompts[i] = -1;

		if (i < 10)
		{
			renderBooleans[i] = false;
			shopPurchases[i] = 0;
		}

		if (i < 6)
		{

			characterStatus[i] = 0;
		}
	}

	characterDead = 0;

	promptBoolean = true; //Set to true to pass the very first name input
	characterCount = 0;   //Starts from 0
	actionComplete = false;
	actionStarted = false;
	errorMessage = "";
	renderMessage = "";

	restDuration = 0;
	milestoneId = 0;
	milestoneId = 0;
	milesTraveled = 0;
	eventCount = -1;
	currentEventId = 0;

	//Reading the files
	executionStatus = fileReader.readGraphics(graphics);
	executionStatus = 	fileReader.readMilestones(milestoneNames, milestoneDistance);
	eventCount = fileReader.readEvents(gameEvents);

	if (eventCount == -1)
	{
		executionStatus = false;
	}

}

//Used by the loop in the main file to keep going or stop execution
bool Trail::getExecutionStatus()
{
	return executionStatus;
}

bool Trail::verifyInput(string input)
{
	/*
		Verifies that a player input matches an active prompt or is otherwise valid.
	*/

	bool exists = true;
	string tempString = "";

	if (stoi(gameEvents[currentEventId].getPrompt()) != 0) //Not in skip mode
	{
		exists = false;
		if (input != "")
		{
			if (stoi(gameEvents[currentEventId].getPrompt()) == 3) //Free response
			{
				if (input.size() > 0 && input.size() < 8)
				{
					exists = true;
				}
				else
				{
					errorMessage = errorMessage + "Expecting string length of at least 1 and at most 7. ";
				}
			}
			else
			{
				for (int i = 1; i < 15; i++)
				{
					if (activePrompts[i] != -1)
					{
						tempString = "" + to_string(i); //Changes this to compare against i, the index of a prompt

						if (input == tempString)
						{
							exists = true;
							break;
						}
					}
				}
			}
		}
	}

	if (exists == false)
	{
		errorMessage = errorMessage + "Ivalid input, please try again. ";
	}

	return exists;
}

void Trail::assignPrompts()
{
	/*
		Assigns the in-game prompts for the player to use.
		The prompts are registered in the order of their appearance.
	*/

	int promptInt;

	string splitPrompt[10]; //Unlikely to exceed 10
	int splitCount;
	string tempString;
	string promptString = gameEvents[currentEventId].getPrompt();

	for (int i = 0; i < 15; i++)
	{
		activePrompts[i] = -1;
	}

	if (promptString != "0" && promptString != "3")
	{
		splitCount = fileReader.split(promptString, ',', splitPrompt, 10);

		for (int i = 0; i < splitCount; i++)
		{
			promptInt = stoi(splitPrompt[i]);

			if (i < 15)
			{
				activePrompts[i + 1] = promptInt;
			}
		}
	}
}

void Trail::progressEvent(string input)
{
	/*
		Responsible for all of the game progression
	*/

	if (verifyInput(input))
	{
		switch (gameEvents[currentEventId].getType())
		{
			case 0: //No action
			{
				actionNone();
				break;
			}

			case 1: //Assign characters
			{
				actionCreateCharacter(input);
				break;
			}
			case 2: //Edit resources
			{
				actionEditResource();
				break;
			}
			case 3: //Shop
			{
				actionShop(input);
				break;
			}
			case 4:
			{
				actionSetDeparture(input);
				break;
			}
			case 5:
			{
				actionMove(input);
				break;
			}
			case 6:
			{
				actionEnding();
				break;
			}
			case 7:
			{
				actionRaiders(input);
				break;
			}
		}
	}

	//Primarily used in the opening scene to help it progress
	if (actionComplete == true)
	{
		actionComplete = false;
		gameEvents[currentEventId].setCounter(true);
	}

	//Goes into the overworld once an event is complete
	if (gameEvents[currentEventId].getCounter() == 0)
	{
		renderMessage = "You take a moment to collect your thoughts.";
		currentEventId = 1;
	}

	assignPrompts();

	if (currentEventId != 2)
		renderMessage = gameEvents[currentEventId].getMessage();

	if (gameEvents[currentEventId].getType() == 3 && promptBoolean == false) //I need a special override for the shop to ensure consistency between the rendered screen and the prompts. It's essentially skipping one rendering cycle here.
	{
		progressEvent("5");
	}
	renderScreen();
}

//Game endings
void Trail::actionEnding()
{
	switch (endingValue)
	{
		case 0: //No oxen left.
		{
			renderMessage = "Alas, you could not withstand the challenges of the new world. Your oxen are dead, and so is your entire travelling party.\nGAME OVER";
			break;
		}
		case 1:
		{
			renderMessage = "As you are taking your last breath, you realize that nothing in your life has ever made less sense than this tragic moment.\nGAME OVER";
			break;
		}
		case 2:
		{
			renderMessage = "You have reached Oregon City, and it is unlike anything you could have imagined. They call this place the \" The Restored United States,\" but you realize that it's more than that. It's a restored civilization, with crystal clear sense of a bright future.\nTHE END";
			break;
		}
		case 3:
		{
			renderMessage = "You haven't been able to reach Oregon City in time before winter. You feel your body going numb as the last of your oxen fades away, and know that you are about to follow it.\n GAME OVER";
			break;
		}
		case 4:
		{
			renderMessage = "You've neglected your wagon for the last time. As its wheels fall apart, clearly beyond repair in this harsh environment, you hear the howls of a pack of hungry wolves.\n GAME OVER";
			break;
		}
		case 5:
		{
			renderMessage = "Devoid of hope, what remains of your travelling party decides to talk a long break. Goodbye.";
			break;
		}
	}
	//Writes the result file after the game's completion
	fileReader.writeScores(gameCharacter[0].getName(), milesTraveled, gameInventory.getMoney(), gameInventory.getOxen(), gameInventory.getBullets(), gameInventory.getFood(), characterDead);
	executionStatus = false;
}

void Trail::actionMove(string input)
{
	//Trail progression

	string restInput;
	bool validInput = true;
	int attackChance;

	//bool validInput = false;

	char lowercaseChar;
	string lowercaseString = "";

	checkStatus(); //Running a status check to kill off characters and end the game

	if (restDuration == 0)
	{
		restDuration = rand() % 3 + 1;
	}

	switch (activePrompts[stoi(input)])
	{
		case 12: //Moving forward
		{
			if (activePrompts[stoi(input)] == 12 && gameInventory.getOxen() > 0)
			{
				milesTraveled += rand() % 100 + 35;
				gameCalendar.editDate(rand() % 11 + 7);
				gameInventory.setWagon(gameInventory.getWagon() - (rand() % 20 + 5)); //The wagon gradually deteriorates
				restDuration = 0;

				if (milestoneDistance[milestoneId] - milesTraveled < 10)
				{
					errorMessage = "You've reached " + milestoneNames[milestoneId];
					currentEventId = milestoneId + 4;
					milesTraveled = milestoneDistance[milestoneId]+1;
					milestoneId++;
					break;
				}

				for (int i = 0; i < characterCount + 1; i++)
				{
					if (characterStatus[i] == true)
					{
						if (gameInventory.getFood() - 3 > 0)
						{
							gameInventory.setFood(gameInventory.getFood() - 3);
						}
						else
						{
							gameInventory.setFood(0);
						}
						gameCharacter[i].setFatigue(gameCharacter[i].getFatigue() + (rand() % 25 + 10));
					}
				}

				attackChance = 10 * (((pow((milesTraveled / 100 - 4), 2) + 72) / (pow((milesTraveled / 100 - 4), 2) + 12)) - 1); //Intentionally stores the probability as an integer. It sacrifices some accuracy for easier use later

				//Raider attack, takes place after a move
				if (rand() % 100 + 1 < attackChance)
				{
					currentEventId = 3;
					renderMessage = gameEvents[currentEventId].getMessage();
				}
			}
			break;
		}
		case 13: //Resting
		{
			while (true)
			{
				cout << "You will be resting for " << restDuration << " days in this location. Your wagon will be automatically repaired with available parts. Characters will use medkits to treat their wounds and diseases. How many pounds of food per person per day would you like to ration? Enter \"Cancel\" to cancel the rest." << endl;

				getline(cin, restInput);

				validInput = validateActionInput(restInput);

				if (validInput == true)
				{
					if (stoi(restInput) > 0)
					{
						if (stoi(restInput) * restDuration * (characterCount - characterDead) <= gameInventory.getFood())
						{
							gameInventory.setFood(gameInventory.getFood() - stoi(restInput) * restDuration * (1 + characterCount - characterDead));
							gameCalendar.editDate(restDuration);

							//Automatically repairing the wagon
							while (gameInventory.getParts() > 0 && gameInventory.getWagon() < 100)
							{
								gameInventory.setParts(gameInventory.getParts() - 1);
								gameInventory.setWagon(gameInventory.getWagon() + 20);

								if (gameInventory.getWagon() > 100)
								{
									gameInventory.setWagon(100);
								}
							}

							for (int i = 0; i < characterCount + 1; i++)
							{

								if (characterStatus[i] == true)
								{
									//Automatically applying medkits
									if (gameCharacter[i].getDisease() == 1 && gameInventory.getMedkits() > 0)
									{
										gameInventory.setMedkits(gameInventory.getMedkits() - 1);
										gameCharacter[i].setDisease(0);
									}

									//Removing fatigue
									if (gameCharacter[i].getFatigue() - restDuration * stoi(restInput) * 2 > 0)
									{
										gameCharacter[i].setFatigue(gameCharacter[i].getFatigue() - restDuration * stoi(restInput) * 2);
									}
									else
									{
										gameCharacter[i].setFatigue(0);
									}
								}
							}

							restDuration = 0;
							break;
						}
						else
						{
							cout << "You don't have enough food." << endl;
						}
					}
					else
					{
						cout << "You need to enter a positive integer.";
					}
				}
				else
				{
					for (int i = 0; i < restInput.size(); i++)
					{
						lowercaseChar = tolower(restInput[i]);
						lowercaseString = lowercaseString + lowercaseChar;
					}

					if (lowercaseString == "cancel")
					{
						break;
					}
					else
					{
						cout << "Invalid input. Enter a positive integer or \"Cancel\"." << endl;
					}

					lowercaseString = "";
				}
			}
			break;
		}

		case 14: //Hunting
		{
			int animals[5];
			int huntYield = 0;
			int bulletsSpent = 0;
			int animalCount = 0;
			string huntInput;
			bool huntAccepted = false;
			string huntString = "You have encountered";

			animals[0] = rand() % 2;
			animals[1] = rand() % 4;
			animals[2] = rand() % 7; //Yields a slightly smaller change of 14.28%, which is more than reasonable
			animals[3] = rand() % 14;
			animals[4] = rand() % 20;

			for (int i = 0; i < 5; i++)
			{
				if (animals[i] == 0)
				{
					switch (i)
					{
						case 0:
						{
							if (animalCount == 0)
							{
								huntString = huntString + " a rabbit";
							}
							else
							{
								huntString = huntString + ", a rabbit";
							}
							huntYield += 2;
							bulletsSpent += 10;

							break;
						}
						case 1:
						{
							if (animalCount == 0)
							{
								huntString = huntString + " a fox";
							}
							else
							{
								huntString = huntString + ", a fox";
							}
							huntYield += 5;
							bulletsSpent += 8;

							break;
						}
						case 2:
						{
							if (animalCount == 0)
							{
								huntString = huntString + " a deer";
							}
							else
							{
								huntString = huntString + ", a deer";
							}
							huntYield += rand() % 55 + 30;
							bulletsSpent += 5;

							break;
						}
						case 3:
						{
							if (animalCount == 0)
							{
								huntString = huntString + " a bear";
							}
							else
							{
								huntString = huntString + ", a bear";
							}
							huntYield += rand() % 350 + 100;
							bulletsSpent += 10;

							break;
						}
						case 4:
						{
							if (animalCount == 0)
							{
								huntString = huntString + " a moose";
							}
							else
							{
								huntString = huntString + ", a moose";
							}
							huntYield += rand() % 600 + 300;
							bulletsSpent += 12;

							break;
						}
					}
					animalCount++;
				}
			}
			if (animalCount > 0)
			{
				cout << huntString << "!" << endl;
			}
			else
			{
				//cout << huntString << "no animals." << endl;
				errorMessage = huntString + " no animals.";
			}

			//Days spent hunting will not consume food, helps counteract new mechanics.

			//We will be hunting all animals at once! The RNG is already quite punishing and it would be unfair to limit the player to a single prey.
			if (animalCount > 0)
			{
				cout << "Would you like to hunt the said animals?\n1. Yes.\n2. No." << endl;

				while (true)
				{
					getline(cin, huntInput);
					validInput = validateActionInput(huntInput);

					if (validInput == true)
					{
						if (stoi(huntInput) == 1)
						{
							huntAccepted = true;
							break;
						}
						else
						{
							if (stoi(huntInput) == 2)
							{
								huntAccepted = false;
								break;
							}
						}
					}
					else
					{
						cout << "Invalid input. Please enter a positive integer between 1 and 2." << endl;
					}
				}

				if (huntAccepted == true)
				{
					if (gameInventory.getBullets() > 10)
					{
						if (actionPuzzle() == true)
						{
							errorMessage = "The animal you've just shot lies in the grass, lifeless. Its meat will help you push further into the unknown.";

							if (gameInventory.getFood() + huntYield > 1300)
							{
								gameInventory.setFood(1300);
								errorMessage = errorMessage + " Some of the meat you've poached won't fit into your wagon. Your party has consumed it.";

								//Removing a predetermined amount of fatigue regardless of the excess catch
								for (int i = 0; i < characterCount + 1; i++)
								{
									if (characterStatus[i] == true)
									{
										if (gameCharacter[i].getFatigue() - 30 > 0)
										{
											gameCharacter[i].setFatigue(gameCharacter[i].getFatigue() - 30);
										}
										else
										{
											gameCharacter[i].setFatigue(0);
										}
									}
								}
							}
							else
							{
								gameInventory.setFood(gameInventory.getFood() + huntYield);
							}
						}
						else
						{
							errorMessage = "Your prey has escaped.";
						}
						if (gameInventory.getBullets() - bulletsSpent > 0)
						{
							gameInventory.setBullets(gameInventory.getBullets() - bulletsSpent);
						}
						else
						{
							gameInventory.setBullets(0);
						}
					}
					else
					{
						gameInventory.setBullets(0);
						errorMessage = "You run out of bullets and the animals escape.";
					}
				}
			}

			gameCalendar.editDate(1); //Adds one day to the counter, we've spent it hunting

			break;
		}

		case 18: //Quitting the game
		{
			endingValue = 5;
			currentEventId = 2;
			break;
		}
	}


	checkStatus(); //Running a status check again to account for new changes
}

//Responsible for raider attacks
void Trail::actionRaiders(string input)
{
	switch (activePrompts[stoi(input)])
	{
		case 15:
		{
			errorMessage = "You escape with your life, but not without losses. You've had to leave behind some of your possessions: 1 ox,";

			gameInventory.setOxen(gameInventory.getOxen() - 1);

			if (gameInventory.getFood() > 9)
			{
				errorMessage = errorMessage + " 10 food";
				gameInventory.setFood(gameInventory.getFood() - 10);
			}
			else
			{
				errorMessage = errorMessage + " remaining food";

				gameInventory.setFood(0);
			}

			if (gameInventory.getParts() > 0)
			{
				errorMessage = errorMessage + " one part";

				gameInventory.setParts(gameInventory.getParts() - 1);
			}
			errorMessage = errorMessage + ".";

			break;
		}
		case 16:
		{
			//One bullet is enough
			if (gameInventory.getBullets() > 1)
			{
				if (actionPuzzle() == true)
				{
					errorMessage = "The savages didn't expect to run into the best shot of St. Louis. The hunter becomes the hunted, you take 75lbs of their own food and some bullets.";

					if (gameInventory.getFood() + 75 > 1300)
					{
						gameInventory.setFood(1300);
						errorMessage = errorMessage + " Some of the food you've retrieved won't fit into your caravan, therefore your party consumes it.";

						//Removing a predetermined amount of fatigue regardless of the excess catch
						for (int i = 0; i < characterCount + 1; i++)
						{
							if (characterStatus[i] == true)
							{
								if (gameCharacter[i].getFatigue() - 30 > 0)
								{
									gameCharacter[i].setFatigue(gameCharacter[i].getFatigue() - 30);
								}
								else
								{
									gameCharacter[i].setFatigue(0);
								}
							}
						}
					}
					else
					{
						gameInventory.setFood(gameInventory.getFood() + 75);
					}

					if (gameInventory.getBullets() + 75 < 250)
					{
						gameInventory.setBullets(gameInventory.getBullets() + 75);
					}
					else
					{
						gameInventory.setBullets(250);
					}
				}
				else
				{
					errorMessage = "You couldn't fend off the raiders. They've taken a quarter of your money and some bullets.";
					gameInventory.setMoney(gameInventory.getMoney() * 3 / 4);

					if (gameInventory.getBullets() > 50)
					{
						gameInventory.setBullets(gameInventory.getBullets() - 50);
					}
					else
					{
						gameInventory.setBullets(0);
					}
				}
			}

			break;
		}
		case 17:
		{
			errorMessage = "You surrender to the raiders, they take a quarter of your money and leave you be, perhaps as a token of good will.";
			gameInventory.setMoney(gameInventory.getMoney() * 3 / 4);
			break;
		}
	}

	currentEventId = 1;
}


//The in-game puzzle where a player has to guess a number
bool Trail::actionPuzzle()
{
	int number = rand() % 10 + 1;
	string puzzleInput;
	int attemptsMade = 1;
	bool validInput = false;
	bool success = false;

	cout << "You are taking aim. Enter an integer between 1 and 10 to hit the mark." << endl;
	while (true)
	{

		getline(cin, puzzleInput);
		validInput = validateActionInput(puzzleInput);

		if (attemptsMade > 2)
		{
			break;
		}

		if (validInput == true)
		{
			if (stoi(puzzleInput) != number)
			{
				if (stoi(puzzleInput) > number)
				{
					cout << "You've missed. Try aiming lower." << endl;
				}
				else
				{
					cout << "Miss! Try aiming higher." << endl;
				}
			}
			else
			{
				success = true;
				break;
			}
			attemptsMade++;
		}
		else
		{
			cout << "Invalid input. Please enter an integer between 1 and 10." << endl;
		}
	}
	return success;
}

//Checks all statuses and conditions
void Trail::checkStatus()
{
	int tempInt;

	//Check characters
	for (int i = 0; i < characterCount + 1; i++)
	{
		if (characterStatus[i] == true)
		{
			//if (characterCount != characterDead)
			//{
			if (gameInventory.getFood() / (characterCount + 1 - characterDead) < 100) //Low food
			{
				if ((rand() % 8 + 0) == 8) //Rolling 8 infects the character
				{
					gameCharacter[i].setDisease(1);
					errorMessage = gameCharacter[i].getName() + " got sick.";
				}
			}
			//}

			if (gameCharacter[i].getDisease() == 1)
			{
				gameCharacter[i].setHealth(gameCharacter[i].getHealth() - (rand() % 25 + 5));
			}

			if (gameInventory.getFood() == 0)
			{
				tempInt = rand() % 70 + 45;

				gameCharacter[i].setHealth(gameCharacter[i].getHealth() - tempInt);
			}

			if (gameCharacter[i].getHealth() <= 0 && characterStatus[i] == true)
			{
				gameCharacter[i].setHealth(0);
				characterDead++;
				characterStatus[i] = false;
				errorMessage = "Alas, " + gameCharacter[i].getName() + " has perished.";
			}
			if (gameCharacter[i].getFatigue() >= 100 && characterStatus[i] == true)
			{
				gameCharacter[i].setFatigue(100);
				characterDead++;
				characterStatus[i] = false;
				errorMessage = "Alas, " + gameCharacter[i].getName() + " has perished.";
			}
		}
	}

	if (gameInventory.getOxen() == 0)
	{
		endingValue = 0;
		currentEventId = 2;
	}

	if (characterStatus[0] == false)
	{
		endingValue = 1;
		currentEventId = 2;
	}

	if (milesTraveled > 2039)
	{
		endingValue = 2;
		currentEventId = 2;
	}

	if (gameCalendar.getMonth() > 7)
	{
		endingValue = 3;
		currentEventId = 2;
	}
	if (gameInventory.getWagon() < 1)
	{
		endingValue = 4;
		currentEventId = 2;
	}
}

//Used to display dialogue
void Trail::actionNone()
{
	actionComplete = true;
}

//Responsible for character creation
void Trail::actionCreateCharacter(string input)
{
	if (promptBoolean == true) //Initializes a character
	{
		gameCharacter[characterCount].setName(input);
		gameCharacter[characterCount].setHealth(100);
		characterStatus[characterCount] = true;
		promptBoolean = false;

		if (renderBooleans[0] == false)
			renderBooleans[0] = true;
	}
	else //Not in name input mode yet
	{
		if (activePrompts[stoi(input)] == 1) //Saying "yes" to taking a family member
		{
			promptBoolean = true;
			characterCount++;
		}
		else //Saying "no"
		{
			gameEvents[currentEventId].setCounter(true);
		}
	}
	actionComplete = true;
}

//Was meant to be extensively used by in-game events, but the game didn't scale as much as I expected.
void Trail::actionEditResource()
{
	string extra = gameEvents[currentEventId].getExtra();
	int resource = stoi(extra.substr(0, 1));
	int quantity = stoi(extra.substr(1, extra.size() - 1));

	if (renderBooleans[2] == false)
		renderBooleans[2] = true;

	switch (resource)
	{
		case 0: //Wagon
		{
			if (renderBooleans[1] == false)
				renderBooleans[1] = true;

			gameInventory.setWagon(gameInventory.getWagon() + quantity);
			break;
		}

		case 1: //Oxen
		{
			gameInventory.setOxen(gameInventory.getOxen() + quantity);
			break;
		}

		case 2: //Money
		{
			gameInventory.setMoney(gameInventory.getMoney() + quantity);
			break;
		}

		case 3: //Food
		{
			gameInventory.setFood(gameInventory.getFood() + quantity);
			break;
		}

		case 4: //Bullets
		{
			gameInventory.setBullets(gameInventory.getBullets() + quantity);
			break;
		}

		case 5: //Medkits
		{
			gameInventory.setMedkits(gameInventory.getMedkits() + quantity);
			break;
		}

		case 6: //Parts
		{
			gameInventory.setParts(gameInventory.getParts() + quantity);
		}
	}
	actionComplete = true;
}

void Trail::actionShop(string input)
{
	/*
		The in-game shop
	*/

	shopMultiplier = stof(gameEvents[currentEventId].getExtra());

	if (promptBoolean == false)
	{
		promptBoolean = true;
		renderBooleans[3] = true; //Showing the shop menu
	}
	else
	{
		int inputInt = activePrompts[stoi(input)];
		int selection;
		int quantity;
		string quantityInput;
		bool validInput = true;

		if (inputInt < 9)
		{
			while (true)
			{
				cout << "How many would you like to buy?" << endl;

				getline(cin, quantityInput);

				validInput = validateActionInput(quantityInput);

				if (validInput == true)
				{
					quantity = stoi(quantityInput);
					break;
				}
				else
				{
					validInput = true;
				}
			}
		}

		//Prompts
		switch (inputInt)
		{
			case 4: //Oxen
			{
				shopPurchases[0] += 120 * shopMultiplier * quantity;
				shopPurchases[1] += quantity;
				break;
			}
			case 5: //Food
			{
				shopPurchases[0] += 0.5 * shopMultiplier * quantity;
				shopPurchases[2] += quantity;
				break;
			}
			case 6: //Bullets
			{
				shopPurchases[0] += 2 * shopMultiplier * quantity;
				shopPurchases[3] += quantity;
				break;
			}
			case 7: //Medkits
			{
				shopPurchases[0] += 25 * shopMultiplier * quantity;
				shopPurchases[4] += quantity;
				break;
			}
			case 8: //Parts
			{
				shopPurchases[0] += 20 * shopMultiplier * quantity;
				shopPurchases[5] += quantity;
				break;
			}
			case 9: //Reset
			{
				for (int i = 0; i < 10; i++)
				{
					shopPurchases[i] = 0;
				}

				renderBooleans[4] = false;

				break;
			}
			case 10: //Complete transaction
			{
				if (gameInventory.getOxen() + shopPurchases[1] == 0 && gameInventory.getMoney() - shopPurchases[0] < 120)
				{
					errorMessage = "\"If you do that, you won't have enough money to buy an ox,\" says the shopkeeper.";
				}
				else
				{
					if (gameInventory.getMoney() - shopPurchases[0] >= 0)
					{
						if (gameInventory.getOxen() + shopPurchases[1] < 3 & gameInventory.getFood() + shopPurchases[2] < 1301 & gameInventory.getBullets() + 20 * shopPurchases[3] < 251 & gameInventory.getMedkits() + shopPurchases[4] < 11 && gameInventory.getParts() + shopPurchases[5] < 16)
						{
							gameInventory.setMoney(gameInventory.getMoney() - shopPurchases[0]);
							gameInventory.setOxen(gameInventory.getOxen() + shopPurchases[1]);
							gameInventory.setFood(gameInventory.getFood() + shopPurchases[2]);
							gameInventory.setBullets(gameInventory.getBullets() + 20 * shopPurchases[3]);
							gameInventory.setMedkits(gameInventory.getMedkits() + shopPurchases[4]);
							gameInventory.setParts(gameInventory.getParts() + shopPurchases[5]);

							renderBooleans[4] = false;

							for (int i = 0; i < 10; i++)
							{
								shopPurchases[i] = 0;
							}
						}
						else
						{
							errorMessage = "You are about to exceed your carrying capacity. The wagon can only accommodate 2 oxen, 1300lbs of food, 250 bullets, 10 medkits and 15 parts.";
						}
					}
					else
					{
						errorMessage = "You don't have enough money.";
					}
				}

				break;
			}
			case 11: //Leave
			{
				if (gameInventory.getOxen() == 0)
				{
					errorMessage = "You forgot to buy oxen for your trip.";
				}
				else
				{
					for (int i = 0; i < 10; i++)
					{
						shopPurchases[i] = 0;
					}
					actionComplete = true;
					promptBoolean = false;
					renderBooleans[3] = false;
					renderBooleans[4] = false;
				}

				break;
			}
		}

		if (renderBooleans[4] == false && shopPurchases[0] > 0)
		{
			renderBooleans[4] = true; //Showing the bill
		}
	}
}

//Allows to enter a custom departure date
void Trail::actionSetDeparture(string input)
{
	renderBooleans[5] = true;
	bool validInput = true;
	bool calendarState = false; //False assigns the month, true -- the date
	string dateInput;

	if (activePrompts[stoi(input)] == 1) //Using the default departure date
	{
		//Set calendar to march 28th
		gameCalendar.setMonthNumber(0);
		gameCalendar.setDay(28);
		actionComplete = true;
	}
	else //Using a custom departure date
	{
		while (true)
		{
			if (calendarState == false)
			{
				cout << "Enter the number of the new departure month between 3 and 5 (March, April or May)." << endl;
			}
			else
			{
				cout << "Enter the departure day." << endl;
			}

			getline(cin, dateInput);

			validInput = validateActionInput(dateInput);

			if (validInput == true)
			{
				if (calendarState == false) //Assigns a month
				{
					if (gameCalendar.setMonthNumber(stoi(dateInput) - 3))
					{
						calendarState = true;
					}
				}
				else //Assigns a day
				{
					if (gameCalendar.setDay(stoi(dateInput)))
					{
						break;
					}
				}
			}
			else
			{
				cout << "Invalid input. Enter a positive integer." << endl;
				validInput = true;
			}
		}
	}

	actionComplete = true;
}

void Trail::renderScreen()
{
	string diseaseTemp;
	string promptTemp;

	/*
		Render booleans: 
						0 -- Character Section
						1 -- Caravan Section
						2 -- Inventory Section
						3 -- Shop Section
						4 -- Shop bill
						5 -- Road status
	*/

	cout << "\033[2J\033[1;1H"; //A magic combination for clearing the screen

	//Character Section
	if (renderBooleans[0] == true)
	{
		cout << "------------------Family Members-----------------" << endl;
		cout << "Name\tHealth\tFatigue\tDisease\tStatus" << endl;
		for (int i = 0; i < 6; i++)
		{
			if (characterStatus[i] == true)
			{
				cout << gameCharacter[i].getName() << "\t" << gameCharacter[i].getHealth() << "\t" << gameCharacter[i].getFatigue() << "\t";

				switch (gameCharacter[i].getDisease())
				{
					case 0:
					{
						diseaseTemp = "Healthy";
						break;
					}
					case 1:
					{
						diseaseTemp = "Sick";
						break;
					}
				}
				cout << diseaseTemp << "\t";

				if (gameCharacter[i].getHealth() > 0)
				{
					cout << "Alive" << endl;
				}
				else
				{
					cout << "Dead" << endl;
				}
			}
		}
	}

	//Caravan Section
	if (renderBooleans[1] == true)
	{
		/*
			The caravan section occupies 7 lines
		*/

		cout << "------------------Caravan Status-----------------" << endl;

		for (int i = 0; i < 7; i++)
		{
			if (gameInventory.getOxen() > 0)
			{
				if (gameInventory.getOxen() == 1)
				{
					cout << graphics[0][i];
					if (i == 1)
					{
						cout << "Integrity: " << gameInventory.getWagon() << "  ";
						if (gameInventory.getWagon() < 100)
						{
							cout << " ";
						}
						if (gameInventory.getWagon() < 10)
						{
							cout << " ";
						}
					}

					if (i == 2)
					{
						cout << "Oxen: " << gameInventory.getOxen() << "    ";
					}
				}
				else
				{
					cout << graphics[1][i];

					if (i == 1)
					{
						cout << "Integrity: " << gameInventory.getWagon() << "";
						if (gameInventory.getWagon() < 100)
						{
							cout << " ";
						}
						if (gameInventory.getWagon() < 10)
						{
							cout << " ";
						}
					}

					if (i == 2)
					{
						cout << "Oxen: " << gameInventory.getOxen() << "  ";
					}
				}
			}
			else
			{
				if (i == 1)
				{
					cout << "     ";
				}
			}

			if (gameInventory.getWagon() > 0)
			{
				cout << graphics[2][i];
			}

			cout << endl;
		}
	}

	//Inventory Section
	if (renderBooleans[2] == true)
	{
		cout << "------------------Inventory----------------------" << endl;
		cout << "Money\tFood\tBullets\tMedkits\tParts" << endl;
		cout << "$" << gameInventory.getMoney() << "\t" << gameInventory.getFood() << "lbs\t" << gameInventory.getBullets() << "\t" << gameInventory.getMedkits() << "\t" << gameInventory.getParts() << endl;
	}

	//Shop Section
	if (renderBooleans[3] == true)
	{
		cout << "------------------General Store------------------" << endl;
		cout << "Oxen\tFood\tBullets\tMedkits\tParts" << endl;

		cout << "$" << 120 * shopMultiplier << "\t$" << 0.5 * shopMultiplier << "\t" << 20 << "/$" << 2 * shopMultiplier << "\t$" << 25 * shopMultiplier << "\t$" << 20 * shopMultiplier << endl;
	}

	//Shop Bill section
	if (renderBooleans[4] == true)
	{
		cout << "----Current Bill----" << endl;
		cout << "Item\tQty\tPrice" << endl;

		if (shopPurchases[1] > 0)
		{
			cout << "Oxen\t" << shopPurchases[1] << "\t$" << shopPurchases[1] * 120 * shopMultiplier << endl;
		}

		if (shopPurchases[2] > 0)
		{
			cout << "Food\t" << shopPurchases[2] << "\t$" << shopPurchases[2] * 0.5 * shopMultiplier << endl;
		}

		if (shopPurchases[3] > 0)
		{
			cout << "Bullets\t" << shopPurchases[3] * 20 << "\t$" << shopPurchases[3] * 1 * shopMultiplier << endl;
		}

		if (shopPurchases[4] > 0)
		{
			cout << "Medkits\t" << shopPurchases[4] << "\t$" << shopPurchases[4] * 25 * shopMultiplier << endl;
		}

		if (shopPurchases[5] > 0)
		{
			cout << "Parts\t" << shopPurchases[5] << "\t$" << shopPurchases[5] * 20 * shopMultiplier << endl;
		}

		cout << "Total: $" << shopPurchases[0] << endl;
	}

	if (renderBooleans[5] == true)
	{
		cout << "-------------------Road Status-------------------" << endl;
		cout << "Date\t\tTraveled\t\tMilestone" << endl;
		cout << gameCalendar.getMonth() + 3 << "/" << gameCalendar.getDay() << "/2027\t" << milesTraveled << "mi\t\t\t" << milestoneDistance[milestoneId] - milesTraveled << "mi" << endl;
	}

	//Message section
	cout << endl
		<< renderMessage << endl;

	cout << endl;

	//Hints
	if (currentEventId == 1 && errorMessage == "")
	{
		switch (rand() % 5 + 0)
		{
			case 0:
			{
				errorMessage = "Hint: pay attention to your characters' fatigue levels. If it reaches 100, your character will die of exhaustion. Timely rest and wholesome meals lift the weariness.";
				break;
			}
			case 1:
			{
				errorMessage = "Hint: Your party automatically uses medkits and repairs the wagon when it's resting.";
				break;
			}
		}
	}

	//Displays an extra string, primarily used to handle errors
	if (errorMessage != "")
	{
		cout << errorMessage << endl;
		errorMessage = "";
	}

	if (gameEvents[currentEventId].getPrompt() != "0")
	{
		if (gameEvents[currentEventId].getPrompt() != "3")
		{
			cout << "[Enter a number]" << endl;
			for (int i = 0; i < 10; i++)
			{
				if (activePrompts[i] != -1)
				{
					cout << i << ". ";

					//All available player prompts
					switch (activePrompts[i])
					{
						case 1:
						{
							promptTemp = "Yes.";
							break;
						}
						case 2:
						{
							promptTemp = "No.";
							break;
						}
						case 4:
						{
							promptTemp = "Buy oxen.";
							break;
						}
						case 5:
						{
							promptTemp = "Buy food.";
							break;
						}
						case 6:
						{
							promptTemp = "Buy bullets.";
							break;
						}
						case 7:
						{
							promptTemp = "Buy medkits.";
							break;
						}
						case 8:
						{
							promptTemp = "Buy wagon parts.";
							break;
						}
						case 9:
						{
							promptTemp = "Reset purchases.";
							break;
						}
						case 10:
						{
							promptTemp = "Complete transaction.";
							break;
						}
						case 11:
						{
							promptTemp = "Leave the store.";
							break;
						}
						case 12:
						{
							promptTemp = "Move forward.";
							break;
						}
						case 13:
						{
							promptTemp = "Rest.";
							break;
						}
						case 14:
						{
							promptTemp = "Hunt.";
							break;
						}
						case 15:
						{
							promptTemp = "Run.";
							break;
						}
						case 16:
						{
							promptTemp = "Fight back.";
							break;
						}
						case 17:
						{
							promptTemp = "Surrender.";
							break;
						}
						case 18:
						{
							promptTemp = "Quit the game.";
							break;
						}
					}
					cout << promptTemp << endl;
				}
			}
		}
		else
		{
			cout << "[Enter a string with the length of 0<L<8]" << endl;
		}
	}
	else
	{
		cout << "[Press Enter]" << endl;
	}
}
