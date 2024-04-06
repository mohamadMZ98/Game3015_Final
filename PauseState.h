#pragma once
#include "State.hpp"
#include "SceneNode.hpp"

class PauseState : public State
{
public:
	PauseState(StateStack& stack, Context context);

	void			buildScene() override;
	void			draw(Game* game) override;
	void			update(const GameTimer& gt) override;
	void			handleEvent() override;

	void			updateOptionText();

private:
	enum OptionNames
	{
		Play,
		Exit,
	};

private:
	std::size_t				mOptionIndex;
	const float menuSelectionDelay = 0.35f;
	float timeSinceLastKeyPressed = 0.0f;

	SceneNode* mMenuBackground;
	SceneNode* mContinuePrompt;
	SceneNode* mQuitPrompt;
	SceneNode* mMenuSelect;
};