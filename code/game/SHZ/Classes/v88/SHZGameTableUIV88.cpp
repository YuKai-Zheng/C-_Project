 #include "../../../../code/common/LibHNLobby/HNLobby/GameLists.h"
//#include "../../TTNN/Classes/v88/BRNNSetLayerV88.h"
//#include "../../TTNN/Classes/GameTable/BRNNActions.h"
#include "../../../../code/common/LibHNUI/UI/UIGoldRain.h"
#include "../../../../code/common/LibHNLobby/HNLobby/GameNotice.h"
#include "HNCommon/HNConverCode.h"
#include "HNLobby/v66/ExtraReward.h"
#include "SHZGameTableUIV88.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/pattern/StateManager.h"
#include "HNLobby/state/PlatformState.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "SHZSettlementV88.h"
#include "SHZGuessSizeV88.h"
#include "HNLobby/pattern/factory/gameSet/GameSetFactory.h"
#include "UI/HelpLayer.h"
#include "UI/ToolBarLayer.h"
#include "UI/Scanner.h"
#include "UI/FloatCoin.h"
#include "base/CCRef.h"
#include "UI/HelpLayer.h"
#include "UI/FloatCoin.h"
#include "Tool/Tools.h"
#include "UI/MoneyGetFourLevel.hpp"
#include "UI/BigWinner.hpp"
#include "HNLobby/v66/IGameLoadingUI.hpp"
#include "HNLobby/PlatformDefine.h"
namespace SHZ {

#define WINSIZE Director::getInstance()->getWinSize()
#define NOTICE_ZORDER 10000
#define ICON_HEIGHT 144
	////////////////////////////////////////////////////////////////////////////
	////常量

	enum Em_TableBtnTag_V88 {
		em_toolbarBtn = 1000,
		em_toolbarChangeTableBtn,
		em_toolbarSetBtn,
		em_toolbarHelpBtn,
		em_toolbarStandupBtn,
		em_toolbarLeaveBtn,
		em_toolbarNextOutBtn
	};

	enum Em_BetButtonTag_V88 {
		//bet
		em_betBarTag = 1100, //Bar
		em_btnSmallTag,
		em_btnBigTag,
		em_btnJiazhuTag,
		em_btnJianzhuTag,
		em_btnAutoTag,
        em_btnXiafenTag,
        em_btnsubNoteTag,
        em_btnZuidayafenTag,
	};

	enum Em_BetMoneyTag_V88 {
		em_money0Tag = 1200,    //下注筹码表识
		em_money1Tag,
		em_money2Tag,
		em_money3Tag,
		em_money4Tag
	};


	enum {
		ZOrder_Bg = 100,
		ZOrder_Loop = 150,
		ZOrder_Cell = 160,
		ZOrder_BetButtonNotes = 210,
		ZOrder_BetButtonLabel = 300,
		ZOrder_CarSprite = 310,
		ZOrder_CarLight = 311,
		ZOrder_BetButton = 315,
		ZOrder_MyInfo = 325,
		ZOrder_StateLayer = 330,
		ZOrder_Timer = 335,
		ZOrder_SettlementLayer = 340,
		ZOrder_LightBar = 345,
		ZOrder_Jackpot = 345,
		ZOrder_GameState = 375,
		ZOrder_Tool = 400,
	};



	//转盘变速类型
	enum LoopType {
		Loop_Start,
		Loop_Max_Speed,
		Loop_Slow_Speed,
		Loop_End
	};


	//////////////////////////////////////////////////////////////////////////
	//CSB文件

	static const char* SHZ_ROOT_V88_CSB = "Games/31000503/Node_Root_V88.csb";

	////////////////////////////////////////////////////////////////////////////
	//图片资源
	//background

	////cloc

	static const char* SHZ_LOGO_PATH = "Games/31000503/shuihuzhuang/kind_%d.png";
	static const char* SHZ_LOGO_G_PATH = "Games/31000503/shuihuzhuang/kindG_%d.png";
	static const char* AUTO_TITLE_PATH = "Games/31000503/shuihuzhuang/38.png";
	static const char* HANDLE_TITLE_PATH = "Games/31000503/shuihuzhuang/45.png";
	//music
//    static const char* SHZ_SHZ_MUSIC = "Games/Medusa/music/medusaBg.mp3";//背景音乐
    static const char* SHZ_SHZ_MUSIC = "Games/31000503/music/xiongdiwushu.mp3";//背景音乐
	static const char* SHZ_ROLL_MUSIC = "Games/31000503/music/gundong.mp3";// 滚动的音乐
	static const char* SHZ_LINE_MUSIC = "Games/31000503/music/line.mp3";// 滚动的音乐
	static const char* SHZ_WIN_END_MUSIC = "Games/31000503/music/win_end.mp3";// 滚动的音乐


	//特效
	const static char* ANIMATION_REWARD_AXE_CSB = "v88/animation/watermargin/water_result/axe.csb";
	const static char* ANIMATION_REWARD_FLAG_CSB = "v88/animation/watermargin/water_result/flag.csb";
	const static char* ANIMATION_REWARD_BROADSWORD_CSB = "v88/animation/watermargin/water_result/broadsword.csb";
	const static char* ANIMATION_REWARD_HALBERD_CSB = "v88/animation/watermargin/water_result/halberd.csb";
	const static char* ANIMATION_REWARD_LINCHONG_CSB = "v88/animation/watermargin/water_result/linchong.csb";
	const static char* ANIMATION_REWARD_LUZHISHEN_CSB = "v88/animation/watermargin/water_result/luzhishen.csb";
	const static char* ANIMATION_REWARD_SONGJIANG_CSB = "v88/animation/watermargin/water_result/songjiang.csb";
	const static char* ANIMATION_REWARD_ZHONGYI_CSB = "v88/animation/watermargin/water_result/zhongyi.csb";
	const static char* ANIMATION_REWARD_WATERMARGIN_CSB = "v88/animation/watermargin/water_result/watermargin.csb";
	//
	static const char* SHZ_LOADING_ANI_PATH = "v88/animation/watermargin/water_loading/water_loading.csb";
	const static char* ANIMATION_WATER_SCROLLBAR_CSB = "v88/animation/watermargin/water_scrollbar/water_scrollbar.csb";




	//static const char* SHUIGUOJI_SHOOT_EFFECT = "v88/music/Shuiguoji/Shoot_%d.mp3";//猜大小进行中



	//////////////////////////////////////////////////////////////////////////

	HNGameUIBase* GameTableUIV88::create(BYTE bDeskIndex, bool bAutoCreate) {
		GameTableUIV88* tableUI = new GameTableUIV88();
		if (tableUI && tableUI->init(bDeskIndex, bAutoCreate)) {
			tableUI->autorelease();
		}
		else {
			delete tableUI;
			tableUI = nullptr;
		}
		return tableUI;
	}

	GameTableUIV88::GameTableUIV88()
    : m_i64WinMoney(0)
		//    : m_iBetMoneyType(-1)
	{
		//    TextureCache::getInstance()->addImage(NN_GOLD_IMAGE);
        scoreText = nullptr;
        m_titleImg= nullptr;
        m_winImg  = nullptr;
        m_winNum  = nullptr;
        m_bibeiTip=nullptr;
        m_defenTip=nullptr;
	}

	GameTableUIV88::~GameTableUIV88() {

		this->unschedule(schedule_selector(GameTableUIV88::updateTip));
	
		CC_SAFE_DELETE(_GameTableLogic);

		resetData();
	}

	bool GameTableUIV88::init(BYTE bDeskIndex, bool bAutoCreate) {
		if (!HNGameUIBase::init()) {
			return false;
		}

		resetData();
		_GameTableLogic = new GameTableLogic(this, bDeskIndex, bAutoCreate, PLAYER_COUNT);

		return true;
	}

    void GameTableUIV88::createRootLayer() {
        /*auto bgBig = ImageView::create("Games/31000503/ui/bgBig.png");
        if (bgBig) {
            bgBig->setPosition(WIN_SIZE / 2);
            addChild(bgBig, ZOrder_Loop - 1);
        }*/
		root = CSLoader::createNode(SHZ_ROOT_V88_CSB);
		if (root) {
           /* auto titleimg = root->getChildByName("shuihuzhuang_1");
            if (titleimg) {
                titleimg->setPositionY(titleimg->getPositionY() - 10);
            }*/
			root->setPosition(WIN_SIZE / 2);
			addChild(root, ZOrder_Loop);
            
            auto panelBg = dynamic_cast<Layout*>(root->getChildByName("Panel_1"));
            if (panelBg) {
                
				//qiuyx animation
                auto jsonPath      = "Games/31000503/top/shuihuzhuan_sl.json";
                auto atlasPath     = "Games/31000503/top/shuihuzhuan_sl.atlas";
                topSpine  = spine::SkeletonAnimation::createWithJsonFile(jsonPath,atlasPath);
                if (topSpine) {
                    topSpine->setPosition(Vec2(0,0));
                    topSpine->setAnimation(0, "jing",true);
                    panelBg->addChild(topSpine,1);
                }
                
            }
            
			auto bg = Sprite::create("Games/31000503/shuihuzhuang/bg.png");
			addChild(bg, ZOrder_Bg);
			bg->setPosition(WIN_SIZE / 2);
			bg->setLocalZOrder(-10);
			
			/*char name[32] = { 0 };
			allLogoSpriteVec.clear();
			for (int i = 0; i < LOTTERY_COUNT; i++)
			{
				sprintf(name, "result_%d", i);
				auto sp = dynamic_cast<Sprite*>(root->getChildByName(name));
				allLogoSpriteVec.push_back(sp);
				sp->setZOrder(-3);
	
			}*/
			////////////////
			char name1[32] = { 0 };
			for (int i = 0; i < LINE_COUNT; i++)
			{
				sprintf(name1, "xian%d", i+1);
				Sprite* sp = dynamic_cast<Sprite*>(root->getChildByName(name1));
				sp->setVisible(false);
				allLineSpriteVec.push_back(sp);
            }

			////////////////////
			yazhuText = dynamic_cast<Text*>(root->getChildByName("yazhuText"));
			zongyazhuText = dynamic_cast<Text*>(root->getChildByName("zongyazhuText"));
			myLastMoneyText = dynamic_cast<Text*>(root->getChildByName("myMoneyText"));
			myName = dynamic_cast<Text*>(root->getChildByName("textName"));
            scoreText = dynamic_cast<Text*>(root->getChildByName("scoreText"));

			///
            startBtn = dynamic_cast<Button*>(root->getChildByName("starBtn"));
			if (startBtn)
			{
				startBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::startButtonCallBack, this));
			}
			shoudongBtn = dynamic_cast<Button*>(root->getChildByName("shoudongBtn"));
			if (shoudongBtn)
			{
				shoudongBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::autoGameButtonCallBack, this));
				shoudongBtn->setTag(em_btnAutoTag);
			}
            cancelAutoBtn = dynamic_cast<Button*>(root->getChildByName("shoudongBtn2"));
            if (cancelAutoBtn) {
                cancelAutoBtn->setVisible(false);
                cancelAutoBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::autoGameButtonCallBack, this));
            }
			//加注减注
            bibeiBtn = dynamic_cast<Button*>(root->getChildByName("bibeiBtn"));
			if (bibeiBtn)
			{
				bibeiBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::chipInButtonCallBack, this));
				bibeiBtn->setTag(em_btnJianzhuTag);
                setButtonEnabled(bibeiBtn, false);
			}
			jiazhuBtn = dynamic_cast<Button*>(root->getChildByName("jiazhuBtn"));
			if (jiazhuBtn)
			{
				jiazhuBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::chipInButtonCallBack, this));
				jiazhuBtn->setTag(em_btnJiazhuTag);
			}
            jianzhuBtn = dynamic_cast<Button*>(root->getChildByName("jianzhuBtn"));
            if (jianzhuBtn)
            {
                jianzhuBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::chipInButtonCallBack, this));
                jianzhuBtn->setTag(em_btnsubNoteTag);
            }
            
            //最大压分
            zuidayafen =  dynamic_cast<Button*>(root->getChildByName("zuidayafen"));
            if (zuidayafen) {
                zuidayafen->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::chipInButtonCallBack, this));
                zuidayafen->setTag(em_btnZuidayafenTag);
            }
            
            //下分
            xiafenBtn = dynamic_cast<Button*>(root->getChildByName("xiafenBtn"));
            if (xiafenBtn)
            {
                xiafenBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::chipInButtonCallBack, this));
                xiafenBtn->setTag(em_btnXiafenTag);
               
            }
            
            m_titleImg = dynamic_cast<ImageView*>(root->getChildByName("Image_title"));
            if (m_titleImg) {
                m_titleImg->runAction(RepeatForever::create(Blink::create(0.5f,1)));
            }
            
            m_winImg   = dynamic_cast<ImageView*>(root->getChildByName("Image_win"));
            if (m_winImg) {
                m_winImg->setVisible(false);
            }
            m_winNum   = dynamic_cast<TextAtlas*>(root->getChildByName("AtlasLabel_1"));
            if (m_winNum) {
                m_winNum->setVisible(false);
            }
            m_bibeiTip = dynamic_cast<ImageView*>(root->getChildByName("Image_bibei"));
            m_defenTip = dynamic_cast<ImageView*>(root->getChildByName("Image_defen"));
		}
	}
    
    void GameTableUIV88::loadTurn() {
        char str[128] = {0};
        for (int i = 0; i < SHZ_COLUMN; i++) {
                //转动的容器
            memset(str, 0, sizeof(str));
            sprintf(str, "Panel_arena%d", i+1);
            Layout* layout = dynamic_cast<Layout*>(root->getChildByName(str));
            if (layout) {
                layout->setClippingEnabled(true);
                m_vPanelTurnArea.push_back(layout);
            }

        }
        
        //转动的icon
        if (m_vPanelTurnArea.size() > 0) {
            setTurnIcomPosition(m_vTurnIconPos, m_vPanelTurnArea.at(0)->getContentSize());
        }
        
        char strex[128] = {0};
        vector<ImageView*> vIcon;
        for (int i = 0; i < SHZ_COLUMN; i++) {
            vIcon.clear();
            for (int j = 0; j < LOTTERY_TYPE_COUNT; j++) {
                sprintf(strex, SHZ_LOGO_PATH, j);
                ImageView* icon = ImageView::create(strex);
                icon->setTag(j);
                m_vPanelTurnArea.at(i)->addChild(icon);
                icon->setPosition(m_vTurnIconPos.at(j));
                
                vIcon.push_back(icon);
            }
            m_vTurnIcon.push_back(vIcon);
        }
    }
    void GameTableUIV88::setTurnIcomPosition(vector<Vec2> &vIconPos, Size parentSize) {
        float fStartPosY = parentSize.height / 2 - ICON_HEIGHT;
        float posX = parentSize.width/2;
        for (int i = 0; i < LOTTERY_TYPE_COUNT; i++) {
            vIconPos.push_back(Vec2(posX, fStartPosY + i*ICON_HEIGHT));
        }
    }
    void GameTableUIV88::cleanResults()
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
    void GameTableUIV88::transformOpenLotteryType() {
        memset(nOpenLotteryTypeColRow, 0, sizeof(nOpenLotteryTypeColRow));
        for (int i = 0; i < SHZ_COLUMN; i++) {
            for (int j = 0; j < 3; j++) {
                int index = (3-j-1)*5+i;
                if (index >=0 && index < LOTTERY_COUNT) {
                    nOpenLotteryTypeColRow[i][j] = allLogoDatas[index];
                }
            }
        }
    }
    void GameTableUIV88::randomColumnDatasAndCalcTurnCount() {
        HNAudioEngine::getInstance()->playEffect(SHZ_ROLL_MUSIC);
        char str[128] = {0};
        if (m_vTurnIcon.size() > 0) {
            for (int i = 0; i < SHZ_COLUMN; i++) {
                int index = 0;
                for (int j = 0; j < LOTTERY_TYPE_COUNT; j++) {
                    memset(str, 0, sizeof(str));
                    auto img = m_vTurnIcon.at(i).at(j);
                    if (img) {
                        int imgTag = img->getTag();
                        if (imgTag == 0 || imgTag == 1 || imgTag == 2 || imgTag == 3) {
                            continue;
                        }
                        if (imgTag >= LOTTERY_TYPE_COUNT - 3) {
                            sprintf(str, SHZ_LOGO_PATH, nOpenLotteryTypeColRow[i][index++]);
                        }
                        else {
                            sprintf(str, SHZ_LOGO_PATH, rand() % LOTTERY_TYPE_COUNT);
                        }
                        img->loadTexture(str);
                    }
                }
            }
        }
        
        //转动数据
        m_nHasTurnUnitCount = 0;
        m_vTurnUnitCount.clear();
        //m_vTurnUnitCount = {10,18,26,34,42};
        for (int i = 0; i < SHZ_COLUMN; i++) {
            int nCount = (2 + i) * LOTTERY_TYPE_COUNT;
            nCount += (LOTTERY_TYPE_COUNT - 3);
            m_vTurnUnitCount.push_back(nCount);
        }
        //启动转盘
        m_nHasTurnUnitCount = 0;
        //schedule(schedule_selector(GameTableUIV88::turnUpdate), 0.05f);
        this->runAction(Sequence::create(CallFunc::create([=]() {
            for (auto layout : m_vPanelTurnArea) {
                if (layout) {
                    auto moveDown = MoveBy::create(0.15f, Vec2(0, -30));
                    layout->runAction(Sequence::create(moveDown, DelayTime::create(0.05f),  moveDown->reverse(), nullptr));
                }
            }
        }), DelayTime::create(0.35f), CallFunc::create([=]() {
            schedule(schedule_selector(GameTableUIV88::turnUpdate), 0.05f);
        }), nullptr));
    }
    void GameTableUIV88::turnUpdate(float dt) {
        //累计
        m_nHasTurnUnitCount++;
        
        for (int i = 0; i < SHZ_COLUMN; i++) {
            for (int j = 0; j < LOTTERY_TYPE_COUNT; j++) {
                if (m_nHasTurnUnitCount <= m_vTurnUnitCount.at(i)) {
                    auto img = m_vTurnIcon.at(i).at(j);
                    if (img) {
                        img->runAction(Sequence::create(MoveBy::create(0.1f, Vec2(0, -ICON_HEIGHT)), CallFunc::create([=]() {
                            //更新index
                            int nOldTag = m_vTurnIcon.at(i).at(j)->getTag();
                            int nNewTag = (nOldTag - 1) < 0 ? (LOTTERY_TYPE_COUNT - 1) : (nOldTag - 1);
                            m_vTurnIcon.at(i).at(j)->setTag(nNewTag);
                            
                            //一轮结束
                            if (j == LOTTERY_TYPE_COUNT - 1) {
                                for (int k = 0; k < LOTTERY_TYPE_COUNT; k++) {
                                    int curTag = m_vTurnIcon.at(i).at(k)->getTag();
                                    m_vTurnIcon.at(i).at(k)->setPosition(m_vTurnIconPos.at(curTag));
                                }
                            }
                        }), nullptr));
                    }
                    
                    if (m_nHasTurnUnitCount == m_vTurnUnitCount.at(i))
                    {
                        char str[128] = {0};
                        for (auto ii = 0; ii < 3; ii++)
                        {
                            memset(str, 0, sizeof(str));
                            sprintf(str, SHZ_LOGO_PATH, nOpenLotteryTypeColRow[i][ii]);
                            //                            auto img = m_vTurnIcon.at(i).at(ii);
                            auto tempTag = 1234567 + ii;
                            if (m_vTurnIcon.at(i).at(ii)->getParent()->getChildByTag(tempTag) == nullptr) {
                                //m_vTurnIcon.at(i).at(ii)->loadTexture(str);
                                //stopRotateAnimation(i);
                                auto img = ImageView::create(str);
                                m_vTurnIcon.at(i).at(ii)->getParent()->addChild(img);
                                img->loadTexture(str);
                                img->setTag(tempTag);
                                auto staticPos = m_vTurnIconPos.at(ii);
                                img->setPosition(staticPos);
                                img->runAction(Sequence::create(MoveTo::create(0.10f,Vec2(staticPos.x,staticPos.y - 30)),MoveTo::create(0.10f,staticPos),NULL));
                                
                                _resultImg.push_back(img);
                            }
                           
                        }
                        
                        for (auto ik = 0; ik < LOTTERY_TYPE_COUNT; ++ik)
                        {
                            m_vTurnIcon.at(i).at(ik)->setVisible(false);
                        }
                    }
                }
            }
        }
        
        //最后格子停下来后
        if (m_nHasTurnUnitCount == m_vTurnUnitCount.at(SHZ_COLUMN - 1))// && j == (LOTTERY_TYPE_COUNT_CSLL - 1))
        {
            unschedule(schedule_selector(GameTableUIV88::turnUpdate));
            
            /*runAction(Sequence::create(CallFunc::create([=](){
                showAllOpenLineVisble(true);
            }), DelayTime::create(1.0f),CallFunc::create([=](){
                showAllOpenLineVisble(false);
            }),NULL));*/
            delayShowOpenLogoEffect(0.1);
            showLogoAnimation(lineResultNumData, logoResultType);
            
            
            if (m_shzHasReward) {
                HNAudioEngine::getInstance()->playEffect(SHZ_LINE_MUSIC);//线的声音
                HNAudioEngine::getInstance()->playEffect(SHZ_WIN_END_MUSIC);
            }
            else
            {
                m_bAnimationFinished = true;
            }
            
        }
    }
	//////////////////////////////////////////////////////////////////////////
	void GameTableUIV88::onEnter() {
		HNGameUIBase::onEnter();
		HNAudioEngine::getInstance()->playBackgroundMusic(SHZ_SHZ_MUSIC, true);

		_fScaleX = WIN_SIZE.width / DESIGN_SIZE.width;
		_fScaleY = WIN_SIZE.height / DESIGN_SIZE.height;

		//触摸
		auto dispatcher = Director::getInstance()->getEventDispatcher();
		auto myListener = EventListenerTouchOneByOne::create();
		myListener->onTouchBegan = CC_CALLBACK_2(GameTableUIV88::onTouchBegan, this);
		myListener->setSwallowTouches(false);
		dispatcher->addEventListenerWithSceneGraphPriority(myListener, this);
        
        /*auto  m_loadUI = IGameLoadingUI::create(true,LOAD_SHZ_GAME);
        if (m_loadUI)
        {
            m_loadUI->loadRes();
            m_loadUI->onCloseCallBack = [=](){
                auto loadingAni = CSLoader::createNode(SHZ_LOADING_ANI_PATH);
                if (loadingAni) {
                    loadingAni->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2) + Vec2(0, -70));
                    this->addChild(loadingAni,100000);
                    auto action = CSLoader::createTimeline(SHZ_LOADING_ANI_PATH);
                    loadingAni->runAction(action);
                    action->play("water_loading", true);
                    loadingAni->runAction(Sequence::create(DelayTime::create(2.0f), RemoveSelf::create(), nullptr));
                }
                
                i64Reward = 0;
                isCanGuessB = false;
                isAutoGame = false;
                createGameSettlementLayer();
                createGameGuessSizeLayer();
                createGameMaryLayer();
                createRootLayer();
                createToolBarUI();
                
                addMoneyTimes = 1;
                
                loadTurn();
                //初始随机数据
                memset(allLogoDatas, -1, sizeof(allLogoDatas));
                 for (int i = 0; i < LOTTERY_COUNT; i++)
                 {
                 allLogoDatas[i] = 8;
                 Sprite* sp = allLogoSpriteVec.at(i);
                 char res[100];
                 memset(res, 0, sizeof(res));
                 sprintf(res, SHZ_LOGO_PATH, allLogoDatas[i]);
                 sp->setTexture(res);
                 }
                
                //showOpenAnimalAnimation(2, 2);
                //////////////////////////////
                /////////////////////
                _GameTableLogic->sendGameInfo();
                
                
                setNick(_GameTableLogic->getUserBySeatNo(_GameTableLogic->getMySeatNo())->nickName);
                
                runAction(Sequence::create(DelayTime::create(2.5f), CallFunc::create([&] () {
                    auto scanner = Scanner::create();
                    this->addChild(scanner, 100000);
                    scanner->starPlayerEffect();
                    scanner->setPosition(295, WIN_SIZE.height * 0.5f - 270);
                }), nullptr));
                
                scheduleUpdate();
                m_loadUI->setVisible(false);
            };
            addChild(m_loadUI);
        }*/
        
        auto loadingAni = CSLoader::createNode(SHZ_LOADING_ANI_PATH);
        if (loadingAni) {
            loadingAni->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2) + Vec2(0, -70));
            this->addChild(loadingAni,100000);
            auto action = CSLoader::createTimeline(SHZ_LOADING_ANI_PATH);
            loadingAni->runAction(action);
            action->play("water_loading", true);
            loadingAni->runAction(Sequence::create(DelayTime::create(2.0f), RemoveSelf::create(), nullptr));
        }
        
        
        
        i64Reward = 0;
        isCanGuessB = false;
        isAutoGame = false;
        createGameSettlementLayer();
        createGameGuessSizeLayer();
        createGameMaryLayer();
        createRootLayer();
        createToolBarUI();
        
        addMoneyTimes = 1;
        loadTurn();
        _GameTableLogic->sendGameInfo();
        setNick(_GameTableLogic->getUserBySeatNo(_GameTableLogic->getMySeatNo())->nickName);
        
        runAction(Sequence::create(DelayTime::create(2.5f), CallFunc::create([&] () {
            auto scanner = Scanner::create();
            this->addChild(scanner, 100000);
            scanner->starPlayerEffect();
            scanner->setPosition(295, WIN_SIZE.height * 0.5f - 270);
        }), nullptr));
        
        scheduleUpdate();
		
	}


	void GameTableUIV88::setNick(char* nick)
	{
		if (nick)
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			myName->setString(ForceGBKToUtf8(nick));
#else
			myName->setString(GBKToUtf8(nick));
#endif
		}
	}



	bool GameTableUIV88::onTouchBegan(Touch* touch, Event* event) {
		return true;
	}

	void GameTableUIV88::createToolBarUI() {
		int zOrder = ZOrder_Tool;
		auto toolbar = ToolBarLayer::create();
		if (toolbar) {
			toolbar->addImageAndCallBack(TOOL_BAR_EXIT, [=]() {
				if (!RoomLogic()->connected()) {
					leaveTheGame();
				}
				else {
					// 游戏不在空闲时期，不允许离开
					if (m_GameStatus != GS_FREE) {
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
			});
			toolbar->addImageAndCallBack(TOOL_BAR_SETTING, [=]() {
				IGameSet* gameSet = GameSetFactory::createWithVersion();
                gameSet->setBgPath("");
				gameSet->open(this, Vec2(0, 0), zOrder + 5, 0);
			});
            toolbar->addImageAndCallBack(TOOL_BAR_HELP, [=]() {
                auto helpLayer = HN::HelpLayer::create();
//                helpLayer->setBgImage("v66/common/help/shuihuz-help/bg.png");
                helpLayer->setContentOnView("v66/common/help/SHZ.png");
                helpLayer->setName("helpLayer");
                helpLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                helpLayer->setOrignalPosition();
                helpLayer->moveSelfInWithAction();
                addChild(helpLayer, zOrder + 5);
            });
			toolbar->create(this, Vec2(10, WIN_SIZE.height), zOrder, 100);
		}
	}
	void GameTableUIV88::leaveTheGame() {
		HNRoomLogic::getInstance()->closeRoom();
		PlatformState* state = PlatformState::create();
		state->setType(ROOMLIST);
		StateManager::getInstance()->setState(state);
	}

	void GameTableUIV88::onExit() {
		HNGameUIBase::onExit();
		if (m_audioEffectID > 0)
			HNAudioEngine::getInstance()->stopEffectWithID(m_audioEffectID);
        
        unscheduleUpdate();
        unschedule(schedule_selector(GameTableUIV88::turnUpdate));
	}

	void GameTableUIV88::onExitTransitionDidStart() {
		HNGameUIBase::onExitTransitionDidStart();
        HNAudioEngine::getInstance()->stopBackgroundMusic();

	}

	void GameTableUIV88::updatePlayerInfo() {

	}

	void GameTableUIV88::addUser(BYTE seatNo, bool bMe) {
		//UserInfoStruct * user = _GameTableLogic->getUserBySeatNo(seatNo);

		//m_vPlayerHead.at(viewSeatNo)->setVisible(true);
		//m_vPlayerHead.at(viewSeatNo)->setHead(user->bLogoID);
		//m_vPlayerHead.at(viewSeatNo)->setNick(user->nickName);
		//m_vPlayerHead.at(viewSeatNo)->setGold(user->i64Money);
	}

    void GameTableUIV88::showNotice(const std::string& message, int nCount, int nType, bool bVisible) {
		if (bVisible) {
			if (!m_Notice) {
				m_Notice = GameNotice::create();
				m_Notice->setLocalZOrder(NOTICE_ZORDER);
				m_Notice->setName("GameNotice");
			}
			m_Notice->openNotic(this, 10000, message, nCount, nType);
		}
	}

	//按钮回调
	////////////////////////////////////////////////////////////////////////////


	//开始按钮
	void GameTableUIV88::startButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) {
			return;
		}
        
		if (zongyazhuMoney > i64MyMoney)
		{
            auto prompt = GamePromptLayer::create();
            prompt->showPrompt("金币不足");
           
            if (isAutoGame) {
                autoGameButtonCallBack(cancelAutoBtn,Widget::TouchEventType::ENDED);
            }
			return;
		}
        
        if (i64MyMoney <= 0)
        {
            auto prompt = GamePromptLayer::create();
            prompt->showPrompt("金币不足");
            
            if (isAutoGame) {
                autoGameButtonCallBack(cancelAutoBtn,Widget::TouchEventType::ENDED);
            }
            return;
        }
        
        if (_GameTableLogic == nullptr || m_GameStatus != GS_FREE || !m_bAnimationFinished)
        {
            return;
        }
        
        setButtonEnabled(jiazhuBtn, false);
        setButtonEnabled(jianzhuBtn,false);
        setButtonEnabled(bibeiBtn, false);
        setButtonEnabled(startBtn, false);
        setButtonEnabled(zuidayafen, false);
        
        m_winImg->setVisible(false);
        m_winNum->setVisible(false);
		auto btn = (Button*)pSender;
		if (btn) {
            //cleanResults();
            //createAnimationRotate();
            m_bAnimationFinished = false;
			_GameTableLogic->sendStart(yazhuMoney);
		}
        if (scoreText) {
            scoreText->setString(StringUtils::format("%d",0));
        }
        topSpine->setAnimation(0,"dong",true);
		//HNAudioEngine::getInstance()->stopBackgroundMusic();
        setAllLineVisblefalse();
	}

	//开始按钮
	void GameTableUIV88::autoGameButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) {
			return;
		}
        isAutoGame = !isAutoGame;
        shoudongBtn->setVisible(!isAutoGame);
        cancelAutoBtn->setVisible(isAutoGame);
        
        if (isAutoGame) {
            startButtonCallBack(startBtn, Widget::TouchEventType::ENDED);
        }
	}

	//下注增加筹码减少筹码
	void GameTableUIV88::chipInButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) {
			return;
		}

		auto btn = (Button*)pSender;
        
        if (btn->getTag() == em_btnJianzhuTag) { //比倍
            _GameTableLogic->sendIsGuessSize(1);
            return;
        }
        else if (btn->getTag() == em_btnXiafenTag) //下分
        {
            _GameTableLogic->sendIsGuessSize(0);
            return;
        }
       
        
        if (btn->getTag() == em_btnJiazhuTag) {
            
            yazhuMoney += i64NoteUnit;
        }
        else if (btn->getTag() == em_btnsubNoteTag)
        {
            
            yazhuMoney -= i64NoteUnit;
        }
        else if (btn->getTag() == em_btnZuidayafenTag)//最大押分
        {
            yazhuMoney = i64NoteMax;
        }
        
        
		
		//大于最大值
		if (yazhuMoney > i64NoteMax)
        {
            
            yazhuMoney = i64NoteMin;
		}
        if (yazhuMoney < i64NoteMin) {
            yazhuMoney = i64NoteMax;
        }
       
		zongyazhuMoney = yazhuMoney * 9;
		updateMoneyState();
	}


	//////////////////////////////////////////////////////////////////////////
	//动画

	//玩家坐下(非自己)
	void GameTableUIV88::userSitDown(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
		addUser(userSit->bDeskStation, userSit->bDeskStation == _GameTableLogic->getMySeatNo());
	}

	//玩家站起
	void GameTableUIV88::userUp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
		if (m_GameStatus == GS_FREE) {

		}
	}
	//
	//void GameTableUIV88::onGameNoteFail(GameNoteFailedMsg *msg) {
	//	if (msg == nullptr) {
	//		return;
	//	}
	//	switch (msg->bFailCode)
	//	{
	//	case 0:
	//		gameStateText->setString(GBKToUtf8("金币不足！"));
	//		HNAudioEngine::getInstance()->playEffect(SHUIGUOJI_BUTTON_WRONG);
	//		break;
	//	case 1:
	//		gameStateText->setString(GBKToUtf8("下注超过限额！"));
	//		HNAudioEngine::getInstance()->playEffect(SHUIGUOJI_BUTTON_WRONG);
	//		break;
	//	default:
	//		break;
	//	}
	//	//this->scheduleOnce(schedule_selector(GameTableUIV88::hideGameStateText), 1.5f);
	//}


	//第一次进入游戏基础数据同步d
	void GameTableUIV88::syncBasicDataFirstTime(struct_s2c_base *msg) {
		if (msg == nullptr) {
			return;
		}		
		i64MyMoney = msg->i64Mymoney;
		i64NoteUnit = msg->i64NoteUnit;
		i64NoteMin = msg->i64NoteMin;
		i64NoteMax = msg->i64NoteMax;
		//memset(nLotteryRateArr, 0, sizeof(nLotteryRateArr));
	/*	for (int i = 0; i < LINE_COUNT; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				nLotteryRateArr[i][j] = msg->nLotteryRateArr[i][j];
			}
		}*/
		yazhuMoney = i64NoteUnit;
		zongyazhuMoney = yazhuMoney * 9;
		updateMoneyState();
	}

	void GameTableUIV88::updateMoneyState()
	{
//        string myMoney = StringUtils::format("%lld", i64MyMoney);
        myLastMoneyText->setString(Tools::addComma(i64MyMoney));
		char str[100];
		sprintf(str, "%lld", yazhuMoney);
		yazhuText->setString(str);
		char str1[100];
		sprintf(str1, "%lld", zongyazhuMoney);
		zongyazhuText->setString(str1);
	}

	//更新游戏状态
	void GameTableUIV88::updateGameState(struct_game_state* msg)
	{
		if (msg == nullptr) {
			return;
		}
		m_GameStatus = msg->bGameState;
		if (isAutoGame && m_GameStatus == GS_FREE)
		{
			/*if (zongyazhuMoney > i64MyMoney || i64MyMoney <= 0)
			{
				return;
			}
            
            setButtonEnabled(jiazhuBtn, false);
            setButtonEnabled(bibeiBtn, false);
            setButtonEnabled(startBtn, false);
			_GameTableLogic->sendStart(yazhuMoney);*/
		}
	}

	//开奖的结果所有的数据
	void GameTableUIV88::openLineResultData(struct_s2c_startGame* msg)
	{
		if (msg == nullptr) {
			return;
		}
        m_titleImg->ignoreContentAdaptWithSize(false);
        m_titleImg->loadTexture("Games/31000503/ui/zi1.png");
        memset(allLogoDatas, 0, sizeof(allLogoDatas));
        memcpy(allLogoDatas, msg->nOpenLotteryType, sizeof(msg->nOpenLotteryType));
        //showStartGameEffect();
		i64MyMoney = msg->i64MyMoney;
		//线一闪一闪的效果
		countLine = 0;
		int count = 0;
        m_shzHasReward = false;
		memset(lineResultNumData, -1, sizeof(lineResultNumData));
		memset(tempLineResultData, -1, sizeof(tempLineResultData));
		memset(logoResultType, -1, sizeof(logoResultType));
		memset(logoResultRate, 0, sizeof(logoResultRate));
		memset(logoOpenNumCount, 0, sizeof(logoOpenNumCount));
		//
		for (int i = 0; i < LINE_COUNT; i++)
		{
			if (msg->nOpenLineResult[i] != 0)
			{
				lineResultData[count] = i;
				tempLineResultData[count] = i;
				count++;
                m_shzHasReward = true;
				
			}
			lineResultNumData[i] = msg->nOpenLineResult[i];
			logoResultRate[i] = msg->nOpenLineRate[i];
			logoResultType[i] = msg->nOpenLineType[i];
			logoOpenNumCount[i] = msg->nOpenLineCount[i];
			
		}
		showEffectTime = msg->nOpenLineTime;
        
        cleanResults();
        transformOpenLotteryType();
        randomColumnDatasAndCalcTurnCount();
		/*if (count > 0)
		{
//            this->runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create([this] {
//                showAllOpenLineVisble(true);
//            }),DelayTime::create(0.2f),CallFunc::create([this] {
//                showAllOpenLineVisble(false);
//            }), nullptr));
			schedule(schedule_selector(GameTableUIV88::delayShowOpenLogoEffect), 0.2f, 1, 3.0f);

			this->runAction(Sequence::create(DelayTime::create(msg->nOpenRoundTime + showEffectTime*count), CallFunc::create([this] {
				showLogoAnimation(lineResultNumData, logoResultType);
				HNAudioEngine::getInstance()->playEffect(SHZ_WIN_END_MUSIC);//
			}), DelayTime::create(1.5f),
                CallFunc::create([this]{
                    for (int i = 0; i < LOTTERY_COUNT; i++)
                    {
                        Sprite* sp = allLogoSpriteVec.at(i);
                        char res[100];
                        memset(res, 0, sizeof(res));
                        sprintf(res, SHZ_LOGO_PATH, allLogoDatas[i]);
                        if (sp)
                        {
                            sp->setTexture(res);
                        }
                    }
                
                
              
				}), nullptr));
		}*/

		updateMoneyState();
	}

    //有开中小玛丽
    void GameTableUIV88::openHasMaryTip(struct_s2c_hasMary *msg) {
        if (msg == nullptr) {
            return;
        }
        if (msg->maryTimes > 0) {
            setAllLineVisblefalse();
            if (_gameMaryLayer) {
                _gameMaryLayer->updateBasicData(msg);
            }
            //播放动画
            this->runAction(Sequence::create(CallFunc::create([&]() {
                playDragonAnimation();
            }), DelayTime::create(1.5f), CallFunc::create([&]() {
                playFlagAnimation();
            }), CallFunc::create([&]() {
                //转到小玛丽界面
                if (_gameMaryLayer) {
                    _gameMaryLayer->showMaryLayer();
                }
            }), nullptr));
        }
    }
    
    //更新小玛丽数据
    void GameTableUIV88::updateMaryData(struct_s2c_maryData *msg) {
        if (msg == nullptr) {
            return;
        }
        //update UI
        if (_gameMaryLayer) {
            _gameMaryLayer->updateData(msg);
        }
    }
    
    //小玛丽结束
    void GameTableUIV88::maryFinishTip(SHZ::struct_s2c_maryFinish *msg) {
        if (msg == nullptr) {
            return;
        }
        //
        if (_gameMaryLayer) {
            _gameMaryLayer->hideMaryLayer();
        }
    }
    
	void GameTableUIV88::delayShowOpenLogoEffect(float time)
	{
//        showLineEffect(tempLineResultData[countLine]);
        runAction(Sequence::create(CallFunc::create([this]{
            showAllOpenLineVisble(true);
            
        }), DelayTime::create(0.2f), CallFunc::create([this]{
            //showAllOpenLineVisble(false);
        }), nullptr));
        
		countLine++;
	}

	//一闪一闪的效果
	void GameTableUIV88::showLineEffect(int lineNum)
	{
		showEffectLine = allLineSpriteVec.at(lineNum);
		Sequence* seq = Sequence::create(DelayTime::create(0.23f*showEffectTime), CallFunc::create([this] {
			showEffectLine->setVisible(true);
			HNAudioEngine::getInstance()->playEffect(SHZ_LINE_MUSIC);//线的声音
		}), DelayTime::create(0.23f*showEffectTime), CallFunc::create([this] {
//            showEffectLine->setVisible(false);
//        }), DelayTime::create(0.23f*showEffectTime), CallFunc::create([this] {
//            showEffectLine->setVisible(true);
//        }), DelayTime::create(0.23f*showEffectTime), CallFunc::create([this] {
			showEffectLine->setVisible(false);
		}), nullptr);
		this->runAction(seq);
	}

	void GameTableUIV88::showLogoAnimation(int lineNum[9], int logoTye[9])
	{
        
        
        
        
		int type = 0;
		int logoIndex;
		int nextIndex;
		int beforeIndex;
		int playEffectArr[15];
		memset(playEffectArr, -1, sizeof(playEffectArr));
		int countNum = 0;
		for (int i = 0; i < LINE_COUNT; i++)
		{
			if (lineNum[i] != 0)
			{
				type = logoTye[i];
				nextIndex = 0;
				beforeIndex = -1;
				for (int j = 0; j < 5; j++)
				{
					logoIndex = linesIndexs[i][j];
					if (j == 4)
					{
						nextIndex   = linesIndexs[i][3];
                        beforeIndex = -1;
					}
					else
					{
						nextIndex = linesIndexs[i][j+1];

						if (j>0)
						{
							beforeIndex = linesIndexs[i][j - 1];
						}
					}
                    
                    bool equal = false;
                    if (beforeIndex != -1)
                    {
                        equal = (isEqualLogoType(type, allLogoDatas[logoIndex]) && isEqualLogoType(type, allLogoDatas[beforeIndex]));
                       
                    }
                    if ((isEqualLogoType(type, allLogoDatas[logoIndex]) && isEqualLogoType(type, allLogoDatas[nextIndex])) || equal)
                    {
                        
                        if (allLogoDatas[logoIndex] == 8)
                        { //播放水浒传的特效
                            showOpenAnimalAnimation(allLogoDatas[logoIndex], logoIndex);
                        }
                        else
                        {
                            showOpenAnimalAnimation(type, logoIndex);
                        }
                        
                        bool ishas = false;
                        for (int k = 0; k < LOTTERY_COUNT; k++)
                        {
                            if (playEffectArr[k] == logoIndex)
                            {
                                ishas = true;
                            }
                        }
                        
                        if (!ishas)
                        {
                            playEffectArr[countNum] = logoIndex;
                            countNum++;
                        }
                    }
					
				}
			}
            
            if (m_shzHasReward) {
                for (int i = 0; i < LOTTERY_COUNT; i++)
                {
                    
                    int idxmap[] = {2,5,8,11,14,1,4,7,10,13,0,3,6,9,12};
                    ImageView* sp = _resultImg.at(idxmap[i]);
                    if (sp->getChildByTag(888888) == nullptr) {
                        char res[100];
                        memset(res, 0, sizeof(res));
                        sprintf(res, SHZ_LOGO_G_PATH, allLogoDatas[i]);
                        if (sp)
                        {
                            sp->loadTexture(res);
                        }
                    }
                    
                    
                }
            }
			
		}
        
        
        
	}

	bool GameTableUIV88::isEqualLogoType(int type,int type2)
	{
		if (type2 == 8)
		{
			return true;
		}
		if (type == type2)
		{
			return true;
		}
		return false;
	}
    
    void GameTableUIV88::playDragonAnimation() {
        auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 150));
        addChild(colorLayer, ZOrder_SettlementLayer);
        colorLayer->runAction(Sequence::create(DelayTime::create(1.5f), RemoveSelf::create(), nullptr));

        auto node = CSLoader::createNode("v88/animation/watermargin/bonusgame/bonusgame_dragon/bonusgame_dragon.csb");
        if (node) {
            node->setPosition(WIN_SIZE/2);
            addChild(node, ZOrder_SettlementLayer+1);
            auto action = CSLoader::createTimeline("v88/animation/watermargin/bonusgame/bonusgame_dragon/bonusgame_dragon.csb");
            if (action) {
                node->runAction(action);
                action->play("bonusgame_dragon", false);
            }
        }
    }
    
    void GameTableUIV88::playFlagAnimation() {
        auto node = CSLoader::createNode("v88/animation/watermargin/bonusgame/bonusgame_flag/bonusgame_flag.csb");
        if (node) {
            node->setPosition(WIN_SIZE/2);
            addChild(node, ZOrder_SettlementLayer+1);
            auto action = CSLoader::createTimeline("v88/animation/watermargin/bonusgame/bonusgame_flag/bonusgame_flag.csb");
            if (action) {
                node->runAction(action);
                action->play("bonusgame_flag", false);
            }
        }
    }
    
	void GameTableUIV88::showAllOpenLineVisble(bool isShow)
	{
		for (int i = 0; i < LINE_COUNT; i++)
		{
			int linenum = tempLineResultData[i];
			if (linenum != -1)
			{
				showEffectLine = allLineSpriteVec.at(linenum);
				showEffectLine->setVisible(isShow);
			}
		}
	}

	void GameTableUIV88::setAllLineVisblefalse()
	{
		for (int i = 0; i < LINE_COUNT; i++)
		{
			showEffectLine = allLineSpriteVec.at(i);
			showEffectLine->setVisible(false);
		}
	}

    //通用播放开奖图标的动画
	void GameTableUIV88::playLogoAnimation(std::string res, std::string tagName, int logoIndex) {
        
        int idxmap[] = {2,5,8,11,14,1,4,7,10,13,0,3,6,9,12};
        auto mFind = idxmap[logoIndex];
        auto img = _resultImg.at(mFind);
        if (img) {
            Node* animation = img->getChildByTag(888888);
            if (animation == nullptr)
            {
                animation = CSLoader::createNode(res);
                animation->setTag(888888);
                img->addChild(animation);
                
                resultAnimationNode = animation;
                if (resultAnimationNode) {
                    //Vec2 pos = img->getPosition();
                    resultAnimationNode->setPosition(img->getContentSize()/2);
                    
                    auto action = CSLoader::createTimeline(res);
                    resultAnimationNode->runAction(action);
                    action->play(tagName, true);
                    //action->setAnimationEndCallFunc(tagName, [=](){
                    //animation->setVisible(false);
                    //});
                }
            }
            
            //animation->setVisible(true);
           
        }
        
      
	}

	void GameTableUIV88::showOpenAnimalAnimation(BYTE index,int logoIndex) {
		switch (index)
		{

		case 0:
			playLogoAnimation(ANIMATION_REWARD_LUZHISHEN_CSB, "luzhishen", logoIndex);
			break;
		case 1:
			playLogoAnimation(ANIMATION_REWARD_LINCHONG_CSB, "linchong", logoIndex);
			break;
		case 2:
			playLogoAnimation(ANIMATION_REWARD_SONGJIANG_CSB, "songjiang", logoIndex);
			break;
		case 3:
			playLogoAnimation(ANIMATION_REWARD_AXE_CSB, "axe", logoIndex);
			break;
		case 4:
			playLogoAnimation(ANIMATION_REWARD_HALBERD_CSB, "halberd", logoIndex);
			break;
		case 5:
			playLogoAnimation(ANIMATION_REWARD_BROADSWORD_CSB, "broadsword", logoIndex);
			break;
		case 6:
			playLogoAnimation(ANIMATION_REWARD_ZHONGYI_CSB, "zhongyi", logoIndex);
			break;
		case 7:
			playLogoAnimation(ANIMATION_REWARD_FLAG_CSB, "flag", logoIndex);
			break;
		case 8:
			playLogoAnimation(ANIMATION_REWARD_WATERMARGIN_CSB, "watermargin", logoIndex);
			break;
		default:
			break;
		}

	}

	void GameTableUIV88::showStartGameEffect()
	{
        HNAudioEngine::getInstance()->playEffect(SHZ_ROLL_MUSIC);//滚动的声音
        float baseTime = 0.2f;      // 动画翻牌速度
        float delayTime[7] = {0};
        for (int i = 0; i < 7; i++) {
            delayTime[i] = 0.7f + baseTime * i;
        }
		for (int i = 0; i < LOTTERY_COUNT; i++)
		{
            float realTime = 1.0f;
            float tmpTime = 0.05f;
            if (i == 0) {
                realTime = delayTime[0];
                realTime = realTime + (i/5)*tmpTime;
            }
            else if (i == 1 || i == 5) {
                realTime = delayTime[1];
                realTime = realTime + (i/5)*tmpTime;
            }
            else if (i == 2 || i == 6 || i == 10) {
                realTime = delayTime[2];
                realTime = realTime + (i/5)*tmpTime;
            }
            else if (i == 3 || i == 7 || i == 11) {
                realTime = delayTime[3];
                realTime = realTime + (i/5)*tmpTime;
            }
            else if (i == 4 || i == 8 || i == 12) {
                realTime = delayTime[4];
                realTime = realTime + (i/5)*tmpTime;
            }
            else if (i == 9 || i == 13) {
                realTime = delayTime[5];
                realTime = realTime + (i/10)*tmpTime;
            }
            else if (i == 14) {
                realTime = delayTime[6];
                realTime = realTime + (i/15)*tmpTime;
            }
			auto node = CSLoader::createNode(ANIMATION_WATER_SCROLLBAR_CSB);
			if (node) {
                Sprite* sp = allLogoSpriteVec.at(i);
                if (sp) {
                    sp->setVisible(false);
                }
				Vec2 pos = allLogoSpriteVec.at(i)->getPosition();
				node->setPosition(pos);
				root->addChild(node, -1);
				auto action = CSLoader::createTimeline(ANIMATION_WATER_SCROLLBAR_CSB);
				node->runAction(action);
				int rand1 = rand() % 10;
				action->gotoFrameAndPlay(rand1);
				action->play("water_scrollbar", true);
				node->setScale(0.91f, 0.89f);
				node->runAction(Sequence::create(DelayTime::create(realTime),
                                                 CallFunc::create([=]() {
                    if (node) {
                        node->setVisible(false);
                    }
                    Sprite* sp = allLogoSpriteVec.at(i);
                    char res[100];
                    memset(res, 0, sizeof(res));
                    sprintf(res, SHZ_LOGO_PATH, allLogoDatas[i]);
                    if (sp) {
                        sp->setVisible(true);
                        sp->setTexture(res);
                    }
                    
                    if (i == LOTTERY_COUNT - 1) {
                        topSpine->setToSetupPose();
                        topSpine->setAnimation(0,"jing",true);
                        m_titleImg->ignoreContentAdaptWithSize(false);
                        m_titleImg->loadTexture("Games/31000503/ui/zi2.png");
                    }
                    
                }), RemoveSelf::create(), nullptr));
			}
		}
	}

	void GameTableUIV88::createGameSettlementLayer() {
		_gameSettlement = nullptr;
		SHZSettlementV88* settle = SHZSettlementV88::create();
		if (settle) {
			_gameSettlement = settle;
			_gameSettlement->setLogic(_GameTableLogic);
			_gameSettlement->setSettlementLayerPosition(WIN_SIZE / 2);
			_gameSettlement->hideSettlementLayer();
			/*	int logoType[9] = { 0, 1, -1, -1, -1, -1, -1, -1, -1 };
				int rate[9] = { 7, 160, -1, -1, -1, -1, -1, -1, -1 };
				_gameSettlement->setSettlementData(logoType, rate);*/
			addChild(_gameSettlement, ZOrder_SettlementLayer);
		}
	}

	void GameTableUIV88::createGameGuessSizeLayer() {
		_gameGuessSizeLayer = nullptr;
		SHZGuessSizeV88* settle = SHZGuessSizeV88::create();
		if (settle) {
			_gameGuessSizeLayer = settle;
			_gameGuessSizeLayer->setLogic(_GameTableLogic);
			_gameGuessSizeLayer->setGuessSiseLayerPosition(WIN_SIZE / 2);
			_gameGuessSizeLayer->hideGueesSizeLayer();
			addChild(_gameGuessSizeLayer, ZOrder_SettlementLayer);
		}
	}

    void GameTableUIV88::createGameMaryLayer() {
        _gameMaryLayer = nullptr;
        SHZMaryTableUIV88* maryLayer = SHZMaryTableUIV88::create();
        if (maryLayer) {
            _gameMaryLayer = maryLayer;
            _gameMaryLayer->setLogic(_GameTableLogic);
            _gameMaryLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            _gameMaryLayer->setPosition(WIN_SIZE/2);
            _gameMaryLayer->hideMaryLayer();
            addChild(_gameMaryLayer, ZOrder_SettlementLayer);
        }
    }

    //开奖出拿条线是否有压中
	void GameTableUIV88::openLineResultIsCanGuess(struct_s2c_caiDaXiao* msg)
	{
		if (msg == nullptr) {
			return;
		}
		if (msg->nFlag)
		{
            setButtonEnabled(bibeiBtn,true);
            xiafenBtn->setVisible(true);
            setButtonEnabled(shoudongBtn,false);
            
            
            m_defenTip->setVisible(true);
            m_bibeiTip->setVisible(true);
            
            m_defenTip->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=](){
                m_bibeiTip->setVisible(true);
                m_defenTip->setVisible(false);
            }),DelayTime::create(1.0f),CallFunc::create([=](){
                m_bibeiTip->setVisible(false);
                m_defenTip->setVisible(true);
            }),DelayTime::create(1.0f),NULL)));
            
            
		}
		/*for (int i = 0; i < LOTTERY_COUNT; i++)
		{
			Sprite* sp = allLogoSpriteVec.at(i);
			char res[100];
			memset(res, 0, sizeof(res));
			sprintf(res, SHZ_LOGO_PATH, allLogoDatas[i]);
			if (sp)
			{
				sp->setTexture(res);
			}
		}*/
		setAllLineVisblefalse();
		this->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([this] {

			if (isAutoGame)
			{
				//if (zongyazhuMoney > i64MyMoney)
				//{
					//return;
				//}
				_GameTableLogic->sendIsGuessSize(0);
				_gameSettlement->hideSettlementLayer();	
			
			}
		
		}), nullptr));

	}

	//猜大小的结果
	void GameTableUIV88::guessSizeResult(struct_s2c_caiResult* msg)
	{
		if (msg == nullptr) {
			return;
		}
		_gameGuessSizeLayer->updateGessSizeDate(msg);
    }

	//开始猜大小
	void GameTableUIV88::gueessSizeStar(struct_s2c_doCaiDaXiao* msg)
	{
		if (msg == nullptr) {
			return;
		}
		if (msg->nFlag)
		{
			_gameGuessSizeLayer->setGuessSizeData(msg->i64WinMoney);
			_gameGuessSizeLayer->playStandByEffect();
		}
}

	//猜奖结束
	void GameTableUIV88::guessSizeOver(struct_s2c_caiFinish* msg)
	{
		if (msg == nullptr) {
			return;
		}
        //
        i64MyMoney = msg->i64MyMoney;
        //if (msg->i64WinMoney > 0) {
            
           /* int n_bigWinNum        = Tools::getLianxianGameData(msg->i64WinMoney,yazhuMoney,25,70);
            if (n_bigWinNum == 0) {
                    
                showMoneyFunc(msg->i64WinMoney);
            }
            else if (n_bigWinNum == 1 or n_bigWinNum == 2)
            {
                    
                auto bigWinLayer = BigWinnerLayer::create();
                if (bigWinLayer) {
                    bigWinLayer->open(this,Vec2::ZERO,200,2,n_bigWinNum);
                }
                runAction(Sequence::create(DelayTime::create(3.0f),CallFunc::create([=](){
                        showMoneyFunc(msg->i64WinMoney);
                        
                   }),NULL));
            }*/
                
            
       // }
       // else
       // {
            if (msg->i64WinMoney > 0) {
                m_winImg->setVisible(true);
                m_winNum->setVisible(true);
                m_winNum->setString(StringUtils::format("%lld",msg->i64WinMoney));
                
                if (scoreText) {
                    scoreText->setString(StringUtils::format("%lld",msg->i64WinMoney));
                }
            }
        
            m_defenTip->stopAllActions();
            m_defenTip->setVisible(false);
            m_bibeiTip->setVisible(false);
        
        
        
            setButtonEnabled(jiazhuBtn, true);
            setButtonEnabled(jianzhuBtn, true);
            setButtonEnabled(zuidayafen,true);
            setButtonEnabled(bibeiBtn, false);
            setButtonEnabled(startBtn, true);
            setButtonEnabled(shoudongBtn, true);
            xiafenBtn->setVisible(false);
        
            runAction(Sequence::create(DelayTime::create(1.0f),CallFunc::create([=](){
               m_bAnimationFinished = true;
            }),NULL));
        
        
            _gameGuessSizeLayer->hideGueesSizeLayer();
            updateMoneyState();
            
            setAllLineVisblefalse();
      //  }
        
       


	}
    void GameTableUIV88::showMoneyFunc(long long winMoney)
    {
        if (scoreText) {
            scoreText->setString(StringUtils::format("%lld",winMoney));
        }
        long long n_multipleNum = winMoney/yazhuMoney;
        int m_level = 0;
        if (n_multipleNum >= 3 && n_multipleNum < 60) {
            m_level = 1;
        }
        else if (n_multipleNum >=60 && n_multipleNum < 100 )
        {
            m_level = 2;
        }
        else if (n_multipleNum >= 100 && n_multipleNum < 120)
        {
            m_level = 3;
        }
        else if (n_multipleNum >= 120 && n_multipleNum < 150)
        {
            m_level = 4;
        }
        else if (n_multipleNum >= 150 && n_multipleNum < 500)
        {
            m_level = 5;
        }
        else if (n_multipleNum > 500)
        {
            m_level = 6;
        }
        auto m_Layer = MoneyGetFourLevel::create();
        if (m_Layer) {
            m_Layer->open(this,Vec2::ZERO,200,m_level);
        }
        runAction(Sequence::create(DelayTime::create(2.0f),CallFunc::create([=](){
            auto coin = FloatCoin::create();
            if (coin) {
                coin->openParent(this, Vec2(WIN_SIZE.width*0.7f, WIN_SIZE.height-120), ZOrder_SettlementLayer);
                coin->updateCoin(winMoney);
            }
            setButtonEnabled(jiazhuBtn, true);
            setButtonEnabled(jianzhuBtn, true);
            setButtonEnabled(zuidayafen, true);
            setButtonEnabled(bibeiBtn, false);
            setButtonEnabled(startBtn, true);
            setButtonEnabled(shoudongBtn, true);
            xiafenBtn->setVisible(false);
            
            
            _gameGuessSizeLayer->hideGueesSizeLayer();
            updateMoneyState();
            
            setAllLineVisblefalse();
            
        }),NULL));
        
    }
	void GameTableUIV88::onGameTipExit(tag_s2c_tip_exit* msg) {
		if (msg == nullptr) {
			return;
		}
		tipMsg = msg;
		//
		//不退出
		if (msg->bIsExit == 0) {
			return;
		}
		//要退出
		this->runAction(Sequence::createWithTwoActions(DelayTime::create(2.0f), CallFunc::create([&]() {
			tipCallback();
		})));
	}



	void GameTableUIV88::tipCallback() {
		if (tipMsg->bIsExit == 1) {
			//准备关闭掉
			//m_PrepareBtn->setVisible(false);

			auto layer = GamePromptLayer::create();
            layer->setOkayCallBack([this]() {
                unschedule(schedule_selector(GameTableUIV88::updateTip));
                _GameTableLogic->sendUserUp();
            });
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

		/*	layer->setCallBack([&]() {
				unschedule(schedule_selector(GameTableUIV88::updateTip));
				_GameTableLogic->sendUserUp();
			});*/
			layer->setColorLayerTouchEnabled(false);

			m_i64TipMoney = tipMsg->i64Money;
			m_TextTip = layer->getPromptText();
			m_TextTip->setTag(sec);
			schedule(schedule_selector(GameTableUIV88::updateTip), 1.0f);
		}
		else {

		}
	}

	void GameTableUIV88::updateTip(float dt) {
		int iTag = m_TextTip->getTag();
		if (iTag == 0) {
			unschedule(schedule_selector(GameTableUIV88::updateTip));
			_GameTableLogic->sendUserUp();
			return;
		}

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




	//
	//void GameTableUIV88::onHandleNewMessage(int iType, const std::string &message, int iCount)
	//{
	//	//全服公告
	//	if (iType == 0)
	//	{
	//		showNotice(message, 100, iType);
	//	}
	//	else if (iType == -11)//银商
	//	{
	//		showNotice(message, iCount, iType);
	//	}
	//}

	void GameTableUIV88::initData() {
		//m_vGameStatePosition.clear();
	}

	void GameTableUIV88::resetNewGame() {
		m_GameStatus = GS_FREE;

		//
		m_iPrevHasNote = 0;	//重置上局下注信息，等待服务端重新通知


		//resetAllNotesAnimationAndClean();
		/*if (_gameSettlement) {
			_gameSettlement->hideSettlementLayer();
		}*/
		//animationStopCarLight();
	}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    

	void GameTableUIV88::resetUINewGame() {
		
		//resetAllNotesAnimationAndClean();
	}

	void GameTableUIV88::resetData() {
		//ui
		_fScaleX = 0;
		_fScaleY = 0;
		//_gameSettlement = nullptr;

//        m_clockObj = nullptr;
//        m_clockPos = Vec2(-247.8, 214);
		gameStateText = nullptr;
		// games
		m_Notice = nullptr;

		m_fTimeCount = 0;

		m_notesPosition.clear();
		m_betAreaPosition.clear();
		m_betLabelPos.clear();

		//resetAllNotesAnimationAndClean();
        
        m_vPanelTurnArea.clear();
        m_vTurnIcon.clear();
        m_vTurnIconPos.clear();
        m_vTurnUnitCount.clear();           //转多少个icon单元
        m_nHasTurnUnitCount = 0;        //已经转的单元

        _resultImg.clear();
        m_bAnimationFinished = true;
	}
   
    //Tools
	void GameTableUIV88::setButtonEnabled(ui::Button* pButton, bool enabled) {
		if (pButton != nullptr) {
//            GLubyte opacity = enabled ? 255 : 125;
			pButton->setTouchEnabled(enabled);
            pButton->setBright(enabled);
//            pButton->setOpacity(opacity);
            
           /* if (pButton->getTag() == em_btnJianzhuTag) { //比倍加特效
                if (enabled) {
                    Tools::addAnimationFrame("v88/animation/watermargin/bibei_button/bibei_button.csb", bibeiBtn, "bibei_button", Vec2(0, bibeiBtn->getContentSize().height*0.1f),1.20f);
                }
                else
                {
                    auto effect = pButton->getChildByName("bibei_button");
                    if (effect) {
                        pButton->stopAllActions();
                        effect->removeFromParent();
                    }
                }
                
               
            }*/
            
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
            i64MyMoney = msg->i64MyMoney;
            runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
                txtReward->setString(Tools::addComma(luckyMoney));
                if (nullptr != myLastMoneyText)
                {
                    myLastMoneyText->setString(Tools::addComma(i64MyMoney));
                }
                
            }), DelayTime::create(retainTime), CallFunc::create([&](){
                this->removeChildByName("rewardResult");
            }), NULL));
        }
    }
    void GameTableUIV88::update(float dt)
    {
        // 自动
        if (isAutoGame && m_GameStatus == GS_FREE)
        {
            startButtonCallBack(startBtn, Widget::TouchEventType::ENDED);
        }
    }
    void GameTableUIV88::createAnimationRotate()
    {
        m_AnimationNodeVec.clear();
        for (int i = 0; i < SHZ_COLUMN; i++) {
            
            int m_animationType = random(1,2);
            int startIdx = 0;
            int endIdx   = 9;
            
            std::string animationName = StringUtils::format("SHZ_ICON_ROTATE_%d",m_animationType);
            Animation * anima = AnimationCache::getInstance()->getAnimation(animationName);
            if (anima == nullptr) {
                anima = Animation::create();
                anima->setDelayPerUnit(0.10f);
                anima->setLoops(-1);
                
                for (int j = startIdx; j <= endIdx; j++) {
                    std::string str = StringUtils::format("shz%d_0000%d.png",m_animationType,j);
                    auto frames = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
                    if (frames) {
                        anima->addSpriteFrame(frames);
                    }
                    
                }
                
                AnimationCache::getInstance()->addAnimation(anima, animationName);
            }
            
            auto animate = Animate::create(anima);
            auto animationSprite = Sprite::create();
            animationSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            //animationSprite->runAction(animate);
            
            animationSprite->setPosition(m_vPanelTurnArea.at(i)->getContentSize()/2);
            m_vPanelTurnArea.at(i)->addChild(animationSprite);
            m_AnimationNodeVec.push_back(animationSprite);
            
            animationSprite->runAction(Sequence::create(DelayTime::create(0.1*i),CallFunc::create([=](){
                for (auto ik = 0; ik < LOTTERY_TYPE_COUNT; ++ik)
                {
                    m_vTurnIcon.at(i).at(ik)->setVisible(false);
                }
                
            }),animate,NULL));
            
            
        }
    }
    void GameTableUIV88::stopRotateAnimation(int nCol)
    {
        if (nCol >= 0 && nCol < m_AnimationNodeVec.size()) {
            auto animationSprite = m_AnimationNodeVec.at(nCol);
            if (animationSprite) {
                animationSprite->removeFromParent();
                m_AnimationNodeVec.at(nCol) = nullptr;
            }
        }
        
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
}
