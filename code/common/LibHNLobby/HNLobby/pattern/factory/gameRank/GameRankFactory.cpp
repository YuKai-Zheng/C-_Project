#include "GameRankFactory.h"
#include "VersionInfo/VersionInfo.h"
//#include "HNLobby/v99/GameRankV99.h"
//#include "HNLobby/v88/GameRankV88.h"
//#include "HNLobby/v99/PlatformResV99.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "HNLobby/v66/GameRankV66.h"


IGameRank* GameRankFactory::createWithVersion() {
    IGameRank* gameRank = nullptr;
//    if (VERSION_NINETY_NINE) {
//        gameRank = GameRankV99::create();
//    } else
    if(VERSION_SIXTY_SIX){
		gameRank = GameRankV66::create();
    }
    if (gameRank) {
        gameRank->setName(GAME_SECOND_LAYER);
    }
    return gameRank;
}
