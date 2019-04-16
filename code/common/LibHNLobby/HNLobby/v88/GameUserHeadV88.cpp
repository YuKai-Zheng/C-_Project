#include "GameUserHeadV88.h"
#include "../CommonMacro.h"
#include "PlatformResV88.h"
#include "cocostudio/CocoStudio.h"
#include "Sound/HNAudioEngine.h"
#include "HNPlatform/HNPlatformLogic.h"
#include "../pattern/CommandManager.h"
#include "CommonRes.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/pattern/factory/background/BackgroundFactory.h"

const char* USER_HEAD_TITLE_V88 = "v66/module/gerenxinxi/touxiangliebiao.png";

bool GameUserHeadV88::init() {
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
    
    auto bg = ImageView::create(MODULE_BG_V88);
    bg->setPosition(WIN_SIZE/2);
    bg->setTouchEnabled(true);
    addChild(bg);
    
    auto title = Sprite::create(USER_HEAD_TITLE_V88);
    title->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height - 44));
    bg->addChild(title);

    isClose = true;

    auto userHeadLayer = CSLoader::createNode(USER_HEAD_V88);
    userHeadLayer->setPosition(bg->getContentSize()/2);
    bg->addChild(userHeadLayer);

    headList = (ListView*)userHeadLayer->getChildByName("headList");
    headList->setScrollBarAutoHideEnabled(false);

    //是否缓存好微信头像
    m_bHasWXHead = false;
    bool isWechatLogin = UserDefault::getInstance()->getBoolForKey(WX_LOGIN);
    auto fileName = FileUtils::getInstance()->getWritablePath() + WX_HEAD_IMG_NAME;
    bool localImgExist = FileUtils::getInstance()->isFileExist(fileName);
    m_bHasWXHead = isWechatLogin && localImgExist;
    
    maxValueInRow = 6;
    int length = 72;
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

    PlatformLogic()->addEventSelector(MDM_GP_SET_LOGO, ASS_GP_SET_LOGO, HN_SOCKET_CALLBACK(GameUserHeadV88::modifyOKSelector, this));

    return true;
}

Node* GameUserHeadV88::createItem(int index) {
    float fSpaceX = 2;
    
    Node* item = Node::create();
    
    if(index == 0 && m_bHasWXHead)
    {
        auto btn = Button::create("v66/module/gerenxinxi/6.png");
        btn->setTag(WX_HEAD_IMG_ID);
        btn->setShowEffect(false);
        btn->setSwallowTouches(false);
        btn->setPosition(Vec2(btn->getContentSize().width/2 + fSpaceX, btn->getContentSize().height/2));
        btn->addTouchEventListener(CC_CALLBACK_2(GameUserHeadV88::headButtonCallBack, this));

        item->addChild(btn);
        
        auto userHead = UserHead::create("v66/module/gerenxinxi/6.png");
        auto fileName = FileUtils::getInstance()->getWritablePath() + WX_HEAD_IMG_NAME;
        userHead->setClipUserHeadWithFile(fileName, USER_HEAD_CLIP_STENCIL_BG, 0.8f);
        userHead->setPosition(btn->getPosition());
        item->addChild(userHead);
        
        return item;
    }
    
    int headTag = m_bHasWXHead ? index - 1 : index;
    std::string path = StringUtils::format(PLAYER_HEAD_V88, headTag);
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
        button->addTouchEventListener(CC_CALLBACK_2(GameUserHeadV88::headButtonCallBack, this));

        bg->setPosition(button->getPosition());

        item->addChild(button);
    }

    return item;
}

void GameUserHeadV88::headButtonCallBack(Ref* pSender, Widget::TouchEventType type) {
    Button* selectedBtn = (Button*)pSender;
    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::BEGAN:
        isMovingList = false;
        break;
    case cocos2d::ui::Widget::TouchEventType::MOVED:
        isMovingList = true;
        break;
    case cocos2d::ui::Widget::TouchEventType::ENDED: {
        if (!isMovingList) {
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

bool GameUserHeadV88::modifyOKSelector(HNSocketMessage* socketMessage) {
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

void GameUserHeadV88::open(Node* parent, Vec2 vec, int zorder, int tag) {
    CCAssert(nullptr != parent, "parent is nullptr");
    this->setPosition(vec);
    parent->addChild(this, zorder, tag);
    runAction(MoveBy::create(0.3f, Vec2(-WIN_SIZE.width, 0)));

    m_GameUser->runAction(MoveBy::create(0.3f, Vec2(-WIN_SIZE.width, 0)));
    //m_GameUser->setName("");
}


void GameUserHeadV88::close() {
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
