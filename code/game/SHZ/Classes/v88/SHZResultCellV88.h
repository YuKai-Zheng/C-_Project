//
//  BBSettlementV88.h
//  MixProject
//
//  Created by 黄百超 on 26/03/2017.
//
//

#ifndef SHZResultCellV88_h
#define SHZResultCellV88_h

#include "UI/Base/HNLayer.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;

namespace SHZ {
    
	class SHZResultCellV88 : public Layer {
        
        //UI Struct
        struct SettlementLayer {

			Node* node;
			Sprite* imgType;
			Node* animationNode;    //动画节点
			cocostudio::timeline::ActionTimeline* animationAction;
		
           
            SettlementLayer() {
				node = nullptr;
				imgType = nullptr;
		
			
               
            }
        } _root;
        
        struct SettleMentData {
            int openLogo;
            char selfName[100];
            long long selfWinMoney;
            char bankerName[100];
            long long bankerWinMoney;
            SettleMentData() {
                openLogo = -1;
                memset(selfName, 0, sizeof(selfName));
                selfWinMoney = 0;
                memset(bankerName, 0, sizeof(bankerName));
                bankerWinMoney = 0;
            }
        } _date;

		//动画
		Node*							resultAnimationNode;		//播放动物动画的统一节点
        
    public:
		SHZResultCellV88();
		~SHZResultCellV88();
        
		CREATE_FUNC(SHZResultCellV88);
        
        virtual bool init() override;
        void hideSettlementLayer();
    
    private:

        void initSettleLayer();
		void playAnimalsAnimation(std::string res, std::string tagName);
		//void showOpenAnimalAnimation(BYTE index,int logoIndex);
		void updateContentRes();

        
    };
    
}

#endif /* BBSettlementV88_h */
