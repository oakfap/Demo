#include "Game.h"



Game::Game(std::string name)
{
	Gamename = name;
}

void Game::Gravity(Spawn &Ent,int ground) {
	if (Ent.J) {
		if (Ent.posY > Ent.JH) {
			int unMovable = Ent.setPos(0,-1);
			if (unMovable) {
				if (unMovable == 1) {
					Ent.J = false;
				}
			}
		}
		else {	
				Ent.J = false;		
		}
	}
	else if (!Ent.J) {
		if (Ent.posY < ground) {
			int unMovable = Ent.setPos(0, 1);
			
		}
	}
}

void Game::Attack(Spawn &attacker, Spawn &o) {
		if (attacker.Action.getGlobalBounds().intersects(o.Char.getGlobalBounds())) {
			//std::cout << "Attack!!";
			if ( o.state.hurted(attacker.state)<= 0) {
				o.state.visible = false;
				o.reSpawnTime = gv::clock.getElapsedTime();
				std::cout << o.name << " Has been Slain" << std::endl;
			}
			int back = 5;
		    back*= attacker.turn;
			o.setPos(back, -20);
			o.Char.setFillColor(sf::Color::Red);
			o.hurtTime = gv::clock.getElapsedTime();
		}
	
}

bool Game::PFCollapse(sf::FloatRect cord) {
	//std::cout << "Finding" << std::endl;
	for (auto &i : sp::Plateform) {
		if (cord.intersects(i.Char.getGlobalBounds())) {
			return true;
		}
	}
	return false;
}


sf::Vector2f Game::RandomSpawn(int left,int right,int up,int down) {
	//std::cout << "..." << std::endl;
	bool notFound = true;
	float x, y;
	while (notFound) {
		x = (rand() % (right-left) + left)*1.0;
		y = (rand() % (down - up) + up)*1.0;
		if (!PFCollapse({ x,y,90,90 })) {
			notFound = false;
		//	std::cout <<" Found Cord => "<< x << " : " << y << std::endl;
			return { x,y };
		}
	}
}

int Game::findTarget(Spawn& finder, int Rep) {
	float MaxDis = 99999;
	auto p = finder.Char.getPosition();
	int idx = -1;
	switch (Rep) {
	case 1:
		for (int i = 0; i < gv::BlueAmount; i++) {
			if (sp::Keepers[i].state.visible == true) {
				float cd = Game::calDis(p, sp::Keepers[i].Char.getPosition());

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
				float cd = Game::calDis(p, sp::Creeps[i].Char.getPosition());
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
				if (sp::Creeps[i].target) {
					if (sp::Creeps[i].Action.getPosition().x < sp::Creeps[i].target->Char.leftSide) {
						sp::Creeps[i].Move('D', step);
					}
					else if (sp::Creeps[i].Action.getPosition().x > sp::Creeps[i].target->Char.leftSide + sp::Creeps[i].target->Char.CharWidth) {
						sp::Creeps[i].Move('A', step);
					}
				}
				gv::window.draw(sp::Creeps[i].Char);
			}
			else {
				if (sp::Creeps[i].Respawn()) {
					sp::Creeps[i].state.hp = 15;
					sp::Creeps[i].state.visible = true;
					sf::Vector2f cord = Game::RandomSpawn(0, 1200, -100, 1);
					sp::Creeps[i].set(cord.x, cord.y, 80, 80, sp::Creeps[i].name);
				}
			}
		}break;
	case 1:
		for (int i = 0; i < gv::BlueAmount; i++) {
			if (sp::Keepers[i].state.visible) {
				float step = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.1));
				if (!sp::Keepers[i].target) {
					int idx = findTarget(sp::Keepers[i], -1);
					sp::Keepers[i].target = (idx == -1 ? nullptr : &sp::Creeps[idx]);
				}
				if (sp::Keepers[i].target) {
					if (sp::Keepers[i].Action.getPosition().x < sp::Keepers[i].target->Char.leftSide) {
						sp::Keepers[i].Move('D', step);
					}
					else if (sp::Keepers[i].Action.getPosition().x > sp::Keepers[i].target->Char.leftSide + sp::Keepers[i].target->Char.CharWidth) {
						sp::Keepers[i].Move('A', step);
					}
				}
				gv::window.draw(sp::Keepers[i].Char);
			}
			else {
				if (sp::Keepers[i].Respawn()) {
					sp::Keepers[i].state.hp = 15;
					sp::Keepers[i].state.visible = true;
					sf::Vector2f cord = Game::RandomSpawn(-1200, 0, -100, 1);
					sp::Keepers[i].Char.setPosition(cord);
					sp::Keepers[i].set(cord.x, cord.y, 80, 80, sp::Keepers[i].name);
				}
			}
		}
		break;
	}
}

void Game::botAttack(Spawn &ent, sf::Color clr) {
	if (ent.state.visible) {
		if (ent.target) {
			if (ent.Action.getGlobalBounds().intersects(ent.target->Char.getGlobalBounds())) {
				if (ent.Act()) {
					Game::Attack(ent, *ent.target);
				}
			}
			if (ent.target->state.visible == false)
				ent.target = nullptr;
		}
	}
	if (gv::clock.getElapsedTime() - ent.hurtTime > sf::seconds(0.2)) {
		ent.Char.setFillColor(clr);
	}
}

Game::~Game()
{
}
