#pragma once
#include "State.hpp"
#include "StateIdentifiers.hpp"

#include <vector>
#include <utility>
#include <functional>
#include <map>

class Game;
class Player;
class GameTimer;
class ID3D12GraphicsCommandList;

class StateStack
{
public:
	enum Action
	{
		Push,
		Pop,
		Clear,
	};


public:
	StateStack(State::Context context);

	template <typename T>
	void				registerState(States::ID stateID);

	void				buildScene();
	void				update(const GameTimer& gt, States::ID idOfRenderedState);
	void				draw(Game* game, States::ID idOfRenderedState);
	void				handleEvent(States::ID idOfRenderedState);

	void				pushState(States::ID stateID);
	void				popState();
	void				clearStates();

	bool				isEmpty() const;
	void				applyPendingChanges();

private:
	State::Ptr			createState(States::ID stateID);

private:
	struct PendingChange
	{
		explicit			PendingChange(Action action, States::ID stateID = States::NUM_STATES);

		Action				action;
		States::ID			stateID;
	};


private:
	std::vector<State::Ptr>								mStack;
	std::vector<PendingChange>							mPendingList;

	std::map<States::ID, std::function<State::Ptr()>>	mFactories;

	State::Context										mContext;
};


template <typename T>
void StateStack::registerState(States::ID stateID)
{
	mFactories[stateID] = [this]()
	{
		return State::Ptr(new T(*this, mContext));
	};
}