#ifndef _CHARACTER_H
#define _CHARACTER_H

#include <spine/spine-sfml.h>
#include <SFML/Graphics.hpp>

class Character
{
private:
	spine::Skeleton* _skeleton;
	spine::SkeletonDrawable* _drawable;

public:
	int action = 0;
	sf::Vector2f _position;

	void Draw(sf::RenderWindow* Window);
	void Update(float Delta);

	Character();
	Character(const char* jsonFilepath, const char* atlasFilepath, sf::Vector2f Position = sf::Vector2f(320, 320));
	~Character();
};

#endif