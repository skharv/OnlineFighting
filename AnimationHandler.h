#ifndef _ANIMATION_HANDLER_H
#define _ANIMATION_HANDLER_H

#include <spine\spine-sfml.h>
#include <SFML/Graphics.hpp>
#include <ActionListLoader.h>

#define REALDEALTA 0.016755

class AnimationHandler
{
private:	
	spine::SkeletonDrawable* _drawable;
	spine::Skeleton* _skeleton;

	bool _flip = false;
	std::string _animationName;
public:
	void Update(float Delta, int& CurrentAction);
	void Draw(sf::RenderWindow* Window);

	spine::SkeletonDrawable* GetDrawable() { return _drawable; };
	sf::Vector2i GetPosition() { return sf::Vector2i(_skeleton->getX(), _skeleton->getY()); };

	void SetPosition(sf::Vector2i Position);
	void SetPosition(float x, float y);
	void SetAnimation(Action NewAction);

	spine::Skeleton* GetSkeleton() { return _skeleton; };

	AnimationHandler();
	AnimationHandler(const char* jsonFilepath, const char* atlasFilepath, sf::Vector2i Position = sf::Vector2i(320, 320));
	~AnimationHandler();
};

#endif