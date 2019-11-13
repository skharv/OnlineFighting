#ifndef _CURRENTACTION_H
#define _CURRENTACTION_H

#include <SFML/Graphics.hpp>
#include <spine/spine-sfml.h>

struct ActionVector
{
	sf::Vector2f _desiredVelocity;
	int _startFrame;
	int _endFrame;
};

class CurrentAction
{
private:
	std::string _name;

	std::string _startupName;
	int _startupDuration;

	std::string _activeName;
	int _activeDuration;

	std::string _recoveryName;
	int _recoveryDuration;

	std::vector<std::string> _hitboxes;
	std::vector<std::string> _safeboxes;
	std::vector<ActionVector> _actionVectors;

public:
	void Update();
	sf::Vector2f GetCurrentVector();
	std::string GetCurrentAnimation();
	int GetCurrentAnimationFrameLength();

	bool LoadAnimations(spine::AnimationState AnimState);

	std::string GetName() { return _name; };

};

#endif