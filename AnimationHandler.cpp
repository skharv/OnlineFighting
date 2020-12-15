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

void AnimationHandler::Update(float Delta, int& Action)
{
	switch (Action)
	{
	case 0:
		break;
	case 1:
		if (_drawable->state->getCurrent(0)->getAnimation()->getName() != "m_stand")
			_drawable->state->setAnimation(0, "m_stand", true);
		break;
	case 2:
		if (_drawable->state->getCurrent(0)->getAnimation()->getName() != "m_walk_f")
			_drawable->state->setAnimation(0, "m_walk_f", true);
		break;
	case 3:
		if (_drawable->state->getCurrent(0)->getAnimation()->getName() != "m_walk_b")
			_drawable->state->setAnimation(0, "m_walk_b", true);
		break;
	case 4:
		_drawable->state->setAnimation(0, "m_rollf_s", false);
		_drawable->state->addAnimation(0, "m_rollf_a", false, 0);
		_drawable->state->addAnimation(0, "m_rollf_r", false, 0);
		break;
	case 5:
		_drawable->state->setAnimation(0, "m_rollb_s", false);
		_drawable->state->addAnimation(0, "m_rollb_a", false, 0);
		_drawable->state->addAnimation(0, "m_rollb_r", false, 0);
		break;
	case 6:
		break;
	case 7:
		break;
	default:
		break;
	}

	_drawable->update(Delta);

	if (_drawable->state->getCurrent(0)->isComplete() && _drawable->state->getCurrent(0)->getNext() == NULL)
	{
		Action = 0;
	}
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
