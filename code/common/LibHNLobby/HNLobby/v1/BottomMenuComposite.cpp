#include "BottomMenuComposite.h"
#include "HNLobby/PersionalCenter/GameUserDataLayer.h"
#include "HNLobby/ChildLayer/GameOnlineReward.h"
#include "HNLobby/ChildLayer/GameRechargeRebate.h"
#include "HNLobby/GamePrompt.h"
//#include "UMeng/Cocos2dx/Common/CCUMTypeDef.h"
#include "../pattern/CommandManager.h"
//#include "UMeng/Cocos2dx/Common/CCUMSocialSDK.h"

//USING_NS_UM_SOCIAL;

BottomMenuComposite::BottomMenuComposite() {
}

bool BottomMenuComposite::init() {
    if (!Node::init()) {
        return false;
    }

    // 玩家信息结构体
    MSG_GP_R_LogonResult &LogonResult = PlatformLogic()->loginResult;

    _lobbyBotttom.node = CSLoader::createNode(PLATFORMUI_BOTTOM_CSB);
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
            _lobbyBotttom.UserHead_Head->setScale(0.3f);
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

        //用户id
        _lobbyBotttom.Label_UserID =
            (Text *)Helper::seekWidgetByName(_lobbyBotttom.layout, "Text_id");

        char buffer[64] = { 0 };
        sprintf(buffer, GBKToUtf8("%u"), LogonResult.dwUserID);
        _lobbyBotttom.Label_UserID->setString(buffer);

        // 用户昵称
        _lobbyBotttom.Label_UserName =
            (Text *)Helper::seekWidgetByName(_lobbyBotttom.layout, "Text_nick");
        std::string nickName(LogonResult.nickName);
        if (!nickName.empty()) {
            _lobbyBotttom.Label_UserName->setString(
                ForceGBKToUtf8(LogonResult.nickName));
        } else {
            _lobbyBotttom.Label_UserName->setString(GBKToUtf8("未知"));
        }

        // 用户金币
        _lobbyBotttom.Label_UserMoney =
            (Text *)Helper::seekWidgetByName(_lobbyBotttom.layout, "Text_money");
        _lobbyBotttom.Label_UserMoney->setString(
            Tools::fourSeparateMoney(LogonResult.i64Money));
    }
    scheduleUpdate();
    return true;
}

void BottomMenuComposite::moveOut(bool isOut) {
    int k = isOut ? -1 : 1;
    if (!isOut) {
        _lobbyBotttom.layout->runAction(Sequence::create(MoveBy::create(0.3f, Vec2(0, _lobbyBotttom.layout->getContentSize().height * k * 1.1)), nullptr));
    } else {
        _lobbyBotttom.layout->runAction(Sequence::create(MoveBy::create(0.3f, Vec2(0, _lobbyBotttom.layout->getContentSize().height * k * 1.1)), nullptr));
    }
}

void BottomMenuComposite::autoMove() {
    auto moveBy = MoveBy::create(0.3f, Vec2(0, _lobbyBotttom.layout->getContentSize().height * -1 * 1.1));
    auto reverseMoveBy = moveBy->reverse();
    auto delay = DelayTime::create(0.1f);
    _lobbyBotttom.layout->runAction(Sequence::create(moveBy, reverseMoveBy, nullptr));
}

void BottomMenuComposite::walletChanged(LLONG money) {
    if (nullptr != _lobbyBotttom.Label_UserMoney) {
        char buffer[32];
        sprintf(buffer, "%lld", money);
        char xin[100] = { 0 }; // 逗号分隔
        {
            if (strlen(buffer) > 4) {
                int wei = strlen(buffer) % 4 > 0 ? strlen(buffer) % 4 : 4;
                int j = 0;
                int idx = 0;
                for (int i = 0; i < strlen(buffer); i++) {
                    if (wei == 0) {
                        if (idx++ == 3) {
                            xin[j++] = ',';
                            idx = 0;
                        }
                        xin[j++] = buffer[i];
                    } else {
                        if (idx++ == wei) {
                            xin[j++] = ',';
                            wei = 3;
                            idx = 0;
                        }
                        xin[j++] = buffer[i];
                    }
                }
                _lobbyBotttom.Label_UserMoney->setString(xin);
            } else {
                _lobbyBotttom.Label_UserMoney->setString(buffer);
            }
        }
    }
}

bool BottomMenuComposite::switchToLayoutControl(Layout *layout, const std::string &controlName,Node *newControl) {
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

void BottomMenuComposite::menuLobbyBottomCallBack(Ref *pSender,Widget::TouchEventType type) {
    if (Widget::TouchEventType::ENDED != type || _isTouch == false)
        return;
    auto btn = (Button *)pSender;
    _isTouch = false;
    btn->runAction(Sequence::create(
    DelayTime::create(0.5f), CCCallFunc::create([=]() {
        _isTouch = true;
    }),
    nullptr));

    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);

    std::string name(btn->getName());

    if (name.compare("Button_friend") == 0) {
        screenShoot();
    } else if (name.compare("Button_task") == 0) {
        unSocketCallBack();
        GameOnlineReward *onlineReward = GameOnlineReward::create();

        onlineReward->onUpdataUserMoney = [this](LLONG money) {
            walletChanged(money);
        };

        onlineReward->onCloseCallBack = [this]() {
            CommandManager::getInstance()->execute(CHECK_GIFT_COM);
        };

        this->addChild(onlineReward, CHILD_LAYER_ZORDER, CHILD_LAYER_ONLINE_TAG);
        onlineReward->setName("onlineReward");
        onlineReward->setIgnoreAnchorPointForPosition(false);
        onlineReward->setAnchorPoint(Vec2(0.5, 0.5));
        onlineReward->setPosition(Vec2(WIN_SIZE / 2));
    } else if (name.compare("Button_activity") == 0) {//活动

        unSocketCallBack();

        GameRechargeRebate *rebateLayer = GameRechargeRebate::create();

        rebateLayer->onUpdataUserMoney = [this](LLONG money) {
            walletChanged(money);
        };

        rebateLayer->onCloseCallBack = [this]() {
            CommandManager::getInstance()->execute("CheckGiftCommand");
        };

        this->addChild(rebateLayer, CHILD_LAYER_ZORDER, CHILD_LAYER_REBATE_TAG);
        rebateLayer->setName("rebateLayer");
        rebateLayer->setIgnoreAnchorPointForPosition(false);
        rebateLayer->setAnchorPoint(Vec2(0.5, 0.5));
        rebateLayer->setPosition(Vec2(WIN_SIZE / 2));
    }    else {
        GamePromptLayer::create()->showPrompt(GBKToUtf8("程序猿使劲开发中。"));
    }
}

//截屏保存
void BottomMenuComposite::screenShoot() {
    std::string filename =
        FileUtils::getInstance()->getWritablePath() + "umeng_share.png";
    if (!FileUtils::getInstance()->isFileExist(filename)) {
        utils::captureScreen(
        [this](bool succeed, const std::string &outputFile) {
            if (succeed) {
                openUMengShare();
            } else {
                MessageBox(GBKToUtf8("获取截屏失败！"), "error");
                return;
            }
        },
        "umeng_share.png");
    } else {
        openUMengShare();
    }
}

void BottomMenuComposite::openUMengShare() {
//    CCUMSocialSDK *sdk = CCUMSocialSDK::create("55c4149a67e58e86020003fc");
//    // sdk->setQQAppIdAndAppKey("1104666954", "G9rpQCEr0yAH2TTf");
//    // 设置友盟分享面板上显示的平台
//    vector<int> *platforms = new vector<int>();
//
//    if (GameCreator()->getRegistGameCount() <= 1) {
//        switch (GameCreator()->getCurrentGameNameID()) {
//        case 10100008:
//            //德州扑克微信号
//            sdk->setWeiXinAppInfo("wx2a070073ba06229e",
//                                  "d4624c36b6795d1d99dcf0547af5443d");
//            break;
//        case 33000306:
//            //李逵劈鱼微信号
//            sdk->setWeiXinAppInfo("wxd9d3ad91c8bed043",
//                                  "d4624c36b6795d1d99dcf0547af5443d");
//            break;
//        case 10100003:
//            //斗地主微信号
//            sdk->setWeiXinAppInfo("wx263b046cf5cd9dfe",
//                                  "d4624c36b6795d1d99dcf0547af5443d");
//            break;
//        case 11105100:
//            //牛牛微信号
//            sdk->setWeiXinAppInfo("wx86bb127186e96222",
//                                  "d4624c36b6795d1d99dcf0547af5443d");
//            break;
//        default:
//            break;
//        }
//    } else {
//        //混合包微信号
//        sdk->setWeiXinAppInfo("wx4ed0eb93cbdeec1d",
//                              "f3e0b868ca297ade78d66e73be313ed7");
//    }

    //#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
    //	platforms->push_back(WEIXIN);
    //	platforms->push_back(WEIXIN_CIRCLE);
    //	platforms->push_back(QZONE);
    //	platforms->push_back(QQ) ;
    //#endif

//    platforms->push_back(WEIXIN);
//    platforms->push_back(WEIXIN_CIRCLE);
//    platforms->push_back(SINA);
//    // platforms->push_back(RENREN) ;
//    platforms->push_back(DOUBAN);
//    sdk->setPlatforms(platforms);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::string url =
        "https://itunes.apple.com/cn/app/ju-le-xiong-you-xi/id1063963773?mt=8";
#else
    std::string url = "http://www.pgyer.com/julexiong";
#endif

    /*std::string	path = FileUtils::getInstance()->getWritablePath() +
    "umeng_share.png";
    if (!FileUtils::getInstance()->isFileExist(path))
    {
    MessageBox(GBKToUtf8("获取截屏失败！"), "error");
    return;
    }*/

    std::string path = FileUtils::getInstance()->fullPathForFilename("share.png");

//    sdk->setPlatformShareContent(
//        WEIXIN,
//        GBKToUtf8("聚乐熊，超人气的棋牌游戏平台，这里有好玩，这里有好礼！"),
//        path.c_str(), GBKToUtf8("聚乐熊，快乐与好运同在！"), url.c_str());
//
//    sdk->setPlatformShareContent(
//        WEIXIN_CIRCLE,
//        GBKToUtf8("聚乐熊，超人气的棋牌游戏平台，这里有好玩，这里有好礼！"),
//        path.c_str(), GBKToUtf8("聚乐熊，快乐与好运同在！"), url.c_str());
//
//    sdk->setPlatformShareContent(
//        SINA, GBKToUtf8("聚乐熊，快乐与好运同在！http://www.hcx78.com/"),
//        path.c_str(), GBKToUtf8("聚乐熊，快乐与好运同在！http://www.hcx78.com/"),
//        url.c_str());

    /*sdk->setPlatformShareContent(QQ,
    GBKToUtf8("聚乐熊，超人气的棋牌游戏平台，这里有好玩，这里有好礼！http://www.hcx78.com/"),
    path.c_str(),
    GBKToUtf8("聚乐熊，快乐与好运同在！http://www.hcx78.com/"),
    url.c_str());

    sdk->setPlatformShareContent(QZONE,
    GBKToUtf8("聚乐熊，超人气的棋牌游戏平台，这里有好玩，这里有好礼！http://www.hcx78.com/"),
    path.c_str(),
    GBKToUtf8("聚乐熊，快乐与好运同在！http://www.hcx78.com/"),
    url.c_str());*/

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    sdk->openShare(GBKToUtf8("聚乐熊，快乐与好运同在！"), path.c_str(), nullptr);
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    sdk->openShare(GBKToUtf8("聚乐熊，快乐与好运同在！"), path.c_str(), nullptr);
//#else
//    GamePromptLayer::create()->showPrompt(GBKToUtf8("仅支持android和ios分享。"));
//#endif
}

//卸载奖励监听
void BottomMenuComposite::unSocketCallBack() {
    PlatformLogic()->removeEventSelector(MDM_GP_SIGN, ASS_GP_SIGN_CHECK);
    PlatformLogic()->removeEventSelector(MDM_GP_ONLINE_AWARD, ASS_GP_ONLINE_AWARD_CHECK);
    PlatformLogic()->removeEventSelector(MDM_GP_NOTIFY_PAY,ASS_GP_GET_MONEY_BY_PAY_CHECK);
}

void BottomMenuComposite::update(float dt) {
    // 玩家信息结构体
    MSG_GP_R_LogonResult &LogonResult = PlatformLogic()->loginResult;
    if (m_money != LogonResult.i64Money) {
        m_money = LogonResult.i64Money;
        _lobbyBotttom.Label_UserMoney->setString(to_string(m_money));
    }
}
