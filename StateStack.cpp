#include "StateStack.hpp"
#include "Game.hpp"

#include <cassert>

StateStack::StateStack(State::Context context)
	: mStack()
	, mPendingList()
	, mFactories()
	, mContext(context)
{
}

void StateStack::buildScene()
{
	for (int i = 0; i < mStack.size(); i++) {
		switch (i) {
			case 0:
				mStack[i]->stateID = States::Title;
				break;
			case 1:
				mStack[i]->stateID = States::Menu;
				break;
			case 2:
				mStack[i]->stateID = States::Game;
				break;
			case 3:
				mStack[i]->stateID = States::Pause;
				break;
		}
		mStack[i]->buildScene();
	}
}

void StateStack::update(const GameTimer& gt, States::ID idOfRenderedState)
{
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (itr->get()->stateID == idOfRenderedState)
			(*itr)->update(gt);
	}

	applyPendingChanges();
}

void StateStack::draw(Game* game, States::ID idOfRenderedState)
{
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (itr->get()->stateID == idOfRenderedState)
			(*itr)->draw(game);
	}
}

void StateStack::handleEvent(States::ID idOfRenderedState)
{
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (itr->get()->stateID == idOfRenderedState)
			itr->get()->handleEvent();
	}

	applyPendingChanges();
}

void StateStack::pushState(States::ID stateID)
{
	mPendingList.push_back(PendingChange(Push, stateID));
}

void StateStack::popState()
{
	mPendingList.push_back(PendingChange(Pop));
}

void StateStack::clearStates()
{
	mPendingList.push_back(PendingChange(Clear));
}

bool StateStack::isEmpty() const
{
	return mStack.empty();
}

State::Ptr StateStack::createState(States::ID stateID)
{
	auto found = mFactories.find(stateID);
	assert(found != mFactories.end());

	return found->second();
}

void StateStack::applyPendingChanges()
{
	for(PendingChange change: mPendingList)
	{
		switch (change.action)
		{
		case Push:
			mStack.push_back(createState(change.stateID));
			break;
		case Pop:
			mStack.pop_back();
			break;
		case Clear:
			mStack.clear();
			break;
		}
	}
	mPendingList.clear();
}

StateStack::PendingChange::PendingChange(Action action, States::ID stateID)
	: action(action)
	, stateID(stateID)
{
}