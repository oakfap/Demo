#pragma once
#include "globalVar.h"
class Status
{
public:
	Status();
	void setState(int a, int d, int h, int b);
	int hurted(Status& Obj);
	bool blocking();
	~Status();
public:
	int atk = 5, def = 0, hp = 50, block = 2;
	int maxHp=50;
	bool isBlock = false;
	sf::Time lastBlock;
	sf::Time ActDelay = sf::seconds(0.5);
	sf::Time lastAct = sf::seconds(0);
	short reputation = 0;
	float jumpAbility = 150,walkAbility = 0.5;
	bool visible = false;
};

