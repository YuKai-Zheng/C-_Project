//
//  NLSettlementV88.h
//  MixProject
//
//  Created by 黄百超 on 26/03/2017.
//
//

#ifndef NLSettlementV88_h
#define NLSettlementV88_h

#include "UI/Base/HNLayer.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "NLCommonDef.h"
#include "NLScorePanelCellV88.h"

USING_NS_CC;

namespace NL {
    class NLSettlementV88 : public Layer {
    private:
        unsigned int                            m_audioEffectID;
        Node*                                   m_root;
        cocostudio::timeline::ActionTimeline*   m_action;
        std::vector<NLScorePanelCellV88*>       scoreCellVec;   //5个下注金额
        int                                     m_totalRate;
        
    public:
		NLSettlementV88();
		~NLSettlementV88();
        
		CREATE_FUNC(NLSettlementV88);
        virtual bool init() override;

        void showSettleLayerAndUpdateData(struct_s2c_game_begin msg);
        void hideSettleLayer();
        
    private:
        void initSettleLayer();
        
    };
}

#endif /* NLSettlementV88_h */
