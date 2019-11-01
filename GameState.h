#ifndef _GAMESTATE_H
#define _GAMESTATE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Character.h"

#define MAX_PLAYERS 2

enum PlayerInputs
{
	INPUT_UP = (1 << 0),
	INPUT_DOWN = (1 << 1),
	INPUT_LEFT = (1 << 2),
	INPUT_RIGHT = (1 << 3),
	INPUT_MOVE = (1 << 4),
	INPUT_BLOCK = (1 << 5),
	INPUT_LP = (1 << 6),
	INPUT_RP = (1 << 7),
	INPUT_LK = (1 << 8),
	INPUT_RK = (1 << 9),
	INPUT_START = (1 << 10),
	INPUT_SELECT = (1 << 11)
};

struct Player
{
	sf::Vector2f _position;
	Character _character;
};

struct GameState
{
	void Init(int NumberOfPlayers);
	void GetPlayerAI(int i);
	void ParsePlayerInputs(int inputs, int i);
	void MovePlayer(int i);
	void Update(int Inputs[], int DisconnectFlags);

	int _framenumber;
	int _numberOfPlayers;
	Player _players[MAX_PLAYERS];
};

#endif;