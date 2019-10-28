#include "Engine.h"

bool Engine::Init()
{
	_window = new sf::RenderWindow(sf::VideoMode(640, 480), "GGPO Test");

	_character = new Character("Resources/blank/blank.json", "Resources/blank/blank.atlas");

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

void Engine::Go()
{
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
