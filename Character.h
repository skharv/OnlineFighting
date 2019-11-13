#ifndef _CHARACTER_H
#define _CHARACTER_H

#include <spine/spine-sfml.h>
#include <SFML/Graphics.hpp>
#include "AnimationHandler.h"
#include "ActionListReader.h"

//These will be read in from a text file.
#define V_WALKFORWARD sf::Vector2f(1, 0)
#define V_WALKBACK sf::Vector2f(-1, 0)
#define V_ROLLFORWARD sf::Vector2f(8, 0)
#define V_ROLLBACK sf::Vector2f(-6, 0)

class Character
{
private:
	AnimationHandler* _animation;
	ActionListReader _reader;
	std::vector<Action> _actions;

	sf::Vector2f _velocity, _position;
	int _frameNumber;
	int _action = 0;
	bool _complete;

public:
	spine::SkeletonDrawable* GetDrawable() { return _animation->GetDrawable(); };
	sf::Vector2f GetPosition() { return _animation->GetPosition(); };
	sf::Vector2f GetVelocity() { return _velocity; };
	bool GetComplete() { return _complete; };

	void SetComplete(bool Complete) { _complete = Complete; };
	
	void SetFrameNumber(int FrameNumber) { _frameNumber = FrameNumber; };
	void SetAction(int Action) { _action = Action; };

	void SetPosition(sf::Vector2f Position);
	void SetPosition(float x, float y);
	void SetVelocity(sf::Vector2f Velocity) { _velocity = Velocity; };
	void SetVelocity(float x, float y) { _velocity = sf::Vector2f(x, y); };

	void Draw(sf::RenderWindow* Window);
	void Update(float Delta, int& Action);

	Character();
	Character(const char* jsonFilepath, const char* atlasFilepath, sf::Vector2f Position = sf::Vector2f(320, 320));
	~Character();
};

#endif