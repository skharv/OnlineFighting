#ifndef _GAMESTATE_H
#define _GAMESTATE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Character.h"

#define MAX_PLAYERS 2

struct Player
{
	sf::Vector2i _position;
	sf::Vector2f _velocity;
	int _action;
	bool _complete;
	Character _character;
};

struct GameState
{
	void Init(int NumberOfPlayers);
	void GetPlayerAI(int i, Character* PlayerCharacter);
	void ParsePlayerInputs(int inputs, int i, Character* PlayerCharacter);
	void UpdatePlayer(int i, int FrameNumber, float Time, Character PlayerCharacter);
	void Update(int Inputs[], int DisconnectFlags, float time);

	int _framenumber;
	int _numberOfPlayers;
	Player _players[MAX_PLAYERS];
};

#endif;