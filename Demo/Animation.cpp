#include "Animation.h"



void Animation::set(sf::Vector2u s,int r , int c){
	row = r, col = c;
	spriteW = s.x / c;
	spriteH = s.y / r;
}
void Animation::stand() {
	from = 0, to = 1;
	nW = from;
	now = globalVar::clock.getElapsedTime();
}
void Animation::walk() {
	from = 2, to = 3;
	nW = from;
	now = globalVar::clock.getElapsedTime();
}
void Animation::attack() {
	from = 4, to = 4;
	nW = from;
	now = globalVar::clock.getElapsedTime();
}
void Animation::fromTo(int f, int t) {
	from = f; to = t;
	nW = from;
}
void Animation::changeRow(int num) {
	nH = num;
}

sf::IntRect Animation::update(bool isStand) {
	if (globalVar::clock.getElapsedTime() - now >= deltaTime) {
		now = globalVar::clock.getElapsedTime();
		nW++;
		if (nW > to) nW = from;
	}
	if (isStand) {

	}
	return { spriteW*nW,spriteH*nH,spriteW,spriteH };
}