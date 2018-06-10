#pragma once

#include <d3d11.h>

#include "../../Library/KitLib/include/Kitlib.h"
#include "../AssetsManager/AssetsManager.h"

// extern‚ÌŒx‚Ì”ñ•\¦
#pragma warning (disable:4091)

namespace kit {
	namespace Engine {

		extern class AssetsManager g_assetsManager;

		class KitBehaviour {
		protected:
			vec3 mvec_position;
			axis m_axis;

		public:
			KitBehaviour();
			~KitBehaviour();

			virtual void Update() = 0;
			virtual void Render() = 0;
		};
	}
}