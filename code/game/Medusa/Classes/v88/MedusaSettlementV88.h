//
//  SettlementV88.h
//  MixProject
//
//  Created by 黄百超 on 26/03/2017.
//
//

#ifndef MedusaSettlementV88_h
#define MedusaSettlementV88_h

#include "UI/Base/HNLayer.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "MedusaCommonDef.h"
#include "MedusaScorePanelCellV88.h"

USING_NS_CC;

namespace Medusa {
    class SettlementV88 : public Layer {
    private:
        unsigned int                            m_audioEffectID;
        Node*                                   m_root;
        cocostudio::timeline::ActionTimeline*   m_action;
        std::vector<ScorePanelCellV88*>         scoreCellVec;   //5个下注金额
        int                                     m_totalRate;
        
    public:
		SettlementV88();
		~SettlementV88();
        
		CREATE_FUNC(SettlementV88);
        virtual bool init() override;

        void showSettleLayerAndUpdateData(struct_s2c_game_begin msg);
        void hideSettleLayer();
        
    private:
        void initSettleLayer();
        
    };
}

#endif /* SettlementV88_h */
