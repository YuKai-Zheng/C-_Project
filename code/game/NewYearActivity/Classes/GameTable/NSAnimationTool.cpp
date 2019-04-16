//
//  NSAnimationTool.cpp
//  MixProject
//
//  Created by 黄百超 on 11/01/2017.
//
//

#include "NSAnimationTool.h"
#include "cocostudio/CocoStudio.h"
#include "Sound/HNAudioEngine.h"

namespace NewYearActivity {
        
    Button* NSAnimationTool::createNSButton(std::string btnName)
    {
        const char* BtnRES = "common/animation/Newyear/Monster/daiji/1_00.png";
        
        Sprite* spr = Sprite::create(BtnRES);
        Button* btn = Button::create();
        if (spr) {
            btn->setPressedActionEnabled(false);
            btn->ignoreContentAdaptWithSize(false);
            btn->setAnchorPoint(Point(0.5, 0.5));
            btn->setContentSize(spr->getContentSize());
            btn->setName(btnName);
        }
        return btn;
    }
    
    Node* NSAnimationTool::createNSDaijiNodeAni(std::string name, bool playLoop)
    {
        const char* IconCSB = "common/animation/Newyear/Monster/Monster_daiji.csb";

        auto action = CSLoader::createTimeline(IconCSB);
        if (action) {
            action->play("daiji", playLoop);
        }

        auto node = CSLoader::createNode(IconCSB);
        if (node) {
            node->setName(name);
            node->runAction(action);
        }
        return node;
    }

    Node* NSAnimationTool::createNSGanNodeAni(std::string name, bool playLoop)
    {
        const char* IconCSB = "common/animation/Newyear/Monster/Monster_behit.csb";

        auto action = CSLoader::createTimeline(IconCSB);
        if (action) {
            action->play("behit", playLoop);
        }

        auto node = CSLoader::createNode(IconCSB);
        if (node) {
            node->setName(name);
            node->runAction(action);
        }
        return node;
    }

	Label* NSAnimationTool::createRewardLabel(long long i64RewardMoney, Layer* parent, Vec2 point)
	{
		Label* label = Label::create();
		label->setSystemFontSize(30);
		label->setColor(Color3B(255,252,0));
		label->setPosition(point);
		parent->addChild(label);

		char str[64] = { 0 };
		sprintf(str, "+%lld", i64RewardMoney);
		label->setString(str);

		label->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(0, 50)), DelayTime::create(0.8f), RemoveSelf::create(), nullptr));

		return label;
	}

	void NSAnimationTool::createTipOfNoProp(Layer* parent, Vec2 point)
	{
		ImageView* tip = ImageView::create("common/noPropTip.png");
		tip->setPosition(point);
		parent->addChild(tip);

		tip->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(0, 50)), DelayTime::create(0.8f), RemoveSelf::create(), nullptr));
	}

	Node* NSAnimationTool::createBoxNodeAni(std::string name, bool playLoop)
	{
		const char* IconCSB = "common/animation/Newyear/baoxiang/baoxiang.csb";

		auto action = CSLoader::createTimeline(IconCSB);
		if (action) {
			action->play("baoxiang", playLoop);
		}

		auto node = CSLoader::createNode(IconCSB);
		if (node) {
			node->setName(name);
			node->runAction(action);
		}
		return node;
	}

	Node* NSAnimationTool::createStarNodeAni(std::string name, bool playLoop)
	{
		const char* IconCSB = "common/animation/Newyear/Sign/Sign.csb";

		auto action = CSLoader::createTimeline(IconCSB);
		if (action) {
			action->play("sign", playLoop);
		}

		auto node = CSLoader::createNode(IconCSB);
		if (node) {
			node->setName(name);
			node->runAction(action);
			node->runAction(Sequence::create(DelayTime::create(1.0f), RemoveSelf::create(), nullptr));
		}
		return node;
	}

    void NSAnimationTool::playTNTAudio(long long money)
    {
        char res[100] = {0};
        if (money > 0 && money < 100000) {
            sprintf(res, "%s", SMALL_TNT_EFFECT_AUDIO);
        }
        else if (money >= 100000 && money < 200000) {
            sprintf(res, "%s", MID_TNT_EFFECT_AUDIO);
        }
        else if (money >= 200000 && money < 1000000) {
            sprintf(res, "%s", BIG_TNT_EFFECT_AUDIO);
        }
        else if (money >= 1000000) {
            sprintf(res, "%s", MID_TNT_EFFECT_AUDIO);
        }
        HNAudioEngine::getInstance()->playEffect(res);
    }
    
    
}
