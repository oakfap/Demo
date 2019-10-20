#include "Game.h"
typedef globalVar gv; typedef Spawn sp;

// GlobalVariable Re-Declaring
sf::RenderWindow gv::window(sf::VideoMode(800, 600), "Demo", sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize);
std::vector <Spawn> sp::Plateform(50);
std::vector <Spawn*> sp::Entities;
std::vector <Spawn> sp::Creeps(50);
std::vector <Spawn> sp::Keepers(50);
sf::Clock globalVar::clock;
Game Console("game");

float gv::Window_Height = 600, gv::Window_Widght = 800;
short gv::RedAmount = 0, gv::BlueAmount = 0 , gv::PfAmount = 50;
float gv::ground = 400.0;

std::string ChPath = "src/assets/";
std::string Ch = "Raven"; 
std::string Chtype = ".png";

Spawn Elsword, NormalState;

int main()
{
	srand(time(NULL));
	
	sf::RectangleShape bg;
	bg.setSize(sf::Vector2f(gv::Window_Height, gv::Window_Widght)); bg.setPosition(sf::Vector2f(0, 0)); bg.setFillColor(sf::Color::Black);
	sf::Texture texture,texture2,propic;
	texture.loadFromFile(ChPath + Ch + Chtype);
	texture2.loadFromFile(ChPath + "Raven_cut" + Chtype);
	auto th = texture.getSize();

	// Plateform Spawn
	for (int i = 0; i < gv::PfAmount; i++) {
		sp::Plateform[i].set(rand() % 1920, 400, rand() % 20 + 25, rand() % 20 + 25, "PF");
		sp::Plateform[i].Char.setFillColor(sf::Color::White);
	}
#pragma region init

	Elsword.state.reputation = 1;
	sf::Texture kolop; kolop.loadFromFile(ChPath+"Kolop"+Chtype);
	sf::Vector2f cord = Console.RandomSpawn(0,600,-100,1);
	Elsword.set(cord.x, cord.y, 128, 128, "Main");
	sp::Entities.push_back(&Elsword);
	Elsword.setTexture(0, kolop);
	Elsword.setTexture(1, texture2);
	Elsword.texTuring();
	Elsword.animated.stand();

	sf::RectangleShape profileBorder;
	profileBorder.setPosition({ 75.0,0.0 });
	sf::Sprite profile;
	propic.loadFromFile(ChPath+"Propic"+Chtype);
	profile.setTexture(propic);
	profile.setPosition({ 0.0,0.0 });
	profile.setScale({0.2,0.2});
	profileBorder.setSize({ 500,25 });
	profileBorder.setFillColor(sf::Color::Red);
	profileBorder.setOutlineColor(sf::Color::Red);
	profileBorder.setOutlineThickness(2.0);
#pragma endregion

	// Monster spawn
	for (int i = 0; i < gv::RedAmount; i++) {
		sf::Vector2f cord = Console.RandomSpawn(0, 600, -100, 1);
		sp::Creeps[i].set( cord.x , cord.y, 80, 80, "Creep");
		sp::Creeps[i].setTexture(0, texture);
		sp::Creeps[i].setTexture(1, texture2);
		sp::Creeps[i].texTuring();
		sp::Creeps[i].state.reputation = -1;
		sp::Creeps[i].Char.setFillColor(sf::Color::Red);
		sp::Entities.push_back(&sp::Creeps[i]);
		gv::window.draw(sp::Creeps[i].Char);
	}
	for (int i = 0; i < gv::BlueAmount; i++) {
		sf::Vector2f cord = Console.RandomSpawn(-600, 0, -100, 1);
		sp::Keepers[i].set(cord.x, cord.y, 80, 80, "Keeper");
		sp::Keepers[i].setTexture(0, texture);
		sp::Keepers[i].setTexture(1, texture2);
		sp::Keepers[i].texTuring();
		sp::Keepers[i].state.reputation = 1; sp::Keepers[i].state.ActDelay = sf::seconds(0.65);
		sp::Keepers[i].Char.setFillColor(sf::Color::Blue);
		sp::Entities.push_back(&sp::Keepers[i]);
		gv::window.draw(sp::Keepers[i].Char);
	}

	
	gv::window.draw(Elsword.Char);

	sf::View view(sf::Vector2f{ 0.0f,0.f }, sf::Vector2f(gv::Window_Widght, gv::Window_Height) );
	sf::View minimap(sf::Vector2f{ 0.0f,0.f }, sf::Vector2f(gv::Window_Widght, gv::Window_Height));
	// Check Interact
	//std::cout<< Elsword.Char.getGlobalBounds().intersects(Elsword.CS.getGlobalBounds());
	while (gv::window.isOpen()){
		view.setCenter(Elsword.Char.getPosition());
		//minimap.setCenter(Elsword.Char.getPosition());
		view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
		minimap.setViewport(sf::FloatRect(-0.5f, -0.5f, 1.0f, 1.0f));
		//minimap.zoom(0.5f);
		Elsword.Char.setTextureRect(Elsword.animated.update());
		gv::window.clear();
		gv::window.setView(view);
	    gv::window.draw(bg);

		// Gravity
		for (int i = 0; i < sp::Entities.size(); i++) {
			Console.Gravity(*sp::Entities[i], gv::ground);
		}

		Console.botMove(-1); Console.botMove(1);

		// PlateForm
		for (auto &i : sp::Plateform) {
			gv::window.draw(i.Char);
		}

		gv::window.draw(Elsword.Char);
		sf::Event evnt;
		while (gv::window.pollEvent(evnt))
		{
			switch (evnt.type)
			{
			case sf::Event::Closed:
				while (!sp::Entities.empty()) sp::Entities.pop_back();
				gv::window.close();
				break;
			case sf::Event::KeyPressed :
					 if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
						while (!sp::Entities.empty()) sp::Entities.pop_back();
						gv::window.close();
					}
				break;
				//  case sf::Event::Resized:
				//	Window_Widght = evnt.size.width, Window_Height = evnt.size.height;
				//	break;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			Elsword.cutScene(50, 100);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			Elsword.Move('A',0);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
				//auto temp = Elsword.posY + (80 * Elsword.turn);
				if(Elsword.Act())
				for (int i = 0; i < gv::RedAmount; i++)
					Console.Attack(Elsword, sp::Creeps[i]);
				
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			Elsword.Move('D',0);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
				auto temp = Elsword.posY + (80 * Elsword.turn);
				if(Elsword.Act())
				for (int i = 0; i < gv::RedAmount; i++)
					Console.Attack(Elsword, sp::Creeps[i]);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
			auto temp = Elsword.posY + (80 * Elsword.turn);
			if (Elsword.Act())
			for (int i = 0; i < gv::RedAmount; i++)
				Console.Attack(Elsword, sp::Creeps[i]);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			if (!Elsword.J) {
				if (Elsword.setPos(0, 0.5) || Elsword.posY >= gv::ground-2) {
					Elsword.setPos(0, -0.5);
					Elsword.jump();
				}
			}
		}
		Elsword.setPos(0,0);

		for (int i = 0; i < gv::RedAmount; i++) {
			Console.botAttack(sp::Keepers[i], sf::Color::Blue);
			Console.botAttack(sp::Creeps[i], sf::Color::Red);
		}

		if (gv::clock.getElapsedTime() - Elsword.hurtTime > sf::seconds(0.2)) {
			Elsword.Char.setFillColor(sf::Color::White);
		}
		double percent = 500.0*((Elsword.state.hp*1.0) / NormalState.state.hp);
		profileBorder.setSize(sf::Vector2f(percent,25));
		
		gv::window.setView(minimap);
		gv::window.draw(profileBorder);
	    gv::window.draw(profile);
		gv::window.display();
		// Dying
		if (Elsword.state.visible && Elsword.state.hp <= 0) {
			std::cout << "You has been slain." << std::endl;
			Elsword.state.hp = 0;
			Elsword.state.visible = false;
			Elsword.reSpawnTime = gv::clock.getElapsedTime();
			if (Elsword.Respawn()) {
				Elsword.state.hp = NormalState.state.hp; 
				Elsword.state.visible = true;
			}
		}
		else if (Elsword.state.visible && Elsword.state.hp > 0) {
			Elsword.state.hp += 0.01;
			if (Elsword.state.hp > NormalState.state.hp) {
				Elsword.state.hp = NormalState.state.hp;
			}
		}
		if (Elsword.Respawn()) {
			Elsword.state.hp = NormalState.state.hp;
			Elsword.state.visible = true;
		}
	}
}