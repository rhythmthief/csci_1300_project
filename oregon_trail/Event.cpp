#include "Event.h"
#include <iostream>
using namespace std;

Event::Event()
{
	elapsedCounter = 0;
	counter = 0;
}

int Event::getCounter()
{
	return counter;
}

int Event::getType()
{
	return type[elapsedCounter];
}

string Event::getExtra()
{
	return extra[elapsedCounter];
}

string Event::getName()
{
	return name;
}

string Event::getMessage()
{
	return message[elapsedCounter];
}

string Event::getPrompt()
{
	return prompt[elapsedCounter];
}

void Event::setName(string newName)
{
	name = newName;
}

void Event::setAction(int newType, string newPrompt, string newExtra, string newMessage)
{
	//cout << newType << "*" << newPrompt << "*" << newExtra << "*" <<newMessage << endl;
	type[counter] = newType;
	prompt[counter] = newPrompt;
	extra[counter] = newExtra;
	message[counter] = newMessage;

}

void Event::setCounter(bool runtime) //I will need to set the counter from the Trail class
{
	if (runtime == true) //Executes when the game is running
	{
		elapsedCounter++;
		counter--;
	}
	else	//Executes when we are reading the files
	{
		counter++;
	}
}