#include "GameUserHeadFactory.h"
#include "VersionInfo/VersionInfo.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "HNLobby/v66/GameUserHeadV66.h"

IGameUserHead* GameUserHeadFactory::createWithVersion() {
    IGameUserHead* gameUserHead = nullptr;
    if (VERSION_NINETY_NINE) {
        
    } else if(VERSION_SIXTY_SIX) {
		gameUserHead = GameUserHeadV66::create();
	}
	else{}

	if (gameUserHead) {
		gameUserHead->setName(GAME_SECOND_LAYER);
    }
	return gameUserHead;
}
