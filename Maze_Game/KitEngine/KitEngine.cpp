#include "KitEngine.h"

using namespace kit;
using namespace kit::Engine;

HINSTANCE                           kit::Engine::g_hInst = nullptr;
HWND                                kit::Engine::g_hWnd = nullptr;
D3D_DRIVER_TYPE                     kit::Engine::g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL                   kit::Engine::g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*                       kit::Engine::g_pd3dDevice = nullptr;
ID3D11RasterizerState*				kit::Engine::g_pd3dRasterizterState = nullptr;
ID3D11DeviceContext*                kit::Engine::g_pImmediateContext = nullptr;
IDXGISwapChain*                     kit::Engine::g_pSwapChain = nullptr;
ID3D11RenderTargetView*             kit::Engine::g_pRenderTargetView = nullptr;
ID3D11Texture2D*                    kit::Engine::g_pDepthStencil = nullptr;
ID3D11DepthStencilView*             kit::Engine::g_pDepthStencilView = nullptr;

FrameRate::FrameRate() {
	mui_numerator = DEFALUT_NUMERATOR;
	mui_denominator = DEFALUT_DENOMINATOR;
}

FrameRate::~FrameRate() {	}

FrameRate FrameRate::operator= (const FrameRate _frameRate) {
	this->mui_numerator = _frameRate.mui_numerator;
	this->mui_denominator = _frameRate.mui_denominator;
	return *this;
}

HRESULT KitEngine::InitDevice() {
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(g_hWnd, &rc);
	UINT width = rc.right - rc.left + 16;
	UINT height = rc.bottom - rc.top + 31;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = mc_config.mc_frameRate.Numerator();
	sd.BufferDesc.RefreshRate.Denominator = mc_config.mc_frameRate.Denominator();
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex) {
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(nullptr, g_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr)) { return hr; }


	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return hr;

	hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	// Create buffer
	D3D11_BUFFER_DESC bufferdesc;
	bufferdesc.ByteWidth = sizeof(kit::Engine::Vertex) * 3;
	bufferdesc.Usage = D3D11_USAGE_DEFAULT;
	bufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferdesc.CPUAccessFlags = 0;
	bufferdesc.MiscFlags = 0;
	bufferdesc.StructureByteStride = 0;

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = g_pd3dDevice->CreateTexture2D(&descDepth, nullptr, &g_pDepthStencil);
	if (FAILED(hr))
		return hr;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	descDSV.Texture2D.MipSlice = 0;
	descDSV.Flags = 0;
	hr = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);
	if (FAILED(hr))
		return hr;

	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);

	// Create Shader
	hr = g_pd3dDevice->CreateVertexShader(&g_vs_main, sizeof(g_vs_main), NULL, &g_pVertexShader);
	if (FAILED(hr)) { return hr; }
	hr = g_pd3dDevice->CreatePixelShader(&g_ps_main, sizeof(g_ps_main), NULL, &g_pPixcelShader);
	if (FAILED(hr)) { return hr; }


	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pImmediateContext->RSSetViewports(1, &vp);

	// Create DirectXTK objects
	RenderInitalize();

	return S_OK;
}

HRESULT KitEngine::HCreateWindow(HINSTANCE _hInstance, int _nCmdShow) {
	g_hInst = _hInstance;
	g_hWnd = CreateWindow(muptr_window->GetWcex().lpszClassName, mc_config.mlp_str, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, muptr_window->GetRect().right - muptr_window->GetRect().left, muptr_window->GetRect().bottom - muptr_window->GetRect().top,
		nullptr, nullptr, _hInstance, nullptr);
	if (!g_hWnd) { return E_FAIL; }

	ShowWindow(g_hWnd, _nCmdShow);

	return S_OK;
}

KitEngine::KitEngine(HINSTANCE _hInstance, int _nCmdShow, CONFIG* _config) {
	mc_config = *_config;
	muptr_window.reset(new CWindow(_hInstance, _nCmdShow, mc_config.mv_windowSize));
	AdjustWindowRect(&muptr_window->GetRect(), WS_OVERLAPPEDWINDOW, FALSE);
	HCreateWindow(_hInstance, _nCmdShow);
	InitDevice();
	muptr_sceneManager.reset(new SceneManager(mc_config.msptr_startScene));
}

KitEngine::~KitEngine() { }

void KitEngine::ChangeScene(std::shared_ptr<kit::Engine::Scene> _changeScene) {
	muptr_sceneManager->ChangeScene(_changeScene);
}

void KitEngine::Update() {
	muptr_sceneManager->Update();
}

void KitEngine::Render() {
	FLOAT clear_color[4] = { 0.5f,0.5f,0.5f,1.0f };
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, clear_color);
	g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 3);

	g_pSwapChain->Present(0, 0);
}