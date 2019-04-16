#ifndef __GameShop_V88_H__
#define __GameShop_V88_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include "UI/HNEditBox.h"
#include "../pattern/factory/background/IBackground.h"
#include "../pattern/factory/gameShop/IGameShop.h"
#include "../component/logic/GameSetLogic.h"
#include "HNNetProtocol/HNPlatformMessage.h"
#include "../Shop/ProductInfo.h"
#include "network/HttpClient.h"
#include "UI/Base/AUIButton.h"
using namespace cocos2d::ui;
using namespace cocos2d::extension;
using namespace HN;
//using namespace ;
USING_NS_CC;


class GameShopV88 : public IGameShop ,public ui::EditBoxDelegate {
public:
    virtual ~GameShopV88();
    virtual bool init() override;
    CREATE_FUNC(GameShopV88);

public:
    virtual void editBoxReturn(ui::EditBox* editBox)override {};
    virtual void open(Node* parent, Vec2 vec, int zorder, int tag) override;
    virtual void close() override;

public:
    virtual void setMenuDelegate(MenuDelegate* delegate)override;

private:
    Node* shopLayer;
    Text* bagMoney;
    Text* bankMoney;
    AUIButton* alipayBtn;
    AUIButton* wechatBtn;
    AUIButton* IAPBtn;
    // 是否支持苹果内购
    bool										_IAP_supported;
    std::string								_orderId; //订单号
    PRODUCT_INFO*					_product;
    bool		_isClose;

    ListView* productsList;

    vector<Sprite*>								m_vSelectSprite;
private:
    void selectItemCallback(Ref* pSender, Widget::TouchEventType type);
    void handlePayBtnEvent(Ref* pSender, Widget::TouchEventType type);
    void handleProductsListEvent(Ref* pSender, Widget::TouchEventType type);
    void handleBuyBtnEvent(Ref* pSender, Widget::TouchEventType type);

    void handleBackBtnEvent(Ref* pSender, Widget::TouchEventType type);
    void executePayment(PRODUCT_INFO* productInfo);
    std::string addOrder_URL(const char* userName, DOUBLE price, std::string payTypeInfo);
    void payCB(const std::string& args);
    void payCallback_IAP(const std::string& data);
    void payCallback();
    void onHttpResponse(network::HttpClient* client, network::HttpResponse* response);
    void supportAlert();
    //  伪发货成功
    void supportFakeSucceed();
    
private:
    void payOnWebWithBrower(DOUBLE price);
    
    Button* backBtn;
};

#endif // __GameShop_V99_H__
