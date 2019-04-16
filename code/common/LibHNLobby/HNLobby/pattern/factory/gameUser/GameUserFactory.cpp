#include "GameUserFactory.h"
#include "VersionInfo/VersionInfo.h"
//#include "HNLobby/v99/GameUserV99.h"
#include "HNLobby/v66/GameUserV66.h"
#include "HNLobby/v66/PlatformResV66.h"


IGameUser* GameUserFactory::createWithVersion() {
    IGameUser* gameUser = nullptr;
//    if (VERSION_NINETY_NINE) {
//        gameUser = GameUserV99::create();
//    } else
    if(VER_SIXTY_SIX) {
		gameUser = GameUserV66::create();
	}
	else{}

    if (gameUser) {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
        gameUser->setName(GAME_SECOND_LAYER);
    }
    return gameUser;
}
