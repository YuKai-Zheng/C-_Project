#include "GameUserHeadV66.h"
#include "../CommonMacro.h"
#include "PlatformResV66.h"
#include "cocostudio/CocoStudio.h"
#include "Sound/HNAudioEngine.h"
#include "HNPlatform/HNPlatformLogic.h"
#include "../pattern/CommandManager.h"
#include "CommonRes.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/pattern/factory/background/BackgroundFactory.h"

const char* USER_HEAD_TITLE = "v66/module/gerenxinxi/touxiangliebiao.png";

bool GameUserHeadV66::init() {
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

	auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 180));
	addChild(colorLayer);
    
    auto bg = ImageView::create(MODULE_BG3_V66);

	auto sizeBg = bg->getContentSize();
	bg->ignoreContentAdaptWithSize(false);
	bg->setContentSize(sizeBg - Size(sizeBg.width/10, sizeBg.height/20));
    bg->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 - WIN_SIZE.height / 22));
    bg->setTouchEnabled(true);
    addChild(bg);
    
    auto title = Sprite::create("v66/common/biaoti/36.png");
    title->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height - 44));
    bg->addChild(title);

	auto closeBtnBg = Button::create("v66/common_v2/tc_btn_close.png", "v66/common_v2/tc_btn_close.png", "Default/Button_Disable.png");
	closeBtnBg->setPosition(Vec2(bg->getContentSize().width - 40, bg->getContentSize().height));
	closeBtnBg->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) return;
		close();
	});
	bg->addChild(closeBtnBg);

    auto closeBtn = Button::create("v66/common_v2/tc_ph_close.png", "v66/common_v2/tc_ph_close.png");
    closeBtn->setPosition(Vec2(bg->getContentSize().width - 40, bg->getContentSize().height));
    bg->addChild(closeBtn);
    closeBtn->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type){
        if (type != Widget::TouchEventType::ENDED) return;
        close();
    });

    isClose = true;

    auto userHeadLayer = CSLoader::createNode(USER_HEAD_V66);
    userHeadLayer->setPosition(bg->getContentSize()/2);
    bg->addChild(userHeadLayer);

    headList = (ListView*)userHeadLayer->getChildByName("headList");
    if (headList) {
        headList->setScrollBarAutoHideEnabled(false);
        auto si = headList->getContentSize();
        headList->setContentSize(si-Size(0,35));
    }


    //是否缓存好微信头像
    m_bHasWXHead = false;
    bool isWechatLogin = UserDefault::getInstance()->getBoolForKey(WX_LOGIN);
    auto fileName = FileUtils::getInstance()->getWritablePath() + WX_HEAD_IMG_NAME;
    bool localImgExist = FileUtils::getInstance()->isFileExist(fileName);
    m_bHasWXHead = isWechatLogin && localImgExist;
    
    maxValueInRow = 6;
    int length = 40;
    length = m_bHasWXHead ? length+1 : length;
    int loopTimes = length / maxValueInRow;
	loopTimes = length % maxValueInRow == 0 ? loopTimes : loopTimes + 1;
    for (int row = 0; row < loopTimes; row++) {
        Layout *default_item = Layout::create();
        //default_item->setTouchEnabled(true);
        default_item->setContentSize(Size(926, 160));

        for (int i = 0; i < maxValueInRow; i++) {
            int index = row * maxValueInRow + i;
            if (index < length) {
                Node *item = createItem(index);
                default_item->addChild(item);
            }
        }
        headList->pushBackCustomItem(default_item);
    }
    headList->forceDoLayout();

    PlatformLogic()->addEventSelector(MDM_GP_SET_LOGO, ASS_GP_SET_LOGO, HN_SOCKET_CALLBACK(GameUserHeadV66::modifyOKSelector, this));

    return true;
}

Node* GameUserHeadV66::createItem(int index) {
    float fSpaceX = 2;
    
    Node* item = Node::create();
    
    if(index == 0 && m_bHasWXHead)
    {
        auto btn = Button::create("v66/module/gerenxinxi/6.png");
        btn->setTag(WX_HEAD_IMG_ID);
        btn->setShowEffect(false);
        btn->setSwallowTouches(false);
        btn->setPosition(Vec2(btn->getContentSize().width/2 + fSpaceX, btn->getContentSize().height/2));
        btn->addTouchEventListener(CC_CALLBACK_2(GameUserHeadV66::headButtonCallBack, this));

        item->addChild(btn);
        
        auto userHead = UserHead::create("v66/module/gerenxinxi/6.png");
        auto fileName = FileUtils::getInstance()->getWritablePath() + WX_HEAD_IMG_NAME;
        userHead->setClipUserHeadWithFile(fileName, USER_HEAD_CLIP_STENCIL_BG, 0.8f);
        userHead->setPosition(btn->getPosition());
        item->addChild(userHead);
        
        return item;
    }
    
    int headTag = m_bHasWXHead ? index - 1 : index;
    std::string path = StringUtils::format(PLAYER_HEAD_V66, headTag);
    auto  button = Button::create(path, path);
    if (button) {
        auto bg = ImageView::create("v66/module/gerenxinxi/6.png");
        item->addChild(bg);

        button->setTag(headTag);
        button->setScale(0.8);
        button->setShowEffect(false);
        button->setSwallowTouches(false);
        Size size = bg->getContentSize();
        button->setPositionX(fSpaceX + size.width/2 + (index % maxValueInRow)*(size.width + fSpaceX));
        button->setPositionY(size.height / 2);
        button->addTouchEventListener(CC_CALLBACK_2(GameUserHeadV66::headButtonCallBack, this));

        bg->setPosition(button->getPosition());

        item->addChild(button);
    }

    return item;
}

void GameUserHeadV66::headButtonCallBack(Ref* pSender, Widget::TouchEventType type) {
    Button* selectedBtn = (Button*)pSender;
    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::BEGAN:
        isMovingList = false;
        gameBtnY = selectedBtn->getTouchBeganPosition().y;
        break;
    case cocos2d::ui::Widget::TouchEventType::MOVED:
        isMovingList = true;
        gameBtnY = selectedBtn->getTouchMovePosition().y;
        break;
    case cocos2d::ui::Widget::TouchEventType::ENDED: {
        auto _gameBtnY = selectedBtn->getTouchEndPosition().y;
        if (fabs(_gameBtnY-gameBtnY)<20) {
            HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
            MSG_GP_S_SET_LOGO logonResult;
            memset(&logonResult, 0, sizeof(logonResult));
            logonResult.iLogoID = selectedBtn->getTag();
            logonResult.iUserID = PlatformLogic()->loginResult.dwUserID;
            
            UserDefault::getInstance()->setIntegerForKey("TMP_LOGO_ID", logonResult.iLogoID);
            PlatformLogic()->sendData(MDM_GP_SET_LOGO, ASS_GP_SET_LOGO, &logonResult, sizeof(logonResult));
        }
    }
    case cocos2d::ui::Widget::TouchEventType::CANCELED:
        break;
    }
}

bool GameUserHeadV66::modifyOKSelector(HNSocketMessage* socketMessage) {
    CCAssert(sizeof(MSG_GP_R_SetLogo) == socketMessage->objectSize, "MSG_GP_R_BindMobile is error.");
    if (socketMessage->objectSize != sizeof(MSG_GP_R_SetLogo)) return true;

    MSG_GP_R_SetLogo* data = (MSG_GP_R_SetLogo*)(socketMessage->object);

    switch (data->dwCommanResult) {
    case 1: {
		CommandManager::getInstance()->showGamePrompt(GetText("USERHEAD_CHANGED_OK"));

        int logoID = UserDefault::getInstance()->getIntegerForKey("TMP_LOGO_ID");
        PlatformLogic()->getInstance()->loginResult.bLogoID = logoID;
        updateHeadCallBack();
        break;
    }
    case 0: {
        CommandManager::getInstance()->showGamePrompt(GetText("USERHEAD_CHANGED_LOSE"));
        break;
    }
    }
    return false;
}

void GameUserHeadV66::open(Node* parent, Vec2 vec, int zorder, int tag) {
    CCAssert(nullptr != parent, "parent is nullptr");
    this->setPosition(vec);
    parent->addChild(this, zorder, tag);
    runAction(MoveBy::create(0.3f, Vec2(-WIN_SIZE.width, 0)));

    m_GameUser->runAction(MoveBy::create(0.3f, Vec2(-WIN_SIZE.width, 0)));
    //m_GameUser->setName("");
}


void GameUserHeadV66::close() {
    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);

    if (!isClose) return;

    PlatformLogic()->removeEventSelector(MDM_GP_SET_LOGO, ASS_GP_SET_LOGO);

    m_GameUser->runAction(MoveBy::create(0.3f, Vec2(WIN_SIZE.width, 0)));
    runAction(Sequence::create(MoveBy::create(0.3f, Vec2(WIN_SIZE.width, 0)),
    CCCallFunc::create([=]() {

        //m_GameUser->setName(GAME_SECOND_LAYER);
        //m_TopMenuDelegate->setLayerDelegate(m_GameUser);
        //UserDefault::getInstance()->setBoolForKey(TOUCH_HEAD, false);

        this->removeFromParent();
    }), nullptr));
}
