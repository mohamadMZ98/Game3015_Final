#include "GameState.h"
#include "Game.hpp"

GameState::GameState(StateStack& stack, Context context) : State (stack, context)
{
	mWorld = new World(context.window);
}

GameState::~GameState()
{
}

void GameState::buildScene()
{
	mWorld->buildScene(stateID);
}

void GameState::draw(Game* game)
{
	mWorld->draw(game);
}

void GameState::update(const GameTimer& gt)
{
	mWorld->update(gt);
}

void GameState::handleEvent()
{
	mContext.player->handleRealtimeInput(*mWorld->getCommandQueue());
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		mContext.window->SetRenderedState(States::ID::Pause);
		LPCWSTR msgbuf = L"Pausing Game\n";
		OutputDebugString(msgbuf);
	}
}


