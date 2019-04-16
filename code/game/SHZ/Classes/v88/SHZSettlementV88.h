//
//  BBSettlementV88.h
//  MixProject
//
//  Created by 黄百超 on 26/03/2017.
//
//

#ifndef SHZSettlementV88_h
#define SHZSettlementV88_h

#include "UI/Base/HNLayer.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "SHZGameTableLogic.h"

USING_NS_CC;

namespace SHZ {

	class ScorePanelCellV88;
    
    class SHZSettlementV88 : public Layer {
        
        //UI Struct
        struct SettlementLayer {

			Node* settlementNode;
			Sprite* bg;
			Text* totalAddMoney;
			Button * bibeiBtn;
			Button *closeBtn;
           
            SettlementLayer() {
				settlementNode = nullptr;
				bg = nullptr;
				totalAddMoney = nullptr;
				bibeiBtn = nullptr;
				closeBtn = nullptr;
               
            }
        } _root;
        
      	public:
			GameTableLogic* _logic;

			std::vector<ScorePanelCellV88*> socreCellVec;   //5个下注金额

			unsigned int    m_audioEffectID;
        
            int             m_totalRate;
        
    public:
		SHZSettlementV88();
		~SHZSettlementV88();
        
		CREATE_FUNC(SHZSettlementV88);
        
        virtual bool init() override;
        void hideSettlementLayer();

	 
    public:
        void setAddTotalMoney(long long addMoney);
		void setSettlementData(int logoType[9], int rate[9],int rateNum[9]);
        void initSettleLayer();
		void setLogic(GameTableLogic* logic);

		virtual bool onTouchBegan(Touch *touch, Event *unused_event) override;
		virtual void onTouchEnded(Touch *touch, Event *unused_event) override;
	
	public:
		void guessSizeCallBack(Ref* pSender, Widget::TouchEventType type);
		void setSettlementLayerPosition(Vec2 pos);
		void moveSelfWithAction();
        
    };
    
}

#endif /* BBSettlementV88_h */
