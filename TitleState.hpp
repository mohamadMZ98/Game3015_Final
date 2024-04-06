#pragma once
#include "State.hpp"
#include "SceneNode.hpp"

class TitleState : public State
{
public:
	TitleState(StateStack& stack, Context context);
	~TitleState();

	void		buildScene() override;
	void		draw(Game* game) override;
	void		update(const GameTimer& gt) override;
	void		handleEvent() override;


private:
	float promptEffectTime;
	bool showPrompt;

	SceneNode*	mBackgroundSprite;
	SceneNode*	mPromptText;
};