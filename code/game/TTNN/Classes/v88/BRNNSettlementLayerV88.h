#ifndef __BRNN_SETTLEMENT_LAYERV88_H__
#define __BRNN_SETTLEMENT_LAYERV88_H__

#include "cocos2d.h"

#include "cocostudio/CocoStudio.h"
#include "UI/Base/HNLayer.h"
USING_NS_CC;
using namespace cocostudio;

namespace BRNN {
class BRNNSettlementLayerV88 : public HNLayer {
public:
    void showSet(Node* parent, int zorder, int tag = -1);
    void closeSet();
    virtual void onEnter() override;

    CREATE_FUNC(BRNNSettlementLayerV88);
    virtual bool init() override;
    virtual bool onTouchBegan(Touch *touch, Event *unused_event) override;
    virtual void onTouchEnded(Touch *touch, Event *unused_event) override;
    /**
       @brief	设置庄家的输赢金额.

       @param	score	金额
     */
    void setBankerMoney(const char* money);
    /**
       @brief	设置闲家的输赢金额.

       @param	score	金额.
     */
    void setMyselfMoney(const char* money);
private:
    Node* loader;
    TextBMFont* selfMoney;
    TextBMFont* bankerMoney;
    bool isWin;
};
}


#endif  //__BRNN_SETTLEMENT_LAYER_H__
