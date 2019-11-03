#include "Status.h"



Status::Status(){}

void Status::setState(int a, int d, int h, int b) {
	atk = a;
	def = d;
	hp  = h;
	maxHp = h;
	block = b;
}

int Status::hurted(Status &Obj) {
	int tDef = def;
	if (globalVar::clock.getElapsedTime() - lastBlock < sf::seconds(0.5)) {
		tDef += block;
	}
	hp -= Obj.atk - tDef;
	return hp;
}

bool Status::blocking(){
	if (globalVar::clock.getElapsedTime() - lastAct >= ActDelay) {
		lastAct = globalVar::clock.getElapsedTime();
		lastBlock = globalVar::clock.getElapsedTime(); 
		return true;
	}
	else return false;
}

Status::~Status()
{
}
