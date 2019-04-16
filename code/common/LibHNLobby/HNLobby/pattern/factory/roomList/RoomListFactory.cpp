#include "RoomListFactory.h"
#include "VersionInfo/VersionInfo.h"
//#include "HNLobby/v1/RoomListComposite.h"
//#include "HNLobby/v2/RoomListCompositeV2.h"
//#include "HNLobby/v3/RoomListCompositeV3.h"
//#include "HNLobby/v88/RoomListCompositeV88.h"
#include "HNLobby/v66/RoomListCompositeV66.h"

IRoomList* RoomListFactory::createWithVersion() {
    //if (VERSION_HUNDRED) {
    //    return RoomListComposite::create();
    //} else if (VERSION_ONE ||VERSION_TWO || VERSION_THREE) {
    //    return RoomListCompositeV2::create();
	//}else
    if (VERSION_SIXTY_SIX){
		return RoomListCompositeV66::create();
	}else{
        log("不存在的版本！");
        assert(false);
    }
    return nullptr;
}
