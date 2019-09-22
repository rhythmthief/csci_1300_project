#ifndef EVENT_H
#define EVENT_H

#include <iostream>
using namespace std;

class Event
{
   public:
	Event();
	// Event(int, string, string[], int[], int);

	//I don't need any other setter functions here -- everything gets initialized with the parameterized contstructor and remains untouched.

	int getCounter();
	int getType();
	string getExtra();
	string getName();
	string getMessage();
	string getPrompt();

	void setName(string);
	void setAction(int, string, string, string);
	void setCounter(bool); //I will need to set the counter from the Trail class

   private:
	int counter; //Number of turns remaining for this event to conclude.
	int elapsedCounter;
	string name;
	int type[50];
	string prompt[50];
	string extra[50];
	string message[50]; //50 is the theoretical limit, it is very unlikely that I will exceed it
};

#endif