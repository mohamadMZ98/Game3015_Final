#include "Player.h"
#include "CommandQueue.hpp"
#include "SceneNode.hpp"
#include <map>
#include <string>
#include <algorithm>
#include <iostream>

#include <Windows.h>

struct AircraftMover
{
	AircraftMover(float vx, float vy) :
		velocity(vx, vy)
	{
	}
	void operator() (SceneNode& node, const GameTimer& gt) const
	{
		SceneNode& aircraft = static_cast<SceneNode&>(node);
		aircraft.accelerate(velocity);
	}
	XMFLOAT2 velocity;
};

Player::Player()
{
	assignKey(MoveLeft, 'A');
	assignKey(MoveRight, 'D');
	assignKey(MoveUp, 'W');
	assignKey(MoveDown, 'S');

	initializeActions();

	for (auto& pair : mActionBinding)
		pair.second.category = Category::PlayerAircraft;
}

void Player::initializeActions()
{
	float moveSpeed = 5.f;
	mActionBinding[MoveLeft].action = derivedAction<SceneNode>(AircraftMover(-moveSpeed, 0.f));
	mActionBinding[MoveRight].action = derivedAction<SceneNode>(AircraftMover(moveSpeed, 0.f));
	mActionBinding[MoveUp].action = derivedAction<SceneNode>(AircraftMover(0.f, moveSpeed));
	mActionBinding[MoveDown].action = derivedAction<SceneNode>(AircraftMover(0.f, -moveSpeed));
}

void Player::handleEvent(InputCommandQueue& commands)
{
}

void Player::handleRealtimeInput(InputCommandQueue& commands)
{
	for (auto pair : mKeyBinding)
	{
		if (GetAsyncKeyState(pair.first) & 0x8000){// && isRealtimeAction(pair.second))
			commands.push(mActionBinding[pair.second]);
		}
	}
}

void Player::assignKey(Action action, int key)
{
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); )
	{
		if (itr->second == action)
			mKeyBinding.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	mKeyBinding[key] = action;
}

int Player::getAssignedKey(Action action) const
{
	for (auto pair : mKeyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}

	return 0;
}
