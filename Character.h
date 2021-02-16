#ifndef _CHARACTER_H
#define _CHARACTER_H

#include <spine/spine-sfml.h>
#include <SFML/Graphics.hpp>
#include "AnimationHandler.h"
#include "ActionListLoader.h"

//These will be read in from a text file.
#define V_WALKFORWARD sf::Vector2f(1, 0)
#define V_WALKBACK sf::Vector2f(-1, 0)
#define V_ROLLFORWARD sf::Vector2f(8, 0)
#define V_ROLLBACK sf::Vector2f(-6, 0)

enum Inputs
{
	INPUT_UP = (1 << 0),
	INPUT_DOWN = (1 << 1),
	INPUT_LEFT = (1 << 2),
	INPUT_RIGHT = (1 << 3),
	INPUT_MOVE = (1 << 4),
	INPUT_BLOCK = (1 << 5),
	INPUT_LIGHT = (1 << 6),
	INPUT_MEDIUM = (1 << 7),
	INPUT_HEAVY = (1 << 8),
	INPUT_SPECIAL = (1 << 9),
	INPUT_START = (1 << 10),
	INPUT_SELECT = (1 << 11)
};

enum State
{
	STATE_IDLE = (1 << 0),
	STATE_JUMPING = (1 << 1),
	STATE_HIT = (1 << 2),
	STATE_ACTING = (1 << 3),
	STATE_KNOCKDOWN = (1 << 4),
	STATE_AIRHIT = (1 << 5),
	STATE_AIRACTING = (1 << 6),
};

class Character
{
private:
	AnimationHandler* _animation;
	ActionListLoader _reader;
	std::vector<Action*> _actions;

	sf::Vector2f _velocity;
	sf::Vector2i _position;
	int _frameNumber;
	int _action = 0;
	bool _complete;

public:
	spine::SkeletonDrawable* GetDrawable() { return _animation->GetDrawable(); };
	sf::Vector2i GetPosition() { return _animation->GetPosition(); };
	sf::Vector2f GetVelocity() { return _velocity; };
	bool GetComplete() { return _complete; };

	void SetComplete(bool Complete) { _complete = Complete; };
	
	void SetFrameNumber(int FrameNumber) { _frameNumber = FrameNumber; };
	void SetAction(int Action) { _action = Action; };

	void SetPosition(sf::Vector2i Position);
	void SetPosition(float x, float y);
	void SetVelocity(sf::Vector2f Velocity) { _velocity = Velocity; };
	void SetVelocity(float x, float y) { _velocity = sf::Vector2f(x, y); };

	void HandleInputs(int inputs);
	void Draw(sf::RenderWindow* Window);
	void Update(float Delta);

	Character();
	Character(const char* jsonFilepath, const char* atlasFilepath, sf::Vector2i Position = sf::Vector2i(320, 320));
	~Character();
};

#endif