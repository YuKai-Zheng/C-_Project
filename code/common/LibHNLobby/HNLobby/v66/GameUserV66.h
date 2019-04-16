#ifndef __GameUser_V66_H__
#define __GameUser_V66_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "../pattern/factory/background/IBackground.h"
#include "../component/logic/GameSetLogic.h"
#include "HNNetProtocol/HNPlatformMessage.h"
#include "UI/HNEditBox.h"
#include "extensions/cocos-ext.h"
#include "../pattern/factory/gameUser/IGameUser.h"
#include "UI/UserHead.h"
#include "HNSocket/HNSocketMessage.h"
#include "UI/Base/AUIButton.h"
#include "HNLobby/v66/TopMenuCompositeV66.h"
#include "HNLobby/v66/BottomMenuCompositeV66.h"


using namespace cocos2d::ui;
using namespace cocos2d::extension;
using namespace HN;
USING_NS_CC;

class GameUserHeadV66;
class GameUserV66 : public IGameUser ,public ui::EditBoxDelegate {
public:
    virtual bool init() override;
    CREATE_FUNC(GameUserV66);

public:
    virtual void editBoxEditingDidBegin(ui::EditBox* editBox) override;
    virtual void editBoxReturn(ui::EditBox* editBox) override;
    virtual void open(Node* parent, Vec2 vec, int zorder, int tag) override;
    virtual void close() override;

public:
	TopMenuCompositeV66* m_TopMenuDelegate;
	void setTopMenuDelegate(TopMenuCompositeV66* topMenuDelegate)
	{
		m_TopMenuDelegate = topMenuDelegate;
	}

private:
    vector<Node*> _vContentnode;
    
    Button* backBtn;
    MSG_GP_R_LogonResult LogonResult;

    UserHead* userHead;
    GameUserHeadV66* userHeadsLayer;
	Node* _rootNode;
    Text* userID;
    Text* bankMoney;
    Text* bagMoney;
    AUIButton* modifyBtn;
    AUIButton* copyBtn;
    AUIButton*  inviteCodeBtn;

	Button  * bindPhone;
    Button  * unBindPhone;
    bool isNickSave;
//    bool isGraphSave;
    bool m_canClose;

    HNEditBox* HNEeditBoxNickName;
//    HNEditBox* HNEeditBoxAutoGraph;
    
    Sprite* m_btnTitle;
    
    MSG_GP_R_LogonResult LoginResult;
    
    HNEditBox* phone;
    HNEditBox* vertifyCode;
    AUIButton* sendBtn;
public:
    void createPersonalPanel();
    void createAccountSafePanel();
    void createDownloadDimensionCode();//创建下载二维码
	void setBindPhoneBtnImg();
    void updateBindInfo(bool isBind, bool isSuccess);
    void handleSendBtn(Ref* pSender, Widget::TouchEventType type);
    void handleGetBtn(Ref* pSender, Widget::TouchEventType type);
    
    void onHttpCodeResponse(HttpClient* client, HttpResponse* response);
    void onHttpBindResponse(network::HttpClient* client, network::HttpResponse* response);
private:
    // 修改按钮回调
    void modifyUserHeadEventCallBack(Ref* pSender, Widget::TouchEventType type);
    void buttonCopyCallBack(Ref* pSender, Widget::TouchEventType type);
    void handleBackBtnEvent(Ref* pSender, Widget::TouchEventType type);
    void modifySignature(std::string signature);
    void modifyNickname(std::string nickname);
    void loadModifyHeadLayer();
    bool modifyNickAndGraphOKSelector(HNSocketMessage* socketMessage);
    bool modifyFailureSelector(HNSocketMessage* socketMessage);
    bool modifyOKSelector(HNSocketMessage* socketMessage);
    void callbackSwitch(Ref* pSender, Control::EventType event);
    void codeCallBack(Ref* pSender, Widget::TouchEventType type);
    
    bool getInvitedCodeSelector(HNSocketMessage* socketMessage);
    
    void onBindPhoneCallback(Ref* pSender, Widget::TouchEventType type);
    
public:
    void openBindPhoneDialog(Node* parent);
};

#endif // __GameUser_V99_H__
