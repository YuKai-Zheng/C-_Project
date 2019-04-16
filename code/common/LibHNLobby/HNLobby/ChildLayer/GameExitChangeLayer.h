#ifndef __GAMEEXITCHOOSE_LAYER_H__
#define __GAMEEXITCHOOSE_LAYER_H__

#include "HNUIExport.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "UI/Base/HNLayer.h"

USING_NS_CC;

using namespace cocostudio;
using namespace ui;

class GameExitChangeLayer : public HNLayer {

public:
    GameExitChangeLayer();

    virtual ~GameExitChangeLayer();

public:
    virtual bool init() override;

    CREATE_FUNC(GameExitChangeLayer);

    void showExitChoose(Node* parent, int zorder, int tag);

    void closeExitChoose();

private:
    LayerColor* colorLayer;
    Node* exitChooseLoader;

    //退出游戏回调
    void exitCallBack(Ref* pSender, Widget::TouchEventType type);
    //切换帐号回调
    void switchCallBack(Ref* pSender, Widget::TouchEventType type);

};

#endif // __GAMESET_LAYER_H__