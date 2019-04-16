#ifndef __GAMEUSERDATA_LAYER_H__
#define __GAMEUSERDATA_LAYER_H__

#include "HNUIExport.h"
#include "HNNetExport.h"
#include "cocos2d.h"
#include "UI/Base/BTableView.h"
#include "UI/HNEditBox.h"
#include "UI/UserHead.h"

USING_NS_CC;

class GameUserDataLayer : public HNLayer, public ui::EditBoxDelegate {
    struct UI {
        Text* Label_LandName;
        Text* Label_UserNickName;
        Text* Label_Sex;
        ImageView*	Image_Sex;
        Text* Label_UserID;
        Text* Label_Money;
        Text* Label_BankMoney;
        Text* Label_Fascination;
        Text* Label_Bank;
        Text* Label_phone;
        Text* Label_geXing;

        Button*	Button_BindPhone;
        Button*	Button_Copy;
        Button*	Button_modify;
        Button*	Button_ModifyPassword;
        Button*	Button_Close;
        Button*	Button_NickName;
        Button*	Button_AutoGraph;

        Sprite*	SpriteBankMoney;
        Sprite*	SpriteGameMoney;
        Sprite*	SpriteUserId;
        Sprite*	autoGraph;
        Sprite*	autoGraphBg;
        Sprite*	nickNameBg;

        HNEditBox* HNEeditBoxNickName;
        HNEditBox* HNEeditBoxAutoGraph;

        bool	isSelectNick;
        bool	isSelectGraph;
        bool isNickSave;
        bool isGraphSave;
        Layout*	layout;
        UserHead*	userHead;
    } _ui;
    // 返回结构体给调用者获取 银行标签精灵 和 显示金币文本;


    Rect					_userRect;
    BTableView*		m_pTableView;
    ImageView*		scallViewBg;
    INT					_curSelectHeadId;

private:
    std::string		_verifyCode;
    bool				_canClose;
    bool				_canReplace;
    bool				_isSlider;
    bool				_isClose;

    LayerColor*	_layerColor;

public:
    CREATE_FUNC(GameUserDataLayer);

    GameUserDataLayer();
    virtual ~GameUserDataLayer();
    LayerColor* colorLayer;
    EventListenerTouchOneByOne* touchListener;
    int _idx;

    typedef std::function<void ()> UpDateHeadCallBack;
    typedef std::function<void(string nickName)>UpDateNickCallBack;

    UpDateHeadCallBack upDateHeadCallBack;
    UpDateNickCallBack	 upDateNickCallBack;
public:
    virtual bool init() override;

    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent) override;

public:

    void closePrompt();

    bool canClose() const {
        return _canClose;
    }

    void openUserLayer(Node* parent, Vec2 vec, int zorder, int tag);

    void closeUserLayer();

    void loadModifyHeadLayer();

private:
    Node* userDataLoader;

    void loadBackGround(const char* fileName);
    void loadTopLogo(const char* fileName);
    void loadKuang(const char* fileName);

    void closeCallBack(Ref* pSender, Widget::TouchEventType type);

    void modifyPasswordUIEventCallBack(Ref* pSender, Widget::TouchEventType type);
    //濞ｅ浂鍠楅弫濂告偨閵婏箑鐓曞鑸垫綑閸庢岸宕堕悙鍓佹
    void modifyUserHeadEventCallBack(Ref* pSender, Widget::TouchEventType type);

    void bindPhoneUIEventCallBack(Ref* pSender, Widget::TouchEventType type);
    //昵称 和 签名编辑按钮
    void nickNameEventCallBack(Ref* pSender, Widget::TouchEventType type);
    void autoGraphEventCallBack(Ref* pSender, Widget::TouchEventType type);
    void headButtonCallBack(Ref* pSender, Widget::TouchEventType type);
    void buttonCopyCallBack(Ref* pSender, Widget::TouchEventType type);


    //头像修改回调
    bool modifyOKSelector(HNSocketMessage* socketMessage);
    bool modifyNoSelector(HNSocketMessage* socketMessage);

    //个性签名个昵称修改回调
    bool modifyNickAndGraphOKSelector(HNSocketMessage* socketMessage);
    bool modifyNickAndGraphNoSelector(HNSocketMessage* socketMessage);
    bool modifyFailureSelector(HNSocketMessage* socketMessage);

    // 保存签名回调
    void modifyGraphButtonCallBack(Ref*pShender, Widget::TouchEventType type);
    // 保存昵称回调
    void modifyNickButtonCallBack(Ref*pShender, Widget::TouchEventType type);

    void closeEvent(Ref* pSender, Widget::TouchEventType type);

    virtual void editBoxReturn(ui::EditBox* editBox) override{};
};

#endif // __GAMEUSERDATA_LAYER_H__
