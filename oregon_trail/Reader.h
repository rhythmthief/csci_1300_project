#ifndef READER_H
#define READER_H

#include "Event.h"
using namespace std;

class Reader
{
   public:
	int split(string, char, string[], int);
	int readEvents(Event[]);
	bool readGraphics(string[][7]);
	bool readMilestones(string[], int[]);
	void writeScores(string, int, float, int, int, int, int);
};

#endif