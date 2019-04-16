//
//  GameLandFactory.h
//  LibHNLobby
//
//  Created by 黄百超 on 21/02/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#ifndef GameLandFactory_h
#define GameLandFactory_h

#include "IGameLand.h"

class GameLandFactory : public IGameLand {
public:
    static IGameLand* createWithVersion();
    
};

#endif /* GameLandFactory_h */
