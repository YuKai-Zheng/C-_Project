#ifndef __GAMESET_LAYERV88_H__
#define __GAMESET_LAYERV88_H__

#include "HNUIExport.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "HNLobby/pattern/factory/gameSet/IGameSet.h"
#include "../component/logic/GameSetLogic.h"
#include "UI/HNEditBox.h"
#include "UI/Base/AUIButton.h"
//#include "BRNNRes.h"

USING_NS_CC;
//using namespace BRNN;
using namespace cocostudio;
using namespace ui;

#define BRNN_SET_BG								 "NN/cow/hundred/set/1.png"
#define BRNN_SET_SOUND							 "NN/cow/hundred/set/3.png"
#define BRNN_SET_MUSIC							 "NN/cow/hundred/set/2.png"
#define BRNN_SET_SCROLL_BAR_BALL				 "NN/cow/hundred/set/5.png"
#define BRNN_SET_SCROLL_BAR_BG					 "NN/cow/hundred/set/4.png"
#define BRNN_SET_SCROLL_BAR_FOREBG				 "NN/cow/hundred/set/6.png"

class GameSetLayerV88 : public IGameSet, public ui::EditBoxDelegate {

    enum MENU_TAG {
//		createSafe_Btn = 1000,
		creatShenMing_Btn,
        creatBound_Btn,
        creatMusicEffect_Btn,
        creatGaiMi_Btn,
        creatHelp_Btn,
		createQianming_Btn,
    };

    bool						_isClose;
    int							_oldViewTag;    //已点击按钮
	bool						m_isBind;

    Node*						_childViewNode; //子页面父节点
    vector<Button *>			_vecButton;		// 按钮

public:
    GameSetLayerV88();
    virtual ~GameSetLayerV88();

public:
    virtual bool init() override;
    CREATE_FUNC(GameSetLayerV88);
public:
    virtual void editBoxReturn(ui::EditBox* editBox) override{};
    virtual void open(Node* parent, Vec2 vec, int zorder, int tag) override;
    virtual void close() override;
private:
    MSG_GP_R_LogonResult LoginResult;

private:
    Slider* effectSlider;
    Slider* musicSlider;
    ////关闭回调
    //void closeSetCallBack(Ref* pSender, Widget::TouchEventType type);
    ////拖动条回调函数
    //void sliderCallback(Ref* pSender, Slider::EventType type, UI_TYPE num);
    ////控制音效
    //void setCallBack(Ref* pSender,Widget::TouchEventType type,int num);


public:
    //初始化子界面按钮
    void loadButton();
    //按钮点击效果
    void touchEffect(unsigned int tag);
    void menuButtonCallBack(Ref* pSender, Widget::TouchEventType type);
	void handleSafeBtn(Ref* pSender, Widget::TouchEventType type);
	bool safeMessageCallBack(HNSocketMessage* socketMessage);
public:
    void createGeXingView(Node* parent);

    void createXiuGaiView(Node* parent);

    void createBoundView(Node* parent);

    void createMusicView(Node* parent);

    void createHelpView(Node* parent);

    void createShenMingView(Node* parent);

	void createSafeView(Node* parent);


public:
    HNEditBox* phone;
    HNEditBox* vertifyCode;
    AUIButton* sendBtn;

	HNEditBox* saveNewPass;
	HNEditBox* saveNewAcc;
	HNEditBox* saveNewPassSure;

	void updateBindInfo(bool isBind, bool isSuccess);
    void handleSendBtn(Ref* pSender, Widget::TouchEventType type);
    void handleGetBtn(Ref* pSender, Widget::TouchEventType type);

	void onHttpCodeResponse(HttpClient* client, HttpResponse* response);
	void onHttpBindResponse(network::HttpClient* client, network::HttpResponse* response);
public:
    //改密
    HNEditBox* oldPwd;
    HNEditBox* newPwd;
    HNEditBox* surePwd;
	AUIButton* modifyBtn;
	AUIButton* tijiaoBtn;

	std::string newUserPwd;

    void handleModifyBtn(Ref* pSender, Widget::TouchEventType type);
	bool modifyPasswordSelector(HNSocketMessage* socketMessage);

};

#endif // __GAMESET_LAYER_H__
