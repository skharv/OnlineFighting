#include "Gamestate.h"

void GameState::Init(int NumberOfPlayers)
{
}

void GameState::GetPlayerAI(int i, double* heading, double* thrust, int* fire)
{
}

void GameState::ParsePlayerInputs(int inputs, int i, double* heading, double* thrust, int* fire)
{
}

void GameState::MovePlayer(int i, double heading, double thrust, int fire)
{
}

void GameState::Update(int inputs[], int disconnect_flags)
{
	_framenumber++;
	for (int i = 0; i < _numberOfPlayers; i++) {
		double thrust, heading;
		int fire;

		if (disconnect_flags & (1 << i)) {
			GetPlayerAI(i, &heading, &thrust, &fire);
		}
		else {
			ParsePlayerInputs(inputs[i], i, &heading, &thrust, &fire);
		}

		MovePlayer(i, heading, thrust, fire);
	}
}