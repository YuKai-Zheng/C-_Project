#ifndef _GAME_MENU_COMPOSITE_V1_H_
#define _GAME_MENU_COMPOSITE_V1_H_

#include "../pattern/factory/gameMenu/IGameMenu.h"
#include "../PlatformMessage.h"

class GameMenuCompositeV1 : public IGameMenu {

public:
    virtual bool init()override;
    CREATE_FUNC(GameMenuCompositeV1);

public:
    virtual void handleKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent)override;

private:
    LOGIN_UI _login_ui;

protected:
    // 是否是游客
    bool			isVisitor;
};

#endif //_GAME_MENU_COMPOSITE_V1_H_
