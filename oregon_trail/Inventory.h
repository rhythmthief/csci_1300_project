#ifndef INVENTORY_H
#define INVENTORY_H

class Inventory
{
   public:
	Inventory();
	void setOxen(int);
	void setFood(int);
	void setBullets(int);
	void setMoney(float);
	void setMedkits(int);
	void setParts(int);
	void setWagon(int);

	int getOxen();
	int getFood();
	int getBullets();
	float getMoney();
	int getMedkits();
	int getParts();
	int getWagon();

   private:
	int wagon;
	int oxen;
	float money;
	int food;
	int bullets;
	int medkits;
	int parts;
};

#endif