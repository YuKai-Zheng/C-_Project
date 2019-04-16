//
//  PubgFinalModeReward.hpp
//  1099_DEMO iOS
//
//  Created by mac on 2018/5/26.
//

#ifndef PubgFinalModeReward_hpp
#define PubgFinalModeReward_hpp

#include "HNUIExport.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

using namespace ui;
using namespace cocostudio;

class PubgFinalRewardLayer : public HNLayer {
  
private:
    ImageView*              topTen;
    ImageView*              numberOne;
    Text*                   rank1Text;
    Text*                   rank2Text;
    Text*                   killNum;
    Text*                   getMoney;
    Button*                 buttonOK;
   
    
public:
    CREATE_FUNC(PubgFinalRewardLayer);
    
    PubgFinalRewardLayer();
    ~PubgFinalRewardLayer();
    
public:
    
    virtual bool init() override;
    
    void showData(int rank,int killPeoPleNum,long long getMoneyNum,Node* parent);
    
    void setOkayCallBack(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
 
    


};

#endif /* PubgFinalModeReward_hpp */
