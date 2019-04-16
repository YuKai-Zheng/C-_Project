#ifndef __GameMenu_V3_H__
#define __GameMenu_V3_H__

#include "../model/one/GameMenuOne.h"


class GameMenuV3 : public GameMenuOne {
public:
    virtual bool init() override;
    CREATE_FUNC(GameMenuV3);

public:
    virtual void loginResult(bool isSuccess, const std::string &message);

private:
};

#endif // __GameMenu_V3_H__
