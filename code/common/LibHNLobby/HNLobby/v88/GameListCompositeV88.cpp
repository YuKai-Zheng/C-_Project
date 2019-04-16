#include "GameListCompositeV88.h"
#include "UI/LoadingLayer.h"

#include "cocos2d.h"
#include "HNLobby/GameRoom.h"
#include "HNLobby/GamePrompt.h"
#include "../pattern/CommandManager.h"
#include "PlatformResV88.h"
#include "../PlatformDefine.h"
#include "Sound/HNAudioEngine.h"
#include "../GameData.h"
USING_NS_CC;

//item vec(202,232)
//list width = 4*202 + 3*20 height = 2*232 + 20 + 2*3

const char* PLATFORM_LEFT_GIRL_V88 = "v88/animation/main/girl/girl.csb";
const char* PLATFORM_LEFT_HOTGAME_V88 = "v88/animation/main/zhutui/zhutui.csb";

const char* PAGE_TAG_ICON_CURRENT_V88	= "v88/main/17.png";
const char* PAGE_TAG_ICON_OTHER_V88		= "v88/main/16.png";

const float MAIN_ICON_SCALE_V88 = 0.62f;

const int	HAI_ZEI_WANG_LA_BA_V88 = 99999995;
const int	SAN_QI_LA_BA_V88 = 99999996;
const int	POINT_21_V88 = 99999997;
const int	SHUI_HU_ZHUANG_V88 = 99999998;
const int	HUAN_LE_MA_JIANG_V88 = 99999999;

//多余需要漏出来部分
#define GameTypeWidth 70
#define fOtherSpace  40
#define	ListSize Size(730 + fOtherSpace, 460)
#define ItemSize Size(400, 360)
#define DesignItemSize Size(ItemSize.width*MAIN_ICON_SCALE_V88, ItemSize.height*MAIN_ICON_SCALE_V88)

GameListCompositeV88::GameListCompositeV88() :
    _iValue(-1) {
}

void GameListCompositeV88::onEnter()
{
	Node::onEnter();

	//快速加入
//	bottomMenuDelegate->quickJoinGame = [&](){
//		int iGameID = 0;
//		iGameID = UserDefault::getInstance()->getIntegerForKey(QUICK_JOIN_GAME_ID);
//
//		bool hasGame = false;
//		Button* targetBtn = nullptr;
//		for (auto btn : m_vButtonItem)
//		{
//			if (btn->getTag() == iGameID)
//			{
//				hasGame = true;
//				targetBtn = btn;
//				break;
//			}
//		}
//
//
//		if (iGameID <= 0 || !hasGame)
//		{
//			//请求下来的游戏信息
//			std::map<std::string, std::vector<int>> mapServerGames = GameData::getInstance()->getGameListData();
//			vector<int> vQuick = mapServerGames.at(QUICK_START_TYPE);
//			if (vQuick.size() > 0)
//			{
//				iGameID = vQuick.at(0);
//			}
//			else if (m_vButtonItem.size() > 0)
//			{
//				iGameID = m_vButtonItem.at(0)->getTag();
//			}
//			else
//			{
//				CommandManager::getInstance()->showGamePrompt(GBKToUtf8("快速启动失败！"));
//				return;
//			}
//		}
//
//		if (targetBtn)
//		{
//			enterGameCallback(targetBtn, Widget::TouchEventType::ENDED);
//			//放按钮回调后面区分是否是quick join
//			UserDefault::getInstance()->setBoolForKey(IS_QUICK_JOIN, true);
//		}
//	};
}

bool GameListCompositeV88::init() {
    if (!Node::init()) {
        return false;
    }
    
    _isTouch = false;

	//是否显示两个假的游戏展示
	//if (!Tools::isiOSReview())
	//{
	//	vRealGames.push_back(SHUI_HU_ZHUANG_V88);
	//	vRealGames.push_back(HUAN_LE_MA_JIANG_V88);
	//}

    //csb
	createCategory();

	m_gameList = ListView::create();
	m_gameList->setScrollBarEnabled(false);
	m_gameList->setDirection(ui::ListView::Direction::HORIZONTAL);
	m_gameList->setBounceEnabled(true);
	m_gameList->setContentSize(ListSize);
	m_gameList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	m_gameList->setPosition(Vec2(WIN_SIZE.width - GameTypeWidth, WIN_SIZE.height / 2));
	addChild(m_gameList);

	//进入的分类
	int nType = UserDefault::getInstance()->getIntegerForKey(ENTER_GAME_LIST_POS);
	btnGameTypeCallback(m_vButtonCagegory.at(nType), Widget::TouchEventType::ENDED);
    
    //70右侧分类 90排行榜
	Vec2 iconPos = Vec2((WIN_SIZE.width - ListSize.width - GameTypeWidth + 90) / 2, WIN_SIZE.height / 2);
    
    auto curGameIconBg = ImageView::create("v88/main/icon/bg.png");
    curGameIconBg->setName("curGameIconBg");
    curGameIconBg->setPosition(iconPos);
    curGameIconBg->setScale(0.9f);
    addChild(curGameIconBg);
    
	//左侧动画
	auto aniCsb = CSLoader::createNode("v88/animation/main/zhutui/zhutui.csb");
	aniCsb->setPosition(curGameIconBg->getContentSize()/2);
    //aniCsb->setScale(0.85f);
	curGameIconBg->addChild(aniCsb);

	auto action = CSLoader::createTimeline(PLATFORM_LEFT_HOTGAME_V88);
	aniCsb->runAction(action);
	action->play("zhutui", true);
    
    /////////////
    
    auto iconSpr = Sprite::create();
    iconSpr->setName("LeftIconSprite");
    iconSpr->setPosition(curGameIconBg->getPosition());
    addChild(iconSpr);
    
    //推荐游戏
	vector<int> hotGame = GameData::getInstance()->getValidGames(4);
	if (hotGame.size() == 0)
	{
		hotGame.push_back(TWO_NN_ID);
	}
			
	char path[100] = { 0 };
	sprintf(path, "v88/main/icon/%d.png", hotGame.at(0));
	iconSpr->setTexture(path);

	auto hotGameBtn = Button::create(path);
	hotGameBtn->setName("HotGameBtn");
	hotGameBtn->setPressedActionEnabled(true);
	hotGameBtn->setTouchEnabled(true);
	hotGameBtn->setContentSize(curGameIconBg->getContentSize());
	hotGameBtn->setPosition(curGameIconBg->getPosition());
	hotGameBtn->setTag(hotGame.at(0));
	addChild(hotGameBtn);
	hotGameBtn->addTouchEventListener(CC_CALLBACK_2(GameListCompositeV88::enterGameCallback, this));
    
    return true;
}

void GameListCompositeV88::pageCallback(Ref* pSender, cocos2d::ui::PageViewEventType type) {
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
					string str = i == curPageIndex ? PAGE_TAG_ICON_CURRENT_V88 : PAGE_TAG_ICON_OTHER_V88;
					icon->loadTexture(str);
					icon->setPositionX(fWidth);

					fWidth += icon->getContentSize().width + 5;
				}
			}
		}
	}
}

void GameListCompositeV88::enterGameCallback(Ref* pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) return;

    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);

    if(_isTouch) return;
    
    _isTouch = true;
    runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([&](){
        _isTouch = false;
    }), nullptr));
    
    int gameID = ((Button*)pSender)->getTag();

	if (gameID >= HAI_ZEI_WANG_LA_BA_V88)
	{
		CommandManager::getInstance()->showGamePrompt(GBKToUtf8("即将开放，敬请期待"));
		return;
	}

	//快速加入数据
//	UserDefault::getInstance()->setBoolForKey(IS_QUICK_JOIN, false);
//	UserDefault::getInstance()->setIntegerForKey(QUICK_JOIN_GAME_ID, gameID);

    GameCreator()->setCurrentGame(gameID);
    //更新单前游戏的房间信息
    RoomInfoModule()->updateRoomsForTheGame(gameID);
    
    //CommandManager::getInstance()->addLoaderCommand(GetText("LODING_REQUEST_ROOMLIST"));
    _gameListLogic->stop();
    platformDelegate->setLayerType(ROOMLIST);
}

void GameListCompositeV88::btnGameTypeCallback(Ref* pSender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;

	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);

	Button* btn = dynamic_cast<Button*>(pSender);
	int nType = btn->getTag();

	//记录分类
	UserDefault::getInstance()->setIntegerForKey(ENTER_GAME_LIST_POS, nType);

	btnTouchEffect(nType);
	updateGameList(nType);
}

void GameListCompositeV88::requestGameRoomTimerCallBack(float dt) {
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

void GameListCompositeV88::iconListMoveOut(bool bOut)
{
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

void GameListCompositeV88::curGameIconMoveIn(bool bIn)
{
    auto hotGameBtn = dynamic_cast<Button*>(getChildByName("HotGameBtn"));
    hotGameBtn->setTouchEnabled(!bIn);
    hotGameBtn->setVisible(!bIn);
    
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
            sprintf(temp, "v88/main/icon/%d.png", gameID);
            leftIcon->setTexture(temp);
        }
    }
}

void GameListCompositeV88::createPageIcon(int page)
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
// 		string str = i == 0 ? PAGE_TAG_ICON_CURRENT_V88 : PAGE_TAG_ICON_OTHER_V88;
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

void GameListCompositeV88::createCategory()
{
	auto categoryBg = ImageView::create("v88/main/page/8.png");
	categoryBg->setName("CategoryBg");
	categoryBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	categoryBg->setPosition(Vec2(WIN_SIZE.width, WIN_SIZE.height/2));
	addChild(categoryBg);

	Size bgSize = categoryBg->getContentSize();

	char str[64] = { 0 };
	for (int i = 0; i < 4; i++)
	{
		Button* btn = Button::create();
		sprintf(str, "v88/main/page/%d.png", i*2);
		btn->loadTextureNormal(str);
		sprintf(str, "v88/main/page/%d.png", i * 2 + 1);
		btn->loadTexturePressed(str);
		btn->loadTextureDisabled(str);

		btn->setTag(i);
		btn->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
		btn->setPosition(Vec2(bgSize.width-1, bgSize.height - 11 - i*btn->getContentSize().height));
		btn->addTouchEventListener(CC_CALLBACK_2(GameListCompositeV88::btnGameTypeCallback, this));
		categoryBg->addChild(btn);

		m_vButtonCagegory.push_back(btn);
	}
}

void GameListCompositeV88::btnTouchEffect(int tag)
{
	for (auto child : m_vButtonCagegory)
	{
		child->setTouchEnabled(tag != child->getTag());
		child->setBright(tag != child->getTag());
	}
}

void GameListCompositeV88::updateGameList(int nType)
{
	m_gameList->removeAllChildren();

	vRealGames = GameData::getInstance()->getValidGames(nType);
	int nGameCount = vRealGames.size();
	
	int nCountOfcol = 2;
	int nCountOfRow = 3;
	int nCountOfPage = 6;
	float fXSpace = (ListSize.width - fOtherSpace - DesignItemSize.width*nCountOfRow) / (nCountOfRow - 1);
	float fYSpace = (ListSize.height - DesignItemSize.height*nCountOfcol) / (nCountOfcol - 1);


	int nCol = vRealGames.size() / 2;
	if (vRealGames.size() % 2 != 0)
	{
		nCol++;
	}
	auto layout = Layout::create();
	m_gameList->pushBackCustomItem(layout);
	layout->setContentSize(Size(nCol*DesignItemSize.width + fXSpace*(nCol - 1), nCountOfcol*DesignItemSize.height + fYSpace*(nCountOfcol - 1)));

	float fStartX = 0;
	float fStartY = 0;

	char tempStr[32] = { 0 };
	for (int i = 0; i < vRealGames.size(); i++)
	{
		int firstRowCount = vRealGames.size() > 6 ? nCol : 3;
		int colIndex = (i >= firstRowCount) ? (i - firstRowCount) : i;
		fStartX = colIndex*(DesignItemSize.width + fXSpace);
		int rowIndex = (i >= firstRowCount) ? 2 : 1;
		fStartY = (rowIndex == 1) ? layout->getContentSize().height : (layout->getContentSize().height - DesignItemSize.height - fYSpace);
		
		UINT uNameID = vRealGames.at(i);
		sprintf(tempStr, "v88/main/icon/%d.png", uNameID);

		auto buttonItem = Button::create(tempStr);
		buttonItem->setScale(MAIN_ICON_SCALE_V88);
		buttonItem->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		buttonItem->setPosition(Vec2(fStartX, fStartY));
		buttonItem->addTouchEventListener(CC_CALLBACK_2(GameListCompositeV88::enterGameCallback, this));
		buttonItem->setTag(uNameID);
		layout->addChild(buttonItem);

		if (uNameID >= HAI_ZEI_WANG_LA_BA_V88)
		{
			auto willOpenTip = ImageView::create("v88/main/jijiangkaifang.png");
			willOpenTip->setPosition(buttonItem->getPosition() + Size(buttonItem->getContentSize().width*MAIN_ICON_SCALE_V88 / 2, -buttonItem->getContentSize().height*MAIN_ICON_SCALE_V88 / 2 + 25));
			layout->addChild(willOpenTip);
		}
	}
}

void GameListCompositeV88::updateGameUserCount(UINT Id, UINT count) {

}

void GameListCompositeV88::closeGameList() {
    iconListMoveOut(true);
    curGameIconMoveIn(true);
}

void GameListCompositeV88::openGameList() {
    iconListMoveOut(false);
    curGameIconMoveIn(false);
}
