//
//  GameRegisterFactory.h
//  LibHNLobby
//
//  Created by 黄百超 on 22/02/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#ifndef GameRegisterFactory_h
#define GameRegisterFactory_h

#include "IGameRegister.h"

class GameRegisterFactory : public IGameRegister {
public:
    static IGameRegister* createWithVersion();
    
};

#endif /* GameRegisterFactory_h */
