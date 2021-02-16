#include "Character.h"

void Character::SetPosition(sf::Vector2i Position)
{
	_position = Position;
	_animation->SetPosition(_position);
}

void Character::SetPosition(float x, float y)
{
	_position = sf::Vector2i(x, y);
	_animation->SetPosition(_position);
}

void Character::HandleInputs(int inputs)
{
	//if (inputs & INPUT_UP)
	//{
	//}

	//if (inputs & INPUT_DOWN)
	//{
	//}

	//if (inputs & INPUT_LEFT)
	//{
	//}

	//if (inputs & INPUT_RIGHT)
	//{

	//}

	if (inputs & INPUT_RIGHT)
	{
		_action = 2;
		_velocity = V_WALKFORWARD;
	}
	else if (inputs & INPUT_LEFT)
	{
		_action = 3;
		_velocity = V_WALKBACK;
	}
	else if (inputs & INPUT_LIGHT)
	{
		_action = 4;
		_velocity = V_ROLLFORWARD;
	}
	else if (inputs & INPUT_MEDIUM)
	{
		_action = 5;
		_velocity = V_ROLLBACK;
	}
	else if (inputs & INPUT_HEAVY)
	{
		_action = 6;
	}
	else if (inputs & INPUT_SPECIAL)
	{
		_action = 7;
	}
	else if (inputs & INPUT_MOVE)
	{
		_action = 8;
	}
	else if (inputs & INPUT_BLOCK)
	{
		_action = 9;
	}
}

void Character::Draw(sf::RenderWindow* Window)
{
	_animation->Draw(Window);
}

void Character::Update(float Delta)
{
	_position.x += _velocity.x;
	_position.y += _velocity.y;

	_animation->SetPosition(_position);
	_animation->Update(Delta, _action);

	_velocity = sf::Vector2f(0, 0);
}

Character::Character()
{

}

Character::Character(const char* jsonFilepath, const char* atlasFilepath, sf::Vector2i Position)
{
	_animation = new AnimationHandler(jsonFilepath, atlasFilepath, Position);
	_reader = ActionListLoader();
	_actions = _reader.ReadList("Resources/BlankMovelist.txt");
	_frameNumber = 0;
	_complete = true;
}

Character::~Character()
{
}
