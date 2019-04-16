#ifndef __GameMenu_V88_H__
#define __GameMenu_V88_H__

#include "../model/one/GameMenuOne.h"


class GameMenuV88 : public GameMenuOne {
public:
	virtual bool init() override;
	CREATE_FUNC(GameMenuV88);

public:
	virtual void loginResult(bool isSuccess, const std::string &message);

private:
};

#endif // __GameMenu_V88_H__

