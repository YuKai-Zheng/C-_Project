#ifndef _PLATFORM_STATE_H_
#define _PLATFORM_STATE_H_

#include "cocos2d.h"
#include "HNLobby/pattern/BaseState.h"
#include "../globel.h"
#include "HNLobby/pattern/factory/progressBar/ProgressBarLeaf.h"
USING_NS_CC;

/*
大厅状态类
*/
class PlatformState : public BaseState {
public:
    void enter() override;
    void exit() override;
    void execute() override;

    virtual bool init()override;

    CREATE_FUNC(PlatformState);

    void setType(LayerType type);
    void loadingTextureCallback(Texture2D* textureData, std::string plist);
private:
    LayerType m_type;
    
    int nMaxFileNum;
    ProgressbarDelegate* _progressBarDelegate;
    int _percent;
    int curFileNum;
};

#endif //_PLATFORM_STATE_H_
