#include "GameSetFactory.h"
#include "HNLobby/v88/GameSetLayerV88.h"
#include "VersionInfo/VersionInfo.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "HNLobby/v66/GameMusiceEffectV66.h"

IGameSet* GameSetFactory::createWithVersion() {
    IGameSet* gameSet = nullptr;
//    if (VERSION_NINETY_NINE) {
//        gameSet = GameSetV99::create();
//    } else
    if(VERSION_SIXTY_SIX) {
		gameSet = MusicEffectV66::create();
	}else{

	}
    if (gameSet) {
        gameSet->setName(GAME_SECOND_LAYER);
    }
    return gameSet;
}
