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
	void block();
	sf::IntRect update();
	
public:
	sf::SoundBuffer sample;
	sf::Sound s;
	int from, to, nW = 0, nH = 0;
	int fromA, toA, nAW = 0;
	int row, col;
	int spriteH, spriteW;
	std::string state = "stand";
	std::string actState = "block";
     
	sf::Time now,nowA;
	sf::Time LastChange;
	sf::Time deltaTime = sf::seconds(0.3);
};

