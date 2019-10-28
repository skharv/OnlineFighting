#ifndef _ENGINE_H
#define _ENGINE_H

#include <ggponet.h>
#include <SFML/Graphics.hpp>
#include "Character.h"

class Engine
{
private:
	

	sf::RenderWindow* _window;
	Character* _character;

	sf::Clock deltaClock;

	bool Init();
	void MainLoop();
	void ProcessInput();
	void RenderFrame();
	void Update();
public:
	void Go();

	Engine();
	~Engine();
};

#endif