#pragma once
#include "SceneNode.hpp"
#include "CommandQueue.hpp"

class World 
{
public:
	explicit							World(Game* window);
	void								update(const GameTimer& gt);
	void								draw(Game* game);

	void								buildScene(States::ID stateID);

	XMVECTOR							getPlayerPosition();
	InputCommandQueue*					getCommandQueue();
	void								adaptPlayerVelocity();

private:
	enum Layer
	{
		Background,
		Air,
		LayerCount
	};


private:
	Game*								mGame;

	SceneNode*							mSceneGraph;

	InputCommandQueue					mInputCommandQueue;

	XMFLOAT4							mWorldBounds;
	XMFLOAT2		    				mSpawnPosition;
	float								mScrollSpeed;
	SceneNode*							mPlayerAircraft;
	SceneNode*							mBackground;
};
