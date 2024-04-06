#include "PauseState.h"
#include "SceneNode.hpp"
#include "Player.h"
#include "Game.hpp"

PauseState::PauseState(StateStack& stack, State::Context context)
	: State(stack, context)
{
	mOptionIndex = 0;
}

void PauseState::buildScene()
{
	SceneNode* backgroundSprite = new SceneNode(mContext.window, SceneNode::Type::MainMenuBackground);
	mMenuBackground = backgroundSprite;
	mMenuBackground->setPosition(0.0f, -0.2f, 0.0f);
	mMenuBackground->setScale(6.0, 1.0, 4.5);

	SceneNode* MenuSelectSprite = new SceneNode(mContext.window, SceneNode::Type::MenuSelection);
	mMenuSelect = MenuSelectSprite;
	mMenuSelect->setPosition(0.0f, 0.1f, 0.0f);
	mMenuSelect->setScale(0.5f, 1.0, 0.1);
	mMenuBackground->attachChild(mMenuSelect);

	SceneNode* PlayPromptSprite = new SceneNode(mContext.window, SceneNode::Type::ContinuePrompt);
	mContinuePrompt = PlayPromptSprite;
	mContinuePrompt->setPosition(0.0f, 0.2f, 0.0f);
	mContinuePrompt->setScale(1.0, 1.0, 0.2);
	mMenuBackground->attachChild(mContinuePrompt);

	SceneNode* QuitPromptSprite = new SceneNode(mContext.window, SceneNode::Type::QuitPrompt);
	mQuitPrompt = QuitPromptSprite;
	mQuitPrompt->setPosition(0.0f, 0.2f, -0.5f);
	mQuitPrompt->setScale(1.0, 1.0, 0.2);
	mMenuBackground->attachChild(mQuitPrompt);

	LPCWSTR msgbuf = L"Building Menu Scene\n";
	OutputDebugString(msgbuf);

	mMenuBackground->build(stateID);
}

void PauseState::draw(Game* game)
{
	mMenuBackground->draw(game);
}

void PauseState::update(const GameTimer& gt)
{
	timeSinceLastKeyPressed += gt.DeltaTime();

	switch (mOptionIndex) {
	case 0:
		if (mMenuSelect->getWorldPosition().z >= 0) mMenuSelect->setVelocity(XMFLOAT2(0, 0));
		break;
	case 1:
		if (mMenuSelect->getWorldPosition().z <= -0.5) mMenuSelect->setVelocity(XMFLOAT2(0, 0));
		break;
	default:
		break;
	}

	mMenuSelect->updateCurrent(gt);
}

void PauseState::handleEvent()
{
	LPCWSTR msgbuf;
	if ((GetAsyncKeyState(VK_DOWN) & 0x8000 || GetAsyncKeyState(VK_UP) & 0x8000) && timeSinceLastKeyPressed > menuSelectionDelay)
	{
		mOptionIndex = (mOptionIndex + 1) % 2;
		timeSinceLastKeyPressed = 0.0f;
		switch (mOptionIndex) {
		case 0:
			mMenuSelect->setVelocity(XMFLOAT2(0, 2));
			msgbuf = L"Continue Selected\n";
			OutputDebugString(msgbuf);
			break;
		case 1:
			mMenuSelect->setVelocity(XMFLOAT2(0, -2));
			msgbuf = L"Quit Selected\n";
			OutputDebugString(msgbuf);
			break;
		default:
			break;
		}
	}
	if (GetAsyncKeyState(' ') & 0x8000)
	{
		switch (mOptionIndex) {
		case 0:
			msgbuf = L"Switching to Play Scene\n";
			mContext.window->SetRenderedState(States::ID::Game);
			OutputDebugString(msgbuf);
			break;
		case 1:
			msgbuf = L"Returning to Title\n";
			mContext.window->SetRenderedState(States::ID::Title);
			OutputDebugString(msgbuf);
			break;
		default:
			break;
		}
	}
}

void PauseState::updateOptionText()
{
}
