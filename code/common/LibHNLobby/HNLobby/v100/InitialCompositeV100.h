#ifndef _INITIAL_COMPOSITE_V100_H_
#define _INITIAL_COMPOSITE_V100_H_
#include "../component/logic/InitialLogic.h"
#include "../pattern/factory/initial/IInitialComposite.h"
//#include "../pattern/factory/progressBar/ProgressBarLeaf.h"
#include "../pattern/factory/progressBar/IProgressbar.h"

/*

*/
class InitialCompositeV100 : public IInitialComposite {

public:
    virtual bool init()override;
    CREATE_FUNC(InitialCompositeV100);

public:
    virtual float getPercentage()override;
    virtual void updateProgressBar(std::string text, float percent)override;

public:
    void waitting(float dt);
    void setLogic(InitialLogic* logic);

    void setBackGroundImage(const std::string &name);
private:
    IProgressbar* progressBarLeaf;
    InitialLogic* m_logic;
};

#endif //_INITIAL_COMPOSITE_V100_H_
