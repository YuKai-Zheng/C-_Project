//
//  GameLandFactory.cpp
//  LibHNLobby
//
//  Created by 黄百超 on 21/02/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#include "GameLandFactory.h"
#include "VersionInfo/VersionInfo.h"
//#include "HNLobby/v99/GameLandCompositeV99.h"
//#include "HNLobby/v88/GameLandCompositeV88.h"
#include "HNLobby/v66/GameLandCompositeV66.h"
//#include "HNLobby/v2/GameLandCompositeV2.h"

IGameLand* GameLandFactory::createWithVersion() {
//	if (VERSION_NINETY_NINE) {
//		return GameLandCopositeV99::create();
//	}else
    if (VERSION_SIXTY_SIX)
	{
		return GameLandCopositeV66::create();
	}
	else {
        assert(false);
        log("未知版本类型!");
        //return GameLandCopositeV2::create();
    }
    return nullptr;
}
