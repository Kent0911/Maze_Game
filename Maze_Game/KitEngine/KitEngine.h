#pragma once

#include <d3d11.h>
#include <memory>

#include "../Library/KitLib/include/Kitlib.h"

#pragma comment(lib "d3d11.lib")

// extern警告の非表示
#pragma warning (disable : 4091)

#define DEFALUT_NUMERATOR (UINT)60
#define DEFALUT_DENOMINATOR (UINT)1

namespace kit {
	namespace Engine {
		//------------------------------------------------------------------------------------------------------------
		//
		// DirectX11で使用するグローバル変数
		//
		//------------------------------------------------------------------------------------------------------------
		extern HINSTANCE							g_hInst;
		extern HWND									g_hWnd;
		extern D3D_DRIVER_TYPE						g_driverType;
		extern D3D_FEATURE_LEVEL					g_featureLevel;
		extern ID3D11Device*						g_pd3dDevice;
		extern ID3D11RasterizerState*				g_pd3dRasterizterState;
		extern ID3D11DeviceContext*					g_pImmediateContext;
		extern IDXGISwapChain*						g_pSwapChain;
		extern ID3D11RenderTargetView*				g_pRenderTargetView;
		extern ID3D11Texture2D*						g_pDepthStencil;
		extern ID3D11DepthStencilView*             	g_pDepthStencilView;

		class FrameRate {
		public:
			FrameRate();
			~FrameRate();
			FrameRate operator=( const FrameRate _framerate );
			inline void Numerator(const UINT _num) {
				mui_numerator = _num;
			}
			inline UINT Numerator() const {
				return mui_numerator;
			}
			inline UINT Denominator() const {
				return mui_denominator;
			}

		private:
			UINT mui_numerator, mui_denominator;
		};

		class Scene;

		// ゲームの初期設定
		typedef struct {
			LPSTR					mlp_str;
			FrameRate				mc_flameRate;
			kit::vec2				mv_windowSize;
			std::shared_ptr<Scene>	msptr_startScene;
		}CONFIG;


		class KitEngine {
		public:
			KitEngine( HINSTANCE _hInstance, int _nCmdShow, CONFIG* _config );
			~KitEngine();

			void ChangeScene( std::shared_ptr<Scene> _changeScene );

			void Update();
			void Render();
		private:
			CONFIG							mc_config;
			std::unique_ptr<CWindow>		muptr_window;
			std::unique_ptr<SceneManager>	mutpr_sceneManager;

			HRESULT InitDevice();
			HRESULT HCreateWindow( HINSTANCE _hInstance, int _nCmdShow );
		};
	}
}


