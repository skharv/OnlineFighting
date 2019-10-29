#ifndef _ENGINE_H
#define _ENGINE_H

#include <ggponet.h>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <iostream>
#include "Character.h"
#include "GameState.h"
#include "NonGameState.h"

#define FRAME_DELAY 3


class Engine
{
private:
	int _localPort, _numberOfPlayers, _numberOfSpectators;

	GGPOPlayer* _players;

	sf::RenderWindow* _window;
	Character* _character;

	sf::Clock deltaClock;

	bool Init();
	void MainLoop();
	void ProcessInput();
	void RenderFrame();
	void Update();

	void AdvanceFrame(int inputs[], int disconnect_flags);
public:
	void Go(int LocalPort, int NumberOfPlayers, GGPOPlayer* Players, int NumberOfSpectators);

	Engine();
	~Engine();
};

#endif