#include "GameTypeFactory.h"
#include "VersionInfo/VersionInfo.h"
//#include "HNLobby/v3/GameTypeCompositeV3.h"
//#include "HNLobby/v2/GameTypeCompositeV2.h"
#include "HNLobby/v66/GameTypeCompositeV66.h"
//#include "HNLobby/v99/GameTypeCompositeV99.h"


IGameType* GameTypeFactory::createWithVersion() {
//    if (VERSION_HUNDRED) {
//    } else if (VERSION_TWO) {
//        return GameTypeCompositeV2::create();
//    }
//    else if (VERSION_NINETY_NINE) {
//        return GameTypeCompositeV99::create();
//    }
//    else if (VERSION_ONE ||VERSION_THREE) {
//        return GameTypeCompositeV3::create();
//    } else {
//    }
    return GameTypeCompositeV66::create();
    return nullptr;
}
