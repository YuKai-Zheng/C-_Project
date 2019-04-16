//
//  GameLogOnFactory.cpp
//  LibHNLobby
//
//  Created by 黄百超 on 21/06/2017.
//  Copyright © 2017 redbird. All rights reserved.
//
#include "GameLogOnFactory.h"
#include "VersionInfo/VersionInfo.h"
#include "HNLobby/v66/GameLogOnCompositeV66.h"
//#include "HNLobby/v88/GameLogOnCompositeV88.h"

IGameLogOn* GameLogOnFactory::createWithVersion() {
    if (VERSION_SIXTY_SIX){
        return GameLogOnCompositeV66::create();
    }
    
    return nullptr;
}
