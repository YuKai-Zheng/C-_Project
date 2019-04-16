//
//  GameMenuCompositeV88.hpp
//  LibHNLobby
//
//  Created by 黄百超 on 21/02/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#ifndef GameMenuCompositeV88_h
#define GameMenuCompositeV88_h

#include "../pattern/factory/gameMenu/IGameMenu.h"
#include "../PlatformMessage.h"

/*
 
 */
class GameMenuCompositeV88 : public IGameMenu {
    
public:
    virtual bool init()override;
    CREATE_FUNC(GameMenuCompositeV88);
    
public:
    virtual void handleKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent)override;
    
private:
    LOGIN_UI _login_ui;
    
protected:
    // 是否是游客
    bool			isVisitor;
};

#endif /* GameMenuCompositeV88_h */
