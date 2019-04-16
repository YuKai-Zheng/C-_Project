#ifndef GameRegisterCompositeV66_h
#define GameRegisterCompositeV66_h

#include "HNNetExport.h"
//#include "HNUIExport.h"
//#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ui/UICheckBox.h"
#include "../pattern/factory/gameRegister/IGameRegister.h"
#include "UI/HNEditBox.h"
#include "UI/Base/AUIButton.h"

//USING_NS_CC;

using namespace cocostudio;
using namespace ui;
using namespace HN;

class GameRegisterCompositeV66 : public IGameRegister, public ui::EditBoxDelegate {
public:
    CREATE_FUNC(GameRegisterCompositeV66);

public:
    GameRegisterCompositeV66();
    virtual ~GameRegisterCompositeV66();

    virtual bool init() override;

private:
    struct REGIST_NODE {
        CheckBox*	manCheckBox;
        CheckBox*	womanCheckBox;
        HNEditBox*	accountEdit;
		HNEditBox*	codeEdit;
//        HNEditBox*	nickEdit;
        HNEditBox*	passwordEdit;
        HNEditBox*	pwCheckEdit;
        HNEditBox*  inviteCodeEdit;
        Button*		registerBtn;
        Button*		backBtn;
		Button*		codeBtn;
        Node*		node;
		Text*	    number;
    } _regist_node;

    std::string	_userName;
    std::string	_userPassword;
	std::string _userCode;//用户验证码;
    std::string _visiblePassword;

    BYTE				_gender;

    virtual void initRegisterUI() override;

private:
    void closeClickCallback(Ref* pSender, Widget::TouchEventType type);
    void registerClickCallback(Ref* pSender, Widget::TouchEventType type);
    void resetClickCallBack(Ref* pSender, Widget::TouchEventType type);
	void codeClickCallBack(Ref* pSender, Widget::TouchEventType type);

	void onHttpReqClientResponse(network::HttpClient *client, network::HttpResponse *response);

    // 复选框回调函数
    void checkBoxCallback(Ref* pSender,CheckBox::EventType type);
    //协议
    void AgreeClickCallBack(Ref* pSender, Widget::TouchEventType type);


    virtual void editBoxReturn(ui::EditBox* editBox)override {};

    void editBoxEditingDidEnd(ui::EditBox* editBox) override;
    void editBoxTextChanged(ui::EditBox* editBox, const std::string& text) override;

	void UpdateTimer(float dt);

	float _fTimeOut;

};

#endif /* GameRegisterCompositeV66_h */
