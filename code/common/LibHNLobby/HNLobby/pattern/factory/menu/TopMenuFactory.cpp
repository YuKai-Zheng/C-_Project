#include "TopMenuFactory.h"
#include "VersionInfo/VersionInfo.h"
//#include "HNLobby/v1/TopMenuComposite.h"
//#include "HNLobby/v99/TopMenuCompositeV99.h"
//#include "HNLobby/v2/TopMenuCompositeV2.h"
//#include "HNLobby/v3/TopMenuCompositeV3.h"
//#include "HNLobby/v88/TopMenuCompositeV88.h"
#include "HNLobby/v66/TopMenuCompositeV66.h"

IMenu* TopMenuFactory::createWithVersion() {
    if (VERSION_SIXTY_SIX) {
		return TopMenuCompositeV66::create();
	}
    return nullptr;
}
