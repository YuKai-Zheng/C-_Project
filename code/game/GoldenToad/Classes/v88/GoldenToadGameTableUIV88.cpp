#include "GoldenToadGameTableUIV88.h"
#include "GoldenToadGameLoadingV88.h"
#include "GoldenToadGameSceneFishTrace.h"
#include "GoldenToadGameFishManagerV88.h"
#include "GoldenToadGameToolbarV88.h"
#include "GoldenToadGameAudio.h"
#include <time.h>
#include "HNLobby/GamePrompt.h"
#include "GoldenToadVersion.h"
#include "GoldenToadEnums.h"
#include "GoldenToadGameAudio.h"
#include "HNLobby/pattern/CommandManager.h"
#include "HNLobby/pattern/StateManager.h"
#include "HNLobby/state/PlatformState.h"
#include "HNLobby/pattern/factory/gameSet/GameSetFactory.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "HNLobby/v66/ExtraReward.h"
#include "UI/HNShake.h"
#include "UI/HelpLayer.h"
#include "UI/ToolBarLayer.h"
#include "UI/Scanner.h"
#include <string>


using namespace Version;

namespace GoldenToad {
static const int	outTime_tag				=   999;
static const int	ShakeAction_Tag			=	100;


//static const int	ShowMessageAction_Tag	=	103;
static const int	DelaySwitchLockFish_Tag	=	104;
static const int    Settlemetn_Tag = 111;
//static const int	DelayExchangeLockButton_Tag = 105;


static const char*	BG_PATH					=	"v66/goldenToad/Scene/bg/";

/**
 *	金蟾捕鱼Node_Table_1.csb
 *	李逵捕鱼Node_Table_2.csb
 */
static const char*	TABLEUI_PATH_GOLDEN			=	"csb/Games/GoldToad_V66/Node_Table.csb";
static const char*	TABLEUI_PATH_LIKUI			=	"csb/Games/GoldToad_V66/Node_Table_LIKUI.csb";
static const char*	TABLEUI_PATH_MONKEY			=	"csb/Games/GoldToad_V66/Node_Table_MONKEY.csb";

static const char*  Score_number_path           =	"v66/goldenToad/number/score_num1.png";

static const char*  Settlemen_path_GOLDEN		=	"csb/Games/GoldToad_V66/SettlementUi.csb";
static const char*	Settlemen_path_LIKUI		=	"csb/Games/GoldToad_V66/SettlementUi_LIKUI.csb";
static const char*	Settlemen_path_MONKEY		=	"csb/Games/GoldToad_V66/SettlementUi_MONKEY.csb";

//static const char* GoldenToad_TOOL_BAR_PATH = "v66/buyu/xiala/%s";

static const FLOAT	SWITCH_SCENE_TIME		=	5.5f;

// 游戏场景初始化，从此进入正式的游戏界面
GameTableUIV88::GameTableUIV88()
    : _isAllowFire (true)
    , _isAutoFire (false)
    , _isReadyLock (false)
    , _isReadySwitchLock (false)
    , _isLockFish(false)
//    , _isShowMenu(true)//默认即刚进入游戏显示所有menu
    , _iBullet_id (0)
    , _lockFishId (0)
    , _touchPoint (Size::ZERO)
    , _currentBg (nullptr)
    , _tableUi (nullptr)
    , _autoBtn (nullptr)
    , _lockBtn (nullptr)
    , _switchLockFishID(0)
    , _switchLockSuccess (false)
    , _switchTouchEnd (true)
    , _switchFirstCheckFinish (false)
    , _switchSecondCheckFinish (false)
    , _switchingLockedFish (false)
    , _switchTimer (0)
    , _switch_tip_sprite (nullptr)
    , _isSwitchScene (false)
      //, _scoreDownBtn(nullptr)
      //, _scoreUpBtn(nullptr)
    , _toolBar(nullptr)
//    , _showHideBtn(nullptr)
//    , _canTouchWhileTeaching(false)
    , _delaySwitchLock(false)
    , m_Notice(nullptr)
    , _showNotify(false)
    , _toolbarBg(nullptr)
    , _ownOperateVerticalLine(0.0f)
    , _isOwnOnTop(true)
    , _bgMusicPath("")
    {
    memset(_players, 0x0, sizeof(_players));
    _resultInfo = new catchResultInfo();

    //捕鱼游戏为了防止手机切后台后回来时出现一大堆鱼，在这里加个控制字段
    //HNSocketThread里面读取为true时跳过MDM_GM_GAME_NOTIFY消息
    Configuration::getInstance()->setValue("bControl", Value(true));
    _audioEngine = new GoldenToadGameAudio();
//    _audioEngine = GoldenToadGameAudio::getInstance();
}

GameTableUIV88::~GameTableUIV88() {
    //移除取消自动捕鱼的监听事件
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("UnlockAutoFire");
    ////////
    m_Notice = nullptr;

//    this->unschedule(schedule_selector(GameTableUIV88::autoRemoveNianShouButton));

    this->unschedule(schedule_selector(GameTableUIV88::scheduleUpdatePlayEffectTag));

    unscheduleAllCallbacks();

    HN_SAFE_DELETE(_tableLogic);
    HN_SAFE_DELETE(_fishManager);
    HN_SAFE_DELETE(_resultInfo);
    HN_SAFE_DELETE(_fishTrace);
    HN_SAFE_DELETE(_audioEngine);

    //捕鱼游戏为了防止手机切后台后回来时出现一大堆鱼，在这里加个控制字段
    //HNSocketThread里面读取为true时跳过MDM_GM_GAME_NOTIFY消息
    //退出捕鱼游戏时设置回来，防止其他游戏也被跳过MDM_GM_GAME_NOTIFY消息
    Configuration::getInstance()->setValue("bControl", Value(false));

}

void GameTableUIV88::onEnter() {
    HNGameUIBase::onEnter();
    
    auto version = GoldenToadVersion::getGameVersionName();
    if (version == Enums::GAME_VERSION_MONKEY_FISH) {
        _bgMusicPath = GoldenToadGameAudio::playBackgroundMusic(2);
    }
    else
    {
        _bgMusicPath = GoldenToadGameAudio::playBackgroundMusic();
    }
    
    schedule(schedule_selector(GameTableUIV88::scheduleUpdatePlayEffectTag), 10.0f);
    //releaseUnusedRes();
    
}

void GameTableUIV88::onExitTransitionDidStart() {
    HNGameUIBase::onExitTransitionDidStart();
    this->unschedule(schedule_selector(GameTableUIV88::scheduleUpdatePlayEffectTag));
    if (_audioEngine) {
        _audioEngine->stopBackgroundMusic();
    }
    ArmatureDataManager::getInstance()->removeArmatureFileInfo("v66/goldenToad/fishAnimation/bigFishAnimation.ExportJson");
}

void GameTableUIV88::onExit() {
    HNGameUIBase::onExit();
}

void GameTableUIV88::cleanup() {
    HNGameUIBase::cleanup();
}

    void GameTableUIV88::onHandleNewMessage(int iType, const std::string &message, int iCount)
    {
        //全服公告
        if (iType == 0)
        {
            showNotice(message, 100, iType);
        }
        else
        {
            showNotice(message, iCount, iType);
        }
    }
    
GameTableUIV88* GameTableUIV88::create(BYTE bDeskIndex, bool autoCreate) {
    GameTableUIV88 *tableUI = new GameTableUIV88();
    if (tableUI && tableUI->init(bDeskIndex, autoCreate)) {
        tableUI->autorelease();
        return tableUI;
    }
    CC_SAFE_DELETE(tableUI);
    return nullptr;
}

bool GameTableUIV88::init(BYTE bDeskIndex, bool autoCreate) {
    if (!HNLayer::init()) return false;

    _deskIndex		= bDeskIndex;
    _bAutoCreate	= autoCreate;

    //异步加载资源
    auto loadingLayer = GoldenToadGameLoadingV88::create(true);
    addChild(loadingLayer,1000);
    loadingLayer->setName("LOADING_LAYER");
    loadingLayer->onCloseCallBack = [&]() {
        initGameUI();
    };
    loadingLayer->loadRes();
    //创建切换后台取消自动捕鱼监听事件
    auto listener = EventListenerCustom::create("UnlockAutoFire", [this](EventCustom *event) {
        if (_isAutoFire) {
            privateControlAutoFire(false);
        }
    });
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
    //
    return true;
}

//	游戏场景进入主入口
bool GameTableUIV88::initGameUI() {
    //监听物理按键
    enableKeyboard();

    //Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGBA8888);

    //打开点击
    auto disptch = Director::getInstance()->getEventDispatcher();
    auto myListener = EventListenerTouchOneByOne::create();
    myListener->onTouchBegan = CC_CALLBACK_2(GameTableUIV88::onTouchBegan,this);
    myListener->onTouchMoved = CC_CALLBACK_2(GameTableUIV88::onTouchMoved,this);
    myListener->onTouchEnded = CC_CALLBACK_2(GameTableUIV88::onTouchEnded,this);
    disptch->addEventListenerWithSceneGraphPriority(myListener, this);

    //背景
    std::string bgName(BG_PATH);
    bgName.append("bg1.png");
    _currentBg = ImageView::create(bgName);
    float scaleY = WIN_SIZE.height / _currentBg->getContentSize().height;
    _currentBg->setPosition(WIN_SIZE / 2);
    _currentBg->setScaleY(scaleY);
    addChild(_currentBg);

    /**
     *	tableUI加载
     *	因为仅《李逵劈鱼》目前需求是8个座位，要使用不同的UI，其他版本不需要，所以这样处理
     *	by HBC
     */
    //暫時玩法中均為6人遊戲，不需要做其餘操作  by HBC
    Node* node;
    switch (GoldenToadVersion::getGameVersionName()) {
    case Enums::GAME_VERSION_TOAD_FISH:
        node = CSLoader::createNode(TABLEUI_PATH_GOLDEN);
        break;
    case Enums::GAME_VERSION_LIKUI_FISH:
        node = CSLoader::createNode(TABLEUI_PATH_LIKUI);
        break;
    case Enums::GAME_VERSION_MONKEY_FISH:
        node = CSLoader::createNode(TABLEUI_PATH_MONKEY);
        break;
    default:
        node = CSLoader::createNode(TABLEUI_PATH_GOLDEN);
        break;
    }
    addChild(node, PLAYER_ZORDER);

    _tableUi = dynamic_cast<Layout*>(node->getChildByName("Panel_Table"));//动态解析成 Layout 类型的文件
    _tableUi->setScaleY(scaleY);
    _tableUi->setPosition(Vec2(WIN_SIZE / 2));

    //适配
    Vector<Node*> children = _tableUi->getChildren();
    float scY = 720 / WIN_SIZE.height;

    for (auto child : children) {
        if (WIN_SIZE.width / WIN_SIZE.height <= 1.7f) {
            child->setScaleY(scY);
        }
    }

    //初始化鱼管理类
    _fishManager = new GameFishManagerV88(this);

    //锁定成功回调
    _fishManager->onLockFishCallBack = [this](INT fishId) {
        _isReadyLock = false;
        _lockFishId = fishId;
        //设置炮台锁定鱼
        _players[_tableLogic->_MYSEATNO]->setIsLockFish(true, fishId);

        //去除锁定提示
        auto lockTip = dynamic_cast<ImageView*>(_tableUi->getChildByName("Image_LockTip"));
        lockTip->setVisible(false);
    };

    //解锁成功回调
    _fishManager->unLockFishCallBack = [this](int fishId) {
        //如果解锁的鱼是自己锁定的鱼，则自己开炮不再锁定
        if (_lockFishId == fishId) {
            _lockFishId = INVALID_FISHID;
        }
        for (int i = 0; i < PLAY_COUNT; i++) {
            _players[i]->setIsLockFish(false, fishId);
        }
    };

    _fishTrace	= new SceneFishTrace();

    //初始化玩家炮台
    for (int i = 0; i < PLAY_COUNT; i++) {
        auto player = GamePlayerV88::createGamePlayer(i, _tableUi, _fishManager);
        player->setBatteryVisible(false);//玩家加炮、减炮逻辑统一更改到菜单栏，此处无需做额外操作
        _players[i] = player;
        addChild(player);
    }

    auto newThread = new std::thread(&GameTableUIV88::asyncBuildData, this);
    newThread->detach();

    scheduleOnce(schedule_selector(GameTableUIV88::delayedLoadAction), 1.0f);
    //初始化工具栏
    initToolBarData();
    createToolBarUI();

    //初始化聊天功能页面
    showOrHideGameChat(false);

    _tableLogic = new GameTableLogic(this, _deskIndex, _bAutoCreate);
    _tableLogic->enterGame();

    //force update players' position and toolbar position  -- by HBC
//    updatePlayerPosition(_isShowMenu);
    //privateShowToolBarTips(_isShowMenu);

    _showNotify = true;

	auto scanner = Scanner::create();
	this->addChild(scanner, 100000);
	scanner->starPlayerEffect();
	scanner->setPosition(60, 60);


    return true;
}

void GameTableUIV88::createToolBarUI() {
    int zOrder = 700;
    auto toolbar = ToolBarLayer::create();
    if (toolbar) {
        toolbar->addImageAndCallBack(TOOL_BAR_EXIT, [=]() {
            //开始下次倒计时
            openExitWindow();
        });
        toolbar->addImageAndCallBack(TOOL_BAR_SETTING, [=]() {
            IGameSet* gameSet = GameSetFactory::createWithVersion();
            gameSet->setBgPath(_bgMusicPath);
            gameSet->open(this, Vec2(0, 0), zOrder+5, 0);
        });
        toolbar->addImageAndCallBack(TOOL_BAR_HELP, [=]() {
            //开始下次倒计时
//            auto seq = Sequence::create(DelayTime::create(60.f), CallFunc::create([this]() {
//                gameOutTime(true);
//            }), nullptr);
//            seq->setTag(outTime_tag);
//            this->runAction(seq);
//            gameOutTime(false);
//            this->stopActionByTag(outTime_tag);
            std::string helpPath = "";
            switch (GoldenToadVersion::getGameVersionName()) {
                case Enums::GAME_VERSION_TOAD_FISH:
                    helpPath = "v66/common/help/jinchan.png";
                    break;
                case Enums::GAME_VERSION_LIKUI_FISH:
                    helpPath = "v66/common/help/likui.png";
                    break;
                case Enums::GAME_VERSION_MONKEY_FISH:
                    helpPath = "v66/common/help/danaotiangong.png";
                    break;
                default:
                    helpPath = "";
                    break;
            }
            auto helpLayer = HN::HelpLayer::create();
            helpLayer->setContentOnView(helpPath);
            helpLayer->setName("helpLayer");
            helpLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            helpLayer->setOrignalPosition();
            helpLayer->moveSelfInWithAction();
            addChild(helpLayer, zOrder+5);
        });
        toolbar->create(this, Vec2(10, WIN_SIZE.height), zOrder, 100);
    }
}

void GameTableUIV88::asyncBuildData() {
    //生成鱼潮路径坐标
    _fishTrace->BuildSceneKind1Trace(WIN_SIZE.width, WIN_SIZE.height);
    _fishTrace->BuildSceneKind2Trace(WIN_SIZE.width, WIN_SIZE.height);
    _fishTrace->BuildSceneKind3Trace(WIN_SIZE.width, WIN_SIZE.height);
    _fishTrace->BuildSceneKind4Trace(WIN_SIZE.width, WIN_SIZE.height);
    _fishTrace->BuildSceneKind5Trace(WIN_SIZE.width, WIN_SIZE.height);
}

void GameTableUIV88::delayedLoadAction(float dt) {
    //水光动画
    char name[24];
    auto waterAnimation = Animation::create();
    waterAnimation->setDelayPerUnit(0.1f);
    waterAnimation->setLoops(-1);
    for(int i = 1; i <= 16; i++) {
        sprintf(name, "water%d.png", i);
        waterAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(name));
    }
    auto  animate = Animate::create(waterAnimation);
    auto water = Sprite::createWithSpriteFrameName(name);
    float X = WIN_SIZE.width / water->getContentSize().width;
    float Y = WIN_SIZE.height / water->getContentSize().height;
    water->setScale(X, Y);
    water->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2));
    water->setOpacity(0);
    water->runAction(Sequence::create(FadeIn::create(0.5f), animate, nullptr));
    addChild(water, WATER_ZORDER);

    //气泡粒子效果
    ParticleSystem *meteor = ParticleSystemQuad::create("v66/goldenToad/particle/qipao.plist");
    meteor->setPosition(WIN_SIZE / 2);
    meteor->setAutoRemoveOnFinish(true);
    addChild(meteor, 1);
}

void GameTableUIV88::initToolBarData() {
    //初始化切换锁定鱼提示标识
    _switch_tip_sprite = dynamic_cast<Sprite*>(_tableUi->getChildByName("Sprite_Switch_Tip"));
    _switch_tip_sprite->setVisible(false);

    auto toolBarBG = dynamic_cast<Layout*>(ui::Helper::seekWidgetByName(_tableUi, "Panel_ToolBar"));
    //toolBarBG->setPositionX(WIN_SIZE.width);
    _toolBar = GoldenToadToolbarV88::create(toolBarBG);
    addChild(_toolBar);
    
//    _showHideBtn = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_tableUi, "Button_Show_Hide"));
//    _toolBar->_onShowHideMenu = [this](Button* showHideBtn) {
//        updateToolBarStationAndPlayerSPosition(false);
//    };

//    //上分、下分
//    _toolBar->_onScoreUp = [this](Button * scoreUpBtn) {
//        _tableLogic->sendScoreUp();
//        timerAutoHideToolBarStation();
//    };
//    _toolBar->_onScoreDown = [this](Button* scoreDownBtn) {
//        _tableLogic->sendScoreDown();
//        timerAutoHideToolBarStation();
//    };
//    _toolBar->_onAddScore = [this](Button* addScoreBtn) {
//        _players[_tableLogic->_MYSEATNO]->addScoreForBullet();
//        timerAutoHideToolBarStation();
//    };
//    _toolBar->_onCutScore = [this](Button* cutScoreBtn) {
//        _players[_tableLogic->_MYSEATNO]->cutScoreForBullet();
//        timerAutoHideToolBarStation();
//    };
    _toolBar->_onAutoFire = [this](Button* autoBtn) {
        _autoBtn = autoBtn;
        if (!_isAutoFire) {
            privateControlAutoFire(true);
        } else {
            privateControlAutoFire(false);
//            timerAutoHideToolBarStation();
        }
    };
//    _toolBar->_onSetting = [this]() {
////        gameSettingWindow();
//        timerAutoHideToolBarStation();
//    };
//    _toolBar->_onExchangeScore = [this]() {
//        _tableLogic->setOldMoney(_tableLogic->getMeMoney());
//        _tableLogic->setOldScore(_players[_tableLogic->_MYSEATNO]->getUserMoney());
//        _toolBar->_exchangeScoreUi = GoldenToadGameExchangeScore::getInstance(this, _tableLogic);
//
//        reorderChild(_toolBar->_exchangeScoreUi, MAX_ZORDER);
//        _toolBar->_exchangeScoreUi->setMinMoneyEdge(0);
//        _toolBar->_exchangeScoreUi->setCurrentMoneyValue(0);
//        _toolBar->_exchangeScoreUi->setMaxMoneyEdge(_tableLogic->getMeMoney());
//        timerAutoHideToolBarStation();//如果没有长按上分按钮使用一定步长更改上分数，则自动隐藏菜单栏
//        _toolBar->_onLongPressScoreUpBtn = [this](bool isLongPress) {
//            if (isLongPress) {
//                //不允许自动隐藏
//                this->stopActionByTag(AutoHideAction_Tag);
//            } else {
//                //如果此时不是长按状态，则启动 5s 自动隐藏菜单栏的逻辑
//                timerAutoHideToolBarStation();
//            }
//        };
//        _toolBar->_exchangeScoreUi->_onScheduleExchangeMoney = [this](LLONG money) {
//            if (_toolBar->_onScheduleUpdateScore) {
//                _toolBar->_onScheduleUpdateScore(money);
//            }
//        };
//    };

    _toolBar->_onLock = [this](Button* lockBtn, bool bLock, bool bNormal) {
        if (_delaySwitchLock) {
            //如果之前的动作没有完成，则此时对于按钮的操作无效。完成之后才可以切换解锁、锁定。
            return;
        }
        _delaySwitchLock = true;
        this->stopActionByTag(DelaySwitchLockFish_Tag);
        auto seq = Sequence::create(DelayTime::create(0.3f), CallFunc::create([this]() {
            _delaySwitchLock = false;
        }), nullptr);
        seq->setTag(DelaySwitchLockFish_Tag);
        this->runAction(seq);
        //每次单击“锁定/解锁”按钮，都会将进行状态的反转。在“锁定”状态，可以通过长按鱼进行锁定的切换操作
        //_isReadyLock = !_isReadyLock;
        _lockBtn = lockBtn;
        _isLockFish = !_isLockFish;
        //锁定状态时锁定按钮显示为“解锁”，未锁定时显示“锁定”。
        privateControlLockFishFunc(_isLockFish);
    };
    _toolBar->_onChangeLockFish = [this](Button* changeLockFishButton) {
        if (_isLockFish) {
            if (_lockFishId != INVALID_FISHID) {
                _fishManager->changeLockBigFish(_lockFishId);
            } else {
                _isLockFish = true;
            }
        }
    };
//    _toolBar->_onScheduleUpdateScore = [this](LLONG money) {
//        LLONG tmpScore = _tableLogic->getBaseScoreValue() + money * _tableLogic->getRoomRate();
//        dealSetUserMoney(_tableLogic->_MYSEATNO, (tmpScore));
//    };

}

void GameTableUIV88::privateControlAutoFire(bool start) {
    if (!_isAllowFire || _isSwitchScene) {
        //如果不允许射击，则所有状态恢复默认
        start = false;
    }
    _isAutoFire = start;
    if (_autoBtn != nullptr) {
        _autoBtn->setBright(!_isAutoFire);
        auto imgTip = dynamic_cast<ImageView*>(_autoBtn->getChildByName("Image_1"));
        if (imgTip) {
            imgTip->setVisible(_isAutoFire);
        }
        
        auto node_effect = dynamic_cast<Node*>(_autoBtn->getChildByName("Node_effect"));
        if (node_effect) {
            
            auto node_action = node_effect->getChildByName("NODE_YELLOE_ROTATE");
            if (node_action == nullptr) {
                node_action = CSLoader::createNode("csb/Games/GoldToad_V66/Node_lock.csb");
                node_action->setPosition(Vec2(0,0));
                node_action->setName("NODE_YELLOE_ROTATE");
                node_effect->addChild(node_action);
                
                auto action = CSLoader::createTimeline("csb/Games/GoldToad_V66/Node_lock.csb");
                if (action) {
                    node_action->runAction(action);
                    action->gotoFrameAndPlay(0);
                }
            }
            
            node_action->setVisible(_isAutoFire);
        }
    }
    if (_isAutoFire) {
        schedule(schedule_selector(GameTableUIV88::setAtuoFire), 0.25f);
    } else {
        unschedule(schedule_selector(GameTableUIV88::setAtuoFire));
    }
}

/**
 *	锁定、解锁逻辑控制封装
 *		将锁定、解锁相关控制统一封装
 */
void GameTableUIV88::publicControlLockedFish(bool lock) {
    if (!_isAllowFire || _isSwitchScene) {
        //不允许射击时强制解除锁定
        lock = false;//显示锁定
    }
    _isLockFish = lock;
    if (!_isLockFish) {
        //设置炮台锁定鱼
        _players[_tableLogic->_MYSEATNO]->setIsLockFish(false, _lockFishId);
        _lockFishId = INVALID_FISHID;
    }
    if (_lockBtn == nullptr) {
        return;
    }
    _lockBtn->setBright(!_isLockFish);
    _toolBar->controlChangeButtonAnimation(_isLockFish);
}

void GameTableUIV88::privateControlLockFishFunc(bool lock) {
    if (!_isAllowFire || _isSwitchScene) {
        //不允许射击时以及切换场景时强制解除锁定
        _isLockFish = false;//显示锁定
    }
    if (_isLockFish) {
        //处理锁定鱼的逻辑
        //自动锁定鱼，默认自动锁定最大倍率的鱼，若期间手动切换、鱼超出屏幕、鱼被捕获，均切换锁定其他的鱼
        if (!_fishManager->autoLockBigFish()) {
            //没有可以锁定的鱼，提示没有可以锁定的鱼
            nofityLockFailUI();
            //_fishManager->lockBigFishFail();
            //dealUnlockFish();
            _players[_tableLogic->_MYSEATNO]->setIsLockFish(false, _lockFishId);
            _lockFishId = INVALID_FISHID;
            _players[_tableLogic->_MYSEATNO]->setLockFishId(_lockFishId);
            _isLockFish = false;
        }
    } else {
        //解锁 解锁 解锁
        //设置炮台锁定鱼
        _players[_tableLogic->_MYSEATNO]->setIsLockFish(false, _lockFishId);
        _lockFishId = INVALID_FISHID;
        _players[_tableLogic->_MYSEATNO]->setLockFishId(_lockFishId);
        _isLockFish = false;
        //关闭锁定
        _fishManager->manualUunlockBigFish();
    }
    if (_lockBtn == nullptr) {
        return;
    }
    _lockBtn->setBright(!_isLockFish);
    auto imgTip = dynamic_cast<ImageView*>(_lockBtn->getChildByName("Image_1"));
    if (imgTip) {
        imgTip->setVisible(_isLockFish);
    }
    
    auto node_effect = dynamic_cast<Node*>(_lockBtn->getChildByName("Node_effect"));
    if (node_effect) {
        
        auto node_action = node_effect->getChildByName("NODE_YELLOE_ROTATE");
        if (node_action == nullptr) {
            node_action = CSLoader::createNode("csb/Games/GoldToad_V66/Node_lock.csb");
            node_action->setPosition(Vec2(0,0));
            node_action->setName("NODE_YELLOE_ROTATE");
            node_effect->addChild(node_action);
            
            auto action = CSLoader::createTimeline("csb/Games/GoldToad_V66/Node_lock.csb");
            if (action) {
                node_action->runAction(action);
                action->gotoFrameAndPlay(0);
            }
        }
        
        node_action->setVisible(_isLockFish);
    }
    _toolBar->controlChangeButtonAnimation(_isLockFish);
}

void GameTableUIV88::privateResetSwitchFishID() {
    _switchLockFishID = INVALID_FISHID;
}

///**
// *	独立封装的判断是否显示上分提示的方法，仅在积分不足单次炮弹时才出现上分提示
// */
//void GameTableUIV88::privateShowToolBarTips(bool isShow) {
//    if (isShow) {
//        INT multiple = 0;
//        if (_players[_tableLogic->_MYSEATNO]->getMinMultiple() <= 0) {
//            multiple = 1000;
//        } else {
//            multiple = _players[_tableLogic->_MYSEATNO]->getFirepower();
//        }
//        //显示的时候判断是否需要显示
//        if (_players[_tableLogic->_MYSEATNO]->getCurrentMoney() >= multiple) {
//            return;
//        }
//        _toolBar->showScoreUpTips(isShow);
//    } else {
//        _toolBar->showScoreUpTips(isShow);
//    }
//}

/**
 *	界面添加玩家，包括炮台及其周围相关信息展示
 *  视图位置
 *	by HBC
 */
void GameTableUIV88::addUser(BYTE seatNo, bool bMe) {
    if (!isValidSeat(seatNo))	return;

    BYTE logicSeatNo = _tableLogic->viewToLogicSeatNo(seatNo);
    UserInfoStruct* userInfo = _tableLogic->getUserBySeatNo(logicSeatNo);
//    if (userInfo)
//    {
//    int ii = _tableLogic->getBaseBulletMultiple();
    if (bMe) {
        _toolBar->controlTouchAction(true);
        _ownOperateVerticalLine = _players[logicSeatNo]->getBatterySize().height * 0.5f;
        _isOwnOnTop = seatNo <= 1 ? true : false;//0、1位置表明在上面
    }
    _players[logicSeatNo]->setIsMe(bMe);
    _players[logicSeatNo]->showMyselfLable();
    _players[logicSeatNo]->setBatteryVisible(true);
//        _players[seatNo]->setUserID(_tableLogic->getUserId(seatNo));
//    _players[seatNo]->setCannonType(BULLET_KIND_1_NORMAL);
//    _players[seatNo]->setCannonNum(_tableLogic->getBaseBulletMultiple());
    if (userInfo) {
        _players[logicSeatNo]->setUserMoney(userInfo->i64Money);
        _players[logicSeatNo]->setPlayerName(userInfo->nickName);
    }
    _players[logicSeatNo]->setGameTable(this);
//    }


//    //如果是自己，则将炮台移动到屏幕中间
//    if (bMe) {
//        _players[seatNo]->resetMyselfBatteryIntoMiddlePosition();
//        //先将自己的位置放置到屏幕中间，然后再将其移回到自己的位置上
//        _players[seatNo]->moveMyselfBatteryRealPosition();
//
//        //教学期间禁止触摸事件的响应
//        this->stopActionByTag(TeachingAction_Tag);
//        auto seq = Sequence::create(DelayTime::create(2.7f), CallFunc::create([=]() {
//            _canTouchWhileTeaching = true;
//            //1、显示菜单栏上分提示
////            privateShowToolBarTips(true);
//            //2、允许菜单栏的按钮响应
//            _toolBar->controlTouchAction(true);
//        }), nullptr);
//        seq->setTag(TeachingAction_Tag);
//        this->runAction(seq);
//
//    }

}

void GameTableUIV88::removeUser(BYTE seatNo, bool bMe) {
    if (!isValidSeat(seatNo))	return;
    _players[seatNo]->setBatteryVisible(false);
    _players[seatNo]->setIsEnergyCannon(false);
    _players[seatNo]->setIsLockFish(false, /*INVALID_FISHID*/_players[seatNo]->getLockFishId());
    _players[seatNo]->setCannonNum(_tableLogic->getBaseBulletMultiple());
    _players[seatNo]->setPlayerName("");
    _players[seatNo]->setUserMoney(0);
}
/**
 *	设置玩家积分
 */
void GameTableUIV88::dealSetUserMoney(BYTE seatNo, LLONG value) {
    if (!isValidSeat(seatNo))	return;
    _players[seatNo]->setUserMoney(value);
}
/**
 *	设置玩家金币
 */
void GameTableUIV88::dealSetUserCoin(BYTE seatNo, LLONG value) {
    if (!isValidSeat(seatNo))	return;
    if (_tableLogic->getMySeatNo() == seatNo) {
        _tableLogic->setMeMoney(value);//将玩家自己的金币数暂存
    }
    _players[seatNo]->setUserMoney(value);
}

//  value 值的来源，该值为进入游戏后玩家默认鱼炮消耗值
//	该值来源于服务端
//  by HBC
void GameTableUIV88::dealSetUserCannonNum(BYTE seatNo, INT value) {
    if (!isValidSeat(seatNo))	return;
    _players[seatNo]->setCannonNum(value);
}

void GameTableUIV88::dealSetGameConfig(CMD_S_GameConfig* gConfig) {
    //获取房间倍率并保存
    _tableLogic->setBaseBulletMultiple(gConfig->baseMul);

    _fishManager->setConfig(gConfig);

    for (BYTE i = 0; i < PLAY_COUNT; i++) {
        _players[i]->setConfig(gConfig);
    }
}

//进游戏时游戏的背景图，为了跟pc同步
void GameTableUIV88::dealSetSceneStyle(SceneStyle Scene) {
    char name[12];
    std::string bgName(BG_PATH);
    switch (GoldenToadVersion::getGameVersionName()) {
    case Enums::GAME_VERSION_TOAD_FISH:
        sprintf(name, "bg%d.png", Scene + 1);
        break;
    case Enums::GAME_VERSION_LIKUI_FISH:
        sprintf(name, "bg%d.png", Scene + 1 + 2);
        break;
    default:
        sprintf(name, "bg%d.png", Scene + 1);
        break;
    }
    //sprintf(name, "bg%d.png", Scene + 1);
    bgName.append(name);
    _currentBg->loadTexture(bgName);
    setVisible(true);
    playSeaweedAnimation(Scene);
    
    auto loadingLayer = this->getChildByName("LOADING_LAYER");
    if (loadingLayer) {
        loadingLayer->removeFromParent();
    }
    if (!_isAutoFire && _isAllowFire) {
        //开始超时退出倒计时
        auto seq = Sequence::create(DelayTime::create(60.f), CallFunc::create([this]() {
            gameOutTime(true);
        }), nullptr);

        seq->setTag(outTime_tag);
        this->runAction(seq);
    }
}

void GameTableUIV88::dealLockFishTimeOut() {
    _fishManager->setAllFishIsPause(false);
}

void GameTableUIV88::dealBulletTimeOut(BYTE seatNo) {
    _players[seatNo]->setIsEnergyCannon(false);
}

//开炮
void GameTableUIV88::dealGameFire(CMD_S_UserFire* pFire, bool bMeFire) {
    BYTE seatNo = pFire->chair_id;
    if (seatNo >= PLAY_COUNT || seatNo < 0) return;
    if (!_players[seatNo]->batteryIsVisible()) return;

    //转换玩家开炮角度
    float degree = 0.0f;
    Fish* LockFish = nullptr;
    if (pFire->lock_fishid > 0) {
        LockFish = dynamic_cast<Fish*>(_fishManager->getFishById(pFire->lock_fishid));
    }

    //如果有锁定，则开炮角度为炮口和目标鱼的角度，否则为服务器发来的玩家开炮角度
    if (pFire->lock_fishid != INVALID_FISHID && nullptr != LockFish) {
        //此次开炮有锁定，则需要设置锁定信息
        _players[seatNo]->setIsLockFish(true, pFire->lock_fishid);
        //转换炮口到目标鱼之间的开炮角度
        Vec2 cononpoint = _players[seatNo]->getCannonPoint();
        float angle = MathAide::CalcAngle(LockFish->getPositionX(), LockFish->getPositionY(), cononpoint.x, cononpoint.y);
        degree = CC_RADIANS_TO_DEGREES(angle);
    } else {
        _players[seatNo]->setIsLockFish(false, /*INVALID_FISHID*/_players[seatNo]->getLockFishId());
        degree = CC_RADIANS_TO_DEGREES(pFire->angle);
    }

    //这个bMeFire是指本地开炮，防止网络延迟，自己开炮不等服务器消息返回直接显示
    if (!bMeFire) {
        //更新玩家积分
        _players[seatNo]->setUserMoney(pFire->fish_score);
    }

    if (!_players[seatNo]->getIsMe()) {
        //显示玩家开炮
        _players[seatNo]->gameFire(degree, seatNo, pFire->bullet_id, pFire->bullet_kind, pFire->bullet_mulriple, pFire->lock_fishid, pFire->bIsRobot);
    } else {
        //如果开炮的是自己，则只显示本地开炮消息
        if (bMeFire) {
            _players[seatNo]->gameFire(degree, seatNo, pFire->bullet_id, pFire->bullet_kind, pFire->bullet_mulriple, pFire->lock_fishid);
        }
    }
}

void GameTableUIV88::dealGameFireWhenLessScore(CMD_S_UserFire* pFire, bool bMeFire) {
    BYTE seatNo = pFire->chair_id;
    if (seatNo >= PLAY_COUNT || seatNo < 0) return;
    if (!_players[seatNo]->batteryIsVisible()) return;

    //转换玩家开炮角度
    float degree = 0.0f;
    Fish* LockFish = nullptr;
    if (pFire->lock_fishid > 0) {
        LockFish = dynamic_cast<Fish*>(_fishManager->getFishById(pFire->lock_fishid));
    }

    //如果有锁定，则开炮角度为炮口和目标鱼的角度，否则为服务器发来的玩家开炮角度
    if (pFire->lock_fishid != INVALID_FISHID && nullptr != LockFish) {
        //此次开炮有锁定，则需要设置锁定信息
        _players[seatNo]->setIsLockFish(true, pFire->lock_fishid);
        //转换炮口到目标鱼之间的开炮角度
        Vec2 cononpoint = _players[seatNo]->getCannonPoint();
        float angle = MathAide::CalcAngle(LockFish->getPositionX(), LockFish->getPositionY(), cononpoint.x, cononpoint.y);
        degree = CC_RADIANS_TO_DEGREES(angle);
    } else {
        _players[seatNo]->setIsLockFish(false, /*INVALID_FISHID*/_players[seatNo]->getLockFishId());
        degree = CC_RADIANS_TO_DEGREES(pFire->angle);
    }
}

//添加普通鱼信息（此信息包含的鱼的数目不定）
void GameTableUIV88::dealAddFishInfo(void* object, WORD objectSize) {
    CCASSERT(objectSize % sizeof(CMD_S_FishTrace) == 0, "CMD_S_FishTrace size is error");
    if (objectSize % sizeof(CMD_S_FishTrace) != 0) return;

    BYTE count = objectSize / sizeof(CMD_S_FishTrace);
    CMD_S_FishTrace* pTrace = (CMD_S_FishTrace*)(object);

    for (BYTE i = 0; i < count; i++) {
        std::vector<Vec2> initPos;
        for (BYTE j = 0;  j < pTrace->init_count; j++) {
            //因为pc端y轴坐标为上面是0，跟手游y轴反向，所以y轴取反
            auto pos = Vec2(pTrace->init_pos[j].x / kResolutionWidth * WIN_SIZE.width,
                            (1.0f - pTrace->init_pos[j].y / kResolutionHeight) * WIN_SIZE.height);
            initPos.push_back(pos);
        }
        std::vector<FPointAngle> fishTraceList;
        _fishManager->addFish(pTrace->fish_kind, pTrace->fish_id, &initPos, fishTraceList, pTrace->fish_kind_ex, pTrace->trace_type);

        ++pTrace;
    }
}

//打死带盘鱼王会在此位置出来三圈鱼群
void GameTableUIV88::dealAddGroupFishInfo(CMD_S_GroupFishTrace* pFish) {
    if (0 == pFish->byIndex) {
        if (_fpCircleFishCenter[0].x == 0 && _fpCircleFishCenter[0].y == 0) {
            return;
        }
    } else {
        if (_fpCircleFishCenter[1].x == 0 && _fpCircleFishCenter[1].y == 0) {
            return;
        }
    }

    //根据保存的带盘鱼王死亡位置设置出来2圈鱼的行动路径
    for (int i = 0; i < pFish->fish_count; ++i) {
        Vec2 init_pos[5];
        int center_x ;
        int center_y ;
        if (0 == pFish->byIndex) {
            center_x = _fpCircleFishCenter[0].x;
            center_y = _fpCircleFishCenter[0].y;
        } else {
            center_x = _fpCircleFishCenter[1].x;
            center_y = _fpCircleFishCenter[1].y;
        }

        float fAngle = (float)((M_PI*2) / (pFish->fish_count));

        init_pos[0].x = static_cast<float>(center_x);
        init_pos[0].y = static_cast<float>(center_y);
        init_pos[1].x = static_cast<float>(center_x + sin((float)(i*fAngle))*500);
        init_pos[1].y = static_cast<float>(center_y + cos((float)(i*fAngle))*500);
        init_pos[2].x = static_cast<float>(center_x + sin((float)(i*fAngle))*1440);
        init_pos[2].y = static_cast<float>(center_y + cos((float)(i*fAngle))*900);

        if (true) {
            init_pos[1].x = init_pos[2].x;
            init_pos[1].y = init_pos[2].y;
        }

        std::vector<Vec2> initPos;
        for (BYTE j = 0;  j < 3; j++) {
            auto pos = Vec2(init_pos[j].x, init_pos[j].y);
            initPos.push_back(pos);
        }
        std::vector<FPointAngle> fishTraceList;
        _fishManager->addFish(pFish->fish_kind[i], pFish->fish_id[i], &initPos, fishTraceList, pFish->fish_red_kind, TRACE_LINEAR, true);
    }
}

void GameTableUIV88::dealCatchFish(CMD_S_CatchFish* pFish) {
    Fish* fish = _fishManager->getFishById(pFish->fish_id);
    ///**
    // *	因为 Fish 类只是鱼的模型，其创建的时候仅通过鱼ID创建，未能将相关倍数传入，所以在此时将其值出入，暂时解决问题，但是这种封装并不好
    // *	by HBC
    // */


    if (nullptr == fish) return;

    fish->_fish_multiple = pFish->fish_multiple;//tmp methods

    if (pFish->bullet_ion) {
        _players[pFish->chair_id]->setIsEnergyCannon(true);
    }

    if (_tableLogic->_MYSEATNO == pFish->chair_id) {
        //自己捕到的这种鱼的类型加1（为了结算展示）
        CCASSERT( pFish->fish_kind < FISH_KIND_COUNT , "访问数组越界");
        if (pFish->fish_kind < FISH_KIND_COUNT) {
            _resultInfo->numberOfKindFIsh[pFish->fish_kind]++;
            _resultInfo->userGoldMoney += pFish->fish_score;
        }
    }

    //抓到定海神针让所有鱼停止游动
    if (FISH_KIND_19 == pFish->fish_kind) {
        _fishManager->setAllFishIsPause(true);
    }

    if (fish != nullptr) {
        GoldenToadGameAudio::playCatchEffect();
        //播放抓到鱼金币翻滚和显示奖金动画
        playAwardFishAnimation(pFish->chair_id, pFish->fish_score, pFish->i64UserFishScore_, fish);
        //显示鱼死亡动画并移除
        _fishManager->removeOrSetFishDieById(pFish->fish_id, true);
    }
}

/**
 *	成功捕捉到鱼后奖励特效
 *	by HBC
 */
void GameTableUIV88::playAwardFishAnimation(BYTE bSeatNo, LLONG money, LLONG userMoney, Fish* fish) {
    //抓到的鱼类型大于16则要做震屏特效
    if (fish->getFishKind() >= FISH_KIND_19) {
        
        bool m_shakeEffect = true;
        if (GoldenToadVersion::getGameVersionName() == Enums::GAME_VERSION_MONKEY_FISH) {
            if (fish->getFishKind() >= FISH_KIND_33 && fish->getFishKind() <= FISH_KIND_38) {
                m_shakeEffect = false;
            }
        }
        
        if (m_shakeEffect) {
            GoldenToadGameAudio::playGoldEffect();
            this->stopActionByTag(ShakeAction_Tag);
            this->setPosition(Size::ZERO);
            auto shake = Shake::create(1.0f, 30, 30);
            shake->setTag(ShakeAction_Tag);
            this->runAction(shake);
        }
       
    }

    //以下种类的鱼要显示bingo转盘
    if ((fish->getFishKind() >= FISH_KIND_18 && fish->getFishKind() <= /*FISH_KIND_21*/FISH_KIND_20) ||
            (fish->getFishKind() >= FISH_KIND_25 && fish->getFishKind() <= FISH_KIND_30)) {
        if (money > 0) {
            _players[bSeatNo]->setBingo(money);
        }
    }

    ParticleSystem *meteor = nullptr;
    switch (fish->getFishKind()) {
    case FISH_KIND_17:
    case FISH_KIND_18:
    case FISH_KIND_20: {
        meteor = ParticleSystemQuad::create("v66/goldenToad/particle/shayu.plist");
    }
    break;
    case FISH_KIND_19: {
        meteor = ParticleSystemQuad::create("v66/goldenToad/particle/ding.plist");

        auto ding = Sprite::create("v66/goldenToad/particle/Ding_Magic.png");
        ding->setPosition(fish->getPosition());
        addChild(ding, FISH_ZORDER + 1);
        ding->runAction(Sequence::create(Blink::create(3.0f, 3), DelayTime::create(1.0f), RemoveSelf::create(true), nullptr));


        auto snowParticle = ParticleSystemQuad::create("v66/goldenToad/particle/snow.plist");
        snowParticle->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height + 50));
        snowParticle->setAutoRemoveOnFinish(true);
        snowParticle->setName("snowParticle");
        addChild(snowParticle, FISH_ZORDER + 2);
        snowParticle->runAction(Sequence::create(DelayTime::create(10.5f), FadeOut::create(0.5f), RemoveSelf::create(true), nullptr));
    }
    break;
    //case FISH_KIND_21:
    case FISH_KIND_22: {
        meteor = ParticleSystemQuad::create("v66/goldenToad/particle/jinlong.plist");
    }
    break;
    case FISH_KIND_23:
    case FISH_KIND_25:
    case FISH_KIND_26: {
        meteor = ParticleSystemQuad::create("v66/goldenToad/particle/bigwin.plist");
    }
    break;
    case FISH_KIND_27:
    case FISH_KIND_28:
    case FISH_KIND_29:
    case FISH_KIND_30: {
        meteor = ParticleSystemQuad::create("v66/goldenToad/particle/dasanyuan.plist");
    }
    break;
    default:
        break;
    }

    if (nullptr != meteor) {
        meteor->setPosition(fish->getPosition());
        meteor->setAutoRemoveOnFinish(true);
        addChild(meteor, FISH_ZORDER + 1);
    }

    char str[24];
    if (money > 0) {
        //单条鱼获得金币数量
        sprintf(str, "%lld", money);
        auto moneyText = TextAtlas::create(str, Score_number_path, 54, 62, ".");
        if (fish->getFishKind() <= FISH_KIND_5) {
            moneyText->setScale(0.6f);
        }
        moneyText->setPosition(fish->getPosition());
        this->addChild(moneyText, FISH_ZORDER + 2);

        moneyText->runAction(Sequence::create(
                                 JumpBy::create(0.3f, Vec2(this->getPosition()), 15, 1),
                                 DelayTime::create(0.5f),
                                 Spawn::create(MoveBy::create(1.0f, Vec2(0, 50)),
                                               FadeOut::create(1.0f), nullptr),
        CallFunc::create([=]() {
            //GoldenToadGameAudio::playCatchEffect();
            moneyText->removeFromParent();
        }), nullptr));
    }

    Vec2 pos = _players[bSeatNo]->getMoneyPoint();
//    auto eFKind = fish->getFishKind();
    Vec2 fish_pos = fish->getPosition();

    //需要根据鱼类型不同产生不同个数金币
    int coin_count = 0;
    if (fish->_fish_multiple <= 10) {
        coin_count = G_Fish_Coin_Count_By_Multiple[0];
    } else if (fish->_fish_multiple > 10 && fish->_fish_multiple <= 25) {
        coin_count = G_Fish_Coin_Count_By_Multiple[1];
    } else if (fish->_fish_multiple > 25 && fish->_fish_multiple <= 120) {
        coin_count = G_Fish_Coin_Count_By_Multiple[2];
    } else {
        coin_count = G_Fish_Coin_Count_By_Multiple[3];
    }

//    auto cache = SpriteFrameCache::getInstance();

    _players[bSeatNo]->setUserMoney(userMoney);
    //根据获得的个数和位置制作翻滚金币动画
    for (int i = 0; i < coin_count; ++i) {
        //设置随机位置
        int k = (rand() % 150) - 75;
        int s = (rand() % 150) - 75;

        std::string name("coin21_");
        //if (fish->getFishKind() < FISH_KIND_5)
        //{
        //	name = "coin11_";
        //}

        Animate* animate = nullptr;
        auto animation = dynamic_cast<Animation*>(AnimationCache::getInstance()->getAnimation(name));
        if (!animation) {
            animate = buildAnimation(name, 12, -1, 0.08f);
        } else {
            animate = Animate::create(animation);
        }

        auto coin = Sprite::create();
        coin->setPosition(Vec2(fish_pos.x + k, fish_pos.y + s));
        this->addChild(coin, FISH_ZORDER + 1);
        coin->runAction(Spawn::create(animate, Sequence::create(DelayTime::create(1.5f),
        MoveTo::create(0.5f, pos), CallFunc::create([=]() {
            //_players[bSeatNo]->setUserMoney(userMoney);
        }), RemoveSelf::create(), nullptr), nullptr));
    }
}

void GameTableUIV88::dealCatchSweepFish(CMD_S_CatchSweepFish* pSweep) {
    Fish* fish = _fishManager->getFishById(pSweep->fish_id);
    if (nullptr == fish) return;

    //抓到大鱼要把当前屏幕中鱼的信息反馈服务器
    CMD_C_CatchSweepFish pRet = _fishManager->catchSweepFish(pSweep->chair_id , pSweep->fish_id);
    _tableLogic->sendSweepFish(&pRet, sizeof(CMD_C_CatchSweepFish));

    //带盘子鱼王，会出现鱼阵，需要记录下坐标
    if (fish->getFishKind() == FISH_KIND_32) {
        _fishManager->getFishPositionByFishId(fish->getFishID());
        if (0 == pSweep->byIndex) {
            _fpCircleFishCenter[0].x = fish->getPositionX();
            _fpCircleFishCenter[0].y = fish->getPositionY();
        } else {
            _fpCircleFishCenter[1].x = fish->getPositionX();
            _fpCircleFishCenter[1].y = fish->getPositionY();
        }
    }
}

void GameTableUIV88::dealCatchSweepFishResult(CMD_S_CatchSweepFishResult* pResult) {
    auto fish = _fishManager->getFishById(pResult->fish_id);
    if (fish != nullptr) {
        fish->_fish_multiple = pResult->fish_multiple;
        GoldenToadGameAudio::playGoldEffect();

        if (_tableLogic->_MYSEATNO == pResult->chair_id) {
            //自己捕到的这种鱼的类型加1（为了结算展示）
            if (fish->getFishKind() < FISH_KIND_COUNT) {
                _resultInfo->numberOfKindFIsh[fish->getFishKind()]++;
                _resultInfo->userGoldMoney += pResult->fish_score_Sum;
            }
        }
        //显示大鱼死亡动画并移除（炸弹，鱼王）
        _fishManager->removeOrSetFishDieById(pResult->fish_id, true);
        //播放抓到大鱼金币翻滚和显示奖金动画
        playAwardFishAnimation(pResult->chair_id, pResult->fish_score_Sum, pResult->i64UserFishScore_, fish);

        if (pResult->fish_score_Sum > 0) {
            _players[pResult->chair_id]->setBingo(pResult->fish_score_Sum);
        }

        GoldenToadGameAudio::playBombEffect();

        ParticleSystem *meteor = nullptr;
        switch (fish->getFishKind()) {
        case FISH_KIND_24: {
            meteor = ParticleSystemQuad::create("v66/goldenToad/particle/bomb.plist");
        }
        break;
        //case FISH_KIND_31:
        //	{
        //		meteor = ParticleSystemQuad::create("goldenToad/particle/xuehua.plist");
        //	}
        //	break;
        case FISH_KIND_32: {
            meteor = ParticleSystemQuad::create("v66/goldenToad/particle/yanhua.plist");
        }
        break;
        default:
            break;
        }

        if (nullptr != meteor) {
            meteor->setPosition(fish->getPosition());
            meteor->setAutoRemoveOnFinish(true);
            addChild(meteor, FISH_ZORDER);
        }

        //抓到全屏炸弹，所有鱼死掉
        for (int i = 0; i < pResult->catch_fish_count; i++) {
            int smallFishId = pResult->catch_fish_id[i];
            auto smallFish = _fishManager->getFishById(smallFishId);
            if (smallFish == nullptr) {
                continue;
            }
            smallFish->_fish_multiple = pResult->fish_multiple;
            if (nullptr != smallFish) {
                //抓到红鱼鱼王和带盘子鱼王，辅助类型相同的鱼会死掉，播放链接闪电特效
                if (/*fish->getFishKind() == FISH_KIND_31 || */fish->getFishKind() == FISH_KIND_32) {
                    Vec2 fishPos = fish->getPosition();
                    Vec2 sFishPos = _fishManager->getFishPositionByFishId(smallFishId);

                    std::string name = "v66/goldenToad/particle/Light_Red.png";/* (fish->getFishKind() == FISH_KIND_31) ? "goldenToad/particle/Light_Blue.png" : "goldenToad/particle/Light_Red.png";*/
                    auto light = Sprite::create(name);
                    auto range = sqrt(pow(sFishPos.x - fishPos.x, 2) + pow(sFishPos.y - fishPos.y, 2));
                    float degree = (atan2(sFishPos.x - fishPos.x, sFishPos.y - fishPos.y) * 180 / 3.14f) - 90;
                    float scale = range / light->getContentSize().width;
                    light->setScaleX(0.0f);
                    light->setRotation(degree);
                    light->setAnchorPoint(Vec2(0, 0.5f));
                    light->setPosition(fishPos);

                    addChild(light, FISH_ZORDER);
                    light->runAction(Sequence::create(ScaleTo::create(1.0f, scale), DelayTime::create(0.5f),
                                                      RemoveSelf::create(true), nullptr));
                }

                //显示鱼死亡动画并移除
                _fishManager->removeOrSetFishDieById(smallFishId, true);
                //播放抓到鱼金币翻滚和显示奖金动画
                playAwardFishAnimation(pResult->chair_id, pResult->fish_score[i], pResult->i64UserFishScore_, smallFish);
            }
        }
    }
}

void GameTableUIV88::dealCatchSweepFishResultEx(CMD_S_CatchSweepFishResultEx* pResult) {
    /**
     *	捕获的鱼的倍率 > 40 或者为全屏炸弹时，游戏内通过滚动公告进行广播通知
     */
    if (pResult->fish_kind >= FISH_KIND_19 && pResult->fish_kind <= FISH_KIND_26)
    {
        char str[128];
        sprintf(str, GetText("GT_Catch_Fish_Notify"), ForceGBKToUtf8(pResult->cRoomName), pResult->iDeskID + 1, ForceGBKToUtf8(pResult->szNickName), ForceGBKToUtf8(pResult->cFishName), pResult->fish_score);
        showNotice(str, 1, 0);
        //Big Boss Animation
        if (pResult->fish_kind == FISH_KIND_25 || pResult->fish_kind == FISH_KIND_26) {
            if (strcmp(_tableLogic->getUserBySeatNo(_tableLogic->getMySeatNo())->nickName, pResult->szNickName) == 0) {
                playBigWinAnimation(GoldenToadVersion::getGameVersionName(), ForceGBKToUtf8(pResult->cFishName), pResult->fish_score);
            }
        }
    }
}

void GameTableUIV88::dealGameNotifyWuHuaNiu(NN::tag_s2c_notify *msg) {
    if (msg->i64WinMoney < 0) return;
    char str[128] = { 0 };

    if (msg->bDeskID == INVALID_BYTE) {
        //广播
        if (msg->bGameID == 1) {
            sprintf(str, GetText("NN_Catch_Bull_Plus_Notify"), GetText("NN_Game_One"), ForceGBKToUtf8(msg->cRoomName), ForceGBKToUtf8(msg->cNickName), Tools::addComma(msg->i64WinMoney).c_str());
        } else if (msg->bGameID == 2) {
            sprintf(str, GetText("NN_Catch_Bull_Plus_Notify"), GetText("NN_Game_Two"), ForceGBKToUtf8(msg->cRoomName), ForceGBKToUtf8(msg->cNickName), Tools::addComma(msg->i64WinMoney).c_str());
        } else if (msg->bGameID == 3) {
            sprintf(str, GetText("NN_Catch_Bull_Plus_Notify"), GetText("NN_Game_Three"), ForceGBKToUtf8(msg->cRoomName), ForceGBKToUtf8(msg->cNickName), Tools::addComma(msg->i64WinMoney).c_str());
        } else {
            return;
        }
        showNotice(str, 1, 1);
    }
}

void GameTableUIV88::showNotice(const std::string& message, int nCount, int nType) {
    if (!_showNotify) {
        return;
    }
    if (m_Notice == nullptr) {
        m_Notice = GameNotice::create();
        m_Notice->setLocalZOrder(MAX_ZORDER);
    }
    m_Notice->setName("GameNotice");
    m_Notice->openNotic(this, 10000, message, nCount, nType);
}

void GameTableUIV88::dealSwitchScene(CMD_S_SwitchScene* pScene) {
    _isSwitchScene = true;
    _isAllowFire = false;
    //切换场景之前需要把之前的自动射击、锁定等状态重置
    //publicControlLockedFish(false);
    privateControlLockFishFunc(false);	//解除锁定
    privateControlAutoFire(false);//关闭自动射击
    _fishManager->manualUunlockBigFish();//取消对鱼的锁定

    _switchScene = *pScene;

    //播放波浪音效
    GoldenToadGameAudio::stopBackgroundMusic();
    GoldenToadGameAudio::playWaveEffect();

    //波浪动画
    char name[64];
    auto waterAnimation = Animation::create();
    waterAnimation->setDelayPerUnit(0.5f);
    waterAnimation->setLoops(-1);
    for(int i = 1; i <= 4; i++) {
        sprintf(name, "v66/goldenToad/Scene/wave/wave_%02d.png", i);
        waterAnimation->addSpriteFrameWithFile(name);
    }
    auto  animate = Animate::create(waterAnimation);
    auto water = Sprite::create(name);
    float ScaleY = WIN_SIZE.height/ water->getContentSize().height;
    water->setScaleY(ScaleY);
    water->setAnchorPoint(Vec2(0, 0.5f));
    water->setPosition(Vec2(WIN_SIZE.width - 100, WIN_SIZE.height / 2));
    water->runAction(animate);
    addChild(water, 200);

    //切换背景图
    std::string bgName(BG_PATH);
    switch (GoldenToadVersion::getGameVersionName()) {
    case Enums::GAME_VERSION_TOAD_FISH:
        sprintf(name, "bg%d.png", pScene->Scene_Style + 1);
        break;
    case Enums::GAME_VERSION_LIKUI_FISH:
        sprintf(name, "bg%d.png", pScene->Scene_Style + 1 + 2);
        break;
    default:
        sprintf(name, "bg%d.png", pScene->Scene_Style + 1);
        break;
    }
    //sprintf(name, "bg%d.png", pScene->Scene_Style + 1);
    bgName.append(name);

    auto backGround = ImageView::create(bgName);
    backGround->setAnchorPoint(Vec2(0 , 0.5f));
    float _xScale = WIN_SIZE.width / backGround->getContentSize().width;
    float _yScale = WIN_SIZE.height/ backGround->getContentSize().height;
    backGround->setScaleX(_xScale);
    backGround->setScaleY(_yScale);
    backGround->setPosition(Vec2(WIN_SIZE.width, WIN_SIZE.height / 2));
    addChild(backGround);

    auto oldBG = _currentBg;
    _currentBg = backGround;

    //移除跟背景图碰撞的鱼
    _fishManager->setSwitchScene(backGround, true);

    //添加播放海草贝壳动画
    playSeaweedAnimation(pScene->Scene_Style);

    //背景图从右往左运动
    auto water_move = MoveBy::create(SWITCH_SCENE_TIME, Vec2( - WIN_SIZE.width, 0));
    auto bg_move = MoveBy::create(SWITCH_SCENE_TIME, Vec2( - WIN_SIZE.width, 0));
    water->runAction(Sequence::create(water_move, FadeOut::create(0.2f), RemoveSelf::create(true), nullptr));

    backGround->runAction(Sequence::create(bg_move, CallFunc::create([=]() {
        oldBG->removeFromParent();
        _fishManager->setSwitchScene(nullptr, false);
        _isAllowFire = true;
        switchScene();
        _isSwitchScene = false;
    }), nullptr));
}

//播放海草贝壳动画
void GameTableUIV88::playSeaweedAnimation(SceneStyle style) {
    switch (style) {
    case GoldenToad::SCENE_STYLE_1: {
        Animate* beike = buildAnimation("bk_", 24, 1, 0.1f);
        auto bkSp = Sprite::createWithSpriteFrameName("bk_01.png");
        bkSp->setPosition(Vec2(_currentBg->getContentSize().width * 0.2f, _currentBg->getContentSize().height * 0.5f));
        bkSp->runAction(RepeatForever::create(Sequence::create(beike, CallFunc::create([=]() {
            auto meteor = ParticleSystemQuad::create("v66/goldenToad/particle/beikeqipao.plist");
            meteor->setPosition(bkSp->getContentSize() / 2);
            meteor->setAutoRemoveOnFinish(true);
            bkSp->addChild(meteor);
        }), DelayTime::create(10.0f), nullptr)));
        _currentBg->addChild(bkSp, 1);

        Animate* haicao = buildAnimation("shuicao_5_", 14, -1, 0.2f);
        auto hcSp = Sprite::createWithSpriteFrameName("shuicao_5_01.png");
        hcSp->setPosition(Vec2(_currentBg->getContentSize().width * 0.8f, _currentBg->getContentSize().height * 0.35f));
        hcSp->runAction(RepeatForever::create(haicao));
        _currentBg->addChild(hcSp, 1);
    }
    break;
    case GoldenToad::SCENE_STYLE_2: {
        Animate* haicao1 = buildAnimation("shuicao_1_", 29, -1, 0.1f);
        auto hcSp1 = Sprite::createWithSpriteFrameName("shuicao_1_01.png");
        hcSp1->setPosition(Vec2(_currentBg->getContentSize().width * 0.18f, _currentBg->getContentSize().height * 0.45f));
        hcSp1->runAction(RepeatForever::create(haicao1));
        _currentBg->addChild(hcSp1, 1);

        Animate* haicao2 = buildAnimation("shuicao_5_", 14, -1, 0.2f);
        auto hcSp2 = Sprite::createWithSpriteFrameName("shuicao_5_01.png");
        hcSp2->setPosition(Vec2(_currentBg->getContentSize().width * 0.85f, _currentBg->getContentSize().height * 0.55f));
        hcSp2->runAction(RepeatForever::create(haicao2));
        _currentBg->addChild(hcSp2, 1);
    }
    break;
    case GoldenToad::SCENE_STYLE_3: {
        Animate* beike = buildAnimation("bk_", 24, 1, 0.1f);
        auto bkSp = Sprite::createWithSpriteFrameName("bk_01.png");
        bkSp->setPosition(Vec2(_currentBg->getContentSize().width * 0.35f, _currentBg->getContentSize().height * 0.36f));
        bkSp->runAction(RepeatForever::create(Sequence::create(beike, CallFunc::create([=]() {
            auto meteor = ParticleSystemQuad::create("v66/goldenToad/particle/beikeqipao.plist");
            meteor->setPosition(bkSp->getContentSize() / 2);
            meteor->setAutoRemoveOnFinish(true);
            bkSp->addChild(meteor);
        }), DelayTime::create(8.0f), nullptr)));
        _currentBg->addChild(bkSp, 1);

        Animate* haixing = buildAnimation("haixing_", 7, 2, 0.1f);
        auto hxSp = Sprite::createWithSpriteFrameName("haixing_01.png");
        hxSp->setScale(0.7f);
        hxSp->setPosition(Vec2(_currentBg->getContentSize().width * 0.55f, _currentBg->getContentSize().height * 0.72f));
        hxSp->runAction(RepeatForever::create(Sequence::create(haixing, DelayTime::create(10.0f), nullptr)));
        _currentBg->addChild(hxSp, 1);
    }
    break;
    default:
        break;
    }
}

//创建动画
Animate* GameTableUIV88::buildAnimation(string name, int num, int loops, float speed) {
    auto animation = dynamic_cast<Animation*>(AnimationCache::getInstance()->getAnimation(name));
    if (!animation) {
        animation = Animation::create();
        animation->setDelayPerUnit(speed);
        animation->setLoops(loops);
        for (int i = 1; i <= num; i++) {
            std::string temp(name);
            if (PRESSED_TEXTURE) {
                temp.append(StringUtils::format("%02d.pvr.ccz", i));
            } else {
                temp.append(StringUtils::format("%02d.png", i));
            }
            animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(temp));
        }
        AnimationCache::getInstance()->addAnimation(animation, name);
    }

    auto animate = Animate::create(animation);
    return animate;
}

void GameTableUIV88::switchScene() {
    auto version = GoldenToadVersion::getGameVersionName();
    if (version == Enums::GAME_VERSION_MONKEY_FISH) {
        _bgMusicPath = GoldenToadGameAudio::playBackgroundMusic(2);
    }
    else
    {
        _bgMusicPath = GoldenToadGameAudio::playBackgroundMusic();
    }

    switch (_switchScene.scene_kind) {
    case SCENE_KIND_1: {
        assert(_switchScene.fish_count == arraysize(_fishTrace->scene_kind_1_trace_));
        if (_switchScene.fish_count != arraysize(_fishTrace->scene_kind_1_trace_))
            return;

//        int index = 0;
        for (int i = 0; i < _switchScene.fish_count; ++i) {
            FishKind fish_kind_ex[5] = {FISH_KIND_1};
            //拷贝生成的鱼潮路径数据给当前鱼
            std::vector<FPointAngle> fishTraceList;
            std::copy(_fishTrace->scene_kind_1_trace_[i].begin(), _fishTrace->scene_kind_1_trace_[i].end(), std::back_inserter(fishTraceList));
            //创建此条鱼
            _fishManager->addFish(_switchScene.fish_kind[i], _switchScene.fish_id[i], nullptr, fishTraceList, fish_kind_ex);
        }
    }
    break;
    case SCENE_KIND_2: {
        assert(_switchScene.fish_count == arraysize(_fishTrace->scene_kind_2_trace_));
        if (_switchScene.fish_count != arraysize(_fishTrace->scene_kind_2_trace_))
            return;

        for (int i = 0; i < _switchScene.fish_count; ++i) {
            FishKind fish_kind_ex[5] = {FISH_KIND_1};
            //拷贝生成的鱼潮路径数据给当前鱼
            std::vector<FPointAngle> fishTraceList;
            std::copy(_fishTrace->scene_kind_2_trace_[i].begin(), _fishTrace->scene_kind_2_trace_[i].end(), std::back_inserter(fishTraceList));
            //创建此条鱼
            auto fish = _fishManager->addFish(_switchScene.fish_kind[i], _switchScene.fish_id[i], nullptr, fishTraceList, fish_kind_ex);
            //鱼潮2要求上下两排小鱼游到屏幕中等待大鱼经过，所以设置停止时机
            if (nullptr != fish) {
                if (i < 200) {
                    fish->setFishStop(_fishTrace->scene_kind_2_small_fish_stop_index_[i], _fishTrace->scene_kind_2_small_fish_stop_count_);
                } else {
                    fish->setFishStop(_fishTrace->scene_kind_2_big_fish_stop_index_, _fishTrace->scene_kind_2_big_fish_stop_count_);
                }
            }
        }
    }
    break;
    case SCENE_KIND_3: {
        assert(_switchScene.fish_count == arraysize(_fishTrace->scene_kind_3_trace_));
        if (_switchScene.fish_count != arraysize(_fishTrace->scene_kind_3_trace_))
            return;

//        int index = 0;
        for (int i = 0; i < _switchScene.fish_count; ++i) {
            FishKind fish_kind_ex[5] = {FISH_KIND_1};
            //if (_switchScene.fish_kind[i] == FISH_KIND_31)
            //{
            //	for(int k=0; k<5; k++)
            //	{
            //		fish_kind_ex[k] = _switchScene.fish_kindEx[index];

            //	}
            //	index++;
            //}

            //拷贝生成的鱼潮路径数据给当前鱼
            std::vector<FPointAngle> fishTraceList;
            std::copy(_fishTrace->scene_kind_3_trace_[i].begin(), _fishTrace->scene_kind_3_trace_[i].end(), std::back_inserter(fishTraceList));
            //创建此条鱼
            _fishManager->addFish(_switchScene.fish_kind[i], _switchScene.fish_id[i], nullptr, fishTraceList, fish_kind_ex);
        }
    }
    break;
    case SCENE_KIND_4: {
        assert(_switchScene.fish_count == arraysize(_fishTrace->scene_kind_4_trace_));
        if (_switchScene.fish_count != arraysize(_fishTrace->scene_kind_4_trace_))
            return;

        for (int i = 0; i < _switchScene.fish_count; ++i) {
            FishKind fish_kind_ex[5] = {FISH_KIND_1};
            //拷贝生成的鱼潮路径数据给当前鱼
            std::vector<FPointAngle> fishTraceList;
            std::copy(_fishTrace->scene_kind_4_trace_[i].begin(), _fishTrace->scene_kind_4_trace_[i].end(), std::back_inserter(fishTraceList));
            //创建此条鱼
            _fishManager->addFish(_switchScene.fish_kind[i], _switchScene.fish_id[i], nullptr, fishTraceList, fish_kind_ex);
        }
    }
    break;
    case SCENE_KIND_5: {
        assert(_switchScene.fish_count == arraysize(_fishTrace->scene_kind_5_trace_));
        if (_switchScene.fish_count != arraysize(_fishTrace->scene_kind_5_trace_))
            return;

//        int index = 0;
        for (int i = 0; i < _switchScene.fish_count; ++i) {
            FishKind fish_kind_ex[5] = {FISH_KIND_1};
            //if (_switchScene.fish_kind[i] == FISH_KIND_31)
            //{
            //	for(int k=0; k<5; k++)
            //	{
            //		fish_kind_ex[k] = _switchScene.fish_kindEx[index];
            //	}
            //	index++;
            //}
            //拷贝生成的鱼潮路径数据给当前鱼
            std::vector<FPointAngle> fishTraceList;
            std::copy(_fishTrace->scene_kind_5_trace_[i].begin(), _fishTrace->scene_kind_5_trace_[i].end(), std::back_inserter(fishTraceList));
            //创建此条鱼
            _fishManager->addFish(_switchScene.fish_kind[i], _switchScene.fish_id[i], nullptr, fishTraceList, fish_kind_ex);
        }
    }
    break;
    default:
        break;
    }
}

//结算退出
void GameTableUIV88::gameSettlementAndExit() {
    //停止超时退出倒计时
    gameOutTime(false);
    this->stopActionByTag(outTime_tag);

    Node* node = CSLoader::createNode("Games/goldenToad/Node_settlement.csb");
    /*switch (GoldenToadVersion::getGameVersionName()) {
    case Enums::GAME_VERSION_TOAD_FISH:
        node = CSLoader::createNode(Settlemen_path_GOLDEN);
        break;
    case Enums::GAME_VERSION_LIKUI_FISH:
        node = CSLoader::createNode(Settlemen_path_LIKUI);
        break;
    case Enums::GAME_VERSION_MONKEY_FISH:
        node = CSLoader::createNode(Settlemen_path_MONKEY);
        break;
    default:
        node = CSLoader::createNode(Settlemen_path_GOLDEN);
        break;
    }*/
    _tableUi->addChild(node, PLAYER_ZORDER);
    node->setName("SettlementNode");
    node->setTag(Settlemetn_Tag);

    auto panel		= dynamic_cast<Layout*>(node->getChildByName("Panel_Settlement"));
    panel->setPosition(_tableUi->getContentSize() / 2);
    //背景
    auto settleBG	= dynamic_cast<ImageView*>(panel->getChildByName("Image_SettlementBG"));
    //settleBG->setScaleY(720.0f/WIN_SIZE.height);
    //倒计时
    auto time = dynamic_cast<TextAtlas*>(settleBG->getChildByName("AtlasLabel_Time"));
    timingSettlementEx(time, 20, [=]() {
        node->runAction(Sequence::create(FadeOut::create(0.1f), RemoveSelf::create(true), nullptr));
    });
    //取消按钮
    auto cancle	= dynamic_cast<Button*>(settleBG->getChildByName("Button_Cancle"));
    if (cancle) {
        //cancle->setVisible(false);
        //auto btnCancle = AUIButton::create("v66/button/yellow1/BG.png", "v66/button/yellow1/BG_1.png");
        //if (btnCancle) {
            //btnCancle->setTitle("v66/button/yellow1/12.png");
            //btnCancle->setTitleOffset(Vec2(0, 4));
            //btnCancle->setPosition(cancle->getPosition());
            cancle->addClickEventListener([=](Ref* ref) {
                time->setVisible(false);
                node->runAction(Sequence::create(FadeOut::create(0.1f), RemoveSelf::create(true), nullptr));
                //开始下次倒计时
                auto seq = Sequence::create(DelayTime::create(60.f), CallFunc::create([this]() {
                    gameOutTime(true);
                    _tableUi->removeChildByTag(Settlemetn_Tag);
                }), nullptr);
                
                seq->setTag(outTime_tag);
                this->runAction(seq);
            });
            //settleBG->addChild(btnCancle);
        //}
    }
    
    auto close    = dynamic_cast<Button*>(settleBG->getChildByName("Button_close"));
    if (close) {
        close->addClickEventListener([=](Ref* ref) {
            time->setVisible(false);
            node->runAction(Sequence::create(FadeOut::create(0.1f), RemoveSelf::create(true), nullptr));
            //开始下次倒计时
            auto seq = Sequence::create(DelayTime::create(60.f), CallFunc::create([this]() {
                gameOutTime(true);
                _tableUi->removeChildByTag(Settlemetn_Tag);
            }), nullptr);
            
            seq->setTag(outTime_tag);
            this->runAction(seq);
        });
    }
    //退出按钮
    auto exitBtn	= dynamic_cast<Button*>(settleBG->getChildByName("Button_True"));
    if (exitBtn) {
        //exitBtn->setVisible(false);
        //auto exitButton = AUIButton::create("v66/button/blue1/BG.png", "v66/button/blue1/BG_1.png");
        //if (exitButton) {
            //exitButton->setTitle("v66/button/blue1/7.png");
            //exitButton->setTitleOffset(Vec2(0, 4));
           // exitButton->setPosition(exitBtn->getPosition());
            exitBtn->addClickEventListener([this](Ref* ref) {
                _tableUi->removeChildByTag(Settlemetn_Tag);
                returnPlatform();
            });
          //  settleBG->addChild(exitButton);
        //}
    }


   /* auto settleLayer = dynamic_cast<Node*>(settleBG->getChildByName("Node_Settle"));
    char str[32];

    //玩家抓鱼收获金币
    auto fishMoney	= dynamic_cast<Text*>(settleBG->getChildByName("AtlasLabel_FishMoney"));
//    fishMoney->setString(GBKToUtf8(Tools::convertNumberOnlySignificantDigits(_resultInfo->userGoldMoney).c_str()));
    fishMoney->setString(Tools::addComma(_resultInfo->userGoldMoney).c_str());

    //玩家累计输赢
    auto userMoney	= dynamic_cast<Text*>(settleBG->getChildByName("AtlasLabel_UserMoney"));
//    userMoney->setString(GBKToUtf8(Tools::convertNumberOnlySignificantDigits(_players[_tableLogic->_MYSEATNO]->getCurrentMoney()).c_str()));
    userMoney->setString(Tools::addComma(_players[_tableLogic->_MYSEATNO]->getCurrentMoney() - _tableLogic->getMeMoney()).c_str());

    //每条鱼抓到的数量
    //24(25)均为大三元，使用25显示，24加入25中，其为直线大三元
    //26(27)均为大四元，使用27显示，26加入27中，其为直线大四元
    int numThree = 0;
    int numFour = 0;
    for (int i = 0; i < FISH_KIND_COUNT; i++) {
        int NumFish = _resultInfo->numberOfKindFIsh[i];
        sprintf(str, "AtlasLabel_Fish%d", i);
        auto fishNum = dynamic_cast<TextAtlas*>(settleLayer->getChildByName(str));
        if (i == FISH_KIND_27) {
            numThree = _resultInfo->numberOfKindFIsh[i];
            fishNum->setVisible(false);
            continue;
        }
        if (i == FISH_KIND_28) {
            NumFish += numThree;
        }
        if (i == FISH_KIND_29) {
            numFour = _resultInfo->numberOfKindFIsh[i];
            fishNum->setVisible(false);
            continue;
        }
        if (i == FISH_KIND_30) {
            NumFish += numFour;
        }
        sprintf(str, "%d", NumFish);
        
        if (fishNum) {
            fishNum->setString(str);
        }
        
    }*/
}

bool GameTableUIV88::isValidSeat(BYTE seatNo) {
    return (seatNo < PLAY_COUNT && seatNo >= 0);
}

/*------------------------------------------------------------------------------*/

//手机返回键回调
void GameTableUIV88::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (EventKeyboard::KeyCode::KEY_BACK != keyCode) return;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

    if (GoldenToad::canPrintDebugLog) {
        CCLOG("\n\nKey Code: %d", keyCode);
    }

    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_SPACE: {
        //  通过键盘物理键控制相关功能
        //	by HBC
        setAtuoFire(0.0f);
        if (GoldenToad::canPrintDebugLog)
            CCLOG("key code:%d space is pressed. ", keyCode);
        return;
    }
    break;
    case EventKeyboard::KeyCode::KEY_F4: {
        //设置声音

        return;
    }
    break;
    case EventKeyboard::KeyCode::KEY_F6: {
        //上满分

        return;
    }
    break;
    case EventKeyboard::KeyCode::KEY_F7: {
        //  每次按 F7 自动开火 状态切换
        _isAutoFire = !_isAutoFire;
        privateControlAutoFire(_isAutoFire);
        return;
    }
    break;
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW: {
        //下分

        return;
    }
    break;
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW: {
        //上分

        return;
    }
    break;
    case EventKeyboard::KeyCode::KEY_ESCAPE: {
        //openExitWindow();
    }
    break;
    case EventKeyboard::KeyCode::KEY_S: {

        return;
    }
    break;
    case EventKeyboard::KeyCode::KEY_Q: {
        //取消锁定

        return;
    }
    break;
    default:
        return;
        break;
    }

#endif

    openExitWindow();

}

/**
 *	结算界面
 *	by HBC
 */
void GameTableUIV88::openExitWindow() {
	if (_tableUi->getChildByName("SettlementNode"))
	{
		//防止反复创建
		return;
	}

    //开始下次倒计时
    auto seq = Sequence::create(DelayTime::create(60.f), CallFunc::create([this]() {
        gameOutTime(true);
    }), nullptr);
    seq->setTag(outTime_tag);
    this->runAction(seq);
    gameSettlementAndExit();
}

/**
 *	当玩家游戏分不足时，提示其上分
 */
void GameTableUIV88::shoeNoticeWhenLessScoreThenNeed() {
    Node* root = Director::getInstance()->getRunningScene();
    auto Prompt = dynamic_cast<GamePromptLayer*>(root->getChildByName("Prompt"));
    if (!Prompt) {
        Prompt = GamePromptLayer::create();
        Prompt->setName("Prompt");
        Prompt->showPrompt(GBKToUtf8("金币不足！"));
    }
}

void GameTableUIV88::nofityLockFailUI() {
    Node* root = Director::getInstance()->getRunningScene();
    auto Prompt = dynamic_cast<GamePromptLayer*>(root->getChildByName("Prompt"));
    if (!Prompt) {
        Prompt = GamePromptLayer::create();
        Prompt->setName("Prompt");
        Prompt->showPrompt(GBKToUtf8("没有可锁定的大鱼出现！"));
    }
}

void GameTableUIV88::onDisconnect() {
    _tableLogic->stop();
    stopAllActions();
    unscheduleAllCallbacks();
    auto prompt = GamePromptLayer::create();
    prompt->setColorLayerTouchEnabled(false);
    prompt->showPrompt(GetText("NET_DISCONNECT"));
    prompt->setOkayCallBack([this]() {
        RoomLogic()->closeRoom();
        PlatformLogic()->closePlatform();
        CommandManager::getInstance()->execute(GOTO_MENUSTATE_COM);
    });
}

/**
 *	退出游戏，返回游戏大厅
 */
void GameTableUIV88::returnPlatform() {
    if (_isAutoFire) {
        unschedule(schedule_selector(GameTableUIV88::setAtuoFire));
    }

    unscheduleAllCallbacks();
    stopAllActions();

    if (PlatformLogic()->connected()) {
        //结算界面点击确定之后执行这部分逻辑  --  by HBC
        _tableLogic->sendUserUp();
        this->runAction(Sequence::create(DelayTime::create(5.0f), CallFunc::create([=]() {
            onDisconnect();
        }), nullptr));
    } else {
        onDisconnect();
    }
}

void GameTableUIV88::dealLeaveDesk() {
    RoomLogic()->closeRoom();
    PlatformState* state = PlatformState::create();
    state->setType(ROOMLIST);
    StateManager::getInstance()->setState(state);

    unscheduleAllCallbacks();
    stopAllActions();
}

//游戏开始通知
void GameTableUIV88::dealGameStart() {

}

//void GameTableUIV88::dealExchangeFishCoinResult(bool isIncrease, bool isSuccess) {
//    if (!isSuccess) {
//        //因为通知不符合实际情况，暂时不做这个系统消息通知。
//        //showNoticeMessage(GBKToUtf8(isIncrease ? "金币不足，请充值！" : "积分不足，下分失败！"));
//        //showExchangeCoinScoreNoticeMessage(GBKToUtf8(isIncrease ? "金币不足，请充值！" : "积分不足，下分失败！"));//仅显示1s
//    }
//}
//
//void GameTableUIV88::dealShowScoreUpTips(bool isIncrease, bool isSuccess) {
//    //仅在上分成功的时候执行这个内容，隐藏上分提示
//    if (isSuccess) {
//        _toolBar->showScoreUpTips(!isIncrease);
//    }
//}

    void GameTableUIV88::dealUserChangeCoin(GoldenToad::CMD_S_ChangeCoin *changeCoin) {
        BYTE seatNo = changeCoin->chair_id;
        if (seatNo >= PLAY_COUNT || seatNo < 0) return;
        if (!_players[seatNo]->batteryIsVisible()) return;
        _players[seatNo]->setCannonNum(changeCoin->bullet_mulriple);
    }
    
void GameTableUIV88::dealUpdateAllowFish(CMD_COM_ALLOW_LOCK_FISH* allowFish) {
    _fishManager->updateLockFishInfoListFromServer(allowFish->fishID, allowFish);
}

void GameTableUIV88::gameOutTime(bool bShow) {
    this->stopActionByTag(outTime_tag);
    auto tip = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(_tableUi, "Image_ExitTip"));
    Text* time = nullptr;
    if (tip != nullptr) {
        tip->setVisible(bShow);
        time = (Text*)tip->getChildByName("AtlasLabel_Time");
        if (time != nullptr) {
            time->setVisible(bShow);
        }
    }

    if (bShow) {
        if (time != nullptr) {
            //超时倒计时结束，直接发送站起离开游戏
            timingSettlement(time, 10, [this]() {
                returnPlatform();
            });
        }
    }
}

/************************************************************************/
/* 结算界面退出倒计时                                                   */
/************************************************************************/
void GameTableUIV88::timingSettlement(Node* node, int dt, std::function<void()> callFunc) {
    if (nullptr == node || !node->isVisible()) return;

    char str[12];
    sprintf(str, "%d", dt);
    dt--;
    if (dt < 0) {
        callFunc();
        return;
    }

    Text* text = (Text*)node;
    text->setString(str);
    node->runAction(Sequence::create(DelayTime::create(1.0f),
                                     CallFuncN::create(CC_CALLBACK_1(GameTableUIV88::timingSettlement, this, dt, callFunc)), nullptr));
}
void GameTableUIV88::timingSettlementEx(Node* node, int dt, std::function<void()> callFunc) {
        if (nullptr == node || !node->isVisible()) return;
        
        char str[12];
        sprintf(str, "%d", dt);
        dt--;
        if (dt < 0) {
            callFunc();
            return;
        }
        
        TextAtlas* text = (TextAtlas*)node;
        text->setString(str);
        node->runAction(Sequence::create(DelayTime::create(1.0f),
                                         CallFuncN::create(CC_CALLBACK_1(GameTableUIV88::timingSettlementEx, this, dt, callFunc)), nullptr));
}
bool GameTableUIV88::onTouchBegan(Touch *touch, Event *unused_event) {
//    Point point = touch->getLocation();
//    point = _currentBg->convertToNodeSpace(point);
//    if(0 == _toolbarBg->getName().compare("1")) {
//        _toolbarBg->setName("0");
//
//        _toolbarBg->runAction(Sequence::create(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height)),
//        CallFunc::create([this]() {
//            this->getChildByTag(gt_toolbarBtn)->setVisible(true);
//        }), nullptr));
//    }


    //停止上次倒计时
    gameOutTime(false);
    this->stopActionByTag(outTime_tag);

    if (switchTargetFishBegan(touch)) {
        _switchingLockedFish = true;//进入手动切换锁定状态
        return true;
    }

//    BYTE me = _tableLogic->_MYSEATNO;

    //长按连续射击定时器
    _touchPoint = touch->getLocation();
    if (_isAutoFire) {
        return false;
    }

    schedule(schedule_selector(GameTableUIV88::setAtuoFire), 0.25f);

    return true;
}

void GameTableUIV88::onTouchMoved(Touch *touch, Event *unused_event) {
//    if (!_canTouchWhileTeaching) {
//        return;
//    }
    _touchPoint = touch->getLocation();
    ++_switchTimer;
    if (_switchTimer >= 9) { //0.15s
        if (_switchFirstCheckFinish) {
            //实时更新锁定动画资源
            updateSwitchLockTips(touch->getLocation());
            //第一个检测成功过，保证该检测不会重复执行，直接进行第二次检测
            //一级检测通过后才可以继续检测
            if (_switchTimer >= 15) { //0.25s
                if (!_switchSecondCheckFinish) {
                    //第二级检测成功过，保证检测不会重复执行，直接不再进行第二次的检测
                    if (switchTargetFishSecondCheck(touch)) {
                        //切换成功
                        switchTargetFishEnd();//不管切换成功还是失败，都可以恢复默认值。若已经判断切换成功且此时手仍然在触摸屏幕，则停止切换检测逻辑做其他操作。
                    }
                }
            }
        } else {
            //第一次检测尚未进行，开始第一次检测
            if (switchTargetFishFirstCheck(touch)) {
                //一级检测通过后才可以继续检测
                if (_switchTimer >= 50) { //2s
                    if (!_switchSecondCheckFinish) {
                        //第二级检测成功过，保证检测不会重复执行，直接不再进行第二次的检测
                        if (switchTargetFishSecondCheck(touch)) {
                            //切换成功
                            switchTargetFishEnd();//不管切换成功还是失败，都可以恢复默认值。若已经判断切换成功且此时手仍然在触摸屏幕，则停止切换检测逻辑做其他操作。
                        }
                    }
                }
            }
        }
    }
}

void GameTableUIV88::onTouchEnded(Touch *touch, Event *unused_event) {
    //this->stopActionByTag(DelaySwitchLockFish_Tag);
//    if (!_canTouchWhileTeaching) {
//        return;
//    }
    //_delaySwitchLock = false;//
    if (_switchingLockedFish) {
        _switchingLockedFish = false;//通过开关约束，方式切换成功后自动射击的停止
        switchTargetFishEnd();
    } else {
        //松开手指卸载长按连续射击定时器
        unschedule(schedule_selector(GameTableUIV88::setAtuoFire));
        setAtuoFire(0.0f);
    }
    resetValueForNextTouch();
}

void GameTableUIV88::resetValueForNextTouch() {
    _switchingLockedFish = false;
    _switchFirstCheckFinish = false;
    _switchSecondCheckFinish = false;
}

// open fire
void GameTableUIV88::setAtuoFire(float dt) {
    if (!_isAllowFire) return;
    openFireByTouch(_touchPoint);
}

// 触摸屏幕开火
void GameTableUIV88::openFireByTouch(const Vec2& touchPoint) {
    if (!PlatformLogic()->connected()) {
        onDisconnect();
        return;
    }
    //已经发射子弹大于20颗，停止射击
    if (_players[_tableLogic->_MYSEATNO]->getBulletNum() > 20) {
        return;
    }

    if (_isOwnOnTop) {
        if (touchPoint.y > WIN_SIZE.height - _ownOperateVerticalLine) {
            return;
        }
    } else {
        if (touchPoint.y < _ownOperateVerticalLine) {
            return;
        }
    }

    //停止超时退出倒计时
    gameOutTime(false);
    this->stopActionByTag(outTime_tag);

    //开始下次倒计时
    auto seq = Sequence::create(DelayTime::create(60.f), CallFunc::create([this]() {
        gameOutTime(true);
    }), nullptr);

    seq->setTag(outTime_tag);
    this->runAction(seq);

    Vec2 firePos = Vec2::ZERO;

    //如果没有锁定的鱼，则开炮角度为手指触摸的地方
    if (_lockFishId == INVALID_FISHID) {
        if (touchPoint == Vec2::ZERO) {
            firePos = Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2);//自动射击默认射击屏幕中央
        } else {
            firePos = touchPoint;
        }
    } else {
        auto fish = dynamic_cast<Fish*>(_fishManager->getFishById(_lockFishId));
        if (nullptr != fish) {
            firePos = fish->getPosition();
        }
    }

    Vec2 cononpoint = _players[_tableLogic->_MYSEATNO]->getCannonPoint();
    CMD_C_UserFire userFire;
    userFire.bullet_kind = _players[_tableLogic->_MYSEATNO]->getBullet_Kind();
    userFire.angle = MathAide::CalcAngle(firePos.x, firePos.y, cononpoint.x, cononpoint.y);
//    userFire.bullet_mulriple = _players[_tableLogic->_MYSEATNO]->getFirepower();
    userFire.bullet_mulriple = _tableLogic->getBaseBulletMultiple();
    userFire.lock_fishid = _lockFishId;
    userFire.bullet_id = ++_iBullet_id;
    _tableLogic->sendOpenFire(&userFire, sizeof(userFire));


    //为了防止网络延迟造成用户体验问题，自己开炮直接显示
    CMD_S_UserFire meFire;
    meFire.bullet_kind = userFire.bullet_kind;
    meFire.chair_id = _tableLogic->_MYSEATNO;
    meFire.angle = userFire.angle;
    meFire.bullet_mulriple = userFire.bullet_mulriple;
    meFire.lock_fishid = _lockFishId;
    meFire.bullet_id = userFire.bullet_id;

    if (!_players[_tableLogic->_MYSEATNO]->isFireEnable()) {
        unschedule(schedule_selector(GameTableUIV88::setAtuoFire));
        if (_isAutoFire) {
            _isAutoFire = false;
            _autoBtn->setBright(true);
        }
        shoeNoticeWhenLessScoreThenNeed();
        dealGameFireWhenLessScore(&meFire, true);
        return;
    } else {
//        if (_isShowMenu) {
//            updateToolBarStationAndPlayerSPosition(_isAutoFire ? true : false);//自动射击时强制显示菜单栏
//            //updateToolBarStationAndPlayerSPosition(false);//自动射击时强制显示菜单栏
//        }
    }

    if (_players[_tableLogic->_MYSEATNO]->getCurrentMoney() >= _players[_tableLogic->_MYSEATNO]->getFirepower()) {
        LLONG tmpScore = _players[_tableLogic->_MYSEATNO]->getCurrentMoney() - _players[_tableLogic->_MYSEATNO]->getFirepower();
        _players[_tableLogic->_MYSEATNO]->setUserMoney(tmpScore);
        //_tableLogic->setMeScore(tmpScore);
    } else {
        privateControlAutoFire(false);
        shoeNoticeWhenLessScoreThenNeed();
        dealGameFireWhenLessScore(&meFire, true);
        return;
    }

    dealGameFire(&meFire, true);
}

void GameTableUIV88::sendHitFish(BYTE bSeatNo, INT fishId, INT bulletId, BulletKind bulletKind, INT bulletMuriple) {
    _tableLogic->sendHitFish(bSeatNo, fishId, bulletId, bulletKind, bulletMuriple);
}
    void GameTableUIV88::sendChangeCoin(BYTE bSeatNo, bool isAdd) {
        _tableLogic->sendChangeCoin(bSeatNo, isAdd);
    }

void GameTableUIV88::showNoticeMessage(const std::string& message) {
//        _NoticeBG->setVisible(true);
//        _notic->postMessage(message);
    //auto seq = Sequence::create(DelayTime::create(18.f), CallFunc::create([this](){
    //	this->stopActionByTag(ShowMessageAction_Tag);
    //	_NoticeBG->setVisible(false);
    //}), nullptr);
    //seq->setTag(ShowMessageAction_Tag);
    //this->runAction(seq);
}

///**
// *	显示金币、积分兑换失败结果，该方法待优化，目前使用通用的消息通知，可以考虑使用单独的动画资源或者其他显示形式
// *	by HBC
// */
//void GameTableUIV88::showExchangeCoinScoreNoticeMessage(const std::string& message)
//{
//	_NoticeBG->setVisible(true);
//	_notic->postMessage(message);
//	_notic->getActionByTag(ShowMessageAction_Tag);
//	auto seq = Sequence::create(DelayTime::create(10.f), CallFunc::create([this](){
//		//_notic->stopAllActions();
//		_notic->stopActionByTag(ShowMessageAction_Tag);
//		_NoticeBG->setVisible(false);
//	}), nullptr);
//	seq->setTag(outTime_tag);
//	this->runAction(seq);
//}

//////////////////////////////////////////////////////////////////////////////////
//聊天
//////////////////////////////////////////////////////////////////////////////////
//显示或隐藏聊天界面
void GameTableUIV88::showOrHideGameChat(bool bShow) {
    //if (!bShow)
    //{
    //    Size WIN_SIZE = Director::getInstance()->getWIN_SIZE();
    //    auto chatLayer = GameChatDlgLayer::create();
    //    chatLayer->setIgnoreAnchorPointForPosition(false);
    //    chatLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
    //    chatLayer->setPosition(WIN_SIZE / 2);
    //    chatLayer->setName("chatLayer");
    //    chatLayer->setVisible(false);
    //
    //    chatLayer->onSpeakeCallBack = [this](const std::string& text)
    //    {
    //        if (RoomLogic()->getRoomRule() & GRR_FORBID_GAME_TALK)
    //        {
    //            auto prompt = GamePromptLayer::create();
    //            prompt->showPrompt(GBKToUtf8("抱歉,此房间禁止聊天!"));
    //            return;
    //        }
    //
    //        _tableLogic->sendChatMsg(text);
    //    };
    //
    //    chatLayer->onCloseCallBack = [this](){
    //        //开始下次倒计时
    //        auto seq = Sequence::create(DelayTime::create(60.f), CallFunc::create([this](){
    //            gameOutTime(true);
    //        }), nullptr);
    //
    //        seq->setTag(outTime_tag);
    //        this->runAction(seq);
    //    };
    //    addChild(chatLayer, 1000);
    //}
    //else
    //{
    //    auto chatLayer = dynamic_cast<GameChatDlgLayer*>(this->getChildByName("chatLayer"));
    //    if (chatLayer)
    //    {
    //        //停止超时退出倒计时
    //        gameOutTime(false);
    //        this->stopAllActions();
    //        chatLayer->showOrHideChatDlg(true);
    //    }
    //}
}
void GameTableUIV88::showUserChatMsg(BYTE seatNo, CHAR msg[]) {
    //if (!isValidSeat(seatNo)) return;
    //
    //char str[501] = { 0 };
    //std::string record;
    //int _RichTag = 0;
    //std::string filename("goldenToad/GameChatUi/Res/");//气泡路径
    //std::string filenameSmallImage("goldenToad/GameChatUi/Res/chatSmallRes/");//小图路径
    //auto maxFaceCount = 68;
    //std::string word;
    //char num[3] = { 0, 0, 0 };
    //Size lineSize(270, 58);
    //int fontSize = 18;
    //int imageSize = 24;
    //float msgAllLenght = 0;
    //int ImageNum = 0;//表情个数，用来计算表情需占用的位置
    //Color3B colorArray[] = {colorRed, colorBlack };
    //
    //auto randNumber = INT(std::rand() / float(RAND_MAX + 1) * (sizeof(colorArray) / sizeof(Color3B)));
    //Color3B fontColor = colorArray[randNumber];
    //
    //auto node = Node::create();
    //auto pos = node->getAnchorPoint();
    //pos = node->getPosition();
    //_tableUi->addChild(node, 1000);
    //
    //sprintf(str, "Image_ChatBg%d", seatNo);
    //auto chatPos = dynamic_cast<ImageView*>(_tableUi->getChildByName(str));
    //auto ptr = chatPos->getPosition();
    //
    //sprintf(str, "node%d", seatNo);
    //auto chat_Node = (Node*)_tableUi->getChildByName(str);
    //if (chat_Node)
    //{
    //    chat_Node->removeFromParent();
    //}
    //
    //if (seatNo < 3)
    //{
    //    filename.append("chat_bg_text1.png");
    //}
    //else
    //{
    //    filename.append("chat_bg_text0.png");
    //}
    //auto tmp = Sprite::create(filename);
    //auto tmpSize = tmp->getContentSize();
    //Rect fullRect(0, 0, tmpSize.width, tmpSize.height);
    //Rect insetRect = Rect(40, 25, 10, 10);//自动缩放的区域
    //
    //auto chatBG = ui::Scale9Sprite::create(filename, fullRect, insetRect);
    //sprintf(str, "node%d", seatNo);
    //node->setName(str);
    //node->addChild(chatBG);
    //node->setPosition(ptr);
    //
    //auto charMsg = RichText::create();
    //charMsg->ignoreContentAdaptWithSize(false);
    //charMsg->setContentSize(Size(320, lineSize.height));
    //node->addChild(charMsg, 1000);
    //
    //// 在聊天记录和弹出语中分别显示说话玩家名字
    //UserInfoStruct* info = _tableLogic->getUserBySeatNo(seatNo);
    //std::string nameSay = StringUtils::format("[%s说]:", info->nickName);
    //record.append(nameSay);
    //record.append(msg);
    //
    //RichElementText* text = nullptr;
    //
    //text = RichElementText::create(_RichTag++, colorRed, 255, GBKToUtf8(" "), "Arial", fontSize);
    //
    //charMsg->pushBackElement(text);
    //
    //for (int i = 0; msg[i] != '\0'; i++)
    //{
    //    if (msg[i] == '/' && msg[i + 1] == ':')
    //    {
    //        //符合表情条件，再判断是否在表情范围内
    //        num[0] = msg[i + 2];
    //        num[1] = msg[i + 3];
    //        auto imageNumber = atoi(num);
    //        // 一定是表情
    //        if (imageNumber >= 0 && imageNumber < maxFaceCount)
    //        {
    //            if (word.length() > 0)
    //            {
    //                auto rtext = RichElementText::create(_RichTag++, fontColor, 255, GBKToUtf8(word.c_str()), "Arial", fontSize);
    //                charMsg->pushBackElement(rtext);
    //                word.clear();
    //            }
    //            ImageNum++;
    //            auto img = RichElementImage::create(_RichTag++, Color3B(255, 255, 255), 255,
    //                                                filenameSmallImage + StringUtils::format("smiley_%d.png", imageNumber));
    //            charMsg->pushBackElement(img);
    //            i += 3;
    //        }
    //        //不是表情
    //        else
    //        {
    //            msgAllLenght += 0.5f;
    //            word.push_back(msg[i]);
    //        }
    //    }
    //    else
    //    {
    //        msgAllLenght += 0.5f;
    //        word.push_back(msg[i]);
    //    }
    //}
    //
    ////显示文本
    //if (word.length() > 0)
    //{
    //    auto text = RichElementText::create(_RichTag++, fontColor, 255, GBKToUtf8(word.c_str()), "Arial", fontSize);
    //    charMsg->pushBackElement(text);
    //    word.clear();
    //}
    //
    //float realAllLenght = msgAllLenght * fontSize + ImageNum * imageSize;//文本图片总的真实大小
    //float msgRowNum = realAllLenght / 320.0f;
    //float bgWidht, bgHeight;
    //bgWidht = bgHeight = 0;
    //if (msgRowNum < 1)
    //{
    //    //msgRowNum小于1，说明消息只有一行，需缩放气泡的宽
    //    if (msgRowNum <= 0.12f)
    //    {
    //        bgWidht = 100;
    //        bgHeight = lineSize.height;
    //    }
    //    else
    //    {
    //        bgWidht = realAllLenght + 60;
    //        bgHeight = lineSize.height;
    //    }
    //}
    //else
    //{
    //    //msgRowNum大于1，需缩放气泡的高
    //    bgWidht = lineSize.width + 60;
    //    bgHeight = (20.0f * (int)msgRowNum) + lineSize.height;
    //}
    ////设置气泡大小
    //chatBG->setContentSize(Size(bgWidht, bgHeight));
    //charMsg->setContentSize(Size(lineSize.width, bgHeight));
    //
    //chatBG->setAnchorPoint(Vec2(0, 0.5f));
    //charMsg->setAnchorPoint(Vec2(0, 0.5f));
    //charMsg->setPosition(Vec2(-charMsg->getContentSize().width / 2 + 35, chatBG->getPositionY() - 15));
    //
    //node->runAction(Sequence::create(DelayTime::create(3.0f), RemoveSelf::create(true), nullptr));
    //
    //auto chatLayer = dynamic_cast<GameChatDlgLayer*>(this->getChildByName("chatLayer"));
    //if (chatLayer)
    //{
    //    chatLayer->addChatRecord(record);
    //}
}

//////////////////////////////////////////////////////////////////////
////超端
//////////////////////////////////////////////////////////////////////
//void GameTableUIV88::dealSetAdminConfig(bool bAdmin) {
//    //auto btnAdmin = dynamic_cast<Button*>(_tableUi->getChildByName("Button_Admin"));
//    //btnAdmin->setVisible(bAdmin);
//}
//
//void GameTableUIV88::dealStockOperateResult(CMD_S_StockOperateResult* pStock) {
//    GameAdmin* admin = dynamic_cast<GameAdmin*>(this->getChildByName("adminLayer"));
//    if (admin) {
//        char str[32];
//        if (3 == pStock->operate_code) {
//            sprintf(str, "当前抽水：%lld", pStock->stock_score);
//        } else {
//            sprintf(str, "当前奖池：%lld", pStock->stock_score);
//        }
//
//        admin->showStockOperateResult(str);
//    }
//}
//
//void GameTableUIV88::dealUpdateOnlinePlayerList(std::vector<OnLineUserInfo> users) {
//    GameAdmin* admin = dynamic_cast<GameAdmin*>(this->getChildByName("adminLayer"));
//    if (admin) admin->createOnlineList(users);
//}
//
//void GameTableUIV88::dealUpdateContorlList(std::vector<ControlInfo> controls) {
//    GameAdmin* admin = dynamic_cast<GameAdmin*>(this->getChildByName("adminLayer"));
//    if (admin) admin->createControlList(controls);
//}
//
//void GameTableUIV88::dealUpdateSpecialList(std::vector<SpecialUser> specials) {
//    GameAdmin* admin = dynamic_cast<GameAdmin*>(this->getChildByName("adminLayer"));
//    if (admin) admin->createSprcialList(specials);
//}
//
//void GameTableUIV88::dealSetSwitchInfo(S_C_SWITCH_INFO* pSwitch) {
//    GameAdmin* admin = dynamic_cast<GameAdmin*>(this->getChildByName("adminLayer"));
//    if (admin) admin->setCheckBoxState(pSwitch);
//}
//
//void GameTableUIV88::dealUpdateFishName(S_C_FISH_NAME* pName) {
//    GameAdmin* admin = dynamic_cast<GameAdmin*>(this->getChildByName("adminLayer"));
//    if (admin) admin->setSpecialFishName(pName);
//}

void GameTableUIV88::resetUserMoneyAndScore(long long money, long long score) {
    if (_players[_tableLogic->_MYSEATNO]) {
        _players[_tableLogic->_MYSEATNO]->setUserMoney(score);
    }
//        _tableLogic->setMeMoney(money);
//    _toolBar->setUserCoin(money);
}

/**
 *	手动切换目标鱼
 */
bool GameTableUIV88::switchTargetFishBegan(Touch* touch) {
    if (!_isLockFish) {
        return false;//若此时不是锁定状态，则不做切换检测
    }




    /************************************************************************/
    /* 暂时屏蔽掉手动切换鱼的逻辑  by HBC                                   */
    /************************************************************************/
    return false;





    if (_fishManager->switchTargetBigFish(touch->getLocation()) == nullptr) {
        //点击的鱼不可做为切换的目标鱼，停止后续检测
        _switchTouchEnd = true;//该值变成true，表明切换逻辑提前结束
        privateResetSwitchFishID();
        return false;//切换中断
    } else {
        //点击的鱼是有效的，此时应该同步开启定时器
        _switchTouchEnd = false;
        auto fish = _fishManager->switchTargetBigFish(touch->getLocation());
        if (fish != nullptr) {
            _switchLockFishID = fish->getFishID();
            return true;//TODO:
        }
    }
    _switchLockSuccess = false;
    return false;//切换可以继续检测
}

// 0.3s 后执行这部分内容，播放动画
bool GameTableUIV88::switchTargetFishFirstCheck(Touch* touch) {
    if (!_switchTouchEnd) {
        auto fish = _fishManager->switchTargetBigFish(touch->getLocation());
        if (fish != nullptr && _switchLockFishID == fish->getFishID()) {
            showSwitchLockTips(touch->getLocation());
            _switchFirstCheckFinish = true;//这个值要在动画播放完变更
            return true;//检测成功则直接返回
        }
    }
    //切换失败，则直接恢复相关状态值并返回
    return switchTargetFishEnd();
}

//0.4s动画后进行该检测，即为第二次检测，这个检测若通过，则代表切换已经成功，可以直接在锁定的鱼身上播放锁定动画
bool GameTableUIV88::switchTargetFishSecondCheck(Touch* touch) {
    //若第一个动画播放过程中，切换没有手动停掉，则该检测有效
    if (!_switchTouchEnd) {
        //若手指触摸的鱼与之前预锁定的鱼 ID 一致，则代表锁定切换成功
        auto fish = _fishManager->switchTargetBigFish(touch->getLocation());
        if (fish != nullptr && _switchLockFishID == fish->getFishID()) {
            //切换锁定成功
            _switchLockSuccess = true;
            //解除之前的鱼的锁定，播放 0.3s 的动画，之后重新锁定预切换的鱼
            //解锁之前锁定的鱼
            _fishManager->manualUunlockBigFish();
            _lockFishId = _switchLockFishID;//这句必须执行，否则会导致鱼池中的鱼的ID有非法值的情况
            _switchSecondCheckFinish = true;//动画播放完之后再将这个值设置为true
            //锁定新的鱼
            _fishManager->manualSwitchLockedBigFish(fish->getFishID());
            return true;//检测成功则直接返回
        }
    }
    //切换失败则恢复相关状态值并返回
    return switchTargetFishEnd();
}

bool GameTableUIV88::switchTargetFishEnd() {
    hideSwitchLockTips();
    _switchTouchEnd = true;
    _switchLockSuccess = false;
    _switchTimer = 0;
    _switchFirstCheckFinish = false;
    _switchSecondCheckFinish = false;
    privateResetSwitchFishID();
    return false;//
}

//这个提示应该放到鱼的类中进行封装
void GameTableUIV88::showSwitchLockTips(Vec2 pos) {
    _switch_tip_sprite->setScale(1.5f);
    _switch_tip_sprite->setPosition(pos);
    _switch_tip_sprite->setVisible(true);
    _switch_tip_sprite->runAction(Sequence::create(ScaleTo::create(0.4f, 0.8f), nullptr));
}

void GameTableUIV88::updateSwitchLockTips(Vec2 pos) {
    _switch_tip_sprite->setPosition(pos);
}

void GameTableUIV88::hideSwitchLockTips() {
    _switch_tip_sprite->setVisible(false);
    _switch_tip_sprite->setScale(1.0f);
}

    void GameTableUIV88::playBigWinAnimation(int gameID, std::string fishName, LLONG money) {
        std::string gameName = "";
        switch (gameID) {
            case Enums::GAME_VERSION_TOAD_FISH:
                gameName = "新金蟾捕鱼";
                break;
            case Enums::GAME_VERSION_LIKUI_FISH:
                gameName = "新李逵捕鱼";
                break;
            case Enums::GAME_VERSION_MONKEY_FISH:
                gameName = "大闹天宫";
                break;
            default:
                break;
        }
        char tipString[256] = {0};
        sprintf(tipString, GetText("BIG_WIN_TIP_FISH"), gameName.c_str(), fishName.c_str());
        //
        auto node = CSLoader::createNode("v88/animation/common/jackpotting/jackpotting.csb");
        if (node) {
            auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 150));
            addChild(colorLayer, 800);
            colorLayer->runAction(Sequence::create(DelayTime::create(2.5f), RemoveSelf::create(), nullptr));
            node->setPosition(WIN_SIZE/2);
            addChild(node, 800);
            auto action = CSLoader::createTimeline("v88/animation/common/jackpotting/jackpotting.csb");
            if (action) {
                node->runAction(action);
                action->play("jackpotting", false);
            }
            //
            char gameNamePath[64] = {0};
            sprintf(gameNamePath, "v66/common/bigwin/%d.png", gameID);
            auto tmpNode = node->getChildByName("Node_1");
            if (tmpNode) {
                auto kuang = tmpNode->getChildByName("kuang");
                if (kuang) {
                    //game name
                    ImageView* gameName = ImageView::create(gameNamePath);
                    if (gameName) {
                        gameName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                        gameName->setPosition(Vec2(kuang->getContentSize().width/2, kuang->getContentSize().height*0.73f));
                        kuang->addChild(gameName);
                    }
                    //money value
                    TextAtlas* moneyAtlas = dynamic_cast<TextAtlas*>(kuang->getChildByName("AtlasLabel_2"));
                    if (moneyAtlas) {
                        moneyAtlas->setProperty(to_string(money), "v66/common/zi/111.png", 55, 78, "0");
                    }
                    //tip
                    Text* tip = Text::create(tipString, FONT_TTF_MSYH, 32);
                    if (tip) {
                        tip->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                        tip->setPosition(Vec2(kuang->getContentSize().width/2, kuang->getContentSize().height*0.24f));
                        tip->setTextVerticalAlignment(TextVAlignment::CENTER);
                        tip->setTextHorizontalAlignment(TextHAlignment::CENTER);
                        tip->ignoreContentAdaptWithSize(false);
                        tip->setContentSize(Size(kuang->getContentSize().width*0.77f, kuang->getContentSize().height*0.26f));
                        kuang->addChild(tip);
                    }
                }
            }
        }
    }

////年兽触发
//void GameTableUIV88::onGameShowNianShou() {
//    //播动画
//    showNianShouAnimation();
//}
//
////赶年兽结果
//void GameTableUIV88::onGameDriveNianShouResult(int nUserID, long long i64RewardMoney) {
//    //过滤只有自己看的到
//    if (nUserID != _tableLogic->getUserId(_tableLogic->getMySeatNo())) {
//        return;
//    }
//
//    removeNianShouButton();
//
//    Vec2 pos = NewYearActivityInGame::NianShouButtonPosition;
//
//    //有鞭炮赶年兽奖励
//    if (i64RewardMoney > 0) {
//        NewYearActivity::NSAnimationTool::playTNTAudio(i64RewardMoney);
//        auto daijiNode = NewYearActivity::NSAnimationTool::createNSGanNodeAni("hit", false);
//        if (daijiNode) {
//            daijiNode->setPosition(Vec2(pos));
//            this->addChild(daijiNode, NewYearActivityInGame::NianShouZOrder);
//        }
//
//        NewYearActivity::NSAnimationTool::createRewardLabel(i64RewardMoney, this, Vec2(pos.x, pos.y + 50));
//    } else { //无鞭炮
//        NewYearActivity::NSAnimationTool::createTipOfNoProp(this, pos);
//    }
//}
//
////对局获得鞭炮
//void GameTableUIV88::onGameMatchGainFirecraker(int nUserID) {
//    //该id获得一个中型鞭炮
//    //只有自己显示
//    if (_tableLogic->getUserId(_tableLogic->getMySeatNo()) == nUserID) {
//        ImageView* pTipImage = ImageView::create("common/gainFirecrakerTip.png");
//        pTipImage->setScale(0.1);
//        pTipImage->setPosition(Vec2(250, 140));
//        addChild(pTipImage, NewYearActivityInGame::NianShouZOrder);
//
//        Spawn* spawn = Spawn::create(ScaleTo::create(0.5f, 1.0f), FadeIn::create(0.5f), nullptr);
//        Sequence* seq = Sequence::create(spawn, DelayTime::create(2.0f), RemoveSelf::create(), nullptr);
//        pTipImage->runAction(seq);
//    }
//}
//
//void GameTableUIV88::showNianShouAnimation() {
//    if (this->getChildByName("DriveNianShouBtn")) {
//        return;
//    }
//    auto btn = NewYearActivity::NSAnimationTool::createNSButton("DriveNianShouBtn");
//    if (btn) {
//        btn->setPosition(NewYearActivityInGame::NianShouButtonPosition);
//        btn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::driveNianShouClickCallback, this));
//        addChild(btn, NewYearActivityInGame::NianShouZOrder);
//
//        auto daijiNode = NewYearActivity::NSAnimationTool::createNSDaijiNodeAni("daiji", true);
//        if (daijiNode) {
//            daijiNode->setPosition(Vec2(btn->getContentSize().width*0.5f, btn->getContentSize().height*0.5f));
//            btn->addChild(daijiNode);
//        }
//        scheduleOnce(schedule_selector(GameTableUIV88::autoRemoveNianShouButton), 5.0f);
//    }
//}
//
//void GameTableUIV88::driveNianShouClickCallback(Ref* pSender, Widget::TouchEventType type) {
//    if (type != Widget::TouchEventType::ENDED)
//        return;
//
////        unschedule(schedule_selector(GameTableUIV88::autoRemoveNianShouButton));
//
//    Button* btn = (Button*)pSender;
//    btn->setTouchEnabled(false);
//
//    //发包
//    int nUserID = _tableLogic->getUserId(_tableLogic->getMySeatNo());
//    _tableLogic->sendDriveNianShou(nUserID);
//}
//
//void GameTableUIV88::removeNianShouButton() {
//    auto btn = this->getChildByName("DriveNianShouBtn");
//    if (btn) {
//        btn->stopAllActions();
//        btn->removeFromParentAndCleanup(true);
//    }
//}
//
//void GameTableUIV88::autoRemoveNianShouButton(float dt) {
//    removeNianShouButton();
//}

void GameTableUIV88::scheduleUpdatePlayEffectTag(float dt) {
    if (_audioEngine) {
        _audioEngine->resetPlayEffectTag();
    }
}

    void GameTableUIV88::onLuckyMoneyNotify(LuckyMoneyNotify *msg)
    {
        if (nullptr == msg)
        {
            return;
        }
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto notify = CSLoader::createNode("Reward/ui.csb");
        notify->setName("RewardNotice");
        auto btnOpenReward = dynamic_cast<Button*>(notify->getChildByName("btnOpen"));
        btnOpenReward->addTouchEventListener(std::bind([&](Ref* psender, Widget::TouchEventType type){
            if (Widget::TouchEventType::ENDED != type)
            {
                return;
            }
            
            RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_OPEN_LUCKYMONEY, 0, 0);
            this->removeChildByName("RewardNotice");
        }, std::placeholders::_1, std::placeholders::_2));
        
        addChild(notify, 200);
        notify->setPosition(Vec2(visibleSize)/2);
        
        runAction(Sequence::create(DelayTime::create(msg->nStayTime), CallFunc::create([&](){
            this->removeChildByName("RewardNotice");
        }), NULL));
    }
    
    void GameTableUIV88::onLuckyMoneyResult(LuckyMoneyResult *msg)
    {
        if (nullptr == msg)
        {
            return;
        }
        
        if (msg->nType != 1)
        {
            GamePromptLayer::create()->showPrompt("手慢无！下次快点哦。");
            return;
        }
        
        auto result = CSLoader::createNode("Reward/reward.csb");
        auto action = CSLoader::createTimeline("Reward/reward.csb");
        if (nullptr != result && nullptr != action)
        {
            addChild(result, 200);
            auto visibleSize = Director::getInstance()->getVisibleSize();
            result->setPosition(Vec2(visibleSize)/2);
            result->setName("rewardResult");
            result->getChildByName("show")->runAction(action);
            action->gotoFrameAndPlay(0, false);
            
            auto txtReward = dynamic_cast<Text*>(result->getChildByName("txtReward"));
            txtReward->setString("");
            
            auto luckyMoney = msg->i64LuckyMoney;
            auto retainTime = msg->nShowTime;
            runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
                txtReward->setString(Tools::addComma(luckyMoney));
            }), DelayTime::create(retainTime), CallFunc::create([&](){
                this->removeChildByName("rewardResult");
            }), NULL));
        }
    }
}
