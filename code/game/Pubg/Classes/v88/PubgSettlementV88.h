//
//  TigerSettlementV88.h
//  MixProject
//
//  Created by 黄百超 on 26/03/2017.
//
//

#ifndef PubgSettlementV88_h
#define PubgSettlementV88_h

#include "UI/Base/HNLayer.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "Pubg/Classes/GameMessage/PubgCommonDef.h"
#include "PubgScorePanelCellV88.h"

USING_NS_CC;

namespace Pubg {
    class PubgSettlementV88 : public Layer {
    private:
        unsigned int                            m_audioEffectID;
        Node*                                   m_root;
        cocostudio::timeline::ActionTimeline*   m_action;
        std::vector<PubgScorePanelCellV88*>       scoreCellVec;   //5个下注金额
        int                                     m_totalRate;
        
    public:
		PubgSettlementV88();
		~PubgSettlementV88();
        
		CREATE_FUNC(PubgSettlementV88);
        virtual bool init() override;

        void showSettleLayerAndUpdateData(struct_s2c_game_begin msg);
        void hideSettleLayer();
        
    private:
        void initSettleLayer();
        
    };
}

#endif /* TigerSettlementV88_h */
