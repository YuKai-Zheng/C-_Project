#ifndef __GAMEINITIAL_V100_H__
#define __GAMEINITIAL_V100_H__

#include "../component/delegate/InitialCompositeDelegate.h"
#include "../model/one/GameInitialOne.h"

class GameInitialV100 : public GameInitialOne {
public:
    virtual bool init() override;
    CREATE_FUNC(GameInitialV100);

public:
    virtual void updateProgressBar(std::string text, float percent) override;
    virtual float getPercentage() override;

private:
    InitialCompositeDelegate* m_initialCompositeDelegate;
};

#endif // __GAMEINITIAL_V100_H__
