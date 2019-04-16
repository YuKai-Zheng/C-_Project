#ifndef ServiceLayer_h__
#define ServiceLayer_h__

#include "HNUIExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "UI/Base/HNLayer.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class ServiceLayer : public HNLayer {
    struct SERVICE_UI {
        enum TAG {
            TEL_TAG	 = 1000,
            MESG_TAG,
            FEEDBACK_TAG,
        };
        Node* serviceLoader;
        Button* Button_Tel;
        Button* Button_Mesg;
        Button*	Button_FeedBack;
        Layout* serviceUILayout;
    } _serviceUI;

private:
    Sprite*			_bg;

    std::string		_phoneNumber;
    std::string		_msgNumber;
    std::string		_email;

public:
    ServiceLayer();
    virtual ~ServiceLayer();

    void showService(Node* parent, Vec2 vec, int zorder, int tag);

    void closeService();

public:
    virtual bool init() override;

    void setBGPositon(Vec2 vec);

private:
    void buttonEventCallBack(Ref* pSender, Widget::TouchEventType type, SERVICE_UI::TAG tag);

public:
    CREATE_FUNC(ServiceLayer);
};

#endif // ServiceLayer_h__
