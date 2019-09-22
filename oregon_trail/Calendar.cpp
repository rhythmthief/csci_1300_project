#include "Calendar.h"
#include <iostream>
using namespace std;


/*
	This class is responsible for tracking time.
*/

int Calendar::getDay()
{
	return months[monthNumber];
}

int Calendar::getMonth()
{
	return monthNumber;
}

bool Calendar::setMonthNumber(int i)
{
	bool success = true;

	if (i > -1 && i < 3)
	{
		monthNumber = i;
	}
	else
	{
		success = false;
		cout << "Invalid month. You can only depart between March 1st and May 1st." << endl;
	}

	return success;
}

bool Calendar::setDay(int i) //This setter only needs to account for three months as it's being used during the opening scene
{
	bool success = false;

	if (i > 0)
	{
		switch (monthNumber)
		{
			case 0:
			{
				if (i < 32 && i > 0)
				{
					success = true;
				}
				else
				{
					cout << "March is 31 days long." << endl;
				}
				break;
			}

			case 1:
			{
				if (i < 31 && i > 0)
				{
					success = true;
				}
				else
				{
					cout << "April is 30 days long." << endl;
				}
				break;
			}
			case 2:
			{
				if (i == 1)
				{
					success = true;
				}
				else
				{
					cout << "You can't leave later than May 1st." << endl;
				}
				break;
			}
		}
	}
	if (success == true)
	{
		months[monthNumber] = i;
	}
	else
	{
		cout << "Invalid date. Please enter a positive integer within acceptable bounds." << endl;
	}

	return success;
}

void Calendar::editDate(int i)
{
	if (months[monthNumber] + i < monthCaps[monthNumber])
	{
		months[monthNumber] = months[monthNumber] + i;
	}
	else
	{
		monthNumber++;
		months[monthNumber] = months[monthNumber - 1] + i - monthCaps[monthNumber - 1];
	}
}