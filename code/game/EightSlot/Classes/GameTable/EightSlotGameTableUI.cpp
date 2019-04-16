#include "../../../../code/common/LibHNLobby/HNLobby/GameNotice.h"
#include "HNLobby/v66/ExtraReward.h"
#include "EightSlotGameTableUI.h"

#include "HNLobby/state/PlatformState.h"
#include "HNLobby/pattern/StateManager.h"
#include "HNLobby/PlatformDefine.h"

#include "HNLobby/globel.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "HNLobby/pattern/factory/gameSet/IGameSet.h"
#include "HNLobby/pattern/factory/gameSet/GameSetFactory.h"
#include "HNLobby/GameData.h"

#include "HNCommon/HNConverCode.h"
#include "UI/Scanner.h"
#include "UI/HelpLayer.h"
#include "UI/ToolBarLayer.h"
#include "UI/FloatCoin.h"
#include "HNLobby/pattern/CommandManager.h"
#include "EightRule.hpp"
#include "EightSet.hpp"
#include "HNLobby/pattern/factory/background/BackgroundFactory.h"
#include "UI/UserHead.h"
#include "HNLobby/PlatformDefine.h"
#include "UI/BigWinner.hpp"

#define ICON_HEIGHT_FRUIT 184

#define EIGHT_SLOT_BGM                    "Games/31000512/music/bgm.mp3"
#define EIGHT_SLOT_CAIJIN                "Games/31000512/music/caijin.mp3"
#define EIGHT_SLOT_TURN_EFFECT            "Games/31000512/music/gundong.mp3"
#define EIGHT_SLOT_TURN_DOWN_EFFECT        "Games/31000512/music/snd_stop_reel.wav"

#define EIGHT_SLOT_SCORE_LOSE_EFFECT    "Games/31000512/music/bang.mp3"
#define EIGHT_SLOT_SCORE_ADD            "Games/31000512/music/winEffect.mp3"
#define EIGHT_SLOT_SCORE_GET            "Games/31000512/music/shoufenEffect.mp3"

namespace EightSlot {

    const static char* kIconTypeTplEight = "Games/31000512/type/%d.png";
    
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
	state->setType(GAMELIST);
	StateManager::getInstance()->setState(state);
}

void GameTableUI::onEnter() {
	HNGameUIBase::onEnter();
	HNAudioEngine::getInstance()->playBackgroundMusic(EIGHT_SLOT_BGM,true);
    
    
    
    auto  m_loadUI = IGameLoadingUI::create(true,LOAD_EIGHT_GAME);
    if (m_loadUI)
    {
        m_loadUI->loadRes();
        m_loadUI->onCloseCallBack = [this](){
            auto loadingBG = BackgroundFactory::create("Games/31000512/ui/bgBig.png");
            loadingBG->setPosition(Vec2(VISIBLE_SIZE.width / 2, VISIBLE_SIZE.height / 2));
            addChild(loadingBG);
            
            m_csb = CSLoader::createNode("Games/31000512/Node_root.csb");
            m_csb->setPosition(WIN_SIZE / 2);
            addChild(m_csb);
            
            loadButton();
            loadTurn();
            
            createToolBarUI();
            
            _GameTableLogic = new GameTableLogic(this, m_bDeskIndex, m_bAutoCreate, PLAYER_COUNT);
            _GameTableLogic->enterGame();
        };
        addChild(m_loadUI);
   }
}

void GameTableUI::onExit() {
	HNGameUIBase::onExit();
    unschedule(schedule_selector(GameTableUI::updateCashOutTimer));
    unschedule(schedule_selector(GameTableUI::updateGetScoreTimer));
    unschedule(schedule_selector(GameTableUI::turnUpdate));
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
    
void GameTableUI::addNoteLogic() {
   
}
void GameTableUI::setMyMoney()
{
    if (m_textMyMoney) {
        m_textMyMoney->setString(Tools::addComma(m_i64MyMoney));
    }
    
}
void GameTableUI::setJackpot()
{
    if (m_jackpot) {
        m_jackpot->setString(StringUtils::format("$ %s",Tools::addComma(m_i64Jackpot).c_str()));
    }
}
void GameTableUI::setNote()
{
    if (m_textNote) {
        m_textNote->setString(to_string(m_currentNoteValue));
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
	
    
    m_i64NoteUnit   = msg->i64NoteUnit;
    m_i64NoteMin    = msg->i64NoteMin;                        //每条线最少压注值
    m_i64NoteMax    = msg->i64NoteMax;                        //每条线最大压注值
    m_i64MyMoney    = msg->i64MyMoney;                        //用户当前金币
    //m_i64Jackpot    = msg->i64Jackpot;                        //当前奖池
    
    m_i64OldMyMoney = m_i64MyMoney;
    memcpy(m_nLotteryRate, msg->nLotteryRate, sizeof(m_nLotteryRate));
    
    m_currentNoteValue = m_i64NoteMin;
    setMyMoney();
    setJackpot();
    setNote();
    
    m_i64Jackpot = GameData::getInstance()->getRoomJackpotData();
    schedule(schedule_selector(GameTableUI::jackpotUpdate), 0.33f);
}

//游戏开始
void GameTableUI::onGameStart(struct_s2c_startGame* msg)
{
    
    
    m_i64MyMoney    = msg->i64MyMoney;
    m_i64OldMyMoney = m_i64MyMoney;
    setMyMoney();
    //m_i64Jackpot = msg->i64Jackpot;
    setJackpot();
    
    m_i64AllScore = msg->i64AllScore;
    m_i64WinScore = msg->i64WinScore;
    m_cashInNum   = m_i64WinScore/120;
    m_nOpenLotteryType = msg->nOpenLotteryType;
    
    nFreeCount    = msg->nFreeCount;
	//转动数据
	m_vTurnUnitCount.clear();
	
	//启动转盘
	m_nHasTurnUnitCount = 0;
    
    m_vTurnUnitCount.push_back(50);
    
    cleanResults();
    int level = 1;
    if (m_i64oldAllScore > 800000 && m_i64oldAllScore < 1500000) {
        level = 2;
    }
    else if (m_i64oldAllScore > 1500000 && m_i64oldAllScore > 3000000) {
        level = 3;
    }
    else if (m_i64oldAllScore > 3000000)
    {
        level = 4;
    }
    showRotateEffect(level);
    
	schedule(schedule_selector(GameTableUI::turnUpdate),0.05);
	
    if (m_nTurnMusicID > 0)
    {
        HNAudioEngine::getInstance()->stopEffectWithID(m_nTurnMusicID);
    }
    m_nTurnMusicID = HNAudioEngine::getInstance()->playEffect(EIGHT_SLOT_TURN_EFFECT, true);
}
void GameTableUI::onGameUpdateMoney(struct_s2c_update_money* msg)
{
    
    m_i64WinScore = msg->i64WinScore;
    
    m_cashOutNum  = m_i64oldAllScore/140;
    m_myMoneyAddNum = (msg->i64MyMoney - m_i64MyMoney)/140;
    m_i64MyMoney  = msg->i64MyMoney;
    
    m_nOtherEffectID = HNAudioEngine::getInstance()->playEffect(EIGHT_SLOT_SCORE_GET);
    schedule(schedule_selector(GameTableUI::updateCashOutTimer));

    
}
    void GameTableUI::jackpotUpdate(float dt)
    {
        m_i64Jackpot += random(50,100);
        setJackpot();
    }
void GameTableUI::stopBtnCallback(Ref* pSender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;
    if (m_bStop) {
        return;
    }
    m_bStop = true;
	
}
void GameTableUI::startBtnCallback(Ref* pSender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;

	HNAudioEngine::getInstance()->playEffect("Games/31000512/music/start.mp3");

	//状态不对
	if (_GameTableLogic->_gameStatus == GS_FREE || _GameTableLogic->_gameStatus == GS_CONTINUE)
	{
        if (m_bAnimation) {
            return;
        }
        
        if (nFreeCount == 0) {
            //下注额不为0
            if (m_currentNoteValue <= 0)
            {
                CommandManager::getInstance()->showGamePrompt(GBKToUtf8("请下注"));
                return;
            }
            
            if (m_i64MyMoney < m_currentNoteValue)
            {
                if (m_i64AllScore > 0) {
                    showTipGetScoreEffect();
                }
                else
                {
					CommandManager::getInstance()->showGamePrompt(GetText("SEND_PACKET_ERROR_1"));
                    //CommandManager::getInstance()->showGamePrompt(GBKToUtf8("金币不足"));
                }
                
                return;
            }
        }
       
        m_bAnimation = true;
        m_bStop      = false;
        sendGameStart();
        
        showStartBtnEffect(0);
        showGetScoreBtnEffect(0);
        setBtnsTouchEnabled(m_btnStart,false);
        setBtnsTouchEnabled(m_btnScore,false);
        setBtnsTouchEnabled(m_btnAddNote,false);
        setBtnsTouchEnabled(m_btnSubNote,false);
	}
}

void GameTableUI::getScoreBtnCallback(Ref* pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    
    HNAudioEngine::getInstance()->playEffect("Games/31000512/music/shoufen.mp3");
    if (m_i64AllScore > 0 && m_bAnimation == false) {
        showStartBtnEffect(0);
        showGetScoreBtnEffect(0);
        
        struct_c2s_score msg;
        msg.i64null = 0;
        
        RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C2S_GET_SCORE, &msg, sizeof(msg));
    }
    
    
    
	
}
void GameTableUI::setBtnCallback(Ref* pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    
    auto eightSet = EightSet::create();
    if (eightSet) {
        eightSet->open(this,Vec2::ZERO,1000,1000);
        eightSet->_rCallBack = [this](cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
            if (type != Widget::TouchEventType::ENDED) {
                return;
            }
            ruleBtnCallback(m_btnRule,Widget::TouchEventType::ENDED);
            
        };
        eightSet->_eCallBack = [this](cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
            if (type != Widget::TouchEventType::ENDED) {
                return;
            }
            if (!RoomLogic()->connected()) {
                leaveTheGame();
            }
            else
            {
                // 游戏不在空闲时期，不允许离开
                if (_GameTableLogic->_gameStatus != GS_FREE && _GameTableLogic->_gameStatus != GS_CONTINUE) {
                    //auto layer = GamePromptLayer::create();
                    //layer->showPrompt(GetText("limit_leave_in_game"));
					GamePromptTip* layer = GamePromptTip::create(this, Color4B(0, 0, 0, 160));
					layer->setGamePromptTip(GetText("limit_leave_in_game"));
                    return;
                }
                else {
                    _GameTableLogic->sendUserUp();
                }
            }
            
        };
        eightSet->_mCallBack= [this](cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
            if (type != Widget::TouchEventType::ENDED) {
                return;
            }
            IGameSet* gameSet = GameSetFactory::createWithVersion();
            gameSet->setBgPath(EIGHT_SLOT_BGM);
            gameSet->open(this, Vec2(0, 0),10001, 0);
        
        };
    }
        
}
void GameTableUI::ruleBtnCallback(Ref* pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    
    if (m_currentNoteValue == 0) {
        return;
    }
    auto eightRule = EightRule::create();
    if (eightRule) {
        eightRule->setData(m_nLotteryRate,m_currentNoteValue);
        eightRule->open(this,Vec2::ZERO,1000,1000);
    }
        
}
void GameTableUI::addNoteBtnCallback(Ref* pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    
    auto name = ((Button*)pSender)->getName();
    if (name.compare("Button_subNote") == 0) {
        m_currentNoteValue -= m_i64NoteUnit;
    }
    else if (name.compare("Button_addNote") == 0)
    {
        m_currentNoteValue += m_i64NoteUnit;
    }
    
    if (m_currentNoteValue < m_i64NoteMin) {
        m_currentNoteValue = m_i64NoteMin;
    }
    
    if (m_currentNoteValue > m_i64NoteMax) {
        m_currentNoteValue = m_i64NoteMax;
    }
    setNote();
        
}
void GameTableUI::loadButton()
{
    auto imageBg = dynamic_cast<ImageView*>(m_csb->getChildByName("Image_1"));
    if (imageBg) {
        m_btnStart = dynamic_cast<Button*>(imageBg->getChildByName("Button_start"));
        m_btnStart->addTouchEventListener(CC_CALLBACK_2(GameTableUI::startBtnCallback, this));
        
        m_btnStop = dynamic_cast<Button*>(imageBg->getChildByName("Button_stop"));
        m_btnStop->setVisible(false);
        m_btnStop->addTouchEventListener(CC_CALLBACK_2(GameTableUI::stopBtnCallback, this));
        
        m_btnScore = dynamic_cast<Button*>(imageBg->getChildByName("Button_getScore"));
        m_btnScore->addTouchEventListener(CC_CALLBACK_2(GameTableUI::getScoreBtnCallback, this));
        
        m_btnAddNote = dynamic_cast<Button*>(imageBg->getChildByName("Button_addNote"));
        m_btnAddNote->addTouchEventListener(CC_CALLBACK_2(GameTableUI::addNoteBtnCallback, this));
        
        m_btnSubNote = dynamic_cast<Button*>(imageBg->getChildByName("Button_subNote"));
        m_btnSubNote->addTouchEventListener(CC_CALLBACK_2(GameTableUI::addNoteBtnCallback, this));
        
        m_btnRule = dynamic_cast<Button*>(imageBg->getChildByName("Button_rule"));
        m_btnRule->addTouchEventListener(CC_CALLBACK_2(GameTableUI::ruleBtnCallback, this));
        
        //m_btnSet = dynamic_cast<Button*>(imageBg->getChildByName("Button_set"));
        //m_btnSet->addTouchEventListener(CC_CALLBACK_2(GameTableUI::setBtnCallback, this));
        
        m_panelBg  = dynamic_cast<Layout*>(imageBg->getChildByName("Panel_bg"));
        
        
        auto top = dynamic_cast<ImageView*>(imageBg->getChildByName("Image_top"));
        if (top) {
           
        }
        

        m_textMyMoney  = dynamic_cast<Text*>(imageBg->getChildByName("txtMyMoney"));
        m_textNote     = dynamic_cast<Text*>(imageBg->getChildByName("txtNote"));
        
        m_getScore     = dynamic_cast<Text*>(imageBg->getChildByName("txtScore"));
        m_jackpot      = dynamic_cast<Text*>(imageBg->getChildByName("txtJackpot"));
    
        m_getScore->setString("$ 0");
        m_jackpot->setString("$ 0");
        m_textMyMoney->setString("0");
        m_textNote->setString("0");
        
        
        
        
        
        
        
        
        auto leftNode = dynamic_cast<Node*>(imageBg->getChildByName("Node_left"));
        auto rightNode = dynamic_cast<Node*>(imageBg->getChildByName("Node_right"));
        char str[32];
        int startIdx = 0;
        int endIdx   = 17;
        std::string animationName = "EIGHT_YINPIN_EFFECT";
        Animation * anima = AnimationCache::getInstance()->getAnimation(animationName);
        if (anima == nullptr) {
            anima = Animation::create();
            anima->setDelayPerUnit(0.10f);
            anima->setLoops(-1);
            
            for (int i=startIdx; i <= endIdx; i++) {
                memset(str,0,sizeof(str));
                sprintf(str, "yp_000%02d.png",i);
                auto frames = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
                if (frames) {
                    anima->addSpriteFrame(frames);
                }
                
            }
            
            AnimationCache::getInstance()->addAnimation(anima, animationName);
        }
        
        if (leftNode) {
            auto animate = Animate::create(anima);
            auto animationSprite = Sprite::create();
            animationSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            animationSprite->runAction(animate);
            animationSprite->setPosition(Vec2::ZERO);
            leftNode->addChild(animationSprite);
        }
        if (rightNode) {
            auto animate = Animate::create(anima);
            auto animationSprite = Sprite::create();
            animationSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            animationSprite->runAction(animate);
            animationSprite->setPosition(Vec2::ZERO);
            rightNode->addChild(animationSprite);
        }
       
        
        
        m_jackpotEffectNode = dynamic_cast<Node*>(imageBg->getChildByName("Node_jackpotEffect"));
        if (m_jackpotEffectNode) {
            auto nodeCsb = CSLoader::createNode("Games/31000512/jackpot.csb");
            if (nodeCsb) {
                nodeCsb->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                m_jackpotEffectNode->addChild(nodeCsb);
                
                
                auto m_Action = CSLoader::createTimeline("Games/31000512/jackpot.csb");
                nodeCsb->runAction(m_Action);
                m_Action->gotoFrameAndPlay(0);
            }
        }
        
        m_moneyEffectNode = dynamic_cast<Node*>(imageBg->getChildByName("Node_moneyEffect"));
        if (m_moneyEffectNode) {
            auto nodeCsb = CSLoader::createNode("Games/31000512/money.csb");
            if (nodeCsb) {
                nodeCsb->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                m_moneyEffectNode->addChild(nodeCsb);
                
                
                auto m_Action = CSLoader::createTimeline("Games/31000512/money.csb");
                nodeCsb->runAction(m_Action);
                m_Action->gotoFrameAndPlay(0);
            }
        }
        
        m_rotateNode = dynamic_cast<Node*>(imageBg->getChildByName("Node_rotate"));
        m_getScoreDoubleNode = dynamic_cast<Node*>(imageBg->getChildByName("Node_double"));
        m_getScoreBox = dynamic_cast<Node*>(imageBg->getChildByName("Node_box"));
        m_getScoreBigNodeShow = dynamic_cast<TextAtlas*>(imageBg->getChildByName("AtlasLabel_1"));
        
        MSG_GP_R_LogonResult &LogonResult = PlatformLogic()->loginResult;
        m_myName = dynamic_cast<Text*>(imageBg->getChildByName("txtName"));
        if (m_myName) {
            std::string nickName(LogonResult.nickName);
            if (!nickName.empty()) {
                m_myName->setString(ForceGBKToUtf8(nickName.c_str()));
            }
            else {
                m_myName->setString(GBKToUtf8("未知"));
            }
        }

        auto userHead = UserHead::create("hallimg/main/touxiangk.png");
        userHead->setHeadTouchEnabled(false);
        userHead->setPosition(Vec2(1050,670));
        imageBg->addChild(userHead);
        if (Tools::needLoadWechatImage()) {
            Texture2D* texture = new Texture2D;
            Tools::loadWechatImageFromLocal(texture);
            if (texture) {
                userHead->setClipUserHeadWithTexTure(texture, "hallimg/niuniu/5_1.png", 0.6f);
            }
        }
        else {
            std::string head_path = StringUtils::format(PLAYER_HEAD_V66, LogonResult.bLogoID);
            if (!FileUtils::getInstance()->isFileExist(head_path)) {
                head_path = DEFAULT_PLAYER_HEAD_V66;
            }
            userHead->setClipUserHeadWithFile(head_path, "hallimg/niuniu/5_1.png",0.6f);
        }
        
        
    }
	
}

void GameTableUI::loadTurn()
{
	char str[128] = { 0 };
    m_vPanelTurnArea.push_back(m_panelBg);

	//转动的icon
	setTurnIcomPosition(m_vTurnIconPos, m_vPanelTurnArea.at(0)->getContentSize());
    setTurnIcomPositionEx(m_vTurnIconPosVirual, m_vPanelTurnArea.at(0)->getContentSize());
    
	vector<ImageView*> vIcon;
	for (int i = 0; i < LOTTERY_COUNT_EIGHT; i++)
	{
		vIcon.clear();
		for (int j = 0; j < LOTTERY_TYPE_COUNT_EIGHT; j++)
		{
			sprintf(str, kIconTypeTplEight, j);
			ImageView* icon = ImageView::create(str);
			icon->setTag(j);
			m_vPanelTurnArea.at(i)->addChild(icon);
			icon->setPosition(m_vTurnIconPos.at(j));

			vIcon.push_back(icon);
		}

		m_vTurnIcon.push_back(vIcon);
	}
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
				if (_GameTableLogic->_gameStatus != GS_FREE && _GameTableLogic->_gameStatus != GS_CONTINUE) {
					// 游戏不在空闲时期，不允许离开
					//auto layer = GamePromptLayer::create();
					//layer->showPrompt(GetText("limit_leave_in_game"));
					GamePromptTip* layer = GamePromptTip::create(this, Color4B(0, 0, 0, 160));
					layer->setGamePromptTip(GetText("limit_leave_in_game"));
					return;
				}
                else {
                    if (m_i64oldAllScore > 0) {
						//CommandManager::getInstance()->showGamePrompt(GBKToUtf8("退出游戏之前请先收分"));
						GamePromptTip* layer = GamePromptTip::create(this, Color4B(0, 0, 0, 160));
						layer->setGamePromptTip(GBKToUtf8("退出游戏之前请先收分"));
                    }
                    else
                    {
                        _GameTableLogic->sendUserUp();
                    }
                    
                    
                }
			}
		});
		toolbar->addImageAndCallBack(TOOL_BAR_SETTING, [=]() {
			IGameSet* gameSet = GameSetFactory::createWithVersion();
			gameSet->setBgPath(EIGHT_SLOT_BGM);
			gameSet->open(this, Vec2(0, 0), zOrder + 5, 0);
		});
        
        toolbar->addImageAndCallBack(TOOL_BAR_HELP,[=](){
            auto eightRule = EightRule::create();
            if (eightRule) {
                eightRule->setData(m_nLotteryRate,m_currentNoteValue);
                eightRule->open(this,Vec2::ZERO,1000,1000);
            }
        });
		toolbar->create(this, Vec2(10, WIN_SIZE.height), zOrder, 100);
	}
}

    void GameTableUI::turnUpdate(float dt)
    {
        //累计
        m_nHasTurnUnitCount++;
        for (int j = 0; j < LOTTERY_TYPE_COUNT_EIGHT; j++)
            {
                if (m_nHasTurnUnitCount <= m_vTurnUnitCount.at(0))
                {
                    m_vTurnIcon.at(0).at(j)->runAction(Sequence::create(
                        MoveBy::create(0.1f, Vec2(0, -ICON_HEIGHT_FRUIT)), CallFunc::create([=](){
                        
                        //更新index
                        int nOldTag = m_vTurnIcon.at(0).at(j)->getTag();
                        int nNewTag = (nOldTag - 1) < 0 ? (LOTTERY_TYPE_COUNT_EIGHT - 1) : (nOldTag - 1);
                        m_vTurnIcon.at(0).at(j)->setTag(nNewTag);
                        
                        //一轮结束
                        if (j == LOTTERY_TYPE_COUNT_EIGHT - 1)
                        {
                            for (int k = 0; k < LOTTERY_TYPE_COUNT_EIGHT; k++)
                            {
                                int curTag = m_vTurnIcon.at(0).at(k)->getTag();
                                m_vTurnIcon.at(0).at(k)->setPosition(m_vTurnIconPos.at(curTag));
                            }
                        }
                        
                    }), nullptr));
                }
            }
        
        if (m_nHasTurnUnitCount == m_vTurnUnitCount.at(0) || m_bStop)
        {
            if (_resultImg.size() == 0) {
                char str[128] = {0};
                for (int m = 0; m < LOTTERY_TYPE_COUNT_EIGHT; m++) {
                    sprintf(str, kIconTypeTplEight,random(0, 14));
                    if (m == 1 && m_nOpenLotteryType != -1) {
                        sprintf(str, kIconTypeTplEight,m_nOpenLotteryType);
                    }
                    auto img = ImageView::create(str);
                    if (img) {
                        m_vTurnIcon.at(0).at(0)->getParent()->addChild(img);
                        img->setPosition(m_vTurnIconPos.at(m));
                        _resultImg.push_back(img);
                    }
                    
                }
                
                if (m_nOpenLotteryType != -1) {
                    
                    for (int n = 0; n < _resultImg.size(); n++) {
                        auto img = _resultImg.at(n);
                        if (img) {
                            img->setPositionY(m_vTurnIconPosVirual.at(n).y);
                        }
                    }
                    
                }
            }
            
            for (auto ik = 0; ik < LOTTERY_TYPE_COUNT_EIGHT; ++ik)
            {
                m_vTurnIcon.at(0).at(ik)->setVisible(false);
            }
        }
        //最后格子停下来后
        if (m_nHasTurnUnitCount == m_vTurnUnitCount.at(0) || m_bStop)
        {
           
            unschedule(schedule_selector(GameTableUI::turnUpdate));
            if (m_nTurnMusicID > 0)
            {
                HNAudioEngine::getInstance()->stopEffectWithID(m_nTurnMusicID);
            }
            HNAudioEngine::getInstance()->playEffect(EIGHT_SLOT_TURN_DOWN_EFFECT);
            m_btnStart->setVisible(true);
            m_btnStop->setVisible(false);
            
            showRotateEffect(0);
            showIconEffect();
            checkHasReward();
        }
}
void GameTableUI::checkHasReward()
{
    if (m_nOpenLotteryType != -1) { //有图案
        if (m_nOpenLotteryType == LT_Bang) {//清空的图案
            m_getScore->setString("$ 0");
            m_i64oldAllScore = 0;
            m_i64WinScore    = 0;
            
            HNAudioEngine::getInstance()->playEffect(EIGHT_SLOT_SCORE_LOSE_EFFECT);
            runAction(Sequence::create(DelayTime::create(1.0f),CallFunc::create([this](){
                    endAnimation();
            }),NULL));
            
        }
        else
        {
            showBigWinner();
            int level = 0;
            if (m_i64AllScore < 800000 && m_i64AllScore > 0) {
                level = 1;
            }
            else if (m_i64AllScore >= 800000 && m_i64AllScore < 1500000)
            {
                level = 2;
            }
            else if(m_i64AllScore >= 1500000)
            {
                level = 3;
            }
            showStartBtnEffect(level);
            showGetScoreBtnEffect(level);
            
            
            if (m_nOpenLotteryType == LT_TwoTimes || m_nOpenLotteryType == LT_ThreeTimes ||m_nOpenLotteryType == LT_FourTimes || m_nOpenLotteryType == LT_FiveTimes || m_nOpenLotteryType == LT_TenTimes ) {
                
                if (m_i64oldAllScore > 0) {
                    int musicLevel = 3;
                    if (m_nOpenLotteryType == LT_TwoTimes || m_nOpenLotteryType == LT_ThreeTimes) {
                        musicLevel = 1;
                    }
                    else if (m_nOpenLotteryType == LT_FourTimes || m_nOpenLotteryType == LT_FiveTimes)
                    {
                        musicLevel = 2;
                    }
                    HNAudioEngine::getInstance()->playEffect(StringUtils::format("Games/31000512/music/beishu%d.mp3",musicLevel).c_str());
                    showGetScoreDoubleEffect(m_nOpenLotteryType - 7,level);
                }
                else
                {
                    endAnimation();
                }
                
            }
            else if (m_nOpenLotteryType == LT_Bonus)
            {
                showCaijinEffect();
            }
            else
            {
                runAction(Sequence::create(CallFunc::create([this](){
                    m_nOtherEffectID = HNAudioEngine::getInstance()->playEffect(EIGHT_SLOT_SCORE_ADD);
                    m_getScoreBigNodeShow->setVisible(true);
                    m_getScoreBigNodeShow->setString(StringUtils::format("+ %s", Tools::addComma(m_i64WinScore).c_str()));
                }),DelayTime::create(1.0f),CallFunc::create([this,level](){
                    showGetScoreBoxEffect(level);
                    schedule(schedule_selector(GameTableUI::updateGetScoreTimer));
                }),NULL));
                
            }
           
        }
    }
    else
    {
        
        endAnimation();
    }
}
void GameTableUI::showBigWinner()
{
    auto winLevel = Tools::getLianxianGameData(m_i64WinScore,m_currentNoteValue,4,8);
    if (winLevel > 0) {
        auto bigWinLayer = BigWinnerLayer::create();
        if (bigWinLayer) {
            bigWinLayer->open(this,Vec2::ZERO,1000,1,winLevel,m_i64WinScore);
        }
    }
   
}
void GameTableUI::showIconEffect()
{
    if (m_nOpenLotteryType != -1) {
        
        if (_resultImg.at(1) != nullptr) {
            if (m_nOpenLotteryType != LT_Bonus && m_nOpenLotteryType !=LT_888 ) {
				//qiuyx animation
                auto actiobCsb = CSLoader::createNode(StringUtils::format("Games/31000512/icon%d.csb",m_nOpenLotteryType));
                actiobCsb->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                actiobCsb->setPosition(_resultImg.at(1)->getContentSize()/2);
                actiobCsb->setName("icon_effect");
                actiobCsb->setScale(1.5f);
                _resultImg.at(1)->addChild(actiobCsb);
                auto m_iconAction = CSLoader::createTimeline(StringUtils::format("Games/31000512/icon%d.csb",m_nOpenLotteryType));
                actiobCsb->runAction(m_iconAction);
                m_iconAction->gotoFrameAndPlay(0);
            }
            else
            {
                _resultImg.at(1)->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5,1.1),DelayTime::create(0.20f),ScaleTo::create(0.5,0.8),NULL)));
                
            }
           
        }
        
        
    }
        
}
void GameTableUI::showStartBtnEffect(int level) 
{
    auto m_startEffect = m_btnStart->getChildByName("EFFECT_START");
    if (m_startEffect) {
        m_startEffect->removeFromParent();
    }
    if (level >= 1 && level <= 3) {
        auto actiobCsb = CSLoader::createNode(StringUtils::format("Games/31000512/spin%d.csb",level));
        if (actiobCsb) {
            actiobCsb->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            actiobCsb->setPosition(m_btnStart->getContentSize()/2);
            actiobCsb->setName("EFFECT_START");
            m_btnStart->addChild(actiobCsb);
            actiobCsb->setScale(1.5f);
            
            auto m_Action = CSLoader::createTimeline(StringUtils::format("Games/31000512/spin%d.csb",level));
            actiobCsb->runAction(m_Action);
            m_Action->gotoFrameAndPlay(0);
        }
        
    }
        
}
void GameTableUI::showGetScoreBtnEffect(int level)
{
    auto m_scoreEffect = m_btnScore->getChildByName("EFFECT_SCORE");
    if (m_scoreEffect) {
        m_scoreEffect->removeFromParent();
    }
    if (level >= 1 && level <= 3) {
        auto actiobCsb = CSLoader::createNode(StringUtils::format("Games/31000512/cash%d.csb",level));
        if (actiobCsb) {
            actiobCsb->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            actiobCsb->setPosition(m_btnScore->getContentSize()/2);
            actiobCsb->setName("EFFECT_SCORE");
            actiobCsb->setScale(1.5f);
            
            m_btnScore->addChild(actiobCsb);
            auto m_Action = CSLoader::createTimeline(StringUtils::format("Games/31000512/cash%d.csb",level));
            actiobCsb->runAction(m_Action);
            m_Action->gotoFrameAndPlay(0);
        }
       
    }
    
}
void GameTableUI::showGetScoreDoubleEffect(int level,int boxLevel)
{
    if (m_getScoreDoubleNode != nullptr) {
        auto m_doubleEffect = m_getScoreDoubleNode->getChildByName("EFFECT_DOUBLE");
        if (m_doubleEffect) {
            m_doubleEffect->removeFromParent();
        }
        if (level >= 1 && level <= 5) {
            auto actiobCsb = CSLoader::createNode(StringUtils::format("Games/31000512/xeff%d.csb",level));
            if (actiobCsb) {
                actiobCsb->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                actiobCsb->setName("EFFECT_DOUBLE");
                m_getScoreDoubleNode->addChild(actiobCsb);
                
                auto m_Action = CSLoader::createTimeline(StringUtils::format("Games/31000512/xeff%d.csb",level));
                actiobCsb->runAction(m_Action);
                m_Action->gotoFrameAndPlay(0,false);
                m_Action->setLastFrameCallFunc([this,actiobCsb,boxLevel](){
                    
                    if (actiobCsb) {
                        actiobCsb->removeFromParent();
                    }
                    
                    runAction(Sequence::create(CallFunc::create([this](){
                        m_getScoreBigNodeShow->setVisible(true);
                        m_getScoreBigNodeShow->setString(StringUtils::format("+ %s", Tools::addComma(m_i64WinScore).c_str()));
                         m_nOtherEffectID = HNAudioEngine::getInstance()->playEffect(EIGHT_SLOT_SCORE_ADD);
                    }),DelayTime::create(0.5f),CallFunc::create([this,boxLevel](){
                        showGetScoreBoxEffect(boxLevel);
                       
                        schedule(schedule_selector(GameTableUI::updateGetScoreTimer));
                    }),NULL));
                    //翻倍特效结束后播放数字变化
                });
            }
            
        }
        
    }
}
void GameTableUI::showRotateEffect(int level)
{
    if (m_rotateNode != nullptr) {
        auto m_rotateEffect = m_rotateNode->getChildByName("EFFECT_ROTATE");
        if (m_rotateEffect) {
            m_rotateEffect->removeFromParent();
        }
        if (level >= 1 && level <= 4) {
            auto actiobCsb = CSLoader::createNode(StringUtils::format("Games/31000512/reel%d.csb",level));
            if (actiobCsb) {
                actiobCsb->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                actiobCsb->setName("EFFECT_ROTATE");
                m_rotateNode->addChild(actiobCsb);
                
                auto m_Action = CSLoader::createTimeline(StringUtils::format("Games/31000512/reel%d.csb",level));
                actiobCsb->runAction(m_Action);
                m_Action->gotoFrameAndPlay(0);
                m_Action->setLastFrameCallFunc([this,actiobCsb](){
                    
                    //if (actiobCsb) {
                        //actiobCsb->removeFromParent();
                   //}

                });
            }
            
        }
        
    }
}
void GameTableUI::showGetScoreBoxEffect(int level)
{
    if (m_getScoreBox != nullptr) {
        auto m_boxEffect = m_getScoreBox->getChildByName("EFFECT_BOX");
        if (m_boxEffect) {
            m_boxEffect->removeFromParent();
        }
        if (level >= 1 && level <= 3) {
            auto actiobCsb = CSLoader::createNode(StringUtils::format("Games/31000512/pot%d.csb",level));
            if (actiobCsb) {
                actiobCsb->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                actiobCsb->setName("EFFECT_BOX");
                m_getScoreBox->addChild(actiobCsb);
                
                auto m_Action = CSLoader::createTimeline(StringUtils::format("Games/31000512/pot%d.csb",level));
                actiobCsb->runAction(m_Action);
                m_Action->gotoFrameAndPlay(0,false);
                
            }
            
        }
        
    }
}
void GameTableUI::setTurnIcomPosition(vector<Vec2> &vIconPos, Size parentSize)
{
	float fStartPosY = ICON_HEIGHT_FRUIT/2 - 4;
    
	for (int i = 0; i < LOTTERY_TYPE_COUNT_EIGHT; i++)
	{
		vIconPos.push_back(Vec2(parentSize.width / 2, fStartPosY + i*ICON_HEIGHT_FRUIT));
	}
}
void GameTableUI::setTurnIcomPositionEx(vector<Vec2> &vIconPos, Size parentSize)
{
    float fStartPosYEx = ICON_HEIGHT_FRUIT/2 - 4 - ICON_HEIGHT_FRUIT/2;
    for (int i = 0; i < LOTTERY_TYPE_COUNT_EIGHT; i++)
    {
        vIconPos.push_back(Vec2(parentSize.width / 2, fStartPosYEx + i*ICON_HEIGHT_FRUIT));
    }
}
void GameTableUI::setTurnUnitCount(vector<int> &vTurnUnitCount, vector<int> vCurBeginIcon, vector<int> vCurEndIcon, int nRoundMinNum)
{
	for (int i = 0; i < LOTTERY_COUNT_EIGHT; i++)
	{
		//整圈数
		int nCount = (nRoundMinNum + i)*LOTTERY_TYPE_COUNT_EIGHT;
		//开的那圈
		nCount += LOTTERY_TYPE_COUNT_EIGHT - vCurBeginIcon.at(i) + vCurEndIcon.at(i);
		vTurnUnitCount.push_back(nCount);
	}
}
void GameTableUI::setBtnsTouchEnabled(Button* btnNode,bool bTouch)
{
    if (btnNode == nullptr) {
        return;
    }
	btnNode->setTouchEnabled(bTouch);
	btnNode->setBright(bTouch);
}
void GameTableUI::cleanData()
{
	m_csb = nullptr;
	
    m_panelBg  = nullptr;
	m_btnStart = nullptr;
	m_btnScore = nullptr;
    m_btnRule  = nullptr;
    m_btnStop  = nullptr;
    
    m_btnAddNote  = nullptr;
    m_btnSubNote  = nullptr;
    m_btnSet      = nullptr;
    
    m_textNote    = nullptr;
    
    m_textMyMoney = nullptr;
   
    m_jackpot     = nullptr;
    m_getScoreBig = nullptr;
    m_getScore    = nullptr;
    m_myName      = nullptr;
    
	m_vPanelTurnArea.clear();
	m_vTurnIcon.clear();
	

	m_bAuto = false;
	
	m_vTurnIconPos.clear();
    m_vTurnIconPosVirual.clear();
	m_vTurnUnitCount.clear();
	m_nHasTurnUnitCount = 0;
	m_nTurnMusicID = -1;

    m_i64NoteUnit      = 0;
    m_i64NoteMin       = 0;                        //每条线最少压注值
    m_i64NoteMax       = 0;                        //每条线最大压注值
    m_i64MyMoney       = 0;                        //用户当前金币
    m_i64Jackpot       = 0;                        //当前奖池
    m_currentNoteValue = 0;
    memset(m_nLotteryRate,0, sizeof(m_nLotteryRate));
    
    m_i64WinScore      = 0;            //本局中奖的得分
    m_i64AllScore      = 0;            //当前累计的得分
    m_i64oldAllScore   = 0;
    m_nFreeCount       = 0;                    //免费次数
    m_nOpenLotteryType = -1;            //本局中奖的图案，-1是没中图案
    
    m_bStop            = false;
    
    m_jackpotEffectNode  = nullptr;
    m_moneyEffectNode    = nullptr;;
    
    m_getScoreBox        = nullptr;
    m_getScoreDoubleNode = nullptr ;
    m_rotateNode         = nullptr;
    m_getScoreBigNodeShow= nullptr;
    
    m_bAnimation         = false;
    m_cashOutNum         = 0;
    m_myMoneyAddNum      = 0;
    m_i64OldMyMoney      = 0;
    m_cashInNum          = 0;
    m_nOtherEffectID     = -1;
    m_Notice             = nullptr;
}

void GameTableUI::sendGameStart()
{
	struct_c2s_start msg;
	msg.i64NoteMul = m_currentNoteValue;
    msg.i64jackpot = m_i64Jackpot;
	RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C2S_START_GAME, &msg, sizeof(msg));
}
void GameTableUI::cleanResults()
{
    for (auto img : _resultImg)
    {
        if (nullptr != img)
        {
            auto imgEffect = img->getChildByName("icon_effect");
            if (imgEffect) {
                imgEffect->removeFromParent();
            }
            img->runAction(Sequence::create(MoveBy::create(0.2f,Vec2(0,-ICON_HEIGHT_FRUIT*2)),CallFunc::create([this](){
                _resultImg.clear();
            }), RemoveSelf::create(),NULL));
        }
    }
        

    for (auto iconVec : m_vTurnIcon)
    {
        for (auto icon : iconVec)
        {
            if(nullptr != icon)
                icon->runAction(Sequence::create(DelayTime::create(0.2f),CallFunc::create([this,icon](){
                    icon->setVisible(true);

                }), NULL));
            }
        }
    
    runAction(Sequence::create(DelayTime::create(1.0f),CallFunc::create(
                                                                        [this](){
                                                                            if (!m_btnStop->isVisible()) {
                                                                                m_btnStop->setVisible(true);
                                                                            }
                                                                            
                                                                            if (m_btnStart->isVisible()) {
                                                                                m_btnStart->setVisible(false);
                                                                            }
                                                                        }),NULL));
    }
    void GameTableUI::updateCashOutTimer(float dt)
    {
        m_i64oldAllScore -= m_cashOutNum;
        m_i64oldAllScore = m_i64oldAllScore > 0 ? m_i64oldAllScore : 0;
         m_getScore->setString(StringUtils::format("$ %s", Tools::addComma(m_i64oldAllScore).c_str()));
        
        
        m_i64OldMyMoney += m_myMoneyAddNum;
        m_i64OldMyMoney = m_i64OldMyMoney > m_i64MyMoney ? m_i64MyMoney : m_i64OldMyMoney;
        m_textMyMoney->setString(StringUtils::format("%s", Tools::addComma(m_i64OldMyMoney).c_str()));
        if (m_i64oldAllScore == 0) {
            m_textMyMoney->setString(StringUtils::format("%s", Tools::addComma(m_i64MyMoney).c_str()));
            
            unschedule(schedule_selector(GameTableUI::updateCashOutTimer));
            if (m_nOtherEffectID > 0)
            {
                HNAudioEngine::getInstance()->stopEffectWithID(m_nOtherEffectID);
            }
            endAnimation();
        }
    }
    void GameTableUI::updateGetScoreTimer(float dt)
    {   
        m_i64oldAllScore += m_cashInNum;
        m_i64oldAllScore = m_i64oldAllScore > m_i64AllScore ? m_i64AllScore : m_i64oldAllScore;
        
        m_getScore->setString(StringUtils::format("$ %s", Tools::addComma(m_i64oldAllScore).c_str()));
        
        m_i64WinScore -= m_cashInNum;
        m_i64WinScore = m_i64WinScore < 0 ? 0 : m_i64WinScore;
        m_getScoreBigNodeShow->setVisible(true);
        m_getScoreBigNodeShow->setString(StringUtils::format("+ %s", Tools::addComma(m_i64WinScore).c_str()));
        if (m_i64WinScore == 0) {
            m_i64oldAllScore = m_i64AllScore;
            m_getScore->setString(StringUtils::format("$ %s", Tools::addComma(m_i64oldAllScore).c_str()));
            m_getScoreBigNodeShow->runAction(Sequence::create(DelayTime::create(0.3f),CallFunc::create([this](){
                m_getScoreBigNodeShow->setVisible(false);
            }),NULL));
            unschedule(schedule_selector(GameTableUI::updateGetScoreTimer));
            if (m_nOtherEffectID > 0)
            {
                HNAudioEngine::getInstance()->stopEffectWithID(m_nOtherEffectID);
            }
            endAnimation();
        }
        
    }
    void GameTableUI::endAnimation()
    {
     
        m_bAnimation = false;
        if (nFreeCount > 0) {
            startBtnCallback(m_btnStart,Widget::TouchEventType::ENDED);
        }
        else
        {
            m_btnStop->setVisible(false);
            m_btnStart->setVisible(true);
            setBtnsTouchEnabled(m_btnStart,true);
            setBtnsTouchEnabled(m_btnScore,true);
            setBtnsTouchEnabled(m_btnAddNote,true);
            setBtnsTouchEnabled(m_btnSubNote, true);
        }
    }
    void GameTableUI::showCaijinEffect()
    {
        HNAudioEngine::getInstance()->playEffect(EIGHT_SLOT_CAIJIN);
        auto imgCaijin = ImageView::create("Games/31000512/rule/img_CaiJinTouJiangChuMo.png");
        if (imgCaijin) {
            imgCaijin->setPosition(WIN_SIZE/2);
            addChild(imgCaijin);
            imgCaijin->setScale(0.2f);
            imgCaijin->runAction(Sequence::create(ScaleTo::create(1.0f,2.0),DelayTime::create(1.0f),CallFunc::create([this](){
                    endAnimation();
            }),RemoveSelf::create(),NULL));
        }
    }
    void GameTableUI::showTipGetScoreEffect()
    {
        auto imgShouFen = ImageView::create("Games/31000512/rule/img_BuZouShouFen.png");
        if (imgShouFen) {
            imgShouFen->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            imgShouFen->setPosition(Vec2(WIN_SIZE.width/2,140));
            addChild(imgShouFen);
            imgShouFen->runAction(Sequence::create(DelayTime::create(1.0f),RemoveSelf::create(),NULL));
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
}
