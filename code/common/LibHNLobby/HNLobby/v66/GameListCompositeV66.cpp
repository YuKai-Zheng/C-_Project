#include "GameListCompositeV66.h"
#include "UI/LoadingLayer.h"

#include "cocos2d.h"
#include "HNLobby/GameRoom.h"
#include "HNLobby/GamePrompt.h"
#include "../pattern/CommandManager.h"
#include "PlatformResV66.h"
#include "../PlatformDefine.h"
#include "Sound/HNAudioEngine.h"
#include "../GameData.h"
#include "../command/ShowGamePrompt.h"
#include "../pattern/factory/gameRank/GameRankFactory.h"
#include "UI/JackpotModel.hpp"
#include "spine/spine.h"
#include <spine/spine-cocos2dx.h>
#include "external/unzip/unzip.h"

USING_NS_CC;


#define BUFFER_SIZE    8192
#define MAX_FILENAME   512

//item vec(202,232)
//list width = 4*202 + 3*20 height = 2*232 + 20 + 2*3

//const char* PLATFORM_LEFT_GIRL = "v88/animation/main/girl/girl.csb";
//const char* PLATFORM_LEFT_HOTGAME = "v88/animation/main/zhutui/zhutui.csb";

const char* PAGE_TAG_ICON_CURRENT	= "hallimg/main/17.png";
const char* PAGE_TAG_ICON_OTHER		= "hallimg/main/16.png";

const char* GAME_DOWNLOAD_BTN       = "hallimg/main/icon/download.png";
const char* GAME_DOWNLOAD_IMG       = "hallimg/main/icon/downloading.png";
const char* GAME_RANK_EFFECT        = "hallimg/main/RankEffect.csb";
const float MAIN_ICON_SCALE = 1.0f;

const int	HAI_ZEI_WANG_LA_BA = 99999995;
const int	SAN_QI_LA_BA = 99999996;
const int	POINT_21 = 99999997;
const int	SHUI_HU_ZHUANG = 99999998;
const int	HUAN_LE_MA_JIANG = 99999999;
#define  SPINE_INTERVAL 70  //人物和左侧hotgamelist间距
#define LIUHAI_X_OFFSET 0	//刘海屏幕偏移15个单位
const vector<vector<std::string>> MUSIC_PATH_VEC = {{"v66/music/qipai2.mp3","v66/music/qipai2.mp3"},
    {"v66/music/buyu1.mp3","v66/music/buyu2.mp3"},
    {"v66/music/qipai1.mp3","v66/music/qipai1.mp3"},
    {"v66/music/xiuxian1.mp3","v66/music/xiuxian2.mp3"}
};

//多余需要漏出来部分
#define GameTypeWidth 20
#define fOtherSpace  1000
#define	ListSize Size(1200 + fOtherSpace, 500)
#define ItemSize Size(204, 204)
#define LeftItemSize Size(201,415)
#define DesignItemSize Size(ItemSize.width*MAIN_ICON_SCALE, ItemSize.height*MAIN_ICON_SCALE)

GameListCompositeV66::GameListCompositeV66() :
    _iValue(-1){
        _roomLogic = nullptr;
        _deskLogic = nullptr;
		m_pagePoint5 = nullptr;
		m_pagePointb0 = nullptr;
		m_pagePointb1 = nullptr;
		m_pagePointb2 = nullptr;
		m_pageArrowLeft = nullptr;
		m_pageArrowRight = nullptr;
        gameBtnX = 0;
		m_nSelType = 2;
		m_nPageNum = 0;
		m_nCurGameID = -1; 
}

GameListCompositeV66::~GameListCompositeV66() {
    if (_roomLogic) {
        HN_SAFE_DELETE(_roomLogic);
    }
    if (_deskLogic) {
        HN_SAFE_DELETE(_deskLogic);
    }
}

void GameListCompositeV66::onEnter()
{
	Node::onEnter();

	//新增加，默认选择捕鱼 qiuyx
	m_nSelType = UserDefault::getInstance()->getIntegerForKey("ENTER_GAME_LIST_TYPE");
	if (m_nSelType==0)
	{
		m_nSelType = 2;
	}

	resetCategoryBg(m_nSelType);
	btnTouchEffect(m_nSelType);
	updateGameList(m_nSelType);
	playSpineAnimation();

	//快速加入
	bottomMenuDelegate->quickJoinGame = [&]() {

		UserDefault::getInstance()->setBoolForKey(IS_QUICK_JOIN, true);
		auto roomId = UserDefault::getInstance()->getIntegerForKey(QUICK_JOIN_ROOM_ID);
		auto gameId = UserDefault::getInstance()->getIntegerForKey("Enter_Game_ID");
		if (roomId && gameId) {
			int roomIdx = UserDefault::getInstance()->getIntegerForKey("Enter_GAME_ROOM_IDX");
			if (roomIdx != -1) { //有奖池
				long long jackpot = GameData::getInstance()->getGameJackpotByGameRoom(gameId, roomIdx);
				GameData::getInstance()->setRoomJackpotData(jackpot);
			}
			CommandManager::getInstance()->addLoaderCommand(GetText("LODING_REQUEST_ROOMLIST"));
			GameCreator()->setCurrentGame(gameId);
			RoomInfoModule()->updateRoomsForTheGame(gameId);
			_roomLogic->start();
			_roomLogic->requestLogin(roomId);
		}
		else {

			CommandManager::getInstance()->showGamePrompt(GBKToUtf8("暂无可快速加入的房间"));
		}

	};
}

void GameListCompositeV66::onExit()
{
    Node::onExit();
	unschedule(schedule_selector(GameListCompositeV66::UpdateTimer));
    if (_deskLogic) {
        _deskLogic->stop();
    }
    if (_roomLogic) {
        _roomLogic->stop();
    }
}

bool GameListCompositeV66::init() 
{
    if (!Node::init())
	{
        return false;
    }
    
    _isTouch = false;
    _roomLogic = new HNRoomLogicBase(this);

	m_hotgameList = PageView::create();
	m_hotgameList->setScrollBarEnabled(false);
	m_hotgameList->setDirection(ui::PageView::Direction::HORIZONTAL);
	//m_hotgameList->setScrollDuration(0.5);//每0.5s滚动一次
	m_hotgameList->setVisible(true);
	m_hotgameList->setBounceEnabled(true);
	m_hotgameList->setContentSize(Size(LeftItemSize.width, LeftItemSize.height));
	m_hotgameList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_hotgameList->setPosition(Vec2(WIN_SIZE.width*(20.0f / 1280.0f), WIN_SIZE.height / 2 + 30));	
	m_hotgameList->setLocalZOrder(0);
	m_hotgameList->setClippingEnabled(true);
	m_hotgameList->removeAllPages();
	m_hotgameList->addEventListener(CC_CALLBACK_2(GameListCompositeV66::selectItemCallback, this));


	_storePath = GameData::getInstance()->getStorePath();
	if (!Tools::IsSearchPathExist(_storePath))
	{
		FileUtils::getInstance()->addSearchPath(_storePath);
	}

	//FileUtils::getInstance()->setWritablePath(_storePath+"/Games");
	if (!FileUtils::getInstance()->isDirectoryExist(_storePath + "/Games"))
	{
		FileUtils::getInstance()->createDirectory(_storePath + "/Games");
	}

    //csb
    LoadCategory();

	m_pageArrowLeft = dynamic_cast<Button*>(m_category->getChildByName("Button_arrow_left"));
	m_pageArrowRight = dynamic_cast<Button*>(m_category->getChildByName("Button_arrow_right"));

	if (m_pageArrowLeft && m_pageArrowRight)
	{
		m_pageArrowLeft->setTouchEnabled(true);
		m_pageArrowLeft->addTouchEventListener([this](Ref* pSender, Widget::TouchEventType type)
		{
			if (type == Widget::TouchEventType::ENDED)
			{
				//m_pagePoint5->setPositionX(m_pagePointb0->getPosition().x + LIUHAI_X_OFFSET);
				m_nPageNum = m_hotgameList->getCurPageIndex();
				if (m_nPageNum >= 1)
				{
					m_hotgameList->scrollToPage(--m_nPageNum);
				}
				
			}
		});
		m_pageArrowRight->setTouchEnabled(true);
		m_pageArrowRight->addTouchEventListener([this](Ref* pSender, Widget::TouchEventType type)
		{
			if (type == Widget::TouchEventType::ENDED)
			{
				m_nPageNum = m_hotgameList->getCurPageIndex();
				if (m_nPageNum < 3)
				{
					m_hotgameList->scrollToPage(++m_nPageNum);
				}
			}
		});
	}

	m_category->addChild(m_hotgameList);
	m_gameList1 = ListView::create();
	m_gameList1->setScrollBarEnabled(false);
	m_gameList1->setDirection(ui::ListView::Direction::HORIZONTAL);
	m_gameList1->setBounceEnabled(true);
	//float fLeftLength = WIN_SIZE.width*(24.0f / 1280.0f) + m_pagePointb2->getContentSize().width + SPINE_INTERVAL;
	float fLeftLength = m_pageArrowRight->getPosition().x + SPINE_INTERVAL;
	m_gameList1->setContentSize(Size(WIN_SIZE.width - fLeftLength, ListSize.height));
	m_gameList1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	m_gameList1->setPosition(Vec2(WIN_SIZE.width, WIN_SIZE.height/2 + 29));
	m_gameList1->setVisible(true);	
	m_gameList1->setLocalZOrder(1);
	m_gameList1->addEventListener((ui::ListView::ccScrollViewCallback)CC_CALLBACK_2(GameListCompositeV66::selectedItemEventScrollView, this));

	m_gameList2 = ListView::create();
	m_gameList2->setScrollBarEnabled(false);
	m_gameList2->setDirection(ui::ListView::Direction::HORIZONTAL);
	m_gameList2->setBounceEnabled(true);
	m_gameList2->setContentSize(Size(WIN_SIZE.width - fLeftLength, ListSize.height));
	m_gameList2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	m_gameList2->setPosition(Vec2(WIN_SIZE.width, WIN_SIZE.height / 2 + 29));
	m_gameList2->setVisible(true);
	m_gameList2->setLocalZOrder(1);
	m_gameList2->addEventListener((ui::ListView::ccScrollViewCallback)CC_CALLBACK_2(GameListCompositeV66::selectedItemEventScrollView, this));

	m_gameList3 = ListView::create();
	m_gameList3->setScrollBarEnabled(false);
	m_gameList3->setDirection(ui::ListView::Direction::HORIZONTAL);
	m_gameList3->setBounceEnabled(true);
	m_gameList3->setContentSize(Size(WIN_SIZE.width - fLeftLength, ListSize.height));
	m_gameList3->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	m_gameList3->setPosition(Vec2(WIN_SIZE.width, WIN_SIZE.height / 2 + 29));
	m_gameList3->setVisible(true);
	m_gameList3->setLocalZOrder(1);
	m_gameList3->addEventListener((ui::ListView::ccScrollViewCallback)CC_CALLBACK_2(GameListCompositeV66::selectedItemEventScrollView, this));

	m_gameList4 = ListView::create();
	m_gameList4->setScrollBarEnabled(false);
	m_gameList4->setDirection(ui::ListView::Direction::HORIZONTAL);
	m_gameList4->setBounceEnabled(true);
	m_gameList4->setContentSize(Size(WIN_SIZE.width - fLeftLength, ListSize.height));
	m_gameList4->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	m_gameList4->setPosition(Vec2(WIN_SIZE.width, WIN_SIZE.height / 2 + 29));
	m_gameList4->setVisible(true);
	m_gameList4->setLocalZOrder(1);
	m_gameList4->addEventListener((ui::ListView::ccScrollViewCallback)CC_CALLBACK_2(GameListCompositeV66::selectedItemEventScrollView, this));

	m_category->addChild(m_gameList1);
	m_category->addChild(m_gameList2);
	m_category->addChild(m_gameList3);
	m_category->addChild(m_gameList4);
	m_gameList1->setVisible(false);
	m_gameList2->setVisible(false);
	m_gameList3->setVisible(false);
	m_gameList4->setVisible(false);
	//街机
	m_mapUIIndexByGameID1.insert(std::pair<int, int >(1, 31000503));//SHZ::GAME_NAME_ID 水浒传
	m_mapUIIndexByGameID1.insert(std::pair<int, int >(2, 31000507));//CSLL::GAME_NAME_ID 财神来了
	m_mapUIIndexByGameID1.insert(std::pair<int, int >(3, 31000513));//Honor::GAME_NAME_ID 王者荣耀
	m_mapUIIndexByGameID1.insert(std::pair<int, int >(4, 31000508));//JSXS::GAME_NAME_ID 僵尸先生
	m_mapUIIndexByGameID1.insert(std::pair<int, int >(5, 31000506));//NL::GAME_NAME_ID 九线拉王
	m_mapUIIndexByGameID1.insert(std::pair<int, int >(6, 31000509));//Tiger::GAME_NAME_ID 皇家老虎机
	m_mapUIIndexByGameID1.insert(std::pair<int, int >(7, 31000510));//Medusa::GAME_NAME_ID 蛇蝎美人
	m_mapUIIndexByGameID1.insert(std::pair<int, int >(8, 31000511));//Pubg::GAME_NAME_ID 今晚吃鸡
	//捕鱼
	m_mapUIIndexByGameID2.insert(std::pair<int ,int >(1, 33003109));//GoldenToad::GAME_MONKEY_NAME_ID 大闹天宫
	m_mapUIIndexByGameID2.insert(std::pair<int ,int >(2, 33003108));//GoldenToad::GAME_LIKUI_NAME_ID 李逵捕鱼
	m_mapUIIndexByGameID2.insert(std::pair<int, int >(3, 33003107));//GoldenToad::GAME_TOAD_NAME_ID 金蝉捕鱼
	//棋牌
	m_mapUIIndexByGameID3.insert(std::pair<int ,int >(1, 11105101));//NN::GAME_TWO_NAME_ID 对战牛牛
	m_mapUIIndexByGameID3.insert(std::pair<int, int >(2, 11105102));//NN::GAME_FOUR_NAME_ID 四人牛牛
	m_mapUIIndexByGameID3.insert(std::pair<int, int >(3, 11105103));//NN::GAME_SIX_NAME_ID 通比牛牛
	m_mapUIIndexByGameID3.insert(std::pair<int, int >(4, 12100202));//STUD::GAME_NAME_ID 梭哈
	m_mapUIIndexByGameID3.insert(std::pair<int, int >(5, 12100004));//ThirteenZhang::GAME_NAME_ID 十三张 
	m_mapUIIndexByGameID3.insert(std::pair<int, int >(6, 12101106));//goldenflower::GAME_NAME_ID 赢三张
	m_mapUIIndexByGameID3.insert(std::pair<int ,int >(7, 11100200));//BJL::GAME_NAME_ID 龙虎斗
	//m_mapUIIndexByGameID3.insert(std::pair<int ,int >(8, 12100404));//D21::NAME_ID
	//休闲
	m_mapUIIndexByGameID4.insert(std::pair<int ,int >(1, 31000512));//EightSlot::GAME_NAME_ID 777拉霸
	m_mapUIIndexByGameID4.insert(std::pair<int, int >(2, 31000504));//FruitSlot::GAME_NAME_ID 水果拉霸
	m_mapUIIndexByGameID4.insert(std::pair<int ,int >(3, 31000505));//OnePieceSlot::GAME_NAME_ID 海贼王拉霸
	m_mapUIIndexByGameID4.insert(std::pair<int, int >(4, 31000501));//BB::GAME_NAME_ID 奔驰宝马
	m_mapUIIndexByGameID4.insert(std::pair<int ,int >(5, 31000500));//FQZS::GAME_NAME_ID 金鲨银鲨
	m_mapUIIndexByGameID4.insert(std::pair<int, int >(6, 31000502));//Fruit::GAME_NAME_ID 铃铛游戏
	m_mapUIIndexByGameID4.insert(std::pair<int ,int >(7, 11100600));//BRXJ::GAME_NAME_ID 百人小九

	IniGameList(1);
	IniGameList(2);
	IniGameList(3);
	IniGameList(4);

	auto btnFish = dynamic_cast<Button*>(Tools::getNodeByName(m_category, "btnFish"));
	auto btnJieji = dynamic_cast<Button*>(Tools::getNodeByName(m_category, "btnJieji"));
	auto btnQipai = dynamic_cast<Button*>(Tools::getNodeByName(m_category, "btnQipai"));
	auto btnOther = dynamic_cast<Button*>(Tools::getNodeByName(m_category, "btnOther"));
	auto btnView = dynamic_cast<ImageView*>(Tools::getNodeByName(m_category, "Image_page_bg"));
	if (btnFish&&btnJieji&&btnQipai&&btnOther&&btnView)
	{
		btnView->setLocalZOrder(9);
		
		btnFish->setLocalZOrder(10);
		btnJieji->setLocalZOrder(10);
		btnQipai->setLocalZOrder(10);
		btnOther->setLocalZOrder(10);

		btnView->setPositionX(WIN_SIZE.width);
	}
	
	_listADSpace.clear();
	_listADSpace = GameADSpaceManager::getInstance()->getADSpaceInfo();

	Vec2 pp = Vec2(0, 0);
	for (int i = 0; i < _listADSpace.size(); i++)
	{
		std::string strFilePath = GameADSpaceManager::getInstance()->getCurrentDir() + _listADSpace[i].strFileName;
		auto buttonItem = Button::create(strFilePath);
		buttonItem->addTouchEventListener(CC_CALLBACK_2(GameListCompositeV66::enterGameCallbackEx, this));
		buttonItem->setTag(i);
		buttonItem->setContentSize(Size(201,415));
		buttonItem->setAnchorPoint(Vec2::ANCHOR_MIDDLE);//0,1
		buttonItem->setPosition(pp);
		buttonItem->setPressedActionEnabled(false);
		
		m_hotgameList->addPage(buttonItem);
	}

	int nin = 1;
	m_hotgameList->scrollToPage(nin);

	schedule(schedule_selector(GameListCompositeV66::UpdateTimer), 3.0f);
    return true;
}

void GameListCompositeV66::onEnterTransitionDidFinish()
{

}

void GameListCompositeV66::selectItemCallback(Ref* pSender, PageView::EventType type)
{
	//if (!m_pagePoint5||!m_pagePointb0||!m_pagePointb1||!m_pagePointb2)
		//return;

	switch (type) 
	{
	case PageView::EventType::TURNING:
		if (m_hotgameList->getCurrentPageIndex() == 0)
		{
			m_nPageNum = 0;
			//m_pagePoint5->setPositionX(m_pagePointb0->getPosition().x+ LIUHAI_X_OFFSET);
		}
		else if (m_hotgameList->getCurrentPageIndex() == 1)
		{
			m_nPageNum = 1;
			//m_pagePoint5->setPositionX(m_pagePointb1->getPosition().x + LIUHAI_X_OFFSET);
		}
		else if (m_hotgameList->getCurrentPageIndex() == 2)
		{
			m_nPageNum = 2;
			//m_pagePoint5->setPositionX(m_pagePointb2->getPosition().x + LIUHAI_X_OFFSET);
		}
		else if (m_hotgameList->getCurrentPageIndex() == 3)
		{
			m_nPageNum = 3;
		}
		break;

	default:
		break;
	}

}

void GameListCompositeV66::pageCallback(Ref* pSender, cocos2d::ui::PageViewEventType type) {
	if (type == PageViewEventType::PAGEVIEW_EVENT_TURNING)
	{
		auto pageList = (PageView*)pSender;
		int curPageIndex = pageList->getCurrentPageIndex();
		int pageCount = pageList->getTag();

		auto pageIconLayer = dynamic_cast<Layer*>(getChildByName("PageIconLayer"));

		if (pageIconLayer)
		{
			float fWidth = 0;
			for (int i = 0; i < pageCount; i++)
			{
				ImageView* icon = dynamic_cast<ImageView*>(pageIconLayer->getChildByTag(i));
				if (icon)
				{
					string str = i == curPageIndex ? PAGE_TAG_ICON_CURRENT : PAGE_TAG_ICON_OTHER;
					icon->loadTexture(str);
					icon->setPositionX(fWidth);

					fWidth += icon->getContentSize().width + 5;
				}
			}
		}
	}
}

void GameListCompositeV66::enterGameCallback(Ref* pSender, Widget::TouchEventType type) {
    
    if(_isTouch || m_nCurGameID>0) return;
    	
    auto curSelecBtn = ((Button*)pSender);
    if (type != Widget::TouchEventType::ENDED)
    {
		if (type == Widget::TouchEventType::BEGAN) 
		{
			curSelecBtn->setScale(0.9f);
			//gameBtnX = curSelecBtn->getPosition().x;
			//_isMove = true;
			//menu->setClickButton(_isMove);
		}
		else if (type == Widget::TouchEventType::CANCELED)
		{
			curSelecBtn->setScale(1.0f);
		}
        return;
    }
    
	curSelecBtn->setScale(1.0f);
    _isTouch = true;
    runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([&](){
        _isTouch = false;
    }), nullptr));
    
	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
	m_nCurGameID = curSelecBtn->getTag();	
	mapButtons.insert(pair<int, Button*>(m_nCurGameID, curSelecBtn));

	int nTempGameID = m_nCurGameID;

	sprintf(resourseStr, "Games/%d", nTempGameID);
	if (CheckUpdateResourse(string(resourseStr), nTempGameID))
	{
		auto tempImg = curSelecBtn->getChildByName("nullimgsprite");
		if (tempImg != nullptr)
		{
			auto dwnImg = tempImg->getChildByName("dwnImg");
			auto dwnSpri = dwnImg->getChildByName("dwnSpri");
			auto dwnLabel = dwnImg->getChildByName("dwnLabel");
			if (dwnSpri != nullptr)
			{
				dwnSpri->removeFromParent();
				dwnLabel->setVisible(true);
			}

			ProgressTimer* fireballTimer = ProgressTimer::create(Sprite::create("common/gameicon/btn_process.png"));
			fireballTimer->setAnchorPoint(Vec2(0, 0));
			fireballTimer->setPosition(Vec2(0, 0));
			fireballTimer->setType(ProgressTimer::Type::RADIAL);
			fireballTimer->setPercentage(0);
			fireballTimer->setReverseDirection(false);
			fireballTimer->setMidpoint(Vec2(0.5, 0.5));
			dwnImg->addChild(fireballTimer);
			mapProgress.insert(pair<int, ProgressTimer*>(m_nCurGameID, fireballTimer));
		}

		auto newVer = mapNewVer[nTempGameID];

		UserDefault::getInstance()->setIntegerForKey(StringUtils::format("%d", nTempGameID).c_str(), newVer);
		/*
		maptipLabel.insert(pair<int, Label*>(m_nCurGameID, Label::createWithTTF("更新资源中，请稍等...", "platform/fonts/simhei.ttf", 30, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER)));
		maptipLabel[m_nCurGameID]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		maptipLabel[m_nCurGameID]->setPosition(Vec2(curSelecBtn->getContentSize().width / 2, curSelecBtn->getContentSize().height / 2));
		maptipLabel[m_nCurGameID]->setColor(Color3B::RED);
		curSelecBtn->addChild(maptipLabel[m_nCurGameID]);
		maptipLabel[m_nCurGameID]->setScale(0.5);
		*/
		

		std::string _downloadUrl;
		downloader.reset(new network::Downloader());

		char strGameID[16];
		sprintf(strGameID, "%d.zip", nTempGameID);
		//_downloadUrl = "https://client-1257384861.cos.ap-guangzhou.myqcloud.com/";// +31000507.zip";
		//_downloadUrl = "https://client-1258299297.cos.ap-guangzhou.myqcloud.com/";
		_downloadUrl = GameData::getInstance()->getSubPackageUrl();
		_downloadUrl += string(strGameID);

		downloader->onTaskProgress = [this](const DownloadTask& task, int64_t bytesReceived, int64_t totalBytesReceived, int64_t totalBytesExpected)
		{
			float percent = totalBytesExpected ? float(totalBytesReceived * 100 / totalBytesExpected) : 0;
			if (mapProgress[m_nCurGameID])
			{
				mapProgress[m_nCurGameID]->setPercentage(percent);
				auto tempImg = mapButtons[m_nCurGameID]->getChildByName("nullimgsprite");
				auto dwnImg = tempImg != nullptr ? tempImg->getChildByName("dwnImg") : nullptr;
				auto dwnLabel = dwnImg != nullptr ? static_cast<Label*>(dwnImg->getChildByName("dwnLabel")) : nullptr;
				if (dwnLabel != nullptr)
				{
					char szPercent[16] = { 0 };
					sprintf(szPercent, "%d", (int)percent);
					dwnLabel->setString(string(szPercent) + string("%"));
				}
			}
		};
		downloader->onFileTaskSuccess = [this](const DownloadTask& task)
		{
			//解压			
			int nTempGameID = m_nCurGameID;
			//if (nTempGameID == 33003107 || nTempGameID == 33003108)
			//{
			//	nTempGameID = 33003109;
			//}

			sprintf(resourseStr, "Games/%d.zip", nTempGameID);
			//maptipLabel[m_nCurGameID]->setString("解压资源中");
			if (decompress(_storePath + resourseStr))
			{
				//maptipLabel[m_nCurGameID]->setString("解压成功");
				scheduleUpdate();

				auto img = mapButtons[m_nCurGameID]->getChildByName("nullimgsprite");
				if (img)
				{
					img->removeFromParent(); 
				}
				auto pSkeAnim = mapButtons[m_nCurGameID]->getChildByName("skeleton_animation");
				if (pSkeAnim != nullptr)
				{
					pSkeAnim->setVisible(true);
				}
				/*
				vector<std::string> spineData = getGameIconSpineName(gameID);
				if (spineData.size() == 2)
				{
					auto jsonPath = spineData.at(0) + ".json";
					auto atlasPath = spineData.at(0) + ".atlas";
					auto skeletonNode = spine::SkeletonAnimation::createWithJsonFile(jsonPath, atlasPath);
					if (skeletonNode)
					{
						Vec2 pp = Vec2(103.5, -29);
						skeletonNode->setAnchorPoint(Vec2(1, 0.5));
						skeletonNode->setPosition(pp);
						skeletonNode->setAnimation(0, spineData.at(1), true);
						mapButtons[gameID]->addChild(skeletonNode);

						auto img = mapButtons[gameID]->getChildByName("nullimgsprite");
						if (img)
						{
							img->removeFromParent();
						}
					}
				}
				*/
			}
			else
			{
				FileUtils::getInstance()->removeDirectory(_storePath + string(resourseStr));
				//maptipLabel[m_nCurGameID]->setString("解压失败");
			}

			//maptipLabel[m_nCurGameID]->removeFromParent();
			//maptipLabel.erase(m_nCurGameID);
			m_nCurGameID = -1;
		};

		downloader->onTaskError = [this](const DownloadTask& task, int errorCode, int errorCodeInternal, const std::string& errorStr)
		{
			//maptipLabel[m_nCurGameID]->setString(StringUtils::format("更新资源失败,错误代号:%d", errorCode));
			m_nCurGameID = -1;
		};

		sprintf(resourseStr, "Games/%d.zip", m_nCurGameID);
		downloader->createDownloadFileTask(_downloadUrl, _storePath + resourseStr, "download");
		return;
	}

	if (m_nCurGameID >= HAI_ZEI_WANG_LA_BA)
	{
		CommandManager::getInstance()->showGamePrompt(GBKToUtf8("即将开放，敬请期待"));
		return;
	}

	//快速加入数据
	UserDefault::getInstance()->setBoolForKey(IS_QUICK_JOIN, false);
	UserDefault::getInstance()->setIntegerForKey(QUICK_JOIN_GAME_ID, m_nCurGameID);

	GameCreator()->setCurrentGame(m_nCurGameID);
	//更新当前游戏的房间信息
	RoomInfoModule()->updateRoomsForTheGame(m_nCurGameID);

	//CommandManager::getInstance()->addLoaderCommand(GetText("LODING_REQUEST_ROOMLIST"));
	_gameListLogic->stop();
	platformDelegate->setLayerType(ROOMLIST);

	m_nCurGameID = -1;

	return;
}


bool GameListCompositeV66::CheckUpdateResourse(string path, int gameid)
{
	//不存在资源路径，要下载
	auto newVer = mapNewVer[gameid];
	if (!FileUtils::getInstance()->isDirectoryExist(_storePath + path))
	{
		//UserDefault::getInstance()->setIntegerForKey(path.c_str(), newVer);
		return true;
	}
	
	auto localVer = UserDefault::getInstance()->getIntegerForKey(StringUtils::format("%d", gameid).c_str());
	if (newVer>localVer)
	{
		//UserDefault::getInstance()->setIntegerForKey(path.c_str(), newVer);
		FileUtils::getInstance()->removeDirectory(_storePath + path);
		return true;
	}

	return false;
}

//无缩放
void GameListCompositeV66::enterGameCallbackEx(Ref* pSender, Widget::TouchEventType type) {

	if (_isTouch) return;

	auto curSelecBtn = ((Button*)pSender);
	if (type != Widget::TouchEventType::ENDED)
	{
		return;
	}

	_isTouch = true;
	runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([&]() {
		_isTouch = false;
	}), nullptr));


	//_isMove = fabs(curSelecBtn->getPosition().x-gameBtnX)<30;

	//if (_isMove) {
	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);

	int nIndex = curSelecBtn->getTag();

	if (nIndex < 0 || nIndex >= _listADSpace.size())
	{
		return;
	}

	string url1 = _listADSpace[nIndex].strAppUrl;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	Application::getInstance()->openURL(url1);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	Application::getInstance()->openURL(url1);
#endif


}

void GameListCompositeV66::btnGameTypeCallback(Ref* pSender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;

	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
    
    Button* btn = dynamic_cast<Button*>(pSender);
    int nType = btn->getTag();
	//m_gameList->setVisible(true);
    
	resetCategoryBg(nType);
    //HideCategory();
	//记录分类
	UserDefault::getInstance()->setIntegerForKey("ENTER_GAME_LIST_TYPE", nType);

	btnTouchEffect(nType);
	updateGameList(nType);
    platformDelegate->setLayerType(GAMELIST);
    
    if (nType > 0 && nType <= MUSIC_PATH_VEC.size()) {
        auto arrayPath = MUSIC_PATH_VEC.at(nType - 1);
        auto nRand = random(0,1);
        if (nRand == 0 || nRand == 1) {
            auto musicPath = arrayPath.at(nRand);
            HNAudioEngine::getInstance()->playEffect(musicPath.c_str());
        }
    }
   
}

void GameListCompositeV66::requestGameRoomTimerCallBack(float dt) {
    switch (getGameType()) {
    case eSingleGame:
        CommandManager::getInstance()->addLoaderCommand(GetText("LODING_REQUEST_ROOMLIST"));
        _roomListLogic->start();
        _roomListLogic->requestRoomList();
        break;
    case eMixGame:
        CommandManager::getInstance()->addLoaderCommand(GetText("LODING_REQUEST_ROOMLIST"));
        _gameListLogic->start();
        _gameListLogic->requestGameList();
        break;
    }
}

void GameListCompositeV66::iconListMoveOut(bool bOut)
{
	ListView* m_gameList;
	if (m_nSelType == 1)
	{
		m_gameList = m_gameList1;
	}
	else if (m_nSelType == 2)
	{
		m_gameList = m_gameList2;
	}
	else if (m_nSelType == 3)
	{
		m_gameList = m_gameList3;
	}
	else if (m_nSelType == 4)
	{
		m_gameList = m_gameList4;
	}
	else
	{
		return;
	}

     auto pageSize = m_gameList->getContentSize();

	 auto gameTypeBg = dynamic_cast<ImageView*>(getChildByName("CategoryBg"));
     
	 float fMovePosX = bOut ? (pageSize.width + GameTypeWidth) : (-pageSize.width - GameTypeWidth);
     
     if(bOut)
     {
		 m_gameList->runAction(Sequence::create(DelayTime::create(0.3f), MoveBy::create(0.3f, Vec2(fMovePosX, 0)), NULL));
		 if (gameTypeBg)
		 {
			 gameTypeBg->runAction(Sequence::create(DelayTime::create(0.3f), MoveBy::create(0.3f, Vec2(fMovePosX, 0)), NULL));
		 }
     }else{
		 m_gameList->runAction(Sequence::create(DelayTime::create(0.5f), MoveBy::create(0.3f, Vec2(fMovePosX, 0)), NULL));
		 if (gameTypeBg)
		 {
			 gameTypeBg->runAction(Sequence::create(DelayTime::create(0.5f), MoveBy::create(0.3f, Vec2(fMovePosX, 0)), NULL));
		 }
     }
 
 	//页数
//  	if (getChildByName("PageIconLayer"))
//  	{
//  		if (bOut)
//  		{
//  			getChildByName("PageIconLayer")->runAction(Sequence::create(DelayTime::create(0.3f), MoveBy::create(0.3f, Vec2(fMovePosX, 0)), NULL));
//  		}
//  		else{
//  			getChildByName("PageIconLayer")->runAction(Sequence::create(DelayTime::create(0.5f), MoveBy::create(0.3f, Vec2(fMovePosX, 0)), NULL));
//  		}
//  	}
}

void GameListCompositeV66::curGameIconMoveIn(bool bIn)
{
    auto hotGameBtn = dynamic_cast<Button*>(getChildByName("HotGameBtn"));
//    hotGameBtn->setTouchEnabled(!bIn);
//    hotGameBtn->setVisible(!bIn);
    runAction(Sequence::create(DelayTime::create(0.35f), CallFunc::create([=]() {
        if (hotGameBtn) {
            hotGameBtn->setTouchEnabled(!bIn);
            hotGameBtn->setVisible(!bIn);
        }
    }), nullptr));
    
    
    char temp[64] = {0};
    
    auto leftIcon =  dynamic_cast<Sprite*>(getChildByName("LeftIconSprite"));
    if(leftIcon)
    {
        int gameID = 0;
        if(bIn)
        {
            gameID = GameCreator()->getCurrentGameNameID();
        }else{
            gameID = hotGameBtn->getTag();
        }
        
        if(gameID > 0)
        {
            sprintf(temp, "hallimg/main/icon/%d.png", gameID);
            leftIcon->setTexture(temp);
            leftIcon->setVisible(false);
			//leftIcon->setPositionX(iconPos.x + 30);

        }
    }
}

void GameListCompositeV66::createPageIcon(int page)
{
// 	Vec2 pagePos = m_pageList->getParent()->getPosition();
// 	Size pageSize = m_pageList->getContentSize();
// 	Vec2 iconPos = Vec2(pagePos.x, pagePos.y - pageSize.height/2 - 10);
// 
// 	auto pageIconLayer = Layer::create();
// 	pageIconLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
// 	addChild(pageIconLayer);
// 	pageIconLayer->setName("PageIconLayer");
// 
// 	float fWidth = 0;
// 	for (int i = 0; i < page; i++)
// 	{
// 		string str = i == 0 ? PAGE_TAG_ICON_CURRENT : PAGE_TAG_ICON_OTHER;
// 		bool tag = i == 0 ? true : false;
// 
// 		auto icon = ImageView::create(str.c_str());
// 		icon->setTag(i);
// 		icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
// 		icon->setPositionX(fWidth);
// 		pageIconLayer->addChild(icon);
// 		
// 		fWidth += icon->getContentSize().width + 5;
// 	}
// 
// 	pageIconLayer->setContentSize(Size(fWidth, 15));
// 	pageIconLayer->setPosition(Vec2(iconPos.x - pageIconLayer->getContentSize().width/2, iconPos.y));
	
}

void GameListCompositeV66::resetCategoryBg(int nType)
{
	auto btnFish = dynamic_cast<Sprite*>(Tools::getNodeByName(m_category, "btnFish")->getChildByName("btnbg"));
	auto btnJieji = dynamic_cast<Sprite*>(Tools::getNodeByName(m_category, "btnJieji")->getChildByName("btnbg"));
	auto btnQipai = dynamic_cast<Sprite*>(Tools::getNodeByName(m_category, "btnQipai")->getChildByName("btnbg"));
	auto btnOther = dynamic_cast<Sprite*>(Tools::getNodeByName(m_category, "btnOther")->getChildByName("btnbg"));

	btnJieji->setTexture("hallimg/main/category/images/hallpic/zjm_btn_jieji01.png");
	btnFish->setTexture("hallimg/main/category/images/hallpic/zjm_btn_buyu01.png");
	btnQipai->setTexture("hallimg/main/category/images/hallpic/zjm_btn_qipai01.png");
	btnOther->setTexture("hallimg/main/category/images/hallpic/zjm_btn_xiuxian01.png");

	if (nType==1)
	{
		btnJieji->setTexture("hallimg/main/category/images/hallpic/zjm_btn_jieji02.png");
	}
	else if (nType==2)
	{
		btnFish->setTexture("hallimg/main/category/images/hallpic/zjm_btn_buyu02.png");
	}
	else if (nType == 3)
	{
		btnQipai->setTexture("hallimg/main/category/images/hallpic/zjm_btn_qipai02.png");
	}
	else if (nType == 4)
	{
		btnOther->setTexture("hallimg/main/category/images/hallpic/zjm_btn_xiuxian02.png");
	}
}

void GameListCompositeV66::LoadCategory()
{
    auto category = CSLoader::createNode("hallimg/main/Category.csb");
    if (nullptr == category)
    {
        return;
    }
		
	m_category = dynamic_cast<Layout*>(category->getChildByName("Panel_9")->getChildByName("layout"));
	m_category->setContentSize((Size(WIN_SIZE.width, m_category->getContentSize().height)));
	category->setName("Category");
	category->setPosition(WIN_SIZE/2);
    addChild(category);
    
    auto btnJieji = dynamic_cast<Button*>(Tools::getNodeByName(m_category, "btnJieji"));
    if (nullptr != btnJieji)
    {
		btnJieji->setAlphaTouchEnable(true);
        btnJieji->setTag(1);
        //btnJieji->setZoomScale(0.05);
        btnJieji->addTouchEventListener(CC_CALLBACK_2(GameListCompositeV66::btnGameTypeCallback, this));
        //auto jiejiNode = btnJieji->getChildByName("Node_jieji");
        //if (jiejiNode) {
        //    jiejiNode->setPosition(Vec2(jiejiNode->getPositionX()-18,jiejiNode->getPositionY()-18));
        //    createIconEffect(0,23,"1_000","csb/v66/Node_jiejiEffect.csb",jiejiNode,true);
        //}
        //

#if 0
		
        auto jackpotModel = JackpotModel::create();
        if (jackpotModel) {
            jackpotModel->setPosition(Vec2(btnJieji->getContentSize().width/2,btnJieji->getContentSize().height - 60));
            btnJieji->addChild(jackpotModel);
            
            jackpotModel->setScale(0.40f);
            jackpotModel->setGameData(1,-1,-1);
			jackpotModel->setVisible(false);
        }
		
#endif
    }
    
    auto btnFish = dynamic_cast<Button*>(Tools::getNodeByName(m_category, "btnFish"));
    if (nullptr != btnFish)
    {   btnFish->setTitleText("");
        btnFish->setTag(2);
        //btnFish->setZoomScale(0.05);
        btnFish->addTouchEventListener(CC_CALLBACK_2(GameListCompositeV66::btnGameTypeCallback, this));
		btnFish->setAlphaTouchEnable(true);
        //createIconEffect(0,11,"h_000","csb/v66/Node_fishFire.csb",btnFish->getChildByName("Node_fire"),true);
        //createIconEffect(1,11,"g_000","csb/v66/Node_fishLight.csb",btnFish->getChildByName("Node_light"),false);
    }
    
    auto btnQipai = dynamic_cast<Button*>(Tools::getNodeByName(m_category, "btnQipai"));
    if (nullptr != btnQipai)
    {
		btnQipai->setAlphaTouchEnable(true);
        btnQipai->setTag(3);
        //btnQipai->setZoomScale(0.05);
        btnQipai->addTouchEventListener(CC_CALLBACK_2(GameListCompositeV66::btnGameTypeCallback, this));
        //createIconEffect(1,10,"3_000","csb/v66/Node_qipaiEffect.csb",btnQipai->getChildByName("Node_qipai"),false);
    }
    
    auto btnOther = dynamic_cast<Button*>(Tools::getNodeByName(m_category, "btnOther"));
    if (nullptr != btnOther)
    {
		btnOther->setAlphaTouchEnable(true);
        btnOther->setTag(4);
        //btnOther->setZoomScale(0.05);
        btnOther->addTouchEventListener(CC_CALLBACK_2(GameListCompositeV66::btnGameTypeCallback, this));
    }

	auto btnTimeLight = dynamic_cast<Node*>(Tools::getNodeByName(m_category, "Node_time_light"));
	if (nullptr != btnTimeLight)
	{
		Tools::PlaySkeletonAnimation(btnTimeLight, "v66/animation/timelight/", "effect_youche_light",0, Vec2(58, -350));
	}

	if (GameData::getInstance()->isFirstSlideGameListFlag())
	{
		auto effect_hint = dynamic_cast<Node*>(Tools::getNodeByName(m_category, "Node_effect_hint"));
		if (nullptr != effect_hint)
		{
			Ref *pRef = Tools::PlaySkeletonAnimation(effect_hint, "v66/animation/arroweffect/", "zjm_img_zhixiang", 0, Vec2(-20, -227));
			auto pSkeAnim = static_cast<spine::SkeletonAnimation*>(pRef);
			if (pRef != nullptr && pSkeAnim != nullptr)
			{
				pSkeAnim->setTimeScale(pSkeAnim->getTimeScale()*0.8f);
			}
		}
	}
	
	

    /*
    auto btnHot = dynamic_cast<Button*>(m_category->getChildByName("btnReMen"));
    if (nullptr != btnHot)
    {
        
        btnHot->setTag(5);
        btnHot->setZoomScale(0.05);
        btnHot->addTouchEventListener(CC_CALLBACK_2(GameListCompositeV66::btnGameTypeCallback, this));
    }
	*/

	//auto jsonPath = "v66/skeletalani/hall/renwu.json";
	//auto atlasPath = "v66/skeletalani/hall/renwu.atlas";
	//auto topSpine = spine::SkeletonAnimation::createWithJsonFile(jsonPath, atlasPath);
	//if (topSpine) {
		//auto myListener = EventListenerTouchOneByOne::create();
		//myListener->setSwallowTouches(true);//阻止触摸向下传递
		//myListener->onTouchBegan = [](Touch *touch, Event *unused_event) {
		//	return true;
		//};
		//myListener->onTouchEnded = [&](Touch *touch, Event *unused_event) {
		//	close();
		//};
		//
		//topSpine->getEventDispatcher()->addEventListenerWithSceneGraphPriority(myListener, topSpine);

		//topSpine->setPosition(Vec2(WIN_SIZE.width / 2-WIN_SIZE.width/8, 0));
		//topSpine->setPosition(Vec2(WIN_SIZE.width*(24.0f / 1280.0f)+m_hotgameList->getContentSize().width+ SPINE_INTERVAL, -20));
		//topSpine->setAnimation(0, "renwu", true);
		//m_category->addChild(topSpine, 2);
	//}

	
}
//创建动画特效-时间控制
void GameListCompositeV66::createIconEffect(int startIdx,int endIdx,std::string picName,std::string animationName,Node* parentNode,bool loop)
{
    if (parentNode == nullptr) {
        return;
    }
       
    auto node_action = CSLoader::createNode(animationName);
    if (node_action) {
        node_action->setPosition(Vec2(0,0));
        parentNode->addChild(node_action);
            
        auto action = CSLoader::createTimeline(animationName);
        if (action) {
            node_action->runAction(action);
            if (loop) {
                action->gotoFrameAndPlay(0);
            }
            else
            {
                action->gotoFrameAndPlay(0,false);
                action->setLastFrameCallFunc([=](){
                    node_action->runAction(Sequence::create(DelayTime::create(2.0f),CallFunc::create([=](){
                        action->gotoFrameAndPlay(0,false);
                    }) ,NULL));
                });
            }
            
        }
    }
    
    
    /*char str[32];
    Animation * anima = AnimationCache::getInstance()->getAnimation(animationName);
    if (anima == nullptr) {
        anima = Animation::create();
        anima->setDelayPerUnit(0.1);
        anima->setLoops(-1);
        for (int i = startIdx; i <= endIdx; i++) {
            std::string name = picName;
            sprintf(str, "%02d.png",i);
            name.append(str);
            anima->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(name));
        }
        AnimationCache::getInstance()->addAnimation(anima, animationName);
    }
    
    auto animate = Animate::create(anima);
    auto fire = Sprite::create();
    fire->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    fire->runAction(animate);
    fire->setPosition(Vec2::ZERO);
    parentNode->addChild(fire,1);*/
    
}
void GameListCompositeV66::createCategory()
{
	auto categoryBg = ImageView::create("hallimg/main/page/8.png");
	categoryBg->setName("CategoryBg");
	categoryBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	categoryBg->setPosition(Vec2(WIN_SIZE.width, WIN_SIZE.height/2));
	addChild(categoryBg);

	Size bgSize = categoryBg->getContentSize();

	char str[64] = { 0 };
	for (int i = 0; i < 4; i++)
	{
		Button* btn = Button::create();
		sprintf(str, "hallimg/main/page/%d.png", i*2);
		btn->loadTextureNormal(str);
		sprintf(str, "hallimg/main/page/%d.png", i * 2 + 1);
		btn->loadTexturePressed(str);
		btn->loadTextureDisabled(str);

		btn->setTag(i+1);
        btn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		btn->setPosition(Vec2(bgSize.width-1, bgSize.height - 11 - i*btn->getContentSize().height));
		btn->addTouchEventListener(CC_CALLBACK_2(GameListCompositeV66::btnGameTypeCallback, this));
		categoryBg->addChild(btn);

		m_vButtonCagegory.push_back(btn);
	}
    
    // 放置鞭炮
//    auto pos = categoryBg->getPosition();
//    pos.x -= bgSize.width;
//    pos.y += 150;
//    CreateFireworks(pos);
}

void GameListCompositeV66::CreateFireworks(Vec2 pos)
{
    const static char* fireworks = "NewYear/fireCracker.csb";
    auto rightFireworksNode = CSLoader::createNode(fireworks);
    auto rightFireworksTimeline = CSLoader::createTimeline(fireworks);
    
    if (nullptr != rightFireworksNode && nullptr != rightFireworksTimeline)
    {
        this->addChild(rightFireworksNode, 200);
        rightFireworksNode->setPosition(pos);
        rightFireworksNode->runAction(rightFireworksTimeline);
        rightFireworksTimeline->gotoFrameAndPlay(0);
    }
    
    auto leftFireworksNode = CSLoader::createNode(fireworks);
    auto leftFireworksTimeline = CSLoader::createTimeline(fireworks);

    auto leftPos = Vec2(WIN_SIZE.width - pos.x, pos.y);
    if (nullptr != leftFireworksNode && nullptr != leftFireworksTimeline)
    {
        leftFireworksNode->setScaleX(-1);
        this->addChild(leftFireworksNode, 200);
        leftFireworksNode->setPosition(leftPos);
        leftFireworksNode->runAction(leftFireworksTimeline);
        //leftFireworksNode->runAction(FlipX::create(true));
        leftFireworksTimeline->gotoFrameAndPlay(0);
    }
}

void GameListCompositeV66::btnTouchEffect(int tag)
{
	for (auto child : m_vButtonCagegory)
	{
		child->setTouchEnabled(tag != child->getTag());
		child->setBright(tag != child->getTag());
	}
}
/*
void GameListCompositeV66::IniGameList_old(int nType)
{
	ListView* m_gameList;
	if (nType == 1)
	{
		m_gameList = m_gameList1;
	}
	else if (nType == 2)
	{
		m_gameList = m_gameList2;
	}
	else if (nType == 3)
	{
		m_gameList = m_gameList3;
	}
	else if (nType == 4)
	{
		m_gameList = m_gameList4;
	}
	else
	{
		return;
	}

	vRealGames = GameData::getInstance()->getValidGames(nType);
	mapNewVer = GameData::getInstance()->getGamesVer();
	auto gameCount = vRealGames.size();
	auto layout = Layout::create();
	auto widthCnt = gameCount / 2;
	if (gameCount % 2 == 1)
	{
		widthCnt += 1;
	}

	m_gameList->pushBackCustomItem(layout);
	layout->setContentSize(Size(widthCnt * DesignItemSize.width + fOtherSpace * (widthCnt - 1) + WIN_SIZE.width*(306.0 / 1280.0), ListSize.height));

	
	float fStartX = 0;
	float fStartY = 0;
	float fup = 102;
	float fdown = 204;
	Vec2 pp = Vec2(103.5, -29);
	for (int i = 0; i < gameCount; i++)
	{
		char tempStr[32] = { 0 };
		char resourseStr[32] = { 0 };
		UINT uNameID = vRealGames.at(i);
		sprintf(tempStr, "hallimg/main/icon/icon_bg.png");
		if (i % 2 == 0)
		{
			fStartY = ListSize.height - 40;
			fStartX = (i / 2) * (DesignItemSize.width + fOtherSpace) + fup;
		}
		else
		{
			fStartY = ItemSize.height + 30;
			fStartX = (i - 1) / 2 * (DesignItemSize.width + fOtherSpace) + fdown;
		}

		auto buttonItem = Button::create(tempStr);
		layout->addChild(buttonItem);
		buttonItem->addTouchEventListener(CC_CALLBACK_2(GameListCompositeV66::enterGameCallback, this));
		buttonItem->setTag(uNameID);
		//buttonItem->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);//0,1
		buttonItem->setAnchorPoint(Vec2::ANCHOR_MIDDLE);//0,1
		buttonItem->setPressedActionEnabled(false);
		buttonItem->setZoomScale(0.0f);
		buttonItem->setPosition(Vec2(fStartX + 1.2*buttonItem->getContentSize().width / 2, fStartY - 1.2*buttonItem->getContentSize().height / 2));
		buttonItem->setScale(1.2f);
		buttonItem->setAlphaTouchEnable(true);


		bool isHasJackpot = GameData::getInstance()->getGameHasJackpot(uNameID);
		if (isHasJackpot)
		{
			auto nodeCsb = CSLoader::createNode("csb/v66/Node_jackpotEffect.csb");
			if (nodeCsb)
			{
				nodeCsb->setPosition(Vec2(0, 0));
				nodeCsb->setPosition(Vec2(buttonItem->getContentSize().width / 2, buttonItem->getContentSize().height + 10));
				buttonItem->addChild(nodeCsb, -1);
				auto nodeAction = CSLoader::createTimeline("csb/v66/Node_jackpotEffect.csb");
				if (nodeAction)
				{
					nodeCsb->runAction(nodeAction);
					nodeAction->gotoFrameAndPlay(0);
				}
			}

			auto imgJackpot = ImageView::create("hallimg/main/icon/jackpot.png");
			if (imgJackpot)
			{
				imgJackpot->setPosition(Vec2(buttonItem->getContentSize().width / 2, buttonItem->getContentSize().height - 20));
				buttonItem->addChild(imgJackpot, 1);
			}

			auto jackpotModel = JackpotModel::create();
			if (jackpotModel)
			{
				jackpotModel->setPosition(Vec2(buttonItem->getContentSize().width / 2, buttonItem->getContentSize().height - 60));
				buttonItem->addChild(jackpotModel, 1);

				jackpotModel->setScale(0.30f);
				jackpotModel->setGameData(-1, uNameID, -1);
			}

			nodeCsb->setVisible(false);
			imgJackpot->setVisible(false);
			jackpotModel->setVisible(false);
		}


		vector<std::string> spineData = getGameIconSpineName(uNameID);
		int nTempGameID = uNameID;
		//if (nTempGameID == 33003107|| nTempGameID == 33003108)
		//{
		//	nTempGameID = 33003109;
		//}

		sprintf(resourseStr, "Games/%d", nTempGameID);
		if (spineData.size() == 2 && !(CheckUpdateResourse(string(resourseStr), nTempGameID)))
		{
			auto jsonPath = spineData.at(0) + ".json";
			auto atlasPath = spineData.at(0) + ".atlas";
			auto skeletonNode = spine::SkeletonAnimation::createWithJsonFile(jsonPath, atlasPath);
			if (skeletonNode)
			{
				skeletonNode->setAnchorPoint(Vec2(1, 0.5));
				skeletonNode->setPosition(pp);
				skeletonNode->setAnimation(0, spineData.at(1), true);
				buttonItem->addChild(skeletonNode);
			}
		}
		else
		{
			char strgameID[32] = { 0 };
			sprintf(strgameID, "common/gameicon/%d.png", uNameID);
			auto imgsprite = Sprite::create(strgameID);
			if (imgsprite)
			{
				imgsprite->setName("nullimgsprite");
				buttonItem->addChild(imgsprite);
				imgsprite->setPosition(buttonItem->getContentSize() / 2);

				auto dwnImg = ImageView::create("common/gameicon/btn_bg.png");
				float fCale = 0.7f;
				dwnImg->setScale(fCale);
				dwnImg->setAnchorPoint(Vec2(1, 0));
				dwnImg->setPosition(Vec2(imgsprite->getContentSize().width - 5, 20));
				dwnImg->setName("dwnImg");
				imgsprite->addChild(dwnImg);

				auto dwnSpri = Sprite::create("common/gameicon/btn_download.png");
				dwnSpri->setAnchorPoint(Vec2(0, 0));
				dwnSpri->setPosition(Vec2(0, 0));
				dwnSpri->setName("dwnSpri");
				dwnImg->addChild(dwnSpri);
			}
		}
	}
}
*/

bool GameListCompositeV66::isExistGameID(const std::vector<int> & listData, int nGameID)
{
	bool bExist = false;
	for (int i = 0; i < listData.size(); ++i)
	{
		if (listData[i] == nGameID)
		{
			bExist = true;
			break;
		}
	}
	return bExist;
}

void GameListCompositeV66::playSpineAnimation()
{
	auto it = m_mapSpines.begin();
	for (; it != m_mapSpines.end(); ++it)
	{
		auto pAuto = static_cast<spine::SkeletonAnimation*>(it->second);
		if (it->second != nullptr && pAuto != nullptr)
		{
			pAuto->setAnimation(0, it->first, true);
		}
	}
}

void GameListCompositeV66::IniGameList(int nType)
{
	std::map<int, int> mapUIIndexByGameID;
	ListView* m_gameList;
	if (nType == 1)
	{
		m_gameList = m_gameList1;
		mapUIIndexByGameID = m_mapUIIndexByGameID1;
	}
	else if (nType == 2)
	{
		m_gameList = m_gameList2;
		mapUIIndexByGameID = m_mapUIIndexByGameID2;
	}
	else if (nType == 3)
	{
		m_gameList = m_gameList3;
		mapUIIndexByGameID = m_mapUIIndexByGameID3;
	}
	else if (nType == 4)
	{
		m_gameList = m_gameList4;
		mapUIIndexByGameID = m_mapUIIndexByGameID4;
	}
	else
	{
		return;
	}

	vRealGames = GameData::getInstance()->getValidGames(nType);
	mapNewVer = GameData::getInstance()->getGamesVer();
	//auto nGameCount = vRealGames.size();
	


	auto pLytGameList = Layout::create();
	pLytGameList->setContentSize(Size(ListSize.width, ListSize.height));
	pLytGameList->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	
	//char szBtnBg[32] = { 0 };
	
	float fUp = 10;
	float fDown = 10;
	float fStartX = 0;
	float fLastItemWidth = 0;
	float fSecondItemHeight = 0;
	float fStartY = fDown;
	float fRightSpace = 290;

	auto btnView = dynamic_cast<ImageView*>(Tools::getNodeByName(m_category, "Image_page_bg"));
	if (btnView != nullptr)
	{
		fRightSpace = btnView->getContentSize().width + 10;
	}

	auto nGameCount = mapUIIndexByGameID.size();
	for (int i = 1; i <= nGameCount; i++)
	{
		char szStaticIconPath[256] = { 0 };
		char szSpineIconPath[256] = { 0 };
		char szResoursePath[256] = { 0 };
		float fPositionX = 0;
		float fPositionY = 0;
		UINT uNameID = mapUIIndexByGameID[i];
		if (!isExistGameID(vRealGames, uNameID))
		{
			continue;
		}

		Button * pBtnIcon = nullptr;
		Sprite * pSprIcon = nullptr;
		Size sSprSize = Size::ZERO;

		//骨骼动画路径;
		sprintf(szSpineIconPath, "common/gameicon/spine/%d/", uNameID);
		bool bSpineExist = FileUtils::getInstance()->isDirectoryExist(szSpineIconPath);

		sprintf(szStaticIconPath, "common/gameicon/static/%d.png", uNameID);

		pBtnIcon = Button::create(szStaticIconPath);
		pBtnIcon->addTouchEventListener(CC_CALLBACK_2(GameListCompositeV66::enterGameCallback, this));
		pBtnIcon->setTag(uNameID);
		pBtnIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		pBtnIcon->setPressedActionEnabled(false);
		pBtnIcon->setZoomScale(0.0f);
		pBtnIcon->setScale(1.0f);
		pBtnIcon->setAlphaTouchEnable(true);

		pSprIcon = Sprite::create(szStaticIconPath);
		sSprSize = pBtnIcon->getContentSize();
		pSprIcon->setContentSize(sSprSize);
		pSprIcon->setColor(Color3B::BLACK);
		pSprIcon->setOpacity(128);
		pSprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		pSprIcon->setPosition(sSprSize.width / 2, sSprSize.height / 2);
		pBtnIcon->addChild(pSprIcon);

		//BlendFunc cbl = { GL_DST_COLOR,GL_ONE_MINUS_SRC_ALPHA };
		//pSprIcon->setBlendFunc(cbl);
		pSprIcon->setVisible(false);
		
		Vec2 vPosition = getGameIconPositon(i, pBtnIcon->getContentSize(), fSecondItemHeight, fStartX, fLastItemWidth);
		pBtnIcon->setPosition(vPosition);

		std::string strFileName;
		spine::SkeletonAnimation * pSkeAnim = nullptr;
		if (!GameData::getInstance()->getGameIconSpineName(uNameID, strFileName))
		{
			std::vector<std::string> listFileName;
			Tools::GetFileDirListByPath(listFileName, szSpineIconPath);
			if (listFileName.size() > 0)
			{
				strFileName = getFirstFileName(listFileName);
				GameData::getInstance()->setGameIconSpineName(uNameID, strFileName);
			}
		}

		if (!strFileName.empty())
		{
			Vec2 vSpace = Vec2(sSprSize.width / 2, sSprSize.height - 300);
			pSkeAnim = static_cast<spine::SkeletonAnimation *>(Tools::PlaySkeletonAnimation(pBtnIcon, szSpineIconPath, strFileName, 0, vSpace, "skeleton_animation",false));
			m_mapSpines[strFileName] = pSkeAnim;
		}
		

		sprintf(szResoursePath, "Games/%d", uNameID);
		if (!(CheckUpdateResourse(string(szResoursePath), uNameID)))
		{
			pSprIcon->removeFromParent();
		}
		else
		{
			if (pSkeAnim != nullptr)
			{
				pSkeAnim->setVisible(false);
			}
			if (pSprIcon)
			{
				pSprIcon->setVisible(true);
				pSprIcon->setName("nullimgsprite");

				auto dwnImg = ImageView::create("common/gameicon/btn_bg.png");
				pSprIcon->addChild(dwnImg);
				dwnImg->setAnchorPoint(Vec2(1, 0));
				float fDwnImgPosX = sSprSize.width / 2 + dwnImg->getContentSize().width / 2;
				float fDwnImgPosY = sSprSize.height / 2 - dwnImg->getContentSize().height / 2;
				dwnImg->setPosition(Vec2(fDwnImgPosX, fDwnImgPosY));
				dwnImg->setName("dwnImg");

				auto dwnLabel = Label::createWithTTF("0%", "platform/fonts/simhei.ttf", 14, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
				float fDwnLabelPosX = dwnImg->getContentSize().width / 2;
				float fDwnLabelPosY = dwnImg->getContentSize().height / 2;
				dwnLabel->setColor(Color3B(149, 212, 248));
				dwnLabel->setPosition(Vec2(fDwnLabelPosX, fDwnLabelPosY));
				dwnLabel->setName("dwnLabel");
				dwnImg->addChild(dwnLabel);
				dwnLabel->setVisible(false);

				auto dwnSpri = Sprite::create("common/gameicon/btn_download.png");
				dwnSpri->setAnchorPoint(Vec2(0, 0));
				float fDwnSpriPosX = dwnImg->getContentSize().width / 2 - dwnSpri->getContentSize().width / 2;
				float fDwnSpriPosY = dwnImg->getContentSize().height / 2 - dwnSpri->getContentSize().height / 2;
				dwnSpri->setPosition(Vec2(fDwnSpriPosX, fDwnSpriPosY));
				dwnSpri->setName("dwnSpri");
				dwnImg->addChild(dwnSpri);
			}
		}
		pLytGameList->addChild(pBtnIcon);
	}
	
	pLytGameList->setContentSize(Size(fStartX + fRightSpace, ListSize.height));
	m_gameList->pushBackCustomItem(pLytGameList);
}

std::string GameListCompositeV66::getFirstFileName(const std::vector<std::string> & listData, const std::string & strFilter)
{
	std::string strName;
	for (int i = 0; i < listData.size(); ++i)
	{
		std::vector<std::string> listFilter;
		Tools::SplitString(listData[i], listFilter, std::string("/"));

		if (listFilter.size() <= 0)
			continue;

		strName = listFilter[listFilter.size() - 1];
		Tools::SplitString(std::string(strName), listFilter, std::string("."));

		if (listFilter.size() <= 0)
			continue;
		if (listFilter[listFilter.size() - 1].compare(strFilter) == 0)
		{
			strName = listFilter[0];
			break;
		}
			
	}
	return strName;
}

Vec2 GameListCompositeV66::getGameIconPositon(int i, const Size & sCSize, float & fSecondItemHeight, float & fStartX, float & fLastItemWidth)
{
	float fPositionX = 0.0f;
	float fPositionY = 0.0f;
	if (i > 2 && i % 2 != 0)
	{
		float h2 = sCSize.height;
		if (h2 > (fSecondItemHeight / 2))
		{
			fPositionY = h2 / 2 - (h2 - (fSecondItemHeight / 2));
		}
		else if (h2 < (fSecondItemHeight / 2))
		{
			fPositionY = fSecondItemHeight / 2 - h2 + (h2 / 2);
		}

		fPositionY = fSecondItemHeight / 2 + fPositionY;
		fLastItemWidth = sCSize.width;
		fPositionX = fStartX + fLastItemWidth / 2;
		fStartX += fLastItemWidth + 5;
	}
	else if (i > 2 && i % 2 == 0)
	{
		float h3 = sCSize.height;
		if (h3 < fSecondItemHeight / 2)
		{
			fPositionY = -(fSecondItemHeight / 2 - h3 + h3 / 2);
		}
		else if (h3 > fSecondItemHeight / 2)
		{
			fPositionY = -(h3 / 2 - (h3 - fSecondItemHeight / 2));
		}
		fPositionY = fSecondItemHeight / 2 + fPositionY;
		fPositionX = fStartX - fLastItemWidth / 2 - 5;
	}
	else
	{
		fSecondItemHeight = sCSize.height;
		fPositionX = fStartX + sCSize.width / 2;
		fPositionY = fSecondItemHeight / 2;
		fStartX += sCSize.width + 5;
	}
	return Vec2(fPositionX, fPositionY);
}

void GameListCompositeV66::updateGameList(int nType)
{
	m_nSelType = nType;
	if (m_nSelType == 1)
	{
		m_gameList1->setVisible(true);
		m_gameList2->setVisible(false);
		m_gameList3->setVisible(false);
		m_gameList4->setVisible(false);
	}
	else if (m_nSelType == 2)
	{
		m_gameList1->setVisible(false);
		m_gameList2->setVisible(true);
		m_gameList3->setVisible(false);
		m_gameList4->setVisible(false);
	}
	else if (m_nSelType == 3)
	{
		m_gameList1->setVisible(false);
		m_gameList2->setVisible(false);
		m_gameList3->setVisible(true);
		m_gameList4->setVisible(false);
	}
	else if (m_nSelType == 4)
	{
		m_gameList1->setVisible(false);
		m_gameList2->setVisible(false);
		m_gameList3->setVisible(false);
		m_gameList4->setVisible(true);
	}
}
/*
vector<std::string> GameListCompositeV66::getGameIconSpineName(int uNameId)
{
    vector<std::string> spineData;
    switch (uNameId) {
        case 31000503:
             spineData.push_back("hallimg/main/icon/spine/icon_shuihuzhuan");
             spineData.push_back("icon_shuihuzhuan");
             break;
        case 31000504:
            spineData.push_back("hallimg/main/icon/spine/icon_shuiguolaba");
            spineData.push_back("icon_shuiguolaba");
            break;
        case 31000506:
            spineData.push_back("hallimg/main/icon/spine/icon_jiuxianlawang");
            spineData.push_back("icon_jiuxianlawang");
            break;
        case 31000507:
             spineData.push_back("hallimg/main/icon/spine/icon_caishen");
             spineData.push_back("icon_caishen");
             break;
        case 31000508:
             spineData.push_back("hallimg/main/icon/spine/icon_jiangshi");
             spineData.push_back("icon_jiangshi");
             break;
        case 31000512:
            spineData.push_back("hallimg/main/icon/spine/icon_888");
            spineData.push_back("icon_888");
            break;
        case 33003107:
            spineData.push_back("hallimg/main/icon/spine/icon_jinchanbuyu");
            spineData.push_back("icon_jinchanbuyu");
            break;
        case 33003108:
            spineData.push_back("hallimg/main/icon/spine/icon_likuipiyu");
            spineData.push_back("icon_likuipiyu");
            break;
        case 33003109:
            spineData.push_back("hallimg/main/icon/spine/icon_danaotiangong");
            spineData.push_back("icon_danaotiangong");
            break;
        case 11105101:
            spineData.push_back("hallimg/main/icon/spine/icon_duizhanniuniu");
            spineData.push_back("icon_duizhanniuniu");
            break;
        case 11105102:
            spineData.push_back("hallimg/main/icon/spine/icon_sirenniuniu");
            spineData.push_back("icon_sirenniuniu");
            break;
        case 11105103:
            spineData.push_back("hallimg/main/icon/spine/icon_tongbiniuniu");
            spineData.push_back("icon_tongbiniuniu");
            break;
        case 11105100:
            spineData.push_back("hallimg/main/icon/spine/icon_bairenniuniu");
            spineData.push_back("icon_bairenniuniu");
            break;

			//add by qiuyx 20181122
		case 11100200:
			spineData.push_back("hallimg/main/icon/spine/icon_longhudou");
			spineData.push_back("icon_longhudou");
			break;
		case 11100600:
			spineData.push_back("hallimg/main/icon/spine/icon_bairenxiaojiu");
			spineData.push_back("icon_bairenxiaojiu");
			break;
		case 12100004:
			spineData.push_back("hallimg/main/icon/spine/icon_shisanzhang");
			spineData.push_back("icon_shisanzhang");
			break;
		case 12100202:
			spineData.push_back("hallimg/main/icon/spine/icon_shuoha");
			spineData.push_back("icon_shuoha");
			break;
		case 12101106:
			spineData.push_back("hallimg/main/icon/spine/icon_yingsanzhang");
			spineData.push_back("icon_yingsanzhang");
			break;
		case 31000500:
			spineData.push_back("hallimg/main/icon/spine/icon_jinshayinsha");
			spineData.push_back("icon_jinshayinsha");
			break;
		case 31000501:
			spineData.push_back("hallimg/main/icon/spine/icon_benchibaoma");
			spineData.push_back("icon_benchibaoma");
			break;
		case 31000502:
			spineData.push_back("hallimg/main/icon/spine/icon_lingdangyouxi");
			spineData.push_back("icon_lingdangyouxi");
			break;
		case 31000505:
			spineData.push_back("hallimg/main/icon/spine/icon_haizeiwanglaba");
			spineData.push_back("icon_haizeiwanglaba");
			break;
		case 31000509:
			spineData.push_back("hallimg/main/icon/spine/icon_slotomania");
			spineData.push_back("icon_slotomania");
			break;
		case 31000510:
			spineData.push_back("hallimg/main/icon/spine/icon_meidushazhiwen");
			spineData.push_back("icon_meidushazhiwen");
			break;
		case 31000511:
			spineData.push_back("hallimg/main/icon/spine/icon_juediqiusheng");
			spineData.push_back("icon_juediqiusheng");
			break;
		case 31000513:
			spineData.push_back("hallimg/main/icon/spine/icon_wangzherongyao");
			spineData.push_back("icon_wangzherongyao");
			break;
		case 12100404:
			spineData.push_back("hallimg/main/icon/spine/icon_21dian");
			spineData.push_back("icon_21dian");
			break;
		case 33001112:
			spineData.push_back("hallimg/main/icon/spine/icon_weixinhongbao");
			spineData.push_back("icon_weixinhongbao");
			break;
        default:
            break;
    }
    
    return spineData;
}
*/

bool GameListCompositeV66::onTouchBegan(Touch* touch, Event* event) {
   
    return false;
}
void GameListCompositeV66::onTouchEnded(Touch* touch, Event* event) {
   
    return;
}

void GameListCompositeV66::onTouchMoved(Touch* touch, Event* event) {
   
    return;
}

void GameListCompositeV66::selectedItemEventScrollView(Ref * pSender, ui::ScrollView::EventType type)
{
	switch (type)
	{
	case ui::ScrollView::EventType::SCROLL_TO_BOTTOM://滑动到底部
		//CCLOG("SCROLL_TO_BOTTOM");
		break;
	case ui::ScrollView::EventType::SCROLL_TO_TOP://滑动到头部
		//CCLOG("SCROLL_TO_TOP");
		break;
	case ui::ScrollView::EventType::SCROLLING://滑动中;
		if (GameData::getInstance()->isFirstSlideGameListFlag())
		{
			GameData::getInstance()->setFirstSlideGameListFlag(false);
			auto effect_hint = dynamic_cast<spine::SkeletonAnimation*>(Tools::getNodeByName(m_category, "zjm_img_zhixiang"));
			if (nullptr != effect_hint)
			{
				effect_hint->setVisible(false);
			}
		}
		break;
	default:
		break;
	}
}

void GameListCompositeV66::updateGameUserCount(UINT Id, UINT count) {

}

void GameListCompositeV66::closeGameList() {
    iconListMoveOut(true);
    curGameIconMoveIn(true);
}

void GameListCompositeV66::openGameList() {
    iconListMoveOut(false);
    curGameIconMoveIn(false);
    //if (menu) {
        //menu->stopMove(0.10f);
    //}
}

void GameListCompositeV66::openGameTypeList()
{
	ListView* m_gameList;
	if (m_nSelType == 1)
	{
		m_gameList = m_gameList1;
	}
	else if (m_nSelType == 2)
	{
		m_gameList = m_gameList2;
	}
	else if (m_nSelType == 3)
	{
		m_gameList = m_gameList3;
	}
	else if (m_nSelType == 4)
	{
		m_gameList = m_gameList4;
	}
	else
	{
		return;
	}
    m_gameList->removeAllChildren();
    ShowCategory();
}
void GameListCompositeV66::onRoomLoginCallback(bool success, const std::string& message, UINT roomID)
{
    auto isQuickJoin = UserDefault::getInstance()->getBoolForKey(IS_QUICK_JOIN);
    if (success && isQuickJoin) {
        
        HNLOG_WARNING("the user enters a room complete message!");
        ComRoomInfo* roomInfo = RoomInfoModule()->getByRoomID(roomID);
        RoomLogic()->setSelectedRoom(roomInfo);
        RoomLogic()->setRoomRule(roomInfo->dwRoomRule);
        
        if (GameCreator()->getCurrentGameType() == HNGameCreator::NORMAL) {// 金币场不扣积分
            _roomLogic->stop();
            
            _deskLogic = new HNRoomDeskLogic(this);
            
            std::vector<DeskInfo*>    _deskinfos;
            int deskNO = 0;
            for (int i = 0; i < roomInfo->uDeskCount; i++) {
                // 获取桌子玩家信息
                std::vector<UserInfoStruct*> deskUsers;
                UserInfoModule()->findDeskUsers(deskNO, deskUsers);
                
                DeskInfo* desk = new DeskInfo();
                //memset(desk, 0x0, sizeof(desk));
                
                if (nullptr != desk) {
                    desk->deskID = deskNO;
                    desk->multiple = roomInfo->iBasePoint;
                    desk->goldMin = roomInfo->iLessPoint;
                    desk->goldMax = roomInfo->iMaxPoint;
                    desk->peopleValue = deskUsers.size();
                    desk->peopleMax = roomInfo->uDeskPeople;
                    BYTE value = RoomLogic()->deskStation.bVirtualDesk[desk->deskID / 8] & (1 << (desk->deskID % 8));
                    if (0 != value) {
                        desk->peopleValue = roomInfo->uDeskPeople;
                    }
                }
                _deskinfos.push_back(desk);
                
                deskNO++;
            }
            
            //找出人数最多的桌子，如有相同，选择第一张（前提是桌子未坐满）
            //有效的桌子号
            int validPeople = 0;
            int validDeskNo = 0;
            vector<DeskInfo*> DeskInfoVecLocal;
            DeskInfoVecLocal.clear();
            for (DeskInfo* deskInfo : _deskinfos) {
                //桌子未坐满
                if (deskInfo->peopleValue != deskInfo->peopleMax) {
                    DeskInfoVecLocal.push_back(deskInfo);
                }
            }
            
            auto vecSize = DeskInfoVecLocal.size();//从未满的桌子中随机一张
            if (vecSize > 0) {
                auto maxSize =(int)vecSize - 1;
                int rand = random(0,maxSize);
                auto deskInfo = DeskInfoVecLocal.at(rand);
                
                validPeople = deskInfo->peopleValue;
                validDeskNo = deskInfo->deskID;
                
                _deskLogic->start();
                _deskLogic->requestSit(validDeskNo);
            }

            
        } else if (GameCreator()->getCurrentGameType() == HNGameCreator::BR) { //百人
            CommandManager::getInstance()->addLoaderCommand(GetText("LODING_REQUEST_ROOMLIST"));
            _roomLogic->requestSit(0, 0);
        } else if (GameCreator()->getCurrentGameType() == HNGameCreator::SINGLE) {
            CommandManager::getInstance()->addLoaderCommand(GetText("LODING_REQUEST_ROOMLIST"));
            for (int i = 0; i < roomInfo->uDeskCount; i++) {
                std::vector<UserInfoStruct*> deskUsers;
                HNUserInfoModule::getInstance()->findGameUsers(i, deskUsers);
                if (deskUsers.empty()) {
                    _roomLogic->requestSit(i, 0);
                    break;
                }
            }
        } else {
            
        }
    }
    else if(!success && isQuickJoin)
    {
        CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
        CommandManager::getInstance()->showGamePrompt(message);
    }
    
}
//直接从房间进入游戏
void GameListCompositeV66::onRoomSitCallback(bool success, const std::string& message, UINT roomID, BYTE deskNo, BYTE seatNo) {
    CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
    if (success) {
        if (_roomLogic) {
            _roomLogic->stop();
        }
        if (_deskLogic) {
            _deskLogic->stop();
        }
        
        if (INVALID_DESKNO != deskNo && INVALID_DESKSTATION != seatNo) {
            // 启动游戏
            bool ret = GameCreator()->startGameClient(RoomLogic()->getSelectedRoom()->uNameID, deskNo, true);
            if (!ret) {
                CommandManager::getInstance()->showGamePrompt(GBKToUtf8("游戏启动失败。"));
            }
            else
            {
                //启动成功置为false防止游戏退到roomlist有执行快速加入逻辑
                UserDefault::getInstance()->setBoolForKey(IS_QUICK_JOIN, true);
            }
        }
    } else {
        ShowGamePrompt* gamePrompt = (ShowGamePrompt*)CommandManager::getInstance()->get(SHOW_GAME_PROMPT);
        gamePrompt->setText(GBKToUtf8(message.c_str()));
        gamePrompt->setCallBack([=]() {
            if (GameCreator()->getCurrentGameType() == HNGameCreator::SINGLE || GameCreator()->getCurrentGameType() == HNGameCreator::BR) {
                RoomLogic()->closeRoom();
                _roomLogic->stop();
            }
        });
        gamePrompt->execute();
    }
}



void GameListCompositeV66::UpdateTimer(float dt)
{
	m_nPageNum++;
	if (m_nPageNum>=4)
	{
		m_nPageNum = 0;
	}

	//if (!m_pagePoint5 || !m_pagePointb0 || !m_pagePointb1 || !m_pagePointb2)
		//return;

	if (m_nPageNum == 0)
	{
		//m_pagePoint5->setPositionX(m_pagePointb0->getPosition().x+LIUHAI_X_OFFSET);
	}
	else if (m_nPageNum == 1)
	{
		//m_pagePoint5->setPositionX(m_pagePointb1->getPosition().x + LIUHAI_X_OFFSET);
	}
	else if (m_nPageNum == 2)
	{
		//m_pagePoint5->setPositionX(m_pagePointb2->getPosition().x + LIUHAI_X_OFFSET);
	}

	m_hotgameList->scrollToPage(m_nPageNum);
}

bool GameListCompositeV66::decompress(const std::string &zip)
{
	// Find root path for zip file
	size_t pos = zip.find_last_of("/\\");
	if (pos == std::string::npos)
	{
		CCLOG("AssetsManagerEx : no root path specified for zip file %s\n", zip.c_str());
		return false;
	}
	const std::string rootPath = zip.substr(0, pos + 1);

	// Open the zip file
	unzFile zipfile = unzOpen(FileUtils::getInstance()->getSuitableFOpen(zip).c_str());//
	if (!zipfile)
	{
		CCLOG("AssetsManagerEx : can not open downloaded zip file %s\n", zip.c_str());
		return false;
	}

	// Get info about the zip file
	unz_global_info global_info;
	if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
	{
		CCLOG("AssetsManagerEx : can not read file global info of %s\n", zip.c_str());
		unzClose(zipfile);
		return false;
	}

	// Buffer to hold data read from the zip file
	char readBuffer[BUFFER_SIZE];
	// Loop to extract all files.
	uLong i;
	for (i = 0; i < global_info.number_entry; ++i)
	{
		// Get info about current file.
		unz_file_info fileInfo;
		char fileName[MAX_FILENAME];
		if (unzGetCurrentFileInfo(zipfile,
			&fileInfo,
			fileName,
			MAX_FILENAME,
			NULL,
			0,
			NULL,
			0) != UNZ_OK)
		{
			CCLOG("AssetsManagerEx : can not read compressed file info\n");
			unzClose(zipfile);
			return false;
		}


		const std::string fullPath = rootPath + fileName;
		// Check if this entry is a directory or a file.
		const size_t filenameLength = strlen(fileName);
		if (fileName[filenameLength - 1] == '/')
		{
			//There are not directory entry in some case.
			//So we need to create directory when decompressing file entry
			if (!FileUtils::getInstance()->createDirectory(basename(fullPath)))
			{
				// Failed to create directory
				CCLOG("AssetsManagerEx : can not create directory %s\n", fullPath.c_str());
				unzClose(zipfile);
				return false;
			}
		}
		else
		{
			// Entry is a file, so extract it.
			// Open current file.
			if (unzOpenCurrentFile(zipfile) != UNZ_OK)
			{
				CCLOG("AssetsManagerEx : can not extract file %s\n", fileName);
				unzClose(zipfile);
				return false;
			}

			// Create a file to store current file.
			FILE *out = fopen(FileUtils::getInstance()->getSuitableFOpen(fullPath).c_str(), "wb");
			if (!out)
			{
				CCLOG("AssetsManagerEx : can not create decompress destination file %s\n", fullPath.c_str());
				unzCloseCurrentFile(zipfile);
				unzClose(zipfile);
				return false;
			}

			// Write current file content to destinate file.
			int error = UNZ_OK;
			do
			{
				error = unzReadCurrentFile(zipfile, readBuffer, BUFFER_SIZE);
				if (error < 0)
				{
					CCLOG("AssetsManagerEx : can not read zip file %s, error code is %d\n", fileName, error);
					fclose(out);
					unzCloseCurrentFile(zipfile);
					unzClose(zipfile);
					return false;
				}

				if (error > 0)
				{
					fwrite(readBuffer, error, 1, out);
				}
			} while (error > 0);

			fclose(out);
		}

		unzCloseCurrentFile(zipfile);

		// Goto next entry listed in the zip file.
		if ((i + 1) < global_info.number_entry)
		{
			if (unzGoToNextFile(zipfile) != UNZ_OK)
			{
				CCLOG("AssetsManagerEx : can not read next file for decompressing\n");
				unzClose(zipfile);
				return false;
			}
		}
	}

	unzClose(zipfile);
	if (remove(zip.c_str()) != 0)
	{
		CCLOG("can not remove downloaded zip file %s", zip.c_str());
	}

	return true;
}

std::string GameListCompositeV66::basename(const std::string& path)
{
	size_t found = path.find_last_of("/\\");

	if (std::string::npos != found)
	{
		return path.substr(0, found);
	}
	else
	{
		return path;
	}
}
