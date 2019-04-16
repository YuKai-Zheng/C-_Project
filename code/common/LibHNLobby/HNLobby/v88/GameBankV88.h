#ifndef GameBankV88_h__
#define GameBankV88_h__

#include "HNLobby/PlatformDefine.h"
#include "HNLobby/Shop/ShopManager.h"
#include "HNNetExport.h"
#include "HNUIExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "network/HttpClient.h"
#include "../pattern/factory/gameBank/IGameBank.h"
#include<string.h>
#include "UI/Base/AUIButton.h"
#include "UI/HNEditBox.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace network;
using namespace std;

class GameBankV88 : public IGameBank, public ui::EditBoxDelegate  {
private:

    struct SAVE_VIEW { //存取界面
        Text*						bagMoneyText;				//背包金币
        Text*						bankMoneyText;				//银行金币

        CheckBox*					saveWordCheckBox;			//一键存放按钮
        CheckBox*					takeWordCheckBox;			//一键取款按钮
        CheckBox*					saveImageCheckBox;
        CheckBox*					takeImageCheckBox;

        AUIButton*					takeButton;
        AUIButton*					saveButton;

        HNEditBox *					moneyField;
        HNEditBox *					mima;

        bool						isAllSaveTouch;
        bool						isAllTakeTouch;
    } _saveView;

    struct GIVE_VIEW {	//赠送
        Text*						targetName;					//目标姓名
        Text*						bankMoney;					//银行金币
        AUIButton*						giveBtn;					//按钮赠送
        HNEditBox *					userId;						//赠送ID
        HNEditBox *					giveMoney;					//赠送金币
        HNEditBox *					mima;						//存取密码

        INT							oldGiveId;
        LLONG						oldGiveMoney;
    } _giveView;

    struct RECORD_VIEW { //记录界面
        ListView *			 listView;
        Button*				 leftBtn;
        Button*				 rightBtn;
    } _recordView;

    struct MODIFICATION_VIEW { //修改密码
        Text*				primitive;							//初始密码
        AUIButton*			modifyBtn;							//修改按钮
        HNEditBox *			curpass;							//旧密码
        HNEditBox *			pass;								//新密码
        HNEditBox *			yanZhengMa;							//新密码验证
    } _modifocationView;

private:
    enum CheckBox_Tag {
        save_word,
        save_image,
        take_word,
        take_iamge
    };

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

    UINT				_oldViewTag;
    Node*				_childViewNode;
    INT					_idxJIlu;
    INT					_toutleRecord;
    bool						_isOpenSaveView;
    bool						_isClose;
    bool						_bankOpen;
    bool						_openRecord;
    LLONG					_bankMoney;			// 钱柜
    LLONG					_walletMoney;		// 钱包的钱
    MenuDelegate*		_delegate;

    vector<Button *>			_vecButton;		// 按钮
	vector<const char*>			_vMenuTitle;	//title
    INT								_time;
    INT								_curpage;

    vector<TMSG_GP_BankTranRecord>			_vecRecord;

public:
    GameBankV88();
    virtual ~GameBankV88();

    virtual bool init() override;
    CREATE_FUNC(GameBankV88);

    virtual void open(Node* parent, Vec2 vec, int zorder, int tag) override;
    virtual void close() override;

    virtual void setMenuDelegate(MenuDelegate* delegate) override{
        _delegate = delegate;
    }

private:
    void starTime(float dt);

    //菜单栏
    void loadButton(ImageView* parent);
    void menuButtonCallBack(Ref* pSender, Widget::TouchEventType touchtype);
    void touchEffect(UINT tag);													// 子页面切换按钮点击效果
    void addDefaultView();														//初始界面
    //清除
    void clearCurViewDat();

    //存取
    void createSaveView(Node* parent);
    void checkBoxCallBack(Ref* sender, CheckBox::EventType type);
    void saveTakeMoneyCallBack(Ref* pSender, Widget::TouchEventType type);

    //赠送
    void createGiveView(Node* parent);
    void giveMoneyCallBack(Ref* pSender, Widget::TouchEventType type);

    //记录
    void createRecordView(Node* parent);
    void recordlistViewCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type);

    //修改
    void createModificationView(Node* parent);
    void modifyPasswordCallBack(Ref* pSender, Widget::TouchEventType type);

    void reqUserNameById(std::string userId);
    void setTargetUserNameById(network::HttpClient* client, network::HttpResponse* response);

    //赠送子页面 输入目标ID 显示玩家称呼
    void scheduleNameCallBack(); //做一个显示目标名称
    void scheduleMoneyCallBack();//
    //金钱转换 亿 万 格式
    void transition(Text * text, LLONG money);
    char * Gettime(unsigned long nSeconds);

private:
    //银行操作处理函数
    bool bankerEventSelector(HNSocketMessage* socketMessage);
    //银行转账处理函数
    bool bankerForwardSelector(HNSocketMessage* socketMessage);

private:
    virtual void editBoxReturn(ui::EditBox* editBox)override {};
    virtual void editBoxTextChanged(ui::EditBox* editBox);
    //赠送金币回调 显示玩家昵称和 金币转换
    virtual void editBoxEditingDidEnd(ui::EditBox* editBox)override;
};


#endif // GameBankV88_h__
