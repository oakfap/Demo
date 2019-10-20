#pragma once
#include "globalVar.h"
class Status
{
public:
	Status();
	void setState(float a, float d, float h, float b);
	int hurted(Status& Obj);
	~Status();
public:
	float atk = 1, def = 0, hp = 10, block = 2;
	sf::Time ActDelay = sf::seconds(0.5);
	sf::Time lastAct = sf::seconds(0);
	short reputation = 0;
	float jumpAbility = 150,walkAbility = 1.5;
	bool visible = false;
};

