//
//  RobotInfo.cpp
//  LibHNUI
//
//  Created by mac on 2018/8/10.
//  Copyright © 2018年 redbird. All rights reserved.
//

#include "RobotInfo.hpp"
#include "Tool/XCaseStringUtil.hpp"
#include "Tool/Tools.h"
#include "HNCommon/HNConverCode.h"
#include "HNLobby/globel.h"

struct UserHeadDoadload
{
    Node*            target;
    HttpResponse*    response;
    HttpClient*        client;
    std::string        robotID;
    UINT            logoID;
    UserHeadDoadload()
    {
        target = nullptr;
        response = nullptr;
        client = nullptr;
        robotID = "";
        logoID = 0;
    }
};

namespace HN {
 RobotInfo::RobotInfo()
 {
     _UserId = 0;
     _myMoney= 0;
     _headMyId = 0;
     _myName = NULL;
     _myWXHeadUrl = NULL;
     userHead = nullptr;
    
 }
 RobotInfo::~RobotInfo()
 {
    
    
 }
    void RobotInfo::onExit()
    {
        Node::onExit();
        Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
        
    }

 bool RobotInfo::init()
 {
    if (!Node::init()) {
        return false;
    }
    
    Node* middle = CSLoader::createNode("common/Node_robot.csb");
    if (middle) {
        addChild(middle);
        
        auto name = dynamic_cast<Text*>(middle->getChildByName("Text_name"));
        if (name) {
            _myNameLabel = name;
            _myNameLabel->setPositionY(_myNameLabel->getPositionY() - 10);
            
        }
        
        auto money = dynamic_cast<Text*>(middle->getChildByName("Text_money"));
        if (money) {
            _myMoneyLabel = money;
            
        }
        auto effectNode = dynamic_cast<Node*>(middle->getChildByName("Node_effect"));
        if (effectNode) {
            _effectNode = effectNode;
            
        }
        
    }
     
    return true;
 }
 void RobotInfo::createRobotData(int id,long long money,char* name,int headId,char* szWXHeadUrl)
 {
     _UserId  = id;
     _myMoney = money;
     _myName  = name;
     _headMyId= headId;
     _myWXHeadUrl = szWXHeadUrl;
    
     createRobotUI();
     
     
     
 }
 void RobotInfo::updateRobotData(int id,long long money,char* name,int headId,char* szWXHeadUrl)
 {
     
     if (_UserId == id && money > _myMoney) {//大赢家动画
         
         auto nodeCsb = CSLoader::createNode("Games/31000507/Node_robotEffect.csb");
         auto nodeAction = CSLoader::createTimeline("Games/31000507/Node_robotEffect.csb");
         if (nodeCsb && nodeAction)
         {
             _effectNode->addChild(nodeCsb);
             nodeCsb->setPosition(Vec2::ZERO);
             nodeCsb->runAction(nodeAction);
             nodeAction->gotoFrameAndPlay(0,false);
             nodeAction->setLastFrameCallFunc([=](){
                 nodeCsb->removeFromParent();
             });
         }
         
         
     }
     
     _UserId  = id;
     _myMoney = money;
     _myName  = name;
     _headMyId= headId;
     _myWXHeadUrl = szWXHeadUrl;
     
     
     createRobotUI();
     
 }
 long RobotInfo::getMyUserId()
 {
     return _UserId;
 }
 void RobotInfo::createRobotUI()
 {
     this->setVisible(_UserId != -1);
     if (_myNameLabel) {
         std::string nickName(_myName);
         if (!nickName.empty()) {
             if (nickName.length() > 6) {
                 auto strUtil = XCaseStringUtil::create(ForceGBKToUtf8(nickName.c_str()));
                 std::string tempName = strUtil->sub(1,5,true);
                 
                 _myNameLabel->setString(tempName);
             }
             else
             {
                 _myNameLabel->setString(ForceGBKToUtf8(nickName.c_str()));
             }
             
         }
         else {
             _myNameLabel->setString(GBKToUtf8("未知"));
         }
     }
     
     
     if (_myMoneyLabel) {
         _myMoneyLabel->setString(Tools::goldDisplayByType(_myMoney));
     }
     
     if (userHead == nullptr) {
         userHead = UserHead::create("hallimg/main/touxiangk.png");
         userHead->setHeadTouchEnabled(false);
         userHead->setPosition(Vec2(0,35));
         addChild(userHead);
     }
   
    // userHead->setName("USERHEAD");
     
     //if (_headMyId == WX_HEAD_IMG_ID) { //是微信头像
        
         
         std::ostringstream stream;
         stream << _UserId;
         std::string robotID = stream.str();
         
         if (Director::getInstance()->getTextureCache()->getTextureForKey(robotID))
         {
             //存在，直接用
             userHead->setClipUserHeadWithTexTure(Director::getInstance()->getTextureCache()->getTextureForKey(robotID),"hallimg/niuniu/5_1.png",0.60f);
         }else{
             //网络下载
             ccHttpRequestCallback callBack = [=](HttpClient* pClient, HttpResponse* pResponse) {
                 
                 UserHeadDoadload data;
                 data.client = pClient;
                 data.response = pResponse;
                 data.robotID = robotID;
                 data.logoID = _headMyId;
                 
                 EventCustom e("ROBOT_HEAD_DOWNLOAD");
                 data.target = this;
                 e.setUserData((void*)(&data));
                 Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
             };
             
             
             auto listener = EventListenerCustom::create("ROBOT_HEAD_DOWNLOAD", [=](EventCustom *event) {
                 UserHeadDoadload* data = (UserHeadDoadload*)(event->getUserData());
                 if (data->target != this)
                     return;
                 if (data && data->client && data->response)
                 {
                     Image* image = new Image();
                     Tools::getImageByData(data->client, data->response, image);
                     if (image && image->getData() != nullptr) {
                         Texture2D* texture = Director::getInstance()->getTextureCache()->addImage(image, data->robotID);
                         if (texture) {
                             userHead->setClipUserHeadWithTexTure(texture,"hallimg/niuniu/5_1.png",0.60f);
                         }
                         else {
                             userHead->setClipUserHeadWithFile(StringUtils::format("hallimg/dayday_toux/%d.png", _headMyId),"hallimg/niuniu/5_1.png",0.60f);
                         }
                         image->autorelease();
                     }
                     else {
                         userHead->setClipUserHeadWithFile(StringUtils::format("hallimg/dayday_toux/%d.png",_headMyId),"hallimg/niuniu/5_1.png",0.60f);
                     }
                 }
             });
             
             Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
             Tools::requestRobotIcon(_myWXHeadUrl, callBack,false);
             
         }
             
         
        // }
    // else
    /* {
         std::string head_path = StringUtils::format("hallimg/dayday_toux/%d.png", _headMyId);
         if (!FileUtils::getInstance()->isFileExist(head_path))
         {
             head_path =  "hallimg/dayday_toux/none.png";
         }
         userHead->setClipUserHeadWithFile(head_path, "hallimg/niuniu/5_1.png",0.6f);
     }*/
     
 }
    
}
