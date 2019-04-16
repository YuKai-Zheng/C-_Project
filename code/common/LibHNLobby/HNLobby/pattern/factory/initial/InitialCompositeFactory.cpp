#include "InitialCompositeFactory.h"
#include "VersionInfo/VersionInfo.h"
//#include "HNLobby/v1/InitialCompositeV1.h"
//#include "HNLobby/v100/InitialCompositeV100.h"
//#include "HNLobby/v99/InitialCompositeV99.h"
#include "HNLobby/v88/InitialCompositeV88.h"
#include "HNLobby/v66/InitialCompositeV66.h"

IInitialComposite* InitialCompositeFactory::createWithVersion() {
    if (VERSION_SIXTY_SIX) {
		return InitialCompositeV66::create();
	}
//    else if (VERSION_HUNDRED) {
//        return InitialCompositeV100::create();
//    }
    return nullptr;
}
