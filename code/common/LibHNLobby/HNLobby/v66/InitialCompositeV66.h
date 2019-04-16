//
//  InitialCompositeV66.h
//  LibHNLobby
//
//  Created by 黄百超 on 14/03/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#ifndef InitialCompositeV66_h
#define InitialCompositeV66_h

#include "../pattern/factory/progressBar/ProgressBarLeaf.h"
#include "../pattern/factory/initial/IInitialComposite.h"
#include "../component/logic/InitialLogic.h"

/*
 
 */
class InitialCompositeV66 : public IInitialComposite {
public:
    virtual bool init()override;
    CREATE_FUNC(InitialCompositeV66);
    
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

#endif /* InitialCompositeV66_h */
