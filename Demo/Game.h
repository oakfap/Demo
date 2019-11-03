#pragma once
#include "globalVar.h"
#include "Spawn.h"
#define distance sqrt(abs(pow(finder.x - tracker.x,2) + abs(pow(finder.y - tracker.y,2) ) ) )
typedef globalVar gv; typedef Spawn sp;

class Game
{
public:
	std::string Gamename;
	Status defaultStat;
public:
	Game(std::string name);
	void Gravity(Spawn &Ent);
	bool Attack(Spawn &attacker, Spawn &Object);
	bool PFCollapse(sf::FloatRect cord);
	sf::Vector2f RandomSpawn(int left, int right, int up, int down, sf::Vector2f size);
	int findTarget(Spawn& finder, int Rep);
	void botMove(int mode);
	bool botAttack(Spawn &ent, sf::Color clr);
	void gameCondition();
	static float calDis(sf::Vector2f finder, sf::Vector2f tracker) {
		return  distance;
	}
	~Game();

};

