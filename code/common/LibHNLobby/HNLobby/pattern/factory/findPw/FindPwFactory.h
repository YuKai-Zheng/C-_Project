//
//  FindPwFactory.h
//  LibHNLobby
//
//  Created by 黄百超 on 22/02/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#ifndef FindPwFactory_h
#define FindPwFactory_h

#include "IFindPw.h"

class FindPwFactory : public IFindPw {
public:
    static IFindPw* createWithVersion();
    
};

#endif /* FindPwFactory_h */
