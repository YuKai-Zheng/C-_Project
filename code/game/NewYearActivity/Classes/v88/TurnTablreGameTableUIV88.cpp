#include "TurnTablreGameTableUIV88.h"
#include "NewYearGameTableLogic.h"
//#include "HNUIExport.h"
//#include "HNLobbyExport.h"
#include "HNLobby/FontConfig.h"
#include "HNLobby/state/PlatformState.h"
#include "HNLobby/pattern/StateManager.h"
#include "HNLobby/command/ShowGamePrompt.h"
#include "HNLobby/pattern/CommandManager.h"
#include "NSAnimationTool.h"
#include "HNLobby/GamePrompt.h"
#include "../../../../code/common/LibHNUI/UI/UIGoldRain.h"
#include "HNLobby/v88/PlatformResV88.h"

namespace NewYearActivity {

#define WINSIZE Director::getInstance()->getWinSize()
#define COUNT		66

//初始阶段减速速度
#define O_SPEED 0.8
	//均速
#define U_SPEED 0.02
	//最后阶段减速速度
#define I_SPEED	0.02

#define TEST 2

static const char* AUDIO_BG_MUSIC_PATH = "music/NewYearActivity/NewYearBG.mp3";

//csb
static const char* LUCKEY_TURNTABLE_CSB = "csb/v66/activity/turntable_v88.csb";

//animation

static const char* ANIMATION_LOTTERY_V88 = "v88/animation/luckyturntable/luckyturntable_jiangli/luckyturntable_jiangli.csb";
static const char* ANIMATION_WENZI_V88 = "v88/animation/luckyturntable/luckyturntable_wenzi/luckyturntable_wenzi.csb";
static const char* ANIMATION_TURN_V88 = "v88/animation/luckyturntable/luckyturntable_dial/luckyturntable_dial.csb";


HNGameUIBase* LcukyTurnTableV88::create(BYTE bDeskIndex, bool bAutoCreate) {
	LcukyTurnTableV88* tableUI = new LcukyTurnTableV88();
    if (tableUI && tableUI->init(bDeskIndex, bAutoCreate)) {
        tableUI->autorelease();
    } else {
        delete tableUI;
        tableUI = nullptr;
    }
    return tableUI;
}

LcukyTurnTableV88::LcukyTurnTableV88()
	:_initalJumpCount(0),
	_blinkIdx(0),
	_isLasturnCount(0),
	m_winArea(0),
	_isLastTage(false),
	_isSelect(false),
     m_Notice(nullptr) {
	_isPlayer = false;
//    TextureCache::getInstance()->addImage(NN_GOLD_IMAGE);
}

LcukyTurnTableV88::~LcukyTurnTableV88() {
    CC_SAFE_DELETE(_GameTableLogic);
}

bool LcukyTurnTableV88::init(BYTE bDeskIndex, bool bAutoCreate) {
    if (!HNGameUIBase::init()) {
        return false;
    }

	auto loadingBG = ImageView::create("v66/activityBg.png");
	loadingBG->setTouchEnabled(true);
	loadingBG->setPosition(Vec2(VISIBLE_SIZE.width / 2, VISIBLE_SIZE.height / 2)); // visibleOrigin +
	float _xScale = VISIBLE_SIZE.width / loadingBG->getContentSize().width;
	float _yScale = VISIBLE_SIZE.height / loadingBG->getContentSize().height;
	loadingBG->setScaleX(_xScale);
	loadingBG->setScaleY(_yScale);
	this->addChild(loadingBG, -1);

	loadingBG->addTouchEventListener([=](Ref * sender, Widget::TouchEventType type){
		if (type == Widget::TouchEventType::ENDED)
		if (_isSelect == false)
		{
			if (_isPlayer)
			{
				//退出当前房间
				_GameTableLogic->stop();
				RoomLogic()->closeRoom();
				PlatformState* state = PlatformState::create();
				state->setType(GAMELIST);
				StateManager::getInstance()->setState(state);
			}
		}
	});

	_GameTableLogic = new GameTableLogic(this, bDeskIndex, bAutoCreate, PLAYER_COUNT);

    initUI();

	_GameTableLogic->enterGame();
	_GameTableLogic->sendIsPlay(PlatformLogic()->getInstance()->loginResult.dwUserID);

    return true;
}

void LcukyTurnTableV88::onEnter() {
    HNGameUIBase::onEnter();
    HNAudioEngine::getInstance()->playBackgroundMusic(AUDIO_BG_MUSIC_PATH, true);
}

void LcukyTurnTableV88::onExit() {
    HNGameUIBase::onExit();
}

void LcukyTurnTableV88::onExitTransitionDidStart() {
    HNLayer::onExitTransitionDidStart();
    HNAudioEngine::getInstance()->stopBackgroundMusic();
}

void LcukyTurnTableV88::initUI() {

	//奖品类型  0 - 11		 0/6 百万 1/7 1W	2/8	再来一次	3/9 10W 4/10 谢谢 5 /11 1W
//	setBackGroundImage("v99/main_interface/map.png");
	auto root = CSLoader::createNode(LUCKEY_TURNTABLE_CSB);
	root->setPosition(WINSIZE / 2);
	this->addChild(root);

	_view.root = root;
	_view.layout = static_cast<Layout*>(_view.root->getChildByName("Panel"));

	//bg title
	auto bg = static_cast<ImageView*>(_view.layout->getChildByName("bg"));
	bg->setTouchEnabled(true);
	bg->addTouchEventListener([=](Ref * sender, Widget::TouchEventType type){
		if (type != Widget::TouchEventType::ENDED)
			return;
		if (_isSelect == false)
		{
			//退出当前房间
            _GameTableLogic->stop();
			RoomLogic()->closeRoom();
			PlatformState* state = PlatformState::create();
			state->setType(GAMELIST);
			StateManager::getInstance()->setState(state);
		}
	});
	_view.Sprite_turnBg = static_cast<Sprite*>(_view.layout->getChildByName("Sprite_turntableBg"));
	auto Sprite_title = static_cast<Sprite*>(_view.layout->getChildByName("Sprite_title"));
	auto Sprite_zi = static_cast<Sprite*>(_view.layout->getChildByName("Sprite_zi"));
	_view.blinkWenzi = static_cast<Sprite*>(_view.layout->getChildByName("Sprite_blink"));
	auto BG_1 = static_cast<Sprite*>(_view.layout->getChildByName("BG_1"));

	auto ziSprSize = Sprite_title->getContentSize();

	//button
	_view.Button_arrow = static_cast<Button*>(_view.layout->getChildByName("Button_arrow"));
	_view.Button_arrow->addTouchEventListener(CC_CALLBACK_2(LcukyTurnTableV88::btnCallBack, this));
	_view.Button_arrow->setSwallowTouches(true);
	_view.Button_arrow->setTouchEnabled(true);
	//blink node
	auto blinkNode = static_cast<Node*>(_view.layout->getChildByName("Node_blink"));
	int tag = 0;
	for (auto child : blinkNode->getChildren())
	{
		auto spr = static_cast<Sprite*>(child);
		spr->setTag(tag);
		_vecBlink.push_back(spr);
		tag++;
	}

	schedule(schedule_selector(LcukyTurnTableV88::blinkTime), 0.9 *O_SPEED);
}

//关闭回调
void LcukyTurnTableV88::closeEvent(Ref* pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED)
        return;
    //退出当前房间
    _GameTableLogic->stop();
    RoomLogic()->closeRoom();
    PlatformState* state = PlatformState::create();
    state->setType(GAMELIST);
    StateManager::getInstance()->setState(state);
}

void LcukyTurnTableV88::reseatData()
{
	_blinkIdx = 0;
	_isLasturnCount = 0;	// 均速旋转计数
	_islastCount = 0;			//最后减速旋转计算
	_toutleLastTurnCount = 0;	//最后需要旋转总数
	_isLastTage = false;		//是否为旋转最后阶段
	_isRandomTurn = true;
	//初始跳动次数
	_initalJumpCount = 0;
	_onceMore = false;
	_isSelect = false;
	_GameTableLogic->sendTurnTableResult(PlatformLogic()->getInstance()->loginResult.dwUserID);

	this->stopAllActions();
}

void LcukyTurnTableV88::btnCallBack(Ref* pSender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	_GameTableLogic->sendTurnTableResult(PlatformLogic()->getInstance()->loginResult.dwUserID);

	onGameTurnTableResult(4);
}

void LcukyTurnTableV88::blinkTime(float dt)
{
	srand(int(time(0)) + rand());
	auto tag = rand() % _vecBlink.size();

	for (auto node : _vecBlink)
	{
		if (_isLastTage != true){
			if (node->getTag() == tag)
			{
				node->setVisible(true);
				_blinkIdx = tag;
			}
			else
			{
				node->setVisible(false);
			}
		}
		else
		{
			if (_blinkIdx >= _vecBlink.size())	//重置
				_blinkIdx = 0;
 			node->setVisible(false);
			if (node->getTag() == _blinkIdx)
			{
				node->setVisible(true);
			}	
		}
	}
	if (!_isRandomTurn)
	{
		if (_isLasturnCount++ >= COUNT && _isSelect == true)
		{
			_isSelect = false;
			unschedule(schedule_selector(LcukyTurnTableV88::blinkTime));
			_isLasturnCount = 0;

			_islastCount = _blinkIdx > m_winArea ? m_winArea - _blinkIdx + _vecBlink.size() : m_winArea - _blinkIdx + _vecBlink.size();
			_toutleLastTurnCount = _islastCount;
			return;
		}
	}
	_blinkIdx++;
}

void LcukyTurnTableV88::animationCallBack(float dt)
{
	// logo 闪闪
	auto wenZiCsb = CSLoader::createNode(ANIMATION_WENZI_V88);
	wenZiCsb->setPosition(Vec2(_view.blinkWenzi->getContentSize().width *0.5f - 9, _view.blinkWenzi->getContentSize().height *0.5f - 1));
	_view.blinkWenzi->addChild(wenZiCsb, 100);

	auto blinkWenzi = CSLoader::createTimeline(ANIMATION_WENZI_V88);
	_view.blinkWenzi->runAction(blinkWenzi);
	blinkWenzi->play("luckyturntable_wenzi", true);

	//奖品类型  0 - 11		 0/6 百万 1/7 1W	2/8	再来一次	3/9 10W 4/10 谢谢 5 /11 1W
	auto resultCsb = CSLoader::createNode(ANIMATION_LOTTERY_V88);
	resultCsb->setName("resultCsb");
	resultCsb->setPosition(WINSIZE / 2.0);
	this->addChild(resultCsb,100);

	auto  jiangli = static_cast<Node*>(resultCsb->getChildByName("jiangli"));
	auto  jiangpin = static_cast<Node*>(jiangli->getChildByName("jiangping"));

	auto reward = static_cast<Sprite*>(jiangpin->getChildByName("jingbi"));
	auto wenzi = static_cast<Sprite*>(jiangpin->getChildByName("wenzi"));
	wenzi->setVisible(true);

	//奖品类型  0-6/100w  1-5-7-11/1w   2-8/谢谢  3-9/ 10W	4/10 再来一次
	std::string wenziStr;
	std::string rewardStr;
	if (m_winArea > 11 || m_winArea < 0) {
	    m_winArea = 4;//异常值展示谢谢参与
	}
	switch (m_winArea)
	{
				case 0:
				{
							wenziStr = "v66/lucky_turntable/4.png";
							rewardStr = "v66/lucky_turntable/100W.png";
							break;
				}
				case 1:
				{
							wenziStr = "v66/lucky_turntable/jb_01.png";
							rewardStr = "v66/lucky_turntable/5K.png";
							break;
				}
				case 2:
				{
							wenziStr = "v66/lucky_turntable/ka_0.png";
							reward->setVisible(false);
							break;
				}
				case 3:
				{
							wenziStr = "v66/lucky_turntable/5_1.png";
							rewardStr = "v66/lucky_turntable/20W.png";
							break;
				}
				case 4:
				{
							wenziStr = "v66/lucky_turntable/ka_1.png";
							reward->setVisible(false);
							break;
				}
				case 5:
				{
							wenziStr = "v66/lucky_turntable/jb_01.png";
							rewardStr = "v66/lucky_turntable/5W.png";
							break;
				}
				case 6:
				{
							wenziStr = "v66/lucky_turntable/4_1.png";
							rewardStr = "v66/lucky_turntable/50W.png";
							break;
				}
				case 7:
				{
							wenziStr = "v66/lucky_turntable/jb_02.png";
							rewardStr = "v66/lucky_turntable/1W.png";
							break;
				}
				case 8:
				{
							wenziStr = "v66/lucky_turntable/ka_0.png";
							reward->setVisible(false);
							break;
				}
				case 9:
				{
							wenziStr = "v66/lucky_turntable/5.png";
							rewardStr = "v66/lucky_turntable/10W.png";
							break;
				}
				case 10:
				{
							wenziStr = "v66/lucky_turntable/ka_1.png";
							reward->setVisible(false);
							break;
				}
				case 11:
				{
							wenziStr = "v66/lucky_turntable/jb_03.png";
							rewardStr = "v66/lucky_turntable/2W.png";
							break;
				}
	                default:
	                {
	                    wenziStr = "v66/lucky_turntable/ka_1.png";
	                    reward->setVisible(false);
	                    break;
	                }
	}

	    if (wenziStr.compare("") != 0) {
	        wenzi->setTexture(wenziStr);
	    }
	    if (rewardStr.compare("") != 0) {
	        reward->setTexture(rewardStr);
	    }

		auto resultAniMation = CSLoader::createTimeline(ANIMATION_LOTTERY_V88);
		this->runAction(resultAniMation);
		resultAniMation->play("luckyturntable_jiangli", false);

		if (m_winArea == 2 || m_winArea == 8)
		{
			//this->reseatData();
			_view.Button_arrow->setTouchEnabled(true);
			_onceMore = true;
		}
		else
		{
			this->runAction(Sequence::create(DelayTime::create(4.0f), CallFunc::create([=](){
				//退出当前房间
				_GameTableLogic->stop();
				RoomLogic()->closeRoom();
				PlatformState* state = PlatformState::create();
				state->setType(GAMELIST);
				StateManager::getInstance()->setState(state);
			}), nullptr));
			return;
		}
}

void LcukyTurnTableV88::onGameTurnTableResult(int winArea)
{
	m_winArea = winArea;
	_isSelect = true;
	_isPlayer = true;
	_isLasturnCount = 0;
	_view.Button_arrow->setTouchEnabled(false);
	_isRandomTurn = false;

	// 转盘动画
	auto turn = CSLoader::createNode(ANIMATION_TURN_V88);
	turn->setName("turn");
	turn->setPosition(Vec2(_view.Sprite_turnBg->getContentSize().width *0.5f, _view.Sprite_turnBg->getContentSize().height *0.5f));
	_view.Sprite_turnBg->addChild(turn, 100);

	auto spr = turn->getChildByName("Node_1")->getChildByName("Sprite_1");
	if (spr)
	{
		int rotation = 360 / 12 * m_winArea;
		turn->setRotation(-(rotation + 180));
	}
	auto turnline = CSLoader::createTimeline(ANIMATION_TURN_V88);
	_view.Sprite_turnBg->runAction(turnline);
	turnline->play("luckyturntable_dial", false);

	scheduleOnce(schedule_selector(LcukyTurnTableV88::animationCallBack), 6.5);
}

void LcukyTurnTableV88::onHandleNewMessage(int iType, const std::string &message, int iCount)
    {
        //全服公告
        if (iType == 0)
        {
            showNotice(message, 100, iType);
        }
        else if (iType == -11)//银商
        {
            showNotice(message, iCount, iType);
        }
    }

void LcukyTurnTableV88::levelGame()
{
    _GameTableLogic->stop();
	if (RoomLogic()->getRoomRule() & GRR_QUEUE_GAME) {
		RoomLogic()->closeRoom();
		//GamePlatform::returnPlatform(LayerType::ROOMLIST);
		PlatformState* state = PlatformState::create();
		if ("v88") {
			state->setType(GAMELIST);
		}
		else if ("v99") {
			state->setType(GAMELIST_ROOM);
		}
		else {
			state->setType(DESKLIST);
		}
		StateManager::getInstance()->setState(state);

	}
	else {
		RoomLogic()->closeRoom();
		//GamePlatform::returnPlatform(LayerType::ROOMLIST); // 临时处理2016 7 28 返回到房间
		PlatformState* state = PlatformState::create();
		if ("v88") {
			state->setType(GAMELIST);
		}
		else if ("v99") {
			state->setType(GAMELIST_ROOM);
		}
		else {
			state->setType(DESKLIST);
		}
		StateManager::getInstance()->setState(state);
	}
}

void LcukyTurnTableV88::showNotice(const std::string& message, int nCount, int nType, bool bVisible) {
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



