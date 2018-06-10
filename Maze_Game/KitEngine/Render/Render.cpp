#include "Render.h"

using namespace kit::Engine;

DirectX::XMMATRIX			kit::Engine::g_mWorld;
DirectX::XMMATRIX			kit::Engine::g_mView;
DirectX::XMMATRIX			kit::Engine::g_mProjection;
ID3D11VertexShader*			kit::Engine::g_pVertexShader;
ID3D11PixelShader*			kit::Engine::g_pPixcelShader;
ID3D11InputLayout*			kit::Engine::g_pInputLayout;

D3D11_INPUT_ELEMENT_DESC	kit::Engine::g_vertexDesc[]{
	{ "POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	0,							 D3D11_INPUT_PER_VERTEX_DATA,0 },
	{ "NORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	{ "TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
};

HRESULT kit::Engine::RenderInitalize() {
	HRESULT hr = S_OK;

	if (FAILED(hr)) { return hr; }
}