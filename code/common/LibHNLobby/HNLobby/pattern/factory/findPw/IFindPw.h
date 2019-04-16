//
//  IFindPw.h
//  LibHNLobby
//
//  Created by 黄百超 on 22/02/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#ifndef IFindPw_h
#define IFindPw_h

#include "HNUIExport.h"
#include "cocos2d.h"

USING_NS_CC;

class IFindPw : public HNLayer {
public:
    typedef std::function<void()> CloseCallBack;
    
public:
    CloseCallBack onCloseCallBack;
    
    virtual void initFindPwUI() {}
    
};

#endif /* IFindPw_h */
