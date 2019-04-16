//
//  GameRegisterFactory.cpp
//  LibHNLobby
//
//  Created by 黄百超 on 22/02/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#include "GameRegisterFactory.h"
#include "VersionInfo/VersionInfo.h"
//#include "HNLobby/v99/GameRegisterCompositeV99.h"
#include "HNLobby/v66/GameRegisterCompositeV66.h"
//#include "HNLobby/v2/GameRegisterCompositeV2.h"

IGameRegister* GameRegisterFactory::createWithVersion() {
//    if (VERSION_NINETY_NINE) {
//        return GameRegisterCompositeV99::create();
//	}else
    if (VERSION_SIXTY_SIX){
		return GameRegisterCompositeV66::create();
	}else {
        log("未知版本！");
        assert(false);
        //return GameRegisterCompositeV2::create();
    }
    return nullptr;
}
