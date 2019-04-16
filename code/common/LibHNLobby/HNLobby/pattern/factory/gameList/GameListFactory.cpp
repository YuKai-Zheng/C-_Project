#include "GameListFactory.h"
#include "VersionInfo/VersionInfo.h"
//#include "HNLobby/v1/GameListComposite.h"
//#include "HNLobby/v2/GameListCompositeV2.h"
//#include "HNLobby/v3/GameListCompositeV3.h"
#include "HNLobby/v66/GameListCompositeV66.h"

IGameList* GameListFactory::createWithVersion() {
    //if (VERSION_HUNDRED ) {
    //    return GameListComposite::create();
    //} else if (VERSION_TWO) {
    //    return GameListCompositeV2::create();
    //} else if (VERSION_ONE ||VERSION_THREE) {
    //    return GameListCompositeV3::create();
	//}else
    if (VER_SIXTY_SIX){
		return GameListCompositeV66::create();
	}
    return nullptr;
}
