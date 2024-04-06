#pragma once
#include "Command.hpp"
#include <map>

class InputCommandQueue;

class Player
{
public:
	enum Action {
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown
	};

	Player();

	void initializeActions();

	void handleEvent(InputCommandQueue& commands);
	void handleRealtimeInput(InputCommandQueue& commands);

	void assignKey(Action action, int key);
	int getAssignedKey(Action action) const;

private:
	//static bool isRealTimeAction(Action action); for later

private: 
	std::map<int, Action> mKeyBinding;
	std::map<Action, Command> mActionBinding;

};