#include "Game.h"
typedef globalVar gv; typedef Spawn sp;

float gv::Window_Height = 600, gv::Window_Widght = 800;
short gv::RedAmount = 10, gv::BlueAmount = 5, gv::PfAmount = 20;
int	  gv::difficulty = 2;
int   gv::RedScore = 0, gv::BlueScore = 0;

// GlobalVariable Re-Declaring
sf::RenderWindow gv::window(sf::VideoMode(800, 600), "Demo", sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize);
std::vector <Spawn> sp::Plateform(50);

std::vector <Spawn*> sp::Entities;
std::vector <Spawn> sp::Creeps(50);
std::vector <Spawn> sp::Keepers(100);
sf::Clock gv::clock;
sf::Font font;
sf::Text gv::gameText;
long gv::Money = 200;

bool gv::pause = false;
Game Console("game");

std::string ChPath = "src/assets/";
std::string Ch = "Raven"; 
std::string Chtype = ".png";

Spawn MainCharacter, NormalState;
Spawn d;
int main()
{
	srand(time(NULL));
	font.loadFromFile(ChPath+"gameFont.ttf");
	gv::gameText.setFont(font);
	//gv::window.setVerticalSyncEnabled(true);
	sf::RectangleShape bg;
	bg.setSize(sf::Vector2f(gv::Window_Height, gv::Window_Widght)); bg.setPosition(sf::Vector2f(0, 0)); bg.setFillColor(sf::Color::Black);
	sf::Texture texture,texture2,propic,floors,flags;
	texture.loadFromFile(ChPath + Ch + Chtype);
	floors.loadFromFile(ChPath + "floors" + Chtype);
	flags.loadFromFile(ChPath + "flags" + Chtype);
	texture2.loadFromFile(ChPath + "Raven_cut" + Chtype);
	auto th = texture.getSize();
	sf::FileInputStream file;
	// Background Music
	sf::SoundBuffer SB; SB.loadFromFile("src/sounds/bg.flac"); sf::Sound bgM;  bgM.setBuffer(SB); bgM.play(); bgM.setLoop(true); bgM.setVolume(10);
	// Attack Sound
	sf::SoundBuffer Action; Action.loadFromFile("src/sounds/attack.wav"); sf::Sound attackSound; attackSound.setBuffer(Action); attackSound.setVolume(50);

#pragma region init
	sp::Plateform[0].set(-1000,500,100000,50,"PF");
	sp::Plateform[0].Char.setOutlineColor(sf::Color::Red);
	sp::Plateform[0].Char.setOutlineThickness(2);
	sp::Plateform[0].setTexture(0, floors, 1, 3);
	sp::Plateform[0].texTuring();
	int fR = rand() % 3;
	sp::Plateform[0].animated.fromTo(fR, fR);
	sp::Plateform[0].Char.setTextureRect(sp::Plateform[0].animated.update());
	// Plateform Spawn
	for (int i = 1; i < gv::PfAmount; i++) {
		int pflevel = rand() % 6 + 1 ;
		sp::Plateform[i].set(rand() % 1920, 480 - 80*pflevel, rand() % 150 + 25, 25, "PF");
		//sp::Plateform[i].Char.setFillColor(sf::Color::White);
		sp::Plateform[i].Char.setOutlineColor(sf::Color::Red);
		sp::Plateform[i].Char.setOutlineThickness(2);
		sp::Plateform[i].setTexture(0,floors,1,3);
		sp::Plateform[i].texTuring();
		sp::Plateform[i].setHitbox({sp::Plateform[i].Char.getSize().x,1});
		int fR = rand() % 3;
		sp::Plateform[i].animated.fromTo(fR, fR);
		sp::Plateform[i].Char.setTextureRect(sp::Plateform[i].animated.update());
	}

	MainCharacter.state.reputation = 1;
	sf::Texture kolop; kolop.loadFromFile(ChPath+"Kolop"+Chtype);
	sf::Vector2f cord = Console.RandomSpawn(0, 600, -100, 1, { 128,128 });
	MainCharacter.set(cord.x, cord.y, 128, 128, "Main");
	sp::Entities.push_back(&MainCharacter);
	MainCharacter.setTexture(0, kolop,1,8);
	//MainCharacter.setTexture(1, texture2);
	MainCharacter.texTuring();
	MainCharacter.animated.stand();
	MainCharacter.setFoot({128,1});
	sf::RectangleShape profileBorder;
	profileBorder.setPosition({ 75.0,0.0 });
	sf::Sprite profile;
	propic.loadFromFile(ChPath+"Propic"+Chtype);
	profile.setTexture(propic);
	profile.setPosition({ 0.0,0.0 });
	profile.setScale({0.2,0.2});
	profileBorder.setSize({ 300,25 });
	profileBorder.setFillColor(sf::Color::Red);
	profileBorder.setOutlineColor(sf::Color::Red);
	profileBorder.setOutlineThickness(2.0);

	sp::Creeps[0].set(2000, 400, 200, 200, "Creep");
	sp::Creeps[0].setTexture(0, kolop, 1, 8);
	sp::Creeps[0].texTuring(); sp::Creeps[0].state.setState(10,1,500,1);
	sp::Creeps[0].state.reputation = -1;
	sp::Entities.push_back(&sp::Creeps[0]);
	sp::Creeps[0].setFoot({ 200,1 });
	gv::window.draw(sp::Creeps[0].Char);

	sp::Keepers[0].set(-100, 400, 200, 200, "Keeper");
	sp::Keepers[0].setTexture(0, kolop, 1, 8);
	sp::Keepers[0].texTuring(); sp::Keepers[0].state.setState(10, 1, 500, 1);
	sp::Keepers[0].state.reputation = 1;
	sp::Entities.push_back(&sp::Keepers[0]);
	sp::Keepers[0].setFoot({ 200,1 });
	gv::window.draw(sp::Keepers[0].Char);

#pragma endregion

	// Monster spawn
	for (int i = 1; i < gv::RedAmount; i++) {
		sf::Vector2f cord = Console.RandomSpawn(800, 1400, 0, 100, { 80,80 });
		sp::Creeps[i].set(cord.x, cord.y, 80, 80, "Creep");
		sp::Creeps[i].setTexture(0, kolop,1,8);
		sp::Creeps[i].texTuring();
		sp::Creeps[i].state.reputation = -1; 
		sp::Creeps[i].setFoot({ 80,1 });
		sp::Entities.push_back(&sp::Creeps[i]);
		gv::window.draw(sp::Creeps[i].Char);
	}
	for (int i = 1; i < gv::BlueAmount; i++) {
		sf::Vector2f cord = Console.RandomSpawn(0, 600, 0, 100, { 80,80 });
		sp::Keepers[i].set(cord.x, cord.y, 80, 80, "Keeper");
		sp::Keepers[i].setTexture(0, kolop,1,8);
		sp::Keepers[i].texTuring();
		sp::Keepers[i].state.reputation = 1; 
		sp::Keepers[i].setFoot({ 80,1 });
		sp::Entities.push_back(&sp::Keepers[i]);
		gv::window.draw(sp::Keepers[i].Char);
	}

	gv::window.draw(MainCharacter.Char);
	gv::window.draw(MainCharacter.HealthBar);
	
	
	sf::View view(sf::Vector2f{ 0.0f,0.f }, sf::Vector2f(gv::Window_Widght, gv::Window_Height) );
	sf::View minimap(sf::Vector2f{ gv::Window_Widght/2,300 }, sf::Vector2f(gv::Window_Widght, gv::Window_Height));
	// Check Interact
	//std::cout<< MainCharacter.Char.getGlobalBounds().intersects(MainCharacter.CS.getGlobalBounds());

	while (gv::window.isOpen()){
	
		if (!gv::pause) {
			view.setCenter(MainCharacter.Char.getPosition());
			view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
			

			gv::window.clear();
			gv::window.setView(view);
			gv::window.draw(bg);
			for (auto &i : sp::Plateform) {
				gv::window.draw(i.Char);
			}
			// Gravity
			for (int i = 0; i < sp::Entities.size(); i++) {
				Console.Gravity(*sp::Entities[i]);
			}
		
			
			Console.botMove(-1); Console.botMove(1);
			
			gv::window.draw(MainCharacter.Char);
			gv::window.draw(MainCharacter.HealthBar);
			sf::Event evnt;
			while (gv::window.pollEvent(evnt))
			{
				switch (evnt.type)
				{
				case sf::Event::Closed:
					while (!sp::Entities.empty()) sp::Entities.pop_back();
					gv::window.close();
					break;
				case sf::Event::KeyPressed:
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
						while (!sp::Entities.empty()) sp::Entities.pop_back();
						gv::window.close();
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
						bool select = true;
						gv::window.clear(sf::Color::Blue);
						for (int i = 0; i < 5; i++) {
							gv::gameText.setString("Press 1 got | Spawn new Troop");
							gv::gameText.setPosition({ 50, 50 + i * 50 });
							gv::window.draw(gv::gameText);
						}
						gv::window.display();
						
						while (select) {
							switch (evnt.type)
							{
								case sf::Event::KeyPressed:
									if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) && gv::Money >= 100 && gv::BlueAmount < 50) {
										sp::Keepers[gv::BlueAmount].set(MainCharacter.Char.getPosition(), { 80, 80 }, "Keeper");
										sp::Keepers[gv::BlueAmount].setTexture(0, kolop, 1, 8);
										sp::Keepers[gv::BlueAmount].texTuring();
										sp::Keepers[gv::BlueAmount].state.reputation = 1;
										sp::Keepers[gv::BlueAmount].setFoot({ 80,1 });
										sp::Entities.push_back(&sp::Keepers[gv::BlueAmount]);
										gv::BlueAmount++;
										gv::Money -= 100;
										select = false;
									}
									else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
										MainCharacter.state.atk += 1;
										select = false;
									}
									else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
										MainCharacter.state.atk += 1;
										select = false;
									}
									break;
							}
						}
					}
					break;
					//  case sf::Event::Resized:
					//	Window_Widght = evnt.size.width, Window_Height = evnt.size.height;
					//	break;
				}
				fflush(stdin);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				MainCharacter.Move('L', 0);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				MainCharacter.Move('R', 0);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				MainCharacter.setPos(0, 1, 'd');
				MainCharacter.J = 2;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
				MainCharacter.animated.attack();
				if (MainCharacter.Act()) {
					attackSound.play();
					for (int i = 0; i < gv::RedAmount; i++)
						if(sp::Creeps[i].state.visible)
							if (Console.Attack(MainCharacter, sp::Creeps[i]))
								gv::Money += 100;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				if (!MainCharacter.J) {
				    MainCharacter.jump();
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
				if (MainCharacter.state.blocking()) {
					MainCharacter.animated.block();
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				globalVar::difficulty++;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				globalVar::difficulty--;
				if (globalVar::difficulty <= 0) globalVar::difficulty = 0;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
				bgM.stop();
				main();
			}
		
			
			
			if (gv::clock.getElapsedTime() - MainCharacter.hurtTime > sf::seconds(0.2)) {
				MainCharacter.Char.setFillColor(sf::Color::White);
			}

			double percent = 300.0*((MainCharacter.state.hp*1.0) / MainCharacter.state.maxHp);
			profileBorder.setSize(sf::Vector2f(percent, 25));

			gv::gameText.setString("Money: "+ std::to_string(gv::Money) + " $");
			gv::gameText.setFillColor(sf::Color::Yellow);
			gv::gameText.setScale(0.5, 0.5);
			minimap.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
			gv::window.setView(minimap);
			gv::gameText.setPosition({ 600,0 });
			gv::window.draw(gv::gameText);
			gv::window.draw(profileBorder);
			gv::window.draw(profile);
			gv::window.display();

			// Damage Step or ETC //
			// Dying
			if (MainCharacter.state.visible && MainCharacter.state.hp <= 0) {
				std::cout << "You has been slain." << std::endl;
				MainCharacter.state.hp = 0;
				MainCharacter.state.visible = false;
				MainCharacter.reSpawnTime = gv::clock.getElapsedTime();
				if (MainCharacter.Respawn()) {
					MainCharacter.state.hp = NormalState.state.hp;
					MainCharacter.state.visible = true;
				}
			}
			else if (MainCharacter.state.visible && MainCharacter.state.hp > 0) {
				MainCharacter.state.hp += 0.01;
				if (MainCharacter.state.hp > NormalState.state.hp) {
					MainCharacter.state.hp = NormalState.state.hp;
				}
			}
			if (MainCharacter.Respawn()) {
				MainCharacter.state.hp = NormalState.state.hp;
				MainCharacter.state.visible = true;
			}
			MainCharacter.Char.setTextureRect(MainCharacter.animated.update());
		}
		//Console.gameCondition();
	}
}