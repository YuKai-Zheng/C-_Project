#include "GameGiftFactory.h"
//#include "HNLobby/v88/GameSetLayerV88.h"
#include "VersionInfo/VersionInfo.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "HNLobby/v66/GameGiftV66.h"

IGameGift* GameGiftFactory::createWithVersion() {
    IGameGift* gameGift = nullptr;

    if(VERSION_SIXTY_SIX) {
		gameGift = GameGiftV66::create();
	}else{

	}
    if (gameGift) {
		gameGift->setName(GAME_SECOND_LAYER);
    }
    return gameGift;
}
