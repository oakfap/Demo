#pragma once
#include "globalVar.h"
#include "Spawn.h"
#define distance sqrt(abs(pow(finder.x - tracker.x,2) + abs(pow(finder.y - tracker.y,2) ) ) )
typedef globalVar gv; typedef Spawn sp;

class Game
{
public:
	std::string Gamename;
public:
	Game(std::string name);
//	void update(std::vector <Spawn*> &AllEntity);
	void Gravity(Spawn &Ent,int ground);
	void Attack(Spawn &attacker, Spawn &Object);
	bool PFCollapse(sf::FloatRect cord);
	sf::Vector2f RandomSpawn(int left, int right, int up, int down);
	int findTarget(Spawn& finder, int Rep);
	void botMove(int mode);
	void botAttack(Spawn &ent, sf::Color clr);
	static float calDis(sf::Vector2f finder, sf::Vector2f tracker) {
		return  distance;
	}
	//bool EntCollapse(Spawn &Ent);
	~Game();

};

