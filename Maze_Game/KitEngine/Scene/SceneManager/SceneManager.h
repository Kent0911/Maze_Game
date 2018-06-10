#pragma once
#include <memory>
#include <iostream>

#include "../Scene.h"
#include "../../AssetsManager/AssetsManager.h"

// extern‚ÌŒx‚Ì”ñ•\¦
#pragma warning (disable:4091)

namespace kit {
	namespace Engine {

		extern AssetsManager		g_assetsManager;
		extern class SceneFonts		g_sceneFonts;

		class Scene;
		
		class SceneManager{
		private:
			std::shared_ptr<Scene> msptr_currentScene;
			std::shared_ptr<Scene> msptr_previousScene;

		public:
			SceneManager(std::shared_ptr<Scene> _startScene);
			~SceneManager();
			void ChangeScene(const std::shared_ptr<Scene> _changeScene);
			bool UndoChange();

			void Update();
			void Render();
		};
	}
}