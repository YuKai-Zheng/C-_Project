#include "PlatformState.h"

//#include "../v99/GamePlatformV99.h"
//#include "../v88/GamePlatformV88.h"
#include "../v66/GamePlatformV66.h"
#include "VersionInfo/VersionInfo.h"
#include "../globel.h"
#include "../model/one/GamePlatformOne.h"
//#include "../v66/PlatformResV66.h"
#include "../v66/PlatformResV66.h"
#include "../pattern/factory/progressBar/ProgressBarFactory.h"

void PlatformState::enter() {
    UserDefault::getInstance()->setBoolForKey("NEW_YEAR", false);
}

void PlatformState::exit() {
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
}

void PlatformState::execute() {
    UserDefault::getInstance()->setBoolForKey(TOUCH_HEAD, true);
    
    //nMaxFileNum = 7;
    //curFileNum  = 0;
    //_percent    = 0;
    
    if (VERSION_SIXTY_SIX) {
        auto mainlayer = GamePlatformV66::create();
        mainlayer->setLayerType(m_type);
        addChild(mainlayer);
    }
    
    /*Director::getInstance()->getTextureCache()->addImageAsync("v66/animation/icon.png", CC_CALLBACK_1(PlatformState::loadingTextureCallback, this,"v66/animation/icon.plist"));
    Director::getInstance()->getTextureCache()->addImageAsync("v66/animation/main/topEffect.png", CC_CALLBACK_1(PlatformState::loadingTextureCallback, this,"v66/animation/main/topEffect.plist"));
    Director::getInstance()->getTextureCache()->addImageAsync("v66/animation/main/bottomEffect.png", CC_CALLBACK_1(PlatformState::loadingTextureCallback, this,"v66/animation/main/bottomEffect.plist"));
    
    Director::getInstance()->getTextureCache()->addImageAsync("v66/animation/main/jieji.png", CC_CALLBACK_1(PlatformState::loadingTextureCallback, this,"v66/animation/main/jieji.plist"));
    Director::getInstance()->getTextureCache()->addImageAsync("v66/animation/main/fishFire.png", CC_CALLBACK_1(PlatformState::loadingTextureCallback, this,"v66/animation/main/fishFire.plist"));
    Director::getInstance()->getTextureCache()->addImageAsync("v66/animation/main/fishLight.png", CC_CALLBACK_1(PlatformState::loadingTextureCallback, this,"v66/animation/main/fishLight.plist"));
    Director::getInstance()->getTextureCache()->addImageAsync("v66/animation/main/qipai.png", CC_CALLBACK_1(PlatformState::loadingTextureCallback, this,"v66/animation/main/qipai.plist"));*/
    
}
void PlatformState::loadingTextureCallback(Texture2D* textureData, std::string plist) {
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist , textureData);
    
    curFileNum++;
    if (_progressBarDelegate) {
        char str[100] = { 0 };
        _percent += 100/nMaxFileNum;
        if (_percent > 100.0f) {
            _percent = 100.0f;
        }
        sprintf(str,"加载大厅:%d%s",_percent,"%");
        
        _progressBarDelegate->updateProgressBar(str, _percent);
    }
    
    if (curFileNum >= nMaxFileNum) {
        if (VERSION_SIXTY_SIX) {
            auto mainlayer = GamePlatformV66::create();
            mainlayer->setLayerType(m_type);
            addChild(mainlayer);
        }
    }
}
bool PlatformState::init() {
    if (!HNScene::init()) {
        return false;
    }
    
    //auto loadingBG = Sprite::create(LOGON_BG_V66);
    //if (loadingBG) {
    //    loadingBG->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2));
    //    addChild(loadingBG);
    //}
    
    auto progressBarLeaf = ProgressBarFactory::create(LOADING_BAR_BG_V66, LOADING_BAR_PRE_V66);
    if (progressBarLeaf) {
        addChild(progressBarLeaf);
        progressBarLeaf->setPosition(Vec2(WIN_SIZE.width / 2, progressBarLeaf->getContentSize().height));
        _progressBarDelegate = progressBarLeaf;
    }
    return true;
}

void PlatformState::setType(LayerType type) {
    m_type = type;
}

