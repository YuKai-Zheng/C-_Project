#ifndef GameLandCompositeV66_h
#define GameLandCompositeV66_h

#include "HNNetExport.h"
#include "HNUIExport.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "UI/Base/AUIButton.h"
#include "../pattern/factory/gameLand/IGameLand.h"
#include "UI/HNEditBox.h"
using namespace HN;
USING_NS_CC;

using namespace cocostudio;
using namespace ui;

class GameLandCopositeV66 : public IGameLand, public ui::EditBoxDelegate {
//public:
//    typedef std::function<void (const std::string& name, const std::string& psd)> LoginCallBack;
//    typedef std::function<void ()> RegistCallBack;
//    typedef std::function<void()> FindPwCallBack;
//    typedef std::function<void(Ref * pSender, cocos2d::ui::Widget::TouchEventType type)> YouKeLoginCallBack;

private:
    struct LOGIN_NODE {
        HNEditBox* accountEdit;
		HNEditBox* codeEdit;
        HNEditBox* passwordEdit;
        CheckBox* keepPwCheckBox;
		Button * btnPwCheckBox;
		CheckBox*   haveReadBox;
		Button*		haveReadBtn;
        Button*	registerBtn;
        ImageView*	findPwBtn;
//        Button* weixinBtn;
//        Button* qqBtn;
        Button* exitBtn;

        Node* node;
        Button* loginBtn;
        AUIButton* youke;
//        AUIButton* wechat;
    } _login_node;

//public:
//    LoginCallBack	onLoginCallBack;
//    YouKeLoginCallBack	youKeLoginCallBack;
//    RegistCallBack  onRegistCallBack;
//    FindPwCallBack  onFindPwCallBack;

public:
    CREATE_FUNC(GameLandCopositeV66);

public:
    GameLandCopositeV66();
    virtual ~GameLandCopositeV66();

    virtual bool init() override;
	void onExitTransitionDidStart()override;
	void onEnter()override;
private:
    //LayerColor* colorLayer;
	bool m_bEdit;
	std::string m_strPassword;

    // 登陆回调函数
    void landEventCallback(Ref* pSender, Widget::TouchEventType type);
    // 关闭回调函数
    void closeEventCallback(Ref* pSender, Widget::TouchEventType type);
    // 复选框(保存密码)回调函数
    void checkBoxCallback(Ref* pSender,CheckBox::EventType type);
    // 注册按钮回调函数
    void registEventCallback(Ref* pSender, Widget::TouchEventType type);
    // 忘記密碼回調
    void findPwEventCallback(Ref* pSender, Widget::TouchEventType type);

    void weiXInLoginCallBack(Ref* pSender, Widget::TouchEventType type);

	void keepPasswordEventCallback(Ref* pSender, Widget::TouchEventType type);

	void haveReadEventCallback(Ref* pSender, Widget::TouchEventType type);

    virtual void editBoxReturn(ui::EditBox* editBox) override{};

};

#endif /* GameLandCompositeV99_h */
