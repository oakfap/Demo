#include "Spawn.h"

//Spawn::Spawn() {};

void Spawn::set(float x, float y, float w, float h, std::string  n) {
	posX = x; height = h;
	posY = y; width = w;
	name = n;
	Char.setSize(sf::Vector2f(w, h));
	Char.setOutlineColor(sf::Color::Red);
	Char.setOutlineThickness(2.5);

	Char.setOrigin(width /2, height /2);
	Char.setPosition(sf::Vector2f(posX, posY));

	CS.setSize(sf::Vector2f(200, 300));
	Blank.setSize(sf::Vector2f(200, 300));
	Blank.setFillColor(sf::Color::Black);
	attack.loadFromFile("src/assets/" + name + "_slash.png");
	Action.setTexture(&attack);
	Action.setSize(sf::Vector2f(w, h));
	state.visible = true;

}

void Spawn::forming(int ind) {
	if (!ind)
	  Char.setTexture(&standard);
	else
	  Char.setTexture(&cutscene);
}

void Spawn::texTuring() {
	Char.setTexture(&standard);
	CS.setTexture(&cutscene);
}

void Spawn::setTexture(int ind,sf::Texture tt) {
	if (!ind) {
		standard = tt;
		animated.set(tt.getSize(), 1, 5);
	}
	else
		cutscene = tt;
}

void Spawn::cutScene(int goL,int goR) {
	float x = 1.0, y = 1.0;
	//Char.setTexture(&cutscene);
	//globalVar::window.draw(Char);
	Blank.setSize(sf::Vector2f(200, 300));
	for (int i = 0; i <= goL; i++) {
		
		//globalVar::window.clear();
		y++;
		CS.setPosition(y, x);
		globalVar::window.draw(CS);
		globalVar::window.display();
		Blank.setPosition(y, x);
		globalVar::window.draw(Blank);
	}
	sf::sleep(sf::milliseconds(1000));
	for (int i = 0; i <= goR; i++) {
		//globalVar::window.clear();
		y--;
		CS.setPosition(y , x);
		globalVar::window.draw(CS);
		globalVar::window.display();
		Blank.setPosition(y, x);
		globalVar::window.draw(Blank);
	}
	//Char.setTexture(&standard);
	//globalVar::window.draw(Char);
}

bool Spawn::Act() {
	if (globalVar::clock.getElapsedTime() - state.lastAct >= state.ActDelay) {
		state.lastAct = globalVar::clock.getElapsedTime();
		auto Tp = Char.getPosition();
		Tp.x += turn * width/4;
		Tp.y -= width / 2;
		Action.setPosition(Tp);
		globalVar::window.draw(Action);
		globalVar::window.display();
		return true;
	}
	else return false;
}

int Spawn::setPos(float x,float y) {
	
	Char.setPosition(sf::Vector2f(posX+x, posY+y));
	if (PFCollapse()) {
		Char.setPosition(sf::Vector2f(posX, posY));
		return 1;
	}
	else {
		posY += y, posX += x;
		Action.setPosition(sf::Vector2f(posX, posY));
		return 0;
	}
	
}

void Spawn::Move(char dir,float dis) {
	dis += state.walkAbility;
	if (dir == 'A') {
		dis*=-1.0;
	//	Char.setOrigin(width / 2, height / 2);
		Char.setScale({ -1, 1 });
		turn = -1;
		Action.setScale({ -1, 1 });
	}
	else if (dir == 'D') {
		turn = 1;
	//	Char.setOrigin(width / 2, height / 2);
		Char.setScale({ 1, 1 });
		Action.setScale({ 1, 1 });	
	}
	int unMovable = setPos(dis, 0);
	if (unMovable && name != "Main") {
			if (unMovable == 1 && !J)
				jump();
	}
	animated.walk();
}

void Spawn::jump() {
	J = true;
	JH = posY - state.jumpAbility;

}

bool Spawn::Respawn() {

	if (!state.visible) {
		if (globalVar::clock.getElapsedTime() - reSpawnTime > sf::seconds(5)) {
			return true;
		}
	}
	return false;
}

bool Spawn::PFCollapse() {
	
	for (auto &i : Spawn::Plateform) {
		if (Char.getGlobalBounds().intersects(i.Char.getGlobalBounds())) {
			//std::cout << "PlateForm Collapse" << std::endl;
			return true;
		}
	}
	return false;
}

bool Spawn::EntCollapse() {
	for (auto &i : Spawn::Entities) {
		if (i->state.reputation != state.reputation && Char.getGlobalBounds().intersects(i->Char.getGlobalBounds())) {
			//std::cout<< name << " engaging enemy" << std::endl;
			return true;
		}
	}
	return false;
}

Spawn::~Spawn(){
	std::cout << name << " Has Despawned." << std::endl;
}
/*
.h
class A{
public:
	static void GG();

}
.cpp
static void A::GG();


main.cpp

Spawn spawn();
Spawn.set();

A::GG();
*/