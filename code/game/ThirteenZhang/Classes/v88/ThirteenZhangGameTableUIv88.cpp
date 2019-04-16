#include "ThirteenZhangGameTableUIv88.h"
#include "ThirteenZhangGameTableLogic.h"
//#include "ThirteenZhangControllerLayerV88.h"
#include "ThirteenZhangWrapperv88.h"
//#include "ThirteenZhangSoundLayer.h"
#include "ThirteenZhangSettlementLayerv88.h"
#include "HNNetExport.h"
#include "HNLobbyExport.h"
#include "ThirteenZhangSoundHelper.h"
//#include "../../TTNN/Classes/v88/BRNNSetLayerV88.h"
//#include "../../TTNN/Classes/GameTable/BRNNActions.h"
//#include "ThirteenZhangHelpLayerv88.h"
#include "HNLobby/state/PlatformState.h"
#include "HNLobby/pattern/StateManager.h"
#include "HNLobby/CommonMacro.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "ThirteenZhangResourceMgrv88.h"
#include "base/ccMacros.h"
#include "UI/HNShake.h"
#include "UI/HelpLayer.h"
#include "UI/ToolBarLayer.h"
#include "UI/Scanner.h"
#include "UI/FloatCoin.h"
#include "HNLobby/pattern/factory/gameSet/GameSetFactory.h"
#include "HNLobby/v66/ExtraReward.h"

using namespace HN;
using namespace ThirteenZhang;
//using namespace BRNN;
////////////////////////////////////////////////////////////////////////////////////////////

#define CHILD_LAYER_ZORDER				1000000

#define	LOCAL_ZORDER_CONTROL_LAYER		10000
//#define	TAG_CONTROL_LAYER	CHILD_LAYER_ZORDER

//#define Controller_Tag                  10200

#define	LOCAL_ZORDER_EXIT_LAYER			10001
#define	TAG_EXIT_LAYER		CHILD_LAYER_ZORDER

#define	LOCAL_ZORDER_SET_LAYER			10002
#define	TAG_SET_LAYER		CHILD_LAYER_ZORDER

#define LOCAL_ZORDER_VS_LAYER           10100

#define	LOCAL_ZORDER_SETTLEMENT_LAYER	20000
#define	TAG_SETTLEMENT_LAYER	CHILD_LAYER_ZORDER


#define WINSIZE Director::getInstance()->getWinSize()
#define NOTICE_ZORDER 100
#define DISTANCE 200.0
#define PI 3.1415926

namespace ThirteenZhang {

//	static const char* ToolBar_Path = "v88/common/tool/%s";
	static const int ToolBar_ZOrder = 200;
//	static const int ToolBar_Tag = 901;
//	static const int ToolBar_Setting_Tag = 902;
//	static const int ToolBar_Help_Tag = 903;
//	static const int ToolBar_Exit_Tag = 904;
//	static const int ToolBar_Next_Tag = 905;
	static const int ShakeDunAction_TAG = 666;

	static const int Special_Shape_Prompt_Tag = 668;

//	const static int Open_Fire_Base_Time = 5;//每个动画时间 5 秒

//	static const char* SoundConfigTag = "SoundConfig";
	static const char* ExitTag = "PromptTag";
	static const char* SpecialShapeTag = "SpecialShapeTag";

	static const char*	CARD_STATE_PAIRING_PATH = "v66/zi/5.png";
	static const char*	CARD_STATE_DONE_PATH = "v66/zi/19.png";
//	static const char*  THIRTEEN_GAME_BG = "hallimg/niuniu/0.png";


    static const char*	AUDIO_BIPAI_YIFEN_PATH  = "music/ThirteenZhang/sound/bipaiyingfen.mp3";
//    static const char*    AUDIO_PAIRING_CARD_PATH = "Games/ThirteenZhang/sound/pairing_card.mp3";
//    static const char*    AUDIO_COMPARE_CARD_PATH = "Games/ThirteenZhang/sound/compare_card.mp3";
    static const char*	AUDIO_BG_MUSIC_PATH     = "v66/music/ThirteenZhang_v66/BG_Music.mp3";
    
	const char* AUDIO_START_BIPAI_PATH_V88 = "music/ThirteenZhang/sound/StartBipai.wav";
	const char* AUDIO_FIRE_ONE_BY_ONE_PATH_V88 = "music/ThirteenZhang/sound/GunVoice.wav";
	const char* AUDIO_FIRE_ALL_PATH_V88 = "music/ThirteenZhang/sound/quanleida.mp3";

	const char* PK_ANIMATION_V88 = "v88/animation/common/bipai/bipai_duoren.csb";
	const char* Quanleida_Animation_1_Path_V88 = "common/animation/strafe/direction1.csb";
	const char* Quanleida_Animation_2_Path_V88 = "common/animation/strafe/direction2.csb";
	const char* Quanleida_Zi_Animation_Path_V88 = "v88/animation/thirteencards/quanleida/quanleida_zi.csb";
	const char* Quanleida_Baozha_Animation_Path_V88 = "v88/animation/thirteencards/quanleida/quanleida_baozha.csb";
	const char* Quanleida_Tanke_Animation_Path_V88 = "v88/animation/thirteencards/quanleida/quanleida_tangke.csb";
	static const char* NN_STAR_ANI = "v88/animation/gamestart/gamestart.csb";



	const char* Specialcard_Ani_V88 = "v88/animation/thirteencards/special/specialcard.csb";

//	const int VSAnimation_Tag = 8800;

	const static int ControlLayerTag = 801;

	const int OPENFIRE_EXTRA_WATER = 3;

	HNGameUIBase* GameTableUIV88::create(BYTE bDeskIndex, bool bAutoCreate) {
		GameTableUIV88* tableUI = new GameTableUIV88(bDeskIndex, bAutoCreate);
		if (tableUI->init()) {
			tableUI->autorelease();
			return tableUI;
		}
		else {
			CC_SAFE_DELETE(tableUI);
			return nullptr;
		}
	}

	GameTableUIV88::GameTableUIV88(BYTE bDeskIndex, bool bAutoCreate)
		: _btnReady(nullptr)
		//, _btnCall(nullptr)
		//, _btnUncall(nullptr)
		// , _toolbarBg(nullptr)
		, _dun_first_b(nullptr)
		, _dun_first_f(nullptr)
		, _dun_middle_b(nullptr)
		, _dun_middle_f(nullptr)
		, _dun_last_b(nullptr)
		, _dun_last_f(nullptr)
		, _isPlaying(false)
		, _useSpecialShape(false)
		, _isWaitingSpecialShape(false)
		, _hasReadyClick(false)
		, _autoLeaveNext(false)
		, _dizhuValue(nullptr)
		, _m_animation_OneByOne_dankong(nullptr)
		, _m_animation_OneByOne_sheji(nullptr)
		//    , _m_animation_All_dankong(nullptr)
		//    , _m_animation_All_saoshe(nullptr)
		, _aniIndex(-1)
		, _fireAllAniIdx(-1)
		, v_sprite(nullptr)
		, s_sprite(nullptr)
		, particle_sprite(nullptr)
		//    , _openFire(nullptr)
    , toolBar_flag(true)
		, m_TextTip(nullptr)
		, m_i64TipMoney(0)
		, _specialPrompt(nullptr)
		, _canSendSpecialMsg(false)
		, m_notice(nullptr)
		//    , _myCards(nullptr)
	{
		_myCards.assign(USER_CARD_COUNT, 0);
		_bDeskIndex = bDeskIndex;
		_bAutoCreate = bAutoCreate;
	}

	GameTableUIV88::~GameTableUIV88() {
//		this->unschedule(schedule_selector(GameTableUIV88::autoRemoveNianShouButton));
		HN_SAFE_DELETE(_logic);

		m_notice = nullptr;
	}

	bool GameTableUIV88::init() {
		if (!HNGameUIBase::init()) {
			return false;
		}

		return true;
	}

	void GameTableUIV88::onEnter() {
		HNGameUIBase::onEnter();

		HNAudioEngine::getInstance()->playBackgroundMusic(AUDIO_BG_MUSIC_PATH, true);
		this->setIgnoreAnchorPointForPosition(false);
		this->setPosition(VISIBLE_SIZE.width / 2, VISIBLE_SIZE.height / 2);

//		float _fScaleX = WIN_SIZE.width / DESIGN_SIZE.width;
//		float _fScaleY = WIN_SIZE.height / DESIGN_SIZE.height;

		SpriteFrameCache* cache = SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile("v66/common/card_AMax/cards.plist");
		cache->addSpriteFramesWithFile("Games/12100004/13zhang/teshupai/teshupai_v88.plist");
		//        cache->addSpriteFramesWithFile("Games/ThirteenZhang/game/Gun.plist");

		//触摸
		auto layer = Layer::create();
		addChild(layer);
		layer->setName("top_layer");
		layer->setLocalZOrder(CHILD_LAYER_ZORDER + 1);

		auto dispatcher = Director::getInstance()->getEventDispatcher();
		auto myListener = EventListenerTouchOneByOne::create();
		myListener->onTouchBegan = CC_CALLBACK_2(GameTableUIV88::onTouchBegan, this);
		myListener->setSwallowTouches(false);
		dispatcher->addEventListenerWithSceneGraphPriority(myListener, layer);
		//bg
		_tableBg = Sprite::create(GAME_BG_COMMON_V66);
		addChild(_tableBg);
		_tableBg->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2));
//		_tableBg->setScaleX(_fScaleX);
//		_tableBg->setScaleY(_fScaleY);

		doLoad();

		_logic = new GameTableLogic(this, _bDeskIndex, _bAutoCreate);
		_logic->loadDeskUsersUI();
		_logic->sendGameInfo();
		//    openCardControl();

		//auto moneyText = TextAtlas::create("(+100", "Games/12100004/13zhang/9.png", 21, 33, "(");
		//moneyText->setPosition(WIN_SIZE / 2);
		//char tmp[128];
		//sprintf(tmp, "(+%d)",10);
		//moneyText->setString(tmp);
		//addChild(moneyText,1000);

	}

	void GameTableUIV88::onExitTransitionDidStart() {
		HNGameUIBase::onExitTransitionDidStart();
		HNAudioEngine::getInstance()->stopBackgroundMusic();

		SpriteFrameCache* cache = SpriteFrameCache::getInstance();
		cache->removeSpriteFramesFromFile("Games/12100004/13zhang/13card/cards.plist");
		cache->removeSpriteFramesFromFile("Games/12100004/13zhang/teshupai/teshupai_v88.plist");
	}

	void GameTableUIV88::onExit() {
		_timer_self->stop();
		_timer_whole->stop();
		HNGameUIBase::onExit();
	}





	void GameTableUIV88::doLoad() {
		float offsetSpace = 10.0f;
		auto rootNode = CSLoader::createNode("Games/12100004/MainScene.csb");
		rootNode->setAnchorPoint(Vec2(0.0f, 0.0f));
		rootNode->setPosition(Vec2::ZERO);
		this->addChild(rootNode);

		//auto table1 = dynamic_cast<Sprite*>(rootNode->getChildByName("table_bg"));
		//table1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		//table1->setPosition(0, WIN_SIZE.height / 2);
		//table1->setScaleY(WIN_SIZE.height / table1->getContentSize().height);

	/*	auto shuiyin = dynamic_cast<Sprite*>(rootNode->getChildByName("shuiyin"));
		shuiyin->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 + 30));*/

		auto dizhu = dynamic_cast<Sprite*>(rootNode->getChildByName("dizhu_label"));
		dizhu->setAnchorPoint(Vec2(1.0f, 1.0f));
		dizhu->setPosition(WIN_SIZE.width - offsetSpace, WIN_SIZE.height - offsetSpace);
		//dizhu->setVisible(false);

		_dizhuValue = dynamic_cast<Text*>(dizhu->getChildByName("dizhu_label_value"));
		_dizhuValue->setString("");

        _timer_self = TimerObject::create();
        if (_timer_self) {
            _timer_self->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2);
            _timer_self->setTimeCallBack([&]() {
                onForceStandDesk();
            });
            rootNode->addChild(_timer_self);
        }
        
        _timer_whole = TimerObject::create();
        if (_timer_whole) {
//            _timer_whole->setVisible(false);
            _timer_whole->setTimeCallBack([&]() {
                HelperWrapperV88 search(this);
                {
                    if (this->getChildByTag(ControlLayerTag)) {
                        this->getChildByTag(ControlLayerTag)->removeFromParent();
                    }
                    if (_specialPrompt != nullptr) {
                        _specialPrompt->closePrompt();
                        _specialPrompt = nullptr;
                    }
                }
            });
            _timer_whole->setPosition(WIN_SIZE.width - _timer_whole->getTimerSize().width / 2 - 10.0f, dizhu->getPositionY() - dizhu->getContentSize().height * 2);
            rootNode->addChild(_timer_whole);
        }
        
		HelperWrapperV88 search(rootNode);
		{
			//图像
			char buf[128];
			for (int i = 0; i < PLAY_COUNT; i++) {
				sprintf(buf, "Head_%d", i);
				_players[i].doLoad(search.find<Node>(buf));
				_players[i].setHeadClickEvent(CC_CALLBACK_1(GameTableUIV88::onHeadFrameClick, this));
				_players[i].doRestore();
				_players[i].setTag(i);
			
			}
			float otherWidthLR = _players[1].getContentSize().width;
			float otherHeightLR = _players[1].getContentSize().height;
			float otherWidthU = _players[2].getContentSize().width;
			float otherHeightU = _players[2].getContentSize().height;
			_players[0].setPosition(offsetSpace, offsetSpace);
			_players[0].setSelf(true);
			_players[0].setAlignBottom();
			_players[1].setPosition(offsetSpace,
				WIN_SIZE.height / 2 - otherHeightLR / 2);
			_players[1].setSelf(false);
			_players[1].setAlignLeft();
			_players[1].setReadyLablePos(1);
			_players[2].setPosition(WIN_SIZE.width / 2 - otherWidthU * 1.4f,
				WIN_SIZE.height - otherHeightU - offsetSpace);
			_players[2].setSelf(false);
			_players[2].setAlignTop();
			_players[3].setPosition(WIN_SIZE.width  - otherWidthLR,
				WIN_SIZE.height / 2 - otherHeightLR / 2);
			_players[3].setSelf(false);
			_players[3].setAlignRight();
			_players[3].setReadyLablePos(3);



			for (int i = 0; i < PLAY_COUNT; i++) {
				_pair_state[i] = Sprite::create(CARD_STATE_PAIRING_PATH);
				_pair_state[i]->setVisible(false);
				_pair_state[i]->setTag(i);
				this->addChild(_pair_state[i]);
			}
			_pair_state[0]->setVisible(false);
			_pair_state[0]->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2 + 100);
			_pair_state[1]->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
			_pair_state[1]->setPosition(_players[1].getPosition().x,
				_players[1].getPosition().y - offsetSpace);
			_pair_state[2]->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
			_pair_state[2]->setPosition(_players[2].getPosition().x,
				_players[2].getPosition().y - offsetSpace);
			_pair_state[3]->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
			_pair_state[3]->setPosition(_players[3].getPosition().x + otherWidthLR,
				_players[3].getPosition().y - offsetSpace);

//            float timerWidth = 100;
//            _timer[0].setSelf(true);
//            _timer[0].setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2);
//            _timer[1].setPosition(_players[1].getPosition().x + timerWidth / 2,
//                _players[1].getPosition().y + timerWidth / 2 - offsetSpace);
//            _timer[2].setPosition(_players[2].getPosition().x + timerWidth / 2,
//                _players[2].getPosition().y + timerWidth / 2 - offsetSpace * 2.0f);
//            _timer[3].setPosition(_players[3].getPosition().x - timerWidth / 2,
//                _players[3].getPosition().y + timerWidth / 2 - offsetSpace);

			float offsetCard = 120.0f;
			//手牌
			for (int i = 0; i < PLAY_COUNT; i++) {
				sprintf(buf, "HandCard_%d", i);
				_handCard[i].doLoad(search.find<Node>(buf));
				_handCard[i].setVisible(false);
				_handCard[i].setTag(i);
			}
			_handCard[0].setAlignRight();
			_handCard[0].setSelf(true);
			_handCard[1].setAlignRight();
			_handCard[2].setAlignRight();
			_handCard[3].setAlignLeft();
			_handCard[0].setPosition(WIN_SIZE.width / 2,
				_players[0].getPosition().y + _players[0].getContentSize().height * 0.7f + 20);
			_handCard[1].setPosition(_players[1].getPosition().x + otherWidthLR + offsetCard,
				_players[1].getPosition().y + otherHeightLR * 0.65f);
			_handCard[2].setPosition(_players[2].getPosition().x + otherWidthU + offsetCard,
				_players[2].getPosition().y + otherHeightU * 0.3f);
			_handCard[3].setPosition(_players[3].getPosition().x - offsetCard,
				_players[3].getPosition().y + otherHeightLR * 0.65f);

			//换桌，准备
			_btnReady = search.find<ui::Button>("Button_Ready");
			_btnReady->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 - _btnReady->getContentSize().height * 1.4f));
			_btnReady->setVisible(false);
			_btnReady->addClickEventListener(CC_CALLBACK_1(GameTableUIV88::onReadyClick, this));
			//         //叫庄
			//_btnCall = AUIButton::create("button/6.png", "button/6.png");
			//auto size_call = _btnCall->getContentSize();
			//_btnCall->setName("call");
			//_btnCall->setTitle("button/btn_jiaozhuang_13.png");
			//_btnCall->setTitleOffset(Vec2(-4, 4));
			//_btnCall->setTag(1);
			//_btnCall->setPosition(Vec2(_btnReady->getPosition().x - size_call.width*0.55, _btnReady->getPosition().y));
			//_btnCall->addTouchEventListener(CC_CALLBACK_1(GameTableUIV88::onCallClick, this));
			//_btnCall->setVisible(false);
			//this->addChild(_btnCall);
			////不叫
			//_btnUncall = AUIButton::create("button/5.png", "button/5.png");
			//auto size_uncall = _btnUncall->getContentSize();
			//_btnUncall->setName("uncall");
			//_btnUncall->setTitle("button/btn_giveup_13.png");
			//_btnUncall->setTitleOffset(Vec2(-4, 4));
			//_btnUncall->setTag(0);
			//_btnUncall->setPosition(Vec2(_btnReady->getPosition().x + size_call.width*0.55, _btnReady->getPosition().y));
			//_btnUncall->addTouchEventListener(CC_CALLBACK_1(GameTableUIV88::onCallClick, this));
			//_btnUncall->setVisible(false);
			//this->addChild(_btnUncall);
			//帮助提示
			// _dun_middle_b = dynamic_cast<Sprite*>(rootNode->getChildByName("dun_middle_b"));
			// _dun_middle_f = dynamic_cast<Sprite*>(_dun_middle_b->getChildByName("dun_middle_f"));
			// _dun_first_b = dynamic_cast<Sprite*>(rootNode->getChildByName("dun_first_b"));
			//_dun_first_f = dynamic_cast<Sprite*>(_dun_first_b->getChildByName("dun_first_f"));
			//  _dun_last_b = dynamic_cast<Sprite*>(rootNode->getChildByName("dun_last_b"));
			//  _dun_last_f = dynamic_cast<Sprite*>(_dun_last_b->getChildByName("dun_last_f"));
			//
			//   _dun_middle_b->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 + _btnReady->getContentSize().height * 1.7));
			// _dun_middle_f->setPosition(Vec2(_dun_middle_b->getContentSize().width / 2, _dun_middle_b->getContentSize().height / 2));
			//  _dun_first_b->setPosition(Vec2(_dun_middle_b->getPositionX() - _dun_middle_b->getContentSize().width / 2 - _dun_first_b->getContentSize().width / 2, _dun_middle_b->getPositionY()));
			//  _dun_first_f->setPosition(Vec2(_dun_first_b->getContentSize().width / 2, _dun_first_b->getContentSize().height / 2));
			//   _dun_last_b->setPosition(Vec2(_dun_middle_b->getPositionX() + _dun_middle_b->getContentSize().width / 2 + _dun_last_b->getContentSize().width / 2, _dun_middle_b->getPositionY()));
			//  _dun_last_f->setPosition(Vec2(_dun_last_b->getContentSize().width / 2, _dun_last_b->getContentSize().height / 2));
			//
			restoreDaoshuiButton();
		}

		//菜单
		initToolBar();
	}

	//void GameTableUIV88::initToolBar() {
	//	char resStr[128] = { 0 };
	//	sprintf(resStr, ToolBar_Path, "1.png");
	//	auto toobarBtn1 = Button::create(resStr, resStr);

	//	toolBar_Size = toobarBtn1->getContentSize();

	//	toobarBtn1->setShowEffect(false);
	//	toobarBtn1->setVisible(false);
	//	toobarBtn1->setAnchorPoint(Vec2(0.5, 0.5f));
	//	toobarBtn1->setPosition(Vec2(toolBar_Size.width *0.5f + 10, WINSIZE.height - toolBar_Size.height*0.5f - 10)); //A  pos
	//	toobarBtn1->setTag(em_toolbarBtn);
	//	this->addChild(toobarBtn1, 9999);
	//	toobarBtn1->setVisible(true);
	//	toobarBtn1->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::toolBarClickCallback, this));

	//	center = Node::create();
	//	center->setAnchorPoint(Vec2(0.5f, 0.5f));
	//	center->setPosition(toobarBtn1->getPosition());
	//	this->addChild(center, 99);

	//	float hudu = 0;
	//	auto angle = 18;
	//	//角度转弧度

	//	hudu = 4.0 / 180.0f * PI;
	//	auto distance = (DISTANCE - 45.0) / 1280.0f *WINSIZE.width;
	//	sprintf(resStr, ToolBar_Path, "1_2.png");
	//	auto helpBtn = Button::create(resStr, resStr);
	//	helpBtn->setShowEffect(false);
	//	helpBtn->setAnchorPoint(Vec2(0.5, 0.5));
	//	helpBtn->setScale(0.8f);
	//	helpBtn->setTag(em_toolbarHelpBtn);
	//	helpBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::toolBarClickCallback, this));
	//	center->addChild(helpBtn);
	//	_vecToolbarPoints.push_back(Vec2(distance*cos(hudu), distance*sin(hudu)));
	//	distance = (DISTANCE - 35.0) / 1280.0f *WINSIZE.width;
	//	_vecToolbarPoints2.push_back(Vec2(distance*cos(hudu), distance*sin(hudu)));
	//	_vecMenuButtonKids.push_back(helpBtn);

	//	hudu = -28.0 / 180.0f * PI;
	//	distance = (DISTANCE - 45.0) / 1280.0 *WINSIZE.width;
	//	sprintf(resStr, ToolBar_Path, "1_1.png");
	//	auto setBtn = Button::create(resStr, resStr);
	//	setBtn->setShowEffect(false);
	//	setBtn->setAnchorPoint(Vec2(0.5, 0.5));
	//	setBtn->setScale(0.8f);
	//	//setBtn->setPosition();
	//	setBtn->setTag(em_toolbarSetBtn);
	//	setBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::toolBarClickCallback, this));
	//	center->addChild(setBtn);
	//	_vecToolbarPoints.push_back(Vec2(distance*cos(hudu), distance*sin(hudu)));
	//	distance = (DISTANCE - 35.0) / 1280.0f *WINSIZE.width;
	//	_vecToolbarPoints2.push_back(Vec2(distance*cos(hudu), distance*sin(hudu)));
	//	_vecMenuButtonKids.push_back(setBtn);


	//	hudu = -61.0 / 180.0f * PI;
	//	distance = (DISTANCE - 45.0) / 1280.0 *WINSIZE.width;
	//	sprintf(resStr, ToolBar_Path, "1_3.png");
	//	auto leveBtn = Button::create(resStr, resStr);
	//	leveBtn->setShowEffect(false);
	//	leveBtn->setAnchorPoint(Vec2(0.5, 0.5));
	//	leveBtn->setTag(em_toolbarLeaveBtn);
	//	leveBtn->setScale(0.8f);
	//	leveBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::toolBarClickCallback, this));
	//	center->addChild(leveBtn);
	//	_vecToolbarPoints.push_back(Vec2(distance*cos(hudu), distance*sin(hudu)));
	//	distance = (DISTANCE - 35.0) / 1280.0f *WINSIZE.width;
	//	_vecToolbarPoints2.push_back(Vec2(distance*cos(hudu), distance*sin(hudu)));
	//	_vecMenuButtonKids.push_back(leveBtn);

	//	hudu = -94.0 / 180.0f * PI;
	//	distance = (DISTANCE - 45.0) / 1280.0 *WINSIZE.width;
	//	sprintf(resStr, ToolBar_Path, "1_4.png");
	//	auto nextBtn = Button::create(resStr, resStr);
	//	nextBtn->setShowEffect(false);
	//	nextBtn->setAnchorPoint(Vec2(0.5, 0.5));
	//	nextBtn->setScale(0.8f);
	//	nextBtn->setTag(em_toolbarNextOutBtn);
	//	nextBtn->setUserData((void*)false);
	//	nextBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::toolBarClickCallback, this));
	//	center->addChild(nextBtn);
	//	_vecToolbarPoints.push_back(Vec2(distance*cos(hudu), distance*sin(hudu)));
	//	distance = (DISTANCE - 35.0) / 1280.0f *WINSIZE.width;
	//	_vecToolbarPoints2.push_back(Vec2(distance*cos(hudu), distance*sin(hudu)));
	//	_vecMenuButtonKids.push_back(nextBtn);


	//	sprintf(resStr, ToolBar_Path, "1_5.png");
	//	auto light = ImageView::create(resStr);
	//	light->setScale(0.8f);
	//	light->setTag(em_toolbarNextOutLight);
	//	light->setPosition(Vec2(nextBtn->getContentSize().width*0.5f, nextBtn->getContentSize().height*0.5f));
	//	light->setVisible(false);
	//	nextBtn->addChild(light);
	//}


	//void GameTableUIV88::toolBarClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
	//{
	//	if (touchtype != Widget::TouchEventType::ENDED)
	//		return;
	//	Node* btn = (Node*)pSender;
	//	switch (btn->getTag()) {
	//	case em_toolbarBtn: {
	//							if (toolBar_flag == true)
	//							{
	//								auto btn = static_cast<Button*>(this->getChildByTag(em_toolbarBtn));
	//								if (btn)
	//									btn->setTouchEnabled(false);
	//								_vecMenuButtonKids.at(0)->runAction(Sequence::create(DelayTime::create(0.1 * 0),
	//									MoveTo::create(0.1, _vecToolbarPoints.at(0))
	//									, MoveTo::create(0.1, _vecToolbarPoints2.at(0))
	//									, MoveTo::create(0.1, _vecToolbarPoints.at(0)), nullptr));
	//								_vecMenuButtonKids.at(1)->runAction(Sequence::create(DelayTime::create(0.1 * 1),
	//									MoveTo::create(0.1, _vecToolbarPoints.at(1))
	//									, MoveTo::create(0.1, _vecToolbarPoints2.at(1))
	//									, MoveTo::create(0.1, _vecToolbarPoints.at(1)), nullptr));
	//								_vecMenuButtonKids.at(2)->runAction(Sequence::create(DelayTime::create(0.1 * 2),
	//									MoveTo::create(0.1, _vecToolbarPoints.at(2))
	//									, MoveTo::create(0.1, _vecToolbarPoints2.at(2))
	//									, MoveTo::create(0.1, _vecToolbarPoints.at(2)), nullptr));
	//								_vecMenuButtonKids.at(3)->runAction(Sequence::create(DelayTime::create(0.1 * 3)
	//									, MoveTo::create(0.1, _vecToolbarPoints.at(3))
	//									, MoveTo::create(0.1, _vecToolbarPoints2.at(3))
	//									, MoveTo::create(0.1, _vecToolbarPoints.at(3)), CallFunc::create([this](){
	//									auto btn = static_cast<Button*>(this->getChildByTag(em_toolbarBtn));
	//									if (btn)
	//									{
	//										btn->setTouchEnabled(true);
	//										btn->setColor(Color3B::GRAY);
	//									}
	//									toolBar_flag = !toolBar_flag;
	//								}), nullptr));
	//							}
	//							else
	//							{
	//								_vecMenuButtonKids.at(0)->runAction(Sequence::create(MoveTo::create(0.05, Vec2(0, 0)), nullptr));
	//								_vecMenuButtonKids.at(1)->runAction(Sequence::create(MoveTo::create(0.05, Vec2(0, 0)), nullptr));
	//								_vecMenuButtonKids.at(2)->runAction(Sequence::create(MoveTo::create(0.05, Vec2(0, 0)), nullptr));
	//								_vecMenuButtonKids.at(3)->runAction(Sequence::create(MoveTo::create(0.05, Vec2(0, 0)), CallFunc::create([&](){
	//									auto btn = this->getChildByTag(em_toolbarBtn);
	//									if (btn)
	//									{
	//										btn->setColor(Color3B::WHITE);
	//									}
	//									toolBar_flag = !toolBar_flag;
	//								}), nullptr));
	//							}
	//							break;
	//	}

	//	case em_toolbarSetBtn: {
	//							   if (getChildByName(SoundConfigTag) == nullptr && getChildByName(ExitTag) == nullptr && getChildByName(SpecialShapeTag) == nullptr) {
	//								   auto setLayer = BRNNSetLayer::create();
	//								   setLayer->setName(SoundConfigTag);
	//								   setLayer->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2);
	//								   setLayer->showSet(this, 1000);
	//							   }
	//							   break;
	//	}
	//	case  em_toolbarHelpBtn: {
	//								 if (this->getChildByName("helpLayer")) {
	//									 BRNN::Actions::actionOut(this->getChildByName("helpLayer"));
	//									 return;
	//								 }
	//								 auto helpLayer = ThirteenZhang::ThirteenZhangHelpLayerv88::create();
	//								 helpLayer->setName("helpLayer");
	//								 addChild(helpLayer);
	//								 helpLayer->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2);
	//								 BRNN::Actions::actionIn(helpLayer);
	//	}
	//		break;
	//	case em_toolbarLeaveBtn: {
	//								 auto exitFunc = [](bool force) {
	//									 //        _logic->closeRoom();
	//									 //GamePlatform::returnPlatform(DESKLIST);
	//									 RoomLogic()->closeRoom();
	//									 PlatformState* state = PlatformState::create();
	//									 state->setType(ROOMLIST);
	//									 StateManager::getInstance()->setState(state);
	//								 };

	//								 if (btn->getTag() == em_toolbarLeaveBtn) {
	//									 if (getChildByName(ExitTag) == nullptr && getChildByName(SoundConfigTag) == nullptr && getChildByName(SpecialShapeTag) == nullptr) {
	//										 if (_isPlaying || _hasReadyClick) {
	//											 auto Prompt = GamePromptLayer::create();
	//											 Prompt->setName(ExitTag);
	//											 Prompt->showPrompt(GBKToUtf8("游戏过程中不允许离开！"));
	//											 Prompt->setLocalZOrder(LOCAL_ZORDER_EXIT_LAYER);
	//											 Prompt->setTag(TAG_EXIT_LAYER);
	//										 }
	//										 else {
	//											 HNAudioEngine::getInstance()->stopBackgroundMusic();
	//											 //当前未准备或者没有在发牌阶段
	//											 _logic->sendForceQuit(exitFunc);

	//										 }
	//									 }
	//								 }
	//	}
	//		break;
	//	case em_toolbarNextOutBtn: {
	//								   if (btn->getTag() == em_toolbarNextOutBtn) {
	//									   if (getChildByName(ExitTag) == nullptr && getChildByName(SoundConfigTag) == nullptr && getChildByName(SpecialShapeTag) == nullptr) {
	//										   if (btn->getUserData() == (void*)true) {
	//											   btn->setUserData((void*)false);
	//											 //  btn->getChildByName("checkSprite")->setVisible(false);
	//											   _autoLeaveNext = false;
	//											   btn->getChildByTag(em_toolbarNextOutLight)->setVisible(true);
	//										   }
	//										   else if (btn->getUserData() == (void*)false) {
	//											   btn->setUserData((void*)true);
	//											  // btn->getChildByName("checkSprite")->setVisible(true);
	//											   _autoLeaveNext = true;
	//										   }
	//									   }



	//								   }
	//	}
	//		break;

	//	default:
	//		break;
	//	}

	//}

	void GameTableUIV88::toolBarFlag(float dt)
	{
		toolBar_flag = !toolBar_flag;
	}


	void GameTableUIV88::initToolBar() {
        int zOrder = ToolBar_ZOrder;
        auto toolbar = ToolBarLayer::create();
        if (toolbar) {
            toolbar->addImageAndCallBack(TOOL_BAR_EXIT, [=]() {
                if (_isPlaying || _hasReadyClick) {
                    //auto Prompt = GamePromptLayer::create();
                    //Prompt->setName(ExitTag);
                    //Prompt->showPrompt(GetText("limit_leave_in_game"));
                    //Prompt->setLocalZOrder(LOCAL_ZORDER_EXIT_LAYER);
                    //Prompt->setTag(TAG_EXIT_LAYER);
					GamePromptTip* layer = GamePromptTip::create(this, Color4B(0, 0, 0, 160));
					layer->setGamePromptTip(GetText("limit_leave_in_game"));
                }
                else {
                    HNAudioEngine::getInstance()->stopBackgroundMusic();
                    _timer_self->stop();
                    _timer_whole->stop();
//                    for (size_t i = 0; i < PLAY_COUNT; i++) {
//                        BYTE v = _logic->logicToViewSeatNo(i);
//                        _timer[v].kill();
//                    }
                    leaveTheGame();
//                    //当前未准备或者没有在发牌阶段
//                    _logic->sendForceQuit([](bool force) {
//                    });
                }
            });
            toolbar->addImageAndCallBack(TOOL_BAR_SETTING, [=]() {
                IGameSet* gameSet = GameSetFactory::createWithVersion();
                gameSet->setBgPath(AUDIO_BG_MUSIC_PATH);
                gameSet->open(this, Vec2(0, 0), zOrder+5, 0);
            });
            toolbar->addImageAndCallBack(TOOL_BAR_HELP, [=]() {
                auto helpLayer = HN::HelpLayer::create();
                helpLayer->setContentOnView("v66/common/help/13shui.png");
                helpLayer->setName("helpLayer");
                helpLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                helpLayer->setOrignalPosition();
                helpLayer->moveSelfInWithAction();
                addChild(helpLayer, zOrder+5);
            });
            toolbar->create(this, Vec2(10, WIN_SIZE.height), zOrder, 100);
        }
	}





	////////////////////////////////////////////////////////////////////////////////////////////////
	bool GameTableUIV88::onTouchBegan(Touch* touch, Event* event) {
//		if (0 == _toolbarBg->getName().compare("1")) {
//			_toolbarBg->setName("0");
//			_toolbarBg->runAction(CCSequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height)),
//				CallFunc::create([this]() {
//				this->getChildByName("top_layer")->getChildByTag(ToolBar_Tag)->setVisible(true);
//			})));
//			return true;
//		}
		return true;
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
    
    void GameTableUIV88::leaveTheGame() {
        if (!RoomLogic()->connected()) {
            forceLeaveTheGame();
        }
        else {
            _logic->HN::HNGameLogicBase::sendUserUp();
        }
    }
    
    void GameTableUIV88::forceLeaveTheGame() {
        HNRoomLogic::getInstance()->closeRoom();
        PlatformState* state = PlatformState::create();
        state->setType(ROOMLIST);
        StateManager::getInstance()->setState(state);
    }

//	void GameTableUIV88::onSetClick(cocos2d::Ref *pSender, Control::EventType event) {
//
//		if (this->getChildByName("helpLayer")) {
//            ((HelpLayer*)this->getChildByName("helpLayer"))->moveSelfOutWithAction();
////			this->getChildByName("helpLayer")->removeFromParent();
//		}
//		_toolbarBg->setName("0");
//		_toolbarBg->runAction(Sequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height)),
//			CallFunc::create([this]() {
//			this->getChildByName("top_layer")->getChildByTag(ToolBar_Tag)->setVisible(true);
//		})));
//		if (this->getChildByName(SoundConfigTag)) {
//			BRNN::Actions::actionOut(this->getChildByName(SoundConfigTag));
//			return;
//		}
//		if (getChildByName(SoundConfigTag) == nullptr && getChildByName(ExitTag) == nullptr && getChildByName(SpecialShapeTag) == nullptr) {
//			auto setLayer = BRNNSetLayerV88::create();
//			setLayer->setName(SoundConfigTag);
//			setLayer->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2);
//			setLayer->showSet(this, 1000);
//		}
//	}
//
//	void GameTableUIV88::onExitClick(cocos2d::Ref *pSender, Control::EventType event) {
//		Node* btn = (Node*)pSender;
//		auto exitFunc = [](bool force) {
//		};
//
//		if (btn->getTag() == ToolBar_Exit_Tag) {
//			if (getChildByName(ExitTag) == nullptr && getChildByName(SoundConfigTag) == nullptr && getChildByName(SpecialShapeTag) == nullptr) {
//				if (_isPlaying || _hasReadyClick) {
//					auto Prompt = GamePromptLayer::create();
//					Prompt->setName(ExitTag);
//					Prompt->showPrompt(GBKToUtf8("游戏过程中不允许离开！"));
//					Prompt->setLocalZOrder(LOCAL_ZORDER_EXIT_LAYER);
//					Prompt->setTag(TAG_EXIT_LAYER);
//				}
//				else {
//					HNAudioEngine::getInstance()->stopBackgroundMusic();
//					_timer_self.kill();
//					_timer_whole.kill();
//					for (size_t i = 0; i < PLAY_COUNT; i++) {
//						BYTE v = _logic->logicToViewSeatNo(i);
//						_timer[v].kill();
//					}
//					//当前未准备或者没有在发牌阶段
//					_logic->sendForceQuit(exitFunc);
//
//				}
//			}
//		}
//
//	}
//
//	void GameTableUIV88::onHelpClick(cocos2d::Ref*pSender, Control::EventType event) {
//		if (this->getChildByName(SoundConfigTag)) {
//
//			this->getChildByName(SoundConfigTag)->removeFromParent();
//		}
//		_toolbarBg->setName("0");
//		_toolbarBg->runAction(Sequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height)),
//			CallFunc::create([this]() {
//			this->getChildByName("top_layer")->getChildByTag(ToolBar_Tag)->setVisible(true);
//		})));
//		if (this->getChildByName("helpLayer")) {
//            ((HelpLayer*)this->getChildByName("helpLayer"))->moveSelfOutWithAction();
////			BRNN::Actions::actionOut(this->getChildByName("helpLayer"));
//			return;
//		}
////		auto helpLayer = ThirteenZhang::ThirteenZhangHelpLayerv88::create();
////		helpLayer->setName("helpLayer");
////		addChild(helpLayer);
////		helpLayer->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2);
////		BRNN::Actions::actionIn(helpLayer);
//        auto helpLayer = HN::HelpLayer::create();
//        helpLayer->setContentOnView("v66/common/help/13shui.png");
//        helpLayer->setName("helpLayer");
//        helpLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
//        helpLayer->setOrignalPosition();
//        helpLayer->moveSelfInWithAction();
//        addChild(helpLayer);
//    }
//
//	void GameTableUIV88::onNextGameClick(cocos2d::Ref *pSender, Control::EventType event) {
//		Node* btn = (Node*)pSender;
//
//		if (btn->getTag() == ToolBar_Next_Tag) {
//			if (getChildByName(ExitTag) == nullptr && getChildByName(SoundConfigTag) == nullptr && getChildByName(SpecialShapeTag) == nullptr) {
//				if (btn->getUserData() == (void*)true) {
//					btn->setUserData((void*)false);
//					btn->getChildByName("checkSprite")->setVisible(false);
//					_autoLeaveNext = false;
//				}
//				else if (btn->getUserData() == (void*)false) {
//					btn->setUserData((void*)true);
//					btn->getChildByName("checkSprite")->setVisible(true);
//					_autoLeaveNext = true;
//				}
//			}
//		}
//	}

//    void GameTableUIV88::onOutCardClick(ControllerLayer* controller) {
//        INT heapCount[3] = { 3, 5, 5 };
//        BYTE heapCard[3][5] = { 0 };
//        controller->getCards(0, heapCard[0]);
//        controller->getCards(1, heapCard[1]);
//        controller->getCards(2, heapCard[2]);
//
//        _logic->sendOpenCard(heapCount, heapCard);
//
//        _card_done_state[_logic->getMySeatNo()] = true;
//
//        controller->removeFromParent();
//    }

	//ControllerLayer* GameTableUIV88::openControlLayer() {
	//    HelperWrapper search(this);
	//    auto control = search.find<ControllerLayer>(TAG_CONTROL_LAYER);
	//    if (nullptr == control) {
	//        control = ControllerLayer::create();
	//        control->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height);
	//        control->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	//        control->setOutCardClickEvent(CC_CALLBACK_1(GameTableUIV88::onOutCardClick, this));
	//        control->setManualRequestOpenCard(CC_CALLBACK_1(GameTableUIV88::onManualRequestCardArray, this));
	//        control->playDunFirstDaoshuiAnimationCallBack(CC_CALLBACK_1(GameTableUIV88::firstDaoshuiTip, this));
	//        control->playDunMiddleDaoshuiAnimationCallBack(CC_CALLBACK_1(GameTableUIV88::middleDaoshuiTip, this));
	//        control->playDunAllDaoshuiAnimationCallBack(CC_CALLBACK_1(GameTableUIV88::allDaoshuiTip, this));
	//        addChild(control, LOCAL_ZORDER_CONTROL_LAYER, TAG_CONTROL_LAYER);
	//    }
	//    //        control->setTag(Controller_Tag);
	//    return control;
	//}

	TZhangCardControlV88* GameTableUIV88::openCardControl(const std::vector<BYTE> cards) {
		if (cards.empty()) {
			return nullptr;
		}

		//        std::vector<BYTE> tmpcards;
		//        tmpcards.clear();
		//        for (int i = 0; i < 13; i++) {
		//            tmpcards.push_back(0x00);
		//        }
		//        
		//        tmpcards.at(0) = 0x0D;
		//        tmpcards.at(1) = 0x1D;
		//        tmpcards.at(2) = 0x2D;
		//        tmpcards.at(3) = 0x3D;
		//        tmpcards.at(4) = 0x2C;
		//        tmpcards.at(5) = 0x3C;
		//        tmpcards.at(6) = 0x2B;
		//        tmpcards.at(7) = 0x2A;
		//        tmpcards.at(8) = 0x29;
		//        tmpcards.at(9) = 0x24;
		//        tmpcards.at(10) = 0x34;
		//        tmpcards.at(11) = 0x14;
		//        tmpcards.at(12) = 0x31;

		auto control = TZhangCardControlV88::create();

		if (control) {
			control->setTag(ControlLayerTag);
			control->setLogic(_logic);
			control->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2));
			addChild(control, LOCAL_ZORDER_CONTROL_LAYER);
			control->loadCards(cards);
			//            control->loadCards(tmpcards);
		}
		return control;
	}

//    void GameTableUIV88::onManualRequestCardArray(ControllerLayer* controller) {
//        _logic->requestManualOpenCard();
//    }

	void GameTableUIV88::restoreChildUI() {
		HelperWrapperV88 search(this);
		{
			//        auto control = search.find<ControllerLayer>(TAG_CONTROL_LAYER);
			//        if (nullptr != control) {
			//            control->removeFromParent();
			//        }
			if (this->getChildByTag(ControlLayerTag)) {
				this->getChildByTag(ControlLayerTag)->removeFromParent();
			}


			//            auto special = search.find<GamePromptLayer>(SpecialShapeTag);
			//            auto special = dynamic_cast<GamePromptLayer*>(getChildByTag(Special_Shape_Prompt_Tag));
			if (_specialPrompt != nullptr) {
				_specialPrompt->closePrompt();
				_specialPrompt = nullptr;
			}

//			auto sound = search.find<SoundLayer>(TAG_SET_LAYER);
//			if (nullptr != sound) sound->removeFromParent();

			auto exit = search.find<GamePromptLayer>(TAG_EXIT_LAYER);
			if (nullptr != exit) exit->removeFromParent();
		}
	}

	void GameTableUIV88::onHeadFrameClick(PlayerWrapperV88* player) {
		if (!player->isValid()) return;

	}

//    void GameTableUIV88::onTimerEndEvent(TimerWrapperV88* timer) {
//        onForceStandDesk();
//    }
//
//    void GameTableUIV88::onTimerWholeEndEvent(TimerWrapperV88 *timerWhole) {
//        HelperWrapper search(this);
//        {
//            //        auto control = search.find<ControllerLayer>(TAG_CONTROL_LAYER);
//            //        if (control != nullptr) {
//            //            control->removeFromParent();
//            //        }
//            if (this->getChildByTag(ControlLayerTag)) {
//                this->getChildByTag(ControlLayerTag)->removeFromParent();
//            }
//
//
//            //            auto special = search.find<GamePromptLayer>(SpecialShapeTag);
//            //            auto special = dynamic_cast<GamePromptLayer*>(getChildByTag(Special_Shape_Prompt_Tag));
//            if (_specialPrompt != nullptr) {
//                _specialPrompt->closePrompt();
//                _specialPrompt = nullptr;
//            }
//        }
//    }

	void GameTableUIV88::onSwitchTableClick(cocos2d::Ref *pSender) {

	}

	void GameTableUIV88::onReadyClick(cocos2d::Ref *pSender) {
		//        _isPlaying = true;
		SoundHepler::getInstance()->playButtonSoundEffect();
		//    _hasReadyClick = true;
		_canSendSpecialMsg = false;

		_logic->sendAgreeGame();

		_btnReady->setVisible(false);

		BYTE l = _logic->getMySeatNo();
		BYTE v = _logic->logicToViewSeatNo(l);
//        _timer[v].setVisible(false);
//        _timer[v].kill();
		_timer_self->stop();

		for (size_t i = 0; i < PLAY_COUNT; i++) {
			BYTE v = _logic->logicToViewSeatNo(i);

			_handCard[v].doRestore();
			_card_done_state[i] = false;
		}

		float otherWidthLR = _players[1].getContentSize().width;
		float otherHeightLR = _players[1].getContentSize().height;
		float otherWidthU = _players[2].getContentSize().width;
		float otherHeightU = _players[2].getContentSize().height;
		float offsetCard = 120.0f;

		_handCard[1].setPosition(_players[1].getPosition().x + otherWidthLR + offsetCard,
			_players[1].getPosition().y + otherHeightLR * 0.65f);
		_handCard[2].setPosition(_players[2].getPosition().x + otherWidthU + offsetCard,
			_players[2].getPosition().y + otherHeightU * 0.3f);
		_handCard[3].setPosition(_players[3].getPosition().x - offsetCard,
			_players[3].getPosition().y + otherHeightLR * 0.65f);


		//判断当前的游戏状态，如果有多个玩家且自己准备了，则不能退出
		for (size_t i = 0; i < PLAY_COUNT; i++) {
			if (i == _logic->getMySeatNo()) continue;
			UserInfoStruct* userInfo = _logic->getUserBySeatNo(i);
			if (userInfo == nullptr) {
				continue;
			}
			_hasReadyClick = true;
		}
	}

	void GameTableUIV88::onForceStandDesk() {
        leaveTheGame();
//		auto exitFunc = [](bool force) {
//		};
//		_logic->sendForceQuit(exitFunc);
	}

	//void GameTableUIV88::onCallClick(cocos2d::Ref *pSender)
	//{
	//    ui::Button* call = (ui::Button*)pSender;
	//
	//    _logic->sendUserRobNt(call->getTag() == 1);
	//
	//    _btnCall->setVisible(false);
	//    _btnUncall->setVisible(false);
	//
	//    BYTE lMySeatNo = _logic->getMySeatNo();
	//    BYTE vMySeatNo = _logic->logicToViewSeatNo(lMySeatNo);
	//    _timer[vMySeatNo].kill();
	//}

	void GameTableUIV88::onSettlementTimer() {
		_isPlaying = false;
		_hasReadyClick = false;
		if (_autoLeaveNext) {
			_autoLeaveNext = false;
//            _logic->leaveTheGame();
            leaveTheGame();
            return;
		}

		_btnReady->setVisible(true);

		BYTE lMySeatNo = _logic->getMySeatNo();
		BYTE vMySeatNo = _logic->logicToViewSeatNo(lMySeatNo);

		for (size_t i = 0; i < PLAY_COUNT; i++) {
			if (STATE_ERR == _logic->TableData.m_EnUserState[i]) continue;

			UserInfoStruct* user = _logic->getUserBySeatNo(i);
			if (nullptr == user) continue;

			BYTE v = _logic->logicToViewSeatNo(i);
			_players[v].setGold(user->i64Money);
		}

		_players[vMySeatNo].setStateLabel(PlayerWrapperV88::STATE_LABEL::NONE);
		_handCard[vMySeatNo].doRestore();
//        _timer[vMySeatNo].start(_logic->TableData.m_byBeginTime);//开始配牌倒计时
//        _timer[vMySeatNo].setVisible(false);
		switchCardStateSprite(vMySeatNo, false);
		_timer_self->start(_logic->TableData.m_byBeginTime);
//        _timer_self.setVisible(true);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void GameTableUIV88::onGSFree(const bool userReady[PLAY_COUNT]) {
		for (size_t i = 0; i < PLAY_COUNT; i++) {
			BYTE v = _logic->logicToViewSeatNo(i);

			_handCard[v].doRestore();
//            _timer[v].setVisible(false);
//            _timer[v].doRestore();
			restoreCardStateSprite(v);

			if (userReady[i]) {
				_players[v].setStateLabel(PlayerWrapperV88::STATE_LABEL::READY, false);
			}
			else {
				_players[v].setStateLabel(PlayerWrapperV88::STATE_LABEL::NONE, false);
				if (i == _logic->getMySeatNo()) {
					if (_autoLeaveNext) {
//                        _logic->leaveTheGame();
                        leaveTheGame();
                    }
					else {
						_btnReady->setVisible(true);
//                        _timer[v].start(_logic->TableData.m_byBeginTime);
//                        _timer[v].setVisible(false);
						_timer_self->start(_logic->TableData.m_byBeginTime);
//                        _timer_self.setVisible(true);
						switchCardStateSprite(v, false);
					}
				}
			}
		}
	}

	//void GameTableUIV88::onGSRobNt(BYTE currRobUser)
	//{
	//    // 显示谁不抢
	//    for (size_t i = 0; i < PLAY_COUNT; i++)
	//    {
	//        BYTE v = _logic->logicToViewSeatNo(i);
	//
	//        _timer[v].kill();
	//
	//        // 没有轮到抢庄
	//        if (STATE_NORMAL == _logic->TableData.m_EnUserState[i])
	//        {
	//            _players[v].setStateLabel(PlayerWrapper::STATE_LABEL::NONE, false);
	//        }
	//
	//        // 已经抢过庄，不抢
	//        if (STATE_HAVE_ROB == _logic->TableData.m_EnUserState[i])
	//        {
	//            _players[v].setStateLabel(PlayerWrapper::STATE_LABEL::UNCALL, false);
	//        }
	//    }
	//
	//    //显示抢庄玩家的时钟倒计时
	//    BYTE v = _logic->logicToViewSeatNo(currRobUser);
	//    if (_logic->getMySeatNo() == currRobUser)
	//    {
	//        _btnCall->setVisible(true);
	//        _btnUncall->setVisible(true);
	//    }
	//
	//    _timer[v].start(_logic->TableData.m_byRemaindTime);
	//}

	void GameTableUIV88::onGSSendCard(const std::vector<BYTE>& cards) {

		//_gameStateLabel->loadTexture("ThirteenZhang/game/commonHeadon/call_banker_label.png", ui::Widget::TextureResType::PLIST);
		//_gameStateLabel->setVisible(true);

		////设置庄家
		//BYTE vBanker = _logic->logicToViewSeatNo(_logic->TableData.m_byNowNtStation);
		//_players[vBanker].setStateLabel(PlayerWrapper::STATE_LABEL::BANKER, false);

		//显示各玩家的牌
		BYTE lMySeatNo = _logic->getMySeatNo();
		_timer_whole->start(_logic->TableData.m_byRemaindTime);
		//        _timer_whole.setVisible(true);

		for (size_t i = 0; i < PLAY_COUNT; i++) {
			if (STATE_ERR == _logic->TableData.m_EnUserState[i]) continue;
			if (i == lMySeatNo) continue;

			BYTE v = _logic->logicToViewSeatNo(i);
//            _timer[v].start(_logic->TableData.m_byRemaindTime);
//            _timer[v].setVisible(false);
			switchCardStateSprite(_logic->logicToViewSeatNo(i), false);
		}

		if (STATE_ERR != _logic->TableData.m_EnUserState[lMySeatNo]) {
			HelperWrapperV88 search(this);
			{
				//            auto controlWrapper = search.find<ControllerLayer>(TAG_CONTROL_LAYER);
				//            if (nullptr != controlWrapper) controlWrapper->removeFromParent();
				if (this->getChildByTag(ControlLayerTag)) {
					this->getChildByTag(ControlLayerTag)->removeFromParent();
				}


//				auto setConfig = search.find<SoundLayer>(TAG_SET_LAYER);
//				if (nullptr != setConfig) setConfig->removeFromParent();

				auto exit = search.find<GamePromptLayer>(TAG_EXIT_LAYER);
				if (nullptr != exit) exit->removeFromParent();
			}

			//        auto control =
			openCardControl(cards);
			//        if (control) {
			//            control->loadCards(cards);
			//        }

			//        control->cardBoardTouch(true);
			//            control->startTimer(_logic->TableData.m_byRemaindTime);
			//            _timer_whole.start(_logic->TableData.m_byRemaindTime);
		}
	}

	void GameTableUIV88::onGSOpenCard(const std::vector<BYTE>& cards, const BYTE heapCard[3][5]) {
		for (size_t i = 0; i < PLAY_COUNT; i++) {
			BYTE v = _logic->logicToViewSeatNo(i);
			_players[v].setStateLabel(PlayerWrapperV88::STATE_LABEL::NONE);
//            _timer[v].setVisible(false);
//            _timer[v].kill();
			//switchCardStateSprite(v, true);
			restoreCardStateSprite(v);
		}
		//        _timer_whole.setVisible(false);
		//        _timer_whole.kill();


		////设置庄家
		//BYTE v = _logic->logicToViewSeatNo(_logic->TableData.m_byNowNtStation);
		//_players[v].setStateLabel(PlayerWrapper::STATE_LABEL::BANKER, false);

		BYTE lMySeatNo = _logic->getMySeatNo();
		//自己不是中途加入
		if (STATE_ERR != _logic->TableData.m_EnUserState[lMySeatNo]) {
			BYTE vSeatNo = _logic->logicToViewSeatNo(lMySeatNo);

			//已经摆牌了
			if (STATE_OPEN_CARD == _logic->TableData.m_EnUserState[lMySeatNo]) {
				_handCard[vSeatNo].setVisible(true);
			}
			else {
				openCardControl(cards);
				//            //显示操作容器
				//            auto control = openControlLayer();
				//            control->loadCards(cards);
				//            control->cardBoardTouch(true);
				//            //                control->startTimer(_logic->TableData.m_byRemaindTime);
				//            control->runAction(CCEaseSineOut::create(MoveTo::create(0.2f, Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2))));
			}
		}
		_timer_whole->start(_logic->TableData.m_byRemaindTime);
		//        _timer_whole.setVisible(true);

		//显示其他各玩家的牌
		for (int i = 0; i < PLAY_COUNT; i++) {
			if (STATE_ERR == _logic->TableData.m_EnUserState[i]) continue;

			if (i == lMySeatNo) continue;

			BYTE v = _logic->logicToViewSeatNo(i);

			//如果已经摆牌了 那么就显示摆牌数据
			if (STATE_OPEN_CARD == _logic->TableData.m_EnUserState[i]) {
				_handCard[v].setVisible(true);
			}
			else {
				_handCard[v].setVisible(false);
				//                _timer[v].start(_logic->TableData.m_byRemaindTime);
				//                _timer[v].setVisible(false);
				switchCardStateSprite(v, false);
			}
		}
		//SoundHepler::getInstance()->playerEffects(AUDIO_COMPARE_CARD_PATH);
	}

	void GameTableUIV88::onGSCompareCard(const BYTE heapCard[PLAY_COUNT][3][5]) {
		for (size_t i = 0; i < PLAY_COUNT; i++) {
			BYTE v = _logic->logicToViewSeatNo(i);
			_players[v].setStateLabel(PlayerWrapperV88::STATE_LABEL::NONE);
//            _timer[v].setVisible(false);
//            _timer[v].kill();
			//switchCardStateSprite(v, true);
			restoreCardStateSprite(v);
		}
//        _timer_whole.setVisible(false);
		_timer_whole->stop();


		//SoundHepler::getInstance()->playerEffects(AUDIO_COMPARE_CARD_PATH);

		////设置庄家
		//BYTE v = _logic->logicToViewSeatNo(_logic->TableData.m_byNowNtStation);
		//_players[v].setStateLabel(PlayerWrapper::STATE_LABEL::BANKER, false);

		//显示其他各玩家的牌
		for (int i = 0; i < PLAY_COUNT; i++) {
			if (STATE_ERR == _logic->TableData.m_EnUserState[i]) continue;

			BYTE v = _logic->logicToViewSeatNo(i);

			//如果已经摆牌了 那么就显示摆牌数据
			if (STATE_OPEN_CARD == _logic->TableData.m_EnUserState[i]) {
				_handCard[v].setVisible(true);
			}
		}

		doCompareCard(heapCard);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void GameTableUIV88::onAddPlayer(BYTE lSeatNo, INT userID, bool self) {
// 		char str[100] = { 0 };
// 		sprintf(str, "%s%d.png", "v66/dayday_toux/", UserInfoModule()->getInstance()->findUser(userID)->bLogoID);
		BYTE v = _logic->logicToViewSeatNo(lSeatNo);
		UserInfoStruct* user = _logic->getUserByUserID(userID);
		_players[v].setUserID(userID);
		_players[v].setName(user->nickName);
		_players[v].setGold(user->i64Money);
		_players[v].setSex(user->bBoy);
		_players[v].showName(true);
		_players[v].showGold(true);
		_players[v].setHead(user);
		_players[v].allPosition();
	}

	void GameTableUIV88::onRemovePlayer(BYTE lSeatNo, LLONG userID, bool self) {
		BYTE v = _logic->logicToViewSeatNo(lSeatNo);
		_players[v].doRestore();
//        _timer[v].setVisible(false);
//        _timer[v].doRestore();
		restoreCardStateSprite(v);
		_handCard[v].doRestore();
	}

	void GameTableUIV88::onGameAgree(BYTE lSeatNo, bool self) {
		BYTE v = _logic->logicToViewSeatNo(lSeatNo);
		_players[v].setStateLabel(PlayerWrapperV88::STATE_LABEL::READY);
		_handCard[v].doRestore();

		SoundHepler::getInstance()->playerEffects("music/ThirteenZhang/sound/ready.mp3");

		if (self) {
			_btnReady->setVisible(false);
//            _timer[v].setVisible(false);
//            _timer[v].kill();
			//switchCardStateSprite(v, true);
			_timer_self->stop();
//            _timer_self.kill();
		}
	}

	//void GameTableUIV88::onGameNoticeRobNt(BYTE lSeatNo, bool self)
	//{
	//    //视图位置
	//    _btnReady->setVisible(false);
	//
	//    for (size_t i = 0; i < PLAY_COUNT; i++)
	//    {
	//        BYTE v = _logic->logicToViewSeatNo(i);
	//        _timer[v].doRestore();
	//        _handCard[v].doRestore();
	//
	//        // 没有轮到抢庄
	//        if (STATE_NORMAL == _logic->TableData.m_EnUserState[i])
	//        {
	//            _players[v].setStateLabel(PlayerWrapper::STATE_LABEL::NONE);
	//        }
	//
	//        // 已经抢过庄，不抢
	//        if (STATE_HAVE_ROB == _logic->TableData.m_EnUserState[i])
	//        {
	//            _players[v].setStateLabel(PlayerWrapper::STATE_LABEL::UNCALL);
	//        }
	//    }
	//
	//    BYTE v = _logic->logicToViewSeatNo(lSeatNo);
	//    _timer[v].start(_logic->TableData.m_byRobTime);
	//    _players[v].setStateLabel(PlayerWrapper::STATE_LABEL::NONE);
	//
	//    if (self)
	//    {
	//        _btnCall->setVisible(true);
	//        _btnUncall->setVisible(true);
	//    }
	//}
	//
	//void GameTableUIV88::onGameRobNtResult(BYTE lSeatNo, bool robResult, bool self)
	//{
	//    if (STATE_ERR == _logic->TableData.m_EnUserState[lSeatNo]) return;
	//
	//    BYTE v = _logic->logicToViewSeatNo(lSeatNo);
	//    _timer[v].kill();
	//
	//    if (robResult)
	//    {
	//
	//        for (size_t i = 0; i < PLAY_COUNT; i++)
	//        {
	//            _timer[i].kill();
	//            _players[i].setStateLabel(PlayerWrapper::STATE_LABEL::NONE);
	//        }
	//
	//        _players[v].setStateLabel(PlayerWrapper::STATE_LABEL::BANKER);

	//        SoundHepler::getInstance()->playerEffects("Games/ThirteenZhang/sound/ready.mp3");
	//    }
	//    else
	//    {
	//        _players[v].setStateLabel(PlayerWrapper::STATE_LABEL::UNCALL);
	//    }
	//
	//    if (self)
	//    {
	//        _btnCall->setVisible(false);
	//        _btnUncall->setVisible(false);
	//    }
	//}
	//
	//void GameTableUIV88::onGameMakeSureNt(BYTE lSeatNo, bool self)
	//{
	//    for (size_t i = 0; i < PLAY_COUNT; i++)
	//    {
	//        _timer[i].kill();
	//        _players[i].setStateLabel(PlayerWrapper::STATE_LABEL::NONE);
	//    }
	//
	//    // 设置庄家
	//    BYTE v = _logic->logicToViewSeatNo(lSeatNo);
	//    if (self)
	//    {
	//
	//    }
	//    else
	//    {
	//
	//    }
	//    _players[v].setStateLabel(PlayerWrapper::STATE_LABEL::BANKER);
	//    SoundHepler::getInstance()->playerEffects("Games/ThirteenZhang/sound/ready.mp3");
	//}

	void GameTableUIV88::onGameSendCard() {
		_isPlaying = true;
		for (int i = 0; i < PLAY_COUNT; i++) {
			//switchCardStateSprite(i, true);
//            _timer[i].setVisible(false);
//            _timer[i].kill();
			restoreCardStateSprite(i);
		}
		_btnReady->setVisible(false);

		for (size_t i = 0; i < PLAY_COUNT; i++) {
			BYTE v = _logic->logicToViewSeatNo(i);

			_players[v].setStateLabel(PlayerWrapperV88::STATE_LABEL::NONE);
//            _timer[v].setVisible(false);
//            _timer[v].kill();
			restoreCardStateSprite(v);
		}

		////设置庄家
		//BYTE v = _logic->logicToViewSeatNo(_logic->TableData.m_byNowNtStation);
		//_players[v].setStateLabel(PlayerWrapper::STATE_LABEL::BANKER, false);

		//_gameStateLabel->loadTexture("ThirteenZhang/game/common/open_card_label.png", ui::Widget::TextureResType::PLIST);
		//_gameStateLabel->setVisible(true);

		BYTE l = _logic->getMySeatNo();
		if (STATE_ERR == _logic->TableData.m_EnUserState[l]) return;

		restoreChildUI();
	}

	void GameTableUIV88::onGameLoadControlCard(const std::vector<BYTE> cards) {
		//    auto control =
		openCardControl(cards);
		//    if (control) {
		//        control->setLogic(_logic);
		//        control->setPosition(WIN_SIZE / 2);
		//        addChild(control, LOCAL_ZORDER_CONTROL_LAYER);
		//        control->loadCards(cards);
		//    }

		//    float dt = (float)(_logic->TableData.m_bySendCardTime + 0.5f) / 13;

		////    control->cardBoardTouch(false);
		//    control->runAction(Sequence::create(CCEaseSineOut::create(MoveTo::create(0.3f, Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2))),
		//                                        DelayTime::create(4.5f),
		//    CallFunc::create([&]() {
		//        //            if (control)
		//        {
		//            HelperWrapper search(this);
		//            auto control = search.find<ControllerLayer>(TAG_CONTROL_LAYER);
		//            if (control != nullptr) {
		//                control->cardBoardTouch(true);
		//            }
		//        }
		//    }), nullptr));
	}

	void GameTableUIV88::onGameHasSpecialCard(const std::vector<int> &specialShapeArray, const std::vector<BYTE>& cards) {
		_myCards.assign(USER_CARD_COUNT, 0);
		memcpy(&_myCards[0], &cards[0], sizeof(BYTE)* USER_CARD_COUNT);

		onGameSendCard();



		if (_logic->TableData.m_iSpecialShape[_logic->getMySeatNo()] > COMMON_CARD) {
			_isWaitingSpecialShape = true;
			BYTE cardType = _logic->TableData.m_iSpecialShape[_logic->getMySeatNo()];
			this->runAction(Sequence::create(DelayTime::create(4.0f), CallFunc::create([&]() {
				_canSendSpecialMsg = true;//3s开牌后再发消息给服务端，防止服务端逻辑混乱
			}), nullptr));
			if (_specialPrompt == nullptr) {
				_specialPrompt = GamePromptLayer::create();
				_specialPrompt->setName(SpecialShapeTag);
				_specialPrompt->setTag(Special_Shape_Prompt_Tag);
				int index = _logic->TableData.m_iSpecialShape[_logic->getMySeatNo()] - specialCardNameIndexOffsetValue;
				char str[128];
                sprintf(str, GetText("TZ_Has_Special_Style_Card"), GetText(SpecialCard[index].c_str()));
                _specialPrompt->showPrompt(str);
//                _specialPrompt->setCloseExitBtn();
                _specialPrompt->setOkayCallBack([&]() {
                    onCompareWithSpecialShape();
                    _specialPrompt = nullptr;
                });
                _specialPrompt->setCloseCallBack([&]() {
                    onCompareWithCommonShape();
                    _specialPrompt = nullptr;
                });

				//            _specialPrompt->setCloseCallBack([&]() {
				//                onCompareWithSpecialShape();
				//                _specialPrompt = nullptr;
				//            });
//				_specialPrompt->setColorLayerTouchEnabled(false);
			}

		}
		else {
			onCompareWithCommonShape();
		}
	}


	//特殊拍牌的效果
	void GameTableUIV88::onShowSpecialEffect(int cardType) {

		BYTE v = _logic->getMySeatNo();
		bool sex = _players[v].getSex();
		std::pair<std::string, std::string> cardTypeResource = ResourceMgrV88::getCardTypeResource(sex, cardType);
		/*auto sprite = Sprite::create();
		sprite->setTexture(cardTypeResource.first);
		addChild(sprite,10000);
		sprite->setPosition(Vec2(WIN_SIZE.width / 2,WIN_SIZE.height/2));*/
		auto speNode = CSLoader::createNode(Specialcard_Ani_V88);

		//CCActionInterval *forwardIn = CCFadeIn::create(0.2);
		//sprite->setScale(1.5);
		//sprite->runAction(Sequence::create(DelayTime::create(0.2f), forwardIn, ScaleTo::create(0.4f, 1.0f, 1.0f), DelayTime::create(0.8f), RemoveSelf::create(), nullptr));
		//
		speNode->setPosition(Vec2(WIN_SIZE.width/2, WIN_SIZE.height / 2));
		addChild(speNode);
		if (speNode)
		{
			auto wenziNode = dynamic_cast<Node*>(speNode->getChildByName("Node_4"));
			if (wenziNode)
			{
				auto wenzi = dynamic_cast<Sprite*>(wenziNode->getChildByName("wenzi"));
				if (wenzi)
				{
                    //wenzi->setSpriteFrame(ResourceMgrV88::getTypeTextureFileName(cardTypeResource));
                    wenzi->setSpriteFrame(cardTypeResource.first);
				}
			}
			
			
			
			auto speAction = CSLoader::createTimeline(Specialcard_Ani_V88);
			speNode->runAction(speAction);
			speAction->play("special", false);
		}

	}

	void GameTableUIV88::controlCompareWithSpecialShapeOrNot(int specialShape, bool useShape) {
		int shape = specialShape;
		bool use = useShape;
		if (_canSendSpecialMsg) {
			_canSendSpecialMsg = false;//说明发送消息的操作已经在通知开牌后5秒执行了
			_logic->requestOpenWithSpecialShape(shape, use);
		}
		else {
			_canSendSpecialMsg = false;//说明发送消息的操作已经在通知开牌后5秒执行了
			this->runAction(Sequence::create(DelayTime::create(4.0f), CallFunc::create([=]() {
				_logic->requestOpenWithSpecialShape(shape, use);
			}), nullptr));
		}
	}

	void GameTableUIV88::onCompareWithCommonShape() {
		_useSpecialShape = false;
		_isWaitingSpecialShape = false;
		int specialShape = _logic->TableData.m_iSpecialShape[_logic->getMySeatNo()];
		controlCompareWithSpecialShapeOrNot(specialShape, false);
		bool noCards = true;
		for (int i = 0; i < USER_CARD_COUNT; i++) {
			if (_myCards.at(i) > 0x00) {
				noCards = false;
			}
		}
		if (!noCards) {
			onGameLoadControlCard(_myCards);
		}
	}

	void GameTableUIV88::onCompareWithSpecialShape() {
		_card_done_state[_logic->getMySeatNo()] = true;
		_handCard[_logic->logicToViewSeatNo(_logic->getMySeatNo())].setVisible(true);
		//_handCard[_logic->logicToViewSeatNo(_logic->getMySeatNo())].showSpecialAnimation();	
		_useSpecialShape = true;
		_isWaitingSpecialShape = false;
		int specialShape = _logic->TableData.m_iSpecialShape[_logic->getMySeatNo()];
		controlCompareWithSpecialShapeOrNot(specialShape, true);
		
		
	}

	//void GameTableUIV88::onGameManualOpenCard(void* manualResult) {
	////    auto control = openControlLayer();
	////    control->updateCardsFromOneKeyTanpai(manualResult);
	//}

	void GameTableUIV88::onGameNoticeOpenCard() {
		for (int i = 0; i < PLAY_COUNT; i++) {
			//switchCardStateSprite(i, true);
//            _timer[i].setVisible(false);
//            _timer[i].kill();
			restoreCardStateSprite(i);
		}
		_timer_whole->start(_logic->TableData.m_byThinkTime - 2);
		//SoundHepler::getInstance()->playerEffects(AUDIO_COMPARE_CARD_PATH);


		//_gameStateLabel->loadTexture("ThirteenZhang/game/common/open_card_label.png", ui::Widget::TextureResType::PLIST);
		//_gameStateLabel->setVisible(true);

		BYTE l = _logic->getMySeatNo();

		for (size_t i = 0; i < PLAY_COUNT; i++) {
			if (STATE_ERR == _logic->TableData.m_EnUserState[i]) continue;

			//跳过自己
			if (i == l) continue;

			BYTE v = _logic->logicToViewSeatNo(i);
//            _timer[v].start(_logic->TableData.m_byThinkTime);
//            _timer[v].setVisible(false);
			switchCardStateSprite(v, false);
		}

		if (STATE_ERR == _logic->TableData.m_EnUserState[l]) return;

		SoundHepler::getInstance()->playerEffects("music/ThirteenZhang/sound/ding.mp3");
		//        _timer_self.start(_logic->TableData.m_byThinkTime - 2);
		//        _timer_whole.start(_logic->TableData.m_byThinkTime);
		//        if (!_useSpecialShape && !_isWaitingSpecialShape && _isPlaying)
		{
			//        HelperWrapper search(this);
			//        auto control = search.find<ControllerLayer>(TAG_CONTROL_LAYER);
			//        if (control != nullptr) {
			//            control->cardBoardTouch(true);
			//            //                control->startTimer(_logic->TableData.m_byThinkTime - 2);
			//        }
			//            _timer_self.setPosition(control->getTimerPos());
		}
		//        else {
		//            _timer_self.setPosition(WIN_SIZE / 2);
		//        }
	}

	void GameTableUIV88::onGameOpenCardResult(BYTE lSeatNo) {
		if (STATE_ERR == _logic->TableData.m_EnUserState[lSeatNo]) return;
		if (lSeatNo != _logic->getMySeatNo()) {
			_card_done_state[lSeatNo] = true;
		}

		BYTE v = _logic->logicToViewSeatNo(lSeatNo);
		_handCard[v].setVisible(true);
		if (_logic->TableData.m_iUseSpecialShape[lSeatNo]) {
			_handCard[v].showSpecialAnimation();
		}
//        _timer[v].setVisible(false);
//        _timer[v].kill();
		switchCardStateSprite(v, true);
		//restoreCardStateSprite(v);
		SoundHepler::getInstance()->playerEffects(AUDIO_BIPAI_YIFEN_PATH);
		bool done = true;
		//显示其他各玩家的牌
		for (int i = 0; i < PLAY_COUNT; i++) {
			if (STATE_ERR == _logic->TableData.m_EnUserState[i])
				continue;
			if (i != _logic->getMySeatNo() && !_card_done_state[i]) {
				done = false;
			}
		}
		if (!_card_done_state[_logic->getMySeatNo()]) {
			done = false;
		}
		if (done) {
			//            _timer_whole.setVisible(false);
			_timer_whole->stop();
		}
	}

	void GameTableUIV88::onGameCompareCard(const BYTE heapCard[PLAY_COUNT][3][5]) {
		//SoundHepler::getInstance()->playerEffects(AUDIO_COMPARE_CARD_PATH);
		//        _timer_whole.setVisible(false);
		//        _timer_whole.kill();
		HelperWrapperV88 search(this);
		//    auto control = search.find<ControllerLayer>(TAG_CONTROL_LAYER);
		//    if (nullptr != control) {
		//        control->removeFromParent();
		//    }
		if (this->getChildByTag(ControlLayerTag)) {
			this->getChildByTag(ControlLayerTag)->removeFromParent();
		}

		//        auto special = search.find<GamePromptLayer>(SpecialShapeTag);
		//        auto special = dynamic_cast<GamePromptLayer*>(getChildByTag(Special_Shape_Prompt_Tag));
		if (_specialPrompt != nullptr) {
			_specialPrompt->closePrompt();
			_specialPrompt = nullptr;
		}

		for (size_t i = 0; i < PLAY_COUNT; i++) {
			if (STATE_ERR == _logic->TableData.m_EnUserState[i]) continue;

			BYTE v = _logic->logicToViewSeatNo(i);
//            _timer[v].setVisible(false);
//            _timer[v].kill();
			//switchCardStateSprite(v, true);
			restoreCardStateSprite(v);
		}
		animationVSShow();
		_timer_whole->stop();
		doCompareCard(heapCard);
	}


	//是否播放开始动画
	void GameTableUIV88::onIsCanPlayBeginAni(bool isCanPlay) {
		if (isCanPlay) {
            auto scanner = Scanner::create();
			this->addChild(scanner, 100000);
			scanner->starPlayerEffect();
			scanner->setPosition(250, 60);

			for (size_t i = 0; i < PLAY_COUNT; i++) {
				BYTE v = _logic->logicToViewSeatNo(i);
				_players[v].setStateLabel(PlayerWrapperV88::STATE_LABEL::NONE, false);
			}
			auto starNode = CSLoader::createNode(NN_STAR_ANI);
			starNode->setPosition(WIN_SIZE / 2);
			addChild(starNode);
			if (starNode)
			{
				auto starAction = CSLoader::createTimeline(NN_STAR_ANI);
				starNode->runAction(starAction);
				starAction->play("gamestart", false);
			}

		}
	}


	void GameTableUIV88::onGameOpenFire(S_C_OpenFire* scOpenFire) {
		float aniInternal = 1.5f;//单个打枪动画耗时
		bool hasAllTag = false;
		_fireAllAniIdx = -1;
		int aniCount = 0;
		for (int i = 0; i < PLAY_COUNT; i++) {
			bool tag = true;    //某个玩家赢其他三个玩家
			for (int j = 0; j < PLAY_COUNT; j++) {
				_openFire[i][j] = scOpenFire->bUserFireUser[i][j];
				if (i != j && !_openFire[i][j]) {
					tag = false;
				}
				if (i != j && _openFire[i][j]) {
					aniCount++;
				}
			}
			if (tag) {
				hasAllTag = true;
				_fireAllAniIdx = i;
			}
		}
		for (int i = 0; i < PLAY_COUNT; i++) {
			_extraWater[i] = 0;
			_extraWaterAll[i] = 0;
		}
		for (int i = 0; i < PLAY_COUNT; i++) {
			_extraWater[i] = scOpenFire->bExtraWater[i];
			if (hasAllTag) {
				_extraWaterAll[i] = scOpenFire->bExtraWaterToAll[i];
			}
		}
		//普通打枪
		schedule(schedule_selector(GameTableUIV88::scheduleOpenFireOBO_CB), aniInternal);
		//_fireAllAniIdx = 1;
		//animationOpenFireToAll();
		//全垒打  动画时间 5 s
		if (hasAllTag) {
			this->runAction(Sequence::create(DelayTime::create(aniInternal * (aniCount + 1)),
				CallFunc::create([&]() {
				animationOpenFireToAll();
			}), nullptr));
		}
	}

	void GameTableUIV88::onGameOpenSpecialShape(S_C_OpenSpecialShape *scSpecialShape) {
		memcpy(_logic->TableData.m_iSpecialCompareResult, scSpecialShape->bSpecialResult, sizeof(_logic->TableData.m_iSpecialCompareResult));
		memcpy(_logic->TableData.m_iSpecialShape, scSpecialShape->bSpecialShape, sizeof(_logic->TableData.m_iSpecialShape));
		memcpy(_logic->TableData.m_byUserCards, scSpecialShape->bCards, sizeof(_logic->TableData.m_byUserCards));

		bool specialTag = false;
		float otherWidthLR = _players[1].getContentSize().width;
		float otherHeightLR = _players[1].getContentSize().height;
		float otherWidthU = _players[2].getContentSize().width;
		float otherHeightU = _players[2].getContentSize().height;
		float offsetCard = 120;
		for (size_t i = 0; i < PLAY_COUNT; i++) {
			if (STATE_ERR == _logic->TableData.m_EnUserState[i]) continue;
			std::vector<BYTE> cards;
			BYTE v = _logic->logicToViewSeatNo(i);
			if (scSpecialShape->bSpecialShape[i] > COMMON_CARD) {
				for (int j = 0; j < USER_CARD_COUNT; j++) {
					if (scSpecialShape->bCards[i][j] > 0x00) {
						cards.push_back(scSpecialShape->bCards[i][j]);
					}
				}
				specialTag = true;
				LLONG score = scSpecialShape->bSpecialResult[i];
				BYTE type = scSpecialShape->bSpecialShape[i];
				if (!cards.empty()) {
					_handCard[v].loadCardsLine(cards);
				}
				_handCard[v].setVisible(true);
				_handCard[v].lightAll();
				_handCard[v].showSpecialShape(_players[v].getSex(), type, score);
				if (v == 1)
				{
					_handCard[1].setPosition(_players[1].getPosition().x + otherWidthLR + offsetCard + 100,
						_players[1].getPosition().y + otherHeightLR * 0.65f);


				}
				else if (v == 2)
				{
					_handCard[2].setPosition(_players[2].getPosition().x + otherWidthU + offsetCard + 100,
						_players[2].getPosition().y + otherHeightU * 0.3f);
				}
				else if (v == 3)
				{
					_handCard[3].setPosition(_players[3].getPosition().x - offsetCard - 100,
						_players[3].getPosition().y + otherHeightLR * 0.65f);
				}

				onShowSpecialEffect(type);
			}
		}
		if (specialTag) {
			for (int m = 0; m < PLAY_COUNT; m++) {
				if (STATE_ERR == _logic->TableData.m_EnUserState[m]) continue;
				_handCard[_logic->logicToViewSeatNo(m)].updateTotalWater(_logic->TableData.m_iSpecialCompareResult[m], true);
			}
		}

		
		
	}

	void GameTableUIV88::scheduleOpenFireOBO_CB(float delta) {
		_aniIndex++;
		if (_aniIndex > 15 || _aniIndex < 0) {
			_aniIndex = -1;
			unschedule(schedule_selector(GameTableUIV88::scheduleOpenFireOBO_CB));
			return;
		}
		for (; _aniIndex < PLAY_COUNT * PLAY_COUNT; ++_aniIndex) {
			if (_openFire[_aniIndex / 4][_aniIndex % 4]) {
				break;
			}
			else {
				continue;
			}
		}
		if (_aniIndex > 15 || _aniIndex < 0) {
			_aniIndex = -1;
			unschedule(schedule_selector(GameTableUIV88::scheduleOpenFireOBO_CB));
			return;
		}
		//播放动画
		{
			if (_openFire[_aniIndex / 4][_aniIndex % 4] || true) {
				animationOpenFireOneByOne(_aniIndex / 4, _aniIndex % 4);
				return;
			}
		}
	}

	//一对一打枪动画特效
	void GameTableUIV88::animationOpenFireOneByOne(int fromX, int toY) {
		//把玩家的实际位置转换成视图位置
		fromX = _logic->logicToViewSeatNo(fromX);
		toY = _logic->logicToViewSeatNo(toY);
		if (fromX == toY) {
			return;
		}

		HNAudioEngine::getInstance()->playEffect(AUDIO_FIRE_ONE_BY_ONE_PATH_V88);
	
		Vec2 pos_from = Vec2(_players[fromX].getPosition().x + _players[fromX].getIconPosition().x,
			_players[fromX].getPosition().y + _players[fromX].getIconPosition().y);
		Vec2 pos_to = Vec2(_players[toY].getPosition().x + _players[toY].getIconPosition().x,
			_players[toY].getPosition().y + _players[toY].getIconPosition().y);
	
		qiangNode = CSLoader::createNode("v88/animation/thirteencards/daqiang/daqiang_qiang.csb");
		addChild(qiangNode,88885);

		ImageView* qiangImg = ImageView::create("v88/animation/thirteencards/daqiang/qiang/qiang__00001.png");
		Size qiangSize = qiangImg->getContentSize();
        qiangNode->setAnchorPoint(Vec2(0.5, 0.5));
        qiangNode->setPosition(pos_from);

//        //calc angle
//        Vec2 posA = _players[fromX]->getPosition();
//        Vec2 posB = _players[toY]->getPosition();
//        float angle = 0.0f;
//        if (abs(posA.x-posB.x) < 0.1f) {
//            angle = 90.0f;
//        }
//        else {
//            angle = atan(abs(posA.y-posB.y)/abs(posA.x-posB.x));
//        }
        
		switch (fromX) {
		case 0: {
            if (toY == 1) {
                qiangNode->setScaleX(-1);
                qiangNode->setRotation(20-90);
            }
            else if (toY == 2) {
                qiangNode->setScaleX(-1);
                qiangNode->setRotation(20-53);
            }
            else if (toY == 3) {
                qiangNode->setScaleX(-1);
                qiangNode->setRotation(20-13);
            }
		}
			break;
		case 1: {
            if (toY == 0) {
                qiangNode->setScaleX(-1);
                qiangNode->setRotation(20+90);
            }
            else if (toY == 2) {
              qiangNode->setScaleX(-1.0f);
              qiangNode->setRotation(20-35);

            }
            else if (toY == 3) {
              qiangNode->setScaleX(-1.0f);
              qiangNode->setRotation(20);
            }
		}
			break;
		case 2: {
            if (toY == 0) {
                qiangNode->setRotation(-20-55);
            }
            else if (toY == 1) {
                qiangNode->setRotation(-20-35);
            }
            else if (toY == 3) {
              qiangNode->setScaleX(-1.0f);
              qiangNode->setRotation(20+20);
            }
		}
			break;
		case 3: {
            if (toY == 0) {
              qiangNode->setRotation(-20-13);
            }
            else if (toY == 1) {
                qiangNode->setRotation(-20+0);
            }
            else if (toY == 2) {
              qiangNode->setRotation(-20+20);
            }
		}
			break;
		default:
			break;
		}
		if (qiangNode) {
		    qiangAction = CSLoader::createTimeline("v88/animation/thirteencards/daqiang/daqiang_qiang.csb");
			qiangNode->runAction(qiangAction);
		}

		danhenNode = CSLoader::createNode("v88/animation/thirteencards/daqiang/daqiang_danheng.csb");
		addChild(danhenNode, 88887);
		danhenNode->setPosition(pos_to);
		if (danhenNode) {
			danhenAction = CSLoader::createTimeline("v88/animation/thirteencards/daqiang/daqiang_danheng.csb");
			danhenNode->runAction(danhenAction);
		}

		 qiangZiNode = CSLoader::createNode("v88/animation/thirteencards/daqiang/daqiang_zi.csb");
		 addChild(qiangZiNode, 88888);

		 if (fromX == 3) {
			 qiangZiNode->setPosition(Vec2(pos_from.x - qiangSize.width+10, pos_from.y));
		 }
		 else {
			 qiangZiNode->setPosition(Vec2(pos_from.x + qiangSize.width+10, pos_from.y));
		 }
		
		if (qiangZiNode) {
			ziAction = CSLoader::createTimeline("v88/animation/thirteencards/daqiang/daqiang_zi.csb");
			qiangZiNode->runAction(ziAction);
		}

		CallFunc* func_sheji1 = CallFunc::create([&]() {
			qiangNode->setVisible(true);
			qiangZiNode->setVisible(true);
			qiangAction->play("daqiang_qiang", false);
			ziAction->play("daqiang_zi", false);
		});
		CallFunc* func_2 = CallFunc::create([&]() {
			danhenNode->setVisible(true);
			danhenAction->play("danqiang_danheng", false);
		});

		CallFunc* func_sheji2 = CallFunc::create([&]() {
			qiangNode->setVisible(false);
			qiangZiNode->setVisible(false);
		});

		this->runAction(Sequence::create(func_sheji1, DelayTime::create(0.2f), func_2, DelayTime::create(0.8f), func_sheji2, nullptr));
		danhenNode->runAction(Sequence::create(func_2, DelayTime::create(0.8f),RemoveSelf::create(),nullptr));
		
		{
			_handCard[fromX].updateTotalWater(OPENFIRE_EXTRA_WATER);
			_handCard[toY].updateTotalWater(-OPENFIRE_EXTRA_WATER);
		}
		
	}

	//全垒打打枪特效
	//传入的是物理位置，需要转换成视图坐标
	void GameTableUIV88::animationOpenFireToAll() {
		if (_fireAllAniIdx < 0 || _fireAllAniIdx > 3) {
			//return;
		}
		
		int fromIdx = _logic->logicToViewSeatNo(_fireAllAniIdx);
		//自己的位置稍微做一个偏移，修复文字超出屏幕的bug
		Vec2 pos_from = Vec2(_players[fromIdx].getPosition().x + _players[fromIdx].getIconPosition().x + (fromIdx == 0 ? 15 : 0),
			_players[fromIdx].getPosition().y + _players[fromIdx].getIconPosition().y);
		auto func = CallFunc::create([&]() {
			HNAudioEngine::getInstance()->playEffect(AUDIO_FIRE_ALL_PATH_V88);
		});
		for (int i = 0; i < 5; i++) {
			this->runAction(Sequence::create(DelayTime::create(0.3f * (rand() % 4) + 0.3f), func, nullptr));
		}


		//this->runAction(Sequence::create(DelayTime::create(0.2f), [this]{
		//	//this->index
		//}, nullptr));
		
		//炮弹角度问题和文字角度问题
		auto qNode = CSLoader::createNode(Quanleida_Zi_Animation_Path_V88);
		addChild(qNode,10000);
		auto tankeNode = CSLoader::createNode(Quanleida_Tanke_Animation_Path_V88);
		tankeNode->setPosition(pos_from);
		addChild(tankeNode);
		baozhaoPosInde = 0;
		this->schedule(schedule_selector(GameTableUIV88::showBaozheEffect), 0.2f, PLAY_COUNT - 1, 0.2f);
		switch (fromIdx)
		{

		case 0:
		{
				  qNode->setPosition(Vec2(pos_from.x+120, pos_from.y-50));
				  tankeNode->setPosition(Vec2(WIN_SIZE.width / 2 - tankeNode->getContentSize().width / 2, pos_from.y));
				  qNode->setPosition(Vec2(WIN_SIZE.width / 2 - 150, WIN_SIZE.height / 2-50));
				  tankeNode->setRotation(0);
				  break;
		}
		case 1:
		{
				  qNode->setPosition(Vec2(pos_from.x+230, pos_from.y - 100));
				  tankeNode->setRotation(90);
				  tankeNode->setPosition(Vec2(pos_from.x+60, WIN_SIZE.height / 2 - tankeNode->getContentSize().width / 2));
				  break;
				 
		}
		case 2:
		{
				  qNode->setPosition(Vec2(pos_from.x + 510, pos_from.y-80));
				  tankeNode->setRotation(180);
				  tankeNode->setPosition(Vec2(WIN_SIZE.width / 2 - tankeNode->getContentSize().width / 2, pos_from.y));
				  break;
		}
		case 3:
		{
				
				  qNode->setPosition(Vec2(pos_from.x-200, pos_from.y - 100));
				  tankeNode->setRotation(270);
				  tankeNode->setPosition(Vec2(WIN_SIZE.width - 105, WIN_SIZE.height / 2 - tankeNode->getContentSize().width / 2));
				  break;
		}
		
			
		default:
			break;
		}


		auto action = CSLoader::createTimeline(Quanleida_Zi_Animation_Path_V88);
		qNode->runAction(action);
		action->play("quanleida_zi", false);
		auto tankeAction = CSLoader::createTimeline(Quanleida_Tanke_Animation_Path_V88);
		tankeNode->runAction(tankeAction);
		tankeAction->play("quanleida_tanke", false);
		
		

		for (int i = 0; i < PLAY_COUNT; i++) {
			_handCard[_logic->logicToViewSeatNo(i)].updateTotalWater(_extraWaterAll[i]);
		}

	}


	void GameTableUIV88::showBaozheEffect(float dt)
	{

		int fromIdx = _logic->logicToViewSeatNo(_fireAllAniIdx);
		if (baozhaoPosInde != fromIdx)
			{
				auto baozhaoNode = CSLoader::createNode(Quanleida_Baozha_Animation_Path_V88);
				baozhaoNode->setPosition(_players[baozhaoPosInde].getPosition().x + _players[baozhaoPosInde].getIconPosition().x + (_fireAllAniIdx == 0 ? 15 : 0),
					_players[baozhaoPosInde].getPosition().y + _players[baozhaoPosInde].getIconPosition().y);
				addChild(baozhaoNode);
				//
				auto actionBaozha = CSLoader::createTimeline(Quanleida_Baozha_Animation_Path_V88);
				baozhaoNode->runAction(actionBaozha);
				actionBaozha->play("quanleida_baozha", false);
			}
		baozhaoPosInde++;
		if (baozhaoPosInde >= 4)
		{
			this->unschedule(schedule_selector(GameTableUIV88::showBaozheEffect));
		}

	}

	void GameTableUIV88::onGameResult(const S_C_GameResult* pObject) {
		auto settlement = SettlementLayerV88::create();
		settlement->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		settlement->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2);
		addChild(settlement, LOCAL_ZORDER_SETTLEMENT_LAYER, TAG_SETTLEMENT_LAYER);
		settlement->setCloseCallback(CC_CALLBACK_0(GameTableUIV88::onSettlementTimer, this));
		settlement->startTimer(_logic->TableData.m_byCountTime - 3);//服务端结算时间为5s，自己展示3s

		BYTE lMySeatNo = _logic->getMySeatNo();
		std::vector<SettlementParameter> pragmas;
		for (size_t i = 0; i < PLAY_COUNT; i++) {
			if (STATE_ERR == _logic->TableData.m_EnUserState[i]) continue;

            //
            auto coin = FloatCoin::create();
            if (coin) {
                coin->openParent(this, _players[i].getPosition() + _players[i].getContentSize()/2, LOCAL_ZORDER_SETTLEMENT_LAYER-1);
                coin->updateCoin(pObject->i64UserMoney[i]);
            }
            
			UserInfoStruct* user = _logic->getUserBySeatNo(i);

			SettlementParameter pragma;
			pragma.score = pObject->i64UserMoney[i];
			pragma.tax = pObject->i64UserFen[i] - pObject->i64UserMoney[i];
			pragma.nickName = user->nickName;
			pragma.self = (i == lMySeatNo);
			pragma.isZhuang = i == _logic->TableData.m_byNowNtStation ? true : false;
			pragmas.push_back(pragma);
		}
		settlement->loadParameter(pragmas);
	}

	void GameTableUIV88::updateTip(float dt) {
		int iTag = m_TextTip->getTag();
		if (iTag == 0) {
			unschedule(schedule_selector(GameTableUIV88::updateTip));
			//            _logic->sendUserUp();
			//            onSettlementTimer();
			_isPlaying = false;
			_hasReadyClick = false;
//            _logic->leaveTheGame();
            leaveTheGame();
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

	void GameTableUIV88::IShowTipExit(long long i64Money) {
		auto layer = GamePromptLayer::create();
//		layer->setCloseExitBtn();
		//     char str[128] = { 0 };
		//     int sec = 3;
		//     sprintf(str, "金币数额低于%lld，%ds后离开游戏！", i64Money, sec);
		//     layer->showPrompt(GBKToUtf8(str));
		int sec = 3;
		char s[128] {0};
		std::string str;
		str.append(GBKToUtf8("金币数额低于"));
		sprintf(s, "%lld", i64Money);
		str.append(s);
		sprintf(s, ",%d", sec);
		str.append(s);
		str.append(GBKToUtf8("s后离开游戏！"));
		layer->showPrompt(str);

		layer->setOkayCallBack([&]() {
			_isPlaying = false;
			_hasReadyClick = false;
//            _logic->leaveTheGame();
            leaveTheGame();
        });
		layer->setCloseCallBack([&]() {
			_isPlaying = false;
			_hasReadyClick = false;
//            _logic->leaveTheGame();
            leaveTheGame();
        });
		layer->setColorLayerTouchEnabled(false);

		m_i64TipMoney = i64Money;
		m_TextTip = layer->getPromptText();
		m_TextTip->setTag(sec);
		schedule(schedule_selector(GameTableUIV88::updateTip), 1.0f);
	}

	void GameTableUIV88::onGameDizhuValue(const int value) {
		if (_dizhuValue != nullptr) {
			char tmp[64] = { 0 };
			sprintf(tmp, "%s", Tools::addComma(value).c_str());
			_dizhuValue->setString(tmp);
		}
	}

	void GameTableUIV88::onGameNotifyWuHuaNiu(NN::tag_s2c_notify *msg) {
		if (msg->i64WinMoney < 0) return;
		char str[128] = { 0 };

		if (msg->bDeskID == INVALID_BYTE) {
			//广播
			if (msg->bGameID == 1) {
				sprintf(str, GetText("NN_Catch_Bull_Plus_Notify"), GetText("NN_Game_One"), ForceGBKToUtf8(msg->cRoomName), ForceGBKToUtf8(msg->cNickName), Tools::addComma(msg->i64WinMoney).c_str());
			}
			else if (msg->bGameID == 2) {
				sprintf(str, GetText("NN_Catch_Bull_Plus_Notify"), GetText("NN_Game_Two"), ForceGBKToUtf8(msg->cRoomName), ForceGBKToUtf8(msg->cNickName), Tools::addComma(msg->i64WinMoney).c_str());
			}
			else if (msg->bGameID == 3) {
				sprintf(str, GetText("NN_Catch_Bull_Plus_Notify"), GetText("NN_Game_Three"), ForceGBKToUtf8(msg->cRoomName), ForceGBKToUtf8(msg->cNickName), Tools::addComma(msg->i64WinMoney).c_str());
			}
			else {
				return;
			}
			showNotice(str, 1, 1);
		}
	}

	void GameTableUIV88::showNotice(const std::string& message, int nCount, int nType) {
		if (m_notice == nullptr) {
			m_notice = GameNotice::create();
			m_notice->setLocalZOrder(30000);
		}
		m_notice->setName("GameNotice");
		m_notice->openNotic(this, 10000, message, nCount, nType);
	}

	void GameTableUIV88::onUserChat(INT sendID, INT TargetID, const std::string& msg) {

	}

	void GameTableUIV88::doCompareCard(const BYTE heapCard[PLAY_COUNT][3][5]) {
		float dt = 0.0f;

		if (_logic->TableData.m_byCompareTime > 3) {
			dt = (float)(_logic->TableData.m_byCompareTime - 3) / 3;
		}

		auto startCompareCard = [=]() {
			compareCard(heapCard, dt);
		};

		startCompareCard();
	}

	void GameTableUIV88::compareCard(const BYTE heapCard[PLAY_COUNT][3][5], float litghtCardTime) {
		bool comparePlayerIndex[4];
		memset(comparePlayerIndex, false, sizeof(comparePlayerIndex));
		for (size_t i = 0; i < PLAY_COUNT; i++) {
			if (STATE_ERR == _logic->TableData.m_EnUserState[i])
				continue;
			if (_logic->TableData.m_iUseSpecialShape[i]) {
				continue;//玩家使用特殊牌，不参与比牌
			}

			BYTE v = _logic->logicToViewSeatNo(i);
			std::vector<BYTE> cards;
			for (size_t j = 0; j < 3; j++) {
				for (size_t k = 0; k < 5; k++) {
					BYTE val = heapCard[i][j][k];
					if (0 == val) continue;
					if (0xFF == val) continue;
					cards.push_back(val);
				}
			}

			if (cards.empty()) {
				comparePlayerIndex[i] = false;

				_handCard[v].setVisible(false);
			}
			else {
				comparePlayerIndex[i] = true;

				_handCard[v].loadCards(cards);
				_handCard[v].setVisible(true);
			}
		}
		float openTime = 1.1f;
		auto seq_0_0 = comparePlayerIndex[0] ? Sequence::create(DelayTime::create(openTime), CallFunc::create([&]() {
			doLightCardOneByOne(0, 0);
		}), NULL) : Sequence::create(DelayTime::create(0.0f), NULL);
		auto seq_1_0 = comparePlayerIndex[1] ? Sequence::create(DelayTime::create(openTime), CallFunc::create([&]() {
			doLightCardOneByOne(1, 0);
		}), NULL) : Sequence::create(DelayTime::create(0.0f), NULL);
		auto seq_2_0 = comparePlayerIndex[2] ? Sequence::create(DelayTime::create(openTime), CallFunc::create([&]() {
			doLightCardOneByOne(2, 0);
		}), NULL) : Sequence::create(DelayTime::create(0.0f), NULL);
		auto seq_3_0 = comparePlayerIndex[3] ? Sequence::create(DelayTime::create(openTime), CallFunc::create([&]() {
			doLightCardOneByOne(3, 0);
		}), NULL) : Sequence::create(DelayTime::create(0.0f), NULL);

		auto firstSeq = Sequence::create(seq_0_0, seq_1_0, seq_2_0, seq_3_0,
			DelayTime::create(0.1f), CallFunc::create([&]() {
			doShowScore(0);
		}),   //显示结果
			nullptr);

		auto seq_0_1 = comparePlayerIndex[0] ? Sequence::create(DelayTime::create(openTime), CallFunc::create([&]() {
			doLightCardOneByOne(0, 1);
		}), NULL) : Sequence::create(DelayTime::create(0.0f), NULL);
		auto seq_1_1 = comparePlayerIndex[1] ? Sequence::create(DelayTime::create(openTime), CallFunc::create([&]() {
			doLightCardOneByOne(1, 1);
		}), NULL) : Sequence::create(DelayTime::create(0.0f), NULL);
		auto seq_2_1 = comparePlayerIndex[2] ? Sequence::create(DelayTime::create(openTime), CallFunc::create([&]() {
			doLightCardOneByOne(2, 1);
		}), NULL) : Sequence::create(DelayTime::create(0.0f), NULL);
		auto seq_3_1 = comparePlayerIndex[3] ? Sequence::create(DelayTime::create(openTime), CallFunc::create([&]() {
			doLightCardOneByOne(3, 1);
		}), NULL) : Sequence::create(DelayTime::create(0.0f), NULL);

		auto secondSeq = Sequence::create(seq_0_1, seq_1_1, seq_2_1, seq_3_1,
			DelayTime::create(0.1f), CallFunc::create([&]() {
			doShowScore(1);
		}),   //显示结果
			nullptr);

		auto seq_0_2 = comparePlayerIndex[0] ? Sequence::create(DelayTime::create(openTime), CallFunc::create([&]() {
			doLightCardOneByOne(0, 2);
		}), NULL) : Sequence::create(DelayTime::create(0.0f), NULL);
		auto seq_1_2 = comparePlayerIndex[1] ? Sequence::create(DelayTime::create(openTime), CallFunc::create([&]() {
			doLightCardOneByOne(1, 2);
		}), NULL) : Sequence::create(DelayTime::create(0.0f), NULL);
		auto seq_2_2 = comparePlayerIndex[2] ? Sequence::create(DelayTime::create(openTime), CallFunc::create([&]() {
			doLightCardOneByOne(2, 2);
		}), NULL) : Sequence::create(DelayTime::create(0.0f), NULL);
		auto seq_3_2 = comparePlayerIndex[3] ? Sequence::create(DelayTime::create(openTime), CallFunc::create([&]() {
			doLightCardOneByOne(3, 2);
		}), NULL) : Sequence::create(DelayTime::create(0.0f), NULL);

		auto thirdSeq = Sequence::create(seq_0_2, seq_1_2, seq_2_2, seq_3_2,
			DelayTime::create(0.1f), CallFunc::create([&]() {
			doShowScore(2);
		}),   //显示结果
			nullptr);

		this->runAction(Sequence::create(DelayTime::create(0.6f), firstSeq,
			DelayTime::create(0.6f), secondSeq,
			DelayTime::create(0.6f), thirdSeq,
			nullptr));
		//单个玩家开牌需要的时间为：openTime * 3 + 0.8 * 3 + 0.8 * 3
	}

	Sequence* GameTableUIV88::createSeqForCompareCard(bool playing, int playIndex, int cardIndex) {
		if (_isPlaying) {
			return Sequence::create(DelayTime::create(0.5f), CallFunc::create([&]() {
				doLightCardOneByOne(playIndex, cardIndex);
			}), NULL);
		}
		else {
			return Sequence::create(DelayTime::create(0.0f), NULL);
		}
	}

	void GameTableUIV88::doShowScore(int index) {
		for (size_t i = 0; i < PLAY_COUNT; i++) {
			if (STATE_ERR == _logic->TableData.m_EnUserState[i]) continue;

			BYTE v = _logic->logicToViewSeatNo(i);

			LLONG score = _logic->TableData.m_iComepareResult[i][index];
			BYTE cardType = _logic->TableData.m_iHeapShape[i][index];

			_handCard[v].showScore(index, _players[v].getSex(), cardType, score);
		}
	}

	void GameTableUIV88::doLightCard(int index) {
		for (size_t i = 0; i < PLAY_COUNT; i++) {
			if (STATE_ERR == _logic->TableData.m_EnUserState[i]) continue;

			BYTE v = _logic->logicToViewSeatNo(i);
			_handCard[v].lightCard(index);
		}
		SoundHepler::getInstance()->playerEffects("music/ThirteenZhang/sound/sendcard.mp3");
	}

	void GameTableUIV88::doShowScoreOneByOne(int seatNo, int index) {
		if (STATE_ERR == _logic->TableData.m_EnUserState[seatNo]) return;

		BYTE v = _logic->logicToViewSeatNo(seatNo);

		LLONG score = _logic->TableData.m_iComepareResult[seatNo][index];
		BYTE cardType = _logic->TableData.m_iHeapShape[seatNo][index];

		_handCard[v].showScore(index, _players[v].getSex(), cardType, score);
	}

	void GameTableUIV88::doLightCardOneByOne(int seatNo, int index) {
		if (seatNo >= PLAY_COUNT) {
			seatNo -= PLAY_COUNT;
		}
		if (seatNo < 0) {
			seatNo += PLAY_COUNT;
		}
		if (STATE_ERR == _logic->TableData.m_EnUserState[seatNo]) return;

		BYTE v = _logic->logicToViewSeatNo(seatNo);
		LLONG score = _logic->TableData.m_iComepareResult[seatNo][index];
		BYTE cardType = _logic->TableData.m_iHeapShape[seatNo][index];
		_handCard[v].lightCard(index);

		_handCard[v].showType(index, _players[v].getSex(), cardType, score);
	}

//    void GameTableUIV88::middleDaoshuiTip(ControllerLayer* controller) {
//        this->runAction(Sequence::create(CallFunc::create([=]() {
//            animationMiddleDaoshui();
//        }), nullptr));
//    }

//    void GameTableUIV88::firstDaoshuiTip(ControllerLayer* controller) {
//        this->runAction(Sequence::create(CallFunc::create([=]() {
//            animationFirstDaoshui();
//        }), nullptr));
//    }
//
//    void GameTableUIV88::allDaoshuiTip(ThirteenZhang::ControllerLayer *controller) {
//        this->runAction(Sequence::create(CallFunc::create([=]() {
//            animationAllDaoshui();
//        }), nullptr));
//    }

	void GameTableUIV88::animationMiddleDaoshui() {
		controlMiddleDaoshuiButton(true);
	}

	void GameTableUIV88::animationFirstDaoshui() {
		controlFirstDaoshuiButton(true);
	}

	void GameTableUIV88::animationAllDaoshui() {
		controlAllDaoshuiButton(true);
	}

	void GameTableUIV88::animationVSShow() {
		   HNAudioEngine::getInstance()->playEffect(AUDIO_START_BIPAI_PATH_V88);
		   //胜利结算
		   auto vsNode = CSLoader::createNode(PK_ANIMATION_V88);
		   vsNode->setPosition(WIN_SIZE / 2);
		   addChild(vsNode);

		auto action = CSLoader::createTimeline(PK_ANIMATION_V88);
		   vsNode->runAction(action);
		   action->play("bipai_duoren", false);
	}

	void GameTableUIV88::controlMiddleDaoshuiButton(bool show) {
		// _dun_middle_b->setVisible(true);
		// _dun_last_b->setVisible(true);
		// _dun_middle_f->setVisible(show);
		// _dun_last_f->setVisible(show);
		// _dun_first_b->setVisible(true);
		//  _dun_first_f->setVisible(false);
		if (show) {
			// _dun_middle_f->stopActionByTag(ShakeDunAction_TAG);
			//  _dun_last_f->stopActionByTag(ShakeDunAction_TAG);
			//  _dun_middle_f->setPosition(Vec2(_dun_middle_b->getContentSize().width / 2, _dun_middle_b->getContentSize().height / 2));
			_dun_last_f->setPosition(Vec2(_dun_last_b->getContentSize().width / 2, _dun_last_b->getContentSize().height / 2));
			auto shake1 = Shake::create(0.5f, 5, 5);
			auto shake2 = Shake::create(0.5f, 5, 5);
			shake1->setTag(ShakeDunAction_TAG);
			shake2->setTag(ShakeDunAction_TAG);
			/*     _dun_middle_f->runAction(Sequence::create(shake1, CCCallFunc::create([this]() {
			restoreDaoshuiButton();
			}), nullptr));
			_dun_last_f->runAction(Sequence::create(shake2, CCCallFunc::create([this]() {
			restoreDaoshuiButton();
			}), nullptr));*/
		}
	}

	void GameTableUIV88::controlFirstDaoshuiButton(bool show) {
		//  _dun_first_b->setVisible(true);
		//  _dun_middle_b->setVisible(true);
		//  _dun_first_f->setVisible(show);
		// _dun_middle_f->setVisible(show);
		// _dun_last_b->setVisible(true);
		// _dun_last_f->setVisible(false);
		if (show) {
			//  _dun_middle_f->stopActionByTag(ShakeDunAction_TAG);
			// _dun_first_f->stopActionByTag(ShakeDunAction_TAG);
			//  _dun_middle_f->setPosition(Vec2(_dun_middle_b->getContentSize().width / 2, _dun_middle_b->getContentSize().height / 2));
			//  _dun_first_f->setPosition(Vec2(_dun_first_b->getContentSize().width / 2, _dun_first_b->getContentSize().height / 2));
			auto shake1 = Shake::create(0.5f, 5, 5);
			auto shake2 = Shake::create(0.5f, 5, 5);
			shake1->setTag(ShakeDunAction_TAG);
			shake2->setTag(ShakeDunAction_TAG);
			/*     _dun_middle_f->runAction(Sequence::create(shake1, CCCallFunc::create([this]() {
			restoreDaoshuiButton();
			}), nullptr));
			_dun_first_f->runAction(Sequence::create(shake2, CCCallFunc::create([this]() {
			restoreDaoshuiButton();
			}), nullptr));*/
		}
	}

	void GameTableUIV88::controlAllDaoshuiButton(bool show) {
		/* _dun_first_b->setVisible(true);
		_dun_middle_b->setVisible(true);*/
		//_dun_first_f->setVisible(show);
		// _dun_middle_f->setVisible(show);
		//_dun_last_b->setVisible(true);
		//  _dun_last_f->setVisible(show);
		if (show) {
			//  _dun_last_f->stopAllActionsByTag(ShakeDunAction_TAG);
			//  _dun_middle_f->stopActionByTag(ShakeDunAction_TAG);
			//  _dun_first_f->stopActionByTag(ShakeDunAction_TAG);
			////  _dun_last_f->setPosition(Vec2(_dun_last_b->getContentSize().width / 2, _dun_last_b->getContentSize().height / 2));
			//  _dun_middle_f->setPosition(Vec2(_dun_middle_b->getContentSize().width / 2, _dun_middle_b->getContentSize().height / 2));
			//  _dun_first_f->setPosition(Vec2(_dun_first_b->getContentSize().width / 2, _dun_first_b->getContentSize().height / 2));
			auto shake1 = Shake::create(0.5f, 5, 5);
			auto shake2 = Shake::create(0.5f, 5, 5);
			auto shake3 = Shake::create(0.5f, 5, 5);
			shake1->setTag(ShakeDunAction_TAG);
			shake2->setTag(ShakeDunAction_TAG);
			shake3->setTag(ShakeDunAction_TAG);
			//_dun_last_f->runAction(Sequence::create(shake3, CCCallFunc::create([this]() {
			//    restoreDaoshuiButton();
			//}), nullptr));
			//_dun_middle_f->runAction(Sequence::create(shake1, CCCallFunc::create([this]() {
			//    restoreDaoshuiButton();
			//}), nullptr));
			//_dun_first_f->runAction(Sequence::create(shake2, CCCallFunc::create([this]() {
			//    restoreDaoshuiButton();
			//}), nullptr));
		}
	}

	void GameTableUIV88::restoreDaoshuiButton() {
		//_dun_first_b->setVisible(true);
		//_dun_first_f->setVisible(false);
		//_dun_middle_b->setVisible(true);
		// _dun_middle_f->setVisible(false);
		// _dun_last_b->setVisible(true);
		//_dun_last_f->setVisible(false);
		// _dun_first_f->stopActionByTag(ShakeDunAction_TAG);
		// _dun_middle_f->stopActionByTag(ShakeDunAction_TAG);
		// _dun_last_f->stopActionByTag(ShakeDunAction_TAG);
	}

	void GameTableUIV88::switchCardStateSprite(int index, bool isDone) {
		if (index == 0) {
			if (_pair_state[index] != nullptr) {
				_pair_state[index]->setVisible(false);
			}
			return;
		}
		if (_pair_state[index] != nullptr) {
			_pair_state[index]->setTexture(isDone ? CARD_STATE_DONE_PATH : CARD_STATE_PAIRING_PATH);
		}
		else {
			_pair_state[index] = Sprite::create(isDone ? CARD_STATE_DONE_PATH : CARD_STATE_PAIRING_PATH);
		}
		_pair_state[index]->setVisible(true);
	}

	void GameTableUIV88::restoreCardStateSprite(int index) {
		if (_pair_state[index] != nullptr) {
			_pair_state[index]->setVisible(false);
		}
	}

//	//年兽触发
//	void GameTableUIV88::onGameShowNianShou() {
//		//播动画
//		showNianShouAnimation();
//	}
//
//	//赶年兽结果
//	void GameTableUIV88::onGameDriveNianShouResult(int nUserID, long long i64RewardMoney) {
//		//过滤只有自己看的到
//		if (nUserID != _logic->getUserId(_logic->getMySeatNo())) {
//			return;
//		}
//
//		removeNianShouButton();
//
//		Vec2 pos = NewYearActivityInGame::NianShouButtonPosition;
//
//		//有鞭炮赶年兽奖励
//		if (i64RewardMoney > 0) {
//			NewYearActivity::NSAnimationTool::playTNTAudio(i64RewardMoney);
//			auto daijiNode = NewYearActivity::NSAnimationTool::createNSGanNodeAni("hit", false);
//			if (daijiNode) {
//				daijiNode->setPosition(Vec2(pos));
//				this->addChild(daijiNode, NewYearActivityInGame::NianShouZOrder);
//			}
//
//			NewYearActivity::NSAnimationTool::createRewardLabel(i64RewardMoney, this, Vec2(pos.x, pos.y + 50));
//		}
//		else { //无鞭炮
//			NewYearActivity::NSAnimationTool::createTipOfNoProp(this, pos);
//		}
//	}
//
//	//对局获得鞭炮
//	void GameTableUIV88::onGameMatchGainFirecraker(int nUserID) {
//		//该id获得一个中型鞭炮
//		//只有自己显示
//		if (_logic->getUserId(_logic->getMySeatNo()) == nUserID) {
//			ImageView* pTipImage = ImageView::create("common/gainFirecrakerTip.png");
//			pTipImage->setScale(0.1);
//			pTipImage->setPosition(Vec2(250, 140));
//			addChild(pTipImage, NewYearActivityInGame::NianShouZOrder);
//
//			Spawn* spawn = Spawn::create(ScaleTo::create(0.5f, 1.0f), FadeIn::create(0.5f), nullptr);
//			Sequence* seq = Sequence::create(spawn, DelayTime::create(2.0f), RemoveSelf::create(), nullptr);
//			pTipImage->runAction(seq);
//		}
//	}
//
//	void GameTableUIV88::showNianShouAnimation() {
//		if (this->getChildByName("DriveNianShouBtn")) {
//			return;
//		}
//		auto btn = NewYearActivity::NSAnimationTool::createNSButton("DriveNianShouBtn");
//		if (btn) {
//			btn->setPosition(NewYearActivityInGame::NianShouButtonPosition);
//			btn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::driveNianShouClickCallback, this));
//			addChild(btn, NewYearActivityInGame::NianShouZOrder);
//
//			auto daijiNode = NewYearActivity::NSAnimationTool::createNSDaijiNodeAni("daiji", true);
//			if (daijiNode) {
//				daijiNode->setPosition(Vec2(btn->getContentSize().width*0.5f, btn->getContentSize().height*0.5f));
//				btn->addChild(daijiNode);
//			}
//			scheduleOnce(schedule_selector(GameTableUIV88::autoRemoveNianShouButton), 5.0f);
//		}
//	}
//
//	void GameTableUIV88::driveNianShouClickCallback(Ref* pSender, Widget::TouchEventType type) {
//		if (type != Widget::TouchEventType::ENDED)
//			return;
//
//		//        unschedule(schedule_selector(GameTableUIV88::autoRemoveNianShouButton));
//
//		Button* btn = (Button*)pSender;
//		btn->setTouchEnabled(false);
//
//		//发包
//		int nUserID = _logic->getUserId(_logic->getMySeatNo());
//		_logic->sendDriveNianShou(nUserID);
//	}
//
//	void GameTableUIV88::removeNianShouButton() {
//		auto btn = this->getChildByName("DriveNianShouBtn");
//		if (btn) {
//			btn->stopAllActions();
//			btn->removeFromParentAndCleanup(true);
//		}
//	}
//
//	void GameTableUIV88::autoRemoveNianShouButton(float dt) {
//		removeNianShouButton();
//	}
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
