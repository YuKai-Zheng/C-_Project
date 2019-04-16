//
//  NLSettlementV88.h
//  MixProject
//
//  Created by 黄百超 on 26/03/2017.
//
//

#ifndef JPMSettlementV88_h
#define JPMSettlementV88_h

#include "UI/Base/HNLayer.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "JPM/Classes/GameMessage/JPMCommonDef.h"
#include "JPMScorePanelCellV88.h"

USING_NS_CC;

namespace JPM {
    class JPMSettlementV88 : public Layer {
    private:
        unsigned int                            m_audioEffectID;
        Node*                                   m_root;
        cocostudio::timeline::ActionTimeline*   m_action;
        std::vector<JPMScorePanelCellV88*>       scoreCellVec;   //5个下注金额
        int                                     m_totalRate;
        
    public:
		JPMSettlementV88();
		~JPMSettlementV88();
        
		CREATE_FUNC(JPMSettlementV88);
        virtual bool init() override;

        void showSettleLayerAndUpdateData(struct_s2c_game_begin msg);
        void hideSettleLayer();
        
    private:
        void initSettleLayer();
        
    };
}

#endif /* NLSettlementV88_h */
