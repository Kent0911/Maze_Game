#pragma once

#include <memory>

#include "SceneManager/SceneManager.h"
#include "../../KitBehaviour/KitBehaviour.h"

namespace kit {
	namespace Engine {

		class Scene {
		protected:
			unsigned char		mc_sceneNumber;
			SceneManager*		mc_sceneManager;

		public:
			Scene() = default;

			void SetManagerRef(SceneManager* _ref);
			inline unsigned char GetSceneNumber() const {
				return mc_sceneNumber;
			}

			virtual void Update();
			virtual void Render();
		};
	}
}