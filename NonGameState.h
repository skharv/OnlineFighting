#ifndef _NONGAMESTATE_H
#define _NONGAMESTATE_H

#include <ggponet.h>

#define MAX_PLAYERS 2

enum PlayerConnectState {
	Connecting = 0,
	Synchronizing,
	Running,
	Disconnected,
	Disconnecting,
};

struct PlayerConnectionInfo {
	GGPOPlayerType       type;
	GGPOPlayerHandle     handle;
	PlayerConnectState   state;
	int                  connect_progress;
	int                  disconnect_timeout;
	int                  disconnect_start;
};

struct NonGameState
{
	struct ChecksumInfo {
		int framenumber;
		int checksum;
	};

	void SetConnectState(GGPOPlayerHandle handle, PlayerConnectState state) {
		for (int i = 0; i < _numberOfPlayers; i++) {
			if (_players[i].handle == handle) {
				_players[i].connect_progress = 0;
				_players[i].state = state;
				break;
			}
		}
	}

	void SetDisconnectTimeout(GGPOPlayerHandle handle, int now, int timeout) {
		for (int i = 0; i < _numberOfPlayers; i++) {
			if (_players[i].handle == handle) {
				_players[i].disconnect_start = now;
				_players[i].disconnect_timeout = timeout;
				_players[i].state = Disconnecting;
				break;
			}
		}
	}

	void SetConnectState(PlayerConnectState state) {
		for (int i = 0; i < _numberOfPlayers; i++) {
			_players[i].state = state;
		}
	}

	void UpdateConnectProgress(GGPOPlayerHandle handle, int progress) {
		for (int i = 0; i < _numberOfPlayers; i++) {
			if (_players[i].handle == handle) {
				_players[i].connect_progress = progress;
				break;
			}
		}
	}

	GGPOPlayerHandle _localPlayerHandle;
	PlayerConnectionInfo _players[MAX_PLAYERS];
	int _numberOfPlayers;

	ChecksumInfo         now;
	ChecksumInfo         periodic;
};

#endif