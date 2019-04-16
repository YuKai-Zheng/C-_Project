#ifndef __GAMESTORE_LAYER_H__
#define __GAMESTORE_LAYER_H__

#include "HNLobby/PlatformDefine.h"
#include "HNLobby/Shop/ShopManager.h"
#include "HNNetExport.h"
#include "HNUIExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "network/HttpClient.h"
#include "UI/Base/AUIButton.h"
USING_NS_CC;
using namespace HN;
using namespace ui;
using namespace cocostudio;
using namespace network;
using namespace std;


#define LEVEL_MONEY_1		6
#define LEVEL_MONEY_2		30
#define LEVEL_MONEY_3		50
#define LEVEL_MONEY_4		118
#define LEVEL_MONEY_5		518
#define LEVEL_MONEY_6		998
#define LEVEL_MONEY_Other	10001

class GameStoreLayer : public HNLayer {
public:
    typedef std::function<void ()> CloseCallBack;
    CloseCallBack	onCloseCallBack;

private:
    bool		_isClose;

    ListView* _storeList;
    Layout* createStoreItemTemplate(const PRODUCT_INFO* productleft, const PRODUCT_INFO* productright,int pos);
    Node* payLoader;
    LayerColor* colorLayer;
    EventListenerTouchOneByOne* touchListener;
    Button* _closeButton;
    vector<const PRODUCT_INFO *> _vecProductInfo;

    class Modle {
    public:
        Button*							close;
        AUIButton*					weixin;
        AUIButton*					zhifubao;
        AUIButton*                  IAP_Pay;

        Text*						text_TopMoney;

        TextField*				TextField_account;
        TextField*				TextField_accountOk;
        TextField*				TextField_otherMoney;

        CheckBox*				CheckBox_1;
        CheckBox*				CheckBox_2;
        CheckBox*				CheckBox_3;
        CheckBox*				CheckBox_4;
        CheckBox*				CheckBox_5;
//		CheckBox*				CheckBox_6;
        //CheckBox*				CheckBox_7;
    } _modle;

    long								_rechargeMoney;
    INT								_targetId;
    INT								_confimId;
    vector<CheckBox*>		_vecCheckBoxs;
    bool					_storyOpen; //银行功能是否开启

public:
    static GameStoreLayer* createGameStore(MoneyChangeNotify* delegate,bool open);

public:
    GameStoreLayer();
    virtual ~GameStoreLayer();

    virtual bool init(bool open);
    void setChangeDelegate(MoneyChangeNotify* delegate);

    void openStore(Node* parent, Vec2 vec, int zorder, int tag);
    void closeStore();

    void showPayLayer(const PRODUCT_INFO* product);
    Button* showStoreMassage(const PRODUCT_INFO* product,int pos);

    // 界面消息回调函数
private:
    void loadTopLogo(const char* fileName);
    void loadKuang(const char* fileName);
    void loadUI();


    void buyEventCallBack(Ref* pSender, Widget::TouchEventType type);

    void closeEventCallBack(Ref* pSender, Widget::TouchEventType type);

    void closePayCallBack(Ref* pSender, Widget::TouchEventType type);

    void buttonCallBack(Ref* pSender, Widget::TouchEventType type);
    //void textFieldCallBack(cocos2d::Ref *pSender, cocos2d::ui::TextFiledEventType eventtype);
    void chackBoxCallBack(Ref* sender, CheckBox::EventType type);

    CheckBox*	 createCheckBox(Node* parent,const std::string name,INT tag);
    TextField*	 createTextField(Node* parent, const std::string name);




    bool payEventSelector(HNSocketMessage* socketMessage);

    void executePayment(PRODUCT_INFO* productInfo);

    std::string addOrder_URL(const char* userName, DOUBLE price, std::string payType);

    // http请求响应
    void onHttpResponse(network::HttpClient* client, network::HttpResponse* response);

private:
    void payCB(const std::string& args);
    void payCallback();
    // 支付成功回调(苹果内购)
    void payCallback_IAP(const std::string& data);
    // 发货失败
    void supportAlert();
    //  伪发货成功
    void supportFakeSucceed();
    
private:
    std::string								_orderId; //订单号
    PRODUCT_INFO*					_product;
    MoneyChangeNotify*				_delegate;
    Text*										_selfCoin;//充值成功后刷新界面，改成成员
    Text*										_selfBankCoin;//银行金币值
    // 是否支持苹果内购
    bool										_IAP_supported;
};

#endif // __GAMESTORE_LAYER_H__
