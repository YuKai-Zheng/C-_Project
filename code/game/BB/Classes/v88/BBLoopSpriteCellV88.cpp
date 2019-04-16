//
//  BBLoopSpriteCellV88.cpp
//  MixProject
//
//  Created by 黄百超 on 11/03/2017.
//
//

#include "BBLoopSpriteCellV88.h"
#include "cocostudio/CocoStudio.h"
#include "BBCommonDef.h"
#include "cocostudio/ActionTimeline/CSLoader.h"

namespace BB {

    const static char* Basic_Cell_CSB = "csb/Games/BB_V66/Node_Basic_Result_Cell_V66.csb";
    const static char* Animation_Reward_CSB = "v88/animation/BenzBMW/reward/reward.csb";
    
    const static char* Cell_Icon_Res = "v66/BB/icon_%d.png";
    const static char* Cell_Bg_Res = "v66/BB/di_%d.png";
    const static char* Cell_Light_Res = "v66/BB/light_%d.png";
	const static char* Cell_Light_Res_YUAN = "v66/BB/8.png";

BBLoopSpriteCellV88::BBLoopSpriteCellV88() {

}

BBLoopSpriteCellV88::~BBLoopSpriteCellV88() {

}

bool BBLoopSpriteCellV88::init() {
    if (!Layer::init()) {
        return false;
    }

    initDefaultUI();
    return true;
}

void BBLoopSpriteCellV88::setCellPosition(cocos2d::Vec2 pos) {
    if (_cell.spriteNode) {
        _cell.spriteNode->setPosition(pos);
    }
}

void BBLoopSpriteCellV88::setCellIndex(int cellIndex) {
    _cell.cellIndex = cellIndex;
    _cell.iconIndex = g_Logos[cellIndex];
    updateContentRes();
}

void BBLoopSpriteCellV88::setCellScale(float scale) {
    if (_cell.spriteNode) {
        _cell.spriteNode->setScale(scale);
    }
}

//    void BBLoopSpriteCellV88::setIconIndex(int iconIndex) {
//        _cell.iconIndex = iconIndex;
//        updateContentRes();
//    }

void BBLoopSpriteCellV88::showLoopCellNode(bool show) {
    if (_cell.spriteNode) {
        _cell.spriteNode->setVisible(show);
    }
}


void BBLoopSpriteCellV88::showLightRound(bool show) {
	if (_cell.light) {
		_cell.light->loadTexture(Cell_Light_Res_YUAN);
		_cell.light->setVisible(true);
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


void BBLoopSpriteCellV88::showLightEffect(bool show) {
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
    
    void BBLoopSpriteCellV88::playOpenLogoAnimation() {
        if (_cell.animationNode) {
            _cell.animationNode->setVisible(true);
        }
        if (_cell.animationAction) {
            _cell.animationAction->play("selected", false);
        }
    }

Size BBLoopSpriteCellV88::getContentImageSize() {
    if (_cell.bg) {
        return _cell.bg->getContentSize();
    } else {
        return Size(76.0f, 76.0f);
    }
}

void BBLoopSpriteCellV88::initValues() {
    _bgResIndex = 0;
    _iconResIndex = 0;
    _lightResIndex = 0;
    _needScale = false;
    _scaleRate = 1.0f;
}

void BBLoopSpriteCellV88::initDefaultUI() {
    if (!validCellIndex()) {
        _cell.cellIndex = 0;
    }
    if (!validIconIndex()) {
        _cell.iconIndex = 0;
    }
    auto root = CSLoader::createNode(Basic_Cell_CSB);
    if (root) {
        _cell.spriteNode = root;
        auto bg = dynamic_cast<ImageView*>(root->getChildByName("Image_bg"));
        if (bg) {
            _cell.bg = bg;
        }
        auto icon = dynamic_cast<ImageView*>(root->getChildByName("Image_Icon"));
        if (icon) {
            _cell.icon = icon;
        }
        auto light = dynamic_cast<ImageView*>(root->getChildByName("Image_Light"));
        if (light) {
            _cell.light = light;
        }
        addChild(root);
    }
    
    updateContentRes();
}

void BBLoopSpriteCellV88::updateContentRes() {
    if (!validIconIndex() || !validCellIndex()) {
        return;
    }
    _bgResIndex = _cell.cellIndex % 2;//奇数还是偶数
    _lightResIndex = _bgResIndex;
    _iconResIndex = _cell.iconIndex < 4 ? _cell.iconIndex : LoopK_A_BetA_Count - _cell.iconIndex - 1;    //0123,3210
    if (_iconResIndex < 0 || _iconResIndex >= LoopK_A_BetA_Count / 2) {
        _iconResIndex = 0;
    }
    _needScale = _bgResIndex == 0 ? false : true;
    _scaleRate = _needScale ? 0.8f : 1.0f;
    //
    char name[128] = {0};
    if (_cell.bg) {
        sprintf(name, Cell_Bg_Res, _bgResIndex);
        _cell.bg->loadTexture(name);
        memset(name, 0, sizeof(name));
    }
    if (_cell.icon) {
        sprintf(name, Cell_Icon_Res, _iconResIndex);
        _cell.icon->loadTexture(name);
        _cell.icon->setScale(_scaleRate);
        if (_cell.animationNode == nullptr) {
            //icon层需要添加动画node
            auto aniNode = CSLoader::createNode(Animation_Reward_CSB);
			if (aniNode) {

                _cell.animationNode = aniNode;
                aniNode->setPosition(_cell.icon->getPosition());
                _cell.spriteNode->addChild(_cell.animationNode, _cell.icon->getLocalZOrder());
            }
        }
        _cell.animationNode->setVisible(false);
        auto node2 = dynamic_cast<Node*>(_cell.animationNode->getChildByName("Node_2"));
		if (node2) {	
			auto logo = dynamic_cast<Sprite*>(node2->getChildByName("biaozhi"));
            if (logo) {
                logo->setTexture(name);
				logo->setScale(_scaleRate);
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
        memset(name, 0, sizeof(name));
    }
    if (_cell.light) {
        sprintf(name, Cell_Light_Res, _lightResIndex);
        _cell.light->loadTexture(name);
        _cell.light->setVisible(false);
        memset(name, 0, sizeof(name));
    }
}

bool BBLoopSpriteCellV88::validCellIndex() {
    if (_cell.cellIndex >= 0 && _cell.cellIndex < Loop_Sprite_Count) {
        return true;//index有效
    }
    return false;
}

bool BBLoopSpriteCellV88::validIconIndex() {
    if (_cell.iconIndex >= 0 && _cell.iconIndex < LoopK_A_BetA_Count) {
        return true;//icon index 有效
    }
    return false;
}

}
