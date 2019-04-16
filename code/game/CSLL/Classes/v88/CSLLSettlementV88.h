//
//  CSLLSettlementV88.h
//  MixProject
//
//  Created by 黄百超 on 26/03/2017.
//
//

#ifndef CSLLSettlementV88_h
#define CSLLSettlementV88_h

#include "UI/Base/HNLayer.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "CSLLCommonDef.h"
#include "CSLLScorePanelCellV88.h"

USING_NS_CC;

namespace CSLL {
    class CSLLSettlementV88 : public Layer {
    private:
        unsigned int                            m_audioEffectID;
        Node*                                   m_root;
        cocostudio::timeline::ActionTimeline*   m_action;
        std::vector<CSLLScorePanelCellV88*>       scoreCellVec;   //5个下注金额
        int                                     m_totalRate;
        
    public:
		CSLLSettlementV88();
		~CSLLSettlementV88();
        
		CREATE_FUNC(CSLLSettlementV88);
        virtual bool init() override;

        void showSettleLayerAndUpdateData(struct_s2c_game_begin msg);
        void hideSettleLayer();
        
    private:
        void initSettleLayer();
        
    };
}

#endif /* CSLLSettlementV88_h */
