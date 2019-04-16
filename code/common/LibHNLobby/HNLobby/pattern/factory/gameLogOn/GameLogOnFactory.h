//
//  GameLogOnFactory.h
//  LibHNLobby
//
//  Created by 黄百超 on 21/06/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#ifndef GameLogOnFactory_h
#define GameLogOnFactory_h

#include "IGameLogOn.h"

class GameLogOnFactory : public IGameLogOn {
    
public:
    static IGameLogOn* createWithVersion();
    
};

#endif /* GameLogOnFactory_h */
