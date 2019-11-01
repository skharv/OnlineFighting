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
#define FRAMERATE 60

void AdvanceFrame(int inputs[], int disconnect_flags);
int ReadInputs(sf::RenderWindow* Window);

class Engine
{
private:
	sf::RenderWindow* _window;
	Character* _characters[MAX_PLAYERS];

	sf::Clock deltaClock;

	bool Init(int LocalPort, int NumberOfPlayers, GGPOPlayer* Players, int NumberOfSpectators);
	void MainLoop();
	void ProcessInput();
	void RenderFrame(GameState& gameState);
	void Update(GameState* gameState, int time);
	void RunFrame(int time);
	void Idle(int time);

public:
	void Go(int LocalPort, int NumberOfPlayers, GGPOPlayer* Players, int NumberOfSpectators);

	Engine();
	~Engine();
};

#endif