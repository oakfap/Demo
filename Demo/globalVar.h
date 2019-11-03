#pragma once
#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <vector>
#include <thread>

#define leftSide getGlobalBounds().left
#define CharWidth getGlobalBounds().width


class globalVar
{
public:
	globalVar();
	~globalVar();
public:
	static sf::RenderWindow window;
	static sf::Clock clock;
	static bool pause;
	static int difficulty;
	static int RedScore, BlueScore;
	static float Window_Height ;
	static float Window_Widght ;
	static short RedAmount , BlueAmount, PfAmount;
	static sf::Text gameText;
	static long Money;

};

