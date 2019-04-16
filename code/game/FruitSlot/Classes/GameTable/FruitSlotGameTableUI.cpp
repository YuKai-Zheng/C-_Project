#include "../../../../code/common/LibHNLobby/HNLobby/GameNotice.h"
#include "HNLobby/v66/ExtraReward.h"
#include "FruitSlotGameTableUI.h"

#include "HNLobby/state/PlatformState.h"
#include "HNLobby/pattern/StateManager.h"
#include "HNLobby/PlatformDefine.h"

#include "HNLobby/globel.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "HNLobby/pattern/factory/gameSet/IGameSet.h"
#include "HNLobby/pattern/factory/gameSet/GameSetFactory.h"

#include "HNCommon/HNConverCode.h"
#include "UI/Scanner.h"
#include "UI/HelpLayer.h"
#include "UI/ToolBarLayer.h"
#include "UI/FloatCoin.h"
#include "HNLobby/pattern/CommandManager.h"
#include "spine/spine.h"
#include <spine/spine-cocos2dx.h>
#include "HNLobby/GameData.h"
#define ICON_HEIGHT_FRUIT 138
#define FRUIT_SLOT_BGM					"Games/31000504/music/slotBgm.mp3"
#define FRUIT_SLOT_START_EFFECT			"Games/31000504/music/fruitStart.mp3"
#define FRUIT_SLOT_TURN_EFFECT			"Games/31000504/music/fruitTurn.mp3"
#define FRUIT_SLOT_TURN_DOWN_EFFECT		"Games/31000504/music/fruitTurnDown.mp3"
#define FRUIT_SLOT_SCORE_WIN_EFFECT		"Games/31000504/music/fruitScoreWin.mp3"
#define FRUIT_SLOT_SCORE_LOSE_EFFECT	"Games/31000504/music/fruitScoreLose.mp3"
#define FRUIT_SLOT_WIN_EFFECT			"Games/31000504/music/win.mp3"
#define FRUIT_SLOT_LOSE_EFFECT			"Games/31000504/music/lose.mp3"

#define FRUIIT_SLOT_APPLE				"Games/31000504/music/fruit_apple.mp3"
#define FRUIIT_SLOT_WATERMELON			"Games/31000504/music/fruit_watermelon.mp3"
#define FRUIIT_SLOT_RING				"Games/31000504/music/fruit_ring.mp3"
#define FRUIIT_SLOT_SEVEN				"Games/31000504/music/fruit_seven.mp3"
#define FRUIIT_SLOT_BAR					"Games/31000504/music/fruit_bar.mp3"

#define FRUIT_SLOT_DOUBLE_WIN_ANI		"v88/animation/slots/fruitslots/fruitslots_win/fruitslots_win.csb"
#define FRUIT_SLOT_DOUBLE_LOSE_ANI		"v88/animation/slots/fruitslots/fruitslots_lose/fruitslots_lose.csb"

#define FRUIT_SLOT_DOUBLE_RESULT_NUM_ANI	"v88/animation/slots/fruitslots/fruitslots_sizetip/fruitslots_sizetip.csb"

namespace FruitSlot {

    const static char* kIconTypeTpl = "Games/31000504/ui/icon%d.png";
    
HNGameUIBase* GameTableUI::create(BYTE bDeskIndex, bool bAutoCreate) {
	GameTableUI* tableUI = new GameTableUI();
	if (tableUI && tableUI->init(bDeskIndex, bAutoCreate)) {
		tableUI->autorelease();
	}
	else {
		delete tableUI;
		tableUI = nullptr;
	}
	return tableUI;
}

GameTableUI::GameTableUI() {
	cleanData();
}

GameTableUI::~GameTableUI() {
	CC_SAFE_DELETE(_GameTableLogic);
}

bool GameTableUI::init(BYTE bDeskIndex, bool bAutoCreate) {
	if (!HNGameUIBase::init()) {
		return false;
	}

	m_bDeskIndex = bDeskIndex;
	m_bAutoCreate = bAutoCreate;

	return true;
}

void GameTableUI::leaveTheGame() {
	HNRoomLogic::getInstance()->closeRoom();
	PlatformState* state = PlatformState::create();
	state->setType(ROOMLIST);
	StateManager::getInstance()->setState(state);
}

void GameTableUI::onEnter() {
	HNGameUIBase::onEnter();
	HNAudioEngine::getInstance()->playBackgroundMusic(FRUIT_SLOT_BGM, true);
    
    ImageView* bg = ImageView::create("Games/31000504/ui/bg.png");
    if (bg) {
        bg->setPosition(WIN_SIZE/2);
        addChild(bg);
    }
	m_csb = CSLoader::createNode("Games/31000504/Node_root.csb");
	m_csb->setPosition(WIN_SIZE / 2);
	addChild(m_csb);
    
    m_csbPanel = dynamic_cast<Layout*>(m_csb->getChildByName("Panel_bg"));
    
	loadButton();
	loadText();
	loadTurn();
	loadDoublePanel();
	createToolBarUI();

	_GameTableLogic = new GameTableLogic(this, m_bDeskIndex, m_bAutoCreate, PLAYER_COUNT);
	_GameTableLogic->enterGame();
    
    runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([&] () {
        auto scanner = Scanner::create();
        this->addChild(scanner, 100000);
        scanner->starPlayerEffect();
        scanner->setPosition(WIN_SIZE.width-60, 180);
    }), nullptr));
}

void GameTableUI::onExit() {
	HNGameUIBase::onExit();
    unschedule(schedule_selector(GameTableUI::jackpotUpdate));
}


void GameTableUI::onExitTransitionDidStart() {
	HNGameUIBase::onExitTransitionDidStart();
	HNAudioEngine::getInstance()->stopBackgroundMusic();
}

void GameTableUI::addUser(BYTE seatNo, bool bMe) {
}

//玩家坐下(非自己)
void GameTableUI::userSitDown(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
}

//玩家站起
void GameTableUI::userUp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
}


void GameTableUI::onGameTipExit(tag_s2c_tip_exit* msg)
{

}

void GameTableUI::onHandleNewMessage(int iType, const std::string &message, int iCount)
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
void GameTableUI::showNotice(const std::string& message, int nCount, int nType, bool bVisible) {
    if (bVisible) {
        if (!m_Notice) {
            m_Notice = GameNotice::create();
            m_Notice->setLocalZOrder(10000);
            m_Notice->setName("GameNotice");
        }
        m_Notice->openNotic(this, 10000, message, nCount, nType);
    }
}
//游戏状态
void GameTableUI::onGameState()
{
	//还需要非退出情况
	if (_GameTableLogic->_gameStatus == GS_FREE && m_bAuto)
	{
		runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([&](){
			if (!m_bAuto) return;
			startBtnCallback(m_btnStart, Widget::TouchEventType::ENDED);
		}), nullptr));
	}
}

void GameTableUI::onGameBaseInfo(struct_s2c_base* msg)
{
	//初始化场次
	m_i64BetUnit = msg->i64NoteUnit;

	char str[128] = { 0 };
	int num = 0;
	if (m_i64BetUnit >= 1000 && m_i64BetUnit < 10000)
	{
		num = m_i64BetUnit / 1000;
		sprintf(str, "%d千场", num);
	}
	else if (m_i64BetUnit >= 10000)
	{
		num = m_i64BetUnit / 10000;
		sprintf(str, "%d万场", num);
	}

	m_textBetUnit->setString(str);

	//
    m_i64MyMoney = msg->i64MyMoney;
	m_textMyMoney->setString(to_string(msg->i64MyMoney));
    
    m_i64Jackpot = GameData::getInstance()->getRoomJackpotData();
    schedule(schedule_selector(GameTableUI::jackpotUpdate), 0.33f);
}

//游戏开始
void GameTableUI::onGameStart(struct_s2c_startGame* msg)
{
    cleanResults();
    
	//更新钱
    m_i64MyMoney = msg->i64MyMoney;
	m_textMyMoney->setString(to_string(msg->i64MyMoney));
	//转动数据
    m_vOpenLotteryType.clear();
	m_vTurnUnitCount.clear();
	for (int i = 0; i < LOTTERY_COUNT_FRUIT; i++)
	{
        m_vOpenLotteryType.push_back(msg->nOpenLotteryType[i]);
		m_vTurnUnitCount.push_back(msg->nTurnUnitCount[i]);
	}
	m_fTurnUnitTime = msg->fOpenTurnUnitTime;
	m_nOpenShowTime = msg->nOpenShowTime;

	//3个lottery
	//msg->nOpenLotteryType;

	//开的类型
	m_nOpenLineType = msg->nOpenLineType;

	//启动转盘
	m_nHasTurnUnitCount = 0;
	schedule(schedule_selector(GameTableUI::turnUpdate), m_fTurnUnitTime);
	
	if (m_nTurnMusicID > 0)
	{
		HNAudioEngine::getInstance()->stopEffectWithID(m_nTurnMusicID);
	}
	m_nTurnMusicID = HNAudioEngine::getInstance()->playEffect(FRUIT_SLOT_TURN_EFFECT, true);
}

//是否猜大小
void GameTableUI::onGameDoCaiDaXiao(struct_s2c_doCaiDaXiao* msg)
{
	if (!msg->nFlag) return;

	//刷新得分
	m_textScoreMoney->setString(to_string(msg->i64WinMoney));

	//提示下分或比倍操作
	m_imageScoreFrame->setVisible(false);
	setBink(m_imageScoreFrame, true);
	m_imageDoubleFrame->setVisible(false);
	setBink(m_imageDoubleFrame, true);

	m_btnDouble->setTouchEnabled(true);
	m_btnDouble->setBright(true);
	m_btnScore->setTouchEnabled(true);
	m_btnScore->setBright(true);

	//是否托管
	runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([&](){
		if (!m_bAuto) return;
		doubleBtnCallback(m_btnScore, Widget::TouchEventType::ENDED);
	}), nullptr));
}

//猜大小
void GameTableUI::onGameCaiDaXiao(struct_s2c_caiDaXiao* msg)
{
	if (!msg->nFlag) return;

	//比倍界面
	if (!m_panelDouble->isVisible())
	{
		m_panelDouble->setVisible(true);
		m_panelDouble->setTouchEnabled(true);
		m_textOpenPoint->setString("0");
	}
	else
	{
		////提示下分或比倍操作
		//m_imageScoreFrame->setVisible(false);
		//setBink(m_imageScoreFrame, true);
		//m_imageDoubleFrame->setVisible(false);
		//setBink(m_imageDoubleFrame, true);
	}

	m_btnSmall->setTouchEnabled(true);
	m_btnBig->setTouchEnabled(true);

	//数字跳动
	setNumJump(true);
}

//猜大小结算
void GameTableUI::onGameCaiResult(struct_s2c_caiResult* msg)
{
	//特效
	showCaiResultEffect(msg->nSuccess == 1, msg->nPoint);

	//停止数字跳动
	setNumJump(false);

	//开点数
	m_textOpenPoint->setString(to_string(msg->nPoint));

	//输赢金币
	m_textScoreMoney->setString(to_string(msg->i64WinMoney));

	//阶段时间
	msg->fTime;
}

//游戏结算结束
void GameTableUI::onGameFinish(struct_s2c_caiFinish* msg)
{
    if (msg == nullptr) {
        return;
    }
    //
    auto coin = FloatCoin::create();
    if (coin) {
        coin->openParent(this, Vec2(WIN_SIZE.width*0.5f, WIN_SIZE.height*0.5f+235), 100);
        coin->updateCoin(msg->i64WinMoney);
    }
	//音效
	if (msg->i64WinMoney > 0)
	{
		HNAudioEngine::getInstance()->playEffect(FRUIT_SLOT_WIN_EFFECT);
	}
	else if (msg->i64WinMoney < 0)
	{
		HNAudioEngine::getInstance()->playEffect(FRUIT_SLOT_LOSE_EFFECT);
	}

	if (m_panelDouble->isVisible())
	{
		m_panelDouble->setVisible(false);
	}

	//刷新金币
    m_i64MyMoney = msg->i64MyMoney;
	m_textMyMoney->setString(to_string(msg->i64MyMoney));

	//输赢金币
	m_textScoreMoney->setString(to_string(msg->i64WinMoney));
	//m_textScoreMoney->setString("0");

	//设置按钮
	setBtnsTouchEnabled(true);
}
void GameTableUI::jackpotUpdate(float dt)
{
        
    m_i64Jackpot += random(50,100);
    if (m_textPoolMoney) {
        m_textPoolMoney->setString(Tools::addComma(m_i64Jackpot));
    }
    
}
void GameTableUI::autoBtnCallback(Ref* pSender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;

    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
	int nTag = (dynamic_cast<Button*>(pSender))->getTag();

	m_bAuto = (nTag == 0);

	//托管时
	if (_GameTableLogic->_gameStatus == GS_FREE && m_bAuto)
	{
        //下注额不为0
		if (m_nCurBetMul <= 0)
		{
			CommandManager::getInstance()->showGamePrompt(GBKToUtf8(("请下注")));
			return;
		}

		//金额不足
		long long i64NeedMoney = m_i64BetUnit * m_nCurBetMul;
		if (m_i64MyMoney < i64NeedMoney)
		{
			CommandManager::getInstance()->showGamePrompt(GetText("SEND_PACKET_ERROR_1"));
			//CommandManager::getInstance()->showGamePrompt(GBKToUtf8("金币不足"));
			return;
		}

		//调开始按钮
		startBtnCallback(m_btnStart, Widget::TouchEventType::ENDED);
	}
	else
	{
		HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
	}

	m_btnAuto->setVisible(!(nTag == 0));
	m_btnAuto->setTouchEnabled(!(nTag == 0));

	m_btnCancel->setVisible(!(nTag == 1));
	m_btnCancel->setTouchEnabled(!(nTag == 1));
}

void GameTableUI::betBtnCallback(Ref* pSender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;

	int nTag = (dynamic_cast<Button*>(pSender))->getTag();

    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
	if (nTag == 1) //加注
	{
		//最大3倍
		m_nCurBetMul++;
		if (m_nCurBetMul == 4)
		{
			m_nCurBetMul = 1;
		}
		m_textBetMul->setString(to_string(m_nCurBetMul));
	}
	else if (nTag == 3) //最大注
	{
		m_nCurBetMul = nTag;
		m_textBetMul->setString(to_string(m_nCurBetMul));

		//调开始按钮
		//startBtnCallback(m_btnStart, Widget::TouchEventType::ENDED);
	}

	//显示押注档次高亮
	//showBetAreaBright(m_nCurBetMul);
}

void GameTableUI::startBtnCallback(Ref* pSender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;

	HNAudioEngine::getInstance()->playEffect(FRUIT_SLOT_START_EFFECT);

	//状态不对
	if (_GameTableLogic->_gameStatus != GS_FREE)
	{
		return;
	}

	//下注额不为0
	if (m_nCurBetMul <= 0)
	{
		CommandManager::getInstance()->showGamePrompt(GBKToUtf8(("请下注")));
		return;
	}

	//金额不足
	long long i64NeedMoney = m_i64BetUnit * m_nCurBetMul;
	if (m_i64MyMoney < i64NeedMoney)
	{
		CommandManager::getInstance()->showGamePrompt(GetText("SEND_PACKET_ERROR_1"));
		//CommandManager::getInstance()->showGamePrompt(GBKToUtf8("金币不足"));
		return;
	}

	setBtnsTouchEnabled(false);
	//调拉拔动画
    m_barAction->gotoFrameAndPlay(0,false);
    m_textScoreMoney->setString("0");
}

void GameTableUI::doubleBtnCallback(Ref* pSender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;

	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);

	int nTag = (dynamic_cast<Button*>(pSender))->getTag();

	//猜大小下分选一个
	if (_GameTableLogic->_gameStatus == GS_SELECT_GUESS_MODEL)
	{
		sendDoCaiDaXiao(nTag);

		//关闭提醒
		setBink(m_imageDoubleFrame, false);
		m_imageDoubleFrame->setVisible(false);
		setBink(m_imageScoreFrame, false);
		m_imageScoreFrame->setVisible(false);
		return;
	}

	//不继续猜大小下分
	if (_GameTableLogic->_gameStatus == GS_GUESS_MODEL)
	{
		sendCaiDaXiao(nTag, 0);
		return;
	}
}

void GameTableUI::guessBtnCallback(Ref* pSender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;

	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);

	if (_GameTableLogic->_gameStatus != GS_GUESS_MODEL) return;

	int nTag = (dynamic_cast<Button*>(pSender))->getTag();

	sendCaiDaXiao(1, nTag);

	m_btnSmall->setTouchEnabled(false);
	m_btnBig->setTouchEnabled(false);

	//关闭提醒
	//setBink(m_imageDoubleFrame, false);
	//m_imageDoubleFrame->setVisible(false);
	//setBink(m_imageScoreFrame, false);
	//m_imageScoreFrame->setVisible(false);
}

void GameTableUI::loadButton()
{
    if (m_csbPanel) {
        m_btnAuto = dynamic_cast<Button*>(m_csbPanel->getChildByName("Button_auto"));
        m_btnAuto->setTouchEnabled(true);
        m_btnAuto->setVisible(true);
        m_btnAuto->setTag(0);
        m_btnAuto->addTouchEventListener(CC_CALLBACK_2(GameTableUI::autoBtnCallback, this));
        
        m_btnCancel = dynamic_cast<Button*>(m_csbPanel->getChildByName("Button_cancel"));
        m_btnCancel->setTouchEnabled(false);
        m_btnCancel->setVisible(false);
        m_btnCancel->setTag(1);
        m_btnCancel->addTouchEventListener(CC_CALLBACK_2(GameTableUI::autoBtnCallback, this));
        
        m_btnBet = dynamic_cast<Button*>(m_csbPanel->getChildByName("Button_bet"));
        m_btnBet->setTouchEnabled(true);
        m_btnBet->setVisible(true);
        m_btnBet->setTag(1);
        m_btnBet->addTouchEventListener(CC_CALLBACK_2(GameTableUI::betBtnCallback, this));
        
        m_btnBetMax = dynamic_cast<Button*>(m_csbPanel->getChildByName("Button_betMax"));
        m_btnBetMax->setTouchEnabled(true);
        m_btnBetMax->setVisible(true);
        m_btnBetMax->setTag(3);
        m_btnBetMax->addTouchEventListener(CC_CALLBACK_2(GameTableUI::betBtnCallback, this));
        
        m_btnStart = dynamic_cast<Button*>(m_csbPanel->getChildByName("Button_turn"));
        m_btnStart->setTouchEnabled(true);
        m_btnStart->setVisible(true);
        m_btnStart->addTouchEventListener(CC_CALLBACK_2(GameTableUI::startBtnCallback, this));
        
        m_btnScore = dynamic_cast<Button*>(m_csbPanel->getChildByName("Button_score"));
        m_btnScore->setTouchEnabled(true);
        m_btnScore->setVisible(true);
        m_btnScore->setTag(0);
        m_btnScore->addTouchEventListener(CC_CALLBACK_2(GameTableUI::doubleBtnCallback, this));
        
        m_btnDouble = dynamic_cast<Button*>(m_csbPanel->getChildByName("Button_double"));
        m_btnDouble->setTouchEnabled(true);
        m_btnDouble->setVisible(true);
        m_btnDouble->setTag(1);
        m_btnDouble->addTouchEventListener(CC_CALLBACK_2(GameTableUI::doubleBtnCallback, this));
        
        //下分比倍外发光
        m_imageScoreFrame = ImageView::create("Games/31000504/shuiguolaba/36.png");
        m_imageScoreFrame->setVisible(false);
        m_imageScoreFrame->setPosition(m_btnScore->getPosition());
        m_btnScore->getParent()->addChild(m_imageScoreFrame);
        
        m_imageDoubleFrame = ImageView::create("Games/31000504/shuiguolaba/36.png");
        m_imageDoubleFrame->setVisible(false);
        m_imageDoubleFrame->setPosition(m_btnDouble->getPosition());
        m_btnDouble->getParent()->addChild(m_imageDoubleFrame);
        
        auto jackpotBack = dynamic_cast<ImageView*>(m_csbPanel->getChildByName("Image_jackpot"));
        if (jackpotBack) {
            auto nodeJackpot = dynamic_cast<Node*>(jackpotBack->getChildByName("Node_jackpot"));
            if (nodeJackpot) {
                auto nodeCsb = CSLoader::createNode("Games/31000504/Node_jackpotLight.csb");
                if (nodeCsb) {
                    nodeCsb->setPosition(Vec2(0,0));
                    nodeJackpot->addChild(nodeCsb);
                    auto nodeAction = CSLoader::createTimeline("Games/31000504/Node_jackpotLight.csb");
                    nodeCsb->runAction(nodeAction);
                    nodeAction->gotoFrameAndPlay(0);
                }
            }
        }
        m_btnDouble->setTouchEnabled(true);
        m_btnDouble->setVisible(true);
        m_btnDouble->setTag(1);
        m_btnDouble->addTouchEventListener(CC_CALLBACK_2(GameTableUI::doubleBtnCallback, this));
        
        //拉拔
        m_btnBar = dynamic_cast<Button*>(m_csbPanel->getChildByName("Button_bar"));
        m_btnBar->addTouchEventListener(CC_CALLBACK_2(GameTableUI::startBtnCallback, this));
        
        
        Node* nodeTop = dynamic_cast<Node*>(m_csbPanel->getChildByName("Node_top"));
        if (nodeTop) {
            auto nodeCsb = CSLoader::createNode("Games/31000504/Node_topEffect.csb");
            if (nodeCsb) {
                nodeCsb->setPosition(Vec2(0,0));
                nodeTop->addChild(nodeCsb);
                auto topAction = CSLoader::createTimeline("Games/31000504/Node_topEffect.csb");
                nodeCsb->runAction(topAction);
                topAction->gotoFrameAndPlay(0);
            }
           
        }
        
        Node* nodeleft = dynamic_cast<Node*>(m_csbPanel->getChildByName("Node_left"));
        if (nodeleft) {
            auto leftCsb = CSLoader::createNode("Games/31000504/Node_liuxian.csb");
            if (leftCsb) {
                leftCsb->setPosition(Vec2(0,0));
                nodeleft->addChild(leftCsb);
                leftCsb->setScaleX(-1);
                auto leftAction = CSLoader::createTimeline("Games/31000504/Node_liuxian.csb");
                leftCsb->runAction(leftAction);
                leftAction->gotoFrameAndPlay(0);
            }
           
        }
        
        Node* noderight = dynamic_cast<Node*>(m_csbPanel->getChildByName("Node_right"));
        if (noderight) {
            auto rightCsb = CSLoader::createNode("Games/31000504/Node_liuxian.csb");
            if (rightCsb) {
                rightCsb->setPosition(Vec2(0,0));
                noderight->addChild(rightCsb);
                auto rightAction = CSLoader::createTimeline("Games/31000504/Node_liuxian.csb");
                rightCsb->runAction(rightAction);
                rightAction->gotoFrameAndPlay(0);
            }
            
        }
        
        
        auto jsonPath      = "Games/31000504/animation/deng/sg_deng.json";
        auto atlasPath     = "Games/31000504/animation/deng/sg_deng.atlas";
        auto skeletonNode  = spine::SkeletonAnimation::createWithJsonFile(jsonPath,atlasPath);
        if (skeletonNode) {
            skeletonNode->setPosition(Vec2(0,0));
            skeletonNode->setAnimation(0,"sg_deng",true);
            m_csbPanel->addChild(skeletonNode,1);
            
        }
        
        auto jsonPath2      = "Games/31000504/animation/jinbi/sg_xg.json";
        auto atlasPath2     = "Games/31000504/animation/jinbi/sg_xg.atlas";
        auto skeletonNode2  = spine::SkeletonAnimation::createWithJsonFile(jsonPath2,atlasPath2);
        if (skeletonNode2) {
            skeletonNode2->setPosition(Vec2(0,0));
            skeletonNode2->setAnimation(0,"sg_xg",true);
            m_csbPanel->addChild(skeletonNode2,1);
            
        }

    }
	
}

void GameTableUI::loadText()
{
    if (m_csbPanel) {
        m_textBetUnit = dynamic_cast<Text*>(m_csbPanel->getChildByName("Text_betBase"));
        m_textBetUnit->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_textBetUnit->setString("");

		auto pos = m_textBetUnit->getPosition();
        m_textBetMul = dynamic_cast<Text*>(m_csbPanel->getChildByName("Text_betMul"));
        m_textBetMul->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_nCurBetMul = 1;
        m_textBetMul->setString(to_string(m_nCurBetMul));
		pos = m_textBetMul->getPosition();
        m_textPoolMoney = dynamic_cast<Text*>(m_csbPanel->getChildByName("Text_jackpot"));
        //m_textPoolMoney->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        //m_textPoolMoney->setString("0");
        
        m_textMyMoney = dynamic_cast<Text*>(m_csbPanel->getChildByName("Text_money"));
       // m_textMyMoney->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        //m_textMyMoney->setString(StringUtils::format("%lld", PlatformLogic()->loginResult.i64Money));
        
        m_textScoreMoney = dynamic_cast<Text*>(m_csbPanel->getChildByName("Text_score"));
       // m_textScoreMoney->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        m_textScoreMoney->setString("0");
    }
	
}

void GameTableUI::loadTurn()
{
	char str[128] = { 0 };
	for (int i = 0; i < 3; i++)
	{
		//转的容器
		Layout* layout = dynamic_cast<Layout*>(m_csbPanel->getChildByName(StringUtils::format("Panel_turnArea%d", i)));
		layout->setClippingEnabled(true);
		m_vPanelTurnArea.push_back(layout);

		//开奖闪烁
		ImageView* imageTurnBlink = dynamic_cast<ImageView*>(m_csbPanel->getChildByName(StringUtils::format("Image_turnBlink%d", i)));
		imageTurnBlink->setVisible(false);
        imageTurnBlink->setLocalZOrder(imageTurnBlink->getLocalZOrder()-1);
		m_vImageTurnBlink.push_back(imageTurnBlink);

		//押注区域闪烁
		//ImageView* imageBetAreaBlink = dynamic_cast<ImageView*>(m_csb->getChildByName(StringUtils::format("Image_betAreaBright%d", i)));
		//imageBetAreaBlink->setVisible(i == 0);
//        imageBetAreaBlink->setLocalZOrder(imageBetAreaBlink->getZOrde()-1);
		//m_vImageBetAreaBlink.push_back(imageBetAreaBlink);
	}

	//转动的icon
	setTurnIcomPosition(m_vTurnIconPos, m_vPanelTurnArea.at(0)->getContentSize());
	vector<ImageView*> vIcon;
	for (int i = 0; i < LOTTERY_COUNT_FRUIT; i++)
	{
		vIcon.clear();
		for (int j = 0; j < LOTTERY_TYPE_COUNT_FRUIT; j++)
		{
			sprintf(str, kIconTypeTpl, j);
			ImageView* icon = ImageView::create(str);
			icon->setTag(j);
			m_vPanelTurnArea.at(i)->addChild(icon);
			icon->setPosition(m_vTurnIconPos.at(j));

			vIcon.push_back(icon);
		}

		m_vTurnIcon.push_back(vIcon);
	}

	//拉bar动画
	Node* nodeBar = dynamic_cast<Node*>(m_csbPanel->getChildByName("Node_yangan"));

	auto barCsb = CSLoader::createNode("Games/31000504/Node_yaogan.csb");
	barCsb->setPosition(Vec2(0,0));
	nodeBar->addChild(barCsb);
	m_barAction = CSLoader::createTimeline("Games/31000504/Node_yaogan.csb");
	barCsb->runAction(m_barAction);
	m_barAction->setLastFrameCallFunc([&](){
		sendGameStart();
	});

	//中奖闪烁动画
	for (int i = 0; i < LOTTERY_COUNT_FRUIT; i++)
	{
		auto blinkCsb = CSLoader::createNode("v88/animation/slots/fruitslots/fruitslots_guangzao_1.csb");
		blinkCsb->setPosition(m_vImageTurnBlink.at(i)->getPosition());
		blinkCsb->setLocalZOrder(m_vImageTurnBlink.at(i)->getLocalZOrder());
		blinkCsb->setVisible(false);
		m_csbPanel->addChild(blinkCsb);
		m_vTurnBlinkCsb.push_back(blinkCsb);

		auto blinkAction = CSLoader::createTimeline("v88/animation/slots/fruitslots/fruitslots_guangzao_1.csb");
		blinkCsb->runAction(blinkAction);
		m_vTurnBlinkAction.push_back(blinkAction);
		blinkAction->setLastFrameCallFunc([=](){
			m_vTurnBlinkCsb.at(i)->setVisible(false);
			//setBink(m_imageOpenTypeBlink, false);
			//m_imageOpenTypeBlink->setVisible(false);
		});

	}

	//开奖类型的
	/*Node* nodeOpenTypeStart = dynamic_cast<Node*>(m_csb->getChildByName("Node_openTypeStart"));

	for (int i = 0; i < 10; i++)
	{
		sprintf(str, "Image_start%d", i);
		ImageView* image = dynamic_cast<ImageView*>(nodeOpenTypeStart->getChildByName(str));
		image->setVisible(false);
		m_vOpenTypePos.push_back(image->getPosition());
	}

	m_imageOpenTypeBlink = ImageView::create("Games/31000504/shuiguolaba/37.png");
	m_imageOpenTypeBlink->setVisible(false);
	m_imageOpenTypeBlink->setPosition(m_vOpenTypePos.at(0));
	nodeOpenTypeStart->addChild(m_imageOpenTypeBlink);*/
}

void GameTableUI::loadDoublePanel()
{
	m_panelDouble = dynamic_cast<Layout*>(m_csbPanel->getChildByName("Panel_double"));
	m_panelDouble->setVisible(false);
	m_panelDouble->setTouchEnabled(false);
	//为了屏蔽其他，只留下分
	m_btnScore->setLocalZOrder(m_panelDouble->getLocalZOrder() + 1);

	m_btnSmall = dynamic_cast<Button*>(m_panelDouble->getChildByName("Button_small"));
	m_btnSmall->setTag(0);
	m_btnSmall->addTouchEventListener(CC_CALLBACK_2(GameTableUI::guessBtnCallback, this));

	m_btnBig = dynamic_cast<Button*>(m_panelDouble->getChildByName("Button_big"));
	m_btnBig->setTag(1);
	m_btnBig->addTouchEventListener(CC_CALLBACK_2(GameTableUI::guessBtnCallback, this));

	m_textOpenPoint = dynamic_cast<Text*>(m_panelDouble->getChildByName("Text_point"));

	//加特效
	auto bg = dynamic_cast<ImageView*>(m_panelDouble->getChildByName("Image_doubleBg"));
	Vec2 pos = Vec2(bg->getPosition().x , bg->getPosition().y + bg->getContentSize().height/2);

	m_caiWinCsb = CSLoader::createNode(FRUIT_SLOT_DOUBLE_WIN_ANI);
	m_caiWinCsb->setPosition(pos);
	m_caiWinCsb->setVisible(false);
	m_panelDouble->addChild(m_caiWinCsb);
	m_caiWinAction = CSLoader::createTimeline(FRUIT_SLOT_DOUBLE_WIN_ANI);
	m_caiWinCsb->runAction(m_caiWinAction);
	m_caiWinAction->setLastFrameCallFunc([&](){

		m_caiWinCsb->setVisible(false);
	});

	m_caiLoseCsb = CSLoader::createNode(FRUIT_SLOT_DOUBLE_LOSE_ANI);
	m_caiLoseCsb->setPosition(pos);
	m_caiLoseCsb->setVisible(false);
	m_panelDouble->addChild(m_caiLoseCsb);
	m_caiLoseAction = CSLoader::createTimeline(FRUIT_SLOT_DOUBLE_LOSE_ANI);
	m_caiLoseCsb->runAction(m_caiLoseAction);
	m_caiLoseAction->setLastFrameCallFunc([&](){

		m_caiLoseCsb->setVisible(false);
	});

	m_caiResultCsb0 = CSLoader::createNode(FRUIT_SLOT_DOUBLE_RESULT_NUM_ANI);
	m_caiResultCsb0->setPosition(m_btnSmall->getPosition());
	m_caiResultCsb0->setVisible(false);
	m_panelDouble->addChild(m_caiResultCsb0);
	m_caiResultAction0 = CSLoader::createTimeline(FRUIT_SLOT_DOUBLE_RESULT_NUM_ANI);
	m_caiResultCsb0->runAction(m_caiResultAction0);
	m_caiResultAction0->setLastFrameCallFunc([&](){

		m_caiResultCsb0->setVisible(false);
	});

	m_caiResultCsb1 = CSLoader::createNode(FRUIT_SLOT_DOUBLE_RESULT_NUM_ANI);
	m_caiResultCsb1->setPosition(m_btnBig->getPosition());
	m_caiResultCsb1->setVisible(false);
	m_panelDouble->addChild(m_caiResultCsb1);
	m_caiResultAction1 = CSLoader::createTimeline(FRUIT_SLOT_DOUBLE_RESULT_NUM_ANI);
	m_caiResultCsb1->runAction(m_caiResultAction1);
	m_caiResultAction1->setLastFrameCallFunc([&](){

		m_caiResultCsb1->setVisible(false);
	});
}

void GameTableUI::createToolBarUI() {
	int zOrder = 60;
	auto toolbar = ToolBarLayer::create();
	if (toolbar) {
		toolbar->addImageAndCallBack(TOOL_BAR_EXIT, [=]() {
			if (!RoomLogic()->connected()) {
				leaveTheGame();
			}
			else {
				if (_GameTableLogic->_gameStatus != GS_FREE) {
					// 游戏不在空闲时期，不允许离开
					//auto layer = GamePromptLayer::create();
					//layer->showPrompt(GetText("limit_leave_in_game"));
					GamePromptTip* layer = GamePromptTip::create(this, Color4B(0, 0, 0, 160));
					layer->setGamePromptTip(GetText("limit_leave_in_game"));
					return;
				}
				_GameTableLogic->sendUserUp();
			}
		});
		toolbar->addImageAndCallBack(TOOL_BAR_SETTING, [=]() {
			IGameSet* gameSet = GameSetFactory::createWithVersion();
			gameSet->setBgPath(FRUIT_SLOT_BGM);
			gameSet->open(this, Vec2(0, 0), zOrder + 5, 0);
		});
        toolbar->addImageAndCallBack(TOOL_BAR_HELP, [=]() {
            auto helpLayer = HN::HelpLayer::create();
            helpLayer->setContentOnView("v66/common/help/LB.png");
            helpLayer->setName("helpLayer");
            helpLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            helpLayer->setOrignalPosition();
            helpLayer->moveSelfInWithAction();
            addChild(helpLayer, zOrder + 5);
        });
		toolbar->create(this, Vec2(10, WIN_SIZE.height), zOrder, 100);
	}
}

    void GameTableUI::turnUpdate(float dt)
    {
        //累计
        m_nHasTurnUnitCount++;
        
        for (int i = 0; i < LOTTERY_COUNT_FRUIT; i++)
        {
            for (int j = 0; j < LOTTERY_TYPE_COUNT_FRUIT; j++)
            {
                if (m_nHasTurnUnitCount <= m_vTurnUnitCount.at(i))
                {
                    m_vTurnIcon.at(i).at(j)->runAction(Sequence::create(
                        MoveBy::create(m_fTurnUnitTime, Vec2(0, -ICON_HEIGHT_FRUIT)), CallFunc::create([=](){
                        
                        //更新index
                        int nOldTag = m_vTurnIcon.at(i).at(j)->getTag();
                        int nNewTag = (nOldTag - 1) < 0 ? (LOTTERY_TYPE_COUNT_FRUIT - 1) : (nOldTag - 1);
                        m_vTurnIcon.at(i).at(j)->setTag(nNewTag);
                        
                        //一轮结束
                        if (j == LOTTERY_TYPE_COUNT_FRUIT - 1)
                        {
                            for (int k = 0; k < LOTTERY_TYPE_COUNT_FRUIT; k++)
                            {
                                int curTag = m_vTurnIcon.at(i).at(k)->getTag();
                                m_vTurnIcon.at(i).at(k)->setPosition(m_vTurnIconPos.at(curTag));
                            }
                        }
                        
                        //停止转动音效（第一列转停）
                        //if (m_nHasTurnUnitCount == m_vTurnUnitCount.at(0))
                        //{
                           // HNAudioEngine::getInstance()->stopEffectWithID(m_nTurnMusicID);
                        //}
                        
                    }), nullptr));
                    
                    if (m_nHasTurnUnitCount == m_vTurnUnitCount.at(i) && j == (LOTTERY_TYPE_COUNT_FRUIT - 1))
                    {
                        HNAudioEngine::getInstance()->playEffect(FRUIT_SLOT_TURN_DOWN_EFFECT);
                        char str[128] = {0};
                        sprintf(str, kIconTypeTpl, m_vOpenLotteryType.at(i));
                        
                        auto img = ImageView::create(str);
                        m_vTurnIcon.at(i).at(0)->getParent()->addChild(img);
                        img->setPosition(m_vTurnIconPos.at(1));
                        
                        _resultImg.push_back(img);
                        
                        for (auto ik = 0; ik < LOTTERY_TYPE_COUNT_FRUIT; ++ik)
                        {
                            m_vTurnIcon.at(i).at(ik)->setVisible(false);
                        }
                    }
                }
            }
        }
        
        //最后格子停下来后
        if (m_nHasTurnUnitCount == m_vTurnUnitCount.at(LOTTERY_COUNT_FRUIT - 1))// && j == (LOTTERY_TYPE_COUNT_CSLL - 1))
        {
            unschedule(schedule_selector(GameTableUI::turnUpdate));
            //检测是否中了
           
            checkTurnIsSuccess();
            if (m_nTurnMusicID > 0) {
                HNAudioEngine::getInstance()->stopEffectWithID(m_nTurnMusicID);
            }
        }
    }

void GameTableUI::setTurnIcomPosition(vector<Vec2> &vIconPos, Size parentSize)
{
	float fStartPosY = parentSize.height / 2 - ICON_HEIGHT_FRUIT;

	for (int i = 0; i < LOTTERY_TYPE_COUNT_FRUIT; i++)
	{
		vIconPos.push_back(Vec2(parentSize.width / 2, fStartPosY + i*ICON_HEIGHT_FRUIT));
	}
}

void GameTableUI::setTurnUnitCount(vector<int> &vTurnUnitCount, vector<int> vCurBeginIcon, vector<int> vCurEndIcon, int nRoundMinNum)
{
	for (int i = 0; i < LOTTERY_COUNT_FRUIT; i++)
	{
		//整圈数
		int nCount = (nRoundMinNum + i)*LOTTERY_TYPE_COUNT_FRUIT;
		//开的那圈
		nCount += LOTTERY_TYPE_COUNT_FRUIT - vCurBeginIcon.at(i) + vCurEndIcon.at(i);
		vTurnUnitCount.push_back(nCount);
	}
}

void GameTableUI::checkTurnIsSuccess()
{
	//中奖
	if (m_nOpenLineType >= 0)
	{
		//m_vTurnBlinkAction动画末尾回调做重置
		//播放中奖光效
		for (int i = 0; i < LOTTERY_COUNT_FRUIT; i++)
		{
			m_vTurnBlinkCsb.at(i)->setVisible(true);
			m_vTurnBlinkAction.at(i)->play("fruitslots_guangzao_1", false);
		}

		//中奖类型光效
		//m_imageOpenTypeBlink->setVisible(false);
		//Vec2 pos = m_vOpenTypePos.at(m_nOpenLineType);
		//m_imageOpenTypeBlink->setPosition(pos);
		//setBink(m_imageOpenTypeBlink, true);

		//音效
		if (m_nOpenLineType <= THREE_APPLE)
		{
			HNAudioEngine::getInstance()->playEffect(FRUIIT_SLOT_APPLE);

		}
		else if (m_nOpenLineType <= THREE_WATERMELON)
		{
			HNAudioEngine::getInstance()->playEffect(FRUIIT_SLOT_WATERMELON);
		}
		else if (m_nOpenLineType <= THREE_RING)
		{
			HNAudioEngine::getInstance()->playEffect(FRUIIT_SLOT_RING);
		}
		else if (m_nOpenLineType <= NINE_BAR)
		{
			HNAudioEngine::getInstance()->playEffect(FRUIIT_SLOT_BAR);
		}
		else if (m_nOpenLineType <= NINE_SEVEN)
		{
			HNAudioEngine::getInstance()->playEffect(FRUIIT_SLOT_SEVEN);
		}
	}
}

void GameTableUI::showCaiResultEffect(bool bSuccess, int nPoint)
{
	runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=](){

		//猜的结果
		if (bSuccess)
		{
			HNAudioEngine::getInstance()->playEffect(FRUIT_SLOT_SCORE_WIN_EFFECT);

			m_caiWinCsb->setVisible(true);
			m_caiWinAction->play("fruitslots_win", false);
		}
		else
		{
			HNAudioEngine::getInstance()->playEffect(FRUIT_SLOT_SCORE_LOSE_EFFECT);

			m_caiLoseCsb->setVisible(true);
			m_caiLoseAction->play("fruitslots_lose", false);
		}

		if (nPoint >= 1 && nPoint <= 5)
		{
			m_caiResultCsb0->setVisible(true);
			m_caiResultAction0->play("fruitslots_sizetip", false);
		}

		if (nPoint >= 5 && nPoint <= 9)
		{
			m_caiResultCsb1->setVisible(true);
			m_caiResultAction1->play("fruitslots_sizetip", false);
		}

		
	}), nullptr));
}

void GameTableUI::showBetAreaBright(int nBetMul)
{
	for (int i = 0; i < m_vImageBetAreaBlink.size(); i++)
	{
		m_vImageBetAreaBlink.at(i)->setVisible((i + 1) == nBetMul);
	}
}

void GameTableUI::blinkUpdate(float dt)
{
	for (auto node : m_vBlinkNode)
	{
		bool bVisible = node->isVisible();
		node->setVisible(!bVisible);
	}
}

void GameTableUI::setBink(Node* target, bool bAdd)
{
	if (bAdd)
	{
		auto iter = std::find(m_vBlinkNode.begin(), m_vBlinkNode.end(), target);
		if (iter == m_vBlinkNode.end())
		{
			m_vBlinkNode.push_back(target);
		}

		bool isRun = isScheduled(schedule_selector(GameTableUI::blinkUpdate));
		if (!isRun && m_vBlinkNode.size() > 0)
		{
			schedule(schedule_selector(GameTableUI::blinkUpdate), 0.2f);
		}
	}
	else
	{
		auto iter = std::find(m_vBlinkNode.begin(), m_vBlinkNode.end(), target);
		if (iter != m_vBlinkNode.end())
		{
			m_vBlinkNode.erase(iter);
		}

		if (m_vBlinkNode.size() == 0)
		{
			unschedule(schedule_selector(GameTableUI::blinkUpdate));
		}
	}
}

void GameTableUI::setBtnsTouchEnabled(bool bTouch)
{
	m_btnBet->setTouchEnabled(bTouch);
	m_btnBet->setBright(bTouch);
	m_btnBetMax->setTouchEnabled(bTouch);
	m_btnBetMax->setBright(bTouch);
	m_btnStart->setTouchEnabled(bTouch);
	m_btnStart->setBright(bTouch);
	m_btnBar->setTouchEnabled(bTouch);
	m_btnBar->setBright(bTouch);

	m_btnDouble->setTouchEnabled(bTouch);
	m_btnDouble->setBright(bTouch);
	m_btnScore->setTouchEnabled(bTouch);
	m_btnScore->setBright(bTouch);
}

void GameTableUI::numJumpUpdate(float dt)
{
	//1-9
	int nRandNum = rand() % 9 + 1;
	m_textOpenPoint->setString(to_string(nRandNum));
}

void GameTableUI::setNumJump(bool bJump)
{
	if (bJump)
	{
		schedule(schedule_selector(GameTableUI::numJumpUpdate), 0.1f);
	}
	else
	{
		unschedule(schedule_selector(GameTableUI::numJumpUpdate));
	}
}

void GameTableUI::cleanData()
{
	m_csb = nullptr;
    m_csbPanel = nullptr;
	m_btnAuto = nullptr;
	m_btnCancel = nullptr;
	m_btnBet = nullptr;
	m_btnBetMax = nullptr;
	m_btnStart = nullptr;
	m_btnScore = nullptr;
	m_btnDouble = nullptr;
	m_btnBar = nullptr;

	m_textBetUnit = nullptr;
	m_textBetMul = nullptr;
	m_textPoolMoney = nullptr;
	m_textMyMoney = nullptr;
	m_textScoreMoney = nullptr;

	m_imageOpenTypeBlink = nullptr;
	m_imageScoreFrame = nullptr;
	m_imageDoubleFrame = nullptr;

	m_vPanelTurnArea.clear();
	m_vTurnIcon.clear();
	m_vImageTurnBlink.clear();
	m_vImageBetAreaBlink.clear();
	m_vTurnBlinkCsb.clear();
	m_vTurnBlinkAction.clear();

	m_panelDouble = nullptr;
	m_btnSmall = nullptr;
	m_btnBig = nullptr;
	m_textOpenPoint = nullptr;

	m_caiWinCsb = nullptr;
	m_caiWinAction = nullptr;
	m_caiLoseCsb = nullptr;
	m_caiLoseAction = nullptr;
	m_caiResultCsb0 = nullptr;
	m_caiResultAction0 = nullptr;
	m_caiResultCsb1 = nullptr;
	m_caiResultAction1 = nullptr;
	//
	m_bAuto = false;
	m_nCurBetMul = 1;
	m_vOpenTypePos.clear();
	m_vBlinkNode.clear();
	m_vTurnIconPos.clear();
	m_vTurnUnitCount.clear();
	m_nHasTurnUnitCount = 0;
	m_fTurnUnitTime = 0;
	m_nOpenShowTime = 0;
	m_nOpenLineType = -1;
	m_nTurnMusicID = -1;
	//
	m_i64BetUnit = 0;
    
    m_i64MyMoney = 0;
    m_Notice     = nullptr;
}

void GameTableUI::sendGameStart()
{
	struct_c2s_start msg;
	msg.i64NoteMul = m_nCurBetMul;

	RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C2S_GAME_START, &msg, sizeof(msg));
}

void GameTableUI::sendDoCaiDaXiao(int nDo)
{
	if (nDo < 0 || nDo > 1) return;

	struct_c2s_doCaiDaXiao msg;
	msg.nDo = nDo;

	RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C2S_DO_CAI, &msg, sizeof(msg));
}

void GameTableUI::sendCaiDaXiao(int nDo, int nType)
{
	if (nType < 0 || nType > 1) return;

	struct_c2s_caiDaXiao msg;
	msg.nDo = nDo;
	msg.nCaiType = nType;

	RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C2S_CAI, &msg, sizeof(msg));
}

void GameTableUI::playBigWinAnimation(int gameID, int openLogo, int logoRate, LLONG money) {
	//        auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 150));
	//        addChild(colorLayer, 50);
	//        colorLayer->runAction(Sequence::create(DelayTime::create(3.0f), RemoveSelf::create(), nullptr));
	//        auto node = CSLoader::createNode("v66/animation/common/jackpotting/jackpotting.csb");
	//        if (node) {
	//            node->setPosition(WIN_SIZE/2);
	//            addChild(node, 50);
	//            auto action = CSLoader::createTimeline("v66/animation/common/jackpotting/jackpotting.csb");
	//            if (action) {
	//                node->runAction(action);
	//                action->play("jackpotting", false);
	//            }
	//            //
	//            char gameNamePath[64] = {0};
	//            sprintf(gameNamePath, "v66/common/bigwin/%d.png", gameID);
	//            std::string logoName = "";
	//            if (openLogo == K_SY) {
	//                logoName = "鲨鱼";
	//            }
	//            else if (openLogo == K_LY) {
	//                logoName = "老鹰";
	//            }
	//            else if (openLogo == K_SZ) {
	//                logoName = "狮子";
	//            }
	//            char tipString[256] = {0};
	//            sprintf(tipString, GetText("BIG_WIN_TIP_FQZS"), logoName.c_str(), logoRate);
	//            auto tmpNode = node->getChildByName("Node_1");
	//            if (tmpNode) {
	//                auto kuang = tmpNode->getChildByName("kuang");
	//                if (kuang) {
	//                    //game name
	//                    ImageView* gameName = ImageView::create(gameNamePath);
	//                    if (gameName) {
	//                        gameName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	//                        gameName->setPosition(Vec2(kuang->getContentSize().width/2, kuang->getContentSize().height*0.73f));
	//                        kuang->addChild(gameName);
	//                    }
	//                    //money value
	//                    TextAtlas* moneyAtals = dynamic_cast<TextAtlas*>(kuang->getChildByName("AtlasLabel_2"));
	//                    if (moneyAtals) {
	//                        moneyAtals->setProperty(to_string(money), "v66/common/zi/111.png", 55, 78, "0");
	//                    }
	//                    //tip
	//                    Text* tip = Text::create(tipString, FONT_TTF_MSYH, 32);
	//                    if (tip) {
	//                        tip->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	//                        tip->setPosition(Vec2(kuang->getContentSize().width/2, kuang->getContentSize().height*0.24f));
	//                        tip->setTextVerticalAlignment(TextVAlignment::CENTER);
	//                        tip->setTextHorizontalAlignment(TextHAlignment::CENTER);
	//                        tip->ignoreContentAdaptWithSize(false);
	//                        tip->setContentSize(Size(kuang->getContentSize().width*0.77f, kuang->getContentSize().height*0.26f));
	//                        kuang->addChild(tip);
	//                    }
	//                }
	//            }
	//        }
}
    
    void GameTableUI::onLuckyMoneyNotify(LuckyMoneyNotify *msg)
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
    
    void GameTableUI::onLuckyMoneyResult(LuckyMoneyResult *msg)
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
            auto i64MyMoney = msg->i64MyMoney;
            runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
                txtReward->setString(Tools::addComma(luckyMoney));
                if (nullptr != m_textMyMoney)
                {
                    m_textMyMoney->setString(to_string(i64MyMoney));
                }
            }), DelayTime::create(retainTime), CallFunc::create([&](){
                this->removeChildByName("rewardResult");
            }), NULL));
        }
    }
    
    void GameTableUI::cleanResults()
    {
        for (auto img : _resultImg)
        {
            if (nullptr != img)
            {
                img->removeFromParent();
            }
        }
        
        _resultImg.clear();
        
        for (auto iconVec : m_vTurnIcon)
        {
            for (auto icon : iconVec)
            {
                if(nullptr != icon)
                    icon->setVisible(true);
            }
        }
    }
}
