#include "KitBehaviour.h"

using namespace kit;
using namespace Engine;

KitBehaviour::KitBehaviour(){
	g_assetsManager.GetInstance().AddAsset(this);
}

KitBehaviour::~KitBehaviour() {	}