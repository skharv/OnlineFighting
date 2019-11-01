#include "Engine.h"

GGPOSession* ggpo = NULL;
GameState gs = { 0 };
NonGameState ngs = { 0 };

#pragma comment( lib, "winmm.lib") //More sketchy shit.. 

int fletcher32_checksum(short* data, size_t len)
{
	int sum1 = 0xffff, sum2 = 0xffff;

	while (len) {
		unsigned tlen = len > 360 ? 360 : len;
		len -= tlen;
		do {
			sum1 += *data++;
			sum2 += sum1;
		} while (--tlen);
		sum1 = (sum1 & 0xffff) + (sum1 >> 16);
		sum2 = (sum2 & 0xffff) + (sum2 >> 16);
	}

	/* Second reduction step to reduce sums to 16 bits */
	sum1 = (sum1 & 0xffff) + (sum1 >> 16);
	sum2 = (sum2 & 0xffff) + (sum2 >> 16);
	return sum2 << 16 | sum1;
}

//------------CALLBACKS------------//

bool __cdecl begin_game_callback(const char* name)
{
	return true;
}

bool __cdecl on_event_callback(GGPOEvent* info)
{
	int progress;
	switch (info->code) {
	case GGPO_EVENTCODE_CONNECTED_TO_PEER:
		ngs.SetConnectState(info->u.connected.player, Synchronizing);
		break;
	case GGPO_EVENTCODE_SYNCHRONIZING_WITH_PEER:
		progress = 100 * info->u.synchronizing.count / info->u.synchronizing.total;
		ngs.UpdateConnectProgress(info->u.synchronizing.player, progress);
		break;
	case GGPO_EVENTCODE_SYNCHRONIZED_WITH_PEER:
		ngs.UpdateConnectProgress(info->u.synchronizing.player, 100);
		break;
	case GGPO_EVENTCODE_RUNNING:
		ngs.SetConnectState(Running);
		break;
	case GGPO_EVENTCODE_CONNECTION_INTERRUPTED:
		ngs.SetDisconnectTimeout(info->u.connection_interrupted.player,
			timeGetTime(),
			info->u.connection_interrupted.disconnect_timeout);
		break;
	case GGPO_EVENTCODE_CONNECTION_RESUMED:
		ngs.SetConnectState(info->u.connection_resumed.player, Running);
		break;
	case GGPO_EVENTCODE_DISCONNECTED_FROM_PEER:
		ngs.SetConnectState(info->u.disconnected.player, Disconnected);
		break;
	case GGPO_EVENTCODE_TIMESYNC:
		Sleep(1000 * info->u.timesync.frames_ahead / FRAMERATE);
		break;
	}
	return true;
}

bool __cdecl advance_frame_callback(int flags)
{
	int inputs[MAX_PLAYERS] = { 0 };
	int disconnect_flags;

	// Make sure we fetch new inputs from GGPO and use those to update
	// the game state instead of reading from the keyboard.
	ggpo_synchronize_input(ggpo, (void*)inputs, sizeof(int) * MAX_PLAYERS, &disconnect_flags);
	AdvanceFrame(inputs, disconnect_flags);
	return true;
}

bool __cdecl load_game_state_callback(unsigned char* buffer, int len)
{
	memcpy(&gs, buffer, len);
	return true;
}

bool __cdecl save_game_state_callback(unsigned char** buffer, int* len, int* checksum, int frame)
{
	*len = sizeof(gs);
	*buffer = (unsigned char*)malloc(*len);
	if (!*buffer) {
		return false;
	}
	memcpy(*buffer, &gs, *len);
	*checksum = fletcher32_checksum((short*)*buffer, *len / 2);
	return true;
}

bool __cdecl log_game_state(char* filename, unsigned char* buffer, int len)
{
	FILE* fp = fopen(filename, "w");
	if (fp) {
		GameState* gamestate = (GameState*)buffer;
		fprintf(fp, "GameState object.\n");
		fprintf(fp, "  _numberOfPlayers: %d.\n", gamestate->_numberOfPlayers);
		for (int i = 0; i < gamestate->_numberOfPlayers; i++)
		{
			Player* player = gamestate->_players + i;
			fprintf(fp, "  player %d position:  %.4f, %.4f\n", i, player->_position.x, player->_position.y);
		}
		fclose(fp);
	}
	return true;
}

void __cdecl free_buffer(void* buffer)
{
	free(buffer);
}

//----------END CALLBACKS----------//

int ReadInputs(sf::RenderWindow* Window)
{
	static const struct {
		sf::Keyboard::Key key;
		int input;
	}  inputtable[] = {
	   { sf::Keyboard::W, INPUT_UP },
	   { sf::Keyboard::S, INPUT_DOWN },
	   { sf::Keyboard::A, INPUT_LEFT },
	   { sf::Keyboard::D, INPUT_RIGHT },
	   { sf::Keyboard::Y, INPUT_MOVE },
	   { sf::Keyboard::H, INPUT_BLOCK },
	};
	int i, inputs = 0;

	if (Window->hasFocus())
	{
		for (i = 0; i < sizeof(inputtable) / sizeof(inputtable[0]); i++)
		{
			if (sf::Keyboard::isKeyPressed(inputtable[i].key))
			{
				inputs |= inputtable[i].input;
			}
		}
	}

	return inputs;
}

void AdvanceFrame(int inputs[], int disconnect_flags)
{
	gs.Update(inputs, disconnect_flags);

	// update the checksums to display in the top of the window.  this
	// helps to detect desyncs.
	ngs.now.framenumber = gs._framenumber;
	ngs.now.checksum = fletcher32_checksum((short*)&gs, sizeof(gs) / 2);
	if ((gs._framenumber % 90) == 0)
	{
		ngs.periodic = ngs.now;
	}

	// Notify ggpo that we've moved forward exactly 1 frame.
	ggpo_advance_frame(ggpo);

	// Update the performance monitor display.
	GGPOPlayerHandle handles[MAX_PLAYERS];
	int count = 0;
	for (int i = 0; i < ngs._numberOfPlayers; i++)
	{
		if (ngs._players[i].type == GGPO_PLAYERTYPE_REMOTE)
		{
			handles[count++] = ngs._players[i].handle;
		}
	}
}

void DrawCurrentFrame()
{
	
}

bool Engine::Init(int LocalPort, int NumberOfPlayers, GGPOPlayer* Players, int NumberOfSpectators)
{
	GGPOErrorCode result;
	GGPOSessionCallbacks cb = { 0 };

	gs.Init(NumberOfPlayers);
	ngs._numberOfPlayers = NumberOfPlayers;

	cb.begin_game = begin_game_callback;
	cb.advance_frame = advance_frame_callback;
	cb.load_game_state = load_game_state_callback;
	cb.save_game_state = save_game_state_callback;
	cb.free_buffer = free_buffer;
	cb.on_event = on_event_callback;
	cb.log_game_state = log_game_state;

	_window = new sf::RenderWindow(sf::VideoMode(640, 480), "Online Fighting");

	result = ggpo_start_session(&ggpo, &cb, "OnlineFighting", NumberOfPlayers, sizeof(int), LocalPort);

	ggpo_set_disconnect_timeout(ggpo, 3000);
	ggpo_set_disconnect_notify_start(ggpo, 1000);

	int i;
	for (i = 0; i < NumberOfPlayers + NumberOfSpectators; i++)
	{
		GGPOPlayerHandle handle;
		result = ggpo_add_player(ggpo, Players + i, &handle);
		ngs._players[i].handle = handle;
		ngs._players[i].type = Players[i].type;

		if (Players[i].type == GGPO_PLAYERTYPE_LOCAL)
		{
			_characters[i] = new Character("Resources/blank/blank.json", "Resources/blank/blank.atlas", sf::Vector2f((320 * i) + 160, 320));
			ngs._players[i].connect_progress = 100;
			ngs._localPlayerHandle = handle;
			ngs.SetConnectState(handle, Connecting);
			ggpo_set_frame_delay(ggpo, handle, FRAME_DELAY);
		}
		else
		{
			ngs._players[i].connect_progress = 0;
			_characters[i] = new Character("Resources/blank/blank.json", "Resources/blank/blank.atlas", sf::Vector2f((320 * i) + 160, 320));
		}
	}

	_window->setTitle("Connecting to peers...");

	return true;
}

void Engine::MainLoop()
{
	MSG msg = { 0 };
	int start, next, now;

	start = next = now = timeGetTime();
	while (_window->isOpen())
	{
		now = timeGetTime();
		Idle(std::max(0, next - now - 1)); 
		if (now >= next)
		{
			RunFrame(now);
			next = now + (1000 / FRAMERATE);
		}
	}
}

void Engine::ProcessInput()
{
	sf::Event evt;

	while (_window->pollEvent(evt))
	{
		if (evt.type == sf::Event::Closed)
			_window->close();
	}
}

void Engine::RenderFrame(GameState& gameState)
{
	_window->clear(sf::Color::Black);

	for (int i = 0; i < gameState._numberOfPlayers; i++)
	{
		gameState._players[i]._character.Draw(_window);
	}
	_window->display();
}

void Engine::Update(GameState* gameState, int time)
{
	for (int i = 0; i < gameState->_numberOfPlayers; i++)
	{
		gameState->_players[i]._character.Update(time);
	}
}

void Engine::RunFrame(int time)
{
	GGPOErrorCode result = GGPO_OK;
	int disconnect_flags;
	int inputs[MAX_PLAYERS] = { 0 };

	ProcessInput();

	if (ngs._localPlayerHandle != GGPO_INVALID_HANDLE)
	{
		int input = ReadInputs(_window);
		result = ggpo_add_local_input(ggpo, ngs._localPlayerHandle, &input, sizeof(input)); //This SHOULD return 0
	}

	if (GGPO_SUCCEEDED(result))
	{
		result = ggpo_synchronize_input(ggpo, (void*)inputs, sizeof(int) * MAX_PLAYERS, &disconnect_flags);

		if (GGPO_SUCCEEDED(result))
		{
			// inputs[0] and inputs[1] contain the inputs for p1 and p2.  Advance
			// the game by 1 frame using those inputs.
			AdvanceFrame(inputs, disconnect_flags);
		}
	}

	Update(&gs, time);

	RenderFrame(gs);
}

void Engine::Idle(int time)
{
	ggpo_idle(ggpo, time);
}

void Engine::Go(int LocalPort, int NumberOfPlayers, GGPOPlayer* Players, int NumberOfSpectators)
{
	//_players = Players;

	if (!Init(LocalPort, NumberOfPlayers, Players, NumberOfSpectators))
		throw "Could not initialise Engine";

	MainLoop();

	memset(&gs, 0, sizeof(gs));
	memset(&ngs, 0, sizeof(ngs));

	if (ggpo) {
		ggpo_close_session(ggpo);
		ggpo = NULL;
	}
}

Engine::Engine()
{

}

Engine::~Engine()
{
}
