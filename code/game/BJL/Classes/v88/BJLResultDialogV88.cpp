#include "BJLResultDialogV88.h"
#include "cocostudio/CocoStudio.h"
#include "HNLobby/FontConfig.h"
#include "HNLobby/CommonMacro.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;

ResultDialogV88::ResultDialogV88()
    : _dealerWin(0)
    , _myWin(0) {

}

ResultDialogV88::~ResultDialogV88() {

}

ResultDialogV88* ResultDialogV88::create(Node* parent, LLONG dealerWin, LLONG myWin) {
	ResultDialogV88* ptr = new ResultDialogV88();
    if (nullptr !=ptr && ptr->init(parent,dealerWin,myWin)) {
        ptr->autorelease();
        return ptr;
    }
    CC_SAFE_DELETE(ptr);
    return nullptr;
}

bool ResultDialogV88::init(Node* parent, LLONG dealerWin, LLONG myWin) {
    if (!HNLayer::init()) {
        return false;
    }

    _dealerWin = dealerWin;
    _myWin = myWin;

// 	auto distpatch = Director::getInstance()->getEventDispatcher();
// 	auto listener = EventListenerTouchOneByOne::create();
// 	listener->onTouchBegan = CC_CALLBACK_2(ResultDialogV99::onTouchBegan,this);
// 	listener->setSwallowTouches(true);
// 	distpatch->addEventListenerWithSceneGraphPriority(listener,this);

    auto bg = ImageView::create("BJL/game/settlement/game_result.png");
    bg->setPosition(Vec2(WIN_SIZE.width / 2 - 105, WIN_SIZE.height / 2 + 100));
    this->addChild(bg);

    char str[50] = { 0 };

    // 庄家输赢
    sprintf(str, "%lld", _dealerWin);
    auto dealerScore = Label::createWithBMFont("BJL/game/settlement/number.fnt", str);
    dealerScore->setAnchorPoint(Vec2(0, 0.5f));
    dealerScore->setPosition(365, 72);
    dealerScore->setScale(0.7f);
    bg->addChild(dealerScore);

    // 玩家输赢
    sprintf(str, "%lld", _myWin);
    auto myScore = Label::createWithBMFont("BJL/game/settlement/number.fnt", str);
    myScore->setAnchorPoint(Vec2(0, 0.5f));
    myScore->setPosition(365, 207);
    myScore->setScale(0.7f);
    bg->addChild(myScore);

    return true;
}

void ResultDialogV88::initUI() {
// 	auto bg = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(_widget,"bg"));
// 	auto size = bg->getContentSize();
// 	char str[50]={0};
//
// 	// 庄家输赢
// 	sprintf(str,"%lld",_dealerWin);
// 	auto dealerScore = Label::createWithBMFont("BJL/game/settlement/gametable/number.fnt", str);
// 	dealerScore->setAnchorPoint(Vec2(0.5f , 0.5f));
// 	dealerScore->setPosition(size.width * 0.5 , size.height * 0.5 - 80);
// 	dealerScore->setScale(0.7f);
// 	_widget->addChild(dealerScore);
//
// 	// 玩家输赢
// 	sprintf(str,"%lld", _myWin);
// 	auto myScore = Label::createWithBMFont("BJL/game/settlement/gametable/number.fnt", str);
// 	myScore->setAnchorPoint(Vec2(0.5f, 0.5f));
// 	myScore->setPosition(size.width * 0.5 , size.height * 0.5 + 50);
// 	myScore->setScale(0.7f);
// 	_widget->addChild(myScore);

}

bool ResultDialogV88::onTouchBegan(Touch *touch, Event *unused_event) {
    return false;
}
