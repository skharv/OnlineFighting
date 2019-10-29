#ifndef _GAMESTATE_H
#define _GAMESTATE_H

#include <SFML/Graphics.hpp>

#define MAX_PLAYERS 2

struct Player {
	sf::Vector2f position;
	sf::Vector2f velocity;
};

struct GameState
{
	void Init(int NumberOfPlayers);
	void GetPlayerAI(int i, double* heading, double* thrust, int* fire);
	void ParsePlayerInputs(int inputs, int i, double* heading, double* thrust, int* fire);
	void MovePlayer(int i, double heading, double thrust, int fire);
	void Update(int Inputs[], int DisconnectFlags);

	int _framenumber;
	int _numberOfPlayers;
	Player _players[MAX_PLAYERS];
};

#endif;