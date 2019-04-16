//
//  TigerScorePanelCellV88.h
//  MixProject
//
//  Created by 黄百超 on 11/03/2017.
//
//

#ifndef TigerScorePanelCellV88_h
#define TigerScorePanelCellV88_h

#include "UI/Base/HNLayer.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;

namespace Tiger {
    
	class TigerScorePanelCellV88 : public Layer {
        //
        //UI
        //
        struct Cell {
            Node* spriteNode;   //整个节点
            Sprite* icon;    //得分的图片类型
			Text*     rateText;

            Cell() {
                spriteNode = nullptr;
                icon = nullptr;
				rateText = nullptr;
            }
        } _cell;
        
        //
        //Data
        //
        int _bgResIndex;
        int _iconResIndex;
        int _lightResIndex;
        bool _needScale;    //true icon scale 0.9
        float _scaleRate;
        //
        //Method
        //
    public:
		TigerScorePanelCellV88();
		~TigerScorePanelCellV88();
        
		CREATE_FUNC(TigerScorePanelCellV88);
        
        virtual bool init() override;
        

        
    private:
        void initDefaultUI();
	public:
		void setCellValue(int type, int rate, int totalRate, int textType);
      
    };
    
}

#endif /* BBLoopSpriteCellV88_h */
