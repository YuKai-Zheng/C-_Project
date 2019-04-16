//
//  RedPacketGamePlayerCellLayer.h
//  MixProject_VIP
//
//  Created by 黄百超 on 02/08/2017.
//
//

#ifndef RedPacketGamePlayerCellLayer_h
#define RedPacketGamePlayerCellLayer_h

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include "UI/UserHead.h"
#include "RedPacketCommonDef.h"

USING_NS_CC;

namespace RedPacket {
    class RedPacketGamePlayerCellLayer : public Button {
    public:
        RedPacketGamePlayerCellLayer();
        ~RedPacketGamePlayerCellLayer();
        CREATE_FUNC(RedPacketGamePlayerCellLayer);
        virtual bool init() override;
        
        void setHead(UserInfoStruct* user);
        void setPacketUIByMy(bool isMe);
        void setParentUI(HNLayer* parent);
        
        void setOpenPacketCallBack(std::function<void(unsigned int)> callBack);
        void setpacketDetailCallBack(std::function<void(unsigned int)> callBack);
        void setPacketData(tag_s2c_RedPacketListReq* object, UserInfoStruct *user);
        
        void setMyPacketRobInfo(tag_c2s_RobRedPacketReq* packetInfo);
        
//        unsigned int getPacketID();
        
        void setHasRobPacket(bool hasRob);
        bool getHasRobPacket();
        
        //更新自己昵称与金币
        void setRequestMyInfoCallBack(std::function<void(unsigned int)> callBack);
        void updateMyWalletMoney(std::string name, LLONG money);
        
    private:
        void resetData();
        void createUI();
        
        //按钮回调
    private:
        void openPacketButtonCallBack(Ref* pSender, Widget::TouchEventType type);
        void userHeadButtonCallBack(Ref* pSender, Widget::TouchEventType type);
        
        void updateLeftTime(float dt);
        
    private:
        ImageView*                      m_timeBg;
        Button*                         m_packetButton;
        UserHead*                       m_userHead;
        Button*                         m_userHeadButton;
        Text*                           m_timeText;
        Text*                           m_packetLeftTimeText;
        HNLayer*                        m_parent;
        bool                            m_isMe;
//        unsigned int                    m_packetID;
        unsigned int                    m_packetLeftTime;   //红包剩余有效时间
        
        std::function<void(unsigned int)>   m_openPacketCallBack;
        std::function<void(unsigned int)>   m_packetDetailCallBack;
        
        //红包数据
        tag_s2c_RedPacketListReq        m_redPacket;
        //发红包人信息
        UserInfoStruct                  m_sendPacketUser;
        //红包是否领取过（点击即算作领取过，包括领取失败）
        bool                            m_hasRobPacket;
        //自己拆红包的相关结果信息
        tag_c2s_RobRedPacketReq         m_myPacketInfo;
        
        //更新自己昵称、金币
        std::function<void(unsigned int)>           m_requestMyInfo;
        
    };
}

#endif /* RedPacketGamePlayerCellLayer_h */
