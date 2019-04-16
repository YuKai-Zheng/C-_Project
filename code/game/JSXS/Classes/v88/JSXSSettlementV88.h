//
//  JSXSSettlementV88.h
//  MixProject
//
//  Created by 黄百超 on 26/03/2017.
//
//

#ifndef JSXSSettlementV88_h
#define JSXSSettlementV88_h

#include "UI/Base/HNLayer.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "JSXSCommonDef.h"
#include "JSXSScorePanelCellV88.h"

USING_NS_CC;

namespace JSXS {
    class JSXSSettlementV88 : public Layer {
    private:
        unsigned int                            m_audioEffectID;
        Node*                                   m_root;
        cocostudio::timeline::ActionTimeline*   m_action;
        std::vector<JSXSScorePanelCellV88*>       scoreCellVec;   //5个下注金额
        int                                     m_totalRate;
        
    public:
		JSXSSettlementV88();
		~JSXSSettlementV88();
        
		CREATE_FUNC(JSXSSettlementV88);
        virtual bool init() override;

        void showSettleLayerAndUpdateData(struct_s2c_game_begin msg);
        void hideSettleLayer();
        
    private:
        void initSettleLayer();
        
    };
}

#endif /* JSXSSettlementV88_h */
