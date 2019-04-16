#ifndef GamePrompt_h__
#define GamePrompt_h__

#include "HNUIExport.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "UI/Base/AUIButton.h"
#include "FontSize.h"

using namespace ui;
using namespace cocostudio;

///////////////////////////////////////////////////////////////////////////////////
class GamePromptTip : public LayerColor
{
public:
	GamePromptTip();
	~GamePromptTip();

	virtual bool init();
	static GamePromptTip* create(Node * pParent, const Color4B& color, GLfloat width=400, GLfloat height=30);
	void setGamePromptTip(const std::string& prompt, GLfloat xPos = WIN_SIZE.width / 2, GLfloat yPos = WIN_SIZE.height / 2);
};

///////////////////////////////////////////////////////////////////////////////////

class GamePromptLayer : public HNLayer {
    Rect _userRect;
    typedef std::function<void()> TimeCallback;

private:
    Text*					promptText;
    Button*              promptButtonOK;
    Node*                   promptLoader;
    Sprite*                 promptTitle;
    bool                   closeBtnExecuteEnsure;//关闭按钮执行确认操作
    
//    AUIButton*		promptButtonSure1;
//    AUIButton*		promptButtonSure2;
//    AUIButton*		promptButtonCancel;

//    AUIButton*      doubleSure;     //成对出现
//    AUIButton*      doubleCancel;   //成对出现


//    ImageView*		promptLessCoinImage;
//    Button*				_exitBtn;

//    ImageView*		_backGround;

    EventListenerTouchOneByOne*		touchListener;

    Label* timeText;
public:
    CREATE_FUNC(GamePromptLayer);

    GamePromptLayer();
    virtual ~GamePromptLayer();

public:

    virtual bool init() override;

    void showPrompt(const std::string& prompt);
//    void showPromptWithTwoButton(const std::string& prompt);
//    void showLessCoinPrompt(bool lessCoin, const std::string& prompt);
    void closePrompt();

    void setPromptCanSelect();

    void setCloseCallBack(std::function<void()> closeCB);
    void setOkayCallBack(std::function<void()> sure);
//    void setCancelCallBack(std::function<void()> cancel);

//    void setDoubleSureCallBack(std::function<void()> doubleSureCB);
//    void setDoucleCancelCallBack(std::function<void()> doubleCancelCB);

    void setTextFontSize(int size);

    void setPrompt(const std::string& prompt);

//    void setCloseExitBtn();
    //有关闭colorlayer的touch的需求
    void setColorLayerTouchEnabled(bool bTouch);

    Text* getPromptText();
    void setTime(int time);
    void updateTime(float dt);
    void setTimeCallback(TimeCallback callback);
private:
    void menuEventCallBack(Ref* pSender, Widget::TouchEventType type);
    void doubleButtonCallBack(Ref* pSender, Widget::TouchEventType type);

    void setPrompt(const std::string& prompt, const std::string& font, int size);

    void showLessCoin(bool show);

private:
    std::function<void()> _sure;
//    std::function<void()> _cancel;

//    std::function<void()> _doubleSure;
//    std::function<void()> _doubleCancel;

    std::function<void()> _closeCB;

    int mTime;
    TimeCallback m_callback;
};

#endif // GamePrompt_h__
