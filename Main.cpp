#include "Engine.h"
#include <windows.h>
#include <iostream>

#pragma comment(lib,"WS2_32") //This is suss, but it makes it work...

#define GGPO_MAX_PLAYERS 2
#define GGPO_MAX_SPECTATORS 32


int local_port, num_players, num_spectators;
GGPOPlayer* players;

int wmain(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int offset = 1, local_player = 0;
	WSADATA wd = { 0 };
	wchar_t wide_ip_buffer[128];

	WSAStartup(MAKEWORD(2, 2), &wd);

	if (__argc < 3)
	{
		std::cout << "Invalid command arguments!" << std::endl;
		std::cout << "Syntax: vectorwar.exe <local port> <num players> ('local' | <remote ip>:<remote port>)" << std::endl;
		std::cout << "That means it's first your local port and the number of players in the game. Then P1 (who is local) and then P2 who has an IP (unless you are P2 in which case P2 is local)" << std::endl;
		return 1;
	}

	local_port = _wtoi(__wargv[offset++]);
	num_players = _wtoi(__wargv[offset++]);

	if (num_players < 0 || __argc < offset + num_players)
	{
		std::cout << "Invalid command arguments!" << std::endl;
		std::cout << "Something went wrong... But I don't know what it is. Did you fuck up the command line?" << std::endl;
		return 1;
	}

	GGPOPlayer players[GGPO_MAX_SPECTATORS + GGPO_MAX_PLAYERS];

	int i;
	for (i = 0; i < num_players; i++)
	{
		const wchar_t* arg = __wargv[offset++];

		players[i].size = sizeof(players[i]);
		players[i].player_num = i + 1;

		if (!_wcsicmp(arg, L"local")) {
			players[i].type = GGPO_PLAYERTYPE_LOCAL;
			local_player = i;
			continue;
		}

		players[i].type = GGPO_PLAYERTYPE_REMOTE;
		if (swscanf(arg, L"%[^:]:%hd", wide_ip_buffer, &players[i].u.remote.port) != 2)
		{
			std::cout << "Something went wrong... But I don't know what it is. Maybe it's you?" << std::endl;
			return 1;
		}
		wcstombs(players[i].u.remote.ip_address, wide_ip_buffer, sizeof(players[i].u.remote.ip_address));
	}
	num_spectators = 0;
	while (offset < __argc)
	{
		players[i].type = GGPO_PLAYERTYPE_SPECTATOR;
		if (swscanf(__wargv[offset++], L"%[^:]:%hd", wide_ip_buffer, &players[i].u.remote.port) != 2)
		{
			std::cout << "Something went wrong... I don't know what it is, but it's probably someone else... Right?" << std::endl;
			return 1;
		}
		wcstombs(players[i].u.remote.ip_address, wide_ip_buffer, sizeof(players[i].u.remote.ip_address));
		i++;
		num_spectators++;
	}

	Engine* engine = new Engine();

	try
	{
		engine->Go(local_port, num_players, players, num_spectators);
	}
	catch (char* e)
	{
		MessageBoxA(NULL, e, "Engine didn't start! Maybe check the spark plugs?", MB_OK | MB_ICONERROR);
	}

	return 0;
}