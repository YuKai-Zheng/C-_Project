//
//  NSAnimationTool.h
//  MixProject
//
//  Created by 黄百超 on 11/01/2017.
//
//

#ifndef NSAnimationTool_h
#define NSAnimationTool_h

//#include <stdio.h>
//#include "HNUIExport.h"
#include "UI/Base/HNLayer.h"

using namespace cocos2d;

namespace NewYearActivity {
    
    static const char* SMALL_TNT_EFFECT_AUDIO = "music/NewYearActivity/smallTntAudio.wav";
    static const char* MID_TNT_EFFECT_AUDIO = "music/NewYearActivity/midTntAudio.wav";
    static const char* BIG_TNT_EFFECT_AUDIO = "music/NewYearActivity/bigTntAudio.wav";
    
    class NSAnimationTool {
    
    public:
        
        //创建年兽按钮
        static Button* createNSButton(std::string btnName);
        
        static Node* createNSDaijiNodeAni(std::string name, bool playLoop);
        
        static Node* createNSGanNodeAni(std::string name, bool playLoop);
        
		static Label* createRewardLabel(long long i64RewardMoney, Layer* parent, Vec2 point);

		static void createTipOfNoProp(Layer* parent, Vec2 point);

		static Node* createBoxNodeAni(std::string name, bool playLoop);

		static Node* createStarNodeAni(std::string name, bool playLoop);
        
        //播放赶年兽TNT音效
        static void playTNTAudio(long long money);
    };
    
}

#endif /* NSAnimationTool.h */
