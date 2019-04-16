#ifndef __GameMenu_V2_H__
#define __GameMenu_V2_H__

#include "../model/one/GameMenuOne.h"


class GameMenuV2 : public GameMenuOne {
public:
    virtual bool init() override;
    CREATE_FUNC(GameMenuV2);

public:
    virtual void loginResult(bool isSuccess, const std::string &message);

private:
};

#endif // __GameMenu_V2_H__
