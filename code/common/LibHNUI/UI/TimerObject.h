//
//  TimerObject.h
//  LibHNUI
//
//  Created by 黄百超 on 06/11/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#ifndef TimerObject_h
#define TimerObject_h

#include "cocos2d.h"
#include "UI/Base/HNNode.h"
#include "cocostudio/CocoStudio.h"
#include "HNUIExport.h"

USING_NS_CC;

/************************************************************************/
/* 闹钟                                                                  */
/************************************************************************/
namespace HN {
    typedef std::function<void()> ClockCallBack;
    
    class TimerObject : public HNNode {
    private:
        ClockCallBack   m_callBack;
        float           m_time;
        Text*           m_timeText;
        unsigned int    m_musicID;
        Node*           m_clock;    //闹钟csb
        
    public:
        CREATE_FUNC(TimerObject);
        
        TimerObject();
        virtual ~TimerObject();
        
        virtual void onExit();
        virtual bool init();
        
    public:
        void start(float t);
        void stop();
        void setTimeCallBack(ClockCallBack callBack);
        float readTimer();
        Size getTimerSize();
        
    private:
        void resetData();
        void updateTime(float dt);
        
    };
    
}

#endif /* TimerObject_h */
