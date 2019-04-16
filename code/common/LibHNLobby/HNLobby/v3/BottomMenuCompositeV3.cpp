#include "BottomMenuCompositeV3.h"
#include "HNLobby/PersionalCenter/GameUserDataLayer.h"
#include "HNLobby/ChildLayer/GameOnlineReward.h"
#include "HNLobby/ChildLayer/GameRechargeRebate.h"
#include "HNLobby/PlatformConfig.h"
#include "HNLobby/GamePrompt.h"
#include "../pattern/CommandManager.h"
#include "PlatformResV3.h"
#include "../ChildLayer/GameBankLayer.h"
#include "../ChildLayer/GameStoreLayer.h"
#include "../ChildLayer/GameRankingList.h"
#include "Tool/Tools.h"
#include "Sound/HNAudioEngine.h"

#define btn_bank_tag "btn_bank"
#define btn_charge_tag "btn_charge"
#define btn_rank_tag "btn_rank"


BottomMenuCompositeV3::BottomMenuCompositeV3() {
}

BottomMenuCompositeV3::~BottomMenuCompositeV3() {
    unscheduleUpdate();
}

bool BottomMenuCompositeV3::init() {
    if (!Node::init()) {
        return false;
    }

    // 玩家信息结构体
    MSG_GP_R_LogonResult &LogonResult = PlatformLogic()->loginResult;
    m_money = LogonResult.i64Money;
    _lobbyBotttom.node = CSLoader::createNode(PLATFORM_BOTTOM_MENU_V3);
    //_lobbyBotttom.node->setPosition(Vec2(WIN_SIZE.width* 0.5f, 0));
    addChild(_lobbyBotttom.node, PLATFORM_UI_ZORDER_TOP);

    _lobbyBotttom.layout = (Layout *)_lobbyBotttom.node->getChildByName("Panel_bottom");

    if (nullptr != _lobbyBotttom.layout) {
        char str[100] = { 0 };
        sprintf(str, "%s%d.png", "common/head/", LogonResult.bLogoID);
        _lobbyBotttom.UserHead_Head = UserHead::create(str);
        _lobbyBotttom.UserHead_Head->setPositionX(WIN_SIZE.width * 0.1);

        if (switchToLayoutControl(_lobbyBotttom.layout, "Text_headPos",
                                  _lobbyBotttom.UserHead_Head)) {
            // char str[100] = { 0 };
            // sprintf(str, "%s%d.png", "common/head/", LogonResult.bLogoID);
            //_lobbyBotttom.UserHead_Head->setUserHead(str);
            _lobbyBotttom.UserHead_Head->setScale(0.6f);
            _lobbyBotttom.UserHead_Head->onHeadClickEvent = [this](UserHead *sender,
            Vec2 touchVec) {
                GameUserDataLayer *userDataLayer = dynamic_cast<GameUserDataLayer *>(
                                                       this->getChildByName("GameUserDataLayer"));
                if (nullptr == userDataLayer) {
                    userDataLayer = GameUserDataLayer::create();
                    userDataLayer->openUserLayer(this->getParent(), Vec2(WIN_SIZE.width, 0),
                                                 CHILD_LAYER_ZORDER,
                                                 CHILD_LAYER_BANK_TAG);

                    userDataLayer->upDateHeadCallBack = [this]() -> void {
                        char str[100] = { 0 };
                        sprintf(str, "%s%d.png", "common/head/",
                                PlatformLogic()->getInstance()->loginResult.bLogoID);
                        _lobbyBotttom.UserHead_Head->setTexture(str);
                    };
                    userDataLayer->upDateNickCallBack = [this](string str)->void{
                        _lobbyBotttom.Label_UserName->setString(str);
                    };
                }
            };
        }

        auto headFram = Sprite::create("v3/xiakuang/touxiangkuang.png");
        headFram->setPosition(Vec2(_lobbyBotttom.UserHead_Head->getPosition().x, _lobbyBotttom.UserHead_Head->getPosition().y + 16 ));
        _lobbyBotttom.layout->addChild(headFram);

        //用户id
        _lobbyBotttom.Label_UserID = (Text *)Helper::seekWidgetByName(_lobbyBotttom.layout, "Text_id");

        char buffer[64] = { 0 };
        sprintf(buffer, GBKToUtf8("%u"), LogonResult.dwUserID);
        _lobbyBotttom.Label_UserID->setString(buffer);

        // 用户昵称
        _lobbyBotttom.Label_UserName = (Text *)Helper::seekWidgetByName(_lobbyBotttom.layout, "Text_nick");
        std::string nickName(LogonResult.nickName);
        if (!nickName.empty()) {
            _lobbyBotttom.Label_UserName->setString(ForceGBKToUtf8(LogonResult.nickName));
        } else {
            _lobbyBotttom.Label_UserName->setString(GBKToUtf8("未知"));
        }

        // 用户金币
        _lobbyBotttom.Label_UserMoney = (Text *)Helper::seekWidgetByName(_lobbyBotttom.layout, "Text_money");
        _lobbyBotttom.Label_UserMoney->setString(Tools::fourSeparateMoney(LogonResult.i64Money));
    }
    scheduleUpdate();
    return true;

    return true;
}

void BottomMenuCompositeV3::moveOut(bool isOut) {
    int k = isOut ? -1 : 1;
    if (!isOut) {
        this->runAction(Sequence::create(MoveBy::create(0.2f, Vec2(0, _lobbyBotttom.layout->getContentSize().height * k * 1.4)), nullptr));
    } else {
        this->runAction(Sequence::create(MoveBy::create(0.2f, Vec2(0, _lobbyBotttom.layout->getContentSize().height * k * 1.4)), nullptr));
    }
}

void BottomMenuCompositeV3::autoMove() {
    auto moveBy = MoveBy::create(0.3f, Vec2(0, _lobbyBotttom.layout->getContentSize().height * -1 * 1.1));
    auto reverseMoveBy = moveBy->reverse();
    auto delay = DelayTime::create(0.1f);
    _lobbyBotttom.layout->runAction(Sequence::create(moveBy, reverseMoveBy, nullptr));
}

void BottomMenuCompositeV3::setType(MenuType type) {

}

void BottomMenuCompositeV3::setLogoTexture(std::string filename) {

}

void BottomMenuCompositeV3::setShowBank(bool isShow) {

}

void BottomMenuCompositeV3::setGameDeskLayer(GameDesk* gameDesk) {

}

void BottomMenuCompositeV3::setRoomInfo(ComRoomInfo* roomInfo) {

}

void BottomMenuCompositeV3::update(float dt) {
    // 玩家信息结构体
    MSG_GP_R_LogonResult &LogonResult = PlatformLogic()->loginResult;
    if (m_money != LogonResult.i64Money) {
        m_money = LogonResult.i64Money;
        _lobbyBotttom.Label_UserMoney->setString(to_string(m_money));
    }
}


void BottomMenuCompositeV3::controlChargeAndBankBtnPosition(bool openCharge, bool openBank, bool openrank, bool forceHide /*= false*/) {

}

//void BottomMenuCompositeV3::upDatePlatformMoney(Node * platformText)
//{
//	if (platformText != nullptr)
//	{
//		auto text = static_cast<Text*>(platformText);
//		//text->setString();
//	}
//}

void BottomMenuCompositeV3::walletChanged(LLONG money) {
//    if (nullptr != _lobbyBotttom.Label_UserMoney) {
//        char buffer[32];
//        sprintf(buffer, "%lld", money);
//        char xin[100] = { 0 }; // 逗号分隔
//        {
//            if (strlen(buffer) > 4) {
//                int wei = strlen(buffer) % 4 > 0 ? strlen(buffer) % 4 : 4;
//                int j = 0;
//                int idx = 0;
//                for (int i = 0; i < strlen(buffer); i++) {
//                    if (wei == 0) {
//                        if (idx++ == 3) {
//                            xin[j++] = ',';
//                            idx = 0;
//                        }
//                        xin[j++] = buffer[i];
//                    } else {
//                        if (idx++ == wei) {
//                            xin[j++] = ',';
//                            wei = 3;
//                            idx = 0;
//                        }
//                        xin[j++] = buffer[i];
//                    }
//                }
//                _lobbyBotttom.Label_UserMoney->setString(xin);
//            } else {
//                _lobbyBotttom.Label_UserMoney->setString(buffer);
//            }
//        }
//    }
}

void BottomMenuCompositeV3::menuLobbyBottomCallBack(Ref *pSender,Widget::TouchEventType type) {
    if (Widget::TouchEventType::ENDED != type/* || !_isTouch*/)
        return;

    auto btn = (Button *)pSender;
    _isTouch = false;
    btn->runAction(Sequence::create(DelayTime::create(0.5f), CCCallFunc::create([=]() {
        _isTouch = true;
    }),
    nullptr));

    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);

    std::string name(btn->getName());

    if (name.compare(btn_bank_tag) == 0) { //银行
        auto bankLayer = GameBankLayer::createGameBank();
        bankLayer->setName("GameBankLayer");
        bankLayer->setChangeDelegate(this);
        bankLayer->openBank(this->getParent(), Vec2(WIN_SIZE.width, 0), CHILD_LAYER_ZORDER, CHILD_LAYER_BANK_TAG);

    } else if (name.compare(btn_charge_tag) == 0) { //商店
        auto storeLayer = GameStoreLayer::createGameStore(this, PlatformConfig::getInstance()->funcConfig.isOpenBankFunc);
        storeLayer->setChangeDelegate(this);
        storeLayer->openStore(this->getParent(), Vec2(WIN_SIZE.width, 0), CHILD_LAYER_ZORDER, CHILD_LAYER_STORE_TAG);
        storeLayer->onCloseCallBack = [this]() {
            CommandManager::getInstance()->execute("CheckGiftCommand");
        };

    } else if (name.compare(btn_rank_tag) == 0) { //排行榜
        auto rankLayer = GameRankingList::create();
        rankLayer->openRank(this->getParent(), Vec2(WIN_SIZE.width, 0), CHILD_LAYER_ZORDER, CHILD_LAYER_RANK_TAG);

    } else {
        //GamePromptLayer::create()->showPrompt(GBKToUtf8("程序猿使劲开发中。"));
    }
}

bool BottomMenuCompositeV3::switchToLayoutControl(Layout *layout, const std::string &controlName, Node *newControl) {
    auto tmp = (Text *)Helper::seekWidgetByName(layout, controlName);
    if (nullptr == tmp)
        return false;
    newControl->setPosition(tmp->getPosition());
    newControl->setTag(tmp->getTag());
    newControl->setLocalZOrder(tmp->getLocalZOrder());
    newControl->setScaleX(tmp->getScaleX());
    newControl->setScaleY(tmp->getScaleY());
    newControl->setAnchorPoint(tmp->getAnchorPoint());
    layout->addChild(newControl);
    return true;
}