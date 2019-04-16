#include "GameShopFactory.h"
#include "VersionInfo/VersionInfo.h"
//#include "HNLobby/v99/GameShopV99.h"
#include "HNLobby/v66/GameShopV66.h"
#include "HNLobby/v66/PlatformResV66.h"


IGameShop* GameShopFactory::createWithVersion() {
    IGameShop* gameShop = nullptr;
//    if (VERSION_NINETY_NINE) {
//        gameShop = GameShopV99::create();
//    } else
    if(VERSION_SIXTY_SIX){
		gameShop =  GameShopV66::create();
	} else{
	}
    if (gameShop) {
        gameShop->setName(GAME_SECOND_LAYER);
    }
    return gameShop;
}
