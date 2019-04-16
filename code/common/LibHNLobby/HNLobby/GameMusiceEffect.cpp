#include "GameMusiceEffect.h"
#include "PlatformDefine.h"
#include "Sound/HNAudioEngine.h"
#include "FontSize.h"
//#include "v3/PlatformResV3.h"
#include "v66/PlatformResV66.h"
//#include "v99/PlatformResV99.h"
#include "VersionInfo/VersionInfo.h"

using namespace HN;

//static const char* GAME_BACKGROUND = "platform/setting/settingRes/shop.png";
static const char* GAME_TEXTPROMPT = "platform/setting/settingRes/1.png";
static const char* GAME_SWITCH_OFF = "platform/setting/settingRes/off.png";
static const char* GAME_SWITCH_ON = "platform/setting/settingRes/on.png";
static const char* GAME_SWITCH_SPOT_BIG = "platform/setting/settingRes/spotBig.png";
//static const char* GAME_SWITCH_SPOT_SMALL = "platform/setting/settingRes/spotSmall.png";
static const char* GAME_LOGO = "platform/setting/settingRes/0.png";


#define WINSIZE Director::getInstance()->getWinSize()

MusicEffect::MusicEffect() {
}

MusicEffect::~MusicEffect() {
}

void MusicEffect::loadUI() {
    auto textLogo = Sprite::create(GAME_LOGO);
    auto logoSize = textLogo->getContentSize();
    textLogo->setPosition(Vec2(0, logoSize.height*0.5f));
    this->addChild(textLogo);

    Vec2 vPos = textLogo->getPosition();

    ControlSwitch* switchMusic = createSwitch(Vec2(vPos.x+100, vPos.y),0);
    switchMusic->addTargetWithActionForControlEvents(this, cccontrol_selector(MusicEffect::callbackSwitchMusic), Control::EventType::VALUE_CHANGED);
    this->addChild(switchMusic);
    switchMusic->setOn(UserDefault::getInstance()->getBoolForKey(MUSIC_SWITCH));

    ControlSwitch* switchEffect = createSwitch(Vec2(vPos.x + 100, vPos.y), 1);
    switchEffect->addTargetWithActionForControlEvents(this, cccontrol_selector(MusicEffect::callbackSwitchMusicEffect), Control::EventType::VALUE_CHANGED);
    this->addChild(switchEffect);

    switchEffect->setOn(UserDefault::getInstance()->getBoolForKey(EFFECT_SWITCH));
}

ControlSwitch* MusicEffect::createSwitch(Vec2 vPos, int nType) {
    Sprite* bg = Sprite::create(GAME_TEXTPROMPT);
    Sprite* on = Sprite::create(GAME_SWITCH_ON);
    Sprite* off = Sprite::create(GAME_SWITCH_OFF);
    Sprite* thumb = Sprite::create(GAME_SWITCH_SPOT_BIG);
    int height = nType == 0 ? bg->getContentSize().height : -bg->getContentSize().height;
    ControlSwitch* pControl = ControlSwitch::create(bg, on, off, thumb, Label::createWithTTF("", DEFAULT_FONT, 16), Label::createWithTTF("", DEFAULT_FONT, 16));
    pControl->setPosition(Vec2(vPos.x*1.75, vPos.y + height));
    return pControl;
}

bool MusicEffect::init() {
    if (!HNLayer::init()) {
        return false;
    }

//    auto bg = Sprite::create(GAME_BACKGROUND);
    loadUI();
    return true;
}

void MusicEffect::callbackSwitchMusic(Ref* pSender, Control::EventType event) {
    //获取事件的传递者CCControlSwitch
    ControlSwitch* controlSwitch = (ControlSwitch*)pSender;

    bool isOnOff = controlSwitch->isOn();
    UserDefault::getInstance()->setBoolForKey(MUSIC_SWITCH, isOnOff);
    UserDefault::getInstance()->flush();
    HNAudioEngine::getInstance()->setSwitchOfMusic(isOnOff);
    //根据开关控件的状态，设置label标签的内容
    if (isOnOff) {
        if (HNAudioEngine::getInstance()->hasBackgroundMusic()) {
            HNAudioEngine::getInstance()->resumeBackgroundMusic();
        } else {
//            if (VERSION_THREE) {
//                HNAudioEngine::getInstance()->playBackgroundMusic(NEW_YEAR_BG, true);
//            } else
            if (VERSION_SIXTY_SIX) {
                HNAudioEngine::getInstance()->playBackgroundMusic(GAME_BACKGROUND_MUSIC_V66, true);
            }
//            else if (VERSION_NINETY_NINE) {
//                HNAudioEngine::getInstance()->playBackgroundMusic(GAME_BACKGROUND_MUSIC_V99, true);
//            }
            else {
                HNAudioEngine::getInstance()->playBackgroundMusic(GAME_BACKGROUND_MUSIC, true);
            }
        }
    } else {
        HNAudioEngine::getInstance()->pauseBackgroundMusic();
    }


}

void MusicEffect::callbackSwitchMusicEffect(Ref* pSender, Control::EventType event) {
    //获取事件的传递者CCControlSwitch
    ControlSwitch* controlSwitch = (ControlSwitch*)pSender;

    bool isMusicOpen = controlSwitch->isOn();
    UserDefault::getInstance()->setBoolForKey(EFFECT_SWITCH, isMusicOpen);
    UserDefault::getInstance()->flush();
    HNAudioEngine::getInstance()->setSwitchOfEffect(isMusicOpen);
}

void MusicEffect::show(Node* parent, int zorder, int tag) {
    CCAssert(nullptr != parent, "parent is nullptr");
    parent->addChild(this, zorder, tag);
}

void MusicEffect::close() {
    this->removeFromParent();
}
