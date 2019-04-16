#ifndef __GameMenu_V66_H__
#define __GameMenu_V66_H__

#include "../model/one/GameMenuOne.h"


class GameMenuV66 : public GameMenuOne {
public:
	virtual bool init() override;
	CREATE_FUNC(GameMenuV66);

public:
	virtual void loginResult(bool isSuccess, const std::string &message, int errCode)override;

private:
};

#endif // __GameMenu_V66_H__

