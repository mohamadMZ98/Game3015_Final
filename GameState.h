#pragma once
#include "State.hpp"
#include "SceneNode.hpp"
#include "World.hpp"

class GameState : public State
{
public:
	GameState(StateStack& stack, Context context);
	~GameState();

	void		buildScene() override;
	void		draw(Game* game) override;
	void		update(const GameTimer& gt) override;
	void		handleEvent() override;

private:
	World* mWorld;
};