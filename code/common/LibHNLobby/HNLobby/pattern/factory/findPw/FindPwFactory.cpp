//
//  FindPwFactory.cpp
//  LibHNLobby
//
//  Created by 黄百超 on 22/02/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#include "FindPwFactory.h"
#include "VersionInfo/VersionInfo.h"
//#include "HNLobby/v99/FindPwCompositeV99.h"
//#include "HNLobby/v88/FindPwCompositeV88.h"
#include "HNLobby/v66/FindPwCompositeV66.h"
//#include "HNLobby/v2/FindPwCompositeV2.h"

IFindPw* FindPwFactory::createWithVersion() {
    if (VERSION_SIXTY_SIX){
        return FindPwCompositeV66::create();
       // return FindPwCompositeV2::create();
    }
    return nullptr;
}
