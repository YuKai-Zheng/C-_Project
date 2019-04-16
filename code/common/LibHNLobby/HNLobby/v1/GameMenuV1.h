#ifndef __GameMenuV1_H__
#define __GameMenuV1_H__

#include "../model/one/GameMenuOne.h"

class GameMenuV1 : public GameMenuOne {
public:
    virtual bool init() override;
    CREATE_FUNC(GameMenuV1);

public:
    virtual void loginResult(bool isSuccess, const std::string &message);

private:
};

#endif // __GameMenuV1_H__
