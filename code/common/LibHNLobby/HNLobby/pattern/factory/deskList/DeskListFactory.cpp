#include "DeskListFactory.h"
#include "VersionInfo/VersionInfo.h"
#include "HNLobby/v66/DeskListComposite.h"

IDeskList* DeskListFactory::createWithVersion(ComRoomInfo *roomInfo) {
    if (VERSION_ONE || VERSION_TWO || VERSION_THREE  || VERSION_HUNDRED || VERSION_NINETY_NINE) {
        return DeskListComposite::create(roomInfo);
    } else if (VERSION_HUNDRED) {
    }
    return nullptr;
}
