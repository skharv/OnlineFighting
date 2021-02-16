#include "AnimationHandler.h"
#include <iostream>

void AnimationHandler::SetPosition(sf::Vector2i Position)
{
	_drawable->skeleton->setPosition(Position.x, Position.y);
}

void AnimationHandler::SetPosition(float x, float y)
{
	_drawable->skeleton->setPosition(x, y);
}

void AnimationHandler::SetAnimation(Action NewAction)
{
	for (int i = 0; i < NewAction.animations.size(); i++)
	{
		if (i == 0)
		{
			_drawable->state->setAnimation(0, NewAction.animations[i].name, false);
		}
		else
		{
			_drawable->state->addAnimation(0, NewAction.animations[i].name, false, 0);
		}
	}
}

void AnimationHandler::Update(float Delta, int& CurrentAction)
{
	_drawable->update(Delta);
}

void AnimationHandler::Draw(sf::RenderWindow* Window)
{
	Window->draw(*_drawable);
}

AnimationHandler::AnimationHandler()
{
}

AnimationHandler::AnimationHandler(const char* jsonFilepath, const char* atlasFilepath, sf::Vector2i Position)
{
	spine::SFMLTextureLoader textureLoader;
	spine::Atlas* atlas = new spine::Atlas(atlasFilepath, &textureLoader);
	spine::SkeletonJson json = spine::SkeletonJson(atlas);
	spine::SkeletonData* skeleData = json.readSkeletonDataFile(jsonFilepath);

	spine::AnimationStateData* stateData = new spine::AnimationStateData(skeleData);

	stateData->setMix("m_walk_b", "m_walk_f", 0.2f);
	stateData->setMix("m_walk_f", "m_walk_b", 0.2f);
	stateData->setMix("m_walk_b", "m_stand", 0.2f);
	stateData->setMix("m_walk_f", "m_stand", 0.2f);
	stateData->setMix("m_stand", "m_walk_f", 0.2f);
	stateData->setMix("m_stand", "m_walk_b", 0.2f);

	_drawable = new spine::SkeletonDrawable(skeleData, stateData);
	_drawable->timeScale = 1;
	_drawable->setUsePremultipliedAlpha(true);

	_skeleton = _drawable->skeleton;
	_skeleton->setToSetupPose();

	_skeleton->setPosition(Position.x, Position.y);
	_skeleton->updateWorldTransform();

	_drawable->state->addAnimation(0, "m_stand", true, 0);
}

AnimationHandler::~AnimationHandler()
{
}
