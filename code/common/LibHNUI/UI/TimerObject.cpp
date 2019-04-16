//
//  TimerObject.cpp
//  LibHNUI
//
//  Created by 黄百超 on 06/11/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#include "TimerObject.h"
#include "HNLobby/v66/PlatformResV66.h"

namespace HN {
    TimerObject::TimerObject() {
    }
    
    TimerObject::~TimerObject() {
    }
    
    void TimerObject::onExit() {
        HNNode::onExit();
        stop();
    }
    
    bool TimerObject::init() {
        if (!HNNode::init()) {
            return false;
        }
        resetData();
        
        auto node = CSLoader::createNode("csb/v66/common/NodeTimer.csb");
        if (node) {
            m_clock = node;
            m_clock->setVisible(false);
            addChild(m_clock);
            //
            auto text = dynamic_cast<Text*>(node->getChildByName("Text"));
            if (text) {
                m_timeText = text;
            }
        }
        
        return true;
    }
    
    void TimerObject::start(float t) {
        if(m_time < 0)
            return;
        stop();
        m_time = t;
        m_clock->setVisible(true);
        m_timeText->setString(StringUtils::format("%d", (int)m_time));
        HNAudioEngine::getInstance()->stopEffectWithID(m_musicID);
        this->schedule(schedule_selector(TimerObject::updateTime), 1.0f);
    }
    
    void TimerObject::stop() {
        m_time = 0;
        this->unschedule(schedule_selector(TimerObject::updateTime));
        HNAudioEngine::getInstance()->stopEffectWithID(m_musicID);
        m_clock->setVisible(false);
    }
    
    void TimerObject::setTimeCallBack(ClockCallBack callBack) {
        if (callBack) {
            m_callBack = callBack;
        }
    }
    
    float TimerObject::readTimer() {
        return m_time;
    }
    
    Size TimerObject::getTimerSize() {
        Size s = Size(92, 92);
        if (m_clock) {
            auto bg = dynamic_cast<ImageView*>(m_clock->getChildByName("Image"));
            if (bg) {
                s = bg->getContentSize();
            }
        }
        return s;
    }
    
    void TimerObject::resetData() {
        m_callBack = nullptr;
        m_time = 0;
        m_timeText = nullptr;
        m_musicID = 0;
        m_clock = nullptr;
    }
    
    void TimerObject::updateTime(float dt) {
        m_time = m_time - 1;
        m_timeText->setString(StringUtils::format("%d", (int)m_time));
        
        //播放音效
        if (m_time == 3.0f){
            m_musicID = HNAudioEngine::getInstance()->playEffect(GAME_TIME_OUT_EFFECT_V66);
        }
        
        if(m_time <= 0) {
            if (m_callBack) {
                m_callBack();
            }
            stop();
        }
    }
}
