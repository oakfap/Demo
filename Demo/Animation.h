#pragma once
#include "globalVar.h"
class Animation
{
public:
	void set(sf::Vector2u SpriteSize, int row, int col);
	void fromTo(int from,int to);
	void changeRow(int num);
	void stand();
	void walk();
	void attack();
	sf::IntRect update(bool isStand);

public:
	int from, to , nW = 0,nH = 0;
	int row,col;
	int spriteH, spriteW;
	sf::Time now;
	sf::Time deltaTime = sf::seconds(0.3);
};

