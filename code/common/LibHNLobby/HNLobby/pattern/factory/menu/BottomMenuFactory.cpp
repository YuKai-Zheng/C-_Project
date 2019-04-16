#include "BottomMenuFactory.h"
#include "VersionInfo/VersionInfo.h"
//#include "HNLobby/v1/BottomMenuComposite.h"
//#include "HNLobby/v3/BottomMenuCompositeV3.h"
//#include "HNLobby/v99/BottomMenuCompositeV99.h"
#include "HNLobby/v88/BottomMenuCompositeV88.h"
#include "HNLobby/v66/BottomMenuCompositeV66.h"

IMenu* BottomMenuFactory::createWithVersion() {
    //if (VERSION_ONE || VERSION_TWO || VERSION_HUNDRED) {
    //    return BottomMenuComposite::create();
    //}
//    else if (VERSION_THREE) {
//        return BottomMenuCompositeV3::create();
//    }
//    else if (VERSION_NINETY_NINE) {
//        return BottomMenuCompositeV99::create();
//	}
	//else
    if (VERSION_SIXTY_SIX) {
		return BottomMenuCompositeV66::create();
	}else if (VERSION_HUNDRED) {
    }
    return nullptr;
}
