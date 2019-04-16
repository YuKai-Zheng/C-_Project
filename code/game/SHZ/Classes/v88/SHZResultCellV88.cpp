//
//  BBSettlementV88.cpp
//  MixProject
//
//  Created by 黄百超 on 26/03/2017.
//
//

#include "SHZResultCellV88.h"
#include "ScorePanelCellV88.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "HNCommon/HNConverCode.h"
#include "Tool/Tools.h"

namespace SHZ {
    
    enum {
		ZOrder_Color_Layer = 10,
		ZOrder_Content_Layer = 11,

    };
    
    static const char* Cell_CSB = "Games/31000503/ResultCellPanel.csb";
	//特效
	const static char* ANIMATION_REWARD_AXE_CSB = "v88/animation/watermargin/water_result/axe.csb";
	const static char* ANIMATION_REWARD_FLAG_CSB = "v88/animation/watermargin/water_result/flag.csb";
	const static char* ANIMATION_REWARD_BROADSWORD_CSB = "v88/animation/watermargin/water_result/broadsword.csb";
	const static char* ANIMATION_REWARD_HALBERD_CSB = "v88/animation/watermargin/water_result/halberd.csb";
	const static char* ANIMATION_REWARD_LINCHONG_CSB = "v88/animation/watermargin/water_result/linchong.csb";
	const static char* ANIMATION_REWARD_LUZHISHEN_CSB = "v88/animation/watermargin/water_result/luzhishen.csb";
	const static char* ANIMATION_REWARD_SONGJIANG_CSB = "v88/animation/watermargin/water_result/songjiang.csb";
	const static char* ANIMATION_REWARD_ZHONGYI_CSB = "v88/animation/watermargin/water_result/zhongyi.csb";
	const static char* ANIMATION_REWARD_WATERMARGIN_CSB = "v88/animation/watermargin/water_result/watermargin.csb";

	SHZResultCellV88::SHZResultCellV88() {
        
    }
    
	SHZResultCellV88::~SHZResultCellV88() {
        
    }
    
	bool SHZResultCellV88::init() {
        if (!Layer::init()) {
            return false;
        }
        initSettleLayer();
        return true;
    }
    
	void SHZResultCellV88::initSettleLayer() {

		auto root = CSLoader::createNode(Cell_CSB);
		if (root) {
			_root.node = root;
			addChild(root, ZOrder_Content_Layer);
			auto img = dynamic_cast<Sprite*>(root->getChildByName("imgKind"));
			if (img)
			{
				_root.imgType = img;
			}

		}
	}

    //通用播放开奖动物的动画
	void SHZResultCellV88::playAnimalsAnimation(std::string res, std::string tagName) {
		resultAnimationNode = CSLoader::createNode(res);
		if (resultAnimationNode) {
			resultAnimationNode->setPosition(WIN_SIZE / 2);
			addChild(resultAnimationNode, 10);

			auto action = CSLoader::createTimeline(res);
			resultAnimationNode->runAction(action);
			action->play(tagName, false);
		}
	}

	void SHZResultCellV88::updateContentRes() {


	}   
}
