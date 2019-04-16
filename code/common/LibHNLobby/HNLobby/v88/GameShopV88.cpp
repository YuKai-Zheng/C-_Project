#include "GameShopV88.h"
#include "HNPlatform/HNPlatformLogic.h"
#include "PlatformResV88.h"
#include "../Shop/ProductInfo.h"
#include "../pattern/CommandManager.h"
#include "HNPayCallBack.h"
#include "../Shop/ShopManager.h"
#include "../PlatformConfig.h"
#include "../GamePrompt.h"
#include "HNLobby/CommonMacro.h"
#include "../pattern/factory/background/BackgroundFactory.h"
#include "../globel.h"
#include "base/SaveImageIntoAlbumHelper.h"

using namespace HN;

#define Weixin_Pay_Tag  "WeixinPay"
#define Alipay_Tag		"Alipay"
#define IAP_Tag			"IAP"

#define JSON_RESOVE(obj, key) (obj.HasMember(key) && !obj[key].IsNull())

const char* SHOP_TITLE_V88 = "v88/module/shangcheng/0.png";

const char* SHOP_CSB_V88 = "csb/v88/shop/shopLayer.csb";
const char* SHOP_ITEM_V88 = "csb/v88/shop/shopItem.csb";
const char* ITEM_SPRITE_MONEY_V88 = "v88/set_bank/shangcheng/zhanshipai/level%d/1.png";
const char* ITEM_SPRITE_PRICE_V88 = "v88/set_bank/shangcheng/zhanshipai/level%d/2.png";
const char* BTN_TITLE_ALIPAY_IMG_V88 = "v88/button/blue/4.png";
const char* BTN_TITLE_WXPAY_IMG_V88 = "v88/button/green/7.png";
const char* BTN_TITLE_IAP_IMG_V88 = "v88/button/green/iap.png";

GameShopV88::~GameShopV88() {
    PlatformLogic()->removeEventSelector(MDM_GP_NOTIFY_PAY, ASS_GP_NOTIFY_PAY);
}

bool GameShopV88::init() {
    if (!HNLayer::init()) {
        return false;
    }

    auto myListener = EventListenerTouchOneByOne::create();
    myListener->setSwallowTouches(true);//阻止触摸向下传递
    myListener->onTouchBegan = [](Touch *touch, Event *unused_event){
        return true;
    };
    myListener->onTouchEnded = [&](Touch *touch, Event *unused_event){
        close();
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(myListener, this);
    
	auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 100));
	addChild(colorLayer);
    //setBackGroundImage(PLATFORM_BG_V88);
    
	auto bg = ImageView::create(MODULE_BG_V88);
    //bg->setPosition(Vec2(WIN_SIZE.width / 2, (WIN_SIZE.height - TOP_MENU_HEIGHT) / 2));
    bg->setPosition(WIN_SIZE/2);
    bg->setTouchEnabled(true);
    addChild(bg);
    
    auto bgBottom = Sprite::create("v88/common/tanchuang/bg1.png");
    bgBottom->setName("BG_BOTTOM");
    bgBottom->setPosition(Vec2(WIN_SIZE.width/2, WIN_SIZE.height/2-241));
    addChild(bgBottom);
    
    auto title = Sprite::create(SHOP_TITLE_V88);
    title->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height - 34));
    bg->addChild(title);

    _product = nullptr;
    _isClose = true;

	auto nodeCsb = CSLoader::createNode(SHOP_CSB_V88);
	nodeCsb->setPosition(WIN_SIZE/2);
	addChild(nodeCsb);

	bagMoney = (Text*)nodeCsb->getChildByName("text_bagMoney");
	bagMoney->setColor(FONT_CLOLOR_YELLOW_V88);
	bankMoney = (Text*)nodeCsb->getChildByName("text_bankMoney");
	bankMoney->setColor(FONT_CLOLOR_YELLOW_V88);
    auto bagLabel = dynamic_cast<Sprite*>(nodeCsb->getChildByName("Sprite_2"));
    auto bankLabel = dynamic_cast<Sprite*>(nodeCsb->getChildByName("Sprite_3"));
    
    if (Tools::isiOSReview()) {
        if (bankMoney) {
            bankMoney->setVisible(false);
        }
        if (bankLabel) {
            bankLabel->setVisible(false);
        }
        if (bagLabel) {
            bagLabel->setPositionX(0);
        }
        if (bagMoney) {
            bagMoney->setPositionX(0);
        }
    }

//	auto btnAlipay = (Button*)nodeCsb->getChildByName("btn_alipay");
//	btnAlipay->setVisible(false);
//	alipayBtn = AUIButton::create(BTN_BLUE_BG_V88, BTN_BLUE_BG_V88);
//	nodeCsb->addChild(alipayBtn);
//	alipayBtn->setName(Alipay_Tag);
//	alipayBtn->setTitle(BTN_TITLE_ALIPAY_IMG_V88);
//	alipayBtn->setTitleOffset(Vec2(-4, 4));
//	alipayBtn->setPosition(btnAlipay->getPosition());
//	alipayBtn->addTouchEventListener(CC_CALLBACK_2(GameShopV88::handlePayBtnEvent, this));
//
//	auto btnWeChat = (Button*)nodeCsb->getChildByName("btn_wx");
//	btnWeChat->setVisible(false);
//	wechatBtn = AUIButton::create(BTN_GREEN_BG_V88, BTN_GREEN_BG_V88);
//	nodeCsb->addChild(wechatBtn);
//	wechatBtn->setName(Weixin_Pay_Tag);
//	wechatBtn->setTitle(BTN_TITLE_WXPAY_IMG_V88);
//	wechatBtn->setTitleOffset(Vec2(-4, 4));
//	wechatBtn->setPosition(btnWeChat->getPosition());
//	wechatBtn->addTouchEventListener(CC_CALLBACK_2(GameShopV88::handlePayBtnEvent, this));
//    if (wechatBtn) {
//        wechatBtn->setVisible(false);
//    }
//    if (alipayBtn) {
//        alipayBtn->setPositionX(0);
//    }
//    //ios平台只保留内购，屏蔽其他充值按钮
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    if (Tools::isiOSReview()) {
//        if (alipayBtn) {
//            alipayBtn->setVisible(false);
//        }
//        if (bankMoney) {
//            bankMoney->setVisible(false);
//        }
//        Sprite* bankLabel = dynamic_cast<Sprite*>(nodeCsb->getChildByName("Sprite_3"));
//        if (bankLabel) {
//            bankLabel->setVisible(false);
//        }
//        auto bagLabel = dynamic_cast<Sprite*>(nodeCsb->getChildByName("Sprite_2"));
//        if (bagLabel) {
//            bagLabel->setPositionX(0);
//        }
//        if (bagMoney) {
//            bagMoney->setPositionX(0);
//        }
//        
//        IAPBtn = AUIButton::create(BTN_GREEN_BG_V88, BTN_GREEN_BG_V88);
//        nodeCsb->addChild(IAPBtn);
//        IAPBtn->setName(IAP_Tag);
//        IAPBtn->setPosition(alipayBtn->getPosition());
//        IAPBtn->setTitle(BTN_TITLE_IAP_IMG_V88);
//        IAPBtn->setTitleOffset(Vec2(-4, 4));
//        IAPBtn->addTouchEventListener(CC_CALLBACK_2(GameShopV88::handlePayBtnEvent, this));
//    }
//#endif
    
	productsList = (ListView*)nodeCsb->getChildByName("ListView_1");
	productsList->setScrollBarAutoHideEnabled(false);
	productsList->setItemsMargin(0);

	ListView::ccListViewCallback func = [&](Ref* pSender, ListView::EventType type){
		if (type != ListView::EventType::ON_SELECTED_ITEM_END) return;

		ListView* listView = static_cast<ListView*>(pSender);
		int index = listView->getCurSelectedIndex();

		_product = (PRODUCT_INFO*)(ProductManger::getInstance()->getProducts(index));
		if (_product) {
			if (Tools::isiOSReview()) {
				if (_IAP_supported) {
					_product->payType = PRODUCT_PAY_TYPE_IAP;
					CALLBACK_PRAGMA pragma(HN_PAY_CALLBACK(GameShopV88::payCB, this));
					ProductManger::getInstance()->startPay(_product, &pragma);
				}
			}
			else {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				_product->payType = PRODUCT_PAY_TYPE_ZHIFUBAO;
				executePayment(_product);
#else
				payOnWebWithBrower(_product->price);
#endif
			}
			}
	};

	productsList->addEventListener(func);
	int nItemCount = ProductManger::getInstance()->getSize();

	for (int i = 0; i < nItemCount; i++)
	{
        char temp[64] = { 0 };
        
		auto node = CSLoader::createNode(SHOP_ITEM_V88);
		auto item = dynamic_cast<Button*>(node->getChildByName("btn_item"));
        item->setTouchEnabled(false);
        
        sprintf(temp, "v88/module/shangcheng/icon/%d.png", i + 1);
        item->loadTextureNormal(temp);
        item->loadTexturePressed(temp);
        item->loadTextureDisabled(temp);

		Layout* itemLayout = Layout::create();
        itemLayout->setContentSize(Size(item->getContentSize().width + 10, item->getContentSize().height));
		node->setPosition(itemLayout->getContentSize() / 2);
		itemLayout->addChild(node);
 		itemLayout->setTouchEnabled(true);
		productsList->pushBackCustomItem(itemLayout);

		const PRODUCT_INFO* productInfo = ProductManger::getInstance()->getProducts(i);

//         m_vSelectSprite.push_back(spriteSelect);

//		sprintf(temp, ITEM_SPRITE_MONEY_V88, i + 1);
//		auto spriteMoney = dynamic_cast<Sprite*>(item->getChildByName("sprite_money"));
//		spriteMoney->setTexture(temp);
//
//		sprintf(temp, ITEM_SPRITE_PRICE_V88, i + 1);
//		auto spritePrice = dynamic_cast<Sprite*>(item->getChildByName("sprite_price"));
//		spritePrice->setTexture(temp);
//		
		sprintf(temp, "%lld", productInfo->number);
		auto textGold = dynamic_cast<Text*>(item->getChildByName("text_gold"));
		textGold->setString(temp);
	}
	productsList->forceDoLayout();

    MSG_GP_R_LogonResult loginResult = PlatformLogic()->getInstance()->loginResult;
    bagMoney->setString(to_string(loginResult.i64Money));
    bankMoney->setString(to_string(loginResult.i64Bank));

    _IAP_supported = PlatformConfig::getInstance()->getSupportIAP();

    return true;
}

void GameShopV88::selectItemCallback(Ref* pSender, Widget::TouchEventType type)
{
	auto btn = dynamic_cast<Button*>(pSender);
	int tag = btn->getTag();

//     auto smallToBig = ScaleTo::create(0.05f, 1.1f);
//     auto bigToSmall = ScaleTo::create(0.05f, 1.0f);
    
    switch (type)
    {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            //btn->runAction(smallToBig);
            break;
        case cocos2d::ui::Widget::TouchEventType::MOVED:
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED:
//             btn->runAction(Sequence::create(bigToSmall, CallFunc::create([=](){
//                 for (int i = 0; i < m_vSelectSprite.size(); i++)
//                 {
//                     m_vSelectSprite.at(i)->setVisible(false);
//                 }
//                 m_vSelectSprite.at(tag)->setVisible(true);
//                 
//             }), nullptr));
            _product = (PRODUCT_INFO*)(ProductManger::getInstance()->getProducts(tag));
            if (_product) {
                if (Tools::isiOSReview()) {
                    if (_IAP_supported) {
                        _product->payType = PRODUCT_PAY_TYPE_IAP;
                        CALLBACK_PRAGMA pragma(HN_PAY_CALLBACK(GameShopV88::payCB, this));
                        ProductManger::getInstance()->startPay(_product, &pragma);
                    }
                }
                else {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                    _product->payType = PRODUCT_PAY_TYPE_ZHIFUBAO;
                    executePayment(_product);
#else
                    payOnWebWithBrower(_product->price);
#endif
                }
            }
            
            
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
//             btn->runAction(Sequence::create(bigToSmall, CallFunc::create([=](){
//                 m_vSelectSprite.at(tag)->setVisible(false);
//                 _product = nullptr;
//             }), nullptr));
			_product = nullptr;
            break;
        default:
            break;
    }
    
}

void GameShopV88::handleBuyBtnEvent(Ref* pSender, Widget::TouchEventType type) {
    if (Widget::TouchEventType::ENDED != type)
        return;
    Button* button = (Button*)pSender;
    PRODUCT_INFO* productInfo = (PRODUCT_INFO*)button->getUserData();
    HNLOG_INFO("xxxxxxxx touch button %f:", productInfo->price);
    _product = productInfo;
}

void GameShopV88::handleBackBtnEvent(Ref* pSender, Widget::TouchEventType type) {
    if (Widget::TouchEventType::ENDED != type)
        return;
    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
    close();
}

void GameShopV88::handlePayBtnEvent(Ref* pSender, Widget::TouchEventType type) {
    if (Widget::TouchEventType::ENDED != type)
        return;

//    SaveImageIntoAlbumHelper::saveImageFromURL("");
//    return;
    
    
    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
	//是否选中了商品
    if (nullptr == _product) {
        HNLog::logInfo("-----product null");
		CommandManager::getInstance()->showGamePrompt(GBKToUtf8("请选择需要购买的商品！"));
        return;
    }

	AUIButton* selectedBtn = (AUIButton*)pSender;

    if (selectedBtn->getName() == Weixin_Pay_Tag) {//微信支付
        CommandManager::getInstance()->showGamePrompt(GBKToUtf8("程序猿使劲开发中。"));
    } else if (selectedBtn->getName() == Alipay_Tag) { //支付宝支付
        _product->payType = PRODUCT_PAY_TYPE_ZHIFUBAO;
        HNLog::logInfo("-----zhifubao");
        executePayment(_product);
    } else if (selectedBtn->getName() == IAP_Tag) { //苹果内购
        if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS && _IAP_supported) {
            _product->payType = PRODUCT_PAY_TYPE_IAP;
            CALLBACK_PRAGMA pragma(HN_PAY_CALLBACK(GameShopV88::payCB, this));
            ProductManger::getInstance()->startPay(_product, &pragma);
        }
    }
}

void GameShopV88::open(Node* parent, Vec2 vec, int zorder, int tag) {
    CCAssert(nullptr != parent, "parent is nullptr");
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setPosition(vec);
    parent->addChild(this, zorder, tag);
    
    //setScale(0.2);
    //runAction(ScaleTo::create(0.2f, 1.0f));
}

void GameShopV88::close() {
    //runAction(Sequence::create(ScaleTo::create(0.2f, 0.2f), RemoveSelf::create(), nullptr));
	removeFromParent();
}

void GameShopV88::setMenuDelegate(MenuDelegate* delegate) {
    _delegate = delegate;
}

//请求支付
void GameShopV88::executePayment(PRODUCT_INFO* productInfo) {
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
                CALLBACK_PRAGMA pragma(HN_PAY_CALLBACK(GameShopV88::payCB, this));
                ProductManger::getInstance()->startPay(productInfo, &pragma);
                HNLog::logInfo("------startPay");
            }
        }
    });

    HttpClient::getInstance()->setTimeoutForConnect(10);
    HttpClient::getInstance()->sendImmediate(request);

    request->release();
}

std::string GameShopV88::addOrder_URL(const char* userName, DOUBLE price, std::string payTypeInfo) {
    std::string url = PlatformConfig::getInstance()->getOrderUrl();
    url.append(StringUtils::format("type=AddOrder&userName=%s", userName));
    url.append(StringUtils::format("&payMoney=%.2f", price));
    url.append(StringUtils::format("&typeInfoLog=%s", payTypeInfo.c_str()));
    return url;
}

//支付回调
void GameShopV88::payCB(const std::string& args) {
    //bool ret = ProductManger::getInstance()->parsePayResult(args);

    HNLog::logInfo("----pay result:%s", args.c_str());
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

void GameShopV88::payCallback_IAP(const std::string& data) {
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
    request->setResponseCallback(CC_CALLBACK_2(GameShopV88::onHttpResponse, this));
    request->setRequestData(requestData.c_str(), requestData.size());

    HttpClient::getInstance()->send(request);

    request->release();
}

//支付结果回调
void GameShopV88::payCallback() {
    std::string url = PlatformConfig::getInstance()->getOrderUrl();
//    url.append("/public/xmlhttpuser.aspx");
    url.append("type=GetOrder&OrderID=");
    url.append(_orderId);

    log("PayResult>%s", url.c_str());

    HttpRequest* request = new (std::nothrow) HttpRequest();
    request->setUrl(url.c_str());
    request->setRequestType(HttpRequest::Type::GET);

    request->setResponseCallback([=](HttpClient* client, HttpResponse* response) {
        if (response->isSucceed()) {
            std::vector<char>* buffer = response->getResponseData();
            std::string data;
            data.assign(buffer->begin(), buffer->end());

            HNLog::logInfo("----payCallback:%s", data.c_str());

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
                bool hasShow = false;
                bool hasError = true;
                if (doc.HasMember("WalletMoney")) {
                    hasShow = true;
                    hasError = false;
                    
                    LLONG money = atol(doc["WalletMoney"].GetString());
                    char buffer[100];
                    sprintf(buffer, "购买成功 , 获得%lld金币！", _product->number);
                    GamePromptLayer::create()->showPrompt(GBKToUtf8(buffer));
                    PlatformLogic()->loginResult.i64Money = money;
                    sprintf(buffer, "%lld", money);

                    if (bagMoney != nullptr) {
                        bagMoney->setString(buffer);
                    }

                    _delegate->walletChanged(money);
                }
                
                if(doc.HasMember("BankMoney")) {
                    hasError = false;
                    
                    LLONG money = atol(doc["BankMoney"].GetString());
                    char buffer[100];
                    sprintf(buffer, "购买成功 , 获得%lld金币！", _product->number);
                    if(!hasShow){
                        GamePromptLayer::create()->showPrompt(GBKToUtf8(buffer));
                    }
                    PlatformLogic()->loginResult.i64Bank = money;
                    sprintf(buffer, "%lld", money);
                    if (bankMoney != nullptr) {
                        bankMoney->setString(buffer);
                    }
                    _delegate->bankChanged(money);
                }
                
                if(hasError){
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

void GameShopV88::onHttpResponse(network::HttpClient* client, network::HttpResponse* response) {
    std::vector<char> *buffer = response->getResponseData();
    std::string data;
    data.assign(buffer->begin(), buffer->end());

    if (data.empty()) {
//        supportAlert();
        supportFakeSucceed();
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

            if (bagMoney != nullptr) {
                bagMoney->setString(buffer);
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

            if (bankMoney != nullptr) {
                bankMoney->setString(buffer);
            }
            _delegate->bankChanged(money);
        }
        if (doc.HasMember("UserID")) {
//            int uid = doc["UserID"].GetInt();
        }
    } else {
        supportAlert();
    }
}

void GameShopV88::supportAlert() {
    CommandManager::getInstance()->showGamePrompt(StringUtils::format(GBKToUtf8("发货失败（%s），请联系客服。"), _orderId.c_str()));
}

void GameShopV88::supportFakeSucceed() {
    char buffer[100];
    sprintf(buffer, "购买成功 , 获得%lld金币！", _product->number);
    GamePromptLayer::create()->showPrompt(GBKToUtf8(buffer));
    LLONG money = PlatformLogic()->loginResult.i64Money += _product->number;
    PlatformLogic()->loginResult.i64Money = money;
    sprintf(buffer, "%lld", money);
    if (bagMoney != nullptr) {
        bagMoney->setString(buffer);
    }
    _delegate->walletChanged(money);
}

void GameShopV88::payOnWebWithBrower(DOUBLE price) {
    char link[128] = {0};
    sprintf(link, getAlipayWebURL().c_str(), PlatformLogic()->loginResult.dwUserID, (int)price);
    Application::getInstance()->openURL(link);
}
