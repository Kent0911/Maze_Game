#include "SceneManager.h"

using namespace kit;
using namespace kit::Engine;

SceneManager::SceneManager(std::shared_ptr<Scene> _startScene) {
	msptr_currentScene = _startScene;
	msptr_currentScene->SetManagerRef(this);
	msptr_previousScene = nullptr;
}

SceneManager::~SceneManager() {
	g_assetsManager.GetInstance().ClearAssets();
	g_sceneFonts.GetInstance().ClearFontList();
	msptr_currentScene.reset();
	msptr_previousScene.reset();
}

void SceneManager::ChangeScene(const std::shared_ptr<Scene> _changeScene) {
	msptr_previousScene = msptr_currentScene;
	msptr_currentScene = _changeScene;
	msptr_currentScene->SetManagerRef(this);

}

bool SceneManager::UndoChange() {
	if (nullptr != msptr_previousScene) {
		msptr_currentScene = msptr_previousScene;
		msptr_previousScene = msptr_currentScene;
		return true;
	}
	else { return false; }
}

void SceneManager::Update() {
	msptr_currentScene->Update();
}

void SceneManager::Render() {
	msptr_currentScene->Render();
}