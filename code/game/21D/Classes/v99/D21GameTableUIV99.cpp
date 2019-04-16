#include "D21GameTableUIV99.h"
#include "D21GameTableLogic.h"
#include "D21GameNoticeV99.h"
#include "D21ResultV99.h"

#include "json/rapidjson.h"
#include "json/document.h"
#include "HNLogicExport.h"
#include "HNLobbyExport.h"

#include "HNUIExport.h"
#include "SimpleAudioEngine.h"
#include "HNLobby/state/PlatformState.h"
#include "VersionInfo/VersionInfo.h"
#include "HNLobby/pattern/StateManager.h"
#include "GUI/CCControlExtension/CCControlButton.h"
#include "GUI/CCControlExtension/CCControl.h"
#include "HNLobby/globel.h"

//#include "../../TTNN/Classes/v99/BRNNSetLayerV99.h"

namespace D21
{
#define WINSIZE	   Director::getInstance()->getWinSize()
#define BETCOUNT	4
#define CARD_SPACE	30

//zorder 
#define D21_CREATE_TOOLBAR_ZORDER_V99		500		//toolbar
#define D21_OTEHER_VIEW_ZORDER_V99				1200      //弹窗一类
#define D21_MENUBTN_ZORDER_V99					800      //按钮
#define D21_RESULT_VIEW_V99								1000      //按钮

#define D21_PLAYER_ZORDER_V99						1100      //卡牌

#define D21_MIN_ZORDER_V99					100
#define D21_BET_ZORDER_V99					150
#define D21_CARD_ZORDER_V99					200
#define D21_TOOLBAR_ZORDER_V99				500

	//csb
	static const char* D21_TABLECARD_PATH_V99	= "Games/12100404/PlayerCard%d.csb";
	static const char* D21_INSULANCE_CSB_V99	= "Games/12100404/BuyInsurance.csb";

	//bg
	static const char* D21_BG_V99				= "v99/cow/bg.png";
	static const char* D21_GREEN_BTN_V99		= "v99/button/green/green.png";
	static const char* D21_BLUE_BTN_V99			= "v99/button/blue/blue.png";
	static const char* D21_PREPARE_TITLE_V99	= "v99/button/blue/zhunbei.png";
	static const char* D21_CATCH_TITLE_V99		= "v99/button/blue/jiaozhuang.png";
	static const char* D21_CANCEL_TITLE_V99		= "v99/button/blue/fangqi.png";
	static const char* D21_DIVIDE_TITLE_V99		= "v99/button/green/Card.png";
	static const char* D21_DOUBLE_TITLE_V99		= "v99/button/green/double.png";
	static const char* D21_TAKE_TITLE_V99		= "v99/button/green/Take_card.png";
	static const char* D21_STOP_TITLE_V99		= "v99/button/green/Suspension.png";
	//toolbar
	static const char* D21_TOOL_BAR_PATH_V99	= "v99/cow/exit/%s";
	//bet
	static const char* D21_BET_BTN_PATH_V99		= "v99/button/1/%d.png";
	static const char* D21_BET_BG_V99			= "Games/12100404/21dian/choumadi.png";
	//
	static const char* D21_FAPAIQI_BG_V99		= "v99/fapaiqi.png";


	static const char* D21_CSB								=	"d21/tableUi/tableUi.csb";
	static const char* BETMESSAGEBOX_CSB		=	"d21/betMessageBox/betMessageBoxUi.csb";
	static const char* JIESUANBOX_CSB				= "d21/jieSuanBox/jieSuanUi.csb";
	static const char* D21_TABLEUI_CSB				= "csb/v99/d21V99/table/table_V99.csb";
	static const char* D21_BET_CSB						= "csb/v99/d21V99/table/betBtn_V99.csb";
	
	static const char* BMFONT								=	"d21/table/number.fnt";

	//animation

	static const char* D21_BAODIAN_ANIMATION_CSB_V99 = "v99/animation/21dian0/baodian/dayday_baodian.csb";
	static const char* D21_WULONG_ANIMATION_CSB_V99 = "v99/animation/21dian0/wulongheijieke/dayday21_wulong.csb";
	static const char* D21_HEIJIEKE_ANIMATION_CSB_V99 = "v99/animation/21dian0/wulongheijieke/dayday21_heijieke.csb";

	GameTableUIV99::GameTableUIV99()
	{
		_tableLogic = nullptr,
		m_bNextOut = false,
		_isOpenHelp = false,

		_vecPlayerNode.clear();
		_vecBetBtn.clear();
		_vecChips.clear();
		_vecPointBg.clear();
		_vecPointText.clear();
		_betBg = NULL;

		for (int i = 0; i < PLAY_COUNT; i++)
		{
			_arrPlayer[i] = nullptr;
		}

		initParams();
	}

	GameTableUIV99::~GameTableUIV99()
	{
		HN_SAFE_DELETE(_tableLogic);
	}

	HNGameUIBase* GameTableUIV99::create(BYTE bDeskIndex, bool bAutoCreate)
	{
		GameTableUIV99* tableUI = new GameTableUIV99();
		if (tableUI->init(bDeskIndex, bAutoCreate))
		{
			tableUI->autorelease();
			return tableUI;
		}
		else
		{
			CC_SAFE_DELETE(tableUI);
			return nullptr;
		}
	}

	void GameTableUIV99::onEnter()
	{
		HNGameUIBase::onEnter();
		//缓存卡组
		SpriteFrameCache* cache = SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile("v99/common/card_AMax/cards.plist");

		_tableLogic = new GameTableLogic(this, _bDeskIndex, _bAutoCreate);
		_tableLogic->enterGame();
	}

	void GameTableUIV99::onExit()
	{
		HNGameUIBase::onExit();

		SpriteFrameCache* cache = SpriteFrameCache::getInstance();
		cache->removeSpriteFramesFromFile("v99/common/card_AMax/cards.plist");
	}

	bool GameTableUIV99::init(BYTE bDeskIndex, bool bAutoCreate)
	{
		if (!HNLayer::init())
		{
			return false;
		}

		_bDeskIndex = bDeskIndex;
		_bAutoCreate = bAutoCreate;

		//触摸
		auto dispatcher = Director::getInstance()->getEventDispatcher();
		auto myListener = EventListenerTouchOneByOne::create();
		myListener->onTouchBegan = CC_CALLBACK_2(GameTableUIV99::onTouchBegan, this);
		myListener->setSwallowTouches(false);
		dispatcher->addEventListenerWithSceneGraphPriority(myListener, this);

		setBackGroundImage(D21_BG_V99);
		
		createTableUI();
		createCard();
 		createToolBarUI();
 		//createRecordingBoard();

		return true;
	}

	/************************************************************************/
	void GameTableUIV99::createTableUI()
	{
		//发牌器
		_faPaiQiImg = ImageView::create(D21_FAPAIQI_BG_V99);
		_faPaiQiImg->setPosition(Vec2(WIN_SIZE.width/2-120, WIN_SIZE.height/2));
		_faPaiQiImg->setVisible(false);
		addChild(_faPaiQiImg, D21_MIN_ZORDER_V99);

		//共有clock
		//_clockObj = BRNN::ClockV99::create();
		//_clockObj->setPosition(WIN_SIZE / 2);
		//_clockObj->setVisible(false);
		//addChild(_clockObj, D21_MIN_ZORDER_V99);

		//用户ui
		vector<Vec2> fPlayerUIPosVec;
		fPlayerUIPosVec.push_back(Vec2(10, 10));
		fPlayerUIPosVec.push_back(Vec2(WIN_SIZE.width - 10, WIN_SIZE.height / 2 - 100));
		fPlayerUIPosVec.push_back(Vec2(250, WIN_SIZE.height - 10 - 200));
		fPlayerUIPosVec.push_back(Vec2(10, WIN_SIZE.height / 2 - 100));
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			auto playerNode = Node::create();
			playerNode->setPosition(fPlayerUIPosVec.at(i));
			addChild(playerNode, D21_MIN_ZORDER_V99);
			_vecPlayerNode.push_back(playerNode);
		}

		//牌操作btn
		char* buttonName[4] = { "AUIButton_branchCard", "AUIButton_double", "AUIButton_stopCard", "AUIButton_wantCard" };
		const char* buttonTitle[4] = { D21_DIVIDE_TITLE_V99, D21_DOUBLE_TITLE_V99, D21_STOP_TITLE_V99, D21_TAKE_TITLE_V99 };

		float fBtnWidth = 194 * 0.8;
		float fSpace = 20;
		Vec2 startPos = Vec2(WIN_SIZE.width / 2 - 2 * fSpace - fBtnWidth, 200);

		for (int i = 0; i < 4; i++) {
			auto menuBtn = AUIButton::create(D21_GREEN_BTN_V99, D21_GREEN_BTN_V99);
			menuBtn->setTitle(buttonTitle[i]);
			menuBtn->setTitleOffset(Vec2(2, 2));
			menuBtn->setName(buttonName[i]);
			menuBtn->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
			menuBtn->setPosition(Vec2(startPos.x + i*(fBtnWidth + fSpace), startPos.y));
			addChild(menuBtn, D21_CARD_ZORDER_V99 - 1);
			menuBtn->setVisible(false);
			menuBtn->setScale(0.8f);
			menuBtn->setPressedActionEnabled(true);
			menuBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::controllBtnClickCallback, this));

			_vecOperateBtn.push_back(menuBtn);
		}

		//准备btn
		_readyBtn = AUIButton::create(D21_BLUE_BTN_V99, D21_BLUE_BTN_V99);
		_readyBtn->setTitle(D21_PREPARE_TITLE_V99);
		_readyBtn->setTitleOffset(Vec2(4, 4));
		_readyBtn->setName("AUIButton_ready");
		_readyBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		_readyBtn->setPosition(Vec2(WIN_SIZE.width/2, WIN_SIZE.height/2 - 150));
		addChild(_readyBtn, D21_MIN_ZORDER_V99);
		//_readyBtn->setScale(0.7f);
		_readyBtn->setVisible(false);
		_readyBtn->setPressedActionEnabled(true);
		_readyBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::btnStartCallback, this));

		//抢庄btn
		_catchBankerBtn = AUIButton::create(D21_BLUE_BTN_V99, D21_BLUE_BTN_V99);
		_catchBankerBtn->setTitle(D21_CATCH_TITLE_V99);
		_catchBankerBtn->setTitleOffset(Vec2(4, 4));
		_catchBankerBtn->setName("AUIButton_catchBanker");
		_catchBankerBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		_catchBankerBtn->setPosition(Vec2(WIN_SIZE.width / 2 - 110, WIN_SIZE.height / 2 - 150));
		addChild(_catchBankerBtn, D21_MIN_ZORDER_V99);
		//_catchBankerBtn->setScale(0.7f);
		_catchBankerBtn->setVisible(false);
		_catchBankerBtn->setPressedActionEnabled(true);
		_catchBankerBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::controllBtnClickCallback, this));

		_cancelBankerBtn = AUIButton::create(D21_BLUE_BTN_V99, D21_BLUE_BTN_V99);
		_cancelBankerBtn->setTitle(D21_CANCEL_TITLE_V99);
		_cancelBankerBtn->setTitleOffset(Vec2(4, 4));
		_cancelBankerBtn->setName("AUIButton_cancelBanker");
		_cancelBankerBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		_cancelBankerBtn->setPosition(Vec2(WIN_SIZE.width / 2 + 110, WIN_SIZE.height / 2 - 150));
		addChild(_cancelBankerBtn, D21_MIN_ZORDER_V99);
		//_catchBankerBtn->setScale(0.7f);
		_cancelBankerBtn->setVisible(false);
		_cancelBankerBtn->setPressedActionEnabled(true);
		_cancelBankerBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::controllBtnClickCallback, this));
	}

	void GameTableUIV99::createCard()
	{
		//牌面板位置
		_vecCardPanelPos;
		_vecCardPanelPos.push_back(Vec2(WINSIZE.width / 2, 0));
		_vecCardPanelPos.push_back(Vec2(-150, 40) + _vecPlayerNode.at(1)->getPosition());
		_vecCardPanelPos.push_back(Vec2(300, 0) + _vecPlayerNode.at(2)->getPosition());
		_vecCardPanelPos.push_back(Vec2(150, 40) + _vecPlayerNode.at(3)->getPosition());

		char str[64] = {0};
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			sprintf(str, D21_TABLECARD_PATH_V99, i);
			auto node = CSLoader::createNode(str);
			node->setPosition(_vecCardPanelPos.at(i));
			addChild(node, D21_CARD_ZORDER_V99 + 1);

			vector<Vec2> vecCardStartPos;
			vector<ImageView*> vecPointBg;
			vector<Text*> vecPointText;
			vector<ImageView*> vecPointSpecial;

			//card panel
			for (int j = 0; j < 2; j++)
			{
				//牌
				for (int k = 0; k < CARD_COUNT; k++)
				{
					sprintf(str, "Image_Card_%d%d", j, k);
					auto card = dynamic_cast<ImageView*>(node->getChildByName(str));
					card->setVisible(false);
					card->setLocalZOrder(D21_CARD_ZORDER_V99 + 1);

					if (k == 0)
					{
						vecCardStartPos.push_back(card->getPosition());
					}
				}

				//point bg
				sprintf(str, "Image_PointBg_%d", j);
				auto pointBg = dynamic_cast<ImageView*>(node->getChildByName(str));
				pointBg->setLocalZOrder(D21_CARD_ZORDER_V99 + 2);
				pointBg->setVisible(false);
				vecPointBg.push_back(pointBg);

				//point text
				sprintf(str, "Text_Point_%d", j);
				auto pointText = dynamic_cast<Text*>(pointBg->getChildByName(str));
				vecPointText.push_back(pointText);

				//point special
				sprintf(str, "Image_PointSpecial_%d", j);
				auto pointSpecial = dynamic_cast<ImageView*>(node->getChildByName(str));
				pointSpecial->setLocalZOrder(D21_CARD_ZORDER_V99 + 3);
				pointSpecial->setVisible(false);
				vecPointSpecial.push_back(pointSpecial);
			}
			//每个方位的两组牌起始位
			_vecCardStartPos.push_back(vecCardStartPos);

			_vecPointBg.push_back(vecPointBg);

			_vecPointText.push_back(vecPointText);

			_vecPointSpecial.push_back(vecPointSpecial);
		}
	}

	void GameTableUIV99::createInsurance()
	{
		auto node = CSLoader::createNode(D21_INSULANCE_CSB_V99);
		node->setName("Insurance");
		node->setScale(0);
		node->setPosition(WIN_SIZE / 2);
		addChild(node, D21_CARD_ZORDER_V99 + 1);

		auto cancelBtn = dynamic_cast<Button*>(node->getChildByName("Button_Cancel"));
		cancelBtn->setTag(0);
		cancelBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::insuranceCallBack, this));

		auto confirmBtn = dynamic_cast<Button*>(node->getChildByName("Button_Confirm"));
		confirmBtn->setTag(1);
		confirmBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::insuranceCallBack, this));

		node->runAction(ScaleTo::create(0.3f, 1));
	}

	//下拉框
	void GameTableUIV99::createToolBarUI()
	{
		char resStr[128] = { 0 };

		sprintf(resStr, D21_TOOL_BAR_PATH_V99, "tool_show_hide.png");
		auto toobarBtn1 = Button::create(resStr, resStr);
		toobarBtn1->addClickEventListener([=](Ref*) {
			showToolBar(true);
		});
		toobarBtn1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		toobarBtn1->setPosition(Vec2(10, WIN_SIZE.height - 10));
		toobarBtn1->setTag(em_toolbarBtn);
		this->addChild(toobarBtn1, D21_TOOLBAR_ZORDER_V99);
		toobarBtn1->setVisible(true);

		sprintf(resStr, D21_TOOL_BAR_PATH_V99, "tool_bg_three.png");
		_toolbarBg = Sprite::create(resStr);
		_toolbarBg->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		_toolbarBg->setPosition(10, WIN_SIZE.height + _toolbarBg->getContentSize().height);
		_toolbarBg->setName("0");//0为在上面 1在下面
		this->addChild(_toolbarBg, D21_TOOLBAR_ZORDER_V99 + 1);     //whao

		Size toolbarSize = _toolbarBg->getContentSize();

		sprintf(resStr, D21_TOOL_BAR_PATH_V99, "tool_setting.png");
		auto setPic = Sprite::create(resStr);
		setPic->setPosition(toolbarSize.width / 2, toolbarSize.height * (5.0f / 6.0f));
		_toolbarBg->addChild(setPic);
		auto setBtn = ControlButton::create();
		setBtn->setPreferredSize(setPic->getContentSize());
		setBtn->setPosition(setPic->getPosition());
		setBtn->setTag(em_toolbarSetBtn);
		setBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(GameTableUIV99::onBtnClick), Control::EventType::TOUCH_UP_INSIDE);
		_toolbarBg->addChild(setBtn);

		// 	sprintf(resStr, FQZS_TOOL_BAR_PATH_V99, "tool_help.png");
		// 	auto helpPic = Sprite::create(resStr);
		// 	helpPic->setPosition(toolbarSize.width / 2, toolbarSize.height * (3.0f/8.0f));
		// 	_toolbarBg->addChild(helpPic);
		// 	auto helpBtn = ControlButton::create();
		// 	helpBtn->setPreferredSize(helpPic->getContentSize());
		// 	helpBtn->setPosition(helpPic->getPosition());
		// 	helpBtn->setTag(em_toolbarHelpBtn);
		// 	helpBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(GameTableUIV99::onBtnClick), Control::EventType::TOUCH_UP_INSIDE);
		// 	_toolbarBg->addChild(helpBtn);

		sprintf(resStr, D21_TOOL_BAR_PATH_V99, "tool_exit.png");
		auto leavePic = Sprite::create(resStr);
		leavePic->setPosition(toolbarSize.width / 2, toolbarSize.height * (3.0f / 6.0f));
		_toolbarBg->addChild(leavePic);
		auto leaveBtn = ControlButton::create();
		leaveBtn->setPreferredSize(leavePic->getContentSize());
		leaveBtn->setPosition(leavePic->getPosition());
		leaveBtn->setTag(em_toolbarLeaveBtn);
		leaveBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(GameTableUIV99::onBtnClick), Control::EventType::TOUCH_UP_INSIDE);
		_toolbarBg->addChild(leaveBtn);

		sprintf(resStr, D21_TOOL_BAR_PATH_V99, "tool_next_out.png");
		auto nextRoundOutPic = Sprite::create(resStr);
		nextRoundOutPic->setPosition(toolbarSize.width / 2, toolbarSize.height * (1.0f / 6.0f));
		_toolbarBg->addChild(nextRoundOutPic);
		auto nextRoundOutBtn = ControlButton::create();
		nextRoundOutBtn->setPreferredSize(nextRoundOutPic->getContentSize());
		nextRoundOutBtn->setPosition(nextRoundOutPic->getPosition());
		nextRoundOutBtn->setTag(em_toolbarNextOutBtn);
		nextRoundOutBtn->setUserData((void*)false);
		nextRoundOutBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(GameTableUIV99::onBtnClick), Control::EventType::TOUCH_UP_INSIDE);
		_toolbarBg->addChild(nextRoundOutBtn);

		sprintf(resStr, D21_TOOL_BAR_PATH_V99, "tool_check.png");
		auto checkSprite = Sprite::create(resStr);
		checkSprite->setPosition(Vec2(25, nextRoundOutPic->getContentSize().height / 2 + 4));
		checkSprite->setName("checkSprite");
		checkSprite->setVisible(false);
		nextRoundOutBtn->addChild(checkSprite);
	}

	void GameTableUIV99::onBtnClick(Ref* pSender, Control::EventType event) {
		//HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN_V99);

		Node* btn = (Node*)pSender;
		switch (btn->getTag()) {
		case em_toolbarBtn: {
								showToolBar(true);
		}
			break;
		//case em_toolbarSetBtn: {
		//						   if (this->getChildByName("setLayer")) {
		//							   (dynamic_cast<BRNNSetLayerV99*>(this->getChildByName("setLayer")))->closeSet();
		//							   return;
		//						   }
		//						   auto setLayer = BRNNSetLayerV99::create();
		//						   setLayer->setName("setLayer");
		//						   setLayer->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2);
		//						   setLayer->showSet(this, D21_TOOLBAR_ZORDER_V99);

		//						   showToolBar(false);
		//}
		//	break;
		case  em_toolbarHelpBtn: {
									 showToolBar(false);
		}
			break;
		case em_toolbarLeaveBtn: {
									 if (!RoomLogic()->connected()) {
										 leaveDesk();
									 }
									 else {
// 										 if (m_GameStatus != GS_FREE) {
// 											 // 游戏不在空闲时期，不允许离开
// 											 bool bHasNote = false;
// 											 for (int i = 0; i < GAME_NOTE_COUNT; i++) {
// 												 if (m_i64OwnNotes[i] > 0) {
// 													 bHasNote = true;
// 												 }
// 											 }
// 
// 											 if (bHasNote) {
// 												 auto layer = GamePromptLayer::create();
// 												 layer->showPrompt(GBKToUtf8("游戏过程中不允许离开！"));
// 
// 												 return;
// 											 }
										 }
										 _tableLogic->sendUserUp();
		}
			break;
		case em_toolbarNextOutBtn: {
									   if (btn->getUserData() == (void*)true) {
										   btn->setUserData((void*)false);
										   btn->getChildByName("checkSprite")->setVisible(false);
										   m_bNextOut = false;
									   }
									   else if (btn->getUserData() == (void*)false) {
										   btn->setUserData((void*)true);
										   btn->getChildByName("checkSprite")->setVisible(true);
										   m_bNextOut = true;
									   }
		}
			break;
		default:
			break;
		}
	}

	void GameTableUIV99::hintClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype) {
		if (Widget::TouchEventType::ENDED == touchtype) {
			auto winSize = Director::getInstance()->getWinSize();

			if (_isOpenHelp == false) {
				auto hintLayer = LayerColor::create(Color4B::BLACK);
				hintLayer->setOpacity(10);
				this->addChild(hintLayer, 100);

				auto helpNode = CSLoader::createNode("csb/v99/stud/table/help_node.csb");
				auto bg = static_cast<ImageView*>(helpNode->getChildByName("Image_1"));
				bg->setTouchEnabled(true);
				bg->setSwallowTouches(true);
				helpNode->setPosition(Vec2(WINSIZE.width*0.5f, WINSIZE.height*0.5f));
				helpNode->setScale(0);
				hintLayer->addChild(helpNode, D21_OTEHER_VIEW_ZORDER_V99);

				helpNode->runAction(Sequence::create(ScaleTo::create(0.3f, 0.7f), nullptr));

				auto listener = EventListenerTouchOneByOne::create();
				listener->onTouchBegan = [=](Touch* touch, Event* event) {
					helpNode->runAction(Sequence::create(ScaleTo::create(0.3f, 0.0f), CallFunc::create([=]() {
						hintLayer->removeFromParent();
					}), nullptr));
					return false;
				};
				_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, hintLayer);
			}
		}
	}
	
	void GameTableUIV99::createRecordingBoard()
	{
		auto limitBg = Sprite::create("Games/12100404/1.png");
		limitBg->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
		limitBg->setPosition(Vec2(WIN_SIZE.width - 10, WIN_SIZE.height - 10));
		addChild(limitBg,D21_CREATE_TOOLBAR_ZORDER_V99);

		_recordBoard.Text_history = Text::create("0", "platform/fonts/simhei.ttf", 30);
		_recordBoard.Text_history->setAnchorPoint(Vec2(0, 0.5));
		_recordBoard.Text_history->setContentSize(Size(300, 30));
		_recordBoard.Text_history->setColor(Color3B::WHITE);
		_recordBoard.Text_history->setPosition(Vec2(WIN_SIZE.width - 10, WIN_SIZE.height - 10));
		_recordBoard.Text_history->setPosition(Vec2(138.0f / 1280 * WIN_SIZE.width, limitBg->getContentSize().height - _recordBoard.Text_history->getContentSize().height *0.85f));
		limitBg->addChild(_recordBoard.Text_history);

		_recordBoard.Text_last = Text::create("0", "platform/fonts/simhei.ttf", 30);
		_recordBoard.Text_last->setAnchorPoint(Vec2(0, 0.5));
		_recordBoard.Text_last->setContentSize(Size(300, 30));
		_recordBoard.Text_last->setColor(Color3B::WHITE);
		_recordBoard.Text_last->setPosition(Vec2(120.0f / 1280 * WIN_SIZE.width, limitBg->getContentSize().height - _recordBoard.Text_last->getContentSize().height *2.2f));
		limitBg->addChild(_recordBoard.Text_last);

		_recordBoard.Text_curTurnTotle = Text::create("0", "platform/fonts/simhei.ttf", 30);
		_recordBoard.Text_curTurnTotle->setAnchorPoint(Vec2(0, 0.5));
		_recordBoard.Text_curTurnTotle->setContentSize(Size(300, 30));
		_recordBoard.Text_curTurnTotle->setColor(Color3B::WHITE);
		_recordBoard.Text_curTurnTotle->setPosition(Vec2(86.0f / 1280 * WIN_SIZE.width, limitBg->getContentSize().height - _recordBoard.Text_last->getContentSize().height *3.55f));
		limitBg->addChild(_recordBoard.Text_curTurnTotle);
	}

	//加载筹码按钮
	void GameTableUIV99::createBetButton(long long i64BetUnit[4])
	{
		if (_betBg)
		{
			return;
		}

		_betBg = ImageView::create(D21_BET_BG_V99);
		_betBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		_betBg->setPosition(Vec2(WIN_SIZE.width/2 - _betBg->getContentSize().width/2, 150));
		addChild(_betBg, D21_BET_ZORDER_V99);
		_betBg->setVisible(false);

		float fPosX = _betBg->getContentSize().width/5;
		float fPosY = _betBg->getContentSize().height / 2;

		char str[64] = { 0 };
		for (int i = 0; i < 4; i++)
		{
			sprintf(str, D21_BET_BTN_PATH_V99, i + 7);
			auto btn = AUIButton::create(str, str);
			btn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			btn->setPosition(Vec2(fPosX*(i + 1), fPosY));
			_betBg->addChild(btn, D21_MIN_ZORDER_V99);
			btn->setTag(i);
			btn->setPressedActionEnabled(true);
			btn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::betBtnCallback, this));

			
			int wan = 10000;
			long long money = i64BetUnit[i];
			string s = to_string(money);

			if (i64BetUnit[i] >= wan)
			{
				s = "";
				money = i64BetUnit[i] / wan;
				s += to_string(money);
				s += "万";
			}

			auto numLabel = Text::create(ForceGBKToUtf8(s.c_str()), "platform/fonts/simhei.ttf", 18);
			numLabel->setPosition(Vec2(btn->getContentSize().width/2 - 2, btn->getContentSize().height/2));
			btn->addChild(numLabel);
			numLabel->enableOutline(Color4B::YELLOW, 0.5);

			_vecBetBtn.push_back(btn);
		}
	}

	void GameTableUIV99::setCatchBankerBtnVisible(bool bVisible)
	{
		_catchBankerBtn->setVisible(bVisible);
		_cancelBankerBtn->setVisible(bVisible);
	}

	void GameTableUIV99::setBetBtnVisible(bool bVisible, bool bTouch)
	{
		_betBg->setVisible(bVisible);

		for (int i = 0; i < 4; i++)
		{
			if (_vecBetBtn.at(i))
			{
				_vecBetBtn.at(i)->setTouchEnabled(bTouch);
				int opacity = bTouch ? 255 : 125;
				_vecBetBtn.at(i)->setOpacity(opacity);
			}
		}
	}

	//游戏按钮显示
	void GameTableUIV99::setOperateBtnVisibleState(bool open)
	{
		for (int i = 0; i < _vecOperateBtn.size(); i++)
		{
			_vecOperateBtn.at(i)->setVisible(open);
		}
	}

	//游戏按钮区分显示
	void GameTableUIV99::setOperateBtnVisibleState(bool pdouble, bool pStopCard, bool watCard, bool pBranchCard)
	{
		setOperateBtnVisibleState(true);
		_vecOperateBtn.at(0)->setTouchEnabled(pBranchCard);
		_vecOperateBtn.at(1)->setTouchEnabled(pdouble);
		_vecOperateBtn.at(2)->setTouchEnabled(pStopCard);
		_vecOperateBtn.at(3)->setTouchEnabled(watCard);

		_vecOperateBtn.at(0)->setGray(!pBranchCard);
		_vecOperateBtn.at(1)->setGray(!pdouble);
		_vecOperateBtn.at(2)->setGray(!pStopCard);
		_vecOperateBtn.at(3)->setGray(!watCard);

		//是否还有操作
		bool bHasOperate = true;
		if (!pdouble && !pStopCard && !watCard && !pBranchCard)
		{
			bHasOperate = false;
		}

		if (!bHasOperate)
		{
			runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([&](){
				setOperateBtnVisibleState(false);
			}), nullptr));
		}
	}

	void GameTableUIV99::showToolBar(bool bShow) {
		if (bShow && 0 == _toolbarBg->getName().compare("0")) {
			_toolbarBg->setName("1");
			_toolbarBg->runAction(MoveBy::create(0.3, Point(0, -_toolbarBg->getContentSize().height - 10)));
			this->getChildByTag(em_toolbarBtn)->setVisible(false);
		}
		else if (!bShow && 0 == _toolbarBg->getName().compare("1")) {
			_toolbarBg->setName("0");
			_toolbarBg->runAction(CCSequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height + 10)),
				CallFunc::create([this]() {
				this->getChildByTag(em_toolbarBtn)->setVisible(true);
			})));
		}
	}

	void GameTableUIV99::stopTimer()
	{
		//_clockObj->stop();
		//_clockObj->setVisible(false);
	}

	void GameTableUIV99::starTimer(BYTE timerID, float time, BYTE seateNo)
	{
		_timerID = timerID;
		_timerSeatNo = seateNo;

#if 0
		_clockObj->start(time);
		_clockObj->setVisible(true);
		_clockObj->setPosition(WIN_SIZE / 2);

		std::function<void(void)> callback = [this](){
			_clockObj->setVisible(false);
		};
		_clockObj->setTimeOutCallback(callback);

		switch (timerID)
		{
		case TIME_WAIT:
		{
			if (_MySeatNo == _timerSeatNo)
			{
				std::function<void(void)> callback = [this](){
					_clockObj->setVisible(false);
					_readyBtn->setVisible(false);
					leaveDesk();
				};
				_clockObj->setTimeOutCallback(callback);
			}
			break;
		}
		case TIME_CATCH:
		{
			break;
		}
		case TIME_INSURANCE:
		{
			//购买保险面板，时钟移位
			if (_MySeatNo == _timerSeatNo)
			{
				_clockObj->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 + 230));

				std::function<void(void)> callback = [this](){
					_clockObj->setVisible(false);
					
					auto node = getChildByName("Insurance");
					if (node)
					{
						node->runAction(Sequence::create(ScaleTo::create(0.3, 0.0f), RemoveSelf::create(), nullptr));
					}
				};
				_clockObj->setTimeOutCallback(callback);
			}
			break;
		}
		case  TIME_OPERATE:
		{
			break;
		}
		default:
			break;
		}
#endif
	}

	Vec2 GameTableUIV99::getCardPos(BYTE seatNo, int cardIndex, int iGroupIndex)
	{
		Vec2 startPos = _vecCardStartPos.at(seatNo).at(iGroupIndex);
		Vec2 posInCardPanel = Vec2(startPos.x + (cardIndex * CARD_SPACE), startPos.y);
		Vec2 posCardPanel = _vecCardPanelPos.at(seatNo);
		Vec2 endPos = posCardPanel + posInCardPanel;
		return endPos;
	}

	void GameTableUIV99::beginSendCard()
	{
		_faPaiQiImg->setVisible(true);
		schedule(schedule_selector(GameTableUIV99::displayCard), 0.6f);
	}

	void GameTableUIV99::displayCard(float delta)
	{
		_cardIndex++;
		if (_cardIndex > _cardSendCount)
		{
			_cardIndex = -1;
			unschedule(schedule_selector(GameTableUIV99::displayCard));
			return;
		}

		//服务端下发庄家第一张是看不见的，闲家的牌都是看得见的
		addHandCardList(_seatNum[_cardIndex], _CardValue[_cardIndex]);
	}

	void GameTableUIV99::addHandCardList(BYTE seatNo, BYTE value, bool bSecGroup)
	{
		int iGroupIndex = bSecGroup ? 1 : 0;
		int iZOrder = bSecGroup ? (D21_CARD_ZORDER_V99 - 1) : D21_CARD_ZORDER_V99;

		if (_HandCardCount[seatNo][iGroupIndex] > CARD_COUNT) return;

		int num = _HandCardCount[seatNo][iGroupIndex];

		_HandCardList[seatNo][iGroupIndex][num] = PokerCardV99::create(0);
		_HandCardList[seatNo][iGroupIndex][num]->setPosition(Vec2(_faPaiQiImg->getPositionX() - 30, _faPaiQiImg->getPositionY()));
		_HandCardList[seatNo][iGroupIndex][num]->setScale(0.2f);
		addChild(_HandCardList[seatNo][iGroupIndex][num], iZOrder);

		Vec2 endPos = getCardPos(seatNo, num, iGroupIndex);

		std::function<void(void)> func = ([=](void){
			//计数跟显示点数
			calculatePoint(seatNo, value, iGroupIndex);
			showPoint(seatNo, iGroupIndex, isPointVisible(seatNo, iGroupIndex));
		});

		_HandCardList[seatNo][iGroupIndex][num]->setCardValue(value);
		_HandCardList[seatNo][iGroupIndex][num]->setEndCallback(func);
		_HandCardList[seatNo][iGroupIndex][num]->playSend(endPos, value);

		_HandCardCount[seatNo][iGroupIndex]++;
	}

	void GameTableUIV99::divideCardAnimation(BYTE seatNo, BYTE card[])
	{
		Vec2 endPos = getCardPos(seatNo, 0, 1);

		//两组牌处理
		_HandCardList[seatNo][0][1]->setLocalZOrder(D21_CARD_ZORDER_V99 - 1);
		_HandCardList[seatNo][0][1]->runAction(Sequence::create(MoveTo::create(0.2f, endPos), CallFunc::create([=](){

			_HandCardList[seatNo][1][0] = _HandCardList[seatNo][0][1];
			_HandCardList[seatNo][0][1] = 0;

			_HandCardList[seatNo][0][0]->initWithCardValue(card[0]);
			_HandCardList[seatNo][1][0]->initWithCardValue(card[1]);

			_HandCardCount[seatNo][0]--;
			_HandCardCount[seatNo][1]++;

			showPoint(seatNo, 0, true);
			showPoint(seatNo, 1, true);
		}), nullptr));
	}

	void GameTableUIV99::calculatePoint(BYTE seatNo, BYTE value, int iGroupIndex)
	{
		int cardFigure = (value & 0x0F);				//牌值是0x01~0x0D(表示2,3,4...Q,K,1),所以后面牌值要加1(cardFigure++)

		//A牌
		if (cardFigure == 13)
		{
			if (_points[seatNo][iGroupIndex] <= 10)		//前面的点数相加小于10时来了A就当做11点，否则当做1点
				cardFigure = 10;						//牌值为1本应作为11，结合后面自加1，所以用10
			else
				cardFigure = 0;							//牌值为1本应作为1，结合后面自加1，所以用0
		}
		//10到k
		else if (cardFigure >= 9 && cardFigure < 13)
		{
			cardFigure = 9;								//牌值为10~13本应作为10，结合后面自加1，所以用9
		}
		cardFigure++;
		_points[seatNo][iGroupIndex] += cardFigure;
	}

	bool GameTableUIV99::isPointVisible(BYTE seatNo, int iGroupIndex)
	{
		bool bShow = false;

		if (_NtSeatNo != seatNo)
		{
			bShow = true;
		}
		else if (_NtSeatNo == _MySeatNo)
		{
			bShow = true;
		}
		else if (_HandCardCount[_NtSeatNo][iGroupIndex] > 2)//大于2说明是庄家开始要牌
		{
			bShow = true;
		}

		return bShow;
	}

	void GameTableUIV99::showPoint(BYTE seatNo, int iGroupIndex, bool bShow)
	{
		if (!bShow) return;

		char ptr[20] = { 0 };
		sprintf(ptr, "%d点", _points[seatNo][iGroupIndex]);

		_vecPointBg.at(seatNo).at(iGroupIndex)->setVisible(bShow);
		_vecPointText.at(seatNo).at(iGroupIndex)->setString(ForceGBKToUtf8(ptr));

		//爆点
		if (_points[seatNo][iGroupIndex] > 21)
		{

		}
		else if (_points[seatNo][iGroupIndex] == 21)//最大点数
		{

		}


		if (_points[seatNo][iGroupIndex] >= 21 && seatNo == _MySeatNo)
		{
			setOperateBtnVisibleState(false, false, false, false);
		}
	}

	void GameTableUIV99::dealCatchBankerMsg(GameMessageCatch* msg)
	{
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			showUserReady(_tableLogic->logicToViewSeatNo(i), false);
		}

		//有上家不叫
		if (msg->bPrevChairID != INVALID_BYTE)
		{
			//
		}

		setCatchBankerBtnVisible(msg->bCurChairID == _tableLogic->getMySeatNo());

		starTimer(TIME_CATCH, _tableLogic->_CatchTime, _tableLogic->logicToViewSeatNo(msg->bCurChairID));
	}

	void GameTableUIV99::dealConfirmBankerMsg(GameMessageNt* msg)
	{
		BYTE viewBankerSeatNo = _tableLogic->logicToViewSeatNo(msg->bNtStation);

		_NtSeatNo = viewBankerSeatNo;

		//隐藏
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			showUserReady(_tableLogic->logicToViewSeatNo(i), false);
		}

		//
		setCatchBankerBtnVisible(false);
		if (_arrPlayer[_NtSeatNo])
		{
			_arrPlayer[_NtSeatNo]->showBanker(true, true);
		}
	}

	void GameTableUIV99::dealBetMsg(GameMessageBetResult* msg)
	{
		//开始下注
		if (msg->bStartBet)
		{
			setCatchBankerBtnVisible(false);

			//是否是庄家
			if (_MySeatNo == _NtSeatNo)
			{
				//
			}
			else
			{
				setBetBtnVisible(true, true);
			}

			starTimer(TIME_BET, _tableLogic->_BetTime);
		}
		else//下注结果
		{
			BYTE viewBetSeatNo = _tableLogic->logicToViewSeatNo(msg->bDeskStation);

			if (_arrPlayer[viewBetSeatNo])
			{
				_arrPlayer[viewBetSeatNo]->updateBetMoney(msg->i64Note, msg->i64TotalNote);
				_arrPlayer[viewBetSeatNo]->setUserMoney(msg->i64UserMoney);

				if (viewBetSeatNo == _MySeatNo)
				{
					setBetBtnVisible(false, false);
				}
			}
		}
	}

	void GameTableUIV99::dealStartSendCardMsg(GameMessageSendCard* msg)
	{
		//第一个发牌的人
		int firstPeople = msg->bFirstPeople;

		//每人两张手牌
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0, t = firstPeople; j < PLAY_COUNT; j++, t++)
			{
				if (t >= PLAY_COUNT)
				{
					t = 0;
				}

				int viewSeatNo = _tableLogic->logicToViewSeatNo(t);
				if (_arrPlayer[viewSeatNo] && _tableLogic->_bPlayerStation[t] != PLAYER_JOIN)
				{
					_cardSendCount++;
					_seatNum[_cardSendCount] = viewSeatNo;
					_CardValue[_cardSendCount] = msg->bUserCard[t][i];
				}
			}
		}

		//开始发牌
		beginSendCard();

		starTimer(TIME_SEND, msg->bSendCardTime, INVALID_BYTE);
	}

	void GameTableUIV99::dealBuyInsuranceMsg(GameMessageInsurance* msg)
	{
		//是INVALID_BYTE说明是保险回包,是1是通知买保险
		if (msg->bIsInsurance == INVALID_BYTE)
		{
			//买不买
			if (msg->bIsBuy)
			{

			}
			else
			{

			}
		}
		else if (msg->bIsInsurance == 1)
		{
			BYTE viewSeatNo = _tableLogic->logicToViewSeatNo(msg->bDeskStation);
			starTimer(TIME_INSURANCE, _tableLogic->_insuranceTime, viewSeatNo);
			if (viewSeatNo == _MySeatNo)
			{
				createInsurance();
			}
		}
	}

	void GameTableUIV99::dealOperateNotifyMsg(GameMessageOperateNotify* msg)
	{
		//有操作
		if (msg->bHasOperate)
		{
			BYTE viewSeatNo = _tableLogic->logicToViewSeatNo(msg->bDeskStaion);
			if (viewSeatNo == _MySeatNo)
			{
				setOperateBtnVisibleState(msg->bCanDouble, true, true, msg->bCanDivide);
			}

			starTimer(TIME_OPERATE, _tableLogic->_CallCardTime, viewSeatNo);
		}
		else
		{
			
		}

	}

	void GameTableUIV99::dealDivideResultMsg(GameMessageDivideResult* msg)
	{

		BYTE viewSeatNo = _tableLogic->logicToViewSeatNo(msg->bDeskStation);

		//牌
		divideCardAnimation(viewSeatNo, msg->bCard);

		//下注，金币数已经扣除筹码
		if (_arrPlayer[viewSeatNo])
		{
			_arrPlayer[viewSeatNo]->updateBetMoney(msg->i64Note, msg->i64TotalNote);
			_arrPlayer[viewSeatNo]->setUserMoney(msg->i64UserMoney);
		}
	}

	void GameTableUIV99::showUserReady(BYTE seatNo, bool visible)
	{
		if (!seatNoIsOk(seatNo))	return;

		if (_arrPlayer[seatNo])
		{
			_arrPlayer[seatNo]->showReady(visible);
			if (seatNo == _MySeatNo)
			{
				_readyBtn->setVisible(false);
			}
		}
	}

	void GameTableUIV99::showBtnStart(BYTE seatNo)
	{
		_GameStatus = GS_WAIT_ARGEE;
		_readyBtn->setVisible(true);
	}

	void GameTableUIV99::setNt(BYTE seatNo, bool IsMe)
	{
		if (!seatNoIsOk(seatNo))	return;
		_NtSeatNo = seatNo;
		if (_arrPlayer[seatNo])
		{
			_arrPlayer[seatNo]->showBanker(true, true);
		}

		for (int i = 0; i < PLAY_COUNT; i++)
		{
			showUserReady(_tableLogic->logicToViewSeatNo(i), false);
		}
	}

	void GameTableUIV99::leaveDesk()
	{
		//RoomLogic()->close();
		HNAudioEngine::getInstance()->resumeBackgroundMusic();
		if (RoomLogic()->getRoomRule() & GRR_QUEUE_GAME) {
			RoomLogic()->closeRoom();
			//GamePlatform::returnPlatform(LayerType::ROOMLIST);
			PlatformState* state = PlatformState::create();
			if (VERSION_EIGHTY_EIGHT) {
				state->setType(ROOMLIST);
			}
			else if (VERSION_NINETY_NINE) {
				state->setType(GAMELIST_ROOM);
			}
			else {
				state->setType(ROOMLIST);
			}
			StateManager::getInstance()->setState(state);
		}
		else {
			RoomLogic()->closeRoom();
			//GamePlatform::returnPlatform(LayerType::ROOMLIST); // 临时处理2016 7 28 返回到房间
			PlatformState* state = PlatformState::create();
			if (VERSION_EIGHTY_EIGHT) {
				state->setType(ROOMLIST);
			}
			else if (VERSION_NINETY_NINE) {
				state->setType(GAMELIST_ROOM);
			}
			else {
				state->setType(ROOMLIST);
			}
			StateManager::getInstance()->setState(state);
		}
	}

	/************************line*********************************/
	void GameTableUIV99::initParams()
	{
		_NtSeatNo	   = INVALID_DESKSTATION;
		_MySeatNo	   = INVALID_DESKSTATION;
		_betMoney	   = 0;
		_betMax		   = 0;
		_GameStatus = GS_WAIT_NEXT;
		memset(_points, 0, sizeof(_points));

		//send card order
		_cardSendCount  = -1;
		_cardIndex	   = -1;
		memset(_seatNum, INVALID_DESKSTATION, sizeof(_seatNum));
		memset(_CardValue, INVALID_DESKSTATION, sizeof(_CardValue));

		memset(_betjeton, 0, sizeof(_betjeton));

		memset(_HandCardList, 0, sizeof(_HandCardList));
		memset(_HandCardCount, 0, sizeof(_HandCardCount));
	}

	void GameTableUIV99::refreshData()
	{
		Size winSize = Director::getInstance()->getWinSize();

		//游戏结束，清除手牌
		removeCards();

// 		for (auto card : _vecPockerCards)
// 		{
// 			card->removeFromParent();
// 		}

		setOperateBtnVisibleState(false);
		_readyBtn->setVisible(true);
		//移除结算
		//ResultV99::removeResult(this);

		for (auto chip : _vecChips)
		{
			chip->removeFromParent();
		}

		_vecChips.clear();

		for (int i = 0; i < PLAY_COUNT; i++)
		{
			if (_arrPlayer[i] != nullptr)
			{
// 				_arrPlayer[i]->removeBankerAnimation();
// 				_arrPlayer[i]->upDataNote(0);
			}
		}

		_NtSeatNo	   = INVALID_DESKSTATION;
		_betMoney	   = 0;
		_betMax		   = 0;
		memset(_points, 0, sizeof(_points));

		//send card order
		_cardSendCount  = -1;
		_cardIndex	   = -1;
		memset(_seatNum, INVALID_DESKSTATION, sizeof(_seatNum));
		memset(_CardValue, INVALID_DESKSTATION, sizeof(_CardValue));

		memset(_betjeton, 0, sizeof(_betjeton));

		memset(_HandCardList, 0, sizeof(_HandCardList));
		memset(_HandCardCount, 0, sizeof(_HandCardCount));
	}
	
	bool GameTableUIV99::onTouchBegan(Touch* touch, Event* event) {

		if (0 == _toolbarBg->getName().compare("1"))
		{
				_toolbarBg->runAction(CCSequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height + 0.5f)),
				CallFunc::create([this]() {
				_toolbarBg->setName("0");
				this->getChildByTag(em_toolbarBtn)->setVisible(true);
			})));
			return true;
		}
		return true;
	}
	
	void GameTableUIV99::btnStartCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
	{
		if (touchtype != Widget::TouchEventType::ENDED)	return;

		HNAudioEngine::getInstance()->playEffect("d21/sound/cue.mp3");

		auto btn = (AUIButton*)pSender;

		stopTimer();
		_readyBtn->setVisible(false);
		_tableLogic->requestReady();
	}

	void GameTableUIV99::controllBtnClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
	{
		if (touchtype != Widget::TouchEventType::ENDED)	return;

		auto btn = (AUIButton*)pSender;
		auto btnName = btn->getName();
		 
		if (btnName.compare("AUIButton_double") == 0)					//加倍
		{
			_tableLogic->requestDoublle();
			setOperateBtnVisibleState(false, false, false, false);

			//加倍的时候就认为默认点了停止按钮 这个后期要删掉，集成到服务端改 need delete qiuyx
			_tableLogic->requestStopCard();
		}
		else if (btnName.compare("AUIButton_wantCard") == 0)			//要牌
		{
			_tableLogic->requestNeedCard();
			//setOperateBtnVisibleState(false, false, false, false);
		}
		else if (btnName.compare("AUIButton_branchCard") == 0)			//分牌
		{
			_tableLogic->requestDividedCard();
			//setOperateBtnVisibleState(false, false, false, false);
		}
		else if (btnName.compare("AUIButton_stopCard") == 0)			//停牌
		{
			_tableLogic->requestStopCard();
			setOperateBtnVisibleState(false, false, false, false);
		}
		else if (btnName.compare("AUIButton_cancelBanker") == 0)		//不抢庄
		{
			GameStationButton toSData;
			toSData.iBtParam= 0;
			toSData.bDeskStation = _NtSeatNo;
			_tableLogic->requestCatchBanker(&toSData);
		}
		else if (btnName.compare("AUIButton_catchBanker") == 0)			//抢庄
		{
			GameStationButton toSData;
			toSData.iBtParam = 1;
			toSData.bDeskStation = _NtSeatNo;
			_tableLogic->requestCatchBanker(&toSData);
		}
	}

	//保险回调
	void GameTableUIV99::insuranceCallBack(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
	{
		if (touchtype != Widget::TouchEventType::ENDED)	return;

		auto btn = dynamic_cast<Button*>(pSender);
		auto tag = btn->getTag();

		GameStationButton pData;
		pData.iBtType = CLICK_INSURANCE;
		pData.bDeskStation = _tableLogic->viewToLogicSeatNo(_MySeatNo);

		// 1 确定 0 取消
		if (tag == 0)
		{
			//不购买
			pData.iBtParam = 0;
		}
		else
		{
			//够买
			pData.iBtParam = 1;
		}
		
		_tableLogic->requestInsurance(&pData);

		//
		auto node = getChildByName("Insurance");
		if (node)
		{
			node->runAction(Sequence::create(ScaleTo::create(0.3f, 0.0f), RemoveSelf::create(), nullptr));
		}	
	}

	void GameTableUIV99::betBtnCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
	{
		if (touchtype != Widget::TouchEventType::ENDED)	return;

		HNAudioEngine::getInstance()->playEffect("d21/sound/cue.mp3");

		if (_tableLogic->_gameStatus != GS_BET) return;

		auto btn = (AUIButton*)pSender;
		int tag = btn->getTag();

		_betMoney = _tableLogic->_i64BetUnit[tag];

		//请求下注
		_tableLogic->requestSendBet(_betMoney);

		setBetBtnVisible(true, false);
		stopTimer();
	}

	/****************************************************************/
	void GameTableUIV99::addUser(BYTE seatNo, bool bMe,LLONG userId)
	{
#if 1
		if (!seatNoIsOk(seatNo))	return;
		if (bMe)
		{
			_MySeatNo = seatNo;
		}
		if (_arrPlayer[seatNo] == nullptr)
		{
			//auto player = PlayerUIV99::create(userId, seatNo);
			auto player = PlayerUIV99::create();
			if (player)
			{
				player->initUI(userId, seatNo);
				//player->retain();
				_arrPlayer[seatNo] = player;
				_vecPlayerNode.at(seatNo)->addChild(player);
			}
		}
#endif
	}

	//更新玩家计分面板
	void GameTableUIV99::upDataRecordBoard(LLONG historyMoney, LLONG lastMoney, LLONG allPlayerMoney)
	{
		_recordBoard.Text_curTurnTotle->setString(to_string(allPlayerMoney));
		_recordBoard.Text_history->setString(to_string(historyMoney));
		_recordBoard.Text_last->setString(to_string(lastMoney));
	}

	void GameTableUIV99::upDataRecordBoard(LLONG allPlayerMoney)
	{
		_recordBoard.Text_curTurnTotle->setString(to_string(allPlayerMoney));
	}

	//添加结算面板
	void GameTableUIV99::openResultView(bool open)
	{
		if (open)
		{
			ResultV99::openResult(this, D21_RESULT_VIEW_V99);
		}
		else
		{
			ResultV99::removeResult(this);
		}
	}

	void GameTableUIV99::upDataResult(std::string name, LLONG money, LLONG cardShap, bool blose,int seatnum)
	{
		ResultV99::getResult(this)->upDataResult(name, money, cardShap, blose);

		//结算的时候如果赢了就更新金币数目，输了不更新（下筹码的时候已经更新了）
		BYTE viewSeatNo = _tableLogic->logicToViewSeatNo(seatnum);
		if (_arrPlayer[viewSeatNo]&&!blose)
		{			
			_arrPlayer[viewSeatNo]->setUserMoney(_arrPlayer[viewSeatNo]->getUserMoney()+money);
		}
	}

	void GameTableUIV99::removeUser(BYTE seatNo, bool bMe)
	{
		if (!seatNoIsOk(seatNo))	return;
		if (_arrPlayer[seatNo])
		{
			_arrPlayer[seatNo]->removeFromParentAndCleanup(true);
			_arrPlayer[seatNo] = nullptr;
		}
	}

	//筹码动画
	void GameTableUIV99::playChipAnimation(BYTE seatNo, BYTE chipType)
	{
		char framPath[80] = { 0 };
		sprintf(framPath, "v99/niuniu/xiazhu/bet%d.png", chipType);
		auto pos = Vec2(0, 0);
		auto size = _table.chipArea->getContentSize();
		auto tmp = Sprite::create(framPath);
		auto rect = _table.chipArea->getBoundingBox();
		auto chipSize = tmp->getContentSize();
		rect.origin = rect.origin;
		rect.origin.x += chipSize.width / 2;
		rect.origin.y += chipSize.width / 2;
		rect.size.width -= chipSize.width;
		rect.size.height -= chipSize.height;

		Vec2 position2;
		for (int i = 0; i < PLAY_COUNT*2; i++)
		{
			bool flag;
			do {
				position2 = rect.origin;
				auto xOffset = fmod(rand()*(rand() % 2 ? -1 : 1), size.width);
				auto yOffset = fmod(rand()*(rand() % 2 ? -1 : 1), size.height);

				position2.x += xOffset;
				position2.y += yOffset;

				flag = rect.containsPoint(position2);
			} while (!flag);

			auto chip = Sprite::create(framPath);
			chip->setScale(1.3f);
			chip->setPosition(_vecPlayerNode.at(seatNo)->getPosition());
			_table.tableRoot->addChild(chip,1);
			_vecChips.push_back(chip);
			chip->runAction(MoveTo::create(0.2f, position2));
		}
	}

	void GameTableUIV99::dealOperateResp(GameMessageNeedCard* msg)
	{
		BYTE seatNoView = _tableLogic->logicToViewSeatNo(msg->bDeskStation);
		BYTE nextSeatNoView = _tableLogic->logicToViewSeatNo(msg->bNextStation);

		//流程继续
		if (msg->bFlag)
		{
			switch (msg->iStyle)
			{
			case SERVRE_EXTRA_START_OPERATE:{
				startOperate(seatNoView, msg->bCanDivide);
				break;
			}
			case SERVER_EXTRA_DOUBLE:{
				addDouble(seatNoView, msg->bCard, msg->i64Note);
				break;
			}
			case SERVER_EXTRA_CLICK_GIVEUP:{
				//giveUpCard(seatNo);
				break;
			}
			case SERVER_EXTRA_CLICK_NEED:{
				needCard(seatNoView, msg->bCard, msg->iPoints);
				break;
			}
			case SERVER_EXTRA_CLICK_STOP:{
				stopCard(seatNoView);
				break;
			}

			default:
				break;
			}

			//time
			if (msg->bNextStation == INVALID_BYTE)
			{
				starTimer(TIME_OPERATE, _tableLogic->_CallCardTime, seatNoView);
			}
			else
			{
				starTimer(TIME_OPERATE, _tableLogic->_CallCardTime, nextSeatNoView);
				startOperate(seatNoView, msg->bCanDivide);
			}
		}
		else
		{
			//爆牌，发停牌消息。要显示爆牌的icon need delete qiuyx
			_tableLogic->requestStopCard();
			setOperateBtnVisibleState(false, false, false, false);

			//所有人要牌结束
			stopTimer();
			if (msg->bCard > 0)//说明是加牌结束
			{
				needCard(seatNoView, msg->bCard, msg->iPoints);
			}


		}
	}

	void GameTableUIV99::startOperate(BYTE seatNo, bool bCanDivide)
	{
		if (!seatNoIsOk(seatNo))	return;
		if (seatNo == _MySeatNo)
		{
			bool bCanDouble = false;
			if (_MySeatNo == _NtSeatNo)
			{
				bCanDouble = false;
			}
			else
			{
				bCanDouble = (_arrPlayer[_MySeatNo]->getUserMoney() >= _betMoney )? true : false;
			}

			setOperateBtnVisibleState(bCanDouble, true, true, bCanDivide);
		}
	}

	void GameTableUIV99::addDouble(BYTE seatNo, BYTE value, LLONG money)
	{
		if (!seatNoIsOk(seatNo))	return;
		HNAudioEngine::getInstance()->playEffect("d21/sound/adddouble.mp3");

		addHandCardList(seatNo, value);
		//setBetResult(seatNo, money * 2);
	}

	void GameTableUIV99::giveUpCard(BYTE seatNo)
	{
		if (!seatNoIsOk(seatNo))	return;
		if (seatNo == _MySeatNo)
		{
			setOperateBtnVisibleState(false, false, false, false);
		}
	}

	void GameTableUIV99::needCard(BYTE seatNo, BYTE value, int points)
	{
		if (!seatNoIsOk(seatNo))	return;

		addHandCardList(seatNo, value);
	}

	void GameTableUIV99::stopCard(BYTE seatNo)
	{
		if (!seatNoIsOk(seatNo))	return;
		if (seatNo == _MySeatNo)
		{
			setOperateBtnVisibleState(false, false, false, false);
		}

		stopTimer();
	}

	void GameTableUIV99::setCallingbtnStatus(bool MeIsNt, bool bCanDivide)
	{
		if (MeIsNt)
		{
			//庄家不能加倍，分牌
			setOperateBtnVisibleState(false, true, true, bCanDivide);
		}
		else 
		{
			//闲家是否够钱加倍
			//漏判断是否可以分牌
			if (_arrPlayer[_MySeatNo]->getUserMoney() >= _betMoney)
			{
				setOperateBtnVisibleState(true, true, true, bCanDivide);
			}
			else
			{
				setOperateBtnVisibleState(false, true, true, bCanDivide);
			}		
		}
	}

	void GameTableUIV99::hideNtstates(BYTE seatNo)
	{
		//_arrPlayer[seatNo]->removeBankerAnimation();
	}

	void GameTableUIV99::setMsg(const std::string &msg)
	{
		GamePromptLayer::create()->showPrompt(msg.c_str());
	}

/****************************************************************/

	bool GameTableUIV99::seatNoIsOk(BYTE seatNo)
	{
		return (seatNo < PLAY_COUNT && seatNo >= 0);
	}

	void GameTableUIV99::removeCards()
	{
		//清除卡牌
		for (int i=0;i<PLAY_COUNT;i++)
		{
			for (int j = 0; j < 2; j++)
			{
				if (_HandCardList[i][j])
				{
					for (int k = 0; k < CARD_COUNT; k++)
					{
						if (_HandCardList[i][j][k])
						{
							_HandCardList[i][j][k]->removeFromParent();
						}
					}
				}

			}
		}

		//清除点数
		for (int i = 0; i < _vecPointBg.size(); i++)
		{
			for (int j = 0; j < _vecPointBg[i].size(); j++)
			{
				if (_vecPointBg[i][j])
				{
					_vecPointBg[i][j]->setVisible(false);
				}
			}
		}

	}

}

