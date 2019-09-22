#include "Reader.h"
#include "Event.h"
#include <fstream>
#include <iostream>
using namespace std;

/*
	This class is responsible for reading and writing files
*/

//A split function
int Reader::split(string str, char c, string array[], int size)
{
	if (str.length() == 0)
	{
		return 0;
	}
	string word = "";
	int count = 0;
	str = str + c;
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == c)
		{
			if (word.length() == 0)
				continue;
			array[count++] = word;
			word = "";
		}
		else
		{
			word = word + str[i];
		}
	}
	return count;
}

//Reads milestones from a file at startup
bool Reader::readMilestones(string milestoneNames[], int milestoneDistance[])
{
	bool success = false;
	ifstream milestoneStream;
	milestoneStream.open("milestones.cyb");
	string line;
	string splitLine[2];
	int count = 0;

	if (milestoneStream.is_open())
	{
		success = true;
		while (getline(milestoneStream, line))
		{
			if (line != "")
			{
				split(line, ',', splitLine, 2);
				milestoneNames[count] = splitLine[0];
				milestoneDistance[count] = stoi(splitLine[1]);
				count++;
			}
		}
	}
	else
	{
		cout << "Could not locate milestones.cyb. Please place it next to the executable." << endl;
	}
	return success;
}

//Reads events from a file at startup
int Reader::readEvents(Event gameEvents[])
{
	ifstream eventsStream;
	eventsStream.open("events.cyb");
	string line;
	string splitLine[4];
	int actionCount = 0;
	int eventCount = -1;

	if (eventsStream.is_open())
	{
		while (getline(eventsStream, line))
		{
			if (line != "" && line[0] != '#')
			{
				if (line == "eventStart")
				{
					eventCount++;
					getline(eventsStream, line); //Reading the next line after eventStart -- it contains the event name
					gameEvents[eventCount].setName(line);
				}
				else
				{
					if (line != "eventEnd")
					{
						split(line, '*', splitLine, 4);
						gameEvents[eventCount].setAction(stoi(splitLine[0]), splitLine[1], splitLine[2], splitLine[3]);
						gameEvents[eventCount].setCounter(false);
					}
				}
			}
		}
	}
	else
	{
		cout << "Could not locate events.cyb. Please place it next to the executable." << endl;
	}

	return eventCount;
}

//Reads graphics from a file at startup
bool Reader::readGraphics(string graphics[][7])
{
	bool complete = false;
	ifstream graphicsStream;
	graphicsStream.open("graphics.cyb");
	string line;
	int graphicsCount = 0;
	int lineCount = 0;

	if (graphicsStream.is_open())
	{
		complete = true;
		while (getline(graphicsStream, line))
		{
			if (line != "end")
			{
				graphics[graphicsCount][lineCount] = line;
				lineCount++;
			}
			else
			{
				lineCount = 0;
				graphicsCount++;
			}
		}
	}
	else
	{
		cout << "Could not locate graphics.cyb. Please place it next to the executable." << endl;
	}

	return complete;
}

//Writes scores during the endgame
void Reader::writeScores(string name, int miles, float money, int oxen, int bullets, int food, int dead)
{
	ofstream scoreStream;
	scoreStream.open("results.txt");

	scoreStream << "Name\tTravelled\tMoney\tOxen\tBullets\tFood\tDead" << endl;
	scoreStream << name << "\t" << miles << "mi\t$" << money << "\t\t" << oxen << "\t" << bullets << "\t" << food << "lbs\t" << dead << endl;
}