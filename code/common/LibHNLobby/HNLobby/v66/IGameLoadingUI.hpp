//
//  IGameLoadingUI.hpp
//  LibHNLobby
//
//  Created by mac on 2018/8/7.
//  Copyright © 2018年 redbird. All rights reserved.
//

#ifndef IGameLoadingUI_hpp
#define IGameLoadingUI_hpp

#include "HNBaseType.h"
#include "UI/Base/HNLayer.h"
#include "cocos2d.h"
#include "HNLobby/pattern/factory/progressBar/ProgressBarLeaf.h"


class IGameLoadingUI : public HNLayer
{
    
public:
    typedef std::function<void ()> CloseCallBack;
    CloseCallBack  onCloseCallBack;
    
    ProgressbarDelegate* _progressBarDelegate;
    Node*                      loadNode;
    
public:
    static IGameLoadingUI* create(bool loading,int gameIdx);
    void loadingTextureCallback(Texture2D* textureData, std::string plist);
    void loadingCallback(float dt);
    void loadRes();
protected:
    IGameLoadingUI();
    virtual ~IGameLoadingUI();
    
private:
    bool init(bool loading,int gameIdx);
	void UpdateTimer(float dt);
    bool _loading;
    INT _percent;
    INT _bgIdx;
    BYTE  fileNum;
    INT nMaxFileNum;
    int m_gameIdx;
	float m_fSpineDuration;//骨骼动画播放时间;
	float m_fSpineTime;//骨骼动画播放时间;
    
};

#endif /* IGameLoadingUI_hpp */
