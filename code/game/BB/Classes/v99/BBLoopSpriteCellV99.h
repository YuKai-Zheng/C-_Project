//
//  BBLoopSpriteCell.h
//  MixProject
//
//  Created by 黄百超 on 11/03/2017.
//
//

#ifndef BBLoopSpriteCellV99_h
#define BBLoopSpriteCellV99_h

#include "UI/Base/HNLayer.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;

namespace BB {
    
    class BBLoopSpriteCellV99 : public Layer {
        //
        //UI
        //
        struct Cell {
            Node* spriteNode;   //整个节点
            Node* animationNode;    //动画节点
            cocostudio::timeline::ActionTimeline* animationAction;
            ImageView* bg;      //loop底图
            ImageView* icon;    //loop实际icon
            ImageView* light;   //loop亮光效果
            int cellIndex;      //cell所在的位置，偶数位置效果都是黄底，奇数位置为蓝色
            int iconIndex;      //icon，共有八种，与下注区域的对应
            Cell() {
                spriteNode = nullptr;
                animationNode = nullptr;
                animationAction = nullptr;
                bg = nullptr;
                icon = nullptr;
                light = nullptr;
                cellIndex = -1;
                iconIndex = -1;
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
        BBLoopSpriteCellV99();
        ~BBLoopSpriteCellV99();
        
        CREATE_FUNC(BBLoopSpriteCellV99);
        
        virtual bool init() override;
        
        void setCellPosition(Vec2 pos);
        void setCellIndex(int cellIndex);
//        void setIconIndex(int iconIndex);
        void setCellScale(float scale);
        
        void showLoopCellNode(bool show);
        
        void showLightEffect(bool show);
        void playOpenLogoAnimation();
        
        Size getContentImageSize();
        
    private:
        void initValues();
        void initDefaultUI();
        void updateContentRes();
        
        bool validCellIndex();
        bool validIconIndex();
        
    };
    
}

#endif /* BBLoopSpriteCellV99_h */
