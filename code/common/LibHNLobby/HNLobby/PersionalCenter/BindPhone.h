#ifndef __BindPhone_h__
#define __BindPhone_h__

#include "HNUIExport.h"
#include "HNNetExport.h"
#include "cocos2d.h"
#include "UI/HNEditBox.h"
#include "UI/Base/HNLayer.h"
USING_NS_CC;

class BindPhoneLayer : public HNLayer, public ui::EditBoxDelegate {
    struct UI {
        Node* bindLoader;
        HNEditBox* TextField_PhoneNumber;
        HNEditBox* TextField_Code;

        Sprite* TextField_PhoneNumberBg;

        Button*	Button_Code;
        Button*	Button_OK;
        Button*	Button_Cancel;

        Sprite*	Sprite_Titile;

        ImageView*	layout;
        Text *	Text_PhoneNumber;
    } _ui;

    Rect				_userRect;
    Size				_backGroundSize;

private:
    std::string		_verifyCode;

    MSG_GP_R_LogonResult LogonResult;

public:
    CREATE_FUNC(BindPhoneLayer);

    BindPhoneLayer();
    virtual ~BindPhoneLayer();

public:
    virtual bool init() override;

    void show(Node* parent, int zorder, int tag);

    void close();
    Size getBackGroundSIze() {
        return _backGroundSize;
    }

private:
    void verifyCodeUIEventCallBack(Ref* pSender, Widget::TouchEventType type);

    void onHttpCodeResponse(network::HttpClient* client, network::HttpResponse* response);

    void okEventCallBack(Ref* pSender, Widget::TouchEventType type);

    void onHttpBindResponse(network::HttpClient* client, network::HttpResponse* response);

    void closeCallBack(Ref* pSender, Widget::TouchEventType type);

private:
    bool verifyCodeSelector(HNSocketMessage* socketMessage);

    bool bindPhoneSelector(HNSocketMessage* socketMessage);

    bool modifyUserInfoSelector(HNSocketMessage* socketMessage);


    virtual void editBoxReturn(ui::EditBox* editBox)override {};
};

#endif // __BindPhone_h__
