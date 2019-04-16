//
//  JackpotModel.cpp
//  LibHNUI
//
//  Created by mac on 2018/8/27.
//  Copyright © 2018年 redbird. All rights reserved.
//

#include "JackpotModel.hpp"
#include "Tool/Tools.h"
#include "HNLobby/GameData.h"
namespace HN {
   
    JackpotModel::JackpotModel()
    {
        m_currentJackpot = 0;
        m_nextJackpot    = 0;
        m_maxJackpot     = 0;
        m_minJackpot     = 0;
        m_jackpotText    = nullptr;
        
        isAdd = false;
        isSub = false;
        
        m_myGameType = -1;
        m_myGameId   = -1;
        m_roomId     = -1;
        
        minAdd       = 0;
        maxAdd       = 0;
    }
    JackpotModel::~JackpotModel()
    {
        
    }
    void JackpotModel::onExit()
    {
        Node::onExit();
        unschedule(schedule_selector(JackpotModel::updateMoney));
    }
    bool JackpotModel::init()
    {
        if (!Node::init()) {
            return false;
        }
        
       
       
        
        char moneyValue[32] = {0};
        m_jackpotText = TextAtlas::create("", "v66/fontpng/jiesuan2.png", 72, 96, "+");
        sprintf(moneyValue, "%s", Tools::addComma(0).c_str());
        
        if (m_jackpotText) {
            m_jackpotText->setPosition(Vec2::ZERO);
            addChild(m_jackpotText);
            m_jackpotText->setString(moneyValue);
        }
        
        return true;
        
    }
    void JackpotModel::setJackPotData()
    {
        
       
        m_currentJackpot = getGameJackpot();
        char moneyValue[32] = {0};
        sprintf(moneyValue, "%s", Tools::addComma(m_currentJackpot).c_str());
        m_jackpotText->setString(moneyValue);
        
        setNextUpdate();
       
        
    }
    void JackpotModel::setNextUpdate()
    {
        /*if (m_nextJackpot == 0) {
            m_nextJackpot = m_currentJackpot + random(10000,100000);
            isAdd = true;
            isSub = false;
        }
        else
        {
            m_nextJackpot = getGameJackpot();
            if (m_currentJackpot == m_nextJackpot) {
                m_nextJackpot = m_currentJackpot + 10000;
            }
            isAdd = m_nextJackpot > m_currentJackpot;
            isSub = m_nextJackpot < m_currentJackpot;
            
        }*/
        m_nextJackpot = m_currentJackpot + random(100000,1000000);
        runAction(Sequence::create(DelayTime::create(random(0,2)),CallFunc::create([this](){
            unschedule(schedule_selector(JackpotModel::updateMoney));
            schedule(schedule_selector(JackpotModel::updateMoney), 0.33f);
        }),NULL));
    }
    void JackpotModel::updateMoney(float dt)
    {
        if (m_nextJackpot > m_currentJackpot) {
            m_currentJackpot += random(50,100);
        }
        /*else
        {
            m_currentJackpot -= 987 ;
        }*/
        
        if (m_currentJackpot > m_nextJackpot) {
           // if (isAdd && !isSub) {
                m_currentJackpot = m_nextJackpot;
                unschedule(schedule_selector(JackpotModel::updateMoney));
                setJackPotData();
           // }
        }
        
       /* if (m_currentJackpot < m_nextJackpot) {
            if (!isAdd && isSub) {
                m_currentJackpot = m_nextJackpot;
                unschedule(schedule_selector(JackpotModel::updateMoney));
                setJackPotData();
            }
        }*/
        
        char moneyValue[32] = {0};
        sprintf(moneyValue, "%s", Tools::addComma(m_currentJackpot).c_str());
        m_jackpotText->setString(moneyValue);
        
        
        
    }
    void JackpotModel::setTextScale(float scale)
    {
        if (m_jackpotText) {
            m_jackpotText->setScale(scale);
        }
    }
    void JackpotModel::setGameData(int _type,int _gameId,int _roomId)
    {
        m_myGameType = _type;
        m_myGameId   = _gameId;
        m_roomId     = _roomId;
        
        /*if (m_myGameId != -1 && m_roomId != -1) {
            if (m_myGameId == 31000504) { //水果拉霸
                
            }
        }*/
        setJackPotData();
    }
    long long JackpotModel::getGameJackpot()
    {
        long long tempJackpot = 0;
        if (m_myGameId != -1 && m_roomId == -1) {
            tempJackpot = GameData::getInstance()->getGameJackpotByGameId(m_myGameId);
        }
        else if(m_myGameId != -1 && m_roomId != -1 )
        {
            tempJackpot = GameData::getInstance()->getGameJackpotByGameRoom(m_myGameId,m_roomId);
        }
        else if (m_myGameType == 1 && m_myGameId == -1 && m_roomId == -1 )
        {
            tempJackpot = GameData::getInstance()->getGameJackpotByGameId(31000506);//连线只有水果拉霸
           
           
            m_currentJackpot = tempJackpot;
            
        }
        
        return tempJackpot;
    }
    long long JackpotModel::getCurJackpot()
    {
        return m_currentJackpot;
    }
}
