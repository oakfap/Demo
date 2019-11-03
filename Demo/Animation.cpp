#include "Animation.h"

void Animation::set(sf::Vector2u s,int r , int c){
	row = r, col = c;
	spriteW = s.x / c;
	spriteH = s.y / r;
}
void Animation::stand() {
	if (state != "stand") {
		state = "stand";
 		from = 0, to = 1;
		nW = from;
		now = globalVar::clock.getElapsedTime();
	}
}
void Animation::walk() {
	if (state != "walk") {
		state = "walk";
		from = 2, to = 3;
		nW = from;
		now = globalVar::clock.getElapsedTime();
	}
	LastChange = globalVar::clock.getElapsedTime();
}

void Animation::attack() {
	if (state != "attack" ) {
		state = "attack";
		from = 4, to= 5;
		nW = from;
		now = globalVar::clock.getElapsedTime();
	//	if (s.pl&&sample.loadFromFile("src/sounds/" + state + ".wav")) {
	//		s.setBuffer(sample);
	//		s.play();
	//	}
	}
	LastChange = globalVar::clock.getElapsedTime();
}
void Animation::block() {
	if (state != "block" ) {
		state = "block";
		from = 6, to = 7;
		nW = from;		
		now = globalVar::clock.getElapsedTime();
	}
	LastChange = globalVar::clock.getElapsedTime();
}

void Animation::fromTo(int f, int t) {
	if (state != "free") {
		state = "free";
		from = f; to = t;
		nW = from;
		now = globalVar::clock.getElapsedTime();
	}
	LastChange = globalVar::clock.getElapsedTime();
}
void Animation::changeRow(int num) {
	nH = num;
}

sf::IntRect Animation::update() {
	if (globalVar::clock.getElapsedTime() - now >= deltaTime) {
		now = globalVar::clock.getElapsedTime();
		nW++;
		if (nW > to) nW = from;
	}
	if (globalVar::clock.getElapsedTime() - LastChange >= sf::seconds(0.5) && state != "free") stand();
	return { spriteW*nW,spriteH*nH,spriteW,spriteH };
}
