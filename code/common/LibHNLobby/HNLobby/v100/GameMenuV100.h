#ifndef __GameMenu_V100_H__
#define __GameMenu_V100_H__

#include "../model/one/GameMenuOne.h"


class GameMenuV100 : public GameMenuOne {
public:
    virtual bool init() override;
    CREATE_FUNC(GameMenuV100);

public:
    //virtual void initMenu()override;
    //virtual void handleKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent)override;

private:
    //LOGIN_UI _login_ui;
};

#endif // __GameMenu_V100_H__