#include "GameState.h"

void GameState::Init(int NumberOfPlayers)
{
	_framenumber = 0;
	_numberOfPlayers = NumberOfPlayers;
	for (int i = 0; i < _numberOfPlayers; i++)
	{
		_players[i]._position.x = ((320 * i) + 160);
		_players[i]._position.y = 320;
		_players[i]._character = Character("Resources/blank/blank.json", "Resources/blank/blank.atlas", _players[i]._position);
	}
}

void GameState::GetPlayerAI(int i)
{

}

void GameState::ParsePlayerInputs(int inputs, int i)
{
	Player* player = _players + i;

	if (inputs & INPUT_RIGHT)
	{
		player->_position.x += 1;
	}
	else if (inputs & INPUT_LEFT)
	{
		player->_position.x -= 1;
	}
}

void GameState::MovePlayer(int i)
{
	Player* player = _players + i;

	player->_character._position = player->_position;
}

void GameState::Update(int inputs[], int disconnect_flags)
{
	_framenumber++;

	for (int i = 0; i < _numberOfPlayers; i++)
	{
		if (disconnect_flags & (1 << i))
		{
			GetPlayerAI(i);
		}
		else
		{
			ParsePlayerInputs(inputs[i], i);
		}

		MovePlayer(i);
	}
}