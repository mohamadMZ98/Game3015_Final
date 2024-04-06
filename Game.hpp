#include "SceneNode.hpp"
#include "Player.h"
#include "StateStack.hpp"

class TitleState;
class MenuState;
class StateStack;

class Game : public D3DApp
{
public:
	Game(HINSTANCE hInstance);
	Game(const Game& rhs) = delete;
	Game& operator=(const Game& rhs) = delete;
	~Game();

	virtual bool Initialize()override;
	FrameResource* GetCurrentFrameResource();

	void LoadTextures();
	void BuildMaterials();
	void BuildDescriptorHeaps();
	void BuildRenderItems();

	void SetRenderedState(States::ID id);

private:
	virtual void OnResize()override;
	virtual void Update(const GameTimer& gt)override;
	virtual void Draw(const GameTimer& gt)override;

	virtual void OnMouseDown(WPARAM btnState, int x, int y)override;
	virtual void OnMouseUp(WPARAM btnState, int x, int y)override;
	virtual void OnMouseMove(WPARAM btnState, int x, int y)override;

	void OnKeyboardInput(const GameTimer& gt);
	void ProcessInput(const GameTimer& gt);
	void UpdateCamera(const GameTimer& gt);
	void AnimateMaterials(const GameTimer& gt);
	void UpdateObjectCBs(const GameTimer& gt);
	void UpdateMaterialCBs(const GameTimer& gt);
	void UpdateMainPassCB(const GameTimer& gt);
	void DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems);
	
	void BuildShapeGeometry();
	void BuildRootSignature();
	void BuildShadersAndInputLayout();
	void BuildFrameResources();
	void BuildPSOs();

	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();

	void RegisterStates();

private:
	Player mPlayer;
	StateStack* mStateStack;
	States::ID idOfRenderedState;

	std::vector<std::unique_ptr<FrameResource>> mFrameResources;
	FrameResource* mCurrFrameResource = nullptr;
	int mCurrFrameResourceIndex = 0;

	UINT mCbvSrvDescriptorSize = 0;

	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;

	//step11
	ComPtr<ID3D12DescriptorHeap> mSrvDescriptorHeap = nullptr;

	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries;
	std::unordered_map<std::string, std::unique_ptr<Material>> mMaterials;

	//step7
	std::unordered_map<std::string, std::unique_ptr<Texture>> mTextures;

	std::unordered_map<std::string, ComPtr<ID3DBlob>> mShaders;

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

	ComPtr<ID3D12PipelineState> mOpaquePSO = nullptr;

	// List of all the render items.
	std::vector<std::unique_ptr<RenderItem>> mAllRitems;

	// Render items divided by PSO.
	std::vector<RenderItem*> mOpaqueRitems;

	PassConstants mMainPassCB;

	XMFLOAT3 mEyePos = { 0.0f, 0.0f, -10.0f };
	XMFLOAT4X4 mView = MathHelper::Identity4x4();
	XMFLOAT4X4 mProj = MathHelper::Identity4x4();

	float mTheta = 1.3f * XM_PI;
	float mPhi = 0.4f * XM_PI;
	float mRadius = 2.5f;

	POINT mLastMousePos;
	Camera mCamera;
	

public:
	Microsoft::WRL::ComPtr<ID3D12Device>& GetDevice() { return md3dDevice; }
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& GetCommandList() { return mCommandList; }

	std::vector<std::unique_ptr<RenderItem>>& getRenderItems() { return mAllRitems; }
	std::vector<RenderItem*>& getOpaqueItems() {return mOpaqueRitems; }
	std::unordered_map<std::string, std::unique_ptr<Material>>& getMaterials() { return mMaterials; }
	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>>& getGeometries() { return mGeometries; }
	ComPtr<ID3D12DescriptorHeap>& getDescriptorHeap() { return mSrvDescriptorHeap; }
	UINT getDescriptorSize() { return mCbvSrvDescriptorSize; }

	std::unordered_map<std::string, std::unique_ptr<Texture>>& GetTextures() { return mTextures; };
	std::unordered_map<std::string, std::unique_ptr<Material>>& GetMaterials() { return mMaterials; };
};
