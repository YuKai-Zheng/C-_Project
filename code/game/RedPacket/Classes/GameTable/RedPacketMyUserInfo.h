//
//  RedPacketMyUserInfo.h
//  MixProject_VIP
//
//  Created by 黄百超 on 04/08/2017.
//
//

#ifndef RedPacketMyUserInfo_h
#define RedPacketMyUserInfo_h

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include "RedPacketCommonDef.h"

USING_NS_CC;

namespace RedPacket {
    class RedPacketMyUserInfo : public Layer {
    public:
        RedPacketMyUserInfo();
        ~RedPacketMyUserInfo();
        CREATE_FUNC(RedPacketMyUserInfo);
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

#endif /* RedPacketMyUserInfo_h */
