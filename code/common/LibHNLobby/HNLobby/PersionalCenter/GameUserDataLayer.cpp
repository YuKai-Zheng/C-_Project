#include "HNLobby/PersionalCenter/GameUserDataLayer.h"
#include "HNLobby/PersionalCenter/ModifyPassword.h"
#include "HNLobby/PersionalCenter/BindPhone.h"
#include "HNLobby/FontConfig.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/PlatformConfig.h"
#include <string>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "base/ClipboardHelper.h"
#endif

static const char* USER_HEAD_FRAME	= "platform/head/kuang.png";                  //user head box
//static const char* MEN_HEAD			= "platform/head/men_head.png";							//user head
//static const char* WOMEN_HEAD		= "platform/head/women_head.png";						//user head
//static const char* MEN_SEX		    = "platform/userData/userDataRes/male.png";							//user sex
//static const char* WOMEN_SEX		= "platform/userData/userDataRes/female.png";						//user sex
//
//static const char* PERSIONAL_JSON	= "platform/persional/PersionalCenterUi_1.json";

static const int MODIFYPASSWORDLAYER_TAG = 100;
static const int BINDPHONELAYER_TAG = 101;
static const char* GAME_BACKGROUND = "platform/hallBg.png";;
static const char* GAME_KUANG = "platform/newbankUi/1.png";						//user sex
static const char* GAME_LOGO = "platform/userData/1.png";
static const char* GAME_HEAD_LOGO = "platform/head/logo.png";
static const char* CLOSE_MENU_N = "platform/lobbyUi/PlatformUi_TOP/return.png";                         //closeMenu
static const char* GAME_LISTVIEW_BG = "platform/head/bg.png";                         //closeMenu
static const char* BRNN_NO_SEAT_SCROLL_BAR_BG = "platform/DeskList/tongyongseat/track2.png";                         //closeMenu
static const char* BRNN_NO_SEAT_SCROLL_BAR_FOREBG = "platform/DeskList/tongyongseat/track.png";                         //closeMenu
static const char* BRNN_NO_SEAT_SCROLL_BAR_BALL = "platform/DeskList/tongyongseat/handle.png";                         //closeMenu

#define WINSIZE Director::getInstance()->getWinSize()

GameUserDataLayer::GameUserDataLayer()
    : _canClose(false)
    , _canReplace(false)
    , _isSlider(true)
    , _isClose(false) {
    memset(&_ui, 0x0, sizeof(_ui));
}

GameUserDataLayer::~GameUserDataLayer() {
    PlatformLogic()->removeEventSelector(MDM_GP_USERINFO, ASS_GP_USERINFO_ACCEPT);
    PlatformLogic()->removeEventSelector(MDM_GP_USERINFO, ASS_GP_USERINFO_NOTACCEPT);
}

void GameUserDataLayer::openUserLayer(Node* parent, Vec2 vec, int zorder, int tag) {
    CCAssert(nullptr != parent, "parent is nullptr");
    Size winSize = Director::getInstance()->getWinSize();
    this->setName("GameUserDataLayer");
    this->setPosition(vec);
    parent->addChild(this, zorder, tag);
    //parent->runAction(EaseSineOut::create(MoveBy::create(0.2f, Vec2(-winSize.width, 0))));
    parent->runAction(MoveBy::create(0.3f, Vec2(-winSize.width, 0)));
}

void GameUserDataLayer::closeUserLayer() {
    _isClose = false;
    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
    auto winSize = Director::getInstance()->getWinSize();
    this->getParent()->runAction(Sequence::create(MoveBy::create(0.3f, Vec2(winSize.width, 0)),
    CCCallFunc::create([&]() {
        this->removeFromParent();
    }), nullptr));
}

bool GameUserDataLayer::init() {
    if ( !HNLayer::init()) return false;

    Size winSize = Director::getInstance()->getWinSize();

    enableKeyboard();

    loadBackGround(GAME_BACKGROUND);
    loadTopLogo(GAME_LOGO);
    loadKuang(GAME_KUANG);
    //屏蔽后面的层
    //colorLayer = LayerColor::create(Color4B(0, 0, 0, 150));
    //addChild(colorLayer, 10);

    //touchListener = EventListenerTouchOneByOne::create();
    //touchListener->onTouchBegan = [=](Touch* touch ,Event* event){
    //	return true;
    //};
    //touchListener ->setSwallowTouches(true);
    //_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, colorLayer);

    userDataLoader = CSLoader::createNode("csb/platform/userData/userDataNode.csb");
    _ui.layout = ( Layout* )userDataLoader->getChildByName("Panel_1");
    userDataLoader->setPosition(WINSIZE.width *0.5f,WINSIZE.height*0.46f);
    userDataLoader->setScale(0);
    addChild(userDataLoader);
    auto userDataLoaderShow = Sequence::create(ScaleTo::create(0.2f, 1.0f), nullptr);
    userDataLoader->runAction(userDataLoaderShow);

    //// 玩家资料框背景
    //_ui.layout = dynamic_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile(PERSIONAL_JSON));
    //if (nullptr != _ui.layout)
    //{
    //	_ui.layout->setAnchorPoint(Vec2(0.5f, 0.5f));
    //	_ui.layout->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
    //	addChild(_ui.layout, 2, 3);

    MSG_GP_R_LogonResult& LogonResult = PlatformLogic()->loginResult;
    // 玩家头像
    _ui.userHead = UserHead::create(USER_HEAD_FRAME);
    if (switchToLayoutControl(_ui.layout, "Text_head", _ui.userHead)) {
        char str[100] = { 0 };
        sprintf(str, "%s%d.png", "common/head/", LogonResult.bLogoID);
        _ui.userHead->setUserHead(str);
        _ui.userHead->setScale(1.1);
        _ui.userHead->onHeadClickEvent = [&](UserHead* sender, Vec2 touchVec) {
            loadModifyHeadLayer();
        };
    }
    //昵称
    _ui.Label_UserNickName = ( Text* )_ui.layout->getChildByName("Text_nickName");
    _ui.Label_UserNickName->setString(LogonResult.nickName);
    std::string nickName(ForceGBKToUtf8(LogonResult.nickName));
    if (!nickName.empty()) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        _ui.Label_UserNickName->setString(ForceGBKToUtf8(LogonResult.nickName));
#else
        _ui.Label_UserNickName->setString((ForceGBKToUtf8(LogonResult.nickName)));
#endif
    } else {
        _ui.Label_UserNickName->setString(ForceGBKToUtf8("未知"));
    }
    //性别
    //_ui.Label_Sex = ( Text* )_ui.layout->getChildByName("Text_sex");
    //_ui.Label_Sex->setString(LogonResult.bBoy ? GBKToUtf8("男") : GBKToUtf8("女"));
    //_ui.Image_Sex = ( ImageView* ) _ui.layout->getChildByName("Image_sexImage");
    //_ui.Image_Sex->loadTexture(LogonResult.bBoy ? MEN_SEX : WOMEN_SEX);

    char buffer[64];
    //魅力
    _ui.Label_Fascination = ( Text* )_ui.layout->getChildByName("Text_fascination");
    sprintf(buffer, "%u", LogonResult.dwFascination);
    _ui.Label_Fascination->setString(buffer);
    //金币
    _ui.Label_Money = ( Text* )_ui.layout->getChildByName("Text_money");
    sprintf(buffer, "%lld", LogonResult.i64Money);
    _ui.Label_Money->setString(buffer);

    //银行
    _ui.Label_BankMoney = (Text*)_ui.layout->getChildByName("Text_bankMoney");
    sprintf(buffer, "%llu", LogonResult.i64Bank);
    _ui.Label_BankMoney->setString(buffer);

    //个性签名
    _ui.Label_geXing = (Text*)_ui.layout->getChildByName("Text_qianMing");
    _ui.Label_geXing->setString(ForceGBKToUtf8(LogonResult.szSignDescr));

    //登陆名
    _ui.Label_LandName = ( Text* ) _ui.layout->getChildByName("Text_UserName");
    sprintf(buffer, GBKToUtf8("%u"), LogonResult.szName);
    _ui.Label_LandName->setString(buffer);
    //玩家ID
    _ui.Label_UserID = ( Text* ) _ui.layout->getChildByName("Text_ID");
    sprintf(buffer, GBKToUtf8("%u"), LogonResult.dwUserID);
    _ui.Label_UserID->setString(buffer);
    //手机号
    _ui.Label_phone = ( Text* )_ui.layout->getChildByName("Text_phone");
    sprintf(buffer, "%u", LogonResult.szMobileNo);
    _ui.Label_phone->setString(LogonResult.iBindMobile == 1 ? buffer : GBKToUtf8( "未绑定！" ));
    //修改
    _ui.Button_modify = (Button*)_ui.layout->getChildByName("Button_1");
    _ui.Button_modify->addTouchEventListener(CC_CALLBACK_2(GameUserDataLayer::modifyUserHeadEventCallBack, this));

    //修改密码
    _ui.Button_ModifyPassword = ( Button* )_ui.layout->getChildByName("Button_modifyPass");
    _ui.Button_ModifyPassword->addTouchEventListener(CC_CALLBACK_2(GameUserDataLayer::modifyPasswordUIEventCallBack, this));
    //绑定手机
    _ui.Button_BindPhone = ( Button* )_ui.layout->getChildByName("Button_bindPhone");
    _ui.Button_BindPhone->addTouchEventListener(CC_CALLBACK_2(GameUserDataLayer::bindPhoneUIEventCallBack, this));

    //复制ID
    _ui.Button_Copy = (Button*)_ui.layout->getChildByName("button_copy");
    _ui.Button_Copy->setShowEffect(false);
    _ui.Button_Copy->addTouchEventListener(CC_CALLBACK_2(GameUserDataLayer::buttonCopyCallBack, this));
    // 昵称编辑按钮
    _ui.Button_AutoGraph = (Button*)_ui.layout->getChildByName("button_aotoGraph");
    _ui.Button_AutoGraph->setShowEffect(false);
    _ui.Button_AutoGraph->addTouchEventListener(CC_CALLBACK_2(GameUserDataLayer::autoGraphEventCallBack, this));
    //个性签名编辑按钮
    _ui.Button_NickName = (Button*)_ui.layout->getChildByName("button_nickName");
    _ui.Button_NickName->setShowEffect(false);
    _ui.Button_NickName->addTouchEventListener(CC_CALLBACK_2(GameUserDataLayer::nickNameEventCallBack, this));



    auto nickNameTextField = (TextField*)_ui.layout->getChildByName("textField_nickName");
    nickNameTextField->setVisible(false);
    _ui.HNEeditBoxNickName = HNEditBox::createEditBox(nickNameTextField, this);
    _ui.HNEeditBoxNickName->setVisible(false);

    auto autoGraph = (TextField*)_ui.layout->getChildByName("textField_autoGraph");
    autoGraph->setVisible(false);
    _ui.HNEeditBoxAutoGraph = HNEditBox::createEditBox(autoGraph, this);
    _ui.HNEeditBoxAutoGraph->setVisible(false);

    _ui.nickNameBg = (Sprite*)_ui.layout->getChildByName("bg_nick");
    _ui.autoGraphBg = (Sprite*)_ui.layout->getChildByName("bg_graph");

    _ui.SpriteBankMoney = (Sprite*)_ui.layout->getChildByName("bankMoneySpr");		//银行标签
    _ui.SpriteGameMoney = (Sprite*)_ui.layout->getChildByName("gameMoneySpr");	//游戏金币标签
    _ui.SpriteUserId = (Sprite*)_ui.layout->getChildByName("userIdSpr");						//用户编号标签
    _ui.autoGraph = (Sprite*)_ui.layout->getChildByName("autoGraphSpr");					//个性签名标签

    if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) {
        _ui.SpriteBankMoney->setVisible(false);
        _ui.Label_BankMoney->setVisible(false);
        _ui.Button_AutoGraph->setPositionY(_ui.Label_geXing->getPositionY());
        _ui.autoGraphBg->setPositionY(_ui.Label_geXing->getPositionY());
        _ui.autoGraph->setPositionY(_ui.autoGraph->getPositionY() + _ui.SpriteGameMoney->getContentSize().height *1.5f);
        _ui.Label_geXing->setPositionY(_ui.autoGraph->getPositionY());
    }

    // 关闭按钮
    auto closeBtn = Button::create(CLOSE_MENU_N, CLOSE_MENU_N);
    closeBtn->setPosition(Vec2(WINSIZE.width - closeBtn->getContentSize().width*0.55, WINSIZE.height - closeBtn->getContentSize().height *0.55));
    addChild(closeBtn);
    closeBtn->addTouchEventListener(CC_CALLBACK_2(GameUserDataLayer::closeEvent, this));

    _ui.isSelectNick = false;
    _ui.isSelectGraph = false;
    _ui.isGraphSave = false;
    _ui.isNickSave = false;

    //
    PlatformLogic()->addEventSelector(MDM_GP_SET_LOGO, ASS_GP_SET_LOGO, HN_SOCKET_CALLBACK(GameUserDataLayer::modifyOKSelector, this));
    //PlatformLogic()->addEventSelector(MDM_GP_SET_LOGO, ASS_GP_SET_LOGO, HN_SOCKET_CALLBACK(GameUserDataLayer::modifyNoSelector, this));

    PlatformLogic()->addEventSelector(MDM_GP_USERINFO, ASS_GP_USERINFO_ACCEPT, HN_SOCKET_CALLBACK(GameUserDataLayer::modifyNickAndGraphOKSelector, this));
    //PlatformLogic()->addEventSelector(MDM_GP_USERINFO, ASS_GP_USERINFO_NOTACCEPT, HN_SOCKET_CALLBACK(GameUserDataLayer::modifyNickAndGraphNoSelector, this));
    PlatformLogic()->addEventSelector(MDM_GP_USERINFO, ASS_GP_USERINFO_NOTACCEPT_BADWORD, HN_SOCKET_CALLBACK(GameUserDataLayer::modifyFailureSelector, this));

    return true;
}

void GameUserDataLayer::closePrompt() {
    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
    scallViewBg->runAction(Sequence::create(ScaleTo::create(0.1f, 0.3f), CCCallFunc::create([&]() {
        scallViewBg->removeFromParent();
        _layerColor->removeFromParentAndCleanup(true);
    }), nullptr));
}

void GameUserDataLayer::loadModifyHeadLayer() {
    _layerColor = LayerColor::create(Color4B(0, 0, 0, 100));
    this->addChild(_layerColor);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
        closePrompt();
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, _layerColor);

    scallViewBg = ImageView::create(GAME_LISTVIEW_BG);
    scallViewBg->setTouchEnabled(true);
    scallViewBg->setSwallowTouches(true);
    auto size = scallViewBg->getContentSize();
    scallViewBg->setPosition(Vec2(WINSIZE.width*0.5f, WINSIZE.height*0.5f));
    this->addChild(scallViewBg,1);

    auto btn = Button::create("platform/curcularExit.png");
    btn->setPosition(Vec2(scallViewBg->getContentSize().width - btn->getContentSize().width*0.3f, scallViewBg->getContentSize().height - btn->getContentSize().height*0.1f));
    btn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            closePrompt();
        }
    });

    scallViewBg->addChild(btn);

    auto logo = Sprite::create(GAME_HEAD_LOGO);
    logo->setPosition(size.width*0.5f, size.height*0.89f);
    scallViewBg->addChild(logo, 1);

    m_pTableView = BTableView::createWithSlider(BRNN_NO_SEAT_SCROLL_BAR_BG, BRNN_NO_SEAT_SCROLL_BAR_FOREBG, BRNN_NO_SEAT_SCROLL_BAR_BALL, [this](TableView* table, ssize_t idx)->TableViewCell* {
        TableViewCell * Cell = table->dequeueCell();
        if (!Cell) {
            Cell = new TableViewCell();
            Cell->autorelease();
        }
        Cell->removeAllChildrenWithCleanup(true);
        for (int i = 0; i < 6; i++) {
            char str[100] = { 0 };
            sprintf(str, "%s%d.png", "common/head/", idx * 6 + (i));
            auto  button = Button::create(str, str);
            if (button) {
                auto bg = ImageView::create("platform/head/ditu.png");
                bg->setPosition(Vec2((bg->getContentSize().width + 6)*i + bg->getContentSize().width / 2, bg->getContentSize().height / 2.1));
                Cell->addChild(bg);

                button->setScale(0.7);
                button->setShowEffect(false);
                button->setSwallowTouches(false);
                button->setTag(idx * 6 + (i));
                button->setPosition(Vec2(bg->getPositionX() - 3, bg->getPositionY()+2));
                button->addTouchEventListener(CC_CALLBACK_2(GameUserDataLayer::headButtonCallBack, this));
                Cell->addChild(button);
            }
        }
        return Cell;
    }
                                               );
    m_pTableView->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    m_pTableView->setViewSize(Size(size.width*0.92,size.height*0.76));
    m_pTableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    m_pTableView->setProgressOpacity(0);
    m_pTableView->setNumberOfCellsInTableView(12);
    m_pTableView->setTableCellSize(Size(m_pTableView->getTable()->getViewSize().width, m_pTableView->getTable()->getViewSize().height/3.0f));
    m_pTableView->reloadData();
    m_pTableView->setSliderScaleY(0.85);
    m_pTableView->setPosition(Vec2(size.width*0.02,size.height*0.05 + 6));
    m_pTableView->getSlider()->setValue(0.0f);
    scallViewBg->addChild(m_pTableView);
}


void GameUserDataLayer::loadBackGround(const char* fileName) {

    auto bg = Sprite::create(fileName);
    bg->setPosition(WINSIZE.width * 0.5f, WINSIZE.height * 0.5f);
    addChild(bg, -1);

    float _xScale = WINSIZE.width / bg->getContentSize().width;
    float _yScale = WINSIZE.height / bg->getContentSize().height;
    bg->setScaleX(_xScale);
    bg->setScaleY(_yScale);
}

void GameUserDataLayer::loadTopLogo(const char* fileName) {
    auto logo = Sprite::create(fileName);
    logo->setPosition(WINSIZE.width * 0.5f, WINSIZE.height - logo->getContentSize().height *0.5f + 10);
    addChild(logo, -1);
}
void GameUserDataLayer::loadKuang(const char* fileName) {
    auto logo = Sprite::create(fileName);
    logo->setPosition(WINSIZE.width * 0.5f, WINSIZE.height * 0.48f);
    addChild(logo, -1);
}
bool GameUserDataLayer::modifyOKSelector(HNSocketMessage* socketMessage) {
    CCAssert(sizeof (MSG_GP_R_SetLogo) == socketMessage->objectSize, "MSG_GP_R_BindMobile is error.");
    if (socketMessage->objectSize != sizeof(MSG_GP_R_SetLogo)) return true;

    MSG_GP_R_SetLogo* data = (MSG_GP_R_SetLogo*)(socketMessage->object);

    switch (data->dwCommanResult) {
    case 1: {
        GamePromptLayer::create()->showPrompt(GBKToUtf8("修改头像成功！！！"));
        PlatformLogic()->getInstance()->loginResult.bLogoID = _curSelectHeadId;
        char str[100] = { 0 };
        sprintf(str, "%s%d.png", "common/head/", PlatformLogic()->getInstance()->loginResult.bLogoID);
        _ui.userHead->setUserHead(str);
        upDateHeadCallBack();
        closePrompt();
        break;
    }
    case 0: {
        GamePromptLayer::create()->showPrompt(GBKToUtf8("修改头像失败。。。"));
        break;
    }
    default:
        break;
    }

    return false;
}

bool GameUserDataLayer::modifyNoSelector(HNSocketMessage* socketMessage) {
    //CCAssert(sizeof (MSG_GP_UserInfo) == socketMessage->objectSize, "MSG_GP_R_BindMobile is error.");
    //if (socketMessage->objectSize != sizeof(MSG_GP_UserInfo)) return true;

    return false;
}

void GameUserDataLayer::nickNameEventCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED)
        return;
    Button* selectedBtn = (Button*)pSender;
    std::string  path;

    if (selectedBtn->getName() == "button_nickName") {
        if (_ui.isSelectNick) {
            modifyNickButtonCallBack(selectedBtn, type);
        } else {
            _ui.isNickSave = false;
            _ui.isSelectNick = !_ui.isSelectNick;
            path = _ui.isSelectNick != false ? "platform/userData/userDataRes/save.png" : "platform/userData/userDataRes/edit.png";

            bool bgVisible = _ui.isSelectNick != false ? true : false;
            _ui.HNEeditBoxNickName->setVisible(bgVisible);
            _ui.nickNameBg->setVisible(bgVisible);
            _ui.Label_UserNickName->setVisible(!bgVisible);
            selectedBtn->loadTextureNormal(path);
        }
    }
}

void GameUserDataLayer::autoGraphEventCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED)
        return;
    Button* selectedBtn = (Button*)pSender;
    std::string  path;

    if (selectedBtn->getName() == "button_aotoGraph") {
        if (_ui.isSelectGraph) {
            modifyGraphButtonCallBack(selectedBtn, type);
        } else {
            _ui.isGraphSave = false;
            _ui.isSelectGraph = !_ui.isSelectGraph;
            path = _ui.isSelectGraph != false ? "platform/userData/userDataRes/save.png" : "platform/userData/userDataRes/edit.png";

            bool bgVisible = _ui.isSelectGraph != false ? true : false;
            _ui.HNEeditBoxAutoGraph->setVisible(bgVisible);
            _ui.autoGraphBg->setVisible(bgVisible);
            _ui.Label_geXing->setVisible(!bgVisible);
            selectedBtn->loadTextureNormal(path);
        }
    }
}

void GameUserDataLayer::headButtonCallBack(Ref* pSender, Widget::TouchEventType type) {
    Button* selectedBtn = (Button*)pSender;
    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::BEGAN:
        _isSlider = true;
        break;
    case cocos2d::ui::Widget::TouchEventType::MOVED:
        _isSlider = false;
        break;
    case cocos2d::ui::Widget::TouchEventType::ENDED: {
        HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
        if (_isSlider) {
            MSG_GP_S_SET_LOGO logonResult;
            memset(&logonResult, 0, sizeof(logonResult));

            _curSelectHeadId = selectedBtn->getTag();
            logonResult.iLogoID = selectedBtn->getTag();
            logonResult.iUserID = PlatformLogic()->loginResult.dwUserID;

            PlatformLogic()->sendData(MDM_GP_SET_LOGO, ASS_GP_SET_LOGO, &logonResult, sizeof(logonResult));
            break;
        }
    }
    case cocos2d::ui::Widget::TouchEventType::CANCELED:
        break;
    default:
        break;
    }
}



void GameUserDataLayer::closeEvent(Ref* pSender, Widget::TouchEventType type) {
    Button* selectedBtn = (Button*)pSender;
    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::BEGAN:
        selectedBtn->setColor(Color3B(170, 170, 170));
        break;
    case cocos2d::ui::Widget::TouchEventType::ENDED:
        selectedBtn->setColor(Color3B(255, 255, 255));
        closeUserLayer();
        break;
    case cocos2d::ui::Widget::TouchEventType::CANCELED:
        selectedBtn->setColor(Color3B(255, 255, 255));
        break;
    default:
        break;

    }
}

void GameUserDataLayer::modifyPasswordUIEventCallBack(Ref* pSender, Widget::TouchEventType type) {
    Button* selectedBtn = ( Button* )pSender;
    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::BEGAN:
        selectedBtn->setColor(Color3B(170, 170, 170));
        break;
    case cocos2d::ui::Widget::TouchEventType::ENDED:
        selectedBtn->setColor(Color3B(255, 255, 255));
        userDataLoader->runAction(Sequence::create(ScaleTo::create(0.1f, 0.3f),CCCallFunc::create([&]() {
            auto modifyPasswordLayer = ModifyPasswordLayer::create();
            modifyPasswordLayer->show(this->getParent(), this->getParent()->getLocalZOrder() + 1100, MODIFYPASSWORDLAYER_TAG);
            this->removeFromParent();
        }), nullptr));
        break;
    case cocos2d::ui::Widget::TouchEventType::CANCELED:
        selectedBtn->setColor(Color3B(255, 255, 255));
        break;
    default:
        break;
    }
}

void GameUserDataLayer::buttonCopyCallBack(Ref* pSender, Widget::TouchEventType type) {
    if(type != Widget::TouchEventType::ENDED)
        return;
#if(CC_TARGET_PLATFORM  == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;//定义Jni函数信息结构体
    //getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"com/jlx/MixProject/AppActivity","paste", "(Ljava/lang/String;)V");
    if (!isHave) {

    } else {

        jobject para = minfo.env->NewStringUTF(_ui.Label_UserID->getString().c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID,para);
        GamePromptLayer::create()->showPrompt(GetText("USERINFO_COPYID"));

    }
#elif CC_TARGET_PLATFORM  == CC_PLATFORM_IOS
    ClipboardHelper::copyStr(_ui.Label_UserID->getString());
    GamePromptLayer::create()->showPrompt(GetText("USERINFO_COPYID"));
#endif
}

void GameUserDataLayer::bindPhoneUIEventCallBack(Ref* pSender, Widget::TouchEventType type) {
    Button* selectedBtn = ( Button* )pSender;
    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::BEGAN:
        selectedBtn->setColor(Color3B(170, 170, 170));
        break;
    case cocos2d::ui::Widget::TouchEventType::ENDED:
        selectedBtn->setColor(Color3B(255, 255, 255));
        userDataLoader->runAction(Sequence::create(ScaleTo::create(0.1f, 0.3f),CCCallFunc::create([&]() {
            auto bindPhoneLayer = BindPhoneLayer::create();
            bindPhoneLayer->show(this->getParent(), this->getParent()->getLocalZOrder()+1100, BINDPHONELAYER_TAG);
            this->removeFromParent();
        }), nullptr));
        break;
    case cocos2d::ui::Widget::TouchEventType::CANCELED:
        selectedBtn->setColor(Color3B(255, 255, 255));
        break;
    default:
        break;
    }


}
// 修改按钮回调
void GameUserDataLayer::modifyUserHeadEventCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::ENDED) {
        //GamePromptLayer::create()->showPrompt(GBKToUtf8("头像功能开发中敬请期待！！！"));
        loadModifyHeadLayer();
        return;
    }
    MSG_GP_UserInfo logonResult;
    memset(&logonResult, 0, sizeof(logonResult));

    strcpy(logonResult.szMD5Pass, PlatformLogic()->loginResult.szMD5Pass);
    PlatformLogic()->sendData(MDM_GP_USERINFO, ASS_GP_USERINFO_UPDATE_BASE, &logonResult, sizeof(logonResult));
}

void GameUserDataLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (EventKeyboard::KeyCode::KEY_BACK == keyCode)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    if (EventKeyboard::KeyCode::KEY_BACKSPACE == keyCode)
#endif
    {
        do {
            ModifyPasswordLayer* serviceLayer = dynamic_cast<ModifyPasswordLayer*>(this->getChildByTag(MODIFYPASSWORDLAYER_TAG));
            if (nullptr != serviceLayer) {
                break;
            }

            BindPhoneLayer* setLayer = dynamic_cast<BindPhoneLayer*>(this->getChildByTag(BINDPHONELAYER_TAG));
            if (nullptr != setLayer) {
                break;
            }
            _canClose = true;
        } while (0);
    }
}


void GameUserDataLayer::modifyGraphButtonCallBack(Ref*pShender, Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::ENDED) {
        HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
        if (strcmp(_ui.HNEeditBoxAutoGraph->getString().c_str(), PlatformLogic()->loginResult.szSignDescr) == 0) {
            GamePromptLayer::create()->showPrompt(ForceGBKToUtf8("内容相同请重新输入！！！"));
            return;
        }

        if (!Tools::isSpecialCharacter(_ui.HNEeditBoxAutoGraph->getString().c_str())) {
            GamePromptLayer::create()->showPrompt(ForceGBKToUtf8("含特殊符号！"));
            return;
        }

        _ui.isGraphSave = true;
        _ui.isNickSave = false;
        MSG_GP_UserInfo logonResult;
        memset(&logonResult, 0, sizeof(logonResult));

        logonResult.dwUserID = PlatformLogic()->loginResult.dwUserID;
        logonResult.i64Bank = PlatformLogic()->loginResult.i64Bank;
        logonResult.i64Money = PlatformLogic()->loginResult.i64Money;
        strcpy(logonResult.szSignDescr, Utf8ToGB(_ui.HNEeditBoxAutoGraph->getString().c_str()));
        strcpy(logonResult.szMD5Pass, PlatformLogic()->loginResult.szMD5Pass);
        PlatformLogic()->sendData(MDM_GP_USERINFO, ASS_GP_USERINFO_UPDATE_BASE, &logonResult, sizeof(logonResult));
        LoadingLayer::createLoading(this, gFontConfig_22, "", "");
        return;
    }
}

void GameUserDataLayer::modifyNickButtonCallBack(Ref*pShender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED)
        return;
    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
    if (strcmp(_ui.HNEeditBoxNickName->getString().c_str(), PlatformLogic()->loginResult.nickName) == 0) {
        GamePromptLayer::create()->showPrompt(ForceGBKToUtf8("内容相同请重新输入！！！"));
        return;
    }
    //获取昵称
    std::string nickEdit = _ui.HNEeditBoxNickName->getString();
    if (nickEdit.empty()) {
        GamePromptLayer::create()->showPrompt(ForceGBKToUtf8("昵称不能为空！"));
        return;
    }

    if (_ui.HNEeditBoxNickName->getStringLength() < 6 ||
            _ui.HNEeditBoxNickName->getStringLength() > 18)			{
        GamePromptLayer::create()->showPrompt(ForceGBKToUtf8("请输入2-6位昵称！"));
        return;
    }

    if (!Tools::isSpecialCharacter(nickEdit.c_str())) {
        GamePromptLayer::create()->showPrompt(ForceGBKToUtf8("含特殊符号！"));
        return;
    }

    _ui.isNickSave = true;
    _ui.isGraphSave = false;
    MSG_GP_UserInfo logonResult;
    auto  result = PlatformLogic()->loginResult;

    memset(&logonResult, 0, sizeof(logonResult));

    CCLOG("length = %s ", _ui.HNEeditBoxNickName->getString().c_str());

    logonResult.dwUserID = result.dwUserID;
    strcpy(logonResult.szMD5Pass, result.szMD5Pass);
    strcpy(logonResult.szSignDescr, Utf8ToGB(result.szSignDescr));
    strcpy(logonResult.szRealName, result.szRealName);
    strcpy(logonResult.szIDCardNo, result.szIDCardNo);
    strcpy(logonResult.szMobileNo, result.szMobileNo);
    strcpy(logonResult.szQQNum, result.szQQNum);
    strcpy(logonResult.szAdrProvince, result.szAdrProvince);
    strcpy(logonResult.szAdrCity, result.szAdrCity);
    strcpy(logonResult.szZipCode, result.szZipCode);
    strcpy(logonResult.szEmail, result.szEmail);
    strcpy(logonResult.szAddress, result.szAddress);
    strcpy(logonResult.szSchooling, result.szSchooling);
    strcpy(logonResult.szHomePage, result.szHomePage);
    strcpy(logonResult.szTelNo, result.szTelNo);
    strcpy(logonResult.szMSNID, result.szMSNID);
    strcpy(logonResult.nickName, Utf8ToGB(_ui.HNEeditBoxNickName->getString().c_str()));

    PlatformLogic()->sendData(MDM_GP_USERINFO, ASS_GP_USERINFO_UPDATE_DETAIL, &logonResult, sizeof(logonResult));
    LoadingLayer::createLoading(this, gFontConfig_22, "", "");
    return;
}

bool GameUserDataLayer::modifyNickAndGraphOKSelector(HNSocketMessage* socketMessage) {
    LoadingLayer::removeLoading(this);
    if (_ui.isNickSave == true) {
        _ui.isSelectNick = !_ui.isSelectNick;
        std::string path = _ui.isSelectNick != false ? "platform/userData/userDataRes/save.png" : "platform/userData/userDataRes/edit.png";

        path = _ui.isSelectNick != false ? "platform/userData/userDataRes/save.png" : "platform/userData/userDataRes/edit.png";

        bool bgVisible = _ui.isSelectNick != false ? true : false;
        _ui.HNEeditBoxNickName->setVisible(bgVisible);
        _ui.nickNameBg->setVisible(bgVisible);
        _ui.Label_UserNickName->setVisible(!bgVisible);
        _ui.Button_NickName->loadTextureNormal(path);
        //CCLOG("", _ui.HNEeditBoxNickName->getString());
        _ui.Label_UserNickName->setString(_ui.HNEeditBoxNickName->getString());
        GamePromptLayer::create()->showPrompt(ForceGBKToUtf8("修改昵称成功！"));
        strcpy(PlatformLogic()->loginResult.nickName, Utf8ToGB(_ui.HNEeditBoxNickName->getString().c_str()));
        this->upDateNickCallBack(_ui.HNEeditBoxNickName->getString());
        _ui.isNickSave = false;
        return false;
    }
    if (_ui.isGraphSave == true) {
        _ui.isSelectGraph = !_ui.isSelectGraph;
        std::string path = _ui.isSelectGraph != false ? "platform/userData/userDataRes/save.png" : "platform/userData/userDataRes/edit.png";

        bool bgVisible = _ui.isSelectGraph != false ? true : false;
        _ui.HNEeditBoxAutoGraph->setVisible(bgVisible);
        _ui.autoGraphBg->setVisible(bgVisible);
        _ui.Label_geXing->setVisible(!bgVisible);
        _ui.Button_AutoGraph->loadTextureNormal(path);
        GamePromptLayer::create()->showPrompt(ForceGBKToUtf8("修改签名成功！"));

        strcpy(PlatformLogic()->loginResult.szSignDescr, Utf8ToGB(_ui.HNEeditBoxAutoGraph->getString().c_str()));
        _ui.Label_geXing->setString(_ui.HNEeditBoxAutoGraph->getString().c_str());
        _ui.HNEeditBoxAutoGraph->setString("");
        _ui.isGraphSave = false;
        return false;
    }
    return false;
}

bool GameUserDataLayer::modifyNickAndGraphNoSelector(HNSocketMessage* socketMessage) {
    LoadingLayer::removeLoading(this);
    GamePromptLayer::create()->showPrompt(ForceGBKToUtf8("修改失败!"));
    return false;
}

bool GameUserDataLayer::modifyFailureSelector(HNSocketMessage* socketMessage) {
    LoadingLayer::removeLoading(this);
    GamePromptLayer::create()->showPrompt(GetText("Sign_Default"));
    return true;
}
