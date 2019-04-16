#include "HNLobby/v66/GameMusiceEffectV66.h"
#include "HNLobby/PlatformDefine.h"
#include "Sound/HNAudioEngine.h"
#include "FontSize.h"
#include "PlatformResV66.h"
#include "VersionInfo/VersionInfo.h"
#include "UI/Base/AUIButton.h"
#include "../GamePrompt.h"

using namespace HN;

const char* SWITCH_BG_MUSIC     = "csb/newplatform/image/tanchuang/bg.png";
const char* SWITCH_BAR_MUSIC    = "csb/newplatform/image/tanchuang/btn.png";

const char* SWITCH_OFF          = "v66/common_v2/tc_btn_off.png";      //关闭字体
const char* SWITCH_ON           = "v66/common_v2/tc_btn_on.png";      //开启字体

const char* SET_TITLE           = "v66/module/set/set.png";

MusicEffectV66::MusicEffectV66() {
    m_bgPath = "";
	m_bMusicOnOff = false;
	m_bEffectOnOff = false;
}

MusicEffectV66::~MusicEffectV66() {
}

ControlSwitch* MusicEffectV66::createSwitch(Vec2 vPos, int nType) {
    Sprite* bg = Sprite::create(SWITCH_BG_MUSIC);
    Sprite* on = Sprite::create(SWITCH_ON);
    Sprite* off = Sprite::create(SWITCH_OFF);
    Sprite* thumb = Sprite::create(SWITCH_BAR_MUSIC);
    ControlSwitch* pControl = ControlSwitch::create(bg, on, off, thumb, Label::createWithTTF("", DEFAULT_FONT, 16), Label::createWithTTF("", DEFAULT_FONT, 16));
    pControl->setPosition(vPos);
    return pControl;
}

bool MusicEffectV66::init() {
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
    

	Node* _rootNode = CSLoader::createNode(SETTING_INFO_V66);
	addChild(_rootNode);
	_rootNode->setPosition(WIN_SIZE / 2);
	(_rootNode->getChildByName("Panel_9"))->setScaleX(WIN_SIZE.width / 1280);

	//auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 180));
	//addChild(colorLayer);

 //   auto bg = ImageView::create(MODULE_BG2_V66);
 //   bg->setPosition(WIN_SIZE/2);
 //   bg->setTouchEnabled(true);
 //   addChild(bg);
    
	//auto closeBtn = Button::create(MODULE_CLOSE_BTN_V66, MODULE_CLOSE_BTN_V66);
	//closeBtn->setPosition(Vec2(bg->getContentSize().width, bg->getContentSize().height +5));
	//bg->addChild(closeBtn);
	auto closeBtn = (Button*)_rootNode->getChildByName("bg")->getChildByName("btn_close");
	closeBtn->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type){
		if (type != Widget::TouchEventType::ENDED) return;

		close();
	});

    //auto title = Sprite::create(SET_TITLE);
    //title->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height - 34));
    //bg->addChild(title);
    
    //初始化音乐字体
    //auto musicTitle = Sprite::create(MUSIC_TITLE);
    //musicTitle->setPosition(Vec2(bg->getContentSize().width/2-170, bg->getContentSize().height/2 + 35));
    //bg->addChild(musicTitle);
    //
    //初始化音效字体
    //auto effectTitle = Sprite::create(EFFECT_TITLE);
    //effectTitle->setPosition(Vec2(bg->getContentSize().width/2-170, bg->getContentSize().height/2 - 75));
    //bg->addChild(effectTitle);
    
 //   ControlSwitch* switchMusic = createSwitch(Vec2(84,20), 0);
 //   switchMusic->addTargetWithActionForControlEvents(this, cccontrol_selector(MusicEffectV66::callbackSwitchMusic), Control::EventType::VALUE_CHANGED);
	//_rootNode->addChild(switchMusic);
 //   switchMusic->setOn(UserDefault::getInstance()->getBoolForKey(MUSIC_SWITCH));
    
 //   ControlSwitch* switchEffect = createSwitch(Vec2(84,-60), 1);
 //   switchEffect->addTargetWithActionForControlEvents(this, cccontrol_selector(MusicEffectV66::callbackSwitchMusicEffectV66), Control::EventType::VALUE_CHANGED);
	//_rootNode->addChild(switchEffect);
 //   switchEffect->setOn(UserDefault::getInstance()->getBoolForKey(EFFECT_SWITCH));
 //   

	_musicBtn = (Button*)_rootNode->getChildByName("bg")->getChildByName("btn_music");
	_effectBtn = (Button*)_rootNode->getChildByName("bg")->getChildByName("btn_effect");
	_musicBtn->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) return;
		onSwitchMusic();
	});
	_effectBtn->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) return;
		onSwitchEffect();
	});

	m_bMusicOnOff = UserDefault::getInstance()->getBoolForKey(MUSIC_SWITCH);
	m_bEffectOnOff = UserDefault::getInstance()->getBoolForKey(EFFECT_SWITCH);
	if (m_bMusicOnOff)
	{
		((Sprite*)_musicBtn->getChildByName("bg"))->setTexture(SWITCH_ON);
	}
	else
	{
		((Sprite*)_musicBtn->getChildByName("bg"))->setTexture(SWITCH_OFF);
	}

	if (m_bEffectOnOff)
	{
		((Sprite*)_effectBtn->getChildByName("bg"))->setTexture(SWITCH_ON);
	}
	else
	{
		((Sprite*)_effectBtn->getChildByName("bg"))->setTexture(SWITCH_OFF);
	}

    
    //Vec2 bgBottomPos = bgBottom->getPosition();
    ////关于声明
    //auto aboutBtn = AUIButton::create(BTN_YELLOW1_NORMAL_BG_V66, BTN_YELLOW1_SELECT_BG_V66, BTN_YELLOW1_ENABLED_BG_V66);
    //aboutBtn->setTitle("v66/button/yellow1/guanyu.png");
    //aboutBtn->setPosition(Vec2(bgBottomPos.x - 160, bgBottomPos.y - 2));
    //aboutBtn->setVisible(false);
    //bg->addChild(aboutBtn);
    //aboutBtn->setTag(0);
    //aboutBtn->addTouchEventListener(CC_CALLBACK_2(MusicEffectV66::openWindow, this));
    //
    //auto statementBtn = AUIButton::create(BTN_YELLOW1_NORMAL_BG_V66, BTN_YELLOW1_SELECT_BG_V66, BTN_YELLOW1_ENABLED_BG_V66);
    //statementBtn->setTitle("v66/button/yellow1/shenming.png");
    //statementBtn->setPosition(Vec2(bgBottomPos.x + 160, bgBottomPos.y -2));
    //statementBtn->setVisible(false);
    //bg->addChild(statementBtn);
    //statementBtn->setTag(1);
    //statementBtn->addTouchEventListener(CC_CALLBACK_2(MusicEffectV66::openWindow, this));
    
    return true;
}

void MusicEffectV66::onSwitchEffect()
{
	m_bEffectOnOff = !m_bEffectOnOff;	
	HNAudioEngine::getInstance()->setSwitchOfEffect(m_bEffectOnOff);
	if (m_bEffectOnOff)
	{
		((Sprite*)_effectBtn->getChildByName("bg"))->setTexture(SWITCH_ON);
	}
	else
	{
		HNAudioEngine::getInstance()->pauseBackgroundMusic();
		((Sprite*)_effectBtn->getChildByName("bg"))->setTexture(SWITCH_OFF);
	}
}

void MusicEffectV66::onSwitchMusic()
{
	m_bMusicOnOff = !m_bMusicOnOff;
	HNAudioEngine::getInstance()->setSwitchOfMusic(m_bMusicOnOff);
	if (m_bMusicOnOff)
	{
		((Sprite*)_musicBtn->getChildByName("bg"))->setTexture(SWITCH_ON);
		if (HNAudioEngine::getInstance()->hasBackgroundMusic()) {
			HNAudioEngine::getInstance()->resumeBackgroundMusic();
		}
		else {
			HNAudioEngine::getInstance()->playBackgroundMusic(m_bgPath.c_str(), true);
		}
	}
	else
	{
		((Sprite*)_musicBtn->getChildByName("bg"))->setTexture(SWITCH_OFF);
		HNAudioEngine::getInstance()->pauseBackgroundMusic();
	}
}

void MusicEffectV66::callbackSwitchMusic(Ref* pSender, Control::EventType event) {
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

void MusicEffectV66::callbackSwitchMusicEffectV66(Ref* pSender, Control::EventType event) {
    //获取事件的传递者CCControlSwitch
    ControlSwitch* controlSwitch = (ControlSwitch*)pSender;

    bool isMusicOpen = controlSwitch->isOn();
//    UserDefault::getInstance()->setBoolForKey(EFFECT_SWITCH, isMusicOpen);
//    UserDefault::getInstance()->flush();
    HNAudioEngine::getInstance()->setSwitchOfEffect(isMusicOpen);
}

void MusicEffectV66::openWindow(Ref* pSender, Widget::TouchEventType touchtype)
{
    if (touchtype != Widget::TouchEventType::ENDED) return;
    
//    auto tag = ((AUIButton*)pSender)->getTag();
}

void MusicEffectV66::open(Node* parent, Vec2 vec, int zorder, int tag) {
    CCAssert(nullptr != parent, "parent is nullptr");
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setPosition(vec);
    parent->addChild(this, zorder, tag);
    
    //setScale(0.2);
    //runAction(ScaleTo::create(0.2f, 1.0f));
}

void MusicEffectV66::close() {
    //runAction(Sequence::create(ScaleTo::create(0.2f, 0.2f), RemoveSelf::create(), nullptr));
	removeFromParent();
}

void MusicEffectV66::setBgPath(std::string path) {
    if (path.compare("") != 0) {
        m_bgPath = path;
    }
}
