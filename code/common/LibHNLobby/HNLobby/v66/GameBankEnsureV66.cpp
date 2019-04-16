//
//  GameBankEnsureV66.cpp
//  LibHNLobby
//
//  Created by mac on 2018/8/22.
//  Copyright © 2018年 redbird. All rights reserved.
//

#include "GameBankEnsureV66.hpp"
#include "HNLobby/GamePrompt.h"
#include "../GameData.h"
#include "../pattern/CommandManager.h"
GameBankEnsureV66::GameBankEnsureV66()
{
    _bCallBack = NULL;
    memset(szBankPassWord,0,sizeof(szBankPassWord));
    savePassword = "";
    mima = nullptr;
}
GameBankEnsureV66::~GameBankEnsureV66()
{
    //PlatformLogic()->removeEventSelector(MDM_GP_BANK, ASS_GP_BANK_OPEN);
    
}

bool GameBankEnsureV66::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto myListener = EventListenerTouchOneByOne::create();
    myListener->setSwallowTouches(true);//阻止触摸向下传递
    myListener->onTouchBegan = [](Touch *touch, Event *unused_event){
        return true;
    };
    myListener->onTouchEnded = [&](Touch *touch, Event *unused_event){
        //close();
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(myListener, this);
    
    auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 180));
    addChild(colorLayer);
    
    auto root = CSLoader::createNode("csb/newplatform/bank/node_bank_login.csb");
    if (root) {
        root->setPosition(WIN_SIZE/2);
        addChild(root);
        
		auto closeBtn = dynamic_cast<Button*>(root->getChildByName("bg")->getChildByName("btn_close"));
		if (closeBtn) {
			closeBtn->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
				if (type != Widget::TouchEventType::ENDED) {
					return;
				}

				close();

			});
		}
            
            auto password = dynamic_cast<TextField*>(root->getChildByName("bg")->getChildByName("txt_money")->getChildByName("TextField_1"));
           // password->setPositionY(password->getPositionY() - 10);
            password->setVisible(false);
            mima = HNEditBox::createEditBox(password, this);
            mima->setInput(HNEditBox::InputMode::SINGLE_LINE);
            mima->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
            
            auto ensureBtn = dynamic_cast<Button*>(root->getChildByName("bg")->getChildByName("btn_ok"));
            if (ensureBtn)
            {
                ensureBtn->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type){
                    if (type != Widget::TouchEventType::ENDED) {
						if (type == Widget::TouchEventType::BEGAN)
						{
							ensureBtn->setScale(0.8f);
						}
						else if (type == Widget::TouchEventType::CANCELED)
						{
							ensureBtn->setScale(1.0f);
						}

                        return;
                    }
                   
					ensureBtn->setScale(1.0f);
                    savePassword = mima->getString();
                    if (savePassword.empty()) {
						GamePromptLayer::create()->showPrompt(GetText("PASSWORLD_NOT_EMPTY"));
                        return;
                    }
                    char szMD5Pass[50];
                    strcpy(szMD5Pass, MD5_CTX::MD5String(savePassword).c_str());
                    
                    TMSG_GP_BankOpenReq pdata;
                    memcpy(pdata.szMD5Pass,szMD5Pass,sizeof(szMD5Pass));
                    
                    CommandManager::getInstance()->addLoaderCommand("验证密码中...");
                    PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_OPEN,&pdata,sizeof(pdata), HN_SOCKET_CALLBACK(GameBankEnsureV66::bankerEventSelector, this));
                   
                    
                });
            }
        }
        
     

    
    //PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_OPEN, nullptr, 0, HN_SOCKET_CALLBACK(GameBankEnsureV66::bankerEventSelector, this));
    /*savePassword = GameData::getInstance()->getUserBankPassWord();
    if (savePassword.empty()) {
         PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_OPEN, nullptr, 0, HN_SOCKET_CALLBACK(GameBankEnsureV66::bankerEventSelector, this));
    }
    else
    {
        if (mima) {
            mima->setString(savePassword);
           memcpy(szBankPassWord,MD5_CTX::MD5String(savePassword).c_str(),sizeof(szBankPassWord));
        }
    }*/
    
   
    return true;
}
bool GameBankEnsureV66::bankerEventSelector(HNSocketMessage* socketMessage) {
    UINT bAssistantID = socketMessage->messageHead.bAssistantID;
    UINT bHandleCode = socketMessage->messageHead.bHandleCode;
    CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
    // 打开银行
    if (ASS_GP_BANK_OPEN == bAssistantID) {
        switch (bHandleCode) {
            case HC_GP_BANK_OPEN_SUC: { //                    1        //打开钱柜成功
            
                
                CCAssert(sizeof(TMSG_GP_BankInfo) == socketMessage->objectSize, "TMSG_GP_BankInfo size is error.");
                if (sizeof(TMSG_GP_BankInfo) != socketMessage->objectSize) return true;
                
                TMSG_GP_BankInfo * bankInfo = reinterpret_cast<TMSG_GP_BankInfo*>(socketMessage->object);
                //memcpy(szBankPassWord,bankInfo->szTwoPassword,sizeof(szBankPassWord)); //为了获取密码
                
                GameData::getInstance()->saveUserBankPassWord(savePassword);
                GameData::getInstance()->saveUserBankPassWordMD5(StringUtils::format("%s",bankInfo->szTwoPassword));
                if (_bCallBack) {
                    _bCallBack();
                }
                close();
                
                
            }
                break;
            case HC_GP_BANK_OPEN_ERR: { //                    2        //打开钱柜错误
                GamePromptLayer::create()->showPrompt(GBKToUtf8("获取银行信息错误"));
            }
                break;
            case HC_GP_BANK_OPEN_ERR_PSW: { //                3        //打开钱框错误
                GamePromptLayer::create()->showPrompt(GBKToUtf8("密码错误"));
            }
                break;
            default:
                break;
        }
    }
    return true;
}
void GameBankEnsureV66::open(Node* parent, Vec2 vec, int zorder, int tag)
{
    CCAssert(nullptr != parent, "parent is nullptr");
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setPosition(vec);
    parent->addChild(this, zorder, tag);
    
}
void GameBankEnsureV66::close()
{
    this->removeFromParent();
}

void GameBankEnsureV66::editBoxReturn(ui::EditBox * editBox) {
    
}
void GameBankEnsureV66::editBoxEditingDidEnd(ui::EditBox * editBox) {
   
}
