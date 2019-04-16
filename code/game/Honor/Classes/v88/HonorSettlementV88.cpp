//
//  TigerSettlementV88.cpp
//  MixProject
//
//  Created by 黄百超 on 26/03/2017.
//
//

#include "HonorSettlementV88.h"
#include "HonorGameTableUIV88.h"
//#include "HonorCommonDef.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "HNCommon/HNConverCode.h"
#include "Tool/Tools.h"
#include "HNLobby/globel.h"

namespace Honor {
    
	HonorSettlementV88::HonorSettlementV88() {
    }
    
	HonorSettlementV88::~HonorSettlementV88() {
    }
    
	bool HonorSettlementV88::init() {
        if (!Layer::init()) {
            return false;
        }
        
        this->setContentSize(WIN_SIZE);
        this->setIgnoreAnchorPointForPosition(false);
        initSettleLayer();
        
        return true;
    }
    
    void HonorSettlementV88::showSettleLayerAndUpdateData(struct_s2c_game_begin msg) {
        int count=0;
        m_totalRate = 0;
        scoreCellVec.clear();
        for (int i = 0; i < LINE_COUNT; i++) {
            if (msg.nOpenLineLotteryType[i] != -1) {
                HonorScorePanelCellV88* newCell = HonorScorePanelCellV88::create();
                //newCell->setCellValue(msg.nOpenLineLotteryType[i], msg.nOpenLineCount[i], msg.nOpenLineRate[i], msg.nOpenLinePrizeType[i]);
                newCell->setPosition(-280 + (count%3)* 200, 125 - 73 * (count / 3));
                newCell->setTag(count);
                m_root->addChild(newCell);
                scoreCellVec.push_back(newCell);
                count++;
                //if (msg.nOpenLinePrizeType[i] == PT_MONEY) {
                   // m_totalRate += msg.nOpenLineRate[i];
               // }
            }
        }
        //
        if (m_action) {
            m_action->play("nineline_settle", false);
        }
        this->setVisible(true);
        this->runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create([=]() {
            hideSettleLayer();
        }), nullptr));
    }
    
    void HonorSettlementV88::hideSettleLayer() {
        this->setVisible(false);
        if (m_action) {
            m_action->stop();
        }
        for (int i = 0; i < scoreCellVec.size(); i++) {
            auto cell = scoreCellVec.at(i);
            if (m_root && cell) {
                // m_root->removeChildByTag(i);
                m_root->removeChild(cell);
            }
        }
        scoreCellVec.clear();
        m_totalRate = 0;
    }
    
    void HonorSettlementV88::initSettleLayer() {
        m_audioEffectID = 0;
        m_root = nullptr;
        m_action = nullptr;
        scoreCellVec.clear();
        m_totalRate = 0;
        //
        std::string path = "v88/animation/nineline/nineline_settle/nineline_settle.csb";
        m_root = CSLoader::createNode(path);
        if (m_root) {
            m_root->setPosition(WIN_SIZE/2);
            addChild(m_root);
            auto action = CSLoader::createTimeline(path);
            if (action) {
                m_action = action;
                m_root->runAction(action);
            }
        }
    }
    
}
