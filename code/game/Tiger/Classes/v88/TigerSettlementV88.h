//
//  TigerSettlementV88.h
//  MixProject
//
//  Created by 黄百超 on 26/03/2017.
//
//

#ifndef TigerSettlementV88_h
#define TigerSettlementV88_h

#include "UI/Base/HNLayer.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "TigerCommonDef.h"
#include "TigerScorePanelCellV88.h"

USING_NS_CC;

namespace Tiger {
    class TigerSettlementV88 : public Layer {
    private:
        unsigned int                            m_audioEffectID;
        Node*                                   m_root;
        cocostudio::timeline::ActionTimeline*   m_action;
        std::vector<TigerScorePanelCellV88*>       scoreCellVec;   //5个下注金额
        int                                     m_totalRate;
        
    public:
		TigerSettlementV88();
		~TigerSettlementV88();
        
		CREATE_FUNC(TigerSettlementV88);
        virtual bool init() override;

        void showSettleLayerAndUpdateData(struct_s2c_game_begin msg);
        void hideSettleLayer();
        
    private:
        void initSettleLayer();
        
    };
}

#endif /* TigerSettlementV88_h */
