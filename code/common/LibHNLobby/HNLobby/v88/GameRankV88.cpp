#include "GameRankV88.h"
#include "PlatformResV88.h"
#include "HNLobby/v88/BottomMenuCompositeV88.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/FontConfig.h"
#include "HNLobby/GamePrompt.h"
#include "HNUIExport.h"
#include <string>
#include "GUI/CCScrollView/CCTableView.h"
#include "UI/LoadingLayer.h"
#include "UI/UserHead.h"

#define WINSIZE Director::getInstance()->getWinSize()

#define DEFOUT_Y 6.0
#define DEFOUT_X 0.0f
#define PAGE_SIZE 20

static const INT    RANK					= 10;
static const INT    NICKNAME				= 11;
static const INT    MONEY					= 12;
static const float  ViewMoveOffset          = 10.0f;

static const char *RANK_BG				= "v88/module/paihangbang/bg.png";
static const char* RANK_FIRST_ICON		= "v88/module/paihangbang/paiming/1.png";
static const char* RANK_SECOND_ICON		= "v88/module/paihangbang/paiming/2.png";
static const char* RANK_THIRD_ICON      = "v88/module/paihangbang/paiming/3.png";
static const char* RANK_OTHERBG_ICON    = "v88/module/paihangbang/paiming/4.png";

static const char* RANK_MYFRAME	        = "platform/ranking/myFrame.png";

GameRankV88::GameRankV88() {
    _toutleRecord = 0;
	_rankType = 0;
	_canClose = false;
    m_parent = nullptr;
}

GameRankV88::~GameRankV88() {
    PlatformLogic()->removeEventSelector(MDM_GP_PAIHANGBANG, 0);
	HttpClient::getInstance()->destroyInstance();
}

void GameRankV88::onExit()
{
	HNLayer::onExit();

	HttpClient::getInstance()->destroyInstance();
}

bool GameRankV88::init() {
    if (!HNLayer::init()) return false;
    
    MyRankNum = -1;
	_isClose = true;
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(GameRankV88::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameRankV88::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    //排行榜的背景框
    rankingFrame = ImageView::create(RANK_BG);
    if (rankingFrame) {
        rankingFrame->setTouchEnabled(true);
        rankingFrame->setAnchorPoint(Point(0.5, 0.5));
        this->addChild(rankingFrame);
        this->setContentSize(rankingFrame->getContentSize());
    }

	Sprite* bg = Sprite::create("v88/module/paihangbang/btnBg.png");
	Sprite* on = Sprite::create("v88/module/paihangbang/5.png");
	Sprite* off = Sprite::create("v88/module/paihangbang/6.png");
	Sprite* thumb = Sprite::create("v88/module/paihangbang/btnLight.png");
	pControl = ControlSwitch::create(bg, on, off, thumb, Label::createWithTTF("", DEFAULT_FONT, 16), Label::createWithTTF("", DEFAULT_FONT, 16));
	pControl->addTargetWithActionForControlEvents(this, cccontrol_selector(GameRankV88::ControlSwitchCallBack), Control::EventType::VALUE_CHANGED);
	pControl->setPositionY(rankingFrame->getContentSize().height*0.5f - bg->getContentSize().height*0.5f - 10);
	//pControl->setTouchEnabled(false);
	pControl->setEnabled(false);
	this->addChild(pControl);

	_btnTitle = Sprite::create("v88/module/paihangbang/totle.png");
	_btnTitle->setPosition(Vec2(thumb->getContentSize().width*0.5f, thumb->getContentSize().height*0.5f));
	thumb->addChild(_btnTitle);
	for (int i = 0; i < 2; i++)
	{
		auto size = Size(rankingFrame->getContentSize().width - 10, rankingFrame->getContentSize().height - 82);
		auto _listView = ListView::create();
		_listView->setInnerContainerSize(size);
		_listView->setContentSize(size);
		_listView->setAnchorPoint(Vec2(0.5, 0.5));
		_listView->setOpacity(100);
		_listView->setPositionY(-35);
		_listView->setItemsMargin(1.9);
		_listView->setScrollBarAutoHideEnabled(false);
		_listView->setBounceEnabled(true);
		_listView->setVisible(false);
		_listView->addTouchEventListener(CC_CALLBACK_2(GameRankV88::listViewItemCallBack, this));
		this->addChild(_listView);

		_vecListview.push_back(_listView);
	}
	pControl->setOn(false);

    return true;
}

void GameRankV88::canTouchTime(float dt)
{
	_canClose = true;
	unschedule(schedule_selector(GameRankV88::canTouchTime));
}

void GameRankV88::ControlSwitchCallBack(Ref* pSender, Control::EventType event)
{
	if (event == Control::EventType::VALUE_CHANGED)
	{
		HttpClient::getInstance()->destroyInstance();

		ControlSwitch* controlSwitch = (ControlSwitch*)pSender;
		bool isMusicOpen = controlSwitch->isOn();
		if (isMusicOpen)
		{
			_rankType = 1;
			_btnTitle->setTexture("v88/module/paihangbang/day.png");
			_vecListview.at(1)->setVisible(true);
			_vecListview.at(0)->setVisible(false);
			_vecListview.at(1)->jumpToTop();
			_vecListview.at(1)->removeAllChildren();

		}
		else
		{
			_btnTitle->setTexture("v88/module/paihangbang/totle.png");
			_vecListview.at(0)->setVisible(true);
			_vecListview.at(1)->setVisible(false);
			_vecListview.at(0)->jumpToTop();
			_vecListview.at(0)->removeAllChildren();
			_rankType = 0;
		}
		int idx = isMusicOpen ? 0 : 1;
		_iList.clear();
		_veMsg.clear();
		pageIdx = 1;
		getRankListData(_rankType);
	}
}


void GameRankV88::open(Node* parent, Vec2 vec, int zorder, int tag) {
	CCAssert(nullptr != parent, "perParent is nullptr");

	m_parent = parent;
    auto size = Size(700, 620);
    if (rankingFrame) {
        size = rankingFrame->getContentSize();
    }
    this->setPosition(Vec2(-size.width / 2 + 5, WIN_SIZE.height / 2));
	m_parent->getParent()->addChild(this, zorder, tag);
	this->runAction(Sequence::create(MoveBy::create(0.3f, Vec2(size.width + ViewMoveOffset, 0)), DelayTime::create(1), CallFunc::create([=](){
		_canClose = true;
	}),nullptr));
    if (m_parent->getChildByName(btn_rank_tag)) {
        m_parent->getChildByName(btn_rank_tag)->runAction(MoveBy::create(0.3f, Vec2(size.width + ViewMoveOffset, 0)));
    }
    //防止二级界面上打开头像信息
    UserDefault::getInstance()->setBoolForKey(TOUCH_HEAD, false);
}

void GameRankV88::close() {
	if (_canClose)
	{
		if (_isClose) {
			this->runAction(Sequence::create(MoveBy::create(0.2, Vec2(-rankingFrame->getContentSize().width - ViewMoveOffset - 5, 0)), DelayTime::create(0.6f), CallFunc::create([=](){
				this->removeFromParent();
			}), nullptr));
            if (m_parent && m_parent->getChildByName(btn_rank_tag)) {
                m_parent->getChildByName(btn_rank_tag)->runAction(MoveBy::create(0.2, Vec2(-rankingFrame->getContentSize().width - ViewMoveOffset, 0)));
            }
			_isClose = false;
			UserDefault::getInstance()->setBoolForKey(TOUCH_HEAD, true);
		}
	}
}

bool GameRankV88::onTouchBegan(Touch *touch, Event *unused_event) {
    return true;
}

void GameRankV88::onTouchEnded(Touch *touch, Event *unused_event) {
    close();
}

void GameRankV88::listViewItemCallBack(Ref* pSender, Widget::TouchEventType type) {
    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::ENDED:
    case cocos2d::ui::Widget::TouchEventType::CANCELED: {
        if (_vecListview.at(_rankType)->getInnerContainer()->getPositionY() > 0) {
            if (_toutleRecord == _iList.size())
                return;
			getRankListData(_rankType);
            break;
        }
    }
    default:
        break;
    }
}

//获取排行数据
void GameRankV88::getRankListData(int reqType) {
    MSG_GP_PaiHangBang_In rank;
    rank.count = 80;
    rank.type = 0;
    rank.curPage = pageIdx;
    rank.reqType = 0;
	rank.paiHangBangType = reqType;
    rank.pageSize = PAGE_SIZE;
    PlatformLogic()->sendData(MDM_GP_PAIHANGBANG, 0, &rank, sizeof(rank),
                              HN_SOCKET_CALLBACK(GameRankV88::openRankListEventSelector, this));
}

//申请打开排行榜回调
bool GameRankV88::openRankListEventSelector(HNSocketMessage* socketMessage) {
	if (ERR_GP_LIST_PART == socketMessage->messageHead.bHandleCode) {

		MSG_GP_MoneyPaiHangBang_Item *nameInfo = (MSG_GP_MoneyPaiHangBang_Item *)socketMessage->object;
		INT Count = socketMessage->objectSize / sizeof(MSG_GP_MoneyPaiHangBang_Item);
		Count = Count < 0 ? 0 : Count;

		cocos2d::log("count>%d", Count);
		int i = 0;
		while (i < Count)
		{
			_veMsg.push_back(nameInfo[i]);
			i++;
		}
	}
	else if (ERR_GP_LIST_FINISH == socketMessage->messageHead.bHandleCode){
	{
			MSG_GP_MoneyPaiHangBang_Result *nameInfo = (MSG_GP_MoneyPaiHangBang_Result *)socketMessage->object;
			INT Count = socketMessage->objectSize / sizeof(ComNameInfo);
			Count = Count < 0 ? 0 : Count;

			cocos2d::log("count>%d", Count);
			int i = 0;
			while (nameInfo->items[i].i64Money > 0)
			{
				_veMsg.push_back(nameInfo->items[i]);
				i++;
			}

			std::string name(PlatformLogic()->loginResult.nickName);
			for (int i = 0; i < _veMsg.size(); i++) {
				if (_iList.size() == _veMsg.at(i).totalRecord) {
					LoadingLayer::removeLoading(this);
					return false;
				}
				if (pageIdx >_veMsg.at(i).curPage) {
					LoadingLayer::removeLoading(this);
					return false;
				}
				_toutleRecord = _veMsg.at(i).totalRecord;
				if (_veMsg.at(i).i64Money > 0) {
					_iList.push_back(_veMsg.at(i));
					addItem(i + (_veMsg.at(i).curPage - 1) *PAGE_SIZE);
				}

				if (name.compare(_veMsg.at(i).nickName) == 0) {
					MyRankNum = i + 1;
				}
			}
			_vecListview.at(_rankType)->requestDoLayout();

			if (pageIdx == 1)
			{
				_vecListview.at(_rankType)->jumpToTop();
			}
			else
			{
				auto container = _vecListview.at(_rankType)->getInnerContainer();
				auto cha = _vecListview.at(_rankType)->getContentSize().height - container->getContentSize().height;
				auto percent = (1 - (float)PAGE_SIZE / _vecListview.at(_rankType)->getChildrenCount()) * 100;
				_vecListview.at(_rankType)->jumpToPercentVertical(percent - 5);
			}

			pageIdx++;
			_veMsg.clear();
			schedule(schedule_selector(GameRankV88::canTouchTime), 4.0);
		};
	}else 
	{
        GamePromptLayer::create()->showPrompt(GBKToUtf8("查询失败,稍后查询!"));
    }
    return true;
}

//添加Cell
TableViewCell* GameRankV88::tableCellAtIndex(TableView *table, ssize_t idx) {
    TableViewCell* cell = table->dequeueCell();
    if (!cell) {
        cell = new TableViewCell();
        cell->autorelease();
    }

    cell->removeAllChildrenWithCleanup(true);

    if (idx >= _iList.size()) return cell; 
    Layout *storeItemTemplate = Layout::create();
    storeItemTemplate->setContentSize(Size(900, 30));
    storeItemTemplate->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    auto posY = storeItemTemplate->getPositionY();
    storeItemTemplate->setPositionY(posY);
    float itemWidth = storeItemTemplate->getContentSize().width;
    float itemHeight = storeItemTemplate->getContentSize().height;

    auto iter = _iList.begin();
    MSG_GP_MoneyPaiHangBang_Item ite = *(iter + idx);
    char str[64];
    //玩家排名
    if (idx == 0) {
        auto first = Sprite::create(RANK_FIRST_ICON);
        first->setScale(0.8f);
        first->setPosition(Vec2(itemWidth * 0.2f - 1.0, itemHeight * 0.5f));
        storeItemTemplate->addChild(first, 1);
    } else if (idx == 1) {
        auto second = Sprite::create(RANK_SECOND_ICON);
        second->setScale(0.8f);
        second->setPosition(Vec2(itemWidth * 0.159f - 1.0, itemHeight * 0.5f - 3));
        storeItemTemplate->addChild(second, 1);
    } else if (idx == 2) {
        auto third = Sprite::create(RANK_THIRD_ICON);
        third->setScale(0.8f);
        third->setPosition(Vec2(itemWidth * 0.159f- 1.0, itemHeight * 0.5f - 3));
        storeItemTemplate->addChild(third, 1);
    } else {
        sprintf(str, "%ld", idx + 1);
        rankingText = Text::create(str, gFontFile, 28);
        rankingText->setColor(FONT_COLOR_PINK_V88);
        rankingText->setPosition(Vec2(itemWidth * 0.159f -1.0, itemHeight * 0.5f - 3));
        storeItemTemplate->addChild(rankingText, 1, RANK);
    }

    //玩家昵称
    sprintf(str, "%s", ite.nickName);
    auto nickNameText = Text::create(ForceGBKToUtf8(str), gFontFile, 28);
    nickNameText->setColor(FONT_COLOR_PINK_V88);
    nickNameText->setPosition(Vec2(itemWidth * 0.5f - 5.0, itemHeight * 0.5f));
    storeItemTemplate->addChild(nickNameText, 1, NICKNAME);
    //玩家金币
    //ssprintf(str, "%lld", ite.i64Money);
    auto moneyText = Label::createWithTTF(Tools::fourSeparateMoney(ite.i64Money), gFontFile, 28);
    moneyText->setColor(FONT_COLOR_PINK_V88);
    moneyText->setHorizontalAlignment(TextHAlignment::LEFT);
    moneyText->setVerticalAlignment(TextVAlignment::TOP);
    moneyText->setAnchorPoint(Vec2(0, 0.5));
    moneyText->setPosition(Vec2(itemWidth * 0.755f + 5.0 ,itemHeight * 0.5f));
    storeItemTemplate->addChild(moneyText , 1 , MONEY);

    cell->addChild(storeItemTemplate);

//     if (idx<3) {
//         nickNameText->setColor(colorWhite);
//         moneyText->setColor(colorWhite);
//     }
    return cell;

}

//设置Cell大小
Size GameRankV88::tableCellSizeForIndex(TableView *table, ssize_t idx) {
    return Size( 1090, 50);
}

void GameRankV88::addItem(INT idx) {
	if (_iList.size() == 0)
		return;

	if (_vecListview.at(_rankType)->getItem(idx))
	{
		auto iter = _iList.begin();
		MSG_GP_MoneyPaiHangBang_Item ite = *(iter + idx);
		auto layout = static_cast<Layout*>(_vecListview.at(_rankType)->getChildByTag(idx));
		auto line = static_cast<Sprite*>(layout->getChildByName("line"));

		auto nickNameText = static_cast<Label*>(line->getChildByName("nickNameText"));
		if (nickNameText)
		{
			char str[64];
			sprintf(str, "%s", ite.nickName);
			nickNameText->setString(ForceGBKToUtf8(str));
		}

		auto moneyText = static_cast<Label*>(line->getChildByName("moneyText"));
		if (moneyText)
		{
			moneyText->setString(Tools::fourSeparateMoney(ite.i64Money));
		}

		auto userHead = static_cast<UserHead*>(line->getChildByName("m_HeadImage"));
		if (userHead)
		{
			UserInfoStruct user;
			memset(&user, 0, sizeof(user));
			user.bLogoID = ite.logoId;
			user.dwUserID = ite.userId;
			memcpy(user.szWXTouXiang, ite.szWXTouXiang, sizeof(CHAR)* 200);
			userHead->setHeadWithInfo(&user);
		}
		return;
	}
	auto bg = Sprite::create(RANK_BG);
	auto size = bg->getContentSize();

	Layout *storeItemTemplate = Layout::create();
	storeItemTemplate->setTag(idx);
	storeItemTemplate->setContentSize(Size(size.width, (int)size.height / 7));
	storeItemTemplate->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//	float itemWidth = storeItemTemplate->getContentSize().width;
//	float itemHeight = storeItemTemplate->getContentSize().height;

	auto line = Sprite::create("v88/module/paihangbang/1.png");
	line->setName("line");
	line->setPosition(Vec2(9, storeItemTemplate->getContentSize().height*0.5f));
	line->setAnchorPoint(Vec2(0, 0.5));
	storeItemTemplate->addChild(line, 1, MONEY);

	auto iter = _iList.begin();
	MSG_GP_MoneyPaiHangBang_Item ite = *(iter + idx);
	char str[64];
	//玩家排名
	if (idx == 0) {
		auto first = Sprite::create(RANK_FIRST_ICON);
		first->setAnchorPoint(Vec2(0, 0.5));
		first->setPosition(Vec2(5, first->getContentSize().height*0.5f - 8));
		line->addChild(first, 1);
	}
	else if (idx == 1) {
		auto second = Sprite::create(RANK_SECOND_ICON);
		second->setAnchorPoint(Vec2(0, 0.5));
		second->setPosition(Vec2(5, second->getContentSize().height*0.5f - 8));
		line->addChild(second, 1);
	}
	else if (idx == 2) {
		auto third = Sprite::create(RANK_THIRD_ICON);
		third->setAnchorPoint(Vec2(0, 0.5));
		third->setPosition(Vec2(5, third->getContentSize().height*0.5f - 8));
		line->addChild(third, 1);
	}
	else {
		sprintf(str, "%d", idx + 1);

		auto third = Sprite::create(RANK_OTHERBG_ICON);
		third->setAnchorPoint(Vec2(0, 0.5));
		third->setPosition(Vec2(5, third->getContentSize().height*0.5f - 2));
		line->addChild(third, 1);

		auto atlas = LabelAtlas::create(str, "v88/module/paihangbang/7.png", 28, 37, '0');
		atlas->setAnchorPoint(Vec2(0.5, 0.5));
		atlas->setColor(FONT_COLOR_PINK_V88);
		atlas->setPosition(Vec2(third->getContentSize().width*0.5f, third->getContentSize().height*0.5f));
		third->addChild(atlas, 1, RANK);
	}

	/*uesrHead*/
	auto loginResult = PlatformLogic()->getInstance()->loginResult;

	auto m_HeadImage = UserHead::create("v88/module/gerenxinxi/4.png");
	m_HeadImage->setName("m_HeadImage");
	m_HeadImage->setPosition(line->getContentSize().width * 0.22, line->getContentSize().height*0.5f - 2);
	m_HeadImage->setLocalZOrder(1);
	m_HeadImage->setScale(0.45f);
	m_HeadImage->setClipUserHeadWithFile(DEFAULT_PLAYER_HEAD_V88, "v88/module/gerenxinxi/5.png");

	line->addChild(m_HeadImage, 1, MONEY);
	UserInfoStruct user;
	memset(&user, 0, sizeof(user));
	user.bLogoID = ite.logoId;
	user.dwUserID = ite.userId;
	memcpy(user.szWXTouXiang, ite.szWXTouXiang,sizeof(CHAR)*200);
	m_HeadImage->setHeadWithInfo(&user);

    //玩家昵称
    sprintf(str, "%s", ite.nickName);
	auto nickNameText = Label::createWithTTF(ForceGBKToUtf8(str), "platform/fonts/simhei.ttf", 22);
	nickNameText->setName("nickNameText");
	nickNameText->setAnchorPoint(Vec2(0, 0.5));
	nickNameText->setHorizontalAlignment(TextHAlignment::LEFT);
	nickNameText->setVerticalAlignment(TextVAlignment::TOP);
	nickNameText->setPosition(Vec2(line->getContentSize().width * 0.29, line->getContentSize().height*0.5f));
    line->addChild(nickNameText, 1, NICKNAME);

	char moneyStr[64] = { 0 };
	if (ite.i64Money >= 100000000)
	{
		sprintf(moneyStr, "%0.1f亿", ite.i64Money * 1.0f / 100000000);
	}
	else if (ite.i64Money >= 10000)
	{
		sprintf(moneyStr, "%0.1f万", ite.i64Money * 1.0f / 10000);
	}
	else
	{
		sprintf(moneyStr, "%lld", ite.i64Money);
	}

    //玩家金币
	auto moneyText = Label::createWithTTF(GBKToUtf8(moneyStr), "platform/fonts/simhei.ttf", 22);
    moneyText->setColor(Color3B(255,237,55));
	moneyText->setName("moneyText");
    moneyText->setHorizontalAlignment(TextHAlignment::LEFT);
    moneyText->setVerticalAlignment(TextVAlignment::TOP);
    moneyText->setAnchorPoint(Vec2(0.5, 0.5));
	moneyText->setPosition(Vec2(line->getContentSize().width * 0.87, line->getContentSize().height*0.5f));
    line->addChild(moneyText, 1, MONEY);
	_vecListview.at(_rankType)->addChild(storeItemTemplate);
//     if (idx < 3) {
//         nickNameText->setColor(colorWhite);
//         moneyText->setColor(colorWhite);
//     }
}

//设置Cell个数
ssize_t GameRankV88::numberOfCellsInTableView(TableView *table) {
    return _iList.size();
}

void GameRankV88::scrollViewDidScroll(extension::ScrollView* view) {
    if (view->getContentOffset().y > 5) {
		_canClose = false;
        getRankListData(_rankType);
        return;
    }
}

//Cell触摸事件
void GameRankV88::tableCellTouched(TableView *table, TableViewCell *cell) {
    log("cell touched at index %d", cell->getIdx());
}

void GameRankV88::showMyRank() {
    Size winSize = Director::getInstance()->getWinSize();

    //我的排名的背景框
    auto myItemTemplate = ui::Scale9Sprite::create(RANK_MYFRAME);
    myItemTemplate->setPreferredSize(Size(1180, 80));
    myItemTemplate->setAnchorPoint(Vec2(0.5, 1));
    myItemTemplate->setPosition(Vec2(rankingFrame->getContentSize().width/2, -10));
    float myrankWidth = myItemTemplate->getContentSize().width;
    float myrankHeight = myItemTemplate->getContentSize().height;
    rankingFrame->addChild(myItemTemplate, 10);

    //我的头像
    auto myHead = Sprite::create(PlatformLogic()->loginResult.bBoy?USER_MEN_HEAD:USER_WOMEN_HEAD);
    myHead->setScale(0.8f);
    myHead->setPosition(0.06f*myrankWidth,myrankHeight/2);
    myItemTemplate->addChild(myHead, 1);
    /*auto myHeadFrame = Sprite::create(MY_HEAD_FRAME);
    myHeadFrame->setScale(0.76);
    myHeadFrame->setPosition(0.06f*myrankWidth,myrankHeight/2);
    myItemTemplate->addChild(myHeadFrame, 1);*/
    //我的排名
    auto other = Sprite::create("platform/ranking/hua1.png");
    other->setPosition(Vec2(myrankWidth * 0.128f, myrankHeight * 0.5f));
    myItemTemplate->addChild(other, 1);
    char str[64];
    if (MyRankNum > 0 && MyRankNum <= 20) {
        sprintf(str, "%d", MyRankNum);
        auto myrankingText = Text::create(str, gFontFile, 28);
        myrankingText->setPosition(Vec2(myrankWidth * 0.13f, myrankHeight * 0.5f));
        myItemTemplate->addChild(myrankingText, 1, RANK);
    } else {
        auto myrankingText = Text::create("20+", gFontFile, 28);
        myrankingText->setPosition(Vec2(myrankWidth * 0.13f, myrankHeight * 0.5f));
        myItemTemplate->addChild(myrankingText, 1, RANK);
    }

    std::string nickName(PlatformLogic()->loginResult.nickName);
    if (!nickName.empty()) {
        sprintf(str, "%s",GBKToUtf8(PlatformLogic()->loginResult.nickName));
    } else {
        sprintf(str, "%s",GBKToUtf8("未知"));
    }
    auto myNameText = Text::create(str, gFontFile, 28);
    myNameText->setPosition(Vec2(myrankWidth * 0.47f, myrankHeight * 0.5f));
    myItemTemplate->addChild(myNameText, 1, NICKNAME);

    sprintf(str, "%lld", PlatformLogic()->loginResult.i64Money);
    auto mymoneyText = Text::create(str, gFontFile, 28);
    mymoneyText->setPosition(Vec2(myrankWidth * 0.825f, myrankHeight * 0.5f));
    myItemTemplate->addChild(mymoneyText , 1 , MONEY);
}
