#include "HNLobby/v88/GameMusiceEffectV88.h"
#include "HNLobby/PlatformDefine.h"
#include "Sound/HNAudioEngine.h"
#include "FontSize.h"
#include "PlatformResV88.h"
#include "VersionInfo/VersionInfo.h"
#include "UI/Base/AUIButton.h"

using namespace HN;

const char* SWITCH_BG_MUSIC_V88     = "v88/module/set/shengyin/5.png";
const char* SWITCH_BAR_MUSIC_V88    = "v88/module/set/shengyin/6.png";
const char* MUSIC_TITLE_V88         = "v88/module/set/shengyin/1.png";      //音乐提示字体
const char* EFFECT_TITLE_V88        = "v88/module/set/shengyin/2.png";      //音效提示字体
const char* SWITCH_OFF_V88          = "v88/module/set/shengyin/4.png";      //关闭字体
const char* SWITCH_ON_V88           = "v88/module/set/shengyin/3.png";      //开启字体

const char* SET_TITLE_V88           = "v88/module/set/set.png";

MusicEffectV88::MusicEffectV88() {
    m_bgPath = "";
}

MusicEffectV88::~MusicEffectV88() {
}

ControlSwitch* MusicEffectV88::createSwitch(Vec2 vPos, int nType) {
    Sprite* bg = Sprite::create(SWITCH_BG_MUSIC_V88);
    Sprite* on = Sprite::create(SWITCH_ON_V88);
    Sprite* off = Sprite::create(SWITCH_OFF_V88);
    Sprite* thumb = Sprite::create(SWITCH_BAR_MUSIC_V88);
    ControlSwitch* pControl = ControlSwitch::create(bg, on, off, thumb, Label::createWithTTF("", DEFAULT_FONT, 16), Label::createWithTTF("", DEFAULT_FONT, 16));
    pControl->setPosition(vPos);
    return pControl;
}

bool MusicEffectV88::init() {
    if (!HNLayer::init()) {
        return false;
    }

    auto myListener = EventListenerTouchOneByOne::create();
    myListener->setSwallowTouches(true);//阻止触摸向下传递
    myListener->onTouchBegan = [](Touch *touch, Event *unused_event){
        return true;
    };
    myListener->onTouchEnded = [&](Touch *touch, Event *unused_event){
        close();
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(myListener, this);
    
	auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 100));
	addChild(colorLayer);

    auto bg = ImageView::create(MODULE_BG2_V88);
    bg->setPosition(WIN_SIZE/2);
    bg->setTouchEnabled(true);
    addChild(bg);
    
    auto bgBottom = Sprite::create("v88/module/set/dikuang_2.png");
    bgBottom->setName("BG_BOTTOM");
    bgBottom->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2-204));
    bg->addChild(bgBottom);
    
    auto title = Sprite::create(SET_TITLE_V88);
    title->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height - 44));
    bg->addChild(title);
    
    //初始化音乐字体
    auto musicTitle = Sprite::create(MUSIC_TITLE_V88);
    musicTitle->setPosition(Vec2(bg->getContentSize().width/2-170, bg->getContentSize().height/2 + 55));
    bg->addChild(musicTitle);
    
    //初始化音效字体
    auto effectTitle = Sprite::create(EFFECT_TITLE_V88);
    effectTitle->setPosition(Vec2(bg->getContentSize().width/2-170, bg->getContentSize().height/2 - 55));
    bg->addChild(effectTitle);
    
    ControlSwitch* switchMusic = createSwitch(Vec2(bg->getContentSize().width/2 + 150, bg->getContentSize().height/2 + 55), 0);
    switchMusic->addTargetWithActionForControlEvents(this, cccontrol_selector(MusicEffectV88::callbackSwitchMusic), Control::EventType::VALUE_CHANGED);
    bg->addChild(switchMusic);
    switchMusic->setOn(UserDefault::getInstance()->getBoolForKey(MUSIC_SWITCH));
    
    ControlSwitch* switchEffect = createSwitch(Vec2(bg->getContentSize().width/2 + 150, bg->getContentSize().height/2 - 55), 1);
    switchEffect->addTargetWithActionForControlEvents(this, cccontrol_selector(MusicEffectV88::callbackSwitchMusicEffectV88), Control::EventType::VALUE_CHANGED);
    bg->addChild(switchEffect);
    switchEffect->setOn(UserDefault::getInstance()->getBoolForKey(EFFECT_SWITCH));
    
    
    Vec2 bgBottomPos = bgBottom->getPosition();
    //关于声明
    auto aboutBtn = AUIButton::create(BTN_YELLOW1_NORMAL_BG_V88, BTN_YELLOW1_SELECT_BG_V88, BTN_YELLOW1_ENABLED_BG_V88);
    aboutBtn->setTitle("v88/button/yellow1/guanyu.png");
    aboutBtn->setPosition(Vec2(bgBottomPos.x - 160, bgBottomPos.y - 2));
    aboutBtn->setVisible(false);
    bg->addChild(aboutBtn);
    aboutBtn->setTag(0);
    aboutBtn->addTouchEventListener(CC_CALLBACK_2(MusicEffectV88::openWindow, this));
    
    auto statementBtn = AUIButton::create(BTN_YELLOW1_NORMAL_BG_V88, BTN_YELLOW1_SELECT_BG_V88, BTN_YELLOW1_ENABLED_BG_V88);
    statementBtn->setTitle("v88/button/yellow1/shenming.png");
    statementBtn->setPosition(Vec2(bgBottomPos.x + 160, bgBottomPos.y -2));
    statementBtn->setVisible(false);
    bg->addChild(statementBtn);
    statementBtn->setTag(1);
    statementBtn->addTouchEventListener(CC_CALLBACK_2(MusicEffectV88::openWindow, this));
    
    return true;
}

void MusicEffectV88::callbackSwitchMusic(Ref* pSender, Control::EventType event) {
    //获取事件的传递者CCControlSwitch
    ControlSwitch* controlSwitch = (ControlSwitch*)pSender;

    bool isOnOff = controlSwitch->isOn();
//    UserDefault::getInstance()->setBoolForKey(MUSIC_SWITCH, isOnOff);
//    UserDefault::getInstance()->flush();
    HNAudioEngine::getInstance()->setSwitchOfMusic(isOnOff);
    //根据开关控件的状态，设置label标签的内容
    if (isOnOff) {
        if (HNAudioEngine::getInstance()->hasBackgroundMusic()) {
            HNAudioEngine::getInstance()->resumeBackgroundMusic();
        }
        else {
            HNAudioEngine::getInstance()->playBackgroundMusic(m_bgPath.c_str(), true);
        }
    } else {
        HNAudioEngine::getInstance()->pauseBackgroundMusic();
    }
}

void MusicEffectV88::callbackSwitchMusicEffectV88(Ref* pSender, Control::EventType event) {
    //获取事件的传递者CCControlSwitch
    ControlSwitch* controlSwitch = (ControlSwitch*)pSender;

    bool isMusicOpen = controlSwitch->isOn();
//    UserDefault::getInstance()->setBoolForKey(EFFECT_SWITCH, isMusicOpen);
//    UserDefault::getInstance()->flush();
    HNAudioEngine::getInstance()->setSwitchOfEffect(isMusicOpen);
}

void MusicEffectV88::openWindow(Ref* pSender, Widget::TouchEventType touchtype)
{
    if (touchtype != Widget::TouchEventType::ENDED) return;
    
//    auto tag = ((AUIButton*)pSender)->getTag();
}

void MusicEffectV88::open(Node* parent, Vec2 vec, int zorder, int tag) {
    CCAssert(nullptr != parent, "parent is nullptr");
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setPosition(vec);
    parent->addChild(this, zorder, tag);
    
    //setScale(0.2);
    //runAction(ScaleTo::create(0.2f, 1.0f));
}

void MusicEffectV88::close() {
    //runAction(Sequence::create(ScaleTo::create(0.2f, 0.2f), RemoveSelf::create(), nullptr));
	removeFromParent();
}

void MusicEffectV88::setBgPath(std::string path) {
    if (path.compare("") != 0) {
        m_bgPath = path;
    }
}
