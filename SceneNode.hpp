#pragma once
#include "Common/d3dApp.h"
#include "Common/MathHelper.h"
#include "Common/UploadBuffer.h"
#include "Common/GeometryGenerator.h"
#include "Common/Camera.h"
#include "FrameResource.h"
#include "Command.hpp"
#include "Category.hpp"
#include "StateIdentifiers.hpp"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")

// Lightweight structure stores parameters to draw a shape.  This will
// vary from app-to-app.
struct RenderItem
{
	RenderItem() = default;
	XMFLOAT4X4 World = MathHelper::Identity4x4();

	XMFLOAT4X4 TexTransform = MathHelper::Identity4x4();
	int NumFramesDirty = gNumFrameResources;

	UINT ObjCBIndex = -1;

	Material* Mat = nullptr;
	MeshGeometry* Geo = nullptr;

	D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	int BaseVertexLocation = 0;

	States::ID parentState;
};

class Game;

class SceneNode
{
public:
	enum Type
	{
		None,
		Eagle,
		Raptor,
		Background,
		MenuBackground,
		TitleScreenPrompt,
		MainMenuBackground,
		PlayPrompt,
		QuitPrompt,
		MenuSelection,
		ContinuePrompt
	};

	typedef std::unique_ptr<SceneNode> Ptr;


public:
	SceneNode(Game* game, Type type);

	void					attachChild(SceneNode* child);
	//SceneNode&				detachChild(const SceneNode& node);

	void					update(const GameTimer& gt);
	void					draw(Game* game);
	void					build(States::ID stateID);

	XMFLOAT3				getWorldPosition() const;
	void					setPosition(float x, float y, float z);
	XMFLOAT3				getWorldRotation() const;
	void					setWorldRotation(float x, float y, float z);
	XMFLOAT3				getWorldScale() const;
	void					setScale(float x, float y, float z);

	XMFLOAT4X4				getWorldTransform() const;
	XMFLOAT4X4				getTransform() const;

	void					move(float x, float y, float z);
	unsigned int			getCategory() const { return mCategory; }

	void					onCommand(const Command& command, const GameTimer& timer);

	void					setVelocity(XMFLOAT2 velocity);
	void					setVelocity(float vx, float vy);
	XMFLOAT2				getVelocity() const;
	void					accelerate(XMFLOAT2 velocity);
	void					accelerate(float vx, float vy);

	void					drawCurrent(Game* game);
	void					updateChild(int index, const GameTimer& gt);

	void					updateCurrent(const GameTimer& gt);
	void					updateChildren(const GameTimer& gt);

private:
	void					drawChildren(Game* game);
	void					buildCurrent(States::ID stateID);
	void					buildChildren(States::ID stateID);

	Game*					mGame = nullptr;
	RenderItem*				renderer;

private:
	XMFLOAT3				mWorldPosition;
	XMFLOAT3				mWorldRotation;
	XMFLOAT3				mWorldScaling;

	XMFLOAT2				mVelocity;

	std::vector<SceneNode*> mChildren;
	SceneNode*				mParent;

	std::string				mSprite;
	bool					mIsVisualized;
	int						mCategory;
	SceneNode::Type			mType;
};
