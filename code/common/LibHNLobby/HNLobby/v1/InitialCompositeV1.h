#ifndef _INITIAL_COMPOSITE_V1_H_
#define _INITIAL_COMPOSITE_V1_H_
#include "../pattern/factory/progressBar/ProgressBarLeaf.h"
#include "../pattern/factory/initial/IInitialComposite.h"
#include "../component/logic/InitialLogic.h"

/*

*/
class InitialCompositeV1 : public IInitialComposite {
public:
    virtual bool init()override;
    CREATE_FUNC(InitialCompositeV1);

public:
    virtual float getPercentage()override;
    virtual void updateProgressBar(std::string text, float percent)override;

public:
    void waitting(float dt);
    void setLogic(InitialLogic* logic);

    void setBackGroundImage(const std::string &name);
private:
    InitialLogic* m_logic;
    ProgressbarDelegate* progressBarDelegate;
};

#endif //_INITIAL_COMPOSITE_V1_H_
