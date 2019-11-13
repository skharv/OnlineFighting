#include "Character.h"

void Character::SetPosition(sf::Vector2f Position)
{
	_position = Position;
	_animation->SetPosition(_position);
}

void Character::SetPosition(float x, float y)
{
	_position = sf::Vector2f(x, y);
	_animation->SetPosition(_position);
}

void Character::Draw(sf::RenderWindow* Window)
{
	_animation->Draw(Window);
}

void Character::Update(float Delta, int& Action)
{
	_animation->Update(Delta, Action);
}

Character::Character()
{

}

Character::Character(const char* jsonFilepath, const char* atlasFilepath, sf::Vector2f Position)
{
	_animation = new AnimationHandler(jsonFilepath, atlasFilepath, Position);
	_reader = ActionListReader();
	_actions = _reader.ReadList("Resources/BlankMovelist.txt");

	_frameNumber = 0;
	_complete = true;
}

Character::~Character()
{
}
