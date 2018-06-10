#pragma once

#include <list>
#include <assert.h>

#include "../../Library/KitLib/include/Kitlib.h"
#include "../KitBehaviour/KitBehaviour.h"

// extern‚ÌŒx‚Ì”ñ•\¦
#pragma warning (disable:4091)

namespace kit {
	namespace Engine {

		extern class KitBehaviour;

		class AssetsManager :public kit::Singleton<AssetsManager> {
		private:
			friend class kit::Singleton<AssetsManager>;
			AssetsManager();
			
			std::list<kit::Engine::KitBehaviour*> mlis_assets;

		public:
			inline void AddAsset(kit::Engine::KitBehaviour* _asset) {
				mlis_assets.emplace_back(_asset);
			}

			bool ClearAssets();

			void Update();
			void Render();
		};
	}
}