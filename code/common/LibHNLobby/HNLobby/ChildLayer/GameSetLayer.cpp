#include "HNLobby/ChildLayer/GameSetLayer.h"
#include "HNLobby/PersionalCenter/ModifyPassword.h"
#include "HNLobby/GameAutograph.h"
#include "HNLobby/PersionalCenter/BindPhone.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/PlatformConfig.h"
#include "../GameMusiceEffect.h"
#include "GameHelpAndShenMing.h"
//static const int COLOR_LAYER_TAG	= 10000;

static const char* GAME_BACKGROUND			= "platform/hallBg.png";
static const char* GAME_TOPLOGO					= "platform/newbankUi/set.png";
static const char* GAME_KUANG						= "platform/newbankUi/1.png";
static const char* GAME_CLOSE_BUTTON		= "platform/lobbyUi/PlatformUi_TOP/return.png";
static const char* BANKER_BUTTON_CSB = "csb/platform/setting/button_node.csb";


#define WINSIZE Director::getInstance()->getWinSize()


GameSetLayer::GameSetLayer() : _layout(nullptr) {
    _oldViewTag = 0;
    _childViewNode = nullptr;
    _isClose = true;
}

GameSetLayer::~GameSetLayer() {
    _vecNormal.clear();
    _vecPrass.clear();
    _vecButton.clear();
    memset(&_structBtn, 0x0, sizeof(_structBtn));
}

void GameSetLayer::showSet(Node* parent, int zorder, int tag) {
    CCAssert(nullptr != parent, "parent is nullptr");
    if (0 != tag) {
        parent->addChild(this, zorder, tag);
    } else {
        parent->addChild(this, zorder);
    }
}

void GameSetLayer::closeSet() {
    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
    colorLayer->removeFromParent();
    this->runAction(Sequence::create(ScaleTo::create(0.1f , 0.3f), CCCallFunc::create([&]() {
        this->removeFromParent();
    }), nullptr));

}

void GameSetLayer::openSetLayer(Node* parent, Vec2 vec, int zorder, int tag) {
    CCAssert(nullptr != parent, "parent is nullptr");
    Size winSize = Director::getInstance()->getWinSize();
    this->setPosition(vec);
    parent->addChild(this, zorder, tag);
    //parent->runAction(EaseSineOut::create(MoveBy::create(0.2f, Vec2(-winSize.width, 0))));
    parent->runAction(MoveTo::create(0.3f, Vec2(-winSize.width, 0)));//-winSize.width
}

void GameSetLayer::closeSetLayer() {
    if (_isClose) {
        _isClose = false;

        HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
        auto winSize = Director::getInstance()->getWinSize();
        this->getParent()->runAction(Sequence::create(MoveBy::create(0.3f, Vec2(winSize.width, 0)),
        CCCallFunc::create([&]() {
            this->removeFromParent();
        }), nullptr));
    }
}

bool GameSetLayer::init() {
    if ( !HNLayer::init())
        return false;

    //this->setPosition(Vec2(0, 0));
    setBackGroundImage(GAME_BACKGROUND);
    loadTopLogo(GAME_TOPLOGO);
    loadKuang(GAME_KUANG);
    loadButton();


    // 这个节点做子页面的父节点管理
    _childViewNode = Node::create();
    _childViewNode->setPosition(Vec2(0,0));
    _kuang->addChild(_childViewNode);


    //退出按钮
    _closeBtn = Button::create(GAME_CLOSE_BUTTON, GAME_CLOSE_BUTTON);
    auto closeSIze = _closeBtn->getContentSize();
    _closeBtn->setShowEffect(false);
    _closeBtn->setTag(close_Btn);
    _closeBtn->setPosition(Vec2(WINSIZE.width - closeSIze.width*0.55, WINSIZE.height - closeSIze.height * 0.55));
    _closeBtn->addTouchEventListener(CC_CALLBACK_2(GameSetLayer::btnCallBack, this));
    addChild(_closeBtn);

    ////屏蔽后面的层
    //colorLayer = LayerColor::create(Color4B(0, 0, 0, 100));
    //addChild(colorLayer);

    //auto touchListener = EventListenerTouchOneByOne::create();
    //touchListener->onTouchBegan = [=](Touch* touch ,Event* event) {
    //    return true;
    //};
    //touchListener->setSwallowTouches(true);
    //_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,colorLayer);

    //创建一个点击事件
    //auto myListener = EventListenerTouchOneByOne::create();
    //myListener->setSwallowTouches(true);//阻止触摸向下传递
    //myListener->onTouchBegan = CC_CALLBACK_2(GameSetLayer::onTouchBegan, this);
    //myListener->onTouchMoved = CC_CALLBACK_2(GameSetLayer::onTouchMoved, this);
    //myListener->onTouchEnded = CC_CALLBACK_2(GameSetLayer::onTouchEnded, this);
    //myListener->onTouchCancelled = CC_CALLBACK_2(GameSetLayer::onTouchCancelled, this);

    //_eventDispatcher->addEventListenerWithSceneGraphPriority(myListener, this);
    //settingLoader = CSLoader::createNode("platform/setting/settingNode.csb");
    //settingLoader->setPosition(winSize/2);
    //settingLoader->setScale(0);
    //addChild(settingLoader);
    //settingLoader->runAction(ScaleTo::create( 0.2f, 1));

    //auto setLayout = (Layout*)settingLoader->getChildByName("Panel_1");
    //auto settingClose = (Button*)setLayout->getChildByName("Button_Close");
    //settingClose->addTouchEventListener(CC_CALLBACK_2(GameSetLayer::closeSetCallBack, this));

    //auto user = UserDefault::getInstance();
    ////音效滑动条
    //effectSlider = (Slider*)setLayout->getChildByName("Slider_Effect");
    //effectSlider->setPercent(user->getIntegerForKey(SOUND_VALUE_TEXT));
    //effectSlider->addEventListener(Slider::ccSliderCallback(CC_CALLBACK_2(GameSetLayer::sliderCallback, this, E_SLIDER)));

    ////音乐滑动条
    //musicSlider = (Slider*)setLayout->getChildByName("Slider_Music");
    //musicSlider->setPercent(user->getIntegerForKey(MUSIC_VALUE_TEXT));
    //musicSlider->addEventListener(Slider::ccSliderCallback(CC_CALLBACK_2(GameSetLayer::sliderCallback, this, M_SLIDER)));

    ////音效按钮
    //auto effectSmallBtn = (Button*)setLayout->getChildByName("Button_EffectSmall");
    //effectSmallBtn->addTouchEventListener(CC_CALLBACK_2(GameSetLayer::setCallBack, this, 1));
    //auto effectBigBtn = (Button*)setLayout->getChildByName("Button_EffectBig");
    //effectBigBtn->addTouchEventListener(CC_CALLBACK_2(GameSetLayer::setCallBack, this, 2));
    ////音乐按钮
    //auto musicSmallBtn = (Button*)setLayout->getChildByName("Button_MusicSmall");
    //musicSmallBtn->addTouchEventListener(CC_CALLBACK_2(GameSetLayer::setCallBack, this, 3));
    //auto musicBigBtn = (Button*)setLayout->getChildByName("Button_MusicBig");
    //musicBigBtn->addTouchEventListener(CC_CALLBACK_2(GameSetLayer::setCallBack, this, 4));
    //////////////////////////////////////////////////////////////////////////
    //auto bg = Sprite::create(BRNN_SET_BG);
    //bg->setPosition(Vec2(bg->getContentSize().width/2,bg->getContentSize().height/2));
    //addChild(bg);

    //this->setIgnoreAnchorPointForPosition(false);
    //this->setContentSize(bg->getContentSize());

    //auto music = Sprite::create(BRNN_SET_MUSIC);
    //music->setPosition(Vec2(30+music->getContentSize().width/2,getContentSize().height-65));
    //addChild(music);

    //auto sound = Sprite::create(BRNN_SET_SOUND);
    //sound->setPosition(music->getPositionX(), music->getPositionY() - music->getContentSize().height - 46);
    //addChild(sound);

    //auto user = UserDefault::getInstance();

    //auto soundSlider = Slider::create(BRNN_SET_SCROLL_BAR_FOREBG,BRNN_SET_SCROLL_BAR_BALL,TextureResType::LOCAL);
    //soundSlider->setPosition(Vec2(sound->getPositionX() + sound->getContentSize().width+68, sound->getPositionY()));
    //soundSlider->loadProgressBarTexture(BRNN_SET_SCROLL_BAR_BG);
    //soundSlider->setPercent(50);
    //soundSlider->setRotation(180);
    //soundSlider->addEventListener(Slider::ccSliderCallback(CC_CALLBACK_2(GameSetLayer::sliderCallback, this, M_SLIDER)));
    //soundSlider->setPercent(user->getIntegerForKey(SOUND_VALUE_TEXT));
    //addChild(soundSlider);

    //auto musicSlider = Slider::create(BRNN_SET_SCROLL_BAR_FOREBG, BRNN_SET_SCROLL_BAR_BALL, TextureResType::LOCAL);
    //musicSlider->setPosition(Vec2(music->getPositionX() + music->getContentSize().width + 68, music->getPositionY()));
    //musicSlider->loadProgressBarTexture(BRNN_SET_SCROLL_BAR_BG);
    //musicSlider->setRotation(180);
    //musicSlider->addEventListener(Slider::ccSliderCallback(CC_CALLBACK_2(GameSetLayer::sliderCallback, this, M_SLIDER)));
    //musicSlider->setPercent(user->getIntegerForKey(MUSIC_VALUE_TEXT));
    //addChild(musicSlider);
    createGeXingView(_childViewNode);
    return true;
}

// 初始化按钮控件
void GameSetLayer::loadButton() {
    _btnRoot = CSLoader::createNode(BANKER_BUTTON_CSB);
    _btnRoot->setPosition(Vec2(_kuang->getContentSize().width*0.05, _kuang->getContentSize().height*0.85-5));

    _kuang->addChild(_btnRoot);

    auto parent = _btnRoot->getChildren();
    int i = 1000;
    for (auto child : parent) {
        auto btn = static_cast<Button *>(child->getChildByName("btn"));
        btn->setShowEffect(false);
        btn->setTag(i++);
        btn->addTouchEventListener(CC_CALLBACK_2(GameSetLayer::btnCallBack, this));
        _vecButton.push_back(btn);

        auto normal = static_cast<Sprite *>(child->getChildByName("normal"));
        _vecNormal.push_back(normal);

        auto prass = static_cast<Sprite *>(child->getChildByName("prass"));
        _vecPrass.push_back(prass);
    }
}

void GameSetLayer::btnCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (Widget::TouchEventType::ENDED != type) return;
    auto btn = static_cast<Button*>(pSender);

    //处理点击相同子界面一处重新加载
    if (_oldViewTag == btn->getTag()) {
        return;
    }
    //按钮已经添加了音效，无需重复添加   by HBC
//	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);

    _childViewNode->removeAllChildren();
    touchEffect(btn->getTag());

    switch (btn->getTag()) {
    case close_Btn: {
        closeSetLayer();
    }
    break;
    case createQianming_Btn:
        createGeXingView(_childViewNode);
        break;
    case creatGaiMi_Btn:
        createXiuGaiView(_childViewNode);
        break;
    case creatBound_Btn: {
        createBoundView(_childViewNode);
        break;
    }
    case creatMusicEffect_Btn:
        createMusicView(_childViewNode);
        break;
    case creatHelp_Btn: {
        createHelpView(_childViewNode);

        break;
    }
    case creatShenMing_Btn: {
        createShenMingView(_childViewNode);

        break;
    }
    default:
        break;
    }
}

//点击效果
void GameSetLayer::touchEffect(UINT tag) {
    int idx = 0;
    for (auto child : _vecButton) {
        if (child->getTag() == tag) {
            _vecPrass.at(idx)->setVisible(true);
            _vecNormal.at(idx)->setVisible(false);
        } else {
            _vecPrass.at(idx)->setVisible(false);
            _vecNormal.at(idx)->setVisible(true);
        }
        idx++;
    }
}
void GameSetLayer::loadBackGround(const char* fileName) {

    auto bg = Sprite::create(fileName);
    bg->setPosition(WINSIZE.width * 0.5f,WINSIZE.height * 0.5f);
    addChild(bg,-1);
}

void GameSetLayer::loadTopLogo(const char* fileName) {
    auto logo = Sprite::create(fileName);
    logo->setPosition(WINSIZE.width * 0.5f,WINSIZE.height - logo->getContentSize().height *0.5f + 10 );
    addChild(logo, -1);
}

void GameSetLayer::loadKuang(const char* fileName) {
    auto logo = Sprite::create(fileName);
    _kuang = logo;
    logo->setPosition(WINSIZE.width * 0.5f,WINSIZE.height * 0.48f);
    addChild(logo, -1);
}


bool GameSetLayer::onTouchBegan(Touch *touch, Event *unused_event) {
    Point point = touch->getLocation();
    if (!this->getBoundingBox().containsPoint(point)) {
        this->removeFromParent();
        //	this->moveSelfWithAction();
        //	_bSelfMoved = true;
    }

    //_bMoved = false;
    //_beginPos = point;
    return true;
}

void GameSetLayer::onTouchMoved(Touch *touch, Event *unused_event) {
    //Point point = touch->getLocation();
    //if (abs(int(point.x - _beginPos.x) > MOVE_THRESHOLD_DEFINE) || abs(int(point.y - _beginPos.y) > MOVE_THRESHOLD_DEFINE))
    //	_bMoved = true;
}

void GameSetLayer::onTouchEnded(Touch *touch, Event *unused_event) {

}

//void GameSetLayer::setCallBack(Ref* pSender,Widget::TouchEventType type,int num) {
//    if (Widget::TouchEventType::ENDED != type) return;
//    auto user = UserDefault::getInstance();
//    switch (num) {
//    case 1:
//        effectSlider->setPercent(0);
//        user->setIntegerForKey(SOUND_VALUE_TEXT,effectSlider->getPercent());
//        HNAudioEngine::getInstance()->setSwitcjofEffect(false);
//        HNAudioEngine::getInstance()->setEffectsVolume(0.0f);
//        break;
//    case 2:
//        if (effectSlider->getPercent()==0) {
//            effectSlider->setPercent(50);
//            user->setIntegerForKey(SOUND_VALUE_TEXT, effectSlider->getPercent());
//            HNAudioEngine::getInstance()->setSwitcjofEffect(true);
//            HNAudioEngine::getInstance()->setEffectsVolume(0.5f);
//        }
//        break;
//    case 3:
//        musicSlider->setPercent(0);
//        user->setIntegerForKey(MUSIC_VALUE_TEXT,musicSlider->getPercent());
//        HNAudioEngine::getInstance()->setSwitchOfMusic(false);
//        //HNAudioEngine::getInstance()->stopBackgroundMusic();
//        HNAudioEngine::getInstance()->pauseBackgroundMusic();
//        HNAudioEngine::getInstance()->pauseGameBackgroundMusic();
//        HNAudioEngine::getInstance()->setBackgroundMusicVolume(0.0f);
//        break;
//    case 4:
//        if (musicSlider->getPercent()==0) {
//            musicSlider->setPercent(50);
//            user->setIntegerForKey(MUSIC_VALUE_TEXT,musicSlider->getPercent());
//            HNAudioEngine::getInstance()->setSwitchOfMusic(true);
//            HNAudioEngine::getInstance()->resumeBackgroundMusic();
////            HNAudioEngine::getInstance()->resumeGameBackgroundMusic();
//            //HNAudioEngine::getInstance()->playBackgroundMusic(GAME_BACKGROUND_MUSIC, true);
//            HNAudioEngine::getInstance()->setBackgroundMusicVolume(0.5f);
//        }
//        break;
//    default:
//        break;
//    }
//}

//void GameSetLayer::closeSetCallBack(Ref* pSender, Widget::TouchEventType type) {
//    Button* selectedBtn = (Button*)pSender;
//    switch (type) {
//    case cocos2d::ui::Widget::TouchEventType::BEGAN:
//        selectedBtn->setColor(Color3B(170, 170, 170));
//        break;
//    case cocos2d::ui::Widget::TouchEventType::ENDED:
//        selectedBtn->setColor(Color3B(255, 255, 255));
//        closeSet();
//        break;
//    case cocos2d::ui::Widget::TouchEventType::CANCELED:
//        selectedBtn->setColor(Color3B(255, 255, 255));
//        break;
//    default:
//        break;
//    }
//}

//void GameSetLayer::sliderCallback(Ref* pSender, Slider::EventType type, UI_TYPE num) {
//    Slider* pSlider = static_cast<Slider*>(pSender);
//    float l = (float)(pSlider->getPercent()) / 100;
//    //auto user = UserDefault::getInstance();
//
//    switch (num) {
//    case M_SLIDER: { //音乐音量条
//        //if (pSlider->getPercent() == 0) {
//        //    //HNAudioEngine::getInstance()->setSwitchOfMusic(false);
//        //    HNAudioEngine::getInstance()->pauseBackgroundMusic();
//        //    HNAudioEngine::getInstance()->pauseGameBackgroundMusic();
//        //} else {
//        //    //HNAudioEngine::getInstance()->setSwitchOfMusic(true);
//        //    HNAudioEngine::getInstance()->resumeBackgroundMusic();
//        //    HNAudioEngine::getInstance()->resumeGameBackgroundMusic();
//        //}
//        HNAudioEngine::getInstance()->setBackgroundMusicVolume(l);
//        //user->setIntegerForKey(MUSIC_VALUE_TEXT, pSlider->getPercent());
//    }
//        //user->flush();
//    break;
//    case E_SLIDER: { //音效音量条
//        //HNAudioEngine::getInstance()->setSwitchOfEffect(!(pSlider->getPercent() == 0));
//        HNAudioEngine::getInstance()->setEffectsVolume(l);
//        //user->setIntegerForKey(SOUND_VALUE_TEXT, pSlider->getPercent());
//    }
//        //user->flush();
//    break;
//    }
//}

////默认子界面
//void GameSetLayer::clearCurViewDat() {
//
//}

void GameSetLayer::createGeXingView(Node* parent) {
    _oldViewTag = creatGaiMi_Btn;
    auto mod = ModifyPasswordLayer::create();
    auto size = mod->getBackGroundSIze();
    mod->setPosition(Vec2(_kuang->getContentSize().width*0.4f, _kuang->getContentSize().height*0.5f));
    parent->addChild(mod);
    //_oldViewTag = createQianming_Btn;
    //auto bound = AutoGraph::create();
    //auto size = bound->getBackGroundSize();
    //bound->setPosition(Vec2(_kuang->getContentSize().width*0.5f - size.width *0.5f, _kuang->getContentSize().height*0.5f - size.height*0.5f));
    //parent->addChild(bound);
}

void GameSetLayer::createXiuGaiView(Node* parent) {
    _oldViewTag = creatGaiMi_Btn;
    auto mod = ModifyPasswordLayer::create();
    auto size = mod->getBackGroundSIze();
    mod->setPosition(Vec2(_kuang->getContentSize().width*0.4f, _kuang->getContentSize().height*0.5f));
    parent->addChild(mod);
}

void GameSetLayer::createBoundView(Node* parent) {
    _oldViewTag = creatBound_Btn;
    auto bound = BindPhoneLayer::create();
    auto size = bound->getBackGroundSIze();
    bound->setPosition(Vec2(_kuang->getContentSize().width*0.4f, _kuang->getContentSize().height*0.5f));
    parent->addChild(bound);
}

void GameSetLayer::createMusicView(Node* parent) {
    _oldViewTag = creatMusicEffect_Btn;
    auto music = MusicEffect::create();
    music->setPosition(Vec2(_kuang->getContentSize().width*0.25, _kuang->getContentSize().height*0.4));
    parent->addChild(music);
}


void  GameSetLayer::createHelpView(Node* parent) {
    _oldViewTag = creatHelp_Btn;
    auto help = HelpAndShenMing::createView(HelpAndShenMing::HELP);
    parent->addChild(help);
}

void GameSetLayer::createShenMingView(Node* parent) {
    _oldViewTag = creatShenMing_Btn;
    auto help = HelpAndShenMing::createView(HelpAndShenMing::SHEN_MING);
    parent->addChild(help);
}


