#include "GameState.h"
#include "Engine.h"

extern GGPOSession* ggpo;

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

void GameState::GetPlayerAI(int i, Character* PlayerCharacter)
{
	Player* player = _players + i;

	*PlayerCharacter = player->_character;
}

void GameState::ParsePlayerInputs(int inputs, int i, Character* PlayerCharacter)
{
	Player* player = _players + i;

	player->_character.HandleInputs(inputs);

	*PlayerCharacter = player->_character;
}

void GameState::UpdatePlayer(int i, int FrameNumber, float Time, Character PlayerCharacter)
{
	Player* player = _players + i;

	PlayerCharacter.Update(Time);
	 
	player->_character = PlayerCharacter;
}

void GameState::Update(int inputs[], int disconnect_flags, float time)
{
	_framenumber++;
	for (int i = 0; i < _numberOfPlayers; i++)
	{
		Player* p = _players + i;
		Character character = p->_character;

		if (disconnect_flags & (1 << i))
		{
			GetPlayerAI(i, &character);
		}
		else
		{
			ParsePlayerInputs(inputs[i], i, &character);
		}

		UpdatePlayer(i, _framenumber, time, character);
	}
}