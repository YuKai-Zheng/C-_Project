#ifndef __HN_HNLayer_H__
#define __HN_HNLayer_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <fstream>

#include "Tool/GraphicsUtils.h"
#include "Tool/WireframeOnOff.h"
#include "HNLobby/CommonMacro.h"

USING_NS_CC;
using namespace ui;
using namespace std;
using namespace HN;

namespace HN {

class HNLayer : public Layer {
    CC_SYNTHESIZE(bool, _drawRect, DrawRect);
    CC_SYNTHESIZE(bool, mAutoClose, AutoClose);

public:
    HNLayer(void);
    virtual ~HNLayer(void);

    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void onExitTransitionDidStart()override;
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    // virtual void visit(Renderer *renderer, const Mat4& parentTransform,uint32_t parentFlags);
    bool onTouchBegan(Touch *touch, Event *unused_event) override;
    void onTouchMoved(Touch *touch, Event *unused_event) override;
    void onTouchEnded(Touch *touch, Event *unused_event) override;

public:
    void setBackGroundImage(const std::string &name);
    void enableKeyboard();
    void startShade(int opacity = 100);
    void quicklyShade(int opacity = 100);
    float getRealScaleX(int designWidth);
    float getRealScaleY(int designHeight);
    bool switchToLayoutControl(Layout *layout, const std::string &controlName,Node *newControl);

public:
    Node *m_pUIRoot;

private:
    void updateOpacity(float dt);

private:
    Layer *_colorLayer;
    unsigned char _opacity;
    CustomCommand _customCommand;
    void onDrawPrimatives(const cocos2d::Mat4 &transform, uint32_t flags);
    DrawNode *drawNode;
    // virtual void visit()override;
};
};

#endif //__HN_HNLayer_H__
