//
//  IGameLoadingUI.cpp
//  LibHNLobby
//
//  Created by mac on 2018/8/7.
//  Copyright © 2018年 redbird. All rights reserved.
//

#include "IGameLoadingUI.hpp"
#include "HNLobby/v66/PlatformResV66.h"
#include "HNLobby/pattern/factory/progressBar/ProgressBarFactory.h"
#include "HNLobby/PlatformDefine.h"
#include "../LibHNUI/Tool/Tools.h"
#include "spine/spine.h"
#include <spine/spine-cocos2dx.h>
IGameLoadingUI::IGameLoadingUI()
:fileNum(0)
,_percent(50)
,_loading(true)
,nMaxFileNum(0)
,m_gameIdx(0)
, m_fSpineTime(0.0f)
, m_fSpineDuration(0.0f)
{
    
}

IGameLoadingUI::~IGameLoadingUI() {
}

IGameLoadingUI* IGameLoadingUI::create(bool loading,int gameIdx) {
    IGameLoadingUI* pRet = new IGameLoadingUI();
    if (nullptr != pRet && pRet->init(loading,gameIdx)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool IGameLoadingUI::init(bool loading,int gameIdx) {
    if (!HNLayer::init()) {
        return false;
    }
    srand(time(NULL));
    
    std::string path = "";
    m_gameIdx = gameIdx;
    switch (gameIdx) {
        case LOAD_JSXS_GAME:
             nMaxFileNum = 1;
             break;
        case LOAD_CSLL_GAME:
             nMaxFileNum = 1;
             break;
        case LOAD_NINELINE_GAME:
             nMaxFileNum = 1;
             break;
        case LOAD_EIGHT_GAME:
            nMaxFileNum = 1;
        case LOAD_SHZ_GAME:
            nMaxFileNum = 1;
        default:
            
             break;
    }
    auto winSize = Director::getInstance()->getWinSize();

	if (LOAD_CSLL_GAME == m_gameIdx)
	{

	}
	else
	{
		auto loadingBG = ImageView::create(LOADING_BG_V66);
		loadingBG->setPosition(winSize / 2);
		this->addChild(loadingBG);
		this->setScaleX(WIN_SIZE.width / 1600.0f);
		this->setScaleY(WIN_SIZE.height / 720.f);

		auto progressBarLeaf = ProgressBarFactory::create(LOADING_BAR_BG_V66, LOADING_BAR_PRE_V66);
		addChild(progressBarLeaf);
		progressBarLeaf->setPosition(Vec2(WIN_SIZE.width / 2, progressBarLeaf->getContentSize().height));

		_progressBarDelegate = progressBarLeaf;
	}
    
    _loading = loading;
    
   
    
    return true;
}

void IGameLoadingUI::UpdateTimer(float dt)
{
	m_fSpineTime += dt;

	if (m_fSpineTime >= m_fSpineDuration)
	{
		unschedule(schedule_selector(IGameLoadingUI::UpdateTimer));
		loadingCallback(0.0f);
	}
}

void IGameLoadingUI::loadRes() 
{
    if(_loading) 
	{
        if (m_gameIdx == LOAD_CSLL_GAME)
		{
           // Director::getInstance()->getTextureCache()->addImageAsync("Games/31000507/cs_icon.png", CC_CALLBACK_1(IGameLoadingUI::loadingTextureCallback, this,"Games/31000507/cs_icon.plist"));
			m_fSpineTime = -1;
			Ref *pRef = Tools::PlaySkeletonAnimation(this, "common/loading/spine/31000507/", "loading_caishenlaile",-1,Vec2(WIN_SIZE.width*0.5f,WIN_SIZE.height*0.5f + 5));
			auto pSkeAnim = static_cast<spine::SkeletonAnimation*>(pRef);
			if (pRef != nullptr && pSkeAnim != nullptr)
			{
				spAnimation * pSpAnim = pSkeAnim->findAnimation("loading_caishenlaile");
				if (pSpAnim != nullptr)
				{
					m_fSpineTime = 0.0f;
					m_fSpineDuration = 2.0f;
					pSkeAnim->setTimeScale(3.0f / 2.0f);
					schedule(schedule_selector(IGameLoadingUI::UpdateTimer), 1.0f);
				}
			}

        }
        else if (m_gameIdx == LOAD_JSXS_GAME)
        {
            Director::getInstance()->getTextureCache()->addImageAsync("Games/31000508/js_icon.png", CC_CALLBACK_1(IGameLoadingUI::loadingTextureCallback, this,"Games/31000508/js_icon.plist"));
        }
        else if (m_gameIdx == LOAD_NINELINE_GAME)
        {
            Director::getInstance()->getTextureCache()->addImageAsync("Games/31000506/nl_icon.png", CC_CALLBACK_1(IGameLoadingUI::loadingTextureCallback, this,"Games/31000506/nl_icon.plist"));
        }
        else if (m_gameIdx == LOAD_EIGHT_GAME)
        {
            Director::getInstance()->getTextureCache()->addImageAsync("Games/31000512/eight_yp.png", CC_CALLBACK_1(IGameLoadingUI::loadingTextureCallback, this,"Games/31000512/eight_yp.plist"));
        }
        else if (m_gameIdx == LOAD_SHZ_GAME)
        {
           // Director::getInstance()->getTextureCache()->addImageAsync("Games/SHZ/SHZ_ICON.png", CC_CALLBACK_1(IGameLoadingUI::loadingTextureCallback, this,"Games/SHZ/SHZ_ICON.plist"));
            
        }
        
    }
}

void IGameLoadingUI::loadingTextureCallback(Texture2D* textureData, std::string plist) {
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist , textureData);
    
    if (_progressBarDelegate) 
	{
        char str[100] = { 0 };

        _percent += 100/nMaxFileNum;

        if (_percent > 100.0f)
		{
            _percent = 100.0f;
        }
        //sprintf(str, "更新配置中:%d%s", _percent,"%");

		_progressBarDelegate->updateProgressBar(str, _percent);
    }
    
    loadingCallback(0.0f);
}

void IGameLoadingUI::loadingCallback(float dt) {
    fileNum++;
    if (fileNum >= nMaxFileNum) {
        if (nullptr != onCloseCallBack) {
            onCloseCallBack();
        }
    }
}
