#include "GameMenuFactory.h"
#include "VersionInfo/VersionInfo.h"
//#include "HNLobby/v2/GameMenuCompositeV1.h"
#include "HNLobby/v66/GameMenuCompositev66.h"
//#include "HNLobby/v88/GameMenuCompositev88.h"

IGameMenu* GameMenuFactory::createWithVersion() {
 if (VERSION_SIXTY_SIX) {
		return GameMenuCompositeV66::create();
	}
    return nullptr;
}
