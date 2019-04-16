//
//  InitialCompositeV88.h
//  LibHNLobby
//
//  Created by 黄百超 on 14/03/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#ifndef InitialCompositeV88_h
#define InitialCompositeV88_h

#include "../pattern/factory/progressBar/ProgressBarLeaf.h"
#include "../pattern/factory/initial/IInitialComposite.h"
#include "../component/logic/InitialLogic.h"

/*
 
 */
class InitialCompositeV88 : public IInitialComposite {
public:
    virtual bool init()override;
    CREATE_FUNC(InitialCompositeV88);
    
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

#endif /* InitialCompositeV88_h */
