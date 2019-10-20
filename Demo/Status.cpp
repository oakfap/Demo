#include "Status.h"



Status::Status(){}

void Status::setState(float a, float d, float h, float b) {
	atk = a;
	def = d;
	hp  = h;
	block = b;
}

int Status::hurted(Status &Obj) {
	hp -= Obj.atk - def;
	return hp;
}

Status::~Status()
{
}
