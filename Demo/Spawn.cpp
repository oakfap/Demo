#include "Spawn.h"

//Spawn::Spawn() {};

void Spawn::set(float x, float y, float w, float h, std::string  n) {
	posX = x; height = h;
	posY = y; width = w;
	name = n;
	Char.setSize(sf::Vector2f(w, h));
	//Char.setOutlineColor(sf::Color::Red);
	//Char.setOutlineThickness(2.5);
	Char.setOrigin(width /2, height /2);
	Char.setPosition(sf::Vector2f(posX, posY));

	Foot.setOrigin(Char.getOrigin());
	Foot.setPosition({Char.getPosition().x,Char.getPosition().y+h});

	Hitbox.setOrigin(Char.getOrigin());
	Hitbox.setPosition({ Char.getPosition().x,Char.getPosition().y});
	Hitbox.setSize(Char.getSize());

	HealthBar.setSize({w,5.f});
	HealthBar.setOutlineColor(sf::Color::White);
	HealthBar.setOutlineThickness(1.5);
	if(name == "Creep")
		HealthBar.setFillColor(sf::Color::Red);
	else if (name == "Keeper")
		HealthBar.setFillColor(sf::Color::Green);
	else 
		HealthBar.setFillColor(sf::Color::Blue);
	HealthBar.setOrigin(Char.getOrigin());
	HealthBar.setPosition(Char.getPosition());

	CS.setSize(sf::Vector2f(200, 300));
	attack.loadFromFile("src/assets/" + name + "_slash.png");
	Action.setTexture(&attack);
	Action.setSize(sf::Vector2f(w, h));
	auto Tp = Char.getPosition();
	Tp.x += turn * width / 4;
	Tp.y -= width / 2;
	Action.setPosition(Tp);
	state.visible = true;
}

void Spawn::set(sf::Vector2f cord, sf::Vector2f size, std::string  name) {
	posX = cord.x; height = size.y;
	posY = cord.y; width = size.x;
	name = name;
	Char.setSize(size);
	//Char.setOutlineColor(sf::Color::Red);
	//Char.setOutlineThickness(2.5);
	Char.setOrigin(width / 2, height / 2);
	Char.setPosition(cord);

	Foot.setOrigin(Char.getOrigin());
	Foot.setPosition({ Char.getPosition().x,Char.getPosition().y + height });

	Hitbox.setOrigin(Char.getOrigin());
	Hitbox.setPosition({ Char.getPosition().x,Char.getPosition().y });
	Hitbox.setSize(Char.getSize());

	HealthBar.setSize({ width,5.f });
	HealthBar.setOutlineColor(sf::Color::White);
	HealthBar.setOutlineThickness(1.5);
	if (name == "Creep")
		HealthBar.setFillColor(sf::Color::Red);
	else if (name == "Keeper")
		HealthBar.setFillColor(sf::Color::Green);
	else
		HealthBar.setFillColor(sf::Color::Blue);
	HealthBar.setOrigin(Char.getOrigin());
	HealthBar.setPosition(Char.getPosition());

	CS.setSize(sf::Vector2f(200, 300));
	attack.loadFromFile("src/assets/" + name + "_slash.png");
	Action.setTexture(&attack);
	Action.setSize(sf::Vector2f(width, height));
	auto Tp = Char.getPosition();
	Tp.x += turn * width / 4;
	Tp.y -= width / 2;
	Action.setPosition(Tp);
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

void Spawn::setTexture(int ind,sf::Texture tt,int r,int c) {
	if (!ind) {
		standard = tt;
		animated.set(tt.getSize(), r, c);
	}
	else
		cutscene = tt;
}

void Spawn::cutScene(int goL,int goR) {}
		
void Spawn::setHitbox(sf::Vector2f size) {
	Hitbox.setPosition(Char.getPosition());
	Hitbox.setSize(size);
	Hitbox.setOrigin(width / 2, height / 2);
	Hitbox.move(0, -size.y);
}
void Spawn::setHitbox(float sx, float sy) {
	Hitbox.setSize(Char.getSize());
	Hitbox.setScale(sx, sy);Hitbox.setOrigin(width / 2, height / 2);
	Hitbox.setPosition(Char.getPosition());
	Hitbox.move(0, 0);
}

void Spawn::setFoot(sf::Vector2f size) {
	Foot.setPosition(Char.getPosition());
	Foot.setSize(size);
	Foot.setOrigin(width / 2, height / 2);
	Foot.setPosition({ posX,posY + height - Foot.getSize().y });
	Foot.move(0, 0);
}
void Spawn::setFoot(float sx,float sy) {
	Foot.setSize(Char.getSize());
	Foot.setScale(sx,sy);
	Foot.setOrigin(width / 2, height / 2);
	Hitbox.setPosition(Char.getPosition());
	Foot.move(0,0);
}

bool Spawn::Act() {
	if (globalVar::clock.getElapsedTime() - state.lastAct >= state.ActDelay) {
		animated.attack();
		state.lastAct = globalVar::clock.getElapsedTime();
		auto Tp = Char.getPosition();
		Tp.x += turn * width/4;
		Tp.y -= width / 2;
		Action.setPosition(Tp);
		globalVar::window.draw(Action);
		return true;
	}
	else return false;
}

int Spawn::setPos(float x,float y,char dir) {
	Char.setPosition(sf::Vector2f(posX+x, posY+y));
	if ( dir == 'D' && PFCollapse()) {
		Char.setPosition(sf::Vector2f(posX, posY));
		return 0;
	}
	if (dir == 'd' && Foot.getGlobalBounds().intersects(Spawn::Plateform[0].Hitbox.getGlobalBounds())) {
		Char.setPosition(sf::Vector2f(posX, posY));
		return 0;
	}
	posY += y, posX += x;
	Action.move(x,y);
	Foot.move(x,y);
	Hitbox.move(x, y);
	HealthBar.move(x, y);
	return 1;
}

void Spawn::Move(char dir,float dis) {
	dis += state.walkAbility;
	if (dir == 'L') {
		dis*=-1.0;
		Char.setScale({ -1, 1 });
		turn = -1;
		Action.setScale({ -1, 1 });
	}
	else if (dir == 'R') {
		turn = 1;
		Char.setScale({ 1, 1 });
		Action.setScale({ 1, 1 });	
	}
	int unMovable = setPos(dis, 0,dir);
	if (unMovable && name != "Main") {
			if (unMovable == 1 && !J)
				jump();
	}
	animated.walk();
}

void Spawn::jump() {
	if (J == 0 ) {
		J = 1;
		JH = posY - state.jumpAbility;
	}
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
		if (Foot.getGlobalBounds().intersects(i.Hitbox.getGlobalBounds())) {
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