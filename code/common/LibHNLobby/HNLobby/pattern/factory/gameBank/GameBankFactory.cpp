#include "GameBankFactory.h"
#include "VersionInfo/VersionInfo.h"
//#include "HNLobby/v99/GameBankV99.h"
//#include "HNLobby/v88/GameBankV88.h"
#include "HNLobby/v66/GameBankV66.h"
//#include "HNLobby/v99/PlatformResV99.h"
//#include "HNLobby/v66/PlatformResV66.h"
#include "HNLobby/v66/PlatformResV66.h"


IGameBank* GameBankFactory::createWithVersion() {
    IGameBank* gameBank = nullptr;
//    if (VERSION_NINETY_NINE) {
//        gameBank = GameBankV99::create();
//    } else
    if(VERSION_SIXTY_SIX) {
		gameBank = GameBankV66::create();
	}else{
	}
    if (gameBank) {
        gameBank->setName(GAME_SECOND_LAYER);
    }
    return gameBank;
}
