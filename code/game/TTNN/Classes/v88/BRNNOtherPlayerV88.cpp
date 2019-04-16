#include "BRNNOtherPlayerV88.h"
#include "BRNNPlayerSeatV88.h"
#include "BRNNGiveGiftV88.h"
#include "UI/Base/BTableView.h"
#include "HNLobby/v66/PlatformResV66.h"

namespace BRNN {
OtherPlayerV88* OtherPlayerV88::create(std::vector</*UserInfoStruct*/char*> vecOthers) {
    OtherPlayerV88* ret = new OtherPlayerV88();
    if (ret && ret->init(vecOthers))
        ret->autorelease();
    else
        CC_SAFE_DELETE(ret);
    return ret;
}

OtherPlayerV88::OtherPlayerV88()
    : _scrollview(nullptr) {
    _vecOthers.clear();
}

OtherPlayerV88::~OtherPlayerV88() {
}

bool OtherPlayerV88::init(std::vector</*UserInfoStruct**/char*> vecOthers) {
    if (!MakeBlackV88::init())
        return false;

    _vecOthers = vecOthers;
    //auto tmp = Sprite::create(BRNN_NO_SEAT_BG);
    //auto layerSize = Size(tmp->getContentSize().width*7, tmp->getContentSize().height*3);
    //this->setContentSize(layerSize);

    this->setIgnoreAnchorPointForPosition(false);

	m_noSeatBg = Sprite::create(BRNN_NO_SEAT_BG_V88);
	this->setContentSize(m_noSeatBg->getContentSize());
	m_noSeatBg->setPosition(getContentSize().width / 2 + 555, getContentSize().height / 2 + 5);
	this->addChild(m_noSeatBg);

	/*auto title = Sprite::create(BRNN_NO_SEAT_TITLE);
    title->setName("title");
    title->setPosition((getContentSize().width - title->getContentSize().width)/2+title->getContentSize().width/2,getContentSize().height-65);
	addChild(title);*/

	/*vector<char*>     tmpVecOthers = _vecOthers;
    BTableView* m_pTableView = BTableView::createWithSlider(BRNN_NO_SEAT_SCROLL_BAR_BG, BRNN_NO_SEAT_SCROLL_BAR_FOREBG, BRNN_NO_SEAT_SCROLL_BAR_BALL, [tmpVecOthers](TableView* table, ssize_t idx) -> TableViewCell* {
        //if (!tmpVecOthers.empty()) {
        TableViewCell * Cell = table->dequeueCell();
        if (!Cell) {
            Cell = new TableViewCell();
            Cell->autorelease();
        }
        Cell->removeAllChildrenWithCleanup(true);

        //UserInfoStruct* info = new UserInfoStruct();
        //info->bLogoID = 1;
        //info->i64Money = 152;
		//i表示一行有几个么？
        for (unsigned int i = 0; i < 5; i++) {
            if ((i + idx * 5)< tmpVecOthers.size()) {
				char* info;
                info = tmpVecOthers.at(i + idx*5);
                auto userSeat = PlayerSeat::create(i, info);
                userSeat->setPosition((userSeat->getContentSize().width + 20)*i + userSeat->getContentSize().width / 2, userSeat->getContentSize().height / 2);
                Cell->addChild(userSeat);
            }
        }
        return Cell;
        //}
    });

    m_pTableView->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    m_pTableView->setPosition(Vec2(20, 30));
    m_pTableView->setTableCellSize(Size(565, 145));
    m_pTableView->setViewSize(Size(565, 145 * 2+47));
    m_pTableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    m_pTableView->setProgressOpacity(0);
    int tmp = tmpVecOthers.size() / 5 + (tmpVecOthers.size() % 5>0 ? 1 : 0);
    m_pTableView->setNumberOfCellsInTableView(tmp);
    m_pTableView->reloadData();
    m_pTableView->setSliderScaleY(0.7f);
    m_pTableView->getSlider()->setValue(0.0f);
    this->addChild(m_pTableView);*/
    //////////////////////////////////////////////////////////////////////////

	//无座玩家层（只显示名字）
	vector<char*>     tmpVecOthers = _vecOthers;
	
	ui::ListView* m_noSeatPlayer = ui::ListView::create();
	m_noSeatPlayer->setDirection(ui::ScrollView::Direction::VERTICAL);
	m_noSeatPlayer->setBackGroundImageScale9Enabled(true);
	m_noSeatPlayer->setContentSize(Size(140, 240));
	m_noSeatPlayer->setAnchorPoint(Vec2(0, 0));
	m_noSeatPlayer->setPosition(Vec2(m_noSeatBg->getContentSize().width / 3 - 40, m_noSeatBg->getContentSize().height / 15 - 13));
	m_noSeatPlayer->setScrollBarEnabled(false);
	m_noSeatBg->addChild(m_noSeatPlayer);
	

	for (int i = 0; i < tmpVecOthers.size(); i++)
	{
		auto pPlayerName = Label::createWithTTF("", FONT_TTF_MSYH, FONT_SIZE_15);
		pPlayerName->setPosition(Vec2(m_noSeatBg->getContentSize().width / 3 + 15, m_noSeatBg->getContentSize().height / 15 + 40));
		pPlayerName->setSystemFontSize(20);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		pPlayerName->setString(ForceGBKToUtf8(tmpVecOthers.at(i)));
#else
		pPlayerName->setString(GBKToUtf8(tmpVecOthers.at(i)));
#endif


		auto layerOut = ui::Layout::create();
		layerOut->setContentSize(Size(pPlayerName->getContentSize().width + 30, pPlayerName->getContentSize().height));
		layerOut->addChild(pPlayerName);

		m_noSeatPlayer->pushBackCustomItem(layerOut);
	}

    //auto title = Label::create(GBKToUtf8("无座玩家"), "", 28);
    //title->setColor(Color3B(255,255,0));
    //title->setPosition(layerSize.width/2, layerSize.height*6/7);
    //this->addChild(title);

    ////计算slayer高度
    //int row = _vecOthers.size()/6;
    //if(0 != _vecOthers.size()%6)
    //    row = row + 1;

    //auto slayer = Layer::create();
    //slayer->setContentSize(Size(tmp->getContentSize().width*6, tmp->getContentSize().height*row));
    //slayer->setAnchorPoint(Vec2(0, 1));
    //slayer->setPosition(0, tmp->getContentSize().height*2);
    //slayer->setTag(100);

    //int userCnt = 0;
    //for(int i=0; i<row; i++) {
    //    for(int j=0; j<6; j++) {
    //        if(userCnt >= _vecOthers.size())
    //            break;

    //        auto userSeat = PlayerSeat::create(i, _vecOthers[userCnt]);
    //        userSeat->setPosition(slayer->getContentSize().width*(2*j+1)/12, slayer->getContentSize().height*(2*(row-1-i)+1)/(2*row));
    //        userSeat->setTag(userCnt);
    //        slayer->addChild(userSeat);
    //        userCnt++;
    //    }
    //}

    //_scrollview = cocos2d::extension::ScrollView::create(Size(tmp->getContentSize().width*6, tmp->getContentSize().height*2));
    //_scrollview->setAnchorPoint(Vec2(0.5, 0.5));
    //_scrollview->setIgnoreAnchorPointForPosition(false);
    //_scrollview->setPosition(this->getContentSize().width/2, this->getContentSize().height*1.2/3);
    //_scrollview->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    //_scrollview->setContainer(slayer);
    //_scrollview->setContentOffset(ccp(0, -tmp->getContentSize().height*2*(row-2)/2.0));
    //this->addChild(_scrollview);

    return true;
}

void OtherPlayerV88::onTouchEnded(Touch* touch, Event* event) {
    if(_bSelfMoved)
        return;

    //Point point = touch->getLocation();
    //auto slayer = _scrollview->getChildByTag(100);
    //if(slayer && !_bMoved) {
    //    point = slayer->convertToNodeSpace(point);
    //    Vector<Node*> childrenList;
    //    childrenList.clear();
    //    childrenList = slayer->getChildren();
    //    for(auto child : childrenList) {
    //        if(child) {
    //            if(child->getBoundingBox().containsPoint(point)) {
    //                GiveGift::create(_vecOthers[child->getTag()]);
    //            }
    //        }
    //    }
    //}
	
}

void OtherPlayerV88::setTitleImage(const char* imageStr) {
    //auto title = (Sprite*)getChildByName("title");
    //title->setTexture(imageStr);
}

}
