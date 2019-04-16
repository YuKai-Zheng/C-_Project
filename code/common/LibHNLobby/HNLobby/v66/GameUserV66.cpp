#include "GameUserV66.h"
#include "../pattern/factory/background/BackgroundFactory.h"
#include "../pattern/factory/gameUserHead/GameUserHeadFactory.h"
#include "../CommonMacro.h"
#include "PlatformResV66.h"
#include "cocostudio/CocoStudio.h"
#include "Sound/HNAudioEngine.h"
#include "../PlatformDefine.h"
#include "UI/UserHead.h"
#include "HNPlatform/HNPlatformLogic.h"
#include "../PlatformMessage.h"
#include "../GamePrompt.h"
#include "GameUserHeadV66.h"
#include "../pattern/CommandManager.h"
#include "CommonRes.h"
#include "base/ClipboardHelper.h"
#include "VersionInfo/VersionInfo.h"
#include "HNLobby/v66/InvitedCodeLayerV66.h"
#include "BindPhone.hpp"

const char* USER_DATA_MODIFY_TITLE_V66 = "v66/button/green/4.png";
const char* USER_DATA_COPY_TITLE_V66 = "v66/button/yellow2/8.png";

const char* SWITCH_BG_PERSONAL = "v66/module/gerenxinxi/btnBg.png";
const char* SWITCH_BAR_PERSONAL = "v66/module/gerenxinxi/2.png";
const char* PERSONAL_SWITCH = "v66/module/gerenxinxi/1_1.png";
const char* ACCOUNT_SAFE_SWITCH = "v66/module/gerenxinxi/1.png";

const char* SET_BUTTON_UNBIND_TITLE_V66 = "v66/button/yellow1/11.png";
const char* SET_BUTTON_BIND_TITLE_V66 = "v66/button/yellow1/11.png";

bool GameUserV66::init() {
    if (!HNLayer::init()) {
        return false;
    }

    LogonResult = PlatformLogic()->loginResult;

    m_canClose = true;
    
    auto myListener = EventListenerTouchOneByOne::create();
    myListener->setSwallowTouches(true);//阻止触摸向下传递
    myListener->onTouchBegan = [](Touch *touch, Event *unused_event){
        return true;
    };
    myListener->onTouchEnded = [&](Touch *touch, Event *unused_event){
        if (m_canClose) {
            close();
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(myListener, this);
    

	//auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 180));
	//addChild(colorLayer);

  //  auto bg = ImageView::create("csb/v66/userData/Images/bg.png");
  //  if (bg) {
  //      bg->setPosition(WIN_SIZE/2);
  //      bg->setTouchEnabled(true);
		//addChild(bg);

	//auto closeBtn = Button::create(MODULE_CLOSE_BTN_V66, MODULE_CLOSE_BTN_V66);
	//closeBtn->setPosition(Vec2(bg->getContentSize().width - 30, bg->getContentSize().height - 30));
	//bg->addChild(closeBtn);
	//closeBtn->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
	//	if (type != Widget::TouchEventType::ENDED) return;
	//	close();
	//});
//}
    
    //Sprite* switchBg = Sprite::create("csb/v66/userData/Images/mask.png");
    //Sprite* personal = Sprite::create("csb/v66/userData/Images/accountbg.png");
    //Sprite* accountSafe = Sprite::create("csb/v66/userData/Images/personalbg.png");
    //Sprite* thumb = Sprite::create();
    //ControlSwitch* pControl = ControlSwitch::create(
    //                                                switchBg,
    //                                                personal,
    //                                                accountSafe,
    //                                                thumb,
    //                                                Label::createWithTTF("", DEFAULT_FONT, 16),
    //                                                Label::createWithTTF("", DEFAULT_FONT, 16));
    //pControl->addTargetWithActionForControlEvents(this, cccontrol_selector(GameUserV66::callbackSwitch), Control::EventType::VALUE_CHANGED);
    //pControl->setPosition(Vec2(bg->getContentSize().width/2 +8, bg->getContentSize().height - 42));
    //bg->addChild(pControl);
    //
    //m_btnTitle = thumb;
    
    //两面板节点
    //Node* personalNode = Node::create();
    //personalNode->setPosition(bg->getContentSize()/2);
    //bg->addChild(personalNode);
    //
    //Node* accountSafeNode = Node::create();
    //accountSafeNode->setPosition(bg->getContentSize()/2);
    //accountSafeNode->setVisible(false);
    //bg->addChild(accountSafeNode);
    //
    //_vContentnode.push_back(personalNode);
    //_vContentnode.push_back(accountSafeNode);
    
    //初始化界面
    createPersonalPanel();
    //createAccountSafePanel();
    
    //pControl->setOn(false);
    //pControl->setEnabled(false);
    
    return true;
}

void GameUserV66::createPersonalPanel()
{
	_rootNode = CSLoader::createNode(USER_INFO_V66);
	addChild(_rootNode);
	_rootNode->setPosition(WIN_SIZE / 2);

	auto closeBtn = (Button*)_rootNode->getChildByName("bg")->getChildByName("Button_1");
	closeBtn->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) return;
		close();
	});
	
    //csb控件
    userID = (Text*)_rootNode->getChildByName("bg")->getChildByName("txtID");
	userID->setAnchorPoint(Vec2(0.5, 0.5));
    auto headPos = _rootNode->getChildByName("bg")->getChildByName("headImgNode");
    
    //动态创建
	HNEeditBoxNickName = HNEditBox::createEditBox((TextField*)(_rootNode->getChildByName("bg")->getChildByName("txtName")), this);
	HNEeditBoxNickName->setName("nickname");

	//位置特殊处理
	auto yPos = HNEeditBoxNickName->getPositionY();
	yPos = yPos * WIN_SIZE.height / 720;
	HNEeditBoxNickName->setPositionY(yPos);

    
    userHead = UserHead::create(HEAD_FRAME_PURPLE_V66);
    userHead->setHeadTouchEnabled(true);
	userHead->setPosition(Vec2(0,0));    
	float fscale11 = 0.35f;
	userHead->setScale(1.0);

    if (Tools::needLoadWechatImage()) {
        Texture2D* texture = new Texture2D;
        Tools::loadWechatImageFromLocal(texture);
        if (texture) {
            userHead->setClipUserHeadWithTexTure(texture, "hallimg/niuniu/5_1.png", 0.9f);
        }
    }
    else {
        std::string head_path = StringUtils::format(PLAYER_HEAD_V66, LogonResult.bLogoID);
        if (!FileUtils::getInstance()->isFileExist(head_path)) {
            head_path = DEFAULT_PLAYER_HEAD_V66;
        }
        userHead->setClipUserHeadWithFile(head_path, "hallimg/niuniu/5_1.png",fscale11);
    }
    
	headPos->addChild(userHead);
    userHead->onHeadClickEvent = [&](UserHead* sender, Vec2 touchVec) {
        loadModifyHeadLayer();
    };
    
    std::string name(ForceGBKToUtf8(LogonResult.nickName));
    if (!name.empty()) {
		HNEeditBoxNickName->setString(name);
    } else {
		HNEeditBoxNickName->setString(ForceGBKToUtf8("未知"));
    }
    
    userID->setString(StringUtils::format("%d", LogonResult.dwUserID));
    
//    copyBtn->addTouchEventListener(CC_CALLBACK_2(GameUserV66::buttonCopyCallBack, this));
//    isGraphSave = false;
    isNickSave = false;
    
    PlatformLogic()->addEventSelector(MDM_GP_SET_LOGO, ASS_GP_SET_LOGO, HN_SOCKET_CALLBACK(GameUserV66::modifyOKSelector, this));
    PlatformLogic()->addEventSelector(MDM_GP_USERINFO, ASS_GP_USERINFO_ACCEPT, HN_SOCKET_CALLBACK(GameUserV66::modifyNickAndGraphOKSelector, this));
    PlatformLogic()->addEventSelector(MDM_GP_USERINFO, ASS_GP_USERINFO_NOTACCEPT_BADWORD, HN_SOCKET_CALLBACK(GameUserV66::modifyFailureSelector, this));
}

void GameUserV66::createAccountSafePanel()
{
//    LoginResult = PlatformLogic()->loginResult;
//    
//    auto csb = CSLoader::createNode("csb/v66/userData/AccountSafeNode.csb");
//    bindPhone = (Button*)csb->getChildByName("btnBindPhone");
//    unBindPhone = (Button*)csb->getChildByName("btnUnBindPhone");
//    
//    auto custom = EventListenerCustom::create("setBindStatus",CC_CALLBACK_0(GameUserV66::setBindPhoneBtnImg,this));
//	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(custom, this);
//
//	setBindPhoneBtnImg();
//    _vContentnode.at(1)->addChild(csb);
////    btn->setPosition(_vContentnode.at(1)->getContentSize() / 2);
//	bindPhone->addTouchEventListener(CC_CALLBACK_2(GameUserV66::onBindPhoneCallback, this));
//    unBindPhone->addTouchEventListener(CC_CALLBACK_2(GameUserV66::onBindPhoneCallback, this));
}

void GameUserV66::setBindPhoneBtnImg()
{
    LoginResult = PlatformLogic()->loginResult;
    auto isNotBind = LoginResult.iBindMobile == 0;
    bindPhone->setVisible(isNotBind);
    unBindPhone->setVisible(!isNotBind);
    
}

void GameUserV66::onBindPhoneCallback(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
//    if (Widget::TouchEventType::ENDED != type)
//    {
//        return;
//    }
//    
////    openBindPhoneDialog(_vContentnode.at(1));
//    auto bindPhoneGui = BindPhoneGui::create();
//    string title("绑定手机");
//    if (PlatformLogic()->loginResult.iBindMobile)
//    {
//        title = "解绑手机";
//    }
//    bindPhoneGui->SetOpType(BindPhoneOpType::SendCode, true);
//    bindPhoneGui->Open(_vContentnode.at(1), title, cocos2d::Vec2::ZERO, 0, 0);
}

void GameUserV66::openBindPhoneDialog(cocos2d::Node *parent)
{
    auto csb = CSLoader::createNode("csb/v66/userData/bindPhone.csb");
    csb->setName("bindPhoneDialog");
    parent->addChild(csb, 100);
    
    auto phoneTemp = (TextField*)csb->getChildByName("textField_phone");
    phoneTemp->setColor(FONT_WHITE_COLOR_V66);
    phoneTemp->setVisible(false);
    phone = HNEditBox::createEditBox(phoneTemp, this);
    phone->setInput(HNEditBox::InputMode::PHONE_NUMBER);
    
    auto vertifyCodeTemp = (TextField*)csb->getChildByName("textField_code");
    vertifyCodeTemp->setColor(FONT_WHITE_COLOR_V66);
    vertifyCodeTemp->setVisible(false);
    vertifyCode = HNEditBox::createEditBox(vertifyCodeTemp, this);
    vertifyCode->setInput(HNEditBox::InputMode::PHONE_NUMBER);
    
    auto tempGetBtn = (Button*)csb->getChildByName("Button_getCode");
    tempGetBtn->setVisible(false);
    auto getBtn = AUIButton::create(BTN_YELLOW1_NORMAL_BG_V66, BTN_YELLOW1_SELECT_BG_V66, BTN_YELLOW1_ENABLED_BG_V66);
    getBtn->setTitle("v66/button/yellow1/yanzheng.png");
    getBtn->setPosition(tempGetBtn->getPosition());
    csb->addChild(getBtn);
    getBtn->addTouchEventListener(CC_CALLBACK_2(GameUserV66::handleGetBtn, this));
    
    auto tempSendBtn = (Button*)csb->getChildByName("Button_bind");
    tempSendBtn->setVisible(false);
    sendBtn = AUIButton::create(BTN_YELLOW1_NORMAL_BG_V66, BTN_YELLOW1_SELECT_BG_V66, BTN_YELLOW1_ENABLED_BG_V66);
    sendBtn->setPosition(tempSendBtn->getPosition());
    csb->addChild(sendBtn);
    sendBtn->addTouchEventListener(CC_CALLBACK_2(GameUserV66::handleSendBtn, this));
    
    updateBindInfo(LoginResult.iBindMobile, true);
}

void GameUserV66::handleGetBtn(Ref* pSender, Widget::TouchEventType type) {
    if (Widget::TouchEventType::ENDED != type)
        return;
    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
    
    std::string phoneNumber = phone->getText();
    if (phoneNumber.empty()) {
        CommandManager::getInstance()->showGamePrompt(GetText("BINDPHONE_INPUT_NUMBER"));
        return;
    }
    
    std::string URL = std::string("http://") + getWebServerUrl() + std::string("/Public/yx.aspx?");
    std::string str("type=sendcode&mobile=");
    str += phoneNumber;
    
    network::HttpRequest* request = new (std::nothrow) network::HttpRequest();
    request->setUrl(URL.c_str());
    request->setRequestType(network::HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(GameUserV66::onHttpCodeResponse, this));
    
    request->setRequestData(str.c_str(), str.size());
    network::HttpClient::getInstance()->sendImmediate(request);
}

void GameUserV66::updateBindInfo(bool isBind, bool isSuccess)
{
    vertifyCode->setText("");
    
    if (isBind && isSuccess)
    {
        sendBtn->removeChildByTag(99);
        sendBtn->setTitle(SET_BUTTON_UNBIND_TITLE_V66);
        
        phone->setString(ForceGBKToUtf8(LoginResult.szMobileNo));
        phone->setTouchEnabled(false);
    }
    
    if (!isBind && isSuccess)
    {
        sendBtn->removeChildByTag(99);
        sendBtn->setTitle(SET_BUTTON_BIND_TITLE_V66);
        
        phone->setTouchEnabled(true);
    }
}

void GameUserV66::handleSendBtn(Ref* pSender, Widget::TouchEventType type) {
    if (Widget::TouchEventType::ENDED != type)
        return;
    
    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
    
    std::string phoneNumber = phone->getString();
    std::string verifyCode = vertifyCode->getString();
    std::string str;
    if (!LoginResult.iBindMobile) {
        str = "type=verifycode&mobile=";
    }
    else {
        str = "type=clearbind&mobile=";
    }
    
    int userID = LoginResult.dwUserID;
    
    if (phoneNumber.empty()) {
        CommandManager::getInstance()->showGamePrompt(GetText("BINDPHONE_INPUT_NUMBER"));
        GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_INPUT_NUMBER"));
        return;
    }
    
    if (verifyCode.empty()) {
        CommandManager::getInstance()->showGamePrompt(GetText("BINDPHONE_INPUT_VERIFICATION"));
        GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_INPUT_VERIFICATION"));
        return;
    }
    
    if (phoneNumber.length() != 11) {
        CommandManager::getInstance()->showGamePrompt(GetText("BINDPHONE_NUMBER_LENGTH_ISNOT"));
        GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_NUMBER_LENGTH_ISNOT"));
        return;
    }
    
    std::string URL = std::string("http://") + getWebServerUrl() + std::string("/Public/yx.aspx?");
    str += phoneNumber;
    str += "&code=" + verifyCode;
    str += "&userid=" + to_string(userID);
    
    network::HttpRequest* request = new (std::nothrow) network::HttpRequest();
    request->setUrl(URL.c_str());
    request->setRequestType(network::HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(GameUserV66::onHttpBindResponse, this));
    
    request->setRequestData(str.c_str(), str.size());
    network::HttpClient::getInstance()->sendImmediate(request);
    
    request->release();
}

void GameUserV66::onHttpCodeResponse(HttpClient* client, HttpResponse* response)
{
    std::vector<char> recv = *response->getResponseData();
    std::string data;
    data.assign(recv.begin(), recv.end());
    
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
    
    if (doc.HasParseError() || !doc.IsObject() || !doc.HasMember("status")) {
        return;
    }
    
    int result = doc["status"].GetInt();
    if (result == 0) {
        CommandManager::getInstance()->showGamePrompt(GetText("BINDPHONE_SEND_LOSER"));
    }
    else if (result == 1) {
        CommandManager::getInstance()->showGamePrompt(GetText("BINDPHONE_SEND_OK"));
    }
}

void GameUserV66::onHttpBindResponse(network::HttpClient* client, network::HttpResponse* response) {
    std::vector<char> recv = *response->getResponseData();
    std::string data;
    data.assign(recv.begin(), recv.end());
    
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
    
    if (doc.HasParseError() || !doc.IsObject() || !doc.HasMember("status")) {
        return;
    }
    
    int result = doc["status"].GetInt();
    if (result == 0) {
        if (!LoginResult.iBindMobile) {//绑定成功
            CommandManager::getInstance()->showGamePrompt(GetText("BINDPHONE_BING_OK"));
            updateBindInfo(true, true);
        }
        else {//解绑失败
            CommandManager::getInstance()->showGamePrompt(GetText("BINDPHONE_BINGJIE_LOSER"));
            updateBindInfo(false, false);
        }
    }
    else if (result == 1) {
        LoginResult.iBindMobile = !LoginResult.iBindMobile;
        PlatformLogic()->loginResult.iBindMobile = LoginResult.iBindMobile;
        if (!LoginResult.iBindMobile) {//解绑成功
            CommandManager::getInstance()->showGamePrompt(GetText("BINDPHONE_BINGJIE_OK"));
            strcpy(LoginResult.szMobileNo, phone->getText());
            updateBindInfo(false, true);
        }
        else {//绑定成功
            CommandManager::getInstance()->showGamePrompt(GetText("BINDPHONE_BING_OK"));
            strcpy(LoginResult.szMobileNo, "");
            strcpy(LoginResult.szMobileNo, phone->getText());
            strcpy(PlatformLogic()->loginResult.szMobileNo, Utf8ToGB(phone->getText()));
            updateBindInfo(true, true);
        }
    }
    else if (result == 2) {//重复绑定
        CommandManager::getInstance()->showGamePrompt(GetText("BINDPHONE_BING_REPORT"));
    }
    else if (result == 3) {//验证码验证失败
        CommandManager::getInstance()->showGamePrompt(GetText("BINDPHONE_VERIFICATION_LOSER"));
    }
}


bool GameUserV66::modifyNickAndGraphOKSelector(HNSocketMessage* socketMessage) {
    CommandManager::getInstance()->execute(REMOVE_LOADER_COM);

    if (isNickSave) {
        isNickSave = false;
        m_canClose = true;
        CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("修改昵称成功！"));
        strcpy(PlatformLogic()->loginResult.nickName, Utf8ToGB(HNEeditBoxNickName->getString().c_str()));
        updateNicknameCallBack(HNEeditBoxNickName->getString());
    }
//    if (isGraphSave) {
//        isGraphSave = false;
//        CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("新签名修改成功!"));
//        strcpy(PlatformLogic()->loginResult.szSignDescr, Utf8ToGB(HNEeditBoxAutoGraph->getString().c_str()));
//    }
    return false;
}

bool GameUserV66::modifyFailureSelector(HNSocketMessage* socketMessage) {
    CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
    CommandManager::getInstance()->showGamePrompt(GetText("Sign_Default"));
    return true;
}

 bool GameUserV66::modifyOKSelector(HNSocketMessage* socketMessage) {
     CCAssert(sizeof(MSG_GP_R_SetLogo) == socketMessage->objectSize, "MSG_GP_R_BindMobile is error.");
     if (socketMessage->objectSize != sizeof(MSG_GP_R_SetLogo)) return true;
 
     MSG_GP_R_SetLogo* data = (MSG_GP_R_SetLogo*)(socketMessage->object);
 
     switch (data->dwCommanResult) {
     case 1: {
         CommandManager::getInstance()->showGamePrompt(GBKToUtf8("修改头像成功！！！"));
         if (userHeadsLayer)
         {
             userHeadsLayer->removeFromParent();
             userHeadsLayer = nullptr;
         }
 
         int logoID = UserDefault::getInstance()->getIntegerForKey("TMP_LOGO_ID");
         PlatformLogic()->getInstance()->loginResult.bLogoID = logoID;
         userHead->setUserHead(StringUtils::format(HEAD_IMAGE_PATH_V66, PlatformLogic()->getInstance()->loginResult.bLogoID));
         userHead->setUserHeadScale(1.3f);
         updateHeadCallBack();
         break;
     }
     case 0: {
         CommandManager::getInstance()->showGamePrompt(GBKToUtf8("修改头像失败。。。"));
         break;
     }
     }
     return false;
 }

void GameUserV66::modifyUserHeadEventCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }
    loadModifyHeadLayer();
}

void GameUserV66::modifySignature(std::string signature) {
    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
    if (strcmp(signature.c_str(), PlatformLogic()->loginResult.szSignDescr) == 0) {
        CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("内容相同请重新输入！！！"));

        return;
    }

    if (!Tools::isSpecialCharacter(signature.c_str())) {
        CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("含特殊符号！"));
        return;
    }

    MSG_GP_UserInfo logonResult;
    memset(&logonResult, 0, sizeof(logonResult));
    logonResult.dwUserID = PlatformLogic()->loginResult.dwUserID;
    logonResult.i64Bank = PlatformLogic()->loginResult.i64Bank;
    logonResult.i64Money = PlatformLogic()->loginResult.i64Money;
    strcpy(logonResult.szSignDescr, Utf8ToGB(signature.c_str()));
    strcpy(logonResult.szMD5Pass, PlatformLogic()->loginResult.szMD5Pass);
    PlatformLogic()->sendData(MDM_GP_USERINFO, ASS_GP_USERINFO_UPDATE_BASE, &logonResult, sizeof(logonResult));
}

void GameUserV66::modifyNickname(std::string nickname) {
    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
    if (strcmp(nickname.c_str(), PlatformLogic()->loginResult.nickName) == 0) {
        CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("内容相同请重新输入！！！"));
        return;
    }
    //获取昵称
    if (nickname.empty()) {
        CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("昵称不能为空！"));
        return;
    }
    
    //假设输入的一个汉字是3个字节，计算时按照2个算
    unsigned long len = nickname.length();
    const char* s = nickname.c_str();
    int i = 0, j = 0;
    while (s[i]) {
        if ((s[i] & 0xc0) != 0x80) j++;
        i++;
    }
    int hanCount = (i-j)/2;
    int otherCount = j-hanCount;
    len = hanCount*2+otherCount;
    
    if (len < 4 || len > 16) {
        CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("请输入2-8位昵称！"));

        return;
    }

    if (!Tools::isSpecialCharacter(nickname.c_str())) {
        CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("含特殊符号！"));

        return;
    }

    HNLOG_INFO("length = %s ", nickname.c_str());

    MSG_GP_UserInfo logonResult;
    auto  result = PlatformLogic()->loginResult;
    memset(&logonResult, 0, sizeof(logonResult));
    logonResult.dwUserID = result.dwUserID;
    strcpy(logonResult.szMD5Pass, result.szMD5Pass);
    strcpy(logonResult.szSignDescr, Utf8ToGB(result.szSignDescr));
    strcpy(logonResult.szRealName, result.szRealName);
    strcpy(logonResult.szIDCardNo, result.szIDCardNo);
    strcpy(logonResult.szMobileNo, result.szMobileNo);
    strcpy(logonResult.szQQNum, result.szQQNum);
    strcpy(logonResult.szAdrProvince, result.szAdrProvince);
    strcpy(logonResult.szAdrCity, result.szAdrCity);
    strcpy(logonResult.szZipCode, result.szZipCode);
    strcpy(logonResult.szEmail, result.szEmail);
    strcpy(logonResult.szAddress, result.szAddress);
    strcpy(logonResult.szSchooling, result.szSchooling);
    strcpy(logonResult.szHomePage, result.szHomePage);
    strcpy(logonResult.szTelNo, result.szTelNo);
    strcpy(logonResult.szMSNID, result.szMSNID);
    strcpy(logonResult.nickName, Utf8ToGB(nickname.c_str())); 

    PlatformLogic()->sendData(MDM_GP_USERINFO, ASS_GP_USERINFO_UPDATE_DETAIL, &logonResult, sizeof(logonResult));
}

void GameUserV66::buttonCopyCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED)
        return;
	char str[180] = { 0 };
	sprintf(str, "用户名:%s\n用户ID:%s", HNEeditBoxNickName->getString().c_str(), userID->getString().c_str());

#if(CC_TARGET_PLATFORM  == CC_PLATFORM_ANDROID)
	JniMethodInfo minfo;//定义Jni函数信息结构体
	//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
	bool isHave = JniHelper::getStaticMethodInfo(minfo, "com/jlx/MixProject/AppActivity", "paste", "(Ljava/lang/String;)V");
	if (!isHave) {
	} else {
		jobject para = minfo.env->NewStringUTF(str);
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, para);
		CommandManager::getInstance()->showGamePrompt(GetText("USERINFO_COPYID"));
	}
#elif CC_TARGET_PLATFORM  == CC_PLATFORM_IOS
	ClipboardHelper::copyStr(str);
	CommandManager::getInstance()->showGamePrompt(GetText("USERINFO_COPYID"));
#endif
}

void GameUserV66::editBoxEditingDidBegin(ui::EditBox *editBox) {
    if (editBox->getName().compare("nickname") == 0) {
        m_canClose = false;
    }
}

void GameUserV66::editBoxReturn(ui::EditBox* editBox) {
    if (editBox->getName().compare("nickname") == 0) {
        isNickSave = true;
        modifyNickname(editBox->getText());
    }
//    m_canClose = true;
}

void GameUserV66::open(Node* parent, Vec2 vec, int zorder, int tag) {
    CCAssert(nullptr != parent, "parent is nullptr");
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setPosition(vec);
    parent->addChild(this, zorder, tag);
    
    //setScale(0.2);
    //runAction(ScaleTo::create(0.2f, 1.0f));
    
    //请求邀请码是否有效
	//MSG_GP_S_InviteCode invite;
	//memset(&invite, 0, sizeof(MSG_GP_S_InviteCode));
	//invite.userID = PlatformLogic()->getInstance()->loginResult.dwUserID;
	//PlatformLogic()->sendData(MDM_GP_GetInviteCode, ASS_GP_GetInviteCode, &invite, sizeof(MSG_GP_S_InviteCode), HN_SOCKET_CALLBACK(GameUserV66::getInvitedCodeSelector, this));
}

void GameUserV66::close() {
    //runAction(Sequence::create(ScaleTo::create(0.2f, 0.2f), RemoveSelf::create(), nullptr));
    PlatformLogic()->removeEventSelector(MDM_GP_SET_LOGO,ASS_GP_SET_LOGO);
    PlatformLogic()->removeEventSelector(MDM_GP_USERINFO,ASS_GP_USERINFO_ACCEPT);
    PlatformLogic()->removeEventSelector(MDM_GP_USERINFO,ASS_GP_USERINFO_NOTACCEPT_BADWORD);
	removeFromParent();
}

void GameUserV66::loadModifyHeadLayer() {
	userHeadsLayer = dynamic_cast<GameUserHeadV66*>(GameUserHeadFactory::createWithVersion());
	userHeadsLayer->setGameUserDelegate(this); 
	//userHeadsLayer->setTopMenuDelegate(m_TopMenuDelegate);
	userHeadsLayer->open(this->getParent(), Vec2(WIN_SIZE.width, 0), MODULE_LAYER_ORDER_V66, 0);

	//m_TopMenuDelegate->setLayerDelegate(userHeadsLayer);

	userHeadsLayer->updateHeadCallBack = [&](){
        unsigned int logoID = PlatformLogic()->getInstance()->loginResult.bLogoID;
        if(Tools::headFromWX(logoID))
        {
            Texture2D* texture = new Texture2D;
            Tools::loadWechatImageFromLocal(texture);
            if (texture && userHead) {
                userHead->setClipUserHeadWithTexTure(texture);
            }
        }
        else
        {
            userHead->setClipUserHeadWithFile(StringUtils::format(PLAYER_HEAD_V66, logoID),"v66/module/gerenxinxi/5.png", 0.35f);
        }
		updateHeadCallBack();
	};
}

void GameUserV66::callbackSwitch(Ref* pSender, Control::EventType event) {
    //获取事件的传递者CCControlSwitch
    ControlSwitch* controlSwitch = (ControlSwitch*)pSender;
    bool isPersonalShow = controlSwitch->isOn();
    
    
    //_vContentnode.at(0)->setVisible(!isPersonalShow);
    //_vContentnode.at(1)->setVisible(isPersonalShow);

//    if(isPersonalShow)
//    {
//        m_btnTitle->setTexture("csb/v66/userData/Images/account.png");
//    }
//    else{
//        m_btnTitle->setTexture("csb/v66/userData/Images/personal.png");
//    }
}

void GameUserV66::codeCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }

	int tag = (dynamic_cast<AUIButton*>(pSender))->getTag();

    auto invitedLayer = InviteCodeLayerV66::create();
    if (invitedLayer) {
		invitedLayer->setCodeType(tag);
        invitedLayer->open(this->getParent(), Vec2::ZERO, MODULE_LAYER_ORDER_V66 + 5);
    }
}

bool GameUserV66::getInvitedCodeSelector(HNSocketMessage* socketMessage) {
    if (socketMessage == nullptr) {
        //网络异常时默认显示邀请码
        if (inviteCodeBtn) {
            inviteCodeBtn->setVisible(true);
            
            //调整下位置
            inviteCodeBtn->setPositionY(inviteCodeBtn->getPositionY() + 60);
            userHead->setPositionY(userHead->getPositionY() + 60);
        }
        return true;
    }
    if (socketMessage->messageHead.bHandleCode == 0) {
      //已经获得过邀请码奖励
      if (inviteCodeBtn) {
          inviteCodeBtn->setVisible(false);
      }
    }
    else if (socketMessage->messageHead.bHandleCode == 1) {
      //没有获得过邀请码，可以去获得
      if (inviteCodeBtn) {
          inviteCodeBtn->setVisible(true);

          //调整下位置
          inviteCodeBtn->setPositionY(inviteCodeBtn->getPositionY() + 60);
          userHead->setPositionY(userHead->getPositionY() + 60);
      }
    }
    return true;
}
void GameUserV66::createDownloadDimensionCode() //创建一个下载二维码
{
    
    std::string imgHeadUrl = "";//填写一个下载地址
    
    cocos2d::network::HttpRequest *request = new (std::nothrow) cocos2d::network::HttpRequest();
    request->setUrl(imgHeadUrl);
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setResponseCallback([=](cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response) {
        if (response->isSucceed()) {
            std::vector<char>* buffer = response->getResponseData();
            char *buff = (char *)malloc(buffer->size());
            std::copy(buffer->begin(), buffer->end(), buff);
            
            if (buffer->size() > 0) {
                auto fileName = FileUtils::getInstance()->getWritablePath() + "dimensionCode.jpg";
                FILE *fp = fopen(fileName.c_str(), "wb+");
                fwrite(buff, 1, buffer->size(), fp);
                fclose(fp);
            }
        }
        
        auto fileName = FileUtils::getInstance()->getWritablePath() + "dimensionCode.jpg";
        if (FileUtils::getInstance()->isFileExist(fileName))
        {
            auto data = FileUtils::getInstance()->getDataFromFile(fileName);
            if (!data.isNull()) {
                Texture2D* texture = new Texture2D;
                Image* image = new (std::nothrow) Image();
                if (image) {
                    image->initWithImageData(data.getBytes(), data.getSize());
                    texture->initWithImage(image);
                    if (texture) {
                        texture->autorelease();
                    }
                    CC_SAFE_RELEASE(image);
                    
                    Sprite* content = Sprite::create();
                    content->initWithTexture(texture);
                    content->setPosition(WIN_SIZE/2);
                    this->addChild(content);
                }
            }
        }
        
        
        
    });
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();
    
    
}
