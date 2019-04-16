//
//  NoticeLayerV66.hpp
//  LibHNLobby
//
//  Created by mac on 2018/8/7.
//  Copyright © 2018年 redbird. All rights reserved.
//

#ifndef NoticeLayerV66_hpp
#define NoticeLayerV66_hpp

#include "HNUIExport.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "UI/Base/AUIButton.h"
#include "FontSize.h"
#include "network/HttpClient.h"
using namespace ui;
using namespace cocostudio;

class NoticeLayer : public HNLayer {
  
private:
   
    EventListenerTouchOneByOne*        touchListener;
    
    Label* _labelMessage;
    Text* promptText;
    ImageView* BG;
public:
    CREATE_FUNC(NoticeLayer);
    
    NoticeLayer();
    virtual ~NoticeLayer();
    
public:
    
    virtual bool init() override;
    void showNotice(const std::string& prompt);
    void close();
    void onHttpUpdate(cocos2d::network::HttpClient *sender,
                      cocos2d::network::HttpResponse *response);
};


#endif /* NoticeLayerV66_hpp */
