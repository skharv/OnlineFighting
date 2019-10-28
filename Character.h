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
	void Draw(sf::RenderWindow* Window);
	void Update(float Delta);

	Character();
	Character(const char* jsonFilepath, const char* atlasFilepath);
	~Character();
};

#endif