#include "Entity.hpp"
#include <Windows.h>

Entity::Entity(Game* game) : SceneNode(game), mVelocity(0, 0)
{
}

void Entity::setVelocity(XMFLOAT2 velocity)
{
	mVelocity = velocity;
}

void Entity::setVelocity(float vx, float vy)
{
	mVelocity.x = vx;
	mVelocity.y = vy;
}

XMFLOAT2 Entity::getVelocity() const
{
	return mVelocity;
}

void Entity::updateCurrent(const GameTimer& gt)
{
	XMFLOAT2 mV;
	mV.x = mVelocity.x * gt.DeltaTime();
	mV.y = mVelocity.y * gt.DeltaTime();

	//switched axes z and y to align movement with camera view
	move(mV.x,0, mV.y);

	renderer->World = getWorldTransform();
	renderer->NumFramesDirty++;
}

void Entity::accelerate(XMFLOAT2 velocity)
{
	mVelocity.x = mVelocity.x + velocity.x;
	mVelocity.y = mVelocity.y + velocity.y;
}

void Entity::accelerate(float vx, float vy)
{
	mVelocity.x += vx;
	mVelocity.y += vy;
}
