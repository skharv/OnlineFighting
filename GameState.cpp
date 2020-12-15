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

void GameState::GetPlayerAI(int i, sf::Vector2i* Position, sf::Vector2f* Velocity, bool* Complete)
{
	Player* player = _players + i;

	*Position = player->_position;
	*Velocity = player->_velocity;
	*Complete = player->_complete;
}

void GameState::ParsePlayerInputs(int inputs, int i, sf::Vector2i* Position, sf::Vector2f* Velocity, bool* Complete)
{
	Player* player = _players + i;

	if (inputs & INPUT_RIGHT)
	{
		player->_action = 2;
		*Velocity = V_WALKFORWARD;
	}
	else if (inputs & INPUT_LEFT)
	{
		player->_action = 3;
		*Velocity = V_WALKBACK;
	}
	else if (inputs & INPUT_LIGHT)
	{
		player->_action = 4;
		*Velocity = V_ROLLFORWARD;
	}
	else if (inputs & INPUT_MEDIUM)
	{
		player->_action = 5;
		*Velocity = V_ROLLBACK;
	}
	else if (inputs & INPUT_HEAVY)
	{
		player->_action = 6;
	}
	else
	{
		player->_action = 1;
	}

	*Position = player->_position;
}

void GameState::UpdatePlayer(int i, int FrameNumber, float Time, sf::Vector2i Position, sf::Vector2f Velocity, bool Complete)
{
	Player* player = _players + i;
	player->_velocity.y += 1;

	if (player->_action == 0 || player->_action == 1)
	{
		if (player->_character.GetDrawable()->state->getCurrent(0)->isComplete() && player->_character.GetDrawable()->state->getCurrent(0)->getNext() == NULL)
		{
			player->_action = 0;
			Velocity = sf::Vector2f(0, 0);
		}
	}
	 
	player->_character.SetComplete(Complete);
	player->_character.Update(Time, player->_action);

	Position.x += Velocity.x;
	Position.y += Velocity.y;
	player->_position = Position;
	player->_velocity = Velocity;

	player->_character.SetPosition(player->_position);

	if (i == 0)
	{
		std::cout << player->_character.GetPosition().x << " | ";
	}
	if (i == 1)
	{
		std::cout << player->_character.GetPosition().x << std::endl;
	}
}

void GameState::Update(int inputs[], int disconnect_flags, float time)
{
	_framenumber++;
	for (int i = 0; i < _numberOfPlayers; i++)
	{
		sf::Vector2i position;
		sf::Vector2f velocity;
		bool complete;

		if (disconnect_flags & (1 << i))
		{
			GetPlayerAI(i, &position, &velocity, &complete);
		}
		else
		{
			ParsePlayerInputs(inputs[i], i, &position, &velocity, &complete);
		}

		UpdatePlayer(i, _framenumber, time, position, velocity, complete);
	}
}