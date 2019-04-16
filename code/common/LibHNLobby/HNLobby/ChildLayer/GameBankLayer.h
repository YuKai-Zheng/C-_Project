#ifndef GameBankerLayer_h__
#define GameBankerLayer_h__

#include "HNLobby/PlatformDefine.h"
#include "HNLobby/Shop/ShopManager.h"
#include "HNNetExport.h"
#include "HNUIExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "network/HttpClient.h"
#include<string.h>
#include "UI/HNEditBox.h"
#include "UI/LoadingLayer.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace network;
using namespace std;

class GameBankLayer : public HNLayer, public ui::EditBoxDelegate  {
private:
    struct PUBLIC_BUTTON { //按钮
        Button*		save;
        Button*		give;
        Button*		record;
        Button*		mondification;
    } _structBtn;

    struct SAVE_VIEW { //存取界面
        Text*						gameMoney;					//游戏金币
        Text*						bankMoney;					// 银行金币
        CheckBox*				allButton;						//一键存放按钮
        CheckBox*				quButton;						//一键取款按钮
        Button*					takeButton;					// 取
        Button*					saveButton;					// 存
        HNEditBox *			moneyField;					//输入框
        HNEditBox *			mima;							//存取密码
        ImageView*			allTakeSpr;					//全取图片
        ImageView*			allSaveSpr;					//全存图片
        bool						isAllSaveTouch;
        bool						isAllTakeTouch;
    } _saveView;

    struct GIVE_VIEW {													//赠送
        Text*						targetName;							//自己姓名
        Text*						giveGlob;								// 金币
        Text*						gameGlob;							// 金币
        Button*					modification;							//按钮赠送
        Button*					leftBtn;									//左翻页按钮
        Button*					rightBtn;								//右翻页
        HNEditBox *			userId;									//ID输入
        HNEditBox *			giveMoney;							//目标金币输入框
        HNEditBox *			mima;									//存取密码
        INT						oldGiveId;								//上次赠送ID
        LLONG					oldGiveMoney;						//上次赠送IDmoney
    } _giveView;

    struct RECORD_VIEW { //记录界面
        ListView *			 listView;
        Button*				 leftBtn;
        Button*				 rightBtn;
    } _recordView;

    struct MODIFICATION_VIEW { //修改密码
        Text*						primitive;							//初始密码
        Button*					giveBtn;								//修改按钮
        HNEditBox *			curpass;							//当前密码
        HNEditBox *			pass;									//目标密码
        HNEditBox *			yanZhengMa;					//验证码
    } _modifocationView;

private:

    enum btnTag {
        btn_save = 1000,
        btn_give,
        btn_record,
        btn_modification,
        btn_cun,
        btn_take,
        give_targetId,
        give_money,
        give_userId,
        page_leftBtn,
        page_rightBtn,
    };
    LoadingLayer*	_loadingLayer;
    UINT					_oldViewTag;
    Node*				_btnRoot;
    Node*				_childViewNode;
    Sprite*				_centerKuang;
    INT					_beginPage;
    INT					_endPage;
    INT					_idxJIlu;
    INT					_recordTag;
    INT					_toutleRecord;
    bool						_isOpenSaveView;
    bool						_isClose;
    bool						_bankOpen;
    bool						_openRecord;
    LLONG					_bankMoney;			// 钱柜
    LLONG					_walletMoney;		// 钱包的钱
    MoneyChangeNotify*		_delegate;

    vector<Sprite*>				_vecNormal;		//按钮没有点击状态
    vector<Sprite*>				_vecPrass;			// 按钮点击状态
    vector<Button *>			_vecButton;		// 按钮
    INT								_time;
    INT								_curpage;

    vector<TMSG_GP_BankTranRecord>			_vecRecord;

public:
    static GameBankLayer* createGameBank();

public:
    GameBankLayer();
    virtual ~GameBankLayer();

    virtual bool init() override;

    void openBank(Node* parent, Vec2 vec, int zorder, int tag);

    void closeBank();

    void setChangeDelegate(MoneyChangeNotify* delegate) {
        _delegate = delegate;
    }

private:
    void starTime(float dt);
    //默认子界面
    void clearCurViewDat();

    void createSaveView(Node* parent);

    void createGiveView(Node* parent);

    void createRecordView(Node* parent);

    void createModificationView(Node* parent);

    void imageVIewCallBack(Ref *pSender, Widget::TouchEventType type);

    void loadButton();

    void reqUserNameById(std::string userId);
    void setTargetUserNameById(network::HttpClient* client, network::HttpResponse* response);

    void addDefaultView();
    // 子页面切换按钮点击效果
    void touchEffect(UINT tag);
    //按钮回调
    void buttonCallBack(Ref* pSender, Widget::TouchEventType touchtype);
    void chackBoxCallBack(Ref* sender, CheckBox::EventType type);
    void pageTurnCallBack(Ref* pSender, Widget::TouchEventType touchtype);
    //赠送子页面 输入目标ID 显示玩家称呼
    void scheduleNameCallBack(); //做一个显示目标名称
    void scheduleMoneyCallBack();//
    // 添加转账记录
    void addListViewItem(UINT begin, UINT end);
    //金钱转换 亿 万 格式
    void transition(Text * text, LLONG money);

    char * Gettime(unsigned long nSeconds);

    void listViewItemCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type);
    //	// 界面消息回调函数
private:
    void closeEvent(Ref* pSender, Widget::TouchEventType type);
private:
    //	// 银行操作处理函数
    bool bankerEventSelector(HNSocketMessage* socketMessage);
    //	// 银行转账处理函数
    bool bankerForwardSelector(HNSocketMessage* socketMessage);
    //
private:
    void operatorUIEventCheckBoxCallBack(Ref* pSender, CheckBox::EventType type);
    void operatorUIEventCallBack(Ref* pSender, Widget::TouchEventType type);
    //
private:
    void userDataUIEventCallBack(Ref* pSender, Widget::TouchEventType type);
    //
private:
    void forwardUIEventCallBack(Ref* pSender, Widget::TouchEventType type);
    void forwardUICheckBoxEvent(Ref* pSender, CheckBox::EventType type);

    virtual void editBoxReturn(ui::EditBox* editBox)override {};
    //赠送金币回调 显示玩家昵称和 金币转换
    virtual void editBoxEditingDidEnd(ui::EditBox * 	editBox) override;

    //
    //private:
    //	//请求转账按钮是否显示
    //	void reqIsShow();
    //
    //	// 获取转账按钮状态回调
    //	void onHttpResponse(network::HttpClient* client, network::HttpResponse* response);
};


#endif // GameBankerLayer_h__
