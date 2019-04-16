//
//  GameMenuCompositeV66.hpp
//  LibHNLobby
//
//  Created by 黄百超 on 21/02/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#ifndef GameMenuCompositeV66_h
#define GameMenuCompositeV66_h

#include "../pattern/factory/gameMenu/IGameMenu.h"
#include "../PlatformMessage.h"

/*
 
 */
class GameMenuCompositeV66 : public IGameMenu {
    
public:
    virtual bool init()override;
    CREATE_FUNC(GameMenuCompositeV66);
    
public:
    virtual void handleKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent)override;
    bool judgeIsUpdateNewVerison();
private:
    LOGIN_UI _login_ui;
    
protected:
    // 是否是游客
    bool			isVisitor;
};

#endif /* GameMenuCompositeV66_h */
