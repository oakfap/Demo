#include "Game.h"

Game::Game(std::string name){
	Gamename = name;
}

void Game::Gravity(Spawn &Ent) {
	if (Ent.J == 1) {
		if (Ent.posY > Ent.JH) {
			Ent.setPos(0, -1, 'U');
		}
		else {	
				Ent.J = 2;		
		}
	}
	else if (Ent.J != 1) {
		if (!Ent.setPos(0, 1, 'D')) {
			Ent.J = 0;
		}

	}
}

bool Game::Attack(Spawn &attacker, Spawn &o) {
		if (attacker.Hitbox.getGlobalBounds().intersects(o.Hitbox.getGlobalBounds()) || attacker.Action.getGlobalBounds().intersects(o.Hitbox.getGlobalBounds())) {
			int oHp = o.state.hurted(attacker.state);
			if ( oHp <= 0) {
				o.state.visible = false;
				o.reSpawnTime = gv::clock.getElapsedTime();
				if (o.state.reputation == -1) globalVar::BlueScore++;
				else globalVar::RedScore++;
				std::cout << "RedScore : " << globalVar::RedScore << " : " << "BlueScore : " << globalVar::BlueScore << std::endl;
				return true;
			}
			int back = 5;
		    back*= attacker.turn;
			o.setPos(back, 0,'B');
			o.Char.setFillColor(sf::Color::Red);
			o.hurtTime = gv::clock.getElapsedTime();
			double percent = o.Char.getSize().x *((o.state.hp*1.0) / (o.state.maxHp*1.0));
			o.HealthBar.setSize(sf::Vector2f(percent, 5));
		}
		return false;
}

bool Game::PFCollapse(sf::FloatRect cord) {
	//std::cout << "Finding" << std::endl;
	for (auto &i : sp::Plateform) {
		if (cord.intersects(i.Hitbox.getGlobalBounds())) {
			return true;
		}
	}
	return false;
}

sf::Vector2f Game::RandomSpawn(int left,int right,int up,int down,sf::Vector2f size) {
	//std::cout << "..." << std::endl;
	bool notFound = true;
	float x, y;
	while (notFound) {
		x = (rand() % (right-left) + left)*1.0;
		y = (rand() % (down - up) + up)*1.0;
		if (!PFCollapse({ x,y,size.y,size.x/2 })) {
			notFound = false;
			return { x,y };
		}
	}
}

int Game::findTarget(Spawn& finder, int Rep) {
	float MaxDis = 99999;
	auto p = finder.Hitbox.getPosition();
	int idx = -1;
	switch (Rep) {
	case 1:
		for (int i = 0; i < gv::BlueAmount; i++) {
			if (sp::Keepers[i].state.visible == true) {
				float cd = Game::calDis(p, sp::Keepers[i].Hitbox.getPosition());
				if (MaxDis > cd) {
					MaxDis = cd;
					idx = i;
				}
			}
		}
		break;
	case -1:
		for (int i = 0; i < gv::RedAmount; i++) {
			if (sp::Creeps[i].state.visible == true) {
				float cd = Game::calDis(p, sp::Creeps[i].Hitbox.getPosition());
				if (MaxDis > cd) {
					MaxDis = cd;
					idx = i;
				}
			}
		}
		break;
	}
	//std::cout << idx << std::endl;

	return idx;
}

void Game::botMove(int mode) {
	switch (mode) {
	case -1:
		for (int i = 0; i < gv::RedAmount; i++) {
			if (sp::Creeps[i].state.visible) {
				float step = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.1));
				if (!sp::Creeps[i].target) {
					int idx = findTarget(sp::Creeps[i], 1);
					sp::Creeps[i].target = (idx == -1 ? nullptr : &sp::Keepers[idx]);
				}

				if (!botAttack(sp::Creeps[i], sf::Color::White)) {
					if (sp::Creeps[i].Char.getPosition().y < sp::Creeps[i].target->Hitbox.getPosition().y) {
						//sp::Creeps[i].setPos(0, 1, 'd');
						//sp::Creeps[i].J = 2;
					}
					else if (sp::Creeps[i].Char.getPosition().y > sp::Creeps[i].target->Hitbox.getPosition().y) {
						if (!sp::Creeps[i].J)
							sp::Creeps[i].jump();
					}
					if (sp::Creeps[i].Hitbox.getPosition().x < sp::Creeps[i].target->Hitbox.leftSide) {
						sp::Creeps[i].Move('R', step);
					}
					else if (sp::Creeps[i].Hitbox.getPosition().x > sp::Creeps[i].target->Hitbox.leftSide - sp::Creeps[i].target->Hitbox.CharWidth) {
						sp::Creeps[i].Move('L', step);
					}
					else {
						if (globalVar::difficulty >= 2) {
							if(sp::Creeps[i].state.blocking())
							sp::Creeps[i].animated.block();
					
						}
					}
					
				}
				
				sp::Creeps[i].Char.setTextureRect(sp::Creeps[i].animated.update());
				gv::window.draw(sp::Creeps[i].Char);
				gv::window.draw(sp::Creeps[i].HealthBar);
				//gv::window.draw(sp::Creeps[i].Action);
				//gv::window.draw(sp::Creeps[i].Foot);
				//gv::window.draw(sp::Creeps[i].Hitbox);
			}
			else {
				if (sp::Creeps[i].Respawn()) {
					sp::Creeps[i].state.hp = 15;
					sp::Creeps[i].state.visible = true;
					sf::Vector2f cord = Game::RandomSpawn(0, 1200, -100, 1, sp::Creeps[i].Char.getSize());
					sp::Creeps[i].set(cord.x, cord.y, sp::Creeps[i].Char.getSize().x, sp::Creeps[i].Char.getSize().y, sp::Creeps[i].name);
				}
			}

		}break;
	case 1:
		for (int i = 0; i < gv::BlueAmount; i++) {
			if (sp::Keepers[i].state.visible) {
				float step = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.1));
				if (globalVar::difficulty >= 2 || !sp::Keepers[i].target) {
					int idx = findTarget(sp::Keepers[i], -1);
					sp::Keepers[i].target = (idx == -1 ? nullptr : &sp::Creeps[idx]);
				}
				if (!botAttack(sp::Keepers[i],sf::Color::White)) {

					if (sp::Keepers[i].Hitbox.getPosition().y < sp::Keepers[i].target->Hitbox.getPosition().y) {
						//sp::Keepers[i].setPos(0, 1, 'd');
						//sp::Keepers[i].J = 2;
					}
					else if (sp::Keepers[i].Hitbox.getPosition().y > sp::Keepers[i].target->Hitbox.getPosition().y) {
					
					}

					if (sp::Keepers[i].Hitbox.getPosition().x < sp::Keepers[i].target->Hitbox.leftSide) {
						sp::Keepers[i].Move('R', step);
					}
					else if (sp::Keepers[i].Hitbox.getPosition().x > sp::Keepers[i].target->Hitbox.leftSide + sp::Keepers[i].target->Char.CharWidth) {
						sp::Keepers[i].Move('L', step);
					}
					
				}
				else {
					if (globalVar::difficulty >= 2) {
						if(sp::Keepers[i].state.blocking())
						sp::Keepers[i].animated.block();
					}
				}
				
				sp::Keepers[i].Char.setTextureRect(sp::Keepers[i].animated.update());
				gv::window.draw(sp::Keepers[i].Char);
				gv::window.draw(sp::Keepers[i].HealthBar);
				//gv::window.draw(sp::Keepers[i].Action);
				//gv::window.draw(sp::Keepers[i].Foot);
			    //gv::window.draw(sp::Keepers[i].Hitbox);
			}
			else {
				if (sp::Keepers[i].Respawn()) {
					sp::Keepers[i].state = defaultStat;
					sp::Keepers[i].state.visible = true;
					sf::Vector2f cord = Game::RandomSpawn(-1200, 0, -100, 1,sp::Keepers[i].Char.getSize());
					sp::Keepers[i].Char.setPosition(cord);
					sp::Keepers[i].set(cord.x, cord.y, sp::Keepers[i].Char.getSize().x, sp::Keepers[i].Char.getSize().y, sp::Keepers[i].name);
				}
			}
		}
		break;
	}
}

bool Game::botAttack(Spawn &ent, sf::Color clr) {
	if (ent.state.visible) {
		if (ent.target) {
			if (ent.Action.getGlobalBounds().intersects(ent.target->Hitbox.getGlobalBounds())) {
				if (ent.Act()) {
					Game::Attack(ent, *ent.target);
				}
				else {
					if (rand() % 100 < 60) {
						if (ent.state.blocking()) {
							ent.animated.block();
						}
						
					}
				}
			}
			else {
				return false;
			}
			if (ent.target->state.visible == false)
				ent.target = nullptr;
		}
	}
	if (gv::clock.getElapsedTime() - ent.hurtTime > sf::seconds(0.2)) {
		ent.Char.setFillColor(clr);
	}
	return true;
}

void Game::gameCondition() {
	if (sp::Keepers[0].state.hp <= 0) {
		std::cout << "Red Win" << std::endl;
		gv::window.clear(); 
		gv::pause = true;
	}
	if (sp::Creeps[0].state.hp <= 0){
		std::cout << "Blue Win" << std::endl;
		gv::window.clear(); 
		gv::pause = true;
	}
	
}

Game::~Game()
{
}
