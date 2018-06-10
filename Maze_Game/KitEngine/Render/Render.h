#pragma once

#include <d3d11.h>
#include <Windows.h>
#include <DirectXMath.h>
#include <iostream>
#include <string>
#include <codecvt>
#include <effects.h>

#include "../KitEngine.h"
#include "Vertex/VertexShader/VertexShader.h"
#include "Pixel/PixelShader/PixelShader.h"

namespace kit {
	namespace Engine {
		extern DirectX::XMMATRIX			g_mWorld;
		extern DirectX::XMMATRIX			g_mView;
		extern DirectX::XMMATRIX			g_mProjection;
		extern ID3D11VertexShader*			g_pVertexShader;
		extern ID3D11PixelShader*			g_pPixcelShader;
		extern ID3D11InputLayout*			g_pInputLayout;
		
		extern D3D11_INPUT_ELEMENT_DESC		g_vertexDesc[];
		
		typedef struct {
			float pos[3];
			float nor[3];
			float tex[3];
		}VERTEX;

		HRESULT RenderInitalize();

	}
}