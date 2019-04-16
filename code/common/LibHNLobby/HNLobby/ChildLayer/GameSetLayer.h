#ifndef __GAMESET_LAYER_H__
#define __GAMESET_LAYER_H__

#include "HNUIExport.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "UI/Base/HNLayer.h"

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

class GameSetLayer : public HNLayer {
    enum UI_TYPE {
        M_SLIDER = 28,                           //音乐拖动条
        E_SLIDER = 32,                           //音效拖动条
        M_CHECKBOX = 40,                         //音乐静音复选框
        E_CHECKBOX = 41,                         //音效静音复选框
    };
    Slider* _musicSlider;
    Slider* _effectSlider;
    CheckBox* _musicCheckBox;
    CheckBox* _effectCheckBox;
    Layout* _layout;

    struct PUBLIC_BUTTON { //按钮
        Button*		save;
        Button*		give;
        Button*		record;
        Button*		mondification;
    } _structBtn;

    enum BTNTAG {
        createQianming_Btn = 1000,
        creatGaiMi_Btn,
        creatBound_Btn,
        creatMusicEffect_Btn,
        creatHelp_Btn,
        creatShenMing_Btn,
        close_Btn,


    };

    bool								_isClose;

    Button*							_closeBtn;
    Node*							_btnRoot;
    Sprite*							_kuang;
    Node*							_childViewNode; //子页面父节点

    vector<Sprite*>				_vecNormal;		//按钮没有点击状态
    vector<Sprite*>				_vecPrass;			// 按钮点击状态
    vector<Button *>			_vecButton;		// 按钮

    int								_oldViewTag;    //已点击按钮
public:
    GameSetLayer();
    virtual ~GameSetLayer();

public:
    virtual bool init() override;
    bool onTouchBegan(Touch *touch, Event *unused_event)override;
    void onTouchMoved(Touch *touch, Event *unused_event)override;
    void onTouchEnded(Touch *touch, Event *unused_event)override;
    void showSet(Node* parent, int zorder, int tag = -1);
    void closeSet();

    void openSetLayer(Node* parent, Vec2 vec, int zorder, int tag);
    void closeSetLayer();
private:
    LayerColor* colorLayer;
    Node* settingLoader;
    Slider* effectSlider;
    Slider* musicSlider;
    ////关闭回调
    //void closeSetCallBack(Ref* pSender, Widget::TouchEventType type);
    ////拖动条回调函数
    //void sliderCallback(Ref* pSender, Slider::EventType type, UI_TYPE num);
    ////控制音效
    //void setCallBack(Ref* pSender,Widget::TouchEventType type,int num);

    void btnCallBack(Ref* pSender, Widget::TouchEventType type);

public:
    CREATE_FUNC(GameSetLayer);
private:
    //初始化子界面按钮
    void loadButton();

public:

    void loadBackGround(const char* fileName);
    void loadTopLogo(const char* fileName);
    void loadKuang(const char* fileName);
    //按钮点击效果
    void touchEffect(unsigned int tag);

public:
    ////默认子界面
    //void clearCurViewDat();

    void createGeXingView(Node* parent);

    void createXiuGaiView(Node* parent);

    void createBoundView(Node* parent);

    void createMusicView(Node* parent);

    void createHelpView(Node* parent);

    void createShenMingView(Node* parent);



};

#endif // __GAMESET_LAYER_H__