//
//  BBLoopSpriteCellV88.cpp
//  MixProject
//
//  Created by 黄百超 on 11/03/2017.
//
//

#include "FruitLoopSpriteCellV88.h"
#include "cocostudio/CocoStudio.h"
#include "FruitCommonDef.h"
#include "cocostudio/ActionTimeline/CSLoader.h"

namespace Fruit {

    const static char* Basic_Cell_CSB = "Games/31000502/Node_Result_Cell_V66.csb";
    const static char* Animation_Reward_CSB = "v66/animation/fruitmachine/lottery_selection/lottery_selection.csb";
    
    const static char* Cell_Icon_Res = "Games/31000502/shuiguoji/icon_%d.png";
    const static char* Cell_Light_Res = "Games/31000502/shuiguoji/light_%d.png";

	FruitLoopSpriteCellV88::FruitLoopSpriteCellV88() {

}

	FruitLoopSpriteCellV88::~FruitLoopSpriteCellV88() {

}

	bool FruitLoopSpriteCellV88::init() {
    if (!Layer::init()) {
        return false;
    }

    initDefaultUI();
    return true;
}

	void FruitLoopSpriteCellV88::setCellPosition(cocos2d::Vec2 pos) {
    if (_cell.spriteNode) {
        _cell.spriteNode->setPosition(pos);
    }
}

	void FruitLoopSpriteCellV88::setCellIndex(int cellIndex) {
    _cell.cellIndex = cellIndex;
    _cell.iconIndex = g_Logos[cellIndex];
    updateContentRes();
}

	void FruitLoopSpriteCellV88::setCellScale(float scale) {
    if (_cell.spriteNode) {
        _cell.spriteNode->setScale(scale);
    }
}

//    void BBLoopSpriteCellV88::setIconIndex(int iconIndex) {
//        _cell.iconIndex = iconIndex;
//        updateContentRes();
//    }

	void FruitLoopSpriteCellV88::showLoopCellNode(bool show) {
    if (_cell.spriteNode) {
        _cell.spriteNode->setVisible(show);
    }
}

	void FruitLoopSpriteCellV88::showLightEffect(bool show) {
    if (_cell.light) {
        _cell.light->setVisible(show);
    }
    if (!show) {
        if (_cell.animationNode) {
            _cell.animationNode->setVisible(false);
        }
        if (_cell.animationAction) {
            _cell.animationAction->stop();
        }
    }
}
    
	void FruitLoopSpriteCellV88::playOpenLogoAnimation() {
        if (_cell.animationNode) {
            _cell.animationNode->setVisible(true);
        }
        if (_cell.animationAction) {
            _cell.animationAction->play("lottery_selection", false);
        }
    }

	Size FruitLoopSpriteCellV88::getContentImageSize() {
    if (_cell.bg) {
        return _cell.bg->getContentSize();
    } else {
        return Size(76.0f, 76.0f);
    }
}

	void FruitLoopSpriteCellV88::initValues() {
    _bgResIndex = 0;
    _iconResIndex = 0;
    _lightResIndex = 0;
    _needScale = false;
    _scaleRate = 1.0f;
}

	void FruitLoopSpriteCellV88::initDefaultUI() {
    if (!validCellIndex()) {
        _cell.cellIndex = 0;
    }
    if (!validIconIndex()) {
        _cell.iconIndex = 0;
    }
    auto root = CSLoader::createNode(Basic_Cell_CSB);
    if (root) {
        _cell.spriteNode = root;
  /*      auto icon = dynamic_cast<ImageView*>(root->getChildByName("Image_Icon"));
        if (icon) {
            _cell.icon = icon;
        }*/
        auto light = dynamic_cast<ImageView*>(root->getChildByName("Image_Light"));
        if (light) {
            _cell.light = light;
        }
        addChild(root);
    }
    
    updateContentRes();
}

	void FruitLoopSpriteCellV88::updateContentRes() {
    if (!validIconIndex() || !validCellIndex()) {
        return;
    }


	
		if (_cell.animationNode == nullptr) {
			//icon层需要添加动画node
			auto aniNode = CSLoader::createNode(Animation_Reward_CSB);
			if (aniNode) {
				_cell.animationNode = aniNode;
				_cell.spriteNode->addChild(_cell.animationNode, 100);
			}
		}

		if (_cell.animationAction == nullptr) {
			auto action = CSLoader::createTimeline(Animation_Reward_CSB);
			if (action) {
				_cell.animationAction = action;
				_cell.animationNode->runAction(_cell.animationAction);
			}
		}
		_cell.animationAction->stop();
	


    if (_cell.light) {
//        sprintf(name, Cell_Light_Res, _lightResIndex);
//        _cell.light->loadTexture(name);
          _cell.light->setVisible(false);
//        memset(name, 0, sizeof(name));
    }
}

	bool FruitLoopSpriteCellV88::validCellIndex() {
    if (_cell.cellIndex >= 0 && _cell.cellIndex < Loop_Sprite_Count) {
        return true;//index有效
    }
    return false;
}

	bool FruitLoopSpriteCellV88::validIconIndex() {
    if (_cell.iconIndex >= 0 && _cell.iconIndex < BetA_Count) {
        return true;//icon index 有效
    }
    return false;
}

}
