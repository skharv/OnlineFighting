#include "Character.h"

void Character::Draw(sf::RenderWindow* Window)
{
	Window->draw(*_drawable);
}

void Character::Update(float Delta)
{
	if (action == 1)
	{
		_drawable->state->setAnimation(0, "m_rollb_s", false);
		_drawable->state->addAnimation(0, "m_rollb_a", false, 0);
		_drawable->state->addAnimation(0, "m_rollb_r", false, 0);
		_drawable->state->addAnimation(0, "m_stand", true, 0);
		action = 0;
	}

	if (action == 2)
	{
		_drawable->state->setAnimation(0, "m_rollf_s", false);
		_drawable->state->addAnimation(0, "m_rollf_a", false, 0);
		_drawable->state->addAnimation(0, "m_rollf_r", false, 0);
		_drawable->state->addAnimation(0, "m_stand", true, 0);
		action = 0;
	}

	_drawable->skeleton->setPosition(_position.x, _position.y);
	_drawable->update(Delta);
}

Character::Character()
{
	//spine::SkeletonData *skeleData = new spine::SkeletonData();

	//spine::AnimationStateData stateData = spine::AnimationStateData(skeleData);
	//_drawable = spine::SkeletonDrawable(skeleData, &stateData);
	//_skeleton = _drawable.skeleton;
}

Character::Character(const char* jsonFilepath, const char* atlasFilepath, sf::Vector2f Position)
{
	spine::SFMLTextureLoader textureLoader;
	spine::Atlas *atlas = new spine::Atlas(atlasFilepath, &textureLoader);
	spine::SkeletonJson json = spine::SkeletonJson(atlas);
	spine::SkeletonData* skeleData = json.readSkeletonDataFile(jsonFilepath);

	spine::AnimationStateData* stateData = new spine::AnimationStateData(skeleData);

	stateData->setMix("m_walk_b", "m_walk_f", 1.0f);
	stateData->setMix("m_walk_f", "m_walk_b", 1.0f);

	_drawable = new spine::SkeletonDrawable(skeleData, stateData);
	_drawable->timeScale = 1;
	_drawable->setUsePremultipliedAlpha(true);

	_skeleton = _drawable->skeleton;
	_skeleton->setToSetupPose();

	_skeleton->setPosition(Position.x, Position.y);
	_skeleton->updateWorldTransform();

	_drawable->state->addAnimation(0, "m_stand", true, 0);
}

Character::~Character()
{
}
