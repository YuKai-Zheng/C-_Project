#include "BRNNBankerLayerV88.h"
#include "BRNNMessageHead.h"
#include "BRNNMsgV88.h"
#include "FontSize.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "BRNNActionsV88.h"
#include "cocostudio/ActionTimeline/CSLoader.h"


namespace BRNN {




BankerLayerV88::BankerLayerV88() {}

BankerLayerV88::~BankerLayerV88() {}

BankerLayerV88 *BankerLayerV88::create(vecTmpBankerInfoV88 vecInfo, LLONG limitMoney,
                                 bool bMeIn) {
    BankerLayerV88 *ret = new BankerLayerV88();
    if (ret && ret->init(vecInfo, limitMoney, bMeIn)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool BankerLayerV88::init(vecTmpBankerInfoV88 vecInfo, LLONG limitMoney, bool bMeIn) {
    if (!Layer::init())
        return false;

    _vecInfo = vecInfo;
    _limitMoney = limitMoney;
    _bMeIn = bMeIn;

	auto node = CSLoader::createNode("csb/v66/BRNN/BankerList.csb");
	addChild(node);

	Sprite* bg = (Sprite*)node->getChildByName("background");
	this->setContentSize(bg->getContentSize());

	node->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height / 2));

	Button* xiazhuangBtn = (Button*)node->getChildByName("xiazhuang");
	xiazhuangBtn->addTouchEventListener(CC_CALLBACK_2(BankerLayerV88::handleXiazhuang, this));
	xiazhuangBtn->setPressedActionEnabled(true);

	bankerListViewItem = (ListView*)node->getChildByName("list");



	showList();

	if (!_bMeIn) {
		applyBanker();
	}
	return true;
}

void BankerLayerV88::handleXiazhuang(Ref *pSender, Widget::TouchEventType type) {
	if (Widget::TouchEventType::ENDED != type/* || !_isTouch*/)
		return;

	SHANG_ZHUANG dealer;
	dealer.station = 0;
	dealer.shang = false;
	dealer.bIsAi = false;
	dealer.iQiangZhuangMoney = HNPlatformLogic::getInstance()->loginResult.i64Money;
	sendData(MDM_GM_GAME_NOTIFY, ASS_SHANG_ZHUANG, (char *)&dealer, sizeof(SHANG_ZHUANG));
	HNAudioEngine::getInstance()->playEffect("platform/sound/sound_close.mp3");
	//this->actionOut();
	BRNN::ActionsV88::actionOut(this);
}

void BankerLayerV88::closeBankerLayer() {
	//HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
	//colorLayer->removeFromParent();
	this->runAction(Sequence::create(ScaleTo::create(0.1f, 0.3f), CallFunc::create([&]() {
		this->removeFromParent();
	}), nullptr));
}

bool BankerLayerV88::onTouchBegan(Touch *touch, Event *event)
{
	Vec2 pos = touch->getLocation();

	if (!this->getBoundingBox().containsPoint(pos))
	{
		BRNN::ActionsV88::actionOut(this);
		return false;
	}

	return true;
}

void BankerLayerV88::applyBanker() {
    LLONG myOwnMoney = HNPlatformLogic::getInstance()->loginResult.i64Money;
    if (myOwnMoney >= _limitMoney) {
        SHANG_ZHUANG dealer;
        dealer.station = 0;
        dealer.shang = true;
        dealer.bIsAi = false;
        dealer.iQiangZhuangMoney = myOwnMoney;
        sendData(MDM_GM_GAME_NOTIFY, ASS_SHANG_ZHUANG, (char *)&dealer,
                 sizeof(SHANG_ZHUANG));
    } else {
        //金币不足 无法上庄
        MsgTipV88::create(
            StringUtils::format(GetText("limit_money_applied_banker"), _limitMoney)
            .c_str());
    }
	
}





void BankerLayerV88::setBankerList(vecTmpBankerInfoV88 list) {
	this->_vecInfo = list;

	if (bankerListViewItem){
		bankerListViewItem->removeAllItems();
		showList();
	}
}
#define ITEM_SIZE Size(225,25)

void BankerLayerV88::showList() {
	for (int i = 0; i < _vecInfo.size(); i++){
		Node *item = createItem(_vecInfo[i].name);

		Layout *default_item = Layout::create();
		default_item->setTouchEnabled(true);
		default_item->setContentSize(ITEM_SIZE);
		default_item->addChild(item);
		bankerListViewItem->pushBackCustomItem(default_item);
	}
}

Node* BankerLayerV88::createItem(std::string name) {
	Node* item = Node::create();

	auto username = Label::createWithTTF(ForceGBKToUtf8(name.c_str()), DEFAULT_FONT, FONT_SIZE_25);
	username->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	username->setPosition(bankerListViewItem->getContentSize().width / 2, -ITEM_SIZE.height / 2);
	item->addChild(username);

	/*auto sprite = Sprite::create("v88/BJL/game_user_bg.png");
	sprite->setPosition(Vec2(bankerListViewItem->getContentSize().width / 2, -ITEM_SIZE.height));
	sprite->setScaleX(bankerListViewItem->getContentSize().width / sprite->getContentSize().width);
	item->addChild(sprite);*/

	return item;
}


void BankerLayerV88::onBtnClick(Ref *pSender, Control::EventType event) {
    auto btn = (ControlButton *)pSender;
    LLONG myOwnMoney = HNPlatformLogic::getInstance()->loginResult.i64Money;
    if (em_upBtn == btn->getTag()) {
        if (myOwnMoney >= _limitMoney) {
            SHANG_ZHUANG dealer;
            dealer.station = 0;
            dealer.shang = true;
            dealer.bIsAi = false;
            dealer.iQiangZhuangMoney = myOwnMoney;
            sendData(MDM_GM_GAME_NOTIFY, ASS_SHANG_ZHUANG, (char *)&dealer,
                     sizeof(SHANG_ZHUANG));
            //this->moveSelfWithAction();
			BRNN::ActionsV88::actionOut(this);
        } else {
            //金币不足 无法上庄
            MsgTipV88::create(StringUtils::format(GetText("limit_money_applied_banker"),
                                               _limitMoney)
                           .c_str());
        }
    } else if (em_downBtn == btn->getTag()) {
        SHANG_ZHUANG dealer;
        dealer.station = 0;
        dealer.shang = false;
        dealer.bIsAi = false;
        dealer.iQiangZhuangMoney = myOwnMoney;
        sendData(MDM_GM_GAME_NOTIFY, ASS_SHANG_ZHUANG, (char *)&dealer,
                 sizeof(SHANG_ZHUANG));
		//this->moveSelfWithAction();
		BRNN::ActionsV88::actionOut(this);
    }
}
}
