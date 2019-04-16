#include "BRXJGameTableUIV88.h"


#include "../../BJL/Classes/v88/BJLNoticeV88.h"
//#include "../../TTNN/Classes/v88/BRNNSetLayerV88.h"
#include "../../TTNN/Classes/v88/BRNNOtherPlayerV88.h"
#include "../../TTNN/Classes/v88/BRNNSettlementLayerV88.h"
#include "../../../../code/common/LibHNLobby/HNLobby/GameNotice.h"
#include "HNLobby/v66/ExtraReward.h"

#include "HNLobby/FontConfig.h"
#include "HNLobby/GamePlatform.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "base/ccMacros.h"
#include "HNLobby/pattern/StateManager.h"
#include "HNLobby/globel.h"
#include "HNLobby/state/PlatformState.h"
#include "Sound/HNAudioEngine.h"
#include "Tool/Tools.h"
#include "BJLGameTableUIData.h"
//#include "../v88/BJLNoticeV88.h"
#include "UI/Scanner.h"
#include "UI/ToolBarLayer.h"
#include "UI/HelpLayer.h"
#include "UI/FloatCoin.h"
#include "HNLobby/pattern/factory/gameSet/GameSetFactory.h"

namespace BRXJ {

#define		WINSIZE Director::getInstance()->getWinSize()
#define		BET_ZORDER					50
#define		LUZI_ZORDER				51
#define		CLOCK_ZORDER			52

#define  ZORDER_GAMETABLE		40
#define  ZORDER_DASHBOARD		50			//控制面板
#define  ZORDER_CARDBOARD		60
#define  ZORDER_MAX					200

#define TOOLBALL_OFFSET_Y_V99 8.0f

//csb
static const char* CSB_BANKER = "Games/11100600/bankSeat_node_v88.csb";
static const char* CSB_TABLE = "Games/11100600/table_node_v88.csb";
static const char* CSB_BOTTOM = "Games/11100600/bottom_node_v88.csb";
//NTLIST
static const char* LIST_CSB = "csb/v66/BJL/applyBankerList_v88.csb";

//music
static const char* BJL_GAME_MUSIC_LOSE = "v66/music/lose.mp3";
static const char* BJL_GAME_MUSIC_WIN = "music/win.mp3";
static const char* GAME_SOUND_BTN = "music/btnEffect/button_click_sound.mp3";
static const char* BJL_GAME_MUSIC_TIP_BET = "v66/music/BJL/tipBet.mp3";
static const char* BJL_GAME_MUSIC_BET = "Games/11100600/music/addchip.mp3";
static const char* BJL_GAME_MUSIC_BET_BACK = "music/NN/betBackGround.mp3";

static const char* CARD_RES = "v66/common/card_Common/cards.plist";
//res
static const char* BRXJ_TONGPEI_CSB = "Games/11100600/allkill_animation.csb";
static const char* BRXJ_TONGSHA_CSB = "Games/11100600/allLose_animation.csb";
static const char* BRXJ_BET_AREA_PATH = "Games/11100600/nine/area%d.png";

static const char* BJL_GAME_HEGUAN = "v88/animation/baccarat/heguan/heguan.csb";

//

// 获取游戏界面
HNGameUIBase* GameTableUIV88::create(BYTE bDeskIndex, bool bAutoCreate) {
	GameTableUIV88* ptr = new GameTableUIV88();
    if (nullptr != ptr) {
        ptr->init(bDeskIndex, bAutoCreate);
        ptr->autorelease();
        return ptr;
    }
    CC_SAFE_DELETE(ptr);
    return nullptr;
}

GameTableUIV88::GameTableUIV88() {
    _pockerIdx = 0;
    _cardTurn = 0;
    m_bNextOut = false;
    m_bAutoBet = false;
    m_isNote = false;
    m_Notice = nullptr;
    memset(_arrayCardValues, 0, sizeof(_arrayCardValues));
    memset(_areaNoteSave, 0, sizeof(_areaNoteSave));
}

GameTableUIV88::~GameTableUIV88() {
    //	_logic->stop();
	//this->unschedule(schedule_selector(GameTableUIV88::autoRemoveNianShouButton));
    CC_SAFE_DELETE(_logic);
    m_Notice = nullptr;
}

void GameTableUIV88::onEnter() {

	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile(CARD_RES);

    HNGameUIBase::onEnter();
}

void GameTableUIV88::onExit() {
	auto cache = SpriteFrameCache::getInstance();
	cache->removeSpriteFramesFromFile(CARD_RES);

    HNGameUIBase::onExit();
}

// 初始化
bool GameTableUIV88::init(BYTE bDeskIndex, bool bAutoCreate) {
    if (!HNGameUIBase::init()) {
        return false;
    }

    //服务端 存牌值 和其他牌类数据 0 庄 然后逆时针递增
    _tableBg = ImageView::create(GAME_BG_HEGUAN_V66);
    _tableBg->setAnchorPoint(Vec2(0, 0));
    _tableBg->setPosition(WIN_SIZE / 2);
    addChild(_tableBg, ZORDER_MAX + 50);
    _tableBg->setOpacity(0);

    //触摸
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto myListener = EventListenerTouchOneByOne::create();
    myListener->onTouchBegan = [](Touch* touch, Event* event)->bool{
        return true;
    };
	myListener->onTouchEnded = CC_CALLBACK_2(GameTableUIV88::onTouchEnded, this);
    myListener->setSwallowTouches(false);
    dispatcher->addEventListenerWithSceneGraphPriority(myListener, _tableBg);

	_logic = new GameTableLogic(this, bDeskIndex, bAutoCreate);

    //初始化UI
    initUI();
    initNTList();
    initBankUI();
    initBottomUI();
    initBet();
    initLuZhi();
    initClock();
    createToolBarUI();

    _logic->sendGameInfo();

    runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([&] () {
        auto scanner = Scanner::create();
        this->addChild(scanner, 100000);
        scanner->starPlayerEffect();
        scanner->setPosition(60, 200);
    }), nullptr));
    
    return true;
}

/*------------------------初始化UI-------------------------------*/
void GameTableUIV88::initUI() {

    //桌子背景
    auto deskBg = ImageView::create("v66/BJL/deskBg.png");
    deskBg->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2));
    addChild(deskBg);
    deskBg->setScaleX(WIN_SIZE.width / 1280);
    deskBg->setScaleY(WIN_SIZE.height / 720);
    //root
    m_root = CSLoader::createNode(CSB_TABLE);
    m_root->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 - 50));
    this->addChild(m_root);

    _tableUi.Panel = static_cast<Layout*>(m_root->getChildByName("Panel"));

    //区域金币
	_tableUi.AtlasLabel_top = static_cast<Text*>(_tableUi.Panel->getChildByName("Text_1"));
	_tableUi.AtlasLabel_sky = static_cast<Text*>(_tableUi.Panel->getChildByName("Text_2"));
    _tableUi.AtlasLabel_bottom = static_cast<Text*>(_tableUi.Panel->getChildByName("Text_3"));
    _vecAreaMoney.push_back(_tableUi.AtlasLabel_top);
    _vecAreaMoney.push_back(_tableUi.AtlasLabel_sky);
    _vecAreaMoney.push_back(_tableUi.AtlasLabel_bottom);
    //区域压注总金币
    _tableUi.AtlasLabel_topTotle = static_cast<TextAtlas*>(_tableUi.Panel->getChildByName("AtlasLabel_topTotle"));
    _tableUi.AtlasLabel_skyTotle = static_cast<TextAtlas*>(_tableUi.Panel->getChildByName("AtlasLabel_skyTotle"));
    _tableUi.AtlasLabel_bottomTotle = static_cast<TextAtlas*>(_tableUi.Panel->getChildByName("AtlasLabel_bottomTotle"));
    _vecAreaTotleMoney.push_back(_tableUi.AtlasLabel_topTotle);
    _vecAreaTotleMoney.push_back(_tableUi.AtlasLabel_skyTotle);
    _vecAreaTotleMoney.push_back(_tableUi.AtlasLabel_bottomTotle);

    //游戏状态
    _tableUi.Text_gameStation = static_cast<Text*>(_tableUi.Panel->getChildByName("Text_gameStation"));
	_tableUi.Text_gameStation->setVisible(false);

	_tableUi.Sprite_gameStation = Sprite::create();

	_tableUi.Sprite_gameStation->setPosition(_tableUi.Text_gameStation->getPosition());
	_tableUi.Sprite_gameStation->setScale(0.6f);
	_tableUi.Sprite_gameStation->runAction(RepeatForever::create(Blink::create(0.5, 1)));
	_tableUi.Text_gameStation->getParent()->addChild(_tableUi.Sprite_gameStation);

    //区域提示框
    _tableUi.Sprite_top = static_cast<Sprite*>(_tableUi.Panel->getChildByName("Sprite_top"));
    _tableUi.Sprite_sky = static_cast<Sprite*>(_tableUi.Panel->getChildByName("Sprite_sky"));
    _tableUi.Sprite_bottom = static_cast<Sprite*>(_tableUi.Panel->getChildByName("Sprite_bottom"));
    _tableUi.Sprite_bankPrompt = static_cast<Sprite*>(_tableUi.Panel->getChildByName("Sprite_bankPrompt"));
    _vecPromptWin.push_back(_tableUi.Sprite_bankPrompt);
    _vecPromptWin.push_back(_tableUi.Sprite_top);
    _vecPromptWin.push_back(_tableUi.Sprite_sky);
    _vecPromptWin.push_back(_tableUi.Sprite_bottom);
    //LOGO
    _tableUi.Sprite_fapaiqi = static_cast<Sprite*>(m_root->getChildByName("Sprite_fapaiqi"));
    _tableUi.puke = static_cast<Sprite*>(_tableUi.Panel->getChildByName("puke"));
    //static_cast<Sprite*>(_tableUi.Panel->getChildByName("girl"))->setVisible(false);

    auto girl = Sprite::create();
    girl->setContentSize(Size(300, 300));
    girl->setScaleX(WIN_SIZE.width / 1280);
    girl->setScaleY(WIN_SIZE.height / 720);
    girl->setVisible(true);
    girl->setPosition(Vec2(WINSIZE.width *0.5f, WINSIZE.height - girl->getContentSize().height *0.54f));
    addChild(girl);

    Vec2 girlPos = girl->getPosition();

    auto csb = CSLoader::createNode(BJL_GAME_HEGUAN);
    csb->setPosition(Vec2(girl->getContentSize().width*0.5f, girl->getContentSize().height*0.5f));
    girl->addChild(csb, ZORDER_CARDBOARD);

    _antAction = CSLoader::createTimeline(BJL_GAME_HEGUAN);
    girl->runAction(_antAction);

	schedule(schedule_selector(GameTableUIV88::girlAnimation), 7.0f);

    //下注区域
    _tableUi.xiazhu0 = static_cast<ImageView*>(_tableUi.Panel->getChildByName("xiazhu0"));//上
    _tableUi.xiazhu1 = static_cast<ImageView*>(_tableUi.Panel->getChildByName("xiazhu1"));//天
    _tableUi.xiazhu2 = static_cast<ImageView*>(_tableUi.Panel->getChildByName("xiazhu2"));//地

    _tableUi.xiazhu0->setTouchEnabled(true);
    _tableUi.xiazhu1->setTouchEnabled(true);
    _tableUi.xiazhu2->setTouchEnabled(true);

	_tableUi.xiazhu0->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::onNoteAreaCallBack, this));
	_tableUi.xiazhu1->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::onNoteAreaCallBack, this));
	_tableUi.xiazhu2->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::onNoteAreaCallBack, this));

    _vecNoteArea.push_back(_tableUi.xiazhu0);
    _vecNoteArea.push_back(_tableUi.xiazhu1);
    _vecNoteArea.push_back(_tableUi.xiazhu2);

    //筹码区域
    auto chipArea0 = static_cast<Layout*>(_tableUi.Panel->getChildByName("Panel_top"));		//上
    auto chipArea1 = static_cast<Layout*>(_tableUi.Panel->getChildByName("Panel_sky"));			//天
    auto chipArea2 = static_cast<Layout*>(_tableUi.Panel->getChildByName("Panel_bottom"));	//地
    _vecChipsArea.push_back(chipArea0);
    _vecChipsArea.push_back(chipArea1);
    _vecChipsArea.push_back(chipArea2);
    getAreaBetPixData();

    auto alts0 = static_cast<TextAtlas*>(_tableUi.Panel->getChildByName("TextAtlas_0"));	//庄
    auto alts1 = static_cast<TextAtlas*>(_tableUi.Panel->getChildByName("TextAtlas_1"));	//上
    auto alts2 = static_cast<TextAtlas*>(_tableUi.Panel->getChildByName("TextAtlas_2"));	//天
    auto alts3 = static_cast<TextAtlas*>(_tableUi.Panel->getChildByName("TextAtlas_3"));	//地
    alts0->setLocalZOrder(10000);
    alts1->setLocalZOrder(10000);
    alts2->setLocalZOrder(10000);
    alts3->setLocalZOrder(10000);
    _vecTextAlts.push_back(alts0);
    _vecTextAlts.push_back(alts1);
    _vecTextAlts.push_back(alts2);
    _vecTextAlts.push_back(alts3);
    auto dian0 = static_cast<Sprite*>(_tableUi.Panel->getChildByName("bjl_dian0"));	//庄
    auto dian1 = static_cast<Sprite*>(_tableUi.Panel->getChildByName("bjl_dian1"));	//上
    auto dian2 = static_cast<Sprite*>(_tableUi.Panel->getChildByName("bjl_dian2"));	//天
    auto dian3 = static_cast<Sprite*>(_tableUi.Panel->getChildByName("bjl_dian3"));	//地
    dian0->setLocalZOrder(10000);
    dian1->setLocalZOrder(10000);
    dian2->setLocalZOrder(10000);
    dian3->setLocalZOrder(10000);
    _vecDianSprs.push_back(dian0);
    _vecDianSprs.push_back(dian1);
    _vecDianSprs.push_back(dian2);
    _vecDianSprs.push_back(dian3);
    for (int i = 0; i < 4; i++) {
        char str[30] = { 0 };
        sprintf(str, "baozi%d", i);
        auto icon = static_cast<Sprite*>(_tableUi.Panel->getChildByName(str));
        icon->setLocalZOrder(10000);
        _vecBaoZiSprs.push_back(icon);
    }

    //cardPos
    auto zhuangPukePos = static_cast<Sprite*>(_tableUi.Panel->getChildByName("Sprite_zhuangPuke"))->getPosition();
    auto topPukePos = static_cast<Sprite*>(_tableUi.Panel->getChildByName("Sprite_topPuke"))->getPosition();
    auto tianPukePos = static_cast<Sprite*>(_tableUi.Panel->getChildByName("Sprite_skyPuke"))->getPosition();
    auto bottompockerPos = static_cast<Sprite*>(_tableUi.Panel->getChildByName("Sprite_bottomPuke"))->getPosition();
    _vecCardPos.push_back(zhuangPukePos);
    _vecCardPos.push_back(topPukePos);
    _vecCardPos.push_back(tianPukePos);
    _vecCardPos.push_back(bottompockerPos);

    for (int i = 0; i < 6; i++) {
        auto imageBg = static_cast<ImageView*>(_tableUi.Panel->getChildByName(to_string(i)));
        _tableUi._vecImageBg.push_back(imageBg);
    }
}

void GameTableUIV88::initBottomUI() {
    auto root = CSLoader::createNode(CSB_BOTTOM);
    this->addChild(root);

    _bottomUi.Panel = static_cast<Layout*>(root->getChildByName("Panel"));
    root->setPosition(Vec2(WINSIZE.width / 2, _bottomUi.Panel->getContentSize().height*0.5f));



    _bottomUi.Text_leiJi = static_cast<Text*>(_bottomUi.Panel->getChildByName("Text_leiJi"));
    _bottomUi.Text_name = static_cast<Text*>(_bottomUi.Panel->getChildByName("Text_name"));
    _bottomUi.Text_money = static_cast<Text*>(_bottomUi.Panel->getChildByName("Text_money"));

	_bottomUi.bg = static_cast<Sprite*>(_bottomUi.Panel->getChildByName("bg"));

    //节点POS  直接在编辑器房NODE 获取坐标
    _bottomUi.bet_pos = static_cast<Node*>(_bottomUi.Panel->getChildByName("bet_pos"))->getPosition();
    _bottomUi.luzi_pos = static_cast<Node*>(_bottomUi.Panel->getChildByName("luzi_pos"))->getPosition();
    _bottomUi.repeat_pos = static_cast<Node*>(_bottomUi.Panel->getChildByName("repeat_pos"))->getPosition();
    _bottomUi.register_pos = static_cast<Node*>(_bottomUi.Panel->getChildByName("register_pos"))->getPosition();

    _bottomUi.name_bg = static_cast<Sprite*>(_bottomUi.Panel->getChildByName("name_bg"));
    _bottomUi.money_bg = static_cast<Sprite*>(_bottomUi.Panel->getChildByName("money_bg"));


    _bottomUi.Button_repeat = AUIButton::create("v66/button/blue2/6.png", "v66/button/blue2/6.png");
    _bottomUi.Button_repeat->setName("Button_repeat");
    _bottomUi.Button_repeat->setTitle("v66/button/blue1/1.png");
   // _bottomUi.Button_repeat->setTitleOffset(Vec2(-4, 4));
	_bottomUi.Button_repeat->setScale(0.7);
	_bottomUi.Button_repeat->setTitleScal(1.3);
    _bottomUi.Button_repeat->setPosition(_bottomUi.repeat_pos);
	_bottomUi.Button_repeat->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::onRepeatBetClickCallback, this));
    _bottomUi.Panel->addChild(_bottomUi.Button_repeat);

    _bottomUi.Button_repeatNo = AUIButton::create("v66/button/blue2/6.png", "v66/button/blue2/6.png");
    _bottomUi.Button_repeatNo->setName("Button_repeatNo");
	_bottomUi.Button_repeatNo->setTitle("v66/button/blue1/27.png");
    //_bottomUi.Button_repeatNo->setTitleOffset(Vec2(-4, 4));
	_bottomUi.Button_repeatNo->setScale(0.7);
	_bottomUi.Button_repeatNo->setTitleScal(1.3);
    _bottomUi.Button_repeatNo->setPosition(_bottomUi.repeat_pos);
    _bottomUi.Button_repeatNo->setVisible(false);
	_bottomUi.Button_repeatNo->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::onRepeatBetClickCallback, this));
    _bottomUi.Panel->addChild(_bottomUi.Button_repeatNo);

    _bottomUi.Button_regist = AUIButton::create("v66/button/green3/4.png", "v66/button/green3/4.png");
    _bottomUi.Button_regist->setName("Button_regist");
    _bottomUi.Button_regist->setTitle("v66/button/green1/2.png");
	_bottomUi.Button_regist->setScale(0.7);
	_bottomUi.Button_regist->setTitleScal(1.3);
//    _bottomUi.Button_regist->setTitleOffset(Vec2(-4, 4));
    _bottomUi.Button_regist->setPosition(_bottomUi.register_pos);
	_bottomUi.Button_regist->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::onApplyNTClickCallback, this));
    _bottomUi.Panel->addChild(_bottomUi.Button_regist);

	_bottomUi.Button_ZhuangList = AUIButton::create("v66/button/green3/4.png", "v66/button/green3/4.png");
    _bottomUi.Button_ZhuangList->setVisible(false);
	_bottomUi.Button_ZhuangList->setScale(0.7);
	_bottomUi.Button_ZhuangList->setTitleScal(1.3);
    _bottomUi.Button_ZhuangList->setName("Button_ZhuangList");
	_bottomUi.Button_ZhuangList->setTitle("v66/button/green1/26.png");
  //  _bottomUi.Button_ZhuangList->setTitleOffset(Vec2(-4, 4));
    _bottomUi.Button_ZhuangList->setPosition(_bottomUi.register_pos);
	_bottomUi.Button_ZhuangList->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::onApplyNTClickCallback, this));
    _bottomUi.Panel->addChild(_bottomUi.Button_ZhuangList);

}

void GameTableUIV88::initBankUI() {
    auto root = CSLoader::createNode(CSB_BANKER);
    this->addChild(root);
    _banker.panel = static_cast<Layout*>(root->getChildByName("Panel"));
	_banker.panel->setScale(0.85f);
    _banker.panel->setVisible(false);
    root->setPosition(Vec2(WINSIZE.width / 4 + 20, WINSIZE.height - _banker.panel->getContentSize().height *0.5f - 35));

    _banker.Text_name = static_cast<Text*>(_banker.panel->getChildByName("Text_name"));
    _banker.Text_money = static_cast<Text*>(_banker.panel->getChildByName("Text_money"));
    _banker.Text_leiJiMoney = static_cast<Text*>(_banker.panel->getChildByName("Text_leiJiMoney"));
    _banker.Text_zhuangCount = static_cast<Text*>(_banker.panel->getChildByName("Text_zhuangCount"));
    _banker.Sprite_head = static_cast<Sprite*>(_banker.panel->getChildByName("Sprite_head"));
	_banker.Sprite_head->setScale(0.85);
}

void GameTableUIV88::initBet() {
    m_BetRoot = BetV88::create();
    m_BetRoot->setPosition(_bottomUi.bet_pos);
    _bottomUi.Panel->addChild(m_BetRoot);
}

void GameTableUIV88::initLuZhi() {
    m_LuZhi = LuZhiV88::create();
	auto size = m_LuZhi->GetLuziLayoutSize();
	m_LuZhi->setPosition(Vec2(WIN_SIZE.width - size.width *0.5f, WIN_SIZE.height - size.height *0.5f -10));
    this->addChild(m_LuZhi,100);
}

void GameTableUIV88::initClock() {
    m_Clock = TimerObject::create();
    if (m_Clock) {
        m_Clock->setPosition(_tableUi.Panel->getChildByName("clock_pos")->getPosition());
        _tableUi.Panel->addChild(m_Clock, CLOCK_ZORDER);
    }
}

void GameTableUIV88::initNTList() {
	///////////////list

	//初始化上庄列表
	_dealerList = new BJL::T_DEALER_LIST();

	auto list_loader = CSLoader::createNode(LIST_CSB);
	list_loader->setPosition(Vec2(WIN_SIZE.width - 130, 310));
	addChild(list_loader, ZORDER_DASHBOARD + 1);
	//上庄列表
	_ntList.ImageView_ListBg = (ImageView*)list_loader->getChildByName("Image_ListBg");
	_ntList.ImageView_ListBg->setVisible(false);
	_ntList.ImageView_ListBg->setUserData((void*)false);

	_ntList.ListView_dealerList = (ListView*)_ntList.ImageView_ListBg->getChildByName("ListView_dealerList");
	_ntList.Image_item = (ImageView*)_ntList.ListView_dealerList->getChildByName("Image_item");
	_ntList.Image_item->setLocalZOrder(100);

	_ntList.Button_ApplyDown = (Button*)_ntList.ImageView_ListBg->getChildByName("Button_ApplyDown");
	_ntList.Button_ApplyDown->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::onApplyNTClickCallback, this));
	//初始化上庄列表控制器
	_dealerList->initDealerList(_ntList.ListView_dealerList, _ntList.Image_item);
}

void GameTableUIV88::createToolBarUI() {
	int zOrder = 60;
	auto toolbar = ToolBarLayer::create();
	if (toolbar) {
		toolbar->addImageAndCallBack(TOOL_BAR_EXIT, [=]() {
			if (!RoomLogic()->connected()) {
				levelGame();
			}
			else {
				if (GS_WAIT_SETGAME == _logic->getGameStatus() || GS_WAIT_AGREE == _logic->getGameStatus() || GS_WAIT_NEXT == _logic->getGameStatus() || m_isNote != true){
						_logic->sendUserUp();
					return;
				}
				// 游戏不在空闲时期，不允许离开
				//auto layer = GamePromptLayer::create();
				//layer->showPrompt(GetText("limit_leave_in_game"));
				GamePromptTip* layer = GamePromptTip::create(this, Color4B(0, 0, 0, 160));
				layer->setGamePromptTip(GetText("limit_leave_in_game"));
			}
		});
		toolbar->addImageAndCallBack(TOOL_BAR_SETTING, [=]() {
			IGameSet* gameSet = GameSetFactory::createWithVersion();
			gameSet->setBgPath("");
			gameSet->open(this, Vec2(0, 0), 10000, 0);
		});
        toolbar->addImageAndCallBack(TOOL_BAR_HELP, [=]() {
            auto helpLayer = HN::HelpLayer::create();
            helpLayer->setContentOnView("v66/common/help/nine.png");
            helpLayer->setName("helpLayer");
            helpLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            helpLayer->setOrignalPosition();
            helpLayer->moveSelfInWithAction();
            addChild(helpLayer, zOrder + 5);
        });
		toolbar->create(this, Vec2(10, WIN_SIZE.height), zOrder, 100);
	}
}

void GameTableUIV88::showToolBar(bool flag) {

	if (flag && 0 == _toolbarBg->getName().compare("0")) {
		_toolbarBg->setName("1");
		_toolbarBg->runAction(MoveBy::create(0.3, Point(0, -_toolbarBg->getContentSize().height + TOOLBALL_OFFSET_Y_V99)));
		this->getChildByTag(em_toolbarBtn)->setVisible(false);
	}
	else if (!flag && 0 == _toolbarBg->getName().compare("1")) {
		_toolbarBg->setName("0");
		_toolbarBg->runAction(CCSequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height + TOOLBALL_OFFSET_Y_V99)),
			CallFunc::create([this]() {
			this->getChildByTag(em_toolbarBtn)->setVisible(true);
		})));
	}
}

//获取下注区域颜色值
void GameTableUIV88::getAreaBetPixData() {
    char str[64] = { 0 };
    for (int i = 0; i < NUMOFBETAREA; i++) {
        sprintf(str, BRXJ_BET_AREA_PATH, i);

        Size size = Size(0, 0);
        FileUtils::getInstance()->skipPressedTexture(true);
        auto image = Tools::getImageData(str, size);
        FileUtils::getInstance()->skipPressedTexture(false);
        m_vImage.push_back(image);

        unsigned char* data = image->getData();
        _vecNoteAreaData.push_back(data);
    }
}

//下注区域总金币文本更新
void GameTableUIV88::upDataAreaTotleMoney(LLONG*quyuzhuT) {
    for (int i = 0; i < NUMOFBETAREA; i++) {
        _vecAreaTotleMoney.at(i)->setString(ForceGBKToUtf8(to_string(quyuzhuT[i]).c_str()));
    }
}
//下注区域自己金币文本更新
void GameTableUIV88::upDataAreaMoney(LLONG*quyuzhu) {
    for (int i = 0; i < NUMOFBETAREA; i++) {
        if (quyuzhu[i] > 0) {
            CCLOG("  EMasdasdasd ");
        }
        _vecAreaMoney.at(i)->setString(ForceGBKToUtf8(to_string(quyuzhu[i]).c_str()));
        _areaNoteSave[i] = quyuzhu[i];
    }
}
//显示中奖区域
void GameTableUIV88::showWinArea(BYTE idx) {
    _vecPromptWin.at(idx)->setVisible(true);
    _vecPromptWin.at(idx)->runAction(Sequence::create(Blink::create(3, 6), DelayTime::create(0.3), CallFunc::create([&]() {
        for (auto icon : _vecPromptWin)
            icon->setVisible(false);
    }), nullptr));
}

void GameTableUIV88::showNotice(const std::string& message, int nCount, int nType, bool bVisible) {
    if (bVisible) {
        if (!m_Notice) {
            m_Notice = GameNotice::create();
            m_Notice->setLocalZOrder(ZORDER_MAX - 1);
            m_Notice->setName("GameNotice");
        }
        m_Notice->openNotic(this, 10000, message, nCount, nType);
    }
}

/*---------------------gameTableUiCallBack.h---------------*/

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

void GameTableUIV88::levelGame() {
    //离开游戏的时候恢复游戏背景音乐
    HNAudioEngine::getInstance()->stopBackgroundMusic();

    //退出当前房间
    RoomLogic()->closeRoom();
    //GamePlatform::returnPlatform(ROOMLIST);
    PlatformState* state = PlatformState::create();
	state->setType(GAMELIST);
    StateManager::getInstance()->setState(state);
}

//下局离开游戏
void GameTableUIV88::nextLevelGame() {
    if (m_bNextOut) {
        _logic->sendUserUp();
    }
}

void GameTableUIV88::IShowNoticeMessage(const std::string& message) {
    //HNAudioEngine::getInstance()->playEffect(BJL_GAME_MUSIC_WARNING);

	auto notice = BJL::BJLGameNoticeV88::create(_bottomUi.Panel, message, _bottomUi.bg);
	_bottomUi.Panel->reorderChild(notice, ZORDER_MAX);
	notice->show();
}

//更新上庄列表
void GameTableUIV88::IUpdateNTList() {
    std::vector<std::string> names = _logic->requestNTList();

    std::vector<BJL::T_DEALER> ntList;
    for (auto item : names) {
        BJL::T_DEALER name;
        name.dealerName = item;
        ntList.push_back(name);
    }
    _dealerList->resetDealerList(ntList);
}

// 申请上、下庄按钮
void GameTableUIV88::IApplyNT(bool isNT) {
    _bottomUi.Button_regist->setVisible(!isNT);
    _bottomUi.Button_ZhuangList->setVisible(isNT);
    _ntList.Button_ApplyDown->setVisible(isNT);
    if (m_bAutoBet && _logic->getMySeatNo() == _logic->getBankerDeskStation()) {
        _bottomUi.Button_repeat->setVisible(true);
        _bottomUi.Button_repeatNo->setVisible(false);
    }
}

//更新玩家菜单 数据
void GameTableUIV88::upDataPlayerInfo(std::string name, LLONG money, LLONG totle) {

    _bottomUi.Text_name->setString(ForceGBKToUtf8(name.c_str()));
    _bottomUi.Text_money->setString(ForceGBKToUtf8(Tools::addComma(money).c_str()));
    _bottomUi.Text_leiJi->setString(Tools::addComma(totle));
}
//更新玩家菜单 金币数据
void GameTableUIV88::upDataPlayerInfo(LLONG money, LLONG totle) {
    _bottomUi.Text_money->setString(ForceGBKToUtf8(Tools::addComma(money).c_str()));
    _bottomUi.Text_leiJi->setString(Tools::addComma(totle));
}
//更新庄家数据
    void GameTableUIV88::upDataNTInfo(int useId,INT logoid, std::string name, LLONG money, LLONG totle, int zhuangCount) {
    char headPath[50] = { 0 };
    sprintf(headPath, "hallimg/dayday_toux/%d.png", logoid);
    _banker.Sprite_head->setTexture(headPath);
    _banker.Text_leiJiMoney->setString(Tools::addComma(totle));
    _banker.Text_money->setString(ForceGBKToUtf8(Tools::addComma(money).c_str()));
    _banker.Text_name->setString(ForceGBKToUtf8(name.c_str()));
    _banker.Text_zhuangCount->setString(to_string(zhuangCount));
    _banker.panel->setVisible(true);
}
//更新庄家金币数据
void GameTableUIV88::upDataNTInfo(LLONG money, LLONG totle) {
    _banker.Text_leiJiMoney->setString(Tools::addComma(totle));
    _banker.Text_money->setString(ForceGBKToUtf8(Tools::addComma(money).c_str()));
}
//隐藏庄家
void GameTableUIV88::hideNtInfo(bool flag) {
    _banker.panel->setVisible(flag);
}
//清理桌面
void GameTableUIV88::clearTable(bool flag) {
    _tableUi.AtlasLabel_topTotle->setVisible(flag);
    _tableUi.AtlasLabel_top->setVisible(flag);
    _tableUi.AtlasLabel_skyTotle->setVisible(flag);
    _tableUi.AtlasLabel_sky->setVisible(flag);
    _tableUi.AtlasLabel_bottomTotle->setVisible(flag);
    _tableUi.AtlasLabel_bottom->setVisible(flag);

    _tableUi.AtlasLabel_topTotle->setString("0");
    _tableUi.AtlasLabel_top->setString("0");
    _tableUi.AtlasLabel_skyTotle->setString("0");
    _tableUi.AtlasLabel_sky->setString("0");
    _tableUi.AtlasLabel_bottomTotle->setString("0");
    _tableUi.AtlasLabel_bottom->setString("0");

    for (int i = 0; i < _tableUi._vecImageBg.size(); i++) {
        _tableUi._vecImageBg.at(i)->setVisible(flag);
    }

    for (auto pocker : _vecPockers) {
        pocker->removeFromParent();
    }

    for (auto chip : _vecChips) {
        chip->removeFromParent();
    }

    for (auto dian : _vecDianSprs) {
        dian->setVisible(false);
    }

    for (auto textAtls : _vecTextAlts) {
        textAtls->setVisible(false);
    }

    for (auto baozi : _vecBaoZiSprs) {
        baozi->setVisible(false);
    }
    _vecChips.clear();
    _vecPockers.clear();

    m_BetRoot->cleanBetValue();

}



//更新路子把数
void GameTableUIV88::upDataLuzi(TGameLuzi *data) {
    m_LuZhi->updateLuzi(data);
}

//通杀通赔
void GameTableUIV88::allAnimation(bool flag) {

	auto nodeName = "tongsha";
	auto csbPath = BRXJ_TONGSHA_CSB;
	if (!flag) {
		nodeName = "tongpei";
		csbPath = BRXJ_TONGPEI_CSB;
	}

	auto node = this->getChildByName(nodeName);
	if (nullptr == node)
	{
		node = CSLoader::createNode(csbPath);
		node->setPosition(WIN_SIZE / 2.0f);
		this->addChild(node, 0, nodeName);
	}
	node->stopAllActions();

	auto action = CSLoader::createTimeline(csbPath);
	node->runAction(action);
	action->gotoFrameAndPlay(0, false);
}

//筹码动画
void GameTableUIV88::chipAnimation(bool flag, int type, int area) {
    int count = 10;
    int chip = 100;
    Point target = Vec2(0, 0);
    count = type == 1000 ? 1 : 10; // 筹码数量
    chip = type == 1000 ? 1 : type / 10; //筹码值面大小
    if (type == 5000000)
        count == 5;

    std::string flieName;
    switch (type) {
    case 100:
        flieName = "v66/chouma/7.png";
        break;
    case 1000:
		flieName = "v66/chouma/7.png";
        break;
    case 10000:
		flieName = "v66/chouma/8.png";
        break;
    case 100000:
		flieName = "v66/chouma/9.png";
        break;
    case 1000000:
		flieName = "v66/chouma/10.png";
        break;
    case 5000000:
		flieName = "v66/chouma/11.png";
        break;
    default:
        break;
    }
    auto chipSize = Sprite::create(flieName);

    auto chipArea = _vecChipsArea.at(area);
    auto bdBox = chipArea->getBoundingBox();
    auto size = chipArea->getContentSize();
    auto minx = bdBox.getMinX();
    auto maxX = bdBox.getMaxX();
    auto minY = bdBox.getMinY();
    auto maxY = bdBox.getMaxY();

    HNAudioEngine::getInstance()->playEffect(BJL_GAME_MUSIC_BET);
    for (int i = 0; i < count; i++) {
        auto chipIcon = Sprite::create(flieName);
        chipIcon->setPosition(Vec2(_bottomUi.luzi_pos.x + 100, _bottomUi.luzi_pos.y - 200 / WINSIZE.height *WINSIZE.height));
		target.y = minY + chipSize->getContentSize().height*0.5f + rand() % ((int)(maxY - minY - chipSize->getContentSize().height));
        target.x = minx + chipSize->getContentSize().width*0.5f + rand() % ((int)(maxX - minx - chipSize->getContentSize().width));
        chipIcon->setScale(0.5);
        _tableUi.Panel->addChild(chipIcon);
        _vecChips.push_back(chipIcon);
        chipIcon->runAction(MoveTo::create(0.3, target));
    }
}

void GameTableUIV88::onGameNotify(NN::tag_s2c_notify* msg) {
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

void GameTableUIV88::onGameTipExit(tag_s2c_tip_exit* msg) {
    tipMsg = msg;

    //不退出
    if (tipMsg->bIsExit == 1) {
        //要退出
        this->runAction(Sequence::createWithTwoActions(DelayTime::create(2.0f), CallFunc::create([&]() {
            tipCallback();
        })));
    }
}

//关闭自动下注
void GameTableUIV88::openRepeatNote(bool flag) {
    m_bAutoBet = flag;
}

/*-------计时器----------*/
//开始
void GameTableUIV88::starTime(int time) {
	m_Clock->setVisible(true);
	m_Clock->start(time);
}

//杀计时器
void GameTableUIV88::killTime() {
	m_Clock->stop();
	m_Clock->setVisible(false);
}

void GameTableUIV88::initCardValue(BYTE cardValus, int turn, int idx) {
    _arrayCardValues[turn][idx] = cardValus;
}

//游戏状态提醒文本
void GameTableUIV88::gameStation(BYTE station) {
    std::string str = "";
	char sprPath[50] = { 0 };
    switch (station) {
    case GS_WAIT_SETGAME: {
		str = "v66/zi/13.png";
        break;
    }
    case GS_WAIT_NEXT: {
		str = "v66/zi/18.png";
        break;
    }
    case GS_NOTE_STATE: {
		str = "v66/zi/16.png";
        break;
    }
    case GS_SEND_CARD: {
		str = "v66/zi/8.png";
        break;
    }
    case GS_SHOW_WIN: {
		str = "v66/zi/7.png";
        break;
    }
    default:
        break;
    }
	_tableUi.Sprite_gameStation->setTexture(str);
}

void GameTableUIV88::ISendCard(bool animate) {
    if (animate) {
        //	_resultCard->initNewGame();
        //_resultCard->showCardBorad();
		this->schedule(schedule_selector(GameTableUIV88::dispatchcard), 0.5);
    } else {

        auto angle = _tableUi.puke->getRotation();
        auto scal = _tableUi.puke->getScale();
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 2; j++) {
                auto pos = _vecCardPos.at(i);
                auto pocker = PockerV88::create(Point(pos.x + j*CARD_INTERVAL, pos.y), scal, Point(pos.x + j*CARD_INTERVAL, pos.y), 0, _arrayCardValues[i][j]);
                _tableUi.Panel->addChild(pocker);
                _vecPockers.push_back(pocker);
            }
        }
    }
}

void GameTableUIV88::showSettlementDialog(S_C_GameResult* pData) {
    CCAssert(pData != nullptr, "nullptr.");

    auto coin = FloatCoin::create();
    if (coin) {
        coin->openParent(this, Vec2(150, 100), ZORDER_MAX);
        coin->updateCoin(pData->i64UserScore);
    }
    
    //结算框
    auto gameResult = BRNN::BRNNSettlementLayerV88::create();
    this->addChild(gameResult, ZORDER_MAX + 1);
    gameResult->setPosition(WIN_SIZE / 2);

    char str[64] = { 0 };
    sprintf(str, "%lld", pData->i64NtScore);
    gameResult->setBankerMoney(str);
    sprintf(str, "%lld", pData->i64UserScore);
    gameResult->setMyselfMoney(str);

    if (pData->i64UserScore > 0) {
        HNAudioEngine::getInstance()->playEffect(BJL_GAME_MUSIC_WIN);
    } else if (pData->i64UserScore < 0) {
        HNAudioEngine::getInstance()->playEffect(BJL_GAME_MUSIC_LOSE);
    } else {

    }
}

/* ------------------------------------------*/
void GameTableUIV88::dispatchcard(float dt) {
    if (_pockerIdx == 4) {
        _pockerIdx = 0;
        _cardTurn += 1;
    }

    if (_cardTurn == 2) {
        _pockerIdx = 0;
        _cardTurn = 0;
		unschedule(schedule_selector(GameTableUIV88::dispatchcard));
        return;
    }

    auto angle = _tableUi.puke->getRotation();
    auto scal = _tableUi.puke->getScale();
    auto pos = _vecCardPos.at(_pockerIdx);
    auto orign = _tableUi.puke->getPosition();

    auto pocker = PockerV88::create(orign, scal, Point(pos.x + _cardTurn*CARD_INTERVAL, pos.y), angle, _arrayCardValues[_pockerIdx][_cardTurn]);
    _tableUi.Panel->addChild(pocker);
    _vecPockers.push_back(pocker);
    pocker->moveTagertPos();
    pocker->runAction(Spawn::create(MoveTo::create(0.15, Point(pos.x + _cardTurn*CARD_INTERVAL, pos.y)), RotateTo::create(0.15, 0.0), ScaleTo::create(0.15, 1.0f),DelayTime::create(0.5f), CallFunc::create([&]() {
        auto cardShapes = _logic->GetCardShapes();
        if (_cardTurn == 1) {
            showCardShap(_pockerIdx - 1, cardShapes[_pockerIdx - 1]);
            char effectMusicPath[50] = { 0 };
            sprintf(effectMusicPath, "v66/music/BRXJ/%dd.ogg", cardShapes[_pockerIdx - 1]);
            HNAudioEngine::getInstance()->playEffect(effectMusicPath);
        }
    }), nullptr));
    _pockerIdx += 1;
}

//置灰下注按钮
void GameTableUIV88::grayEffectBet(bool flag) {
    m_BetRoot->grayEffectBet(flag);
}

//显示牌型点数
void GameTableUIV88::showCardShap(int area, int num) {
    if (num != 10) {
        _vecTextAlts.at(area)->setString(to_string(num));
        _vecTextAlts.at(area)->setVisible(true);
        _vecDianSprs.at(area)->setVisible(true);
    } else {
        _vecBaoZiSprs.at(area)->setVisible(true);
    }
}

//恢复牌
void GameTableUIV88::recoveryCards(BYTE value) {

}

//播放阶段音效
void GameTableUIV88::playEffectOnStation() {
    switch (_logic->getGameStatus()) {
    case GS_NOTE_STATE: {
        HNAudioEngine::getInstance()->playEffect(BJL_GAME_MUSIC_TIP_BET);
        HNAudioEngine::getInstance()->playBackgroundMusic(BJL_GAME_MUSIC_BET_BACK);
        break;
    }
    case GS_SHOW_WIN: {
        HNAudioEngine::getInstance()->stopBackgroundMusic(BJL_GAME_MUSIC_BET_BACK);
        break;
    }
    default: {
        break;
    }
    }
}

//重复下注
void GameTableUIV88::repeatNote() {
    LLONG money[] = { 1000, 10000, 100000, 1000000, 5000000 };
    if (m_bAutoBet) {
        for (int i = 0; i < 3; i++) {
            auto qian = 0;
            auto wan = 0;
            auto shiwan = 0;
            auto baiwan = 0;
            auto wubaiwan = 0;

            if (_areaNoteSave[i] != 0) {
                if (_areaNoteSave[i] < money[1]) { //x < 1w
                    qian = _areaNoteSave[i] / money[0];
                } else if (_areaNoteSave[i] >= money[1] && _areaNoteSave[i] < money[2]) { //1w< X < 10W
                    wan = (int)_areaNoteSave[i] / money[1];
                    qian = (_areaNoteSave[i] - wan * money[1]) / money[0];
                } else if (_areaNoteSave[i] >= money[2] && _areaNoteSave[i] < money[3]) { //   10w< X < 100W
                    shiwan = (int)_areaNoteSave[i] / money[2];
                    wan = (_areaNoteSave[i] - shiwan * money[2]) / money[1];
                    qian = (_areaNoteSave[i] - shiwan * money[2] - wan * money[1]) / money[0];			//100w< X < 500W
                } else if (_areaNoteSave[i] >= money[3] && _areaNoteSave[i]  < money[4]) { //x >= 500W
                    baiwan = (int)_areaNoteSave[i] / money[3];
                    shiwan = (_areaNoteSave[i] - baiwan * money[3]) / money[2];
                    wan = (_areaNoteSave[i] - shiwan * money[2] - baiwan * money[3]) / money[1];
                    qian = (_areaNoteSave[i] - shiwan * money[2] - baiwan * money[3] - wan*money[1]) / money[0];
                } else if (_areaNoteSave[i] >= money[4]) {
                    wubaiwan = (int)_areaNoteSave[i] / money[4];
                    baiwan = (_areaNoteSave[i] - wubaiwan * money[4]) / money[3];
                    shiwan = (_areaNoteSave[i] - baiwan * money[3] + wubaiwan * money[4]) / money[2];
                    wan = (_areaNoteSave[i] - shiwan * money[2] - baiwan * money[3] - wubaiwan*money[4]) / money[1];
                    qian = (_areaNoteSave[i] - shiwan * money[2] - baiwan * money[3] - wubaiwan*money[4] - wan*money[1]) / money[0];
                }
            }
            if (qian != 0) {
                for (int j = 0; j < qian; j++)
                    _logic->requestBet(0, i);
            }
            if (wan != 0) {
                for (int j = 0; j < wan; j++)
                    _logic->requestBet(1, i);
            }
            if (shiwan != 0) {
                for (int j = 0; j < shiwan; j++)
                    _logic->requestBet(2, i);
            }
            if (baiwan != 0) {
                for (int j = 0; j < baiwan; j++)
                    _logic->requestBet(3, i);
            }
            if (wubaiwan != 0) {
                for (int j = 0; j < wubaiwan; j++)
                    _logic->requestBet(4, i);
            }
        }
    }
}

/*-----------------------回调--------------------------*/

void GameTableUIV88::onNoteAreaCallBack(Ref* pShender, Widget::TouchEventType  type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }

    if (_logic->getMySeatNo() == _logic->getBankerDeskStation()) {
        IShowNoticeMessage(ForceGBKToUtf8("庄家不能做此操作！"));
        return;
    }

    if (m_bAutoBet) {
        IShowNoticeMessage(ForceGBKToUtf8("已自动下注,如果加注请取消自动下注!"));
        return;
    }

    if (GS_NOTE_STATE != _logic->getGameStatus()) {
        IShowNoticeMessage(ForceGBKToUtf8("未到下注时间!"));
        return;
    }

    if (m_BetRoot->getCurSeleteBet() < 0 || m_BetRoot->getCurSeleteBet() > (CHOUMANUM - 1)) {
        IShowNoticeMessage(ForceGBKToUtf8("请选择下注筹码!"));
        return;
    }

    //区域
    int iIndex = 0;
    for (int i = 0; i < NUMOFBETAREA; i++) {
        if (pShender == _vecNoteArea.at(i)) {
            iIndex = i;
            CCLOG(" Area ============  %d", i);
            break;
        }
    }

    ImageView* image = (ImageView*)pShender;
    Vec2 point = image->getTouchEndPosition();
    Vec2 pos = image->convertToNodeSpace(point);
    Size size = _vecNoteArea.at(iIndex)->getContentSize();
    Color4B pColor = Tools::getPixelColor(_vecNoteAreaData.at(iIndex), pos, size);

    //透明区域
    bool bValid = false;
    for (int i = 0; i < 4; i++) {
        if (pColor.r != 0 || pColor.g != 0 || pColor.b != 0 || pColor.a != 0) {
            bValid = true;
            break;
        }
    }

    //非有效区域跳出
    if (!bValid) {
        return;
    }

    // 请求下注
    _logic->requestBet(m_BetRoot->getCurSeleteBet(), iIndex);
    m_isNote = true;
    LLONG money[5] = { 1000, 10000, 100000, 1000000, 5000000 };
    //取消重复下注
    if (m_bAutoBet) {
        m_bAutoBet = false;
        memset(_areaNoteSave, 0, sizeof(_areaNoteSave));
        _bottomUi.Button_repeat->setVisible(true);
        //	_bottomUi->Button_CancelPrevBet->setVisible(false);
    }
}


void GameTableUIV88::onRepeatBetClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype) {
    if (touchtype != Widget::TouchEventType::ENDED) return;

    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);
    auto curSeletedBtn = static_cast<Button*>(pSender);
    auto name = curSeletedBtn->getName();
    if (name =="Button_repeat") {
        if (_logic->getMySeatNo() == _logic->getBankerDeskStation()) {
            IShowNoticeMessage(ForceGBKToUtf8("庄家不能做此操作！"));
            return;
        }

        bool isNote = false;
        for (auto money : _areaNoteSave) {
            if (money > 0)
                isNote = true;
        }

        if (isNote) {
            IShowNoticeMessage(ForceGBKToUtf8("下局生效！"));
        } else {
            IShowNoticeMessage(ForceGBKToUtf8("您上局没有投注！"));
            return;
        }
        //         if (_logic->_gameStatus == GS_NOTE_STATE) {
        //             IShowNoticeMessage(GBKToUtf8("下局生效！"));
        //         }
        //if (m_vPrevBetArea2.size() == 0 && m_vPrevBetMoney2.size() == 0)
        //{
        //	if (m_vPrevBetArea1.size() == 0 && m_vPrevBetMoney1.size() == 0)
        //	{
        //		IShowNoticeMessage(GBKToUtf8("您上局没有投注！"));
        //		return;
        //	}
        //}

        m_bAutoBet = true;

        _bottomUi.Button_repeat->setVisible(false);
        _bottomUi.Button_repeatNo->setVisible(true);
        m_BetRoot->cleanBetValue();
        grayEffectBet(false);
    } else if (name == "Button_repeatNo") {
        m_bAutoBet = false;
        IShowNoticeMessage(ForceGBKToUtf8("取消重复下局投注！"));
        _bottomUi.Button_repeat->setVisible(true);
        _bottomUi.Button_repeatNo->setVisible(false);
        if (_logic->getGameStatus() == GS_NOTE_STATE)
            grayEffectBet(true);
    }
}

void GameTableUIV88::tipCallback() {
    if (tipMsg->bIsExit == 1) {

        auto layer = GamePromptLayer::create();
        //layer->setCloseExitBtn();
//         char str[128] = { 0 };
//         int sec = 3;
//         sprintf(str, "金币数额低于%lld，%ds后离开游戏！", tipMsg->i64Money, sec);
//         layer->showPrompt(GBKToUtf8(str));
        int sec = 3;
        char s[128] {0};
        std::string str;
        str.append(GBKToUtf8("金币数额低于"));
        sprintf(s, "%lld", tipMsg->i64Money);
        str.append(s);
        sprintf(s, ",%d", sec);
        str.append(s);
        str.append(GBKToUtf8("s后离开游戏！"));
        layer->showPrompt(str);

    /*    layer->setCallBack([&]() {
			unschedule(schedule_selector(GameTableUIV88::updateTip));
            _logic->sendUserUp();
        });*/
        layer->setColorLayerTouchEnabled(false);

        m_i64TipMoney = tipMsg->i64Money;
        m_TextTip = layer->getPromptText();
        m_TextTip->setTag(sec);
		schedule(schedule_selector(GameTableUIV88::updateTip), 1.0f);

        //把重复下注关了,防止下注快，发离开消息不成功
        m_bAutoBet = false;
    } else {

    }
}
void GameTableUIV88::girlAnimation(float dt) {
    auto index = 0;
    index = rand() % 5;
    switch (index) {
    case 1:
        _antAction->play("danyanzha", false);
        break;
    case 2:
        _antAction->play("zhayan", false);
        break;
    case 3:
        _antAction->play("please", false);
        break;
    case 4: {
        // 						   CCCallFunc* fun0 = CCCallFunc::create([&](){
        // 							   _antAction->play("kiss", loop);
        // 						   });
        //
        // 						   CCCallFunc* fun1 = CCCallFunc::create([&](){
        // 							   _antAction->play("kiss", loop);
        // 						   });
        // 						   CCCallFunc* fun2 = CCCallFunc::create([&](){
        // 							   _antAction->play("kiss", loop);
        // 						   });
        // 						   _gameTableUi->runAction(Sequence::create(fun0, DelayTime::create(1.0f), fun1, DelayTime::create(1.0f), fun2, nullptr));
        // 						   break;
        _antAction->play("kiss", false);
        break;
    }
    case 5: {
        _antAction->play("qiaoji", false);
        break;
    }
    default:
        break;
    }
}

void GameTableUIV88::updateTip(float dt) {
    int iTag = m_TextTip->getTag();
    if (iTag == 0) {
		unschedule(schedule_selector(GameTableUIV88::updateTip));
        _logic->sendUserUp();
        return;
    }

//     char str[128] = { 0 };
//     iTag -= 1;
//     sprintf(str, "金币数额低于%lld，%ds后离开游戏！", m_i64TipMoney, iTag);
//     m_TextTip->setString(GBKToUtf8(str));
//     m_TextTip->setTag(iTag);
    iTag -= 1;

    char s[128] {0};
    std::string str;
    str.append(GBKToUtf8("金币数额低于"));
    sprintf(s, "%lld", m_i64TipMoney);
    str.append(s);
    sprintf(s, ",%d", iTag);
    str.append(s);
    str.append(GBKToUtf8("s后离开游戏！"));
    m_TextTip->setString(str);
    m_TextTip->setTag(iTag);
}

void GameTableUIV88::onApplyNTClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype) {
    if (touchtype != Widget::TouchEventType::ENDED) {
        return;
    }
    auto curSelectBtn = static_cast<Button*>(pSender);
    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);

    // 申请上庄
    if (curSelectBtn->getName() == "Button_regist") {
        _logic->requestBeNT(true);
    }

    // 申请下庄
    else if (curSelectBtn->getName() == "Button_ApplyDown") {
        _logic->requestBeNT(false);
        _ntList.ImageView_ListBg->setVisible(false);
        _ntList.ImageView_ListBg->setUserData((void*)false);
    }

    if (curSelectBtn->getName() == "Button_ZhuangList") {
        _ntList.ImageView_ListBg->stopAllActions();
        if (_ntList.ImageView_ListBg->getUserData() == (void*)false) {
            _ntList.ImageView_ListBg->setVisible(true);
            _ntList.ImageView_ListBg->setUserData((void*)true);
            _ntList.ImageView_ListBg->runAction(CCSequence::createWithTwoActions(DelayTime::create(10.0f), CallFunc::create([this]() {
                _ntList.ImageView_ListBg->setVisible(false);
                _ntList.ImageView_ListBg->setUserData((void*)false);
            })));
        } else {
            _ntList.ImageView_ListBg->setVisible(false);
            _ntList.ImageView_ListBg->setUserData((void*)false);
        }
    }
}

void GameTableUIV88::onBtnClick(Ref* pSender, Control::EventType event) {
    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);

    Node* btn = (Node*)pSender;
    switch (btn->getTag()) {
    case em_toolbarBtn: {
      
        break;
    }
    case em_toolbarSetBtn: {
//        if (this->getChildByName("setLayer")) {
//            (dynamic_cast<BRNNSetLayerV88*>(this->getChildByName("setLayer")))->closeSet();
//            return;
//        }
//        auto setLayer = BRNNSetLayerV88::create();
//        setLayer->setName("setLayer");
//        setLayer->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2);
//        setLayer->showSet(this, ZORDER_MAX + 50);
        break;
    }

    case  em_toolbarHelpBtn: {
        IShowNoticeMessage(GBKToUtf8("开发中"));
        break;
    }
    case em_toolbarLeaveBtn: {
        if (GS_NOTE_STATE == _logic->getGameStatus() || GS_SEND_CARD == _logic->getGameStatus()) {
            bool hasBet = false;
            for (int i = 0; i < NUMOFBETAREA; i++) {
                if (_areaNoteSave[i] > 0) {
                    hasBet = true;
                    break;
                }
            }
            if (hasBet || (_logic->getMySeatNo() == _logic->getBankerDeskStation())) {
                IShowNoticeMessage(ForceGBKToUtf8("正在游戏中，禁止退出。"));
                return;
            }
        }
        _logic->requestStandUp();
        break;
    }
    case em_toolbarNextOutBtn: {
        if (btn->getUserData() == (void*)true) {
            btn->setUserData((void*)false);
            btn->getChildByName("checkSprite")->setVisible(false);
            m_bNextOut = false;
        } else if (btn->getUserData() == (void*)false) {
            btn->setUserData((void*)true);
            btn->getChildByName("checkSprite")->setVisible(true);
            m_bNextOut = true;
        }

        break;
    }
    }
}

void GameTableUIV88::onMenuBtnCallBack(Ref* pShender, Widget::TouchEventType  type) {

}


void  GameTableUIV88::onTouchEnded(Touch* touch, Event* event) {
    Point point = touch->getLocation();
    point = _tableBg->convertToNodeSpace(point);
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
