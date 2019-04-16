#include "HNLobby/ChildLayer/GameStoreLayer.h"
#include "HNLobby/FontConfig.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/GamePrompt.h"
#include "HNMarketExport.h"
#include "HNLobby/PlatformConfig.h"
#include <string>
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"
#include "network/HttpRequest.h"
#include "UI/Base/AUIButton.h"
#include "HNLobby/globel.h"

static const char* STORE_BG = "platform/hallBg.png";                  //backGround
static const char* GAME_LOGO = "platform/recharge/logo.png";                  //backGround
static const char* GAME_KUANG = "platform/newbankUi/1.png";

static const char* GAME_CLOSE_BUTTON = "platform/lobbyUi/PlatformUi_TOP/return.png";
//static const char* CLOSE_MENU_N = "platform/store/second_back.png";                         //closeMenu
//static const char* CLOSE_MENU_D = "platform/store/second_back.png";                      //closeMenu
//static const char* CLOSE_PAY = "platform/store/windowsClose.png";                      //closePay
//static const char* PAY_FRAME		= "platform/store/payPopu/payBg.png";               //payBg
//static const char* WEIXIN_PAY		= "platform/store/payPopu/weixin.png";               //weixinPay
//static const char* ZHIFUBAO_PAY	    = "platform/store/payPopu/zhifubao.png";               //zhifubaoPay
//static const char* YINLIAN_PAY		= "platform/store/payPopu/yinlian.png";               //yinlianPay
//static const char* XINYONGKA_PAY	= "platform/store/payPopu/xinyongka.png";           //xinyongkaPay
static const char* IAP_PAY_BTN_RES  = "button/PayNowBtn.png";                       //IAP立即支付
//static const char* SELF_FRAME	    = "platform/store/common_opaque.png";           //frame
//static const char* SELF_COIN	    = "platform/store/coinMid.png";
//
//static const char* LIST_BG		= "platform/store/listItem_bg.png";               //listBG
static const char* COIN_ICO		= "platform/store/coin_1.png";                       //chip
static const char* STORE_BUY	= "platform/store/good_bg.png";                       //buy
//static const char* PAY_BG	    = "platform/store/shop.png";                       //pay
//static const char* LEAF_LEFT	    = "platform/subViewBambooLeft.png";
//static const char* LEAF_RIGHT	    = "platform/subViewBambooRight.png";
//
//static const char* CHIP_BG		= "platform/chip.png";
//static const char* STORE_TITLE	= "platform/store/title_btn.png";

//static const char* GetOrder_URL = "http://192.168.0.106:88/public/xmlhttpuser.aspx";
//static const char* Alipay_URL	= "http://192.168.0.106:88/Manage/Pay/Alipay/Alipay_Mobile.aspx";
//static const char* Yeepay_URL	= "http://192.168.0.106:88/Manage/Pay/Yeepay/YeePayMobileReceive.aspx";

const static char* Weixin_Pay_Tag = "WeixinPay";
const static char* Alipay_Tag = "Alipay";
const static char* IAP_Tag = "IAP";

#define WINSIZE Director::getInstance()->getWinSize()
#define JSON_RESOVE(obj, key) (obj.HasMember(key) && !obj[key].IsNull())

GameStoreLayer::GameStoreLayer()
    : _product(nullptr)
    , _selfCoin(nullptr)
    , _selfBankCoin(nullptr)
    , onCloseCallBack(nullptr)
    , _rechargeMoney(0)
    , _targetId(0)
    , _confimId(0)
    , _storyOpen(false)
    , _isClose(true) {
    _vecCheckBoxs.clear();

}

GameStoreLayer::~GameStoreLayer() {
    PlatformLogic()->removeEventSelector(MDM_GP_NOTIFY_PAY, ASS_GP_NOTIFY_PAY);

}
GameStoreLayer* GameStoreLayer::createGameStore(MoneyChangeNotify* delegate,bool open) {
    GameStoreLayer *pRet = new GameStoreLayer();
    if (pRet && pRet->init(open)) {
        pRet->setChangeDelegate(delegate);
        pRet->autorelease();
        return pRet;
    } else {
        CC_SAFE_DELETE(pRet);
        return nullptr;
    }
}

void GameStoreLayer::openStore(Node* parent, Vec2 vec, int zorder, int tag) {
    CCAssert(nullptr != parent, "parent is nullptr");

    Size winSize = Director::getInstance()->getWinSize();
    this->setPosition(vec);
    parent->addChild(this, zorder, tag);
    parent->runAction(MoveBy::create(0.3f, Vec2(-winSize.width, 0)));
}

void GameStoreLayer::closeStore() {
    if (_isClose) {
        HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
        auto winSize = Director::getInstance()->getWinSize();
        this->getParent()->runAction(Sequence::create(MoveBy::create(0.3f, Vec2(winSize.width, 0)),
        CCCallFunc::create([&]() {
            this->removeFromParent();
        }), nullptr));

        _isClose = false;
    }
}

bool GameStoreLayer::init(bool open) {
    if (!HNLayer::init()) {
        return false;
    }
    _storyOpen = open;
    Size winSize = Director::getInstance()->getWinSize();

    //商店背景
    setBackGroundImage(STORE_BG);
    loadTopLogo(GAME_LOGO);
    loadKuang(GAME_KUANG);
    loadUI();

    _IAP_supported = PlatformConfig::getInstance()->getSupportIAP();

    ////商店标题
    //auto titleBtn = Button::create(STORE_TITLE,STORE_TITLE);
    //titleBtn->setAnchorPoint(Point(0.5, 1));
    //titleBtn->setPosition(Vec2(winSize.width / 2, winSize.height));
    //auto titleSize = titleBtn->getContentSize();
    //titleBtn->setScale((0.8f*1280.0f/winSize.width)/(720/winSize.height),(0.8f*1280.0f/winSize.width)/(720/winSize.height));
    //this->addChild(titleBtn, 2);

    //auto MyListener = EventListenerTouchOneByOne::create();
    ////阻止触摸向下传递
    //MyListener->setSwallowTouches(true);
    //MyListener->onTouchBegan = [&](Touch* touch, Event* event) {
    //    auto target = dynamic_cast<GameStoreLayer*>(event->getCurrentTarget());//获取的当前触摸的目标
    //    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    //    Size s = target->getContentSize();
    //    Rect rect = Rect(0, 0, s.width, s.height);
    //    return rect.containsPoint(locationInNode);//判断触摸点是否在目标的范围内

    //};
    //_eventDispatcher->addEventListenerWithSceneGraphPriority(MyListener, this);

    // 关闭按钮
    _modle.close = Button::create(GAME_CLOSE_BUTTON, GAME_CLOSE_BUTTON);
    _modle.close->setShowEffect(false);
    auto closeSIze = _modle.close->getContentSize();
    _modle.close->setPosition(Vec2(winSize.width * 0.05f, winSize.height * 0.94f));
    _modle.close->setPosition(Vec2(WINSIZE.width - closeSIze.width*0.55, WINSIZE.height - closeSIze.height * 0.55));
    addChild(_modle.close, 5);
    _modle.close->addTouchEventListener(CC_CALLBACK_2(GameStoreLayer::closeEventCallBack, this));



    //商品背景底色
    //auto storebg = cocos2d::ui::Scale9Sprite::create(SELF_FRAME);
    //storebg->setPreferredSize(Size(1300,(580.0f*(1300.0f/winSize.width))/(720.0f/winSize.height)));
    //storebg->setPosition(Vec2(winSize.width / 2, winSize.height/2-10));
    //addChild(storebg);

    //// 商品列表
    //_storeList = ListView::create();
    //_storeList->setItemsMargin(5);//列表间隙
    //_storeList->setBounceEnabled(true);//回弹
    //_storeList->setGravity(ListView::Gravity::CENTER_HORIZONTAL);//滑动方向(上下)
    //_storeList->setContentSize(Size(1280.0f, (580.0f* (1280.0f / winSize.width)) / ( 720.0f/winSize.height)));//列表范围
    //_storeList->setAnchorPoint(Vec2(0.5f, 0.5f));
    //_storeList->setPosition(Vec2(winSize.width / 2, winSize.height / 2 - 10));
    //this->addChild(_storeList);

    //for (UINT i = 0; i < ProductManger::getInstance()->getSize(); i += 2) {
    //	const PRODUCT_INFO* productLeft = i < ProductManger::getInstance()->getSize() ? ProductManger::getInstance()->getProducts(i) : nullptr;
    //	const PRODUCT_INFO* productRight = (i + 1) < ProductManger::getInstance()->getSize() ? ProductManger::getInstance()->getProducts(i + 1) : nullptr;
    //	if (nullptr != productLeft || nullptr != productRight) {
    //		auto storeItem = createStoreItemTemplate(productLeft, productRight, i);
    //		_storeList->pushBackCustomItem(storeItem);
    //	}
    //}

    ////财富
    //char bufferCoin[128];
    //auto moneyLoader = CSLoader::createNode("platform/store/moneyNode.csb");
    //moneyLoader->setPosition(winSize.width/2, 30);
    //addChild(moneyLoader, 10);

    //auto moneyLayout = (Layout*)moneyLoader->getChildByName("Panel_1");
    //auto imageBg= (ImageView*)moneyLayout->getChildByName("Image_bg");
    //imageBg->setContentSize(Size(1280.0f , (75.0f*( 1280.0f/winSize.width))/(720.0f/winSize.height)));

    //_selfCoin = (Text*)moneyLayout->getChildByName("Text_money");
    //_selfCoin->setFontName(gFontFile);
    //_selfCoin->setColor(colorGold);
    //sprintf(bufferCoin, GBKToUtf8("%lld"), PlatformLogic()->loginResult.i64Money);
    //_selfCoin->setString(bufferCoin);

    //auto selfQuan= (Text*)moneyLayout->getChildByName("Text_quan");
    //selfQuan->setFontName(gFontFile);
    //selfQuan->setColor(colorGold);
    //sprintf(bufferCoin, GBKToUtf8("%lld"), PlatformLogic()->loginResult.i64Bank);
    //selfQuan->setString(bufferCoin);

    //auto text_vip = (Text*)moneyLayout->getChildByName("Text_3");
    //if(text_vip != nullptr) {
    //    text_vip->setVisible(false);
    //}

    return true;
}

void GameStoreLayer::loadTopLogo(const char* fileName) {
    auto logo = Sprite::create(fileName);
    logo->setPosition(WINSIZE.width * 0.5f, WINSIZE.height - logo->getContentSize().height *0.5f);
    addChild(logo, -1);
}

void GameStoreLayer::setChangeDelegate(MoneyChangeNotify* delegate) {
    _delegate = delegate;
}

void GameStoreLayer::loadKuang(const char* fileName) {
    auto logo = Sprite::create(fileName);
    m_pUIRoot = logo;
    logo->setPosition(WINSIZE.width * 0.5f, WINSIZE.height * 0.48f);
    addChild(logo, -1);
}

void GameStoreLayer::loadUI() {
    auto root = CSLoader::createNode("csb/platform/recharge/Node.csb");
    auto posSize = root->getChildByName("Panel_1")->getContentSize();
    m_pUIRoot->addChild(root);

    auto priceLabelSprite = dynamic_cast<Sprite*>(root->getChildByName("Sprite_1"));
    Size priceSize = priceLabelSprite->getContentSize();
    float offsetHeight = priceSize.height * 0.4f;
    auto referSprite = dynamic_cast<Sprite*>(root->getChildByName("Sprite_3"));
    referSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    referSprite->setPosition(Vec2(priceLabelSprite->getPositionX() - priceSize.width / 2,
                                  priceLabelSprite->getPositionY() + priceSize.height / 2 + offsetHeight));
    Vec2 pos = referSprite->getPosition();

    auto coin_bag_label = dynamic_cast<Sprite*>(root->getChildByName("coin_bag"));
    coin_bag_label->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    coin_bag_label->setPosition(Vec2(referSprite->getPositionX() - coin_bag_label->getContentSize().width * 1.9f,referSprite->getPositionY()));
    auto gold_bag_label = dynamic_cast<Sprite*>(root->getChildByName("gold_bag"));
    gold_bag_label->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    gold_bag_label->setPosition(Vec2(coin_bag_label->getPositionX(), coin_bag_label->getPositionY() - offsetHeight));

    auto coin_bank_label = dynamic_cast<Sprite*>(root->getChildByName("coin_bank"));
    coin_bank_label->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    coin_bank_label->setVisible(_storyOpen);

    coin_bank_label->setPosition(Vec2(gold_bag_label->getPositionX(), gold_bag_label->getPositionY() - offsetHeight));
    auto gold_bank_label = dynamic_cast<Sprite*>(root->getChildByName("gold_bank"));
    gold_bank_label->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    gold_bank_label->setVisible(_storyOpen);
    gold_bank_label->setPosition(Vec2(coin_bank_label->getPositionX(), coin_bank_label->getPositionY() - offsetHeight));

    MSG_GP_R_LogonResult loginResult = PlatformLogic()->getInstance()->loginResult;
    _selfCoin = static_cast<Text*>(root->getChildByName("Text_1"));
    _selfCoin->setPosition(Vec2(gold_bag_label->getPositionX() + gold_bag_label->getContentSize().width + _selfCoin->getContentSize().width / 2,
                                gold_bag_label->getPositionY() - gold_bag_label->getContentSize().height / 2));
    _selfBankCoin = static_cast<Text*>(root->getChildByName("Text_1_0"));
    _selfBankCoin->setPosition(Vec2(_selfCoin->getPositionX(), gold_bank_label->getPositionY() - gold_bank_label->getContentSize().height / 2));
    _selfCoin->setString(to_string(loginResult.i64Money));
    _selfBankCoin->setString(to_string(loginResult.i64Bank));
    _selfBankCoin->setVisible(_storyOpen);

    for (int i = 0; i < ProductManger::getInstance()->getSize(); i++) { // 获取商品列表信息 应需求服务端应该新增 任意充值类型 每次充值数量最少为6R;
        const PRODUCT_INFO* productLeft = ProductManger::getInstance()->getProducts(i);
        _vecProductInfo.push_back(productLeft);
    }

    _modle.weixin = AUIButton::create("button/yellowBg.png", "button/yellowBg.png");
    auto size = _modle.weixin->getContentSize();
    _modle.weixin->setName(Weixin_Pay_Tag);
    _modle.weixin->setTitle("button/weixin.png");
    _modle.weixin->setTitleOffset(Vec2(-4, 4));
    _modle.weixin->setPosition(Vec2(posSize.width*0.65f - size.width*0.65f, size.height*1.3));
    _modle.weixin->addTouchEventListener(CC_CALLBACK_2(GameStoreLayer::buttonCallBack, this));
    m_pUIRoot->addChild(_modle.weixin);

    _modle.zhifubao = AUIButton::create("button/blueBg.png", "button/blueBg.png");
    _modle.zhifubao->setName(Alipay_Tag);
    _modle.zhifubao->setTitle("button/zhifubao.png");
    _modle.zhifubao->setTitleOffset(Vec2(-4, 4));
    _modle.zhifubao->setPosition(Vec2(posSize.width*0.65f + size.width*0.65f, size.height*1.3));
    _modle.zhifubao->addTouchEventListener(CC_CALLBACK_2(GameStoreLayer::buttonCallBack, this));
    m_pUIRoot->addChild(_modle.zhifubao);

    _modle.IAP_Pay = AUIButton::create("button/yellowBg.png", "button/yellowBg.png");
    _modle.IAP_Pay->setName(IAP_Tag);
    _modle.IAP_Pay->setTitle(IAP_PAY_BTN_RES);
    _modle.IAP_Pay->setTitleOffset(Vec2(-4, 4));
    _modle.IAP_Pay->setPosition(Vec2(WINSIZE.width / 2, size.height * 1.3f));
    _modle.IAP_Pay->addTouchEventListener(CC_CALLBACK_2(GameStoreLayer::buttonCallBack, this));
    m_pUIRoot->addChild(_modle.IAP_Pay);

    //_modle.weixin = AUIButton::create()
    //_modle.TextField_account = createTextField(root, "account");
    //_modle.TextField_accountOk = createTextField(root, "confirm");
    //_modle.TextField_otherMoney = createTextField(root, "money");

    //6元复选框
    _modle.CheckBox_1 = createCheckBox(root, "1", LEVEL_MONEY_1);
    const PRODUCT_INFO* produce6 = (0 < ProductManger::getInstance()->getSize()) ? ProductManger::getInstance()->getProducts(0) : nullptr;
    _modle.CheckBox_1->setUserData(const_cast<PRODUCT_INFO*>(produce6));
    _vecCheckBoxs.push_back(_modle.CheckBox_1);
    //50元复选框
    _modle.CheckBox_2 = createCheckBox(root, "2", LEVEL_MONEY_3);
    const PRODUCT_INFO* produce50 = (0 < ProductManger::getInstance()->getSize()) ? ProductManger::getInstance()->getProducts(2) : nullptr;
    _modle.CheckBox_2->setUserData(const_cast<PRODUCT_INFO*>(produce50));
    _vecCheckBoxs.push_back(_modle.CheckBox_2);
    //518元复选框
    _modle.CheckBox_3 = createCheckBox(root, "3", LEVEL_MONEY_5);
    const PRODUCT_INFO* produce518 = (0 < ProductManger::getInstance()->getSize()) ? ProductManger::getInstance()->getProducts(4) : nullptr;
    _modle.CheckBox_3->setUserData(const_cast<PRODUCT_INFO*>(produce518));
    _vecCheckBoxs.push_back(_modle.CheckBox_3);
    //30元复选框
    _modle.CheckBox_4 = createCheckBox(root, "4", LEVEL_MONEY_2);
    const PRODUCT_INFO* produce30 = (0 < ProductManger::getInstance()->getSize()) ? ProductManger::getInstance()->getProducts(1) : nullptr;
    _modle.CheckBox_4->setUserData(const_cast<PRODUCT_INFO*>(produce30));
    _vecCheckBoxs.push_back(_modle.CheckBox_4);
    //118元复选框
    _modle.CheckBox_5 = createCheckBox(root, "5", LEVEL_MONEY_4);
    const PRODUCT_INFO* produce118 = (0 < ProductManger::getInstance()->getSize()) ? ProductManger::getInstance()->getProducts(3) : nullptr;
    _modle.CheckBox_5->setUserData(const_cast<PRODUCT_INFO*>(produce118));
    _vecCheckBoxs.push_back(_modle.CheckBox_5);
    //998元复选框
//	_modle.CheckBox_6 = createCheckBox(root, "6", LEVEL_MONEY_6);
//    const PRODUCT_INFO* produce998 = (0 < ProductManger::getInstance()->getSize()) ? ProductManger::getInstance()->getProducts(5) : nullptr;
//    _modle.CheckBox_6->setUserData(const_cast<PRODUCT_INFO*>(produce998));
//	_vecCheckBoxs.push_back(_modle.CheckBox_6);
    //隐藏
    //_modle.CheckBox_7 = createCheckBox(root, "7", LEVEL_MONEY_Other);
    //_vecCheckBoxs.push_back(_modle.CheckBox_7);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    _modle.zhifubao->setVisible(false);
    _modle.weixin->setVisible(false);
#else
    _modle.IAP_Pay->setVisible(false);
#endif
}

Layout* GameStoreLayer::createStoreItemTemplate(const PRODUCT_INFO* productLeft,const PRODUCT_INFO* productRight, int pos) {
    // 商品列表背景图
    auto storeItem = Layout::create();
    storeItem->setContentSize(Size(1280, 155));
    auto itemWidth = storeItem->getContentSize().width;
    auto itemHeight = storeItem->getContentSize().height;

    auto btnLeft = showStoreMassage(productLeft,pos+1);
    storeItem->addChild(btnLeft,10);
    btnLeft->setPosition(Vec2(itemWidth*0.26f, itemHeight * 0.5f));
    if (productRight!=nullptr) {
        auto btnRight = showStoreMassage(productRight,pos+2);
        storeItem->addChild(btnRight,10);
        btnRight->setPosition(Vec2(itemWidth*0.74f, itemHeight * 0.5f));
    }
    return storeItem;
}

CheckBox*	 GameStoreLayer::createCheckBox(Node* parent, const std::string name, INT tag) {
    auto checkBox = static_cast<CheckBox*>(parent->getChildByName(name));
    checkBox->setTag(tag);
    checkBox->addEventListener(CC_CALLBACK_2(GameStoreLayer::chackBoxCallBack,this));
    return checkBox;
}

Button* GameStoreLayer::showStoreMassage(const PRODUCT_INFO* product,int pos) {
    auto storeBtn = Button::create(STORE_BUY, STORE_BUY);
    storeBtn->setScale(0.8f);
    auto btnW= storeBtn->getContentSize().width;
    auto btnH = storeBtn->getContentSize().height;
    storeBtn->setUserData(const_cast<PRODUCT_INFO*>(product));
    storeBtn->addTouchEventListener(Widget::ccWidgetTouchCallback(CC_CALLBACK_2(GameStoreLayer::buyEventCallBack, this)));

    char buffer[128];
    // 商品图片
    auto itemImage = ImageView::create(COIN_ICO);
    itemImage->setPosition(Vec2(btnW*0.15f, btnH*0.5f));
    sprintf(buffer, "platform/store/coin_%d.png", pos);
    itemImage->loadTexture(buffer);
    storeBtn->addChild(itemImage, 1, 1);
    // 文字介绍
    auto storeText = Text::create("", gFontFile, 25);
    storeText->setAnchorPoint(Vec2(0, 0.5));
    storeText->setPosition(Vec2(btnW*0.3f, btnH*0.7f));
    sprintf(buffer, GBKToUtf8("%lld 金币"), product->number);
    storeText->setString(buffer);
    storeBtn->addChild(storeText, 1, 2);
    // 价格
    auto priceText = Text::create("", gFontFile, 25);
    priceText->setColor(colorGold);
    priceText->setAnchorPoint(Vec2(1, 0.5));
    priceText->setPosition(Vec2(btnW*0.85f, btnH*0.7f));
    sprintf(buffer, GBKToUtf8("价格 %.2f元"), product->price);
    priceText->setString(buffer);
    storeBtn->addChild(priceText, 1, 3);

    //兑换价格
    if ((int)product->price != 0) {
        auto replacePrice = Text::create("", gFontFile, 24);
        replacePrice->setAnchorPoint(Vec2(0, 0.5));
        replacePrice->setPosition(Vec2(btnW*0.3f, btnH*0.3f));
        sprintf(buffer, GBKToUtf8(" 1 元 = %d 金币"), (int)product->number * 100 / (int)(product->price * 100));//改
        replacePrice->setString(buffer);
        storeBtn->addChild(replacePrice, 1, 4);
    }

    return storeBtn;
}

void GameStoreLayer::buttonCallBack(Ref* pSender, Widget::TouchEventType type) {
    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::BEGAN:
        break;
    case cocos2d::ui::Widget::TouchEventType::MOVED:
        break;
    case cocos2d::ui::Widget::TouchEventType::ENDED: {
        HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
        if (nullptr == _product) {
            HNLog::logInfo("-----product null");
            return;
        }
        Button* selectedBtn = (Button*)pSender;
        if (selectedBtn->getName() == Weixin_Pay_Tag) {//微信支付
            GamePromptLayer::create()->showPrompt(GBKToUtf8("程序猿使劲开发中。"));
        } else if (selectedBtn->getName() == Alipay_Tag) {//支付宝支付
            _product->payType = PRODUCT_PAY_TYPE_ZHIFUBAO;
            HNLog::logInfo("-----zhifubao");
            executePayment(_product);
        } else if (selectedBtn->getName() == IAP_Tag) {//苹果内购
            if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS && _IAP_supported) {
                _product->payType = PRODUCT_PAY_TYPE_IAP;
                CALLBACK_PRAGMA pragma(HN_PAY_CALLBACK(GameStoreLayer::payCB, this));
                ProductManger::getInstance()->startPay(_product, &pragma);
            }
        }

        break;
    }
    default:
        break;
    }

}


//void GameStoreLayer::textFieldCallBack(cocos2d::Ref *pSender, cocos2d::ui::TextFiledEventType eventtype)
//{
//	auto textfiled = static_cast<TextField *>(pSender);
//	switch (eventtype)
//	{
//	case TextFiledEventType::TEXTFIELD_EVENT_DELETE_BACKWARD:
//	case TextFiledEventType::TEXTFIELD_EVENT_INSERT_TEXT:
//		if (textfiled->getName() == "")
//		{
//		}
//		else
//		{
//		}
//		break;
//	default:
//		break;
//	}
//}


void GameStoreLayer::chackBoxCallBack(Ref* sender, CheckBox::EventType type) {

    auto box = static_cast<CheckBox*>(sender);
    for (auto key : _vecCheckBoxs) {
        if (key->getName() != box->getName()) {
            key->setSelected(false);
        }
    }
    switch (type) {
    case CheckBox::EventType::SELECTED:
        if (box->getTag() != LEVEL_MONEY_Other) {
            _rechargeMoney = box->getTag();
            _product = (PRODUCT_INFO*)box->getUserData();
        } else {
            _rechargeMoney = atoll(_modle.TextField_otherMoney->getString().c_str());
            _product = nullptr;
        }
        break;
    case CheckBox::EventType::UNSELECTED:
        _rechargeMoney = 0;
        _product = nullptr;
        break;
    default:
        break;
    }
}


void GameStoreLayer::buyEventCallBack(Ref* pSender, Widget::TouchEventType type) {
    return;
    //废弃 by HBC

    switch (type) {
    case Widget::TouchEventType::ENDED: {

        HNAudioEngine::getInstance()->playEffect("music/btnEffect/button_click_sound.mp3");

        Button* selectedBtn = (Button*)pSender;
        _product = new PRODUCT_INFO;
        if (selectedBtn->getName() == Weixin_Pay_Tag) {
            GamePromptLayer::create()->showPrompt(GBKToUtf8("程序猿使劲开发中。"));
            return;
        } else if (selectedBtn->getName() == Alipay_Tag) {
            _product->payType = PRODUCT_PAY_TYPE_ZHIFUBAO;
        }

//        _product = (PRODUCT_INFO*)selectedBtn->getUserData();
        if (nullptr == _product) {
            return;
        }
        _product->number = _rechargeMoney;
        if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS && _IAP_supported) {
            _closeButton->setEnabled(false);
            _product->payType = PRODUCT_PAY_TYPE_IAP;
            CALLBACK_PRAGMA pragma(HN_PAY_CALLBACK(GameStoreLayer::payCB, this));
            ProductManger::getInstance()->startPay(_product, &pragma);
        } else {
            _product->payType = PRODUCT_PAY_TYPE_ZHIFUBAO;
            executePayment(_product);
        }
    }
    break;
    }
}

void GameStoreLayer::showPayLayer(const PRODUCT_INFO* product) {
    auto winSize = Director::getInstance()->getWinSize();
    //屏蔽后面的层
    colorLayer = LayerColor::create(Color4B(0,0,0,100));
    addChild(colorLayer,10);

    touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [=](Touch* touch ,Event* event) {
        return true;
    };
    touchListener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,colorLayer);
    //支付框
    payLoader = CSLoader::createNode("csb/platform/store/Node.csb");
    auto payLayout = (Layout*)payLoader->getChildByName("Panel_1");
    auto payLayoutSize = payLayout->getContentSize();
    auto payBtnClose= (Button*)payLayout->getChildByName("Button_close");
    auto payBtnWeixin= (Button*)payLayout->getChildByName("Button_weixin");
    auto payBtnZhifubao= (Button*)payLayout->getChildByName("Button_zhifubao");
    auto payBtnYinlian= (Button*)payLayout->getChildByName("Button_yinlian");
    auto payBtnXinyongka= (Button*)payLayout->getChildByName("Button_xinyongka");
    payLoader->setPosition(winSize/2);
    payLoader->setScale(0);
    addChild(payLoader,10);
    payLoader->runAction(ScaleTo::create(0.2f,1.0f));

    payBtnClose->addTouchEventListener(CC_CALLBACK_2(GameStoreLayer::closePayCallBack, this));
    payBtnWeixin->addTouchEventListener(CC_CALLBACK_2(GameStoreLayer::closePayCallBack, this));
    payBtnZhifubao->addTouchEventListener(CC_CALLBACK_2(GameStoreLayer::closePayCallBack, this));
    payBtnYinlian->addTouchEventListener(CC_CALLBACK_2(GameStoreLayer::closePayCallBack, this));
    payBtnXinyongka->addTouchEventListener(CC_CALLBACK_2(GameStoreLayer::closePayCallBack, this));

    char buffer[128];
    // 文字信息
    auto storeText = Text::create("", gFontFile, 25);
    storeText->setPosition(Vec2(payLayoutSize.width/4, payLayoutSize.height/2+90));
    sprintf(buffer, GBKToUtf8("商品: %lld"), product->number);
    storeText->setString(buffer);
    payLayout->addChild(storeText,10);

    // 价格信息
    auto priceText= Text::create("", gFontFile, 25);
    priceText->setPosition(Vec2(payLayoutSize.width*3/4,payLayoutSize.height/2+90));
    sprintf(buffer, GBKToUtf8("价格：%.2f元"), product->price);
    priceText->setString(buffer);
    payLayout->addChild(priceText, 10);
}

//支付回调
void GameStoreLayer::payCB(const std::string& args) {
    //bool ret = ProductManger::getInstance()->parsePayResult(args);

    HNLog::logInfo("----pay result:%s",args.c_str());
    std::string ext = "";
    if ((PRODUCT_PAY_TYPE_IAP == _product->payType) && _IAP_supported) {
//        _closeButton->setEnabled(true);

        bool ret = ProductManger::getInstance()->parsePayResult(args, ext);
        if (ret) {
            payCallback_IAP(ext);
        }
    } else {
        bool ret = ProductManger::getInstance()->parsePayResult(args);
        if (ret) {
            if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) {
                this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([&]() {
                    payCallback();
                }), nullptr));
            } else {
                payCallback();
            }
        }
    }
}
//关闭支付按钮回调
void GameStoreLayer::closePayCallBack(Ref* pSender, Widget::TouchEventType type) {
    Button* selectedBtn = (Button*)pSender;
    auto seqPay = Sequence::create(ScaleTo::create(0.1f, 0.3f),CCCallFunc::create([&]() {
        payLoader->removeFromParentAndCleanup(true);
        colorLayer->removeFromParentAndCleanup(true);
    }), nullptr);
    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::BEGAN: {
        selectedBtn->setColor(Color3B(170, 170, 170));
        break;
    }
    case cocos2d::ui::Widget::TouchEventType::ENDED: {
        HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);

        selectedBtn->setColor(Color3B(255, 255, 255));
        payLoader->runAction(seqPay);

        _product->payType = PRODUCT_PAY_TYPE_NONE;
        if (selectedBtn->getName().compare("Button_weixin") == 0)
            _product->payType = PRODUCT_PAY_TYPE_WEIXIN;
        else if (selectedBtn->getName().compare("Button_zhifubao") == 0)
            _product->payType = PRODUCT_PAY_TYPE_ZHIFUBAO;
        else if (selectedBtn->getName().compare("Button_yinlian") == 0)
            _product->payType = PRODUCT_PAY_TYPE_YINLIAN;
        else if (selectedBtn->getName().compare("Button_xinyongka") == 0)
            _product->payType = PRODUCT_PAY_TYPE_XINYONGKA;

        if(PRODUCT_PAY_TYPE_NONE != _product->payType) {
            CALLBACK_PRAGMA pragma(HN_PAY_CALLBACK(GameStoreLayer::payCB, this));
            ProductManger::getInstance()->startPay(_product, &pragma);
        }
        break;
    }
    case cocos2d::ui::Widget::TouchEventType::CANCELED: {
        selectedBtn->setColor(Color3B(255, 255, 255));
        break;
    }
    default:
        break;
    }
}

//关闭按钮回调
void GameStoreLayer::closeEventCallBack(Ref* pSender, Widget::TouchEventType type) {

    Button* selectedBtn = (Button*)pSender;
    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::BEGAN:
        selectedBtn->setColor(Color3B(170, 170, 170));
        break;
    case cocos2d::ui::Widget::TouchEventType::ENDED:
        HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
        selectedBtn->setColor(Color3B(255, 255, 255));
        closeStore();
        break;
    case cocos2d::ui::Widget::TouchEventType::CANCELED:
        selectedBtn->setColor(Color3B(255, 255, 255));
        break;
    default:
        break;
    }
}

bool GameStoreLayer::payEventSelector(HNSocketMessage* socketMessage) {
    if (1 == socketMessage->messageHead.bHandleCode) {
        CCAssert(sizeof(MSG_GP_CS_NOTIFY_IOSPayResult) == socketMessage->objectSize, "MSG_GP_CS_NOTIFY_IOSPayResult is error.");
        MSG_GP_CS_NOTIFY_IOSPayResult* pIOSPayResult = (MSG_GP_CS_NOTIFY_IOSPayResult*)socketMessage->object;
        PlatformLogic()->loginResult.i64Money += pIOSPayResult->llMoney;

        auto loginResult = PlatformLogic()->getInstance()->loginResult;

        if (_storyOpen) {
            if (_selfBankCoin) {
                _selfBankCoin->setString(StringUtils::format("%lld", pIOSPayResult->llMoney + loginResult.i64Bank));
            }
            _selfBankCoin->setString(to_string(loginResult.i64Bank + pIOSPayResult->llMoney));
        } else {
            if(_selfCoin) {
                _selfCoin->setString(StringUtils::format("%lld", pIOSPayResult->llMoney + loginResult.i64Money));
            }
            _delegate->walletChanged(pIOSPayResult->llMoney + loginResult.i64Money);
        }

        if (nullptr != onCloseCallBack) {
            onCloseCallBack();
        }

        char buffer[256];
        sprintf(buffer, "购买成功 , 获得%lld金币！", _product->number);
        GamePromptLayer::create()->showPrompt(GBKToUtf8(buffer));
    }
    return true;
}

//请求支付
void GameStoreLayer::executePayment(PRODUCT_INFO* productInfo) {
    std::string url = addOrder_URL(PlatformLogic()->loginResult.szName, 1, "zhifubao");

    HNLog::logInfo("------add order url:%s", url.c_str());

    HttpRequest* request = new (std::nothrow) HttpRequest();
    request->setUrl(url.c_str());
    request->setRequestType(HttpRequest::Type::GET);

    request->setResponseCallback([=](HttpClient* client, HttpResponse* response) {
        log("get call back1");
        if (response->isSucceed()) {
            std::vector<char> *buffer = response->getResponseData();
            std::string resp(buffer->begin(), buffer->end());
            rapidjson::Document doc;
            doc.Parse<rapidjson::kParseDefaultFlags>(resp.c_str());
            if (!doc.IsObject()) {
                GamePromptLayer::create()->showPrompt(GBKToUtf8("数据格式不对！"));
                return;
            }
            int rs = 0;
            if (JSON_RESOVE(doc, "rs")) rs = doc["rs"].GetInt();
            if (JSON_RESOVE(doc, "OrderID")) _orderId = doc["OrderID"].GetString();

            HNLog::logInfo("------OrderID:%s", _orderId.c_str());

            if (rs) {
                productInfo->orderID.assign(_orderId);
                CALLBACK_PRAGMA pragma(HN_PAY_CALLBACK(GameStoreLayer::payCB, this));
                ProductManger::getInstance()->startPay(productInfo, &pragma);
                HNLog::logInfo("------startPay");
            }
        }
    });

    HttpClient::getInstance()->setTimeoutForConnect(10);
    HttpClient::getInstance()->sendImmediate(request);

    request->release();
}

//支付结果回调
void GameStoreLayer::payCallback() {
    std::string url = PlatformConfig::getInstance()->getOrderUrl();
    url.append("/public/xmlhttpuser.aspx");
    url.append("?type=GetOrder&OrderID=");
    url.append(_orderId);

    log("PayResult>%s", url.c_str());

    HttpRequest* request = new (std::nothrow) HttpRequest();
    request->setUrl(url.c_str());
    request->setRequestType(HttpRequest::Type::GET);

    request->setResponseCallback([=](HttpClient* client, HttpResponse* response) {
        if (response->isSucceed()) {
            std::vector<char> buffer = *response->getResponseData();
            std::string data;
            data.assign(buffer.begin(), buffer.end());

            HNLog::logInfo("----payCallback:%s",data.c_str());

            if (data.empty()) {
                log("data is empty");
                supportAlert();
                return;
            }

            rapidjson::Document doc;
            doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());

            if (doc.HasParseError()) {
                log("data HasParseError");
                supportAlert();
                return;
            }

            // 解析结果
            if (!doc.IsObject() || !doc.HasMember("rs") || !doc.HasMember("msg")) {
                log("data HasParseError2");
                supportAlert();
                return;
            }

            int rs = doc["rs"].GetInt();
            if (rs == 1) {
                if (doc.HasMember("WalletMoney")) {
                    LLONG money = atol(doc["WalletMoney"].GetString());
                    char buffer[100];
                    sprintf(buffer, "购买成功 , 获得%lld金币！", _product->number);
                    GamePromptLayer::create()->showPrompt(GBKToUtf8(buffer));
                    PlatformLogic()->loginResult.i64Money = money;
                    sprintf(buffer, "%lld", money);
                    if (_selfCoin != nullptr) {
                        _selfCoin->setString(buffer);
                    }
                    _delegate->walletChanged(money);
                } else if (doc.HasMember("BankMoney")) {
                    LLONG money = doc["BankMoney"].GetUint64();
                    char buffer[100];
                    sprintf(buffer, "购买成功 , 获得%lld金币！", _product->number);
                    GamePromptLayer::create()->showPrompt(GBKToUtf8(buffer));
                    PlatformLogic()->loginResult.i64Bank = money;
                    sprintf(buffer, "%lld", money);
                    if (_selfBankCoin != nullptr) {
                        _selfBankCoin->setString(buffer);
                    }
                    _delegate->bankChanged(money);
                } else {
                    log("data HasParseError3");
                    supportAlert();
                }
            } else {
                log("失败信息：%s", doc["msg"].GetString());
                log("data HasParseError4");
                supportAlert();
            }
        }
    });

    HttpClient::getInstance()->setTimeoutForConnect(3);
    HttpClient::getInstance()->sendImmediate(request);

    request->release();
}

void GameStoreLayer::payCallback_IAP(const std::string& data) {
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());

    if (doc.HasParseError()) {
        return;
    }

    // check code send to app store for server.
    std::string receiptData;
    if (doc.IsObject() && doc.HasMember("receipt-data")) {
        receiptData = doc["receipt-data"].GetString();
    }

    // orderid get from app store.
    std::string orderId;
    if (doc.IsObject() && doc.HasMember("order-id")) {
        orderId = doc["order-id"].GetString();
        _orderId = orderId;
    }

    std::string requestData;

    // 类型
    requestData.append("type=PaySucceedByIOS");

    // 用户名
    requestData.append("&userName=");
    requestData.append(PlatformLogic()->loginResult.szName);

    // 验证数据
    requestData.append("&receipt_data=");
    requestData.append(receiptData);

    // 订单号
    requestData.append("&OrderID=");
    requestData.append(orderId);

    // 支付金额
    requestData.append("&payMoney=");
    char ch[20] = { 0 };
    sprintf(ch, "%lf", _product->price);
    requestData.append(ch);

    // 支付方式
    requestData.append("&typeInfoLog=IAP");

    //  Modified by HBC on 2016-10-25
    //  修复IAP回调访问HTTP错误的BUG
    HttpRequest* request = new (std::nothrow) HttpRequest();
    request->setUrl(PlatformConfig::getInstance()->getPayCallbackUrl_iOS().c_str());
    request->setRequestType(HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(GameStoreLayer::onHttpResponse, this));
    request->setRequestData(requestData.c_str(), requestData.size());

    HttpClient::getInstance()->send(request);

    request->release();
}

void GameStoreLayer::onHttpResponse(network::HttpClient* client, network::HttpResponse* response) {
    std::vector<char> *buffer = response->getResponseData();
    std::string data;
    data.assign(buffer->begin(), buffer->end());

    if (data.empty()) {
        supportAlert();
        return;
    }
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());

    if (doc.HasParseError()) {
        supportAlert();
        return;
    }

    // 解析结果
    if (!doc.IsObject() || !doc.HasMember("rs") || !doc.HasMember("msg")) {
        supportAlert();
        return;
    }

    int rs = doc["rs"].GetInt();
    if (rs == 1) {
        if (doc.HasMember("WalletMoney")) {
            LLONG money = doc["WalletMoney"].GetUint64();
            char buffer[100];
            sprintf(buffer, "购买成功 , 获得%lld金币！", _product->number);
            GamePromptLayer::create()->showPrompt(GBKToUtf8(buffer));
            PlatformLogic()->loginResult.i64Money = money;
            sprintf(buffer, "%lld", money);
            if (_selfCoin != nullptr) {
                _selfCoin->setString(buffer);
            }
            _delegate->walletChanged(money);
        }
        if (doc.HasMember("BankMoney")) {
            LLONG money = doc["BankMoney"].GetUint64();
            char buffer[100];
            sprintf(buffer, "购买成功 , 获得%lld金币！", _product->number);
            GamePromptLayer::create()->showPrompt(GBKToUtf8(buffer));
            PlatformLogic()->loginResult.i64Bank = money;
            sprintf(buffer, "%lld", money);
            if (_selfBankCoin != nullptr) {
                _selfBankCoin->setString(buffer);
            }
            _delegate->bankChanged(money);
        }
        if (doc.HasMember("UserID")) {
            int uid = doc["UserID"].GetInt();
        }
    } else {
        supportAlert();
    }
}

void GameStoreLayer::supportAlert() {
    char buffer[100] = { 0 };
    sprintf(buffer, "发货失败（%s），请联系客服。", _orderId.c_str());
    GamePromptLayer::create()->showPrompt(GBKToUtf8(buffer));
}

void GameStoreLayer::supportFakeSucceed() {
    char buffer[100];
    sprintf(buffer, "购买成功 , 获得%lld金币！", _product->number);
    GamePromptLayer::create()->showPrompt(GBKToUtf8(buffer));
    LLONG money = PlatformLogic()->loginResult.i64Money += _product->number;
    PlatformLogic()->loginResult.i64Money = money;
    sprintf(buffer, "%lld", money);
    if (_selfCoin != nullptr) {
        _selfCoin->setString(buffer);
    }
    _delegate->walletChanged(money);
}

std::string GameStoreLayer::addOrder_URL(const char* userName, DOUBLE price, std::string payTypeInfo) {
    std::string url = PlatformConfig::getInstance()->getOrderUrl();
    url.append(StringUtils::format("type=AddOrder&userName=%s", userName));
    url.append(StringUtils::format("&payMoney=%.2f", price));
    url.append(StringUtils::format("&typeInfoLog=%s", payTypeInfo.c_str()));
    return url;
}

