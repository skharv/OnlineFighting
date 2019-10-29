#include "Engine.h"

GGPOSession* ggpo = NULL;
GameState gs = { 0 };
NonGameState ngs = { 0 };

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
			1,
			info->u.connection_interrupted.disconnect_timeout);
		break;
	case GGPO_EVENTCODE_CONNECTION_RESUMED:
		ngs.SetConnectState(info->u.connection_resumed.player, Running);
		break;
	case GGPO_EVENTCODE_DISCONNECTED_FROM_PEER:
		ngs.SetConnectState(info->u.disconnected.player, Disconnected);
		break;
	case GGPO_EVENTCODE_TIMESYNC:
		Sleep(1000 * info->u.timesync.frames_ahead / 60);
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
	//AdvanceFrame(inputs, disconnect_flags);
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
	//FILE* fp = fopen(filename, "w");
	//if (fp) {
	//	GameState* gamestate = (GameState*)buffer;
	//	fprintf(fp, "GameState object.\n");
	//	fprintf(fp, "  bounds: %d,%d x %d,%d.\n", gamestate->_bounds.left, gamestate->_bounds.top,
	//		gamestate->_bounds.right, gamestate->_bounds.bottom);
	//	fprintf(fp, "  num_ships: %d.\n", gamestate->_num_ships);
	//	for (int i = 0; i < gamestate->_num_ships; i++)
	//	{
	//		Ship* ship = gamestate->_ships + i;
	//		fprintf(fp, "  ship %d position:  %.4f, %.4f\n", i, ship->position.x, ship->position.y);
	//		fprintf(fp, "  ship %d velocity:  %.4f, %.4f\n", i, ship->velocity.dx, ship->velocity.dy);
	//		fprintf(fp, "  ship %d radius:    %d.\n", i, ship->radius);
	//		fprintf(fp, "  ship %d heading:   %d.\n", i, ship->heading);
	//		fprintf(fp, "  ship %d health:    %d.\n", i, ship->health);
	//		fprintf(fp, "  ship %d speed:     %d.\n", i, ship->speed);
	//		fprintf(fp, "  ship %d cooldown:  %d.\n", i, ship->cooldown);
	//		fprintf(fp, "  ship %d score:     %d.\n", i, ship->score);
	//		for (int j = 0; j < MAX_BULLETS; j++)
	//		{
	//			Bullet* bullet = ship->bullets + j;
	//			fprintf(fp, "  ship %d bullet %d: %.2f %.2f -> %.2f %.2f.\n", i, j,
	//				bullet->position.x, bullet->position.y,
	//				bullet->velocity.dx, bullet->velocity.dy);
	//		}
	//	}
	//	fclose(fp);
	//}
	return true;
}

void __cdecl free_buffer(void* buffer)
{
	free(buffer);
}

//----------END CALLBACKS----------//

bool Engine::Init()
{
	GGPOErrorCode result;
	GGPOSessionCallbacks cb = { 0 };

	gs.Init(_numberOfPlayers);
	ngs._numberOfPlayers = _numberOfPlayers;

	cb.begin_game = begin_game_callback;
	cb.advance_frame = advance_frame_callback;
	cb.load_game_state = load_game_state_callback;
	cb.save_game_state = save_game_state_callback;
	cb.free_buffer = free_buffer;
	cb.on_event = on_event_callback;
	cb.log_game_state = log_game_state;

	_window = new sf::RenderWindow(sf::VideoMode(640, 480), "GGPO Test");

	_character = new Character("Resources/blank/blank.json", "Resources/blank/blank.atlas");

	result = ggpo_start_session(&ggpo, &cb, "vectorwar", _numberOfPlayers, sizeof(int), _localPort);

	ggpo_set_disconnect_timeout(ggpo, 3000);
	ggpo_set_disconnect_notify_start(ggpo, 1000);

	int i;
	for (i = 0; i < _numberOfPlayers + _numberOfSpectators; i++) {
		GGPOPlayerHandle handle;
		result = ggpo_add_player(ggpo, _players + i, &handle);
		ngs._players[i].handle = handle;
		ngs._players[i].type = _players[i].type;
		if (_players[i].type == GGPO_PLAYERTYPE_LOCAL) {
			ngs._players[i].connect_progress = 100;
			ngs._localPlayerHandle = handle;
			ngs.SetConnectState(handle, Connecting);
			ggpo_set_frame_delay(ggpo, handle, FRAME_DELAY);
		}
		else {
			ngs._players[i].connect_progress = 0;
		}
	}

	return true;
}

void Engine::MainLoop()
{
	while (_window->isOpen())
	{
		ProcessInput();
		Update();
		RenderFrame();
	}
}

void Engine::ProcessInput()
{

}

void Engine::RenderFrame()
{
	_window->clear(sf::Color::Black);
	_character->Draw(_window);
	_window->display();
}

void Engine::Update()
{
	float delta = deltaClock.getElapsedTime().asSeconds();

	_character->Update(delta);

	deltaClock.restart();
}

void Engine::AdvanceFrame(int inputs[], int disconnect_flags)
{
	gs.Update(inputs, disconnect_flags);

	// update the checksums to display in the top of the window.  this
	// helps to detect desyncs.
	ngs.now.framenumber = gs._framenumber;
	ngs.now.checksum = fletcher32_checksum((short*)&gs, sizeof(gs) / 2);
	if ((gs._framenumber % 90) == 0) {
		ngs.periodic = ngs.now;
	}

	// Notify ggpo that we've moved forward exactly 1 frame.
	ggpo_advance_frame(ggpo);

	// Update the performance monitor display.
	GGPOPlayerHandle handles[MAX_PLAYERS];
	int count = 0;
	for (int i = 0; i < ngs._numberOfPlayers; i++) {
		if (ngs._players[i].type == GGPO_PLAYERTYPE_REMOTE) {
			handles[count++] = ngs._players[i].handle;
		}
	}
}

void Engine::Go(int LocalPort, int NumberOfPlayers, GGPOPlayer* Players, int NumberOfSpectators)
{
	_localPort = LocalPort;
	_numberOfPlayers = NumberOfPlayers;
	_numberOfSpectators = NumberOfSpectators;
	_players = Players;

	if (!Init())
		throw "Could not initialise Engine";

	MainLoop();
}

Engine::Engine()
{

}

Engine::~Engine()
{
}
