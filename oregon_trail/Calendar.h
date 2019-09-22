#ifndef CALENDAR_H
#define CALENDAR_H

class Calendar
{
   public:
	bool setMonthNumber(int);

	bool setDay(int i);

	void changeDate();
	int getDay();
	int getMonth();

	void editDate(int);



   private:
	int monthNumber;
	const int monthCaps[9]{31, 30, 31, 30, 31, 31, 30, 31, 30};

	int months[9];
};

#endif