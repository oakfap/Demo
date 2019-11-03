#pragma once
#include "Status.h"
#include "globalVar.h"
#include "Animation.h"

class Spawn 
{
  private:
	  sf::Texture standard;
	  sf::Texture cutscene;
	  sf::Texture attack;
	  
  public:
	//  Spawn();
	  void set(float x, float y, float w, float h, std::string  name);
	  void set(sf::Vector2f cord, sf::Vector2f size, std::string  name);
	  void forming(int ind);
	  void setHitbox(sf::Vector2f size);
	  void setHitbox(float scailX,float scailY);
	  void setFoot(sf::Vector2f size);
	  void setFoot(float scailX, float scailY);
	  void setTexture(int mode,sf::Texture tt,int row, int col);
	  void texTuring();
	  void cutScene( int goL, int goR);
	  void Move(char dir,float dis);
	  bool PFCollapse();
	  bool EntCollapse();
	  int setPos(float x,float y,char dir);
	  bool Act();
	  void jump();
	  //void runMultiThread();
	  bool Respawn();
	  ~Spawn();
  public:
	static std::vector <Spawn> Plateform;
	static std::vector <Spawn*> Entities;
	static std::vector <Spawn> Creeps;
	static std::vector <Spawn> Keepers;
  public:
	  sf::RectangleShape Char,CS,Foot;
	  sf::RectangleShape Action,Hitbox,HealthBar;;
	  std::string name;
	  int turn = 1;
      sf::Time hurtTime = sf::seconds(0);
	  sf::Time reSpawnTime;
	  Spawn *target = nullptr ;
	  int  J = 2;
	  float JH = 0;
	  float posX = 0 , posY = 0 , height = 0 , width = 0 ;  // position
	  Status state;
	  Animation animated;
};

