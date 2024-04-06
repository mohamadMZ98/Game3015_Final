#include "SceneNode.hpp"
#include "Game.hpp"
#include "Command.hpp"


SceneNode::SceneNode(Game* game, SceneNode::Type type)
	: mParent(nullptr)
	, mGame(game)
	, mVelocity(0,0)
	, mWorldPosition(0,0,0)
	, mWorldScaling(1,1,1)
	, mWorldRotation(0,0,0)
	, renderer()
	, mType(type)
	, mIsVisualized()
{
}

void SceneNode::attachChild(SceneNode* child)
{
	child->mParent = this;
	mChildren.push_back(child);
}

//SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
//{
//	auto found = std::find_if(mChildren.begin(), mChildren.end(), [&](Ptr& p) { return p.get() == &node; });
//	assert(found != mChildren.end());
//
//	Ptr result = std::move(*found);
//	result->mParent = nullptr;
//	mChildren.erase(found);
//	return result;
//}

void SceneNode::update(const GameTimer& gt)
{
	updateCurrent(gt);
	updateChildren(gt);
}

void SceneNode::updateCurrent(const GameTimer& gt)
{
	if (!mIsVisualized) return;

	LPCWSTR msgbuf = L"Updating Scene Node\n";
	OutputDebugString(msgbuf);

	XMFLOAT2 mV;
	mV.x = mVelocity.x * gt.DeltaTime();
	mV.y = mVelocity.y * gt.DeltaTime();

	move(mV.x, 0, mV.y);

	renderer->World = getWorldTransform();
	renderer->NumFramesDirty++;
}

void SceneNode::updateChild(int index, const GameTimer& gt)
{
	if (mChildren.empty()) return;
	mChildren[index]->update(gt);
}

void SceneNode::updateChildren(const GameTimer& gt)
{
	for (SceneNode* child : mChildren)
		child->update(gt);
}

void SceneNode::draw(Game* game)
{
	drawCurrent(game);
	drawChildren(game);
}

void SceneNode::drawCurrent(Game* game)
{
	if (!mIsVisualized) return;

	LPCWSTR msgbuf = L"Drawing Scene Node\n";
	OutputDebugString(msgbuf);

	UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
	UINT matCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(MaterialConstants));

	auto objectCB = game->GetCurrentFrameResource()->ObjectCB->Resource();
	auto matCB = game->GetCurrentFrameResource()->MaterialCB->Resource();

	game->GetCommandList().Get()->IASetVertexBuffers(0, 1, &renderer->Geo->VertexBufferView());
	game->GetCommandList().Get()->IASetIndexBuffer(&renderer->Geo->IndexBufferView());
	game->GetCommandList().Get()->IASetPrimitiveTopology(renderer->PrimitiveType);

	//step18
	CD3DX12_GPU_DESCRIPTOR_HANDLE tex(game->getDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());
	tex.Offset(renderer->Mat->DiffuseSrvHeapIndex, game->getDescriptorSize());

	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress() + renderer->ObjCBIndex * objCBByteSize;
	D3D12_GPU_VIRTUAL_ADDRESS matCBAddress = matCB->GetGPUVirtualAddress() + renderer->Mat->MatCBIndex * matCBByteSize;

	//step19
	game->GetCommandList().Get()->SetGraphicsRootDescriptorTable(0, tex);
	game->GetCommandList().Get()->SetGraphicsRootConstantBufferView(1, objCBAddress);
	game->GetCommandList().Get()->SetGraphicsRootConstantBufferView(3, matCBAddress);

	game->GetCommandList().Get()->DrawIndexedInstanced(renderer->IndexCount, 1, renderer->StartIndexLocation, renderer->BaseVertexLocation, 0);
}

void SceneNode::drawChildren(Game* game)
{
	for (SceneNode* child : mChildren)
		child->draw(game);
}

void SceneNode::build(States::ID stateID)
{
	buildCurrent(stateID);
	buildChildren(stateID);
}

void SceneNode::buildCurrent(States::ID stateID)
{
	switch (mType)
	{
	case (SceneNode::Eagle):
		mSprite = "Eagle";
		mIsVisualized = true;
		mCategory = Category::PlayerAircraft;
		break;
	case (SceneNode::Raptor):
		mSprite = "Raptor";
		mIsVisualized = true;
		mCategory = Category::AlliedAircraft;
		break;
	case(SceneNode::Background):
		mSprite = "Desert";
		mIsVisualized = true;
		mCategory = Category::Scene;
		break;
	case(SceneNode::MenuBackground):
		mSprite = "MenuBG";
		mIsVisualized = true;
		mCategory = Category::Scene;
		break;
	case(SceneNode::TitleScreenPrompt):
		mSprite = "PromptText";
		mIsVisualized = true;
		mCategory = Category::Scene;
		break;
	case(SceneNode::MainMenuBackground):
		mSprite = "MainMenuBG";
		mIsVisualized = true;
		mCategory = Category::Scene;
		break;
	case(SceneNode::PlayPrompt):
		mSprite = "PlayText";
		mIsVisualized = true;
		mCategory = Category::Scene;
		break;
	case(SceneNode::QuitPrompt):
		mSprite = "QuitText";
		mIsVisualized = true;
		mCategory = Category::Scene;
		break;
	case (SceneNode::MenuSelection):
		mSprite = "MenuSelection";
		mIsVisualized = true;
		mCategory = Category::Scene;
		break;
	case(SceneNode::ContinuePrompt):
		mSprite = "ContinuePrompt";
		mIsVisualized = true;
		mCategory = Category::Scene;
		break;
	default:
		mIsVisualized = false;
		mCategory = Category::Scene;
		return;
	}

	auto render = std::make_unique<RenderItem>();
	renderer = render.get();
	renderer->World = getTransform();
	if (mType == Background) XMStoreFloat4x4(&renderer->TexTransform, XMMatrixScaling(10.0f, 10.0f, 10.0f));
	renderer->ObjCBIndex = mGame->getRenderItems().size();
	renderer->Mat = mGame->getMaterials()[mIsVisualized ? mSprite : "MenuSelection"].get();
	renderer->Geo = mGame->getGeometries()["boxGeo"].get();
	renderer->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	renderer->IndexCount = renderer->Geo->DrawArgs["box"].IndexCount;
	renderer->StartIndexLocation = renderer->Geo->DrawArgs["box"].StartIndexLocation;
	renderer->BaseVertexLocation = renderer->Geo->DrawArgs["box"].BaseVertexLocation;
	renderer->parentState = stateID;

	if (!mIsVisualized) return;

	LPCWSTR msgbuf = L"Building Scene Node\n";
	OutputDebugString(msgbuf);

	mGame->getRenderItems().push_back(std::move(render));
}

void SceneNode::buildChildren(States::ID stateID)
{
	for (int i = 0; i < mChildren.size(); i++) {
		mChildren[i]->build(stateID);
	}
}

XMFLOAT3 SceneNode::getWorldPosition() const
{
	return mWorldPosition;
}

void SceneNode::setPosition(float x, float y, float z)
{
	mWorldPosition = XMFLOAT3(x, y, z);
}

XMFLOAT3 SceneNode::getWorldRotation() const
{
	return mWorldRotation;
}

void SceneNode::setWorldRotation(float x, float y, float z)
{
	mWorldRotation = XMFLOAT3(x, y, z);
}

XMFLOAT3 SceneNode::getWorldScale() const
{
	return mWorldScaling;
}

void SceneNode::setScale(float x, float y, float z)
{
	mWorldScaling = XMFLOAT3(x, y, z);
}

XMFLOAT4X4 SceneNode::getWorldTransform() const
{
	XMFLOAT4X4 transform = MathHelper::Identity4x4();
	XMMATRIX T = XMLoadFloat4x4(&transform);

	for (const SceneNode* node = this; node != nullptr; node = node->mParent)
	{
		XMMATRIX Tp = XMLoadFloat4x4(&node->getTransform());
		T = Tp * T;
	}
	XMStoreFloat4x4(&transform, T);

	return transform;
}

XMFLOAT4X4 SceneNode::getTransform() const
{
	XMFLOAT4X4 transform;
	XMStoreFloat4x4(&transform, XMMatrixScaling(mWorldScaling.x, mWorldScaling.y, mWorldScaling.z) *
		XMMatrixRotationX(mWorldRotation.x) *
		XMMatrixRotationY(mWorldRotation.y) *
		XMMatrixRotationZ(mWorldRotation.z) *
		XMMatrixTranslation(mWorldPosition.x, mWorldPosition.y, mWorldPosition.z));
	return transform;
}

void SceneNode::move(float x, float y, float z)
{
	mWorldPosition.x += x;
	mWorldPosition.y += y;
	mWorldPosition.z += z;
}

void SceneNode::onCommand(const Command& command, const GameTimer& timer)
{
	if (command.category & getCategory()){
		command.action(*this, timer);
		LPCWSTR msgbuf = L"Player Aircraft scene node executing command\n";
		OutputDebugString(msgbuf);
	}
	for (SceneNode* child : mChildren)
		child->onCommand(command, timer);

	/*for (int i = 0; i < mChildren.size(); i++) {
		mChildren[i]->onCommand(command, timer);
	}*/
}

void SceneNode::setVelocity(XMFLOAT2 velocity)
{
	mVelocity = velocity;
}

void SceneNode::setVelocity(float vx, float vy)
{
	mVelocity.x = vx;
	mVelocity.y = vy;
}

XMFLOAT2 SceneNode::getVelocity() const
{
	return mVelocity;
}

void SceneNode::accelerate(XMFLOAT2 velocity)
{
	mVelocity.x += velocity.x;
	mVelocity.y += velocity.y;
}

void SceneNode::accelerate(float vx, float vy)
{
	mVelocity.x += vx;
	mVelocity.y += vy;
}
