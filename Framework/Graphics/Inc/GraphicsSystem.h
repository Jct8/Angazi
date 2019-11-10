#pragma once

namespace Angazi::Graphics {

class GraphicsSystem
{
public:
	static void StaticInitialize(HWND window, bool fullscreen);
	static void StaticTerminate();
	static GraphicsSystem* Get();

public:
	GraphicsSystem() = default;
	~GraphicsSystem();

	GraphicsSystem(const GraphicsSystem&) = delete;
	GraphicsSystem& operator=(const GraphicsSystem&) = delete;

	void Initialize(HWND window, bool fullscreen);
	void Terminate();

	void BeginRender();
	void EndRender();

	void ToggleFullscreen();
	void Resize(uint32_t width, uint32_t height);

	void ResetRenderTarget();
	void ResetViewport();

	//void SetClearColor(Color clearColor) { mClearColor = clearColor; }
	void SetVSync(bool vSync) { mVSync = vSync ? 1 : 0; }

	uint32_t GetBackBufferWidth() const;
	uint32_t GetBackBufferHeight() const;

	//TODO - Remove these evenetually
	ID3D11Device* GetDevice() const { return mD3DDevice; };
	ID3D11DeviceContext* GetContext() const { return mImmediateContext; };

private:
	friend LRESULT CALLBACK GraphicsSystemMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	friend ID3D11Device* GetDevice();
	friend ID3D11DeviceContext* GetContext();

	ID3D11Device* mD3DDevice = nullptr;
	ID3D11DeviceContext* mImmediateContext = nullptr;

	// Cross platform that rarely changes - DXGI
	IDXGISwapChain* mSwapChain = nullptr;
	ID3D11RenderTargetView* mRenderTargetView = nullptr; // back buffer

	ID3D11Texture2D* mDepthStencilBuffer = nullptr;
	ID3D11DepthStencilView* mDepthStencilView = nullptr;

	DXGI_SWAP_CHAIN_DESC mSwapChainDesc{};
	D3D11_VIEWPORT mViewport{};

	//Color mClearColor = Colors::White;
	UINT mVSync = 1;
};

} // namespace Angazi::Graphics