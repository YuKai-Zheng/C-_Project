#include "TopMenuComposite.h"

#include "HNLobby/pattern/CommandManager.h"

#include "HNLobby/FontConfig.h"
#include "HNLobby/PlatformConfig.h"
#include "HNLobby/ChildLayer/GameBankLayer.h"
#include "HNLobby/ChildLayer/GameStoreLayer.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/ChildLayer/GameSignLayer.h"
#include "HNLobby/ChildLayer/ServiceLayer.h"
#include "HNLobby/ChildLayer/GameSetLayer.h"
#include "HNLobby/ChildLayer/GameExitChangeLayer.h"
#include "HNLobby/ChildLayer/GameGiftShop.h"
#include "HNLobby/ChildLayer/GameRankingList.h"
#include "HNLobby/ChildLayer/NoticeList.h"
#include "HNLobby/GameNotice.h"
#include "HNRoom/HNRoomLogic.h"

using namespace HN;

static const char *PLATFORMUI_TOP_CSB = "csb/platform/lobbyUi/PlatformUi_TOP/PlatformUi_TOP.csb";

TopMenuComposite::TopMenuComposite()
    :
    _isTouch(true), _orignalChargePosition(Vec2::ZERO), _orignalBankPosition(Vec2::ZERO) {
}

bool TopMenuComposite::init() {
    if (!Node::init()) {
        return false;
    }

    _spreadLogic.init(this);

    _lobbyTop.node = CSLoader::createNode(PLATFORMUI_TOP_CSB);
    addChild(_lobbyTop.node,PLATFORM_UI_ZORDER_TOP);

    _lobbyTop.layout = (Layout *)_lobbyTop.node->getChildByName("Panel_top");
    _lobbyTop.layout->setPosition(Vec2(0, 0));

    _lobbyTop.Sprite_topTitle = Sprite::create("platform/topFrame.png");
    auto titleSize = _lobbyTop.Sprite_topTitle->getContentSize();
    _lobbyTop.Sprite_topTitle->setPosition(Vec2(_lobbyTop.Sprite_topTitle->getContentSize().width * 0.5f,titleSize.height * 0.32));
    _lobbyTop.layout->addChild(_lobbyTop.Sprite_topTitle, -1);


    _lobbyTop.Sprite_platformLogo =static_cast<Sprite *>(_lobbyTop.layout->getChildByName("game_hall_2"));

    _lobbyTop.Sprite_LogoSpr =Sprite::create("platform/lobbyUi/PlatformUi_TOP/11105100.png");
    _lobbyTop.Sprite_LogoSpr->setVisible(false);
    _lobbyTop.Sprite_LogoSpr->setPosition( _lobbyTop.layout->getContentSize().width * 0.5f,_lobbyTop.Sprite_LogoSpr->getContentSize().height * 0.5f + 10);
    _lobbyTop.layout->addChild(_lobbyTop.Sprite_LogoSpr);

    _lobbyTop.Sprite_TypeSpr =Sprite::create("platform/lobbyUi/PlatformUi_TOP/xinshouchang.png");
    _lobbyTop.Sprite_TypeSpr->setVisible(false);
    _lobbyTop.Sprite_TypeSpr->setPosition( _lobbyTop.layout->getContentSize().width * 0.5f +_lobbyTop.Sprite_TypeSpr->getContentSize().width * 0.75,_lobbyTop.Sprite_LogoSpr->getPositionY() - 8);
    _lobbyTop.layout->addChild(_lobbyTop.Sprite_TypeSpr);

    _lobbyTop.Sprite_FishBiLi = Sprite::create("platform/lobbyUi/PlatformUi_TOP/fishing/chujichang.png");
    _lobbyTop.Sprite_FishBiLi->setVisible(false);
    _lobbyTop.Sprite_FishBiLi->setPosition(Vec2(_lobbyTop.Sprite_LogoSpr->getPositionX() +_lobbyTop.Sprite_TypeSpr->getContentSize().width * 0.8f,_lobbyTop.Sprite_LogoSpr->getContentSize().height * 0.45f));
    _lobbyTop.layout->addChild(_lobbyTop.Sprite_FishBiLi);

    // 退出
    _lobbyTop.Button_Exit =(Button *)Helper::seekWidgetByName(_lobbyTop.layout, "Button_exit");
    //   _lobbyTop.Button_Exit->setPositionX(WINSIZE.width - 90);
    _lobbyTop.Button_Exit->setShowEffect(false);
    _lobbyTop.Button_Exit->addTouchEventListener(CC_CALLBACK_2(TopMenuComposite::menuLobbyTopCallBack, this));


    _lobbyTop.Button_quicklyGame = (Button *)Helper::seekWidgetByName(_lobbyTop.layout, "Button_quickly");
    _lobbyTop.Button_quicklyGame->addTouchEventListener(CC_CALLBACK_2(TopMenuComposite::menuLobbyTopCallBack, this));

    // 返回
    _lobbyTop.Button_Return =(Button *)Helper::seekWidgetByName(_lobbyTop.layout, "Button_return");
    _lobbyTop.Button_Return->setPositionX(_lobbyTop.Button_Exit->getPositionX());
    _lobbyTop.Button_Return->setShowEffect(false);
    //   _oldReturnBtnPos = _lobbyTop.Button_Return->getPosition();
    _lobbyTop.Button_Return->addTouchEventListener( CC_CALLBACK_2(TopMenuComposite::menuLobbyTopCallBack, this));


    //保险箱
    _lobbyTop.Button_Bank = (Button *)Helper::seekWidgetByName(_lobbyTop.layout, "Button_bank");
    _lobbyTop.Button_Bank->addTouchEventListener(CC_CALLBACK_2(TopMenuComposite::menuLobbyTopCallBack, this));
    _lobbyTop.Button_Bank->setShowEffect(false);
    _lobbyTop.Button_Bank->setVisible(PlatformConfig::getInstance()->funcConfig.isOpenRankFunc);
    _orignalBankPosition = _lobbyTop.Button_Bank->getPosition();


    //充值
    _lobbyTop.Button_Charge =(Button *)Helper::seekWidgetByName(_lobbyTop.layout, "Button_charge");
    _lobbyTop.Button_Charge->addTouchEventListener(CC_CALLBACK_2(TopMenuComposite::menuLobbyTopCallBack, this));
    _lobbyTop.Button_Charge->setVisible(false && !getExtendVersion());
    _lobbyTop.Button_Charge->setShowEffect(false);
    _orignalChargePosition = _lobbyTop.Button_Charge->getPosition();

    // 设置
    _lobbyTop.Button_Setting = (Button *)Helper::seekWidgetByName(_lobbyTop.layout, "Button_set");
    _lobbyTop.Button_Setting->setShowEffect(false);
    _lobbyTop.Button_Setting->addTouchEventListener(CC_CALLBACK_2(TopMenuComposite::menuLobbyTopCallBack, this));

    //排行榜
    _lobbyTop.Button_rankList = Button::create("platform/lobbyUi/PlatformUi_TOP/paihangbang.png", "platform/lobbyUi/PlatformUi_TOP/paihangbang.png", "");
    _lobbyTop.Button_rankList->setName("Button_rank");
    _lobbyTop.Button_rankList->setPosition(Vec2(_lobbyTop.Button_Setting->getPositionX() - 100, _lobbyTop.Button_Setting->getPositionY()));
    _lobbyTop.Button_rankList->setVisible(true);
    _lobbyTop.Button_rankList->setShowEffect(false);
    _lobbyTop.Button_rankList->addTouchEventListener(CC_CALLBACK_2(TopMenuComposite::menuLobbyTopCallBack, this));
    _lobbyTop.Button_Charge->getParent()->addChild(_lobbyTop.Button_rankList);

    _lobbyTop.Sprite_nickName =static_cast<Sprite *>(_lobbyTop.layout->getChildByName("id_10"));
    _lobbyTop.Sprite_id =static_cast<Sprite *>(_lobbyTop.layout->getChildByName("nicheng_9"));

    // auto textCount = _lobbyTop.Label_UserMoney->getString().length();
    //_lobbyTop.bgGlob =
    // static_cast<ImageView*>(_lobbyTop.layout->getChildByName("Sprite_4"));
    //_lobbyTop.bgGlob->setAnchorPoint(Vec2(0, 0.5));
    //_lobbyTop.bgGlob->setScale9Enabled(true);
    // if (textCount > 12) // 这里做一个常量 12 是应为
    // 左上方玩家金币的底图要根据金币长度做变化 做一个百分比  这个是估值
    //{
    //	auto width = _lobbyTop.Label_UserMoney->getContentSize().width / 12 *
    // textCount;
    //}
    _lobbyTop.globButton =static_cast<Button *>(_lobbyTop.layout->getChildByName("Button_1"));
    _lobbyTop.globButton->addTouchEventListener(CC_CALLBACK_2(TopMenuComposite::menuLobbyTopCallBack, this));


    //新春活动按钮
    _lobbyTop.Button_newYear = (Button *)Helper::seekWidgetByName(_lobbyTop.layout, "Button_newYear");
    _lobbyTop.Button_newYear->addTouchEventListener(CC_CALLBACK_2(TopMenuComposite::newYearActivityCallBack, this));


    _lobbyTop.Button_quicklyGame = (Button *)Helper::seekWidgetByName(_lobbyTop.layout, "Button_quickly");
    _lobbyTop.Button_quicklyGame->addTouchEventListener(CC_CALLBACK_2(TopMenuComposite::menuLobbyTopCallBack, this));
    /////

    return true;
}

void TopMenuComposite::newYearActivityCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }

    if (platformDelegate) {
        UserDefault::getInstance()->setIntegerForKey("Enter_Game_ID", 11118888);
        platformDelegate->openNewYearActivity();
    }
}

void TopMenuComposite::moveOut(bool isOut) {
    int s = isOut ? 1 : -1;
    if (!isOut) {
        _lobbyTop.layout->runAction(Sequence::create(MoveBy::create(0.3f, Vec2(0, _lobbyTop.layout->getContentSize().height * s)), nullptr));
    } else {
        _lobbyTop.layout->runAction(Sequence::create(MoveBy::create(0.3f, Vec2(0, _lobbyTop.layout->getContentSize().height * s)), nullptr));
    }
}

void TopMenuComposite::autoMove() {
    //int s = isOut ? 1 : -1;
    auto moveBy = MoveBy::create(0.3f, Vec2(0, _lobbyTop.layout->getContentSize().height));
    auto reverseMoveBy = moveBy->reverse();
    auto delay = DelayTime::create(0.3f);
    //_lobbyTop.layout->runAction(Sequence::create(moveBy, delay,reverseMoveBy, nullptr));
    _lobbyTop.layout->runAction(Sequence::create(moveBy, reverseMoveBy, nullptr));
    //if (!isOut) {
    //    _lobbyTop.layout->runAction(Sequence::create(MoveBy::create(0.3f, Vec2(0, _lobbyTop.layout->getContentSize().height * s)), nullptr));
    //} else {
    //    _lobbyTop.layout->runAction(Sequence::create(MoveBy::create(0.3f, Vec2(0, _lobbyTop.layout->getContentSize().height * s)), nullptr));
    //}
}

void TopMenuComposite::menuLobbyTopCallBack(Ref *pSender, Widget::TouchEventType type) {
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
    auto winSize = Director::getInstance()->getWinSize();

    std::string name(btn->getName());

    if (name.compare("Button_popularize") == 0) {// 推广员
        CommandManager::getInstance()->addLoaderCommand(GBKToUtf8("正在获取信息，请稍后..."));

        char tmpstr[100];
        sprintf(tmpstr, "option=GetTuiGuangInfo&tgUserId=%d",HNPlatformLogic::getInstance()->loginResult.dwUserID);
        _spreadLogic.requestSpreadInfo(PlatformConfig::getInstance()->getPromoterUrl(), tmpstr);

    } else if (name.compare("Button_head") == 0) {// 用户头像
        GamePromptLayer::create()->showPrompt(GBKToUtf8("程序猿使劲开发中。"));

    } else if (name.compare("Button_bank") == 0) { //银行
        auto bankLayer = GameBankLayer::createGameBank();
        bankLayer->setChangeDelegate(this);
        bankLayer->openBank(this->getParent(), Vec2(winSize.width, 0), CHILD_LAYER_ZORDER, CHILD_LAYER_BANK_TAG);

    } else if (name.compare("Button_charge") == 0) {//商店
        auto storeLayer =GameStoreLayer::createGameStore(this, PlatformConfig::getInstance()->funcConfig.isOpenBankFunc);
        storeLayer->setChangeDelegate(this);
        storeLayer->openStore(this->getParent(), Vec2(winSize.width, 0), CHILD_LAYER_ZORDER,CHILD_LAYER_STORE_TAG);
        storeLayer->onCloseCallBack = [this]() {
            CommandManager::getInstance()->execute("CheckGiftCommand");
        };

    } else if (name.compare("Button_1") == 0) {
        auto storeLayer =GameStoreLayer::createGameStore(this, PlatformConfig::getInstance()->funcConfig.isOpenBankFunc);
        storeLayer->setChangeDelegate(this);
        storeLayer->openStore(this->getParent(), Vec2(winSize.width, 0), CHILD_LAYER_ZORDER,CHILD_LAYER_STORE_TAG);
        storeLayer->onCloseCallBack = [this]() {
            CommandManager::getInstance()->execute("CheckGiftCommand");
        };

    } else if (name.compare("Button_buy") == 0) {        // 	公告
        NoticeList* noticeList = NoticeList::create();
        this->addChild(noticeList, CHILD_LAYER_ZORDER);
        noticeList->setPosition(Vec2(winSize.width / 2, winSize.height /2));

    } else if (name.compare("Button_return") == 0) { // 返回
        platformDelegate->handleReturnButton();

    } else if (name.compare("Button_quickly") == 0) {//快速加入
        int idx = 0;
        int deskNo = 0;
        int empty = 0;

        for (int i = 0; i < _gameDesk->getCurRoomDeskCount();
                i++) { //遍历每桌玩家如记录最多玩家最多的桌子号
            //如果有相同取遍历的第一个
            std::vector<UserInfoStruct *> _vecUserInfo;
            _vecUserInfo.clear();
            UserInfoModule()->findDeskUsers(i, _vecUserInfo);
            if (_vecUserInfo.size() >= idx &&
                    _vecUserInfo.size() != _gameDesk->getCurRoomInfo()->uDeskPeople) {
                idx = _vecUserInfo.size() >= idx ? _vecUserInfo.size() : idx;
                deskNo = i;
            }
        }
        std::vector<UserInfoStruct *> _vecUserInfo;
        _vecUserInfo.clear();
        UserInfoModule()->findDeskUsers(deskNo, _vecUserInfo);

        if (_vecUserInfo.size() == _gameDesk->getCurRoomInfo()->uDeskPeople) {
            GamePromptLayer::create()->showPrompt(GetText("ROOM_PEPOLE_MAX"));
            return;
        }

        if (_vecUserInfo.size() == 0) {
            _gameDesk->quickEnterGameCallBack(deskNo, 0);
            return;
        }
        vector<INT> seatNo;
        int randNum;
        for (auto user : _vecUserInfo) {
            seatNo.push_back(user->bDeskStation);
        }

        while (1) {
            randNum = int(random(0, _gameDesk->getPlayerCount() - 1));
            bool same = true;
            for (auto key : seatNo) {
                if (key == randNum)
                    same = false;
            }
            if (same) {
                _gameDesk->quickEnterGameCallBack(deskNo, randNum);
                return;
            }
        }
    } else if (name.compare("Button_gift") == 0) {        // 签到
        //unSocketCallBack();
        GameSignLayer* signLayer = GameSignLayer::create();

        signLayer->onUpdataUserMoney = [this](LLONG money) {
            walletChanged(money);
        };
        signLayer->onCloseCallBack = [this]() {
            CommandManager::getInstance()->execute(CHECK_GIFT_COM);
        };

        auto winSize = Director::getInstance()->getWinSize();
        this->addChild(signLayer, CHILD_LAYER_ZORDER,CHILD_LAYER_SIGN_TAG);
        signLayer->setName("signLayer");
        signLayer->setIgnoreAnchorPointForPosition(false);
        signLayer->setAnchorPoint(Vec2(0.5, 0.5));
        signLayer->setPosition(Vec2(winSize/2));

    } else if (name.compare("Button_service") == 0) {        // 客服
        auto serviceLayer = ServiceLayer::create();
        serviceLayer->showService(this->getParent(), Vec2(990, winSize.height - 55),CHILD_LAYER_ZORDER, CHILD_LAYER_SERVICE_TAG);

    } else if (name.compare("Button_exit") == 0) {    // 退出
        HNAudioEngine::getInstance()->playEffect("music/leave.mp3");
        auto exitChangeLayer = GameExitChangeLayer::create();
        exitChangeLayer->showExitChoose(this->getParent(), CHILD_LAYER_EXITCHANGE_TAG,CHILD_LAYER_EXITCHANGE_TAG);

    } else if (name.compare("Button_set") == 0) {    // 设置
        auto setLayer = GameSetLayer::create();
        setLayer->openSetLayer(this->getParent(), Vec2(winSize.width, 0), CHILD_LAYER_ZORDER,CHILD_LAYER_SET_TAG);

    } else if (name.compare("Button_shop") == 0) {        //礼品商城
        auto giftShopLayer = GameGiftShop::createGameGiftShop(this, 0);
        giftShopLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        giftShopLayer->setPosition(winSize / 2);
        giftShopLayer->setName("giftShopLayer");
        addChild(giftShopLayer, 1000);

    } else if (name.compare("Button_rank") == 0) {//排行榜
        auto rankLayer = GameRankingList::create();
        rankLayer->openRank(this->getParent(), Vec2(winSize.width, 0), CHILD_LAYER_ZORDER,CHILD_LAYER_RANK_TAG);

    } else {  // 其他
        GamePromptLayer::create()->showPrompt(GBKToUtf8("程序猿使劲开发中。"));
    }
}

void TopMenuComposite::walletChanged(LLONG money) {
}

void TopMenuComposite::checkGiftMessages() {
    CommandManager::getInstance()->execute(CHECK_GIFT_COM);
}

void TopMenuComposite::controlChargeAndBankBtnPosition(bool openCharge, bool openBank, bool openrank, bool forceHide) {
    if (forceHide) {
        _lobbyTop.Button_Charge->setVisible(false && !getExtendVersion());
        _lobbyTop.Button_Bank->setVisible(false);
        _lobbyTop.Button_rankList->setVisible(false);
        return;
    }

    //设置位置
    if (openCharge && openBank) {
        _lobbyTop.Button_Charge->setPosition(_orignalChargePosition);
        _lobbyTop.Button_Bank->setPosition(_orignalBankPosition);

    } else if (openCharge && !openBank) {
        _lobbyTop.Button_Charge->setPosition(_orignalChargePosition);

    } else if (!openCharge && openBank) {
        _lobbyTop.Button_Bank->setPosition(_orignalChargePosition);
    }

    //设置可见
    _lobbyTop.Button_Charge->setVisible(openCharge && !getExtendVersion());
    _lobbyTop.Button_Bank->setVisible(openBank);

    if (isShowBank) {
        _lobbyTop.Button_rankList->setVisible(false);
    } else {
        _lobbyTop.Button_rankList->setVisible(openrank);
    }
}

void TopMenuComposite::setType(MenuType type) {
    if (type == game_list_type || type == game_type_type) {
        canDisplayButton(_lobbyTop.Sprite_LogoSpr, false);
        canDisplayButton(_lobbyTop.Button_Setting, true);
        canDisplayButton(_lobbyTop.Sprite_platformLogo, true);
        canDisplayButton(_lobbyTop.Button_Exit, true);
        canDisplayButton(_lobbyTop.Button_newYear, false);
        controlChargeAndBankBtnPosition(PlatformConfig::getInstance()->funcConfig.isOpenRechargeFunc, PlatformConfig::getInstance()->funcConfig.isOpenBankFunc, PlatformConfig::getInstance()->funcConfig.isOpenRankFunc);

    } else if (type == room_type) {
        canDisplayButton(_lobbyTop.Sprite_LogoSpr, true);
        canDisplayButton(_lobbyTop.Button_Exit, false);
        canDisplayButton(_lobbyTop.Button_Setting, false);
        canDisplayButton(_lobbyTop.Sprite_platformLogo, false);
        canDisplayButton(_lobbyTop.Sprite_TypeSpr, false);
        canDisplayButton(_lobbyTop.Button_quicklyGame, false);
        canDisplayButton(_lobbyTop.Sprite_FishBiLi, false);
        canDisplayButton(_lobbyTop.Button_Return, true);
        canDisplayButton(_lobbyTop.Button_newYear, false);

        controlChargeAndBankBtnPosition(false,false,false,true);

        _lobbyTop.Sprite_LogoSpr->setTexture(StringUtils::format("platform/lobbyUi/PlatformUi_TOP/%d.png", UserDefault::getInstance()->getIntegerForKey("Enter_Game_ID")));
        _lobbyTop.Sprite_LogoSpr->setPositionX(_lobbyTop.layout->getContentSize().width * 0.5f);

    } else if (type == desk_type) {
        canDisplayButton(_lobbyTop.Sprite_LogoSpr, true);
        canDisplayButton(_lobbyTop.Button_Exit, false);
        canDisplayButton(_lobbyTop.Button_Setting, false);
        canDisplayButton(_lobbyTop.Sprite_platformLogo, false);
        canDisplayButton(_lobbyTop.Sprite_FishBiLi, false);
        //
        canDisplayButton(_lobbyTop.Sprite_TypeSpr, true);
        canDisplayButton(_lobbyTop.Button_quicklyGame, true);
        canDisplayButton(_lobbyTop.Button_Return, true);
        canDisplayButton(_lobbyTop.Button_newYear, false);

        controlChargeAndBankBtnPosition(false, false, false, true);

        setRoomInfo(RoomLogic()->getSelectedRoom());

        _lobbyTop.Sprite_LogoSpr->setTexture(StringUtils::format("platform/lobbyUi/PlatformUi_TOP/%d.png", UserDefault::getInstance()->getIntegerForKey("Enter_Game_ID")));
        _lobbyTop.Sprite_LogoSpr->setPositionX(_lobbyTop.Sprite_LogoSpr->getPositionX() - _lobbyTop.Sprite_LogoSpr->getContentSize().width * 0.25f);

        char str[100] = { 0 };
        char bStr[100] = { 0 };
        getRoomSpriteByRoomName(str, bStr, roomInfo->szGameRoomName);

        _lobbyTop.Sprite_TypeSpr->setTexture(str);


        if (roomInfo->uNameID == 33003107 || roomInfo->uNameID == 33003108 || roomInfo->uNameID == 33003109) {
            _lobbyTop.Sprite_TypeSpr->setPosition(Vec2(WIN_SIZE.width * 0.5f, _lobbyTop.Sprite_LogoSpr->getPositionY()));
            _lobbyTop.Sprite_FishBiLi->setTexture(bStr);
            _lobbyTop.Sprite_FishBiLi->setVisible(true);
            _lobbyTop.Sprite_LogoSpr->setVisible(false);
        } else {
            _lobbyTop.Sprite_TypeSpr->setPosition(_lobbyTop.layout->getContentSize().width * 0.5f +_lobbyTop.Sprite_TypeSpr->getContentSize().width * 0.75, _lobbyTop.Sprite_LogoSpr->getPositionY() - 8);
        }
    }

    if (type == game_list_type) {
        canDisplayButton(_lobbyTop.Button_Return, true);
        canDisplayButton(_lobbyTop.Button_Exit, false);
        controlChargeAndBankBtnPosition(PlatformConfig::getInstance()->funcConfig.isOpenRechargeFunc, PlatformConfig::getInstance()->funcConfig.isOpenBankFunc, PlatformConfig::getInstance()->funcConfig.isOpenRankFunc);
        canDisplayButton(_lobbyTop.Button_newYear, false);
        canDisplayButton(_lobbyTop.Button_rankList, false);

    } else if (type == game_type_type) {
        canDisplayButton(_lobbyTop.Button_newYear, PlatformConfig::getInstance()->funcConfig.isOpenYearFunc);
        canDisplayButton(_lobbyTop.Button_rankList, PlatformConfig::getInstance()->funcConfig.isOpenRankFunc);
        canDisplayButton(_lobbyTop.Button_Return, false);
        canDisplayButton(_lobbyTop.Button_Exit, true);
    }
}

void TopMenuComposite::canDisplayButton(Node *node, bool isDisplay) {
    node->setVisible(isDisplay);
}

void TopMenuComposite::setShowBank(bool isShow) {
    isShowBank = isShow;
}

void TopMenuComposite::getRoomSpriteByRoomName(char name[], char fishName[], char* gameName) {
    char str[100] = { 0 };
    char bStr[100] = { 0 };

    string tmpName = ForceGBKToUtf8(gameName);

    string s0 = GBKToUtf8("新手场");
    string s1 = GBKToUtf8("初级场");
    string s2 = GBKToUtf8("中级场");
    string s3 = GBKToUtf8("高级场");
    string s4 = GBKToUtf8("超级场");
    string s5 = GBKToUtf8("至尊场");

    if (std::strcmp(tmpName.c_str(), s0.c_str()) == 0) {
        sprintf(str, "%s", "platform/lobbyUi/PlatformUi_TOP/xinshouchang.png");
        sprintf(bStr, "%s", "platform/lobbyUi/PlatformUi_TOP/fishBiLi/xinshouchang.png");
    } else if (std::strcmp(tmpName.c_str(), s1.c_str()) == 0) {
        sprintf(str, "%s", "platform/lobbyUi/PlatformUi_TOP/chujichang.png");
        sprintf(bStr, "%s", "platform/lobbyUi/PlatformUi_TOP/fishBiLi/chujichang.png");
    } else if (std::strcmp(tmpName.c_str(), s2.c_str()) == 0) {
        sprintf(str, "%s", "platform/lobbyUi/PlatformUi_TOP/zhongjichang.png");
        sprintf(bStr, "%s", "platform/lobbyUi/PlatformUi_TOP/fishBiLi/zhongjichang.png");
    } else if (std::strcmp(tmpName.c_str(), s3.c_str()) == 0) {
        sprintf(str, "%s", "platform/lobbyUi/PlatformUi_TOP/gaojichang.png");
        sprintf(bStr, "%s", "platform/lobbyUi/PlatformUi_TOP/fishBiLi/gaojichang.png");
    } else if (std::strcmp(tmpName.c_str(), s4.c_str()) == 0) {
        sprintf(str, "%s", "platform/lobbyUi/PlatformUi_TOP/chaojichang.png");
        sprintf(bStr, "%s", "platform/lobbyUi/PlatformUi_TOP/fishBiLi/chaojichang.png");
    } else if (std::strcmp(tmpName.c_str(), s5.c_str()) == 0) {
        sprintf(str, "%s", "platform/lobbyUi/PlatformUi_TOP/zhizunchang.png");
        sprintf(bStr, "%s", "platform/lobbyUi/PlatformUi_TOP/fishBiLi/zhizunchang.png");
    } else {
        sprintf(str, "%s", "platform/lobbyUi/PlatformUi_TOP/xinshouchang.png");
        sprintf(bStr, "%s", "platform/lobbyUi/PlatformUi_TOP/fishBiLi/xinshouchang.png");
    }
    strcpy(name, str);
    strcpy(fishName, bStr);
}

void TopMenuComposite::setGameDeskLayer(GameDesk* gameDesk) {
    _gameDesk = gameDesk;
}

void TopMenuComposite::setRoomInfo(ComRoomInfo* roomInfo) {
    this->roomInfo = roomInfo;
}
