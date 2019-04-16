//
//  RedPacketMyUserInfoV77.h
//  MixProject_VIP
//
//  Created by 黄百超 on 04/08/2017.
//
//

#ifndef RedPacketMyUserInfoV77_h
#define RedPacketMyUserInfoV77_h

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include "RedPacketCommonDef.h"

USING_NS_CC;

namespace RedPacket {
    class RedPacketMyUserInfoV77 : public Layer {
    public:
        RedPacketMyUserInfoV77();
        ~RedPacketMyUserInfoV77();
        CREATE_FUNC(RedPacketMyUserInfoV77);
        virtual bool init() override;
        
        void openAddToParent(Button* parent, Vec2 pos);
        void closeLayer();
        void updateMyNameAndMoney(std::string name, LLONG money);
        
    private:
        void resetData();
        void createUI();
        
    private:
        ImageView*      m_bg;
        Text*           m_userName;
        Text*           m_userMoney;
        
    };
}

#endif /* RedPacketMyUserInfoV77_h */
