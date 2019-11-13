#ifndef _ACTIONLISTREADER_H
#define _ACTIONLISTREADER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <fstream>

struct Interrupt
{
	std::string name;
	int startFrame;
	int endFrame;
};

struct Hitbox
{
	std::string name;
	int damage;
};

struct Anim
{
	Anim()
	{
		name = "";
		frames = 0;
		loop = false;
		velocity = sf::Vector2f(0, 0);
		hitboxes = std::vector<Hitbox>();
	}

	std::string name;
	int frames;
	bool loop;
	sf::Vector2f velocity;
	std::vector<Hitbox> hitboxes;
};

struct Action
{
	Action()
	{
		name = "";
		complete = false;
		currentAnimation = 0;
		currentFrame = 0;
		animations = std::vector<Anim>();
		direction = 5;
		directions = std::vector<std::string>();
		buttons = std::vector<std::string>();
		interruptions = std::vector<Interrupt>();
	}

	void Copy(Action action)
	{
		name = action.name;
		complete = action.complete;
		currentFrame = action.currentFrame;
		currentAnimation = action.currentAnimation;
		animations = action.animations;
		direction = action.direction;
		directions = action.directions;
		buttons = action.buttons;
		interruptions = action.interruptions;
	}

	std::string name;
	bool complete;
	int currentFrame;
	int currentAnimation;
	std::vector<Anim> animations;
	int direction;
	std::vector<std::string> directions;
	std::vector<std::string> buttons;
	std::vector<Interrupt> interruptions;
};

class ActionListReader
{
private:
	std::vector<Action> _actions;

public:
	std::vector<Action> ReadList(std::string Filepath);

	ActionListReader();
	~ActionListReader();
};

#endif