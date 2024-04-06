#pragma once
#include "StateIdentifiers.hpp"
#include "ResourceIdentifiers.hpp"

#include <memory>


class StateStack;
class Game;
class Player;
class ID3D12GraphicsCommandList;
class GameTimer;

class State
{
public:
	typedef std::unique_ptr<State> Ptr;

	States::ID stateID;

	struct Context
	{
		Context(Game* window, Player* player);

		Game* window;
		Player* player;
	};

public:
	State(StateStack& stack, Context context);
	~State();

	virtual void		buildScene(){};
	virtual void		draw(Game* game){};
	virtual void		update(const GameTimer& gt){};
	virtual void		handleEvent(){};

protected:
	void				requestStackPush(States::ID stateID);
	void				requestStackPop();
	void				requestStateClear();

	Context				getContext();

protected:
	StateStack*			mStack;
	Context				mContext;
};