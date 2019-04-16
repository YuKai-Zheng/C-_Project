#include "GameUserV88.h"
#include "../pattern/factory/background/BackgroundFactory.h"
#include "../pattern/factory/gameUserHead/GameUserHeadFactory.h"
#include "../CommonMacro.h"
#include "PlatformResV88.h"
#include "cocostudio/CocoStudio.h"
#include "Sound/HNAudioEngine.h"
#include "../PlatformDefine.h"
#include "UI/UserHead.h"
#include "HNPlatform/HNPlatformLogic.h"
#include "../PlatformMessage.h"
#include "../GamePrompt.h"
#include "GameUserHeadV88.h"
#include "../pattern/CommandManager.h"
#include "CommonRes.h"
#include "base/ClipboardHelper.h"
#include "VersionInfo/VersionInfo.h"
#include "HNLobby/v88/InvitedCodeLayerV88.h"

//const char* USER_DATA_MODIFY_TITLE_V88 = "v88/button/green/4.png";
//const char* USER_DATA_COPY_TITLE_V88 = "v88/button/yellow2/8.png";

const char* SWITCH_BG_PERSONAL_V88 = "v88/module/gerenxinxi/btnBg.png";
const char* SWITCH_BAR_PERSONAL_V88 = "v88/module/gerenxinxi/2.png";
const char* PERSONAL_SWITCH_V88 = "v88/module/gerenxinxi/1_1.png";
const char* ACCOUNT_SAFE_SWITCH_V88 = "v88/module/gerenxinxi/1.png";

const char* SET_BUTTON_UNBIND_TITLE_V88 = "v88/button/yellow1/jiebang.png";
const char* SET_BUTTON_BIND_TITLE_V88 = "v88/button/yellow1/bangding.png";

bool GameUserV88::init() {
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
    
	auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 100));
	addChild(colorLayer);

    auto bg = ImageView::create(MODULE_BG_V88);
    bg->setPosition(WIN_SIZE/2);
    bg->setTouchEnabled(true);
    addChild(bg);
    
    Sprite* switchBg = Sprite::create(SWITCH_BG_PERSONAL_V88);
    Sprite* personal = Sprite::create(PERSONAL_SWITCH_V88);
    Sprite* accountSafe = Sprite::create(ACCOUNT_SAFE_SWITCH_V88);
    Sprite* thumb = Sprite::create(SWITCH_BAR_PERSONAL_V88);
    ControlSwitch* pControl = ControlSwitch::create(switchBg, personal, accountSafe, thumb, Label::createWithTTF("", DEFAULT_FONT, 16), Label::createWithTTF("", DEFAULT_FONT, 16));
    pControl->addTargetWithActionForControlEvents(this, cccontrol_selector(GameUserV88::callbackSwitch), Control::EventType::VALUE_CHANGED);
    pControl->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height - 42));
    bg->addChild(pControl);
    if (Tools::isiOSReview()) {
        if (pControl) {
            pControl->setVisible(false);
        }
    }
    
    m_btnTitle = Sprite::create();
    m_btnTitle->setPosition(thumb->getContentSize()/2);
    thumb->addChild(m_btnTitle);
    
    //两面板节点
    Node* personalNode = Node::create();
    personalNode->setPosition(bg->getContentSize()/2);
    bg->addChild(personalNode);
    
    Node* accountSafeNode = Node::create();
    accountSafeNode->setPosition(bg->getContentSize()/2);
    accountSafeNode->setVisible(false);
    bg->addChild(accountSafeNode);
    
    _vContentnode.push_back(personalNode);
    _vContentnode.push_back(accountSafeNode);
    
    //初始化界面
    createPersonalPanel();
    createAccountSafePanel();
    
    pControl->setOn(false);
    
    return true;
}

void GameUserV88::createPersonalPanel()
{
    auto userDataLayer = CSLoader::createNode(USER_INFO_V88);
    _vContentnode.at(0)->addChild(userDataLayer);
    
    //csb控件
    userID = (Text*)userDataLayer->getChildByName("Text_ID");
    userID->setColor(FONT_CLOLOR_YELLOW_V88);
    TextField* nickname = (TextField*)userDataLayer->getChildByName("textField_nickName");
    nickname->setColor(FONT_CLOLOR_YELLOW_V88);
    bankMoney = (Text*)userDataLayer->getChildByName("Text_bankMoney");
    bankMoney->setColor(FONT_CLOLOR_YELLOW_V88);
    bagMoney = (Text*)userDataLayer->getChildByName("Text_money");
    bagMoney->setColor(FONT_CLOLOR_YELLOW_V88);
//    TextField* signature = (TextField*)userDataLayer->getChildByName("textField_autoGraph");
//    signature->setColor(FONT_CLOLOR_YELLOW_V88);
    auto textHead = (Text*)userDataLayer->getChildByName("Text_head");
    textHead->setVisible(false);
    //auto tempCopyBtn = (Button*)userDataLayer->getChildByName("button_copy");
    //auto tempModifyBtn = (Button*)userDataLayer->getChildByName("button_yes");
    auto bankLabel = dynamic_cast<Sprite*>(userDataLayer->getChildByName("Sprite_3"));
    
    //IOS审核版本
    if (Tools::isiOSReview())
    {
        if (bankMoney) {
            bankMoney->setVisible(false);
        }
        if (bankLabel) {
            bankLabel->setVisible(false);
        }
    }
    
    //动态创建
    HNEeditBoxNickName = HNEditBox::createEditBox(nickname, this);
    HNEeditBoxNickName->setName("nickname");
//    HNEeditBoxAutoGraph = HNEditBox::createEditBox(signature, this);
//    HNEeditBoxAutoGraph->setName("signature");
    
//    tempModifyBtn->setVisible(false);
//    modifyBtn = AUIButton::create(BTN_GREEN_BG_V88, BTN_GREEN_BG_V88);
//    modifyBtn->setTitle(USER_DATA_MODIFY_TITLE_V88);
//    modifyBtn->setTitleOffset(Vec2(0, 4));
//    modifyBtn->setPosition(tempModifyBtn->getPosition());
//    userDataLayer->addChild(modifyBtn);
    
//    tempCopyBtn->setVisible(false);
//    copyBtn = AUIButton::create(BTN_YELLOW2_BG_V88, BTN_YELLOW2_BG_V88);
//    copyBtn->setTitle(USER_DATA_COPY_TITLE_V88);
//    copyBtn->setPosition(tempCopyBtn->getPosition());
//    userDataLayer->addChild(copyBtn);
    
    userHead = UserHead::create("v88/module/gerenxinxi/4.png");
    userHead->setHeadTouchEnabled(true);
	userHead->setPosition(Vec2(textHead->getPosition().x, textHead->getPosition().y));
    
    if (Tools::needLoadWechatImage()) {
        Texture2D* texture = new Texture2D;
        Tools::loadWechatImageFromLocal(texture);
        if (texture) {
            userHead->setClipUserHeadWithTexTure(texture, "v88/module/gerenxinxi/5.png", 0.9f);
        }
    }
    else {
        std::string head_path = StringUtils::format(PLAYER_HEAD_V88, LogonResult.bLogoID);
        if (!FileUtils::getInstance()->isFileExist(head_path)) {
            head_path = DEFAULT_PLAYER_HEAD_V88;
        }
        userHead->setClipUserHeadWithFile(head_path, "v88/module/gerenxinxi/5.png");
    }
    
    userDataLayer->addChild(userHead);
    userHead->onHeadClickEvent = [&](UserHead* sender, Vec2 touchVec) {
        loadModifyHeadLayer();
    };
	/*inviteCodeBtn = nullptr;
	if (!Tools::isiOSReview()) {
	inviteCodeBtn = AUIButton::create("v88/button/yellow1/BG.png", "v88/button/yellow1/BG_1.png");
	if (inviteCodeBtn) {
	inviteCodeBtn->setVisible(false);
	inviteCodeBtn->setTitle("v88/button/yellow1/13.png");
	inviteCodeBtn->setTitleOffset(Vec2(-4, 4));
	inviteCodeBtn->setTag(0);
	inviteCodeBtn->addTouchEventListener(CC_CALLBACK_2(GameUserV88::codeCallBack, this));
	inviteCodeBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	inviteCodeBtn->setPosition(Vec2(userHead->getPositionX(), userHead->getPositionY()-userHead->getContentSize().height/2-inviteCodeBtn->getContentSize().height/2));
	userDataLayer->addChild(inviteCodeBtn);
	}
	}*/
    
    std::string name(ForceGBKToUtf8(LogonResult.nickName));
    if (!name.empty()) {
        HNEeditBoxNickName->setString(name);
    } else {
        HNEeditBoxNickName->setString(ForceGBKToUtf8("未知"));
    }
    
    bagMoney->setString(StringUtils::format("%lld",LogonResult.i64Money));
    bankMoney->setString(StringUtils::format("%lld", LogonResult.i64Bank));
//    HNEeditBoxAutoGraph->setString(ForceGBKToUtf8(LogonResult.szSignDescr));
    userID->setString(StringUtils::format("%d", LogonResult.dwUserID));
    
//    modifyBtn->addTouchEventListener(CC_CALLBACK_2(GameUserV88::modifyUserHeadEventCallBack, this));
//    copyBtn->addTouchEventListener(CC_CALLBACK_2(GameUserV88::buttonCopyCallBack, this));
//    isGraphSave = false;
    isNickSave = false;
    
    //  PlatformLogic()->addEventSelector(MDM_GP_SET_LOGO, ASS_GP_SET_LOGO, HN_SOCKET_CALLBACK(GameUserV88::modifyOKSelector, this));
    PlatformLogic()->addEventSelector(MDM_GP_USERINFO, ASS_GP_USERINFO_ACCEPT, HN_SOCKET_CALLBACK(GameUserV88::modifyNickAndGraphOKSelector, this));
    PlatformLogic()->addEventSelector(MDM_GP_USERINFO, ASS_GP_USERINFO_NOTACCEPT_BADWORD, HN_SOCKET_CALLBACK(GameUserV88::modifyFailureSelector, this));
}

void GameUserV88::createAccountSafePanel()
{
    LoginResult = PlatformLogic()->loginResult;
    
    auto csb = CSLoader::createNode("csb/v88/userData/bindPhone.csb");
    _vContentnode.at(1)->addChild(csb);
    
    auto phoneTemp = (TextField*)csb->getChildByName("textField_phone");
    phoneTemp->setColor(FONT_CLOLOR_YELLOW_V88);
    phoneTemp->setVisible(false);
    phone = HNEditBox::createEditBox(phoneTemp, this);
    phone->setInput(HNEditBox::InputMode::PHONE_NUMBER);
    
    auto vertifyCodeTemp = (TextField*)csb->getChildByName("textField_code");
    vertifyCodeTemp->setColor(FONT_CLOLOR_YELLOW_V88);
    vertifyCodeTemp->setVisible(false);
    vertifyCode = HNEditBox::createEditBox(vertifyCodeTemp, this);
    vertifyCode->setInput(HNEditBox::InputMode::PHONE_NUMBER);
    
    auto tempGetBtn = (Button*)csb->getChildByName("Button_getCode");
    tempGetBtn->setVisible(false);
    auto getBtn = AUIButton::create(BTN_YELLOW1_NORMAL_BG_V88, BTN_YELLOW1_SELECT_BG_V88, BTN_YELLOW1_ENABLED_BG_V88);
    getBtn->setTitle("v88/button/yellow1/yanzheng.png");
    getBtn->setPosition(tempGetBtn->getPosition());
    csb->addChild(getBtn);
    getBtn->addTouchEventListener(CC_CALLBACK_2(GameUserV88::handleGetBtn, this));
    
    auto tempSendBtn = (Button*)csb->getChildByName("Button_bind");
    tempSendBtn->setVisible(false);
    sendBtn = AUIButton::create(BTN_YELLOW1_NORMAL_BG_V88, BTN_YELLOW1_SELECT_BG_V88, BTN_YELLOW1_ENABLED_BG_V88);
    sendBtn->setPosition(tempSendBtn->getPosition());
    csb->addChild(sendBtn);
    sendBtn->addTouchEventListener(CC_CALLBACK_2(GameUserV88::handleSendBtn, this));
    
    updateBindInfo(LoginResult.iBindMobile, true);
}

void GameUserV88::handleGetBtn(Ref* pSender, Widget::TouchEventType type) {
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
    request->setResponseCallback(CC_CALLBACK_2(GameUserV88::onHttpCodeResponse, this));
    
    request->setRequestData(str.c_str(), str.size());
    network::HttpClient::getInstance()->sendImmediate(request);
}

void GameUserV88::updateBindInfo(bool isBind, bool isSuccess)
{
    vertifyCode->setText("");
    
    if (isBind && isSuccess)
    {
        sendBtn->removeChildByTag(99);
        sendBtn->setTitle(SET_BUTTON_UNBIND_TITLE_V88);
        
        phone->setString(ForceGBKToUtf8(LoginResult.szMobileNo));
        phone->setTouchEnabled(false);
    }
    
    if (!isBind && isSuccess)
    {
        sendBtn->removeChildByTag(99);
        sendBtn->setTitle(SET_BUTTON_BIND_TITLE_V88);
        
        phone->setTouchEnabled(true);
    }
}

void GameUserV88::handleSendBtn(Ref* pSender, Widget::TouchEventType type) {
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
        //GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_INPUT_NUMBER"));
        return;
    }
    
    if (verifyCode.empty()) {
        CommandManager::getInstance()->showGamePrompt(GetText("BINDPHONE_INPUT_VERIFICATION"));
        //GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_INPUT_VERIFICATION"));
        return;
    }
    
    if (phoneNumber.length() != 11) {
        CommandManager::getInstance()->showGamePrompt(GetText("BINDPHONE_NUMBER_LENGTH_ISNOT"));
        //GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_NUMBER_LENGTH_ISNOT"));
        return;
    }
    
    std::string URL = std::string("http://") + getWebServerUrl() + std::string("/Public/yx.aspx?");
    str += phoneNumber;
    str += "&code=" + verifyCode;
    str += "&userid=" + to_string(userID);
    
    network::HttpRequest* request = new (std::nothrow) network::HttpRequest();
    request->setUrl(URL.c_str());
    request->setRequestType(network::HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(GameUserV88::onHttpBindResponse, this));
    
    request->setRequestData(str.c_str(), str.size());
    network::HttpClient::getInstance()->sendImmediate(request);
    
    request->release();
}

void GameUserV88::onHttpCodeResponse(HttpClient* client, HttpResponse* response)
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

void GameUserV88::onHttpBindResponse(network::HttpClient* client, network::HttpResponse* response) {
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


bool GameUserV88::modifyNickAndGraphOKSelector(HNSocketMessage* socketMessage) {
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

bool GameUserV88::modifyFailureSelector(HNSocketMessage* socketMessage) {
    CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
    CommandManager::getInstance()->showGamePrompt(GetText("Sign_Default"));
    return true;
}

// bool GameUserV88::modifyOKSelector(HNSocketMessage* socketMessage) {
//     CCAssert(sizeof(MSG_GP_R_SetLogo) == socketMessage->objectSize, "MSG_GP_R_BindMobile is error.");
//     if (socketMessage->objectSize != sizeof(MSG_GP_R_SetLogo)) return true;
// 
//     MSG_GP_R_SetLogo* data = (MSG_GP_R_SetLogo*)(socketMessage->object);
// 
//     switch (data->dwCommanResult) {
//     case 1: {
//         CommandManager::getInstance()->showGamePrompt(GBKToUtf8("修改头像成功！！！"));
// 		if (userHeadsLayer)
// 		{
// 			userHeadsLayer->removeFromParent();
// 			userHeadsLayer = nullptr;
// 		}
// 
//         int logoID = UserDefault::getInstance()->getIntegerForKey("TMP_LOGO_ID");
//         PlatformLogic()->getInstance()->loginResult.bLogoID = logoID;
//         userHead->setUserHead(StringUtils::format(HEAD_IMAGE_PATH_V88, PlatformLogic()->getInstance()->loginResult.bLogoID));
// 		userHead->setUserHeadScale(1.3f);
// 		updateHeadCallBack();
//         break;
//     }
//     case 0: {
//         CommandManager::getInstance()->showGamePrompt(GBKToUtf8("修改头像失败。。。"));
//         break;
//     }
//     }
//     return false;
// }

void GameUserV88::modifyUserHeadEventCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }
    loadModifyHeadLayer();
}

//void GameUserV88::modifySignature(std::string signature) {
//    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
//    if (strcmp(signature.c_str(), PlatformLogic()->loginResult.szSignDescr) == 0) {
//        CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("内容相同请重新输入！！！"));
//
//        return;
//    }
//
//    if (!Tools::isSpecialCharacter(signature.c_str())) {
//        CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("含特殊符号！"));
//        return;
//    }
//
//    MSG_GP_UserInfo logonResult;
//    memset(&logonResult, 0, sizeof(logonResult));
//    logonResult.dwUserID = PlatformLogic()->loginResult.dwUserID;
//    logonResult.i64Bank = PlatformLogic()->loginResult.i64Bank;
//    logonResult.i64Money = PlatformLogic()->loginResult.i64Money;
//    strcpy(logonResult.szSignDescr, Utf8ToGB(signature.c_str()));
//    strcpy(logonResult.szMD5Pass, PlatformLogic()->loginResult.szMD5Pass);
//    PlatformLogic()->sendData(MDM_GP_USERINFO, ASS_GP_USERINFO_UPDATE_BASE, &logonResult, sizeof(logonResult));
//}

void GameUserV88::modifyNickname(std::string nickname) {
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

void GameUserV88::buttonCopyCallBack(Ref* pSender, Widget::TouchEventType type) {
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

void GameUserV88::editBoxEditingDidBegin(ui::EditBox *editBox) {
    if (editBox->getName().compare("nickname") == 0) {
        m_canClose = false;
    }
}

void GameUserV88::editBoxReturn(ui::EditBox* editBox) {
    if (editBox->getName().compare("nickname") == 0) {
        isNickSave = true;
        modifyNickname(editBox->getText());
    }
//    m_canClose = true;
}

void GameUserV88::open(Node* parent, Vec2 vec, int zorder, int tag) {
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
	//PlatformLogic()->sendData(MDM_GP_GetInviteCode, ASS_GP_GetInviteCode, &invite, sizeof(MSG_GP_S_InviteCode), HN_SOCKET_CALLBACK(GameUserV88::getInvitedCodeSelector, this));
}

void GameUserV88::close() {
    //runAction(Sequence::create(ScaleTo::create(0.2f, 0.2f), RemoveSelf::create(), nullptr));
	removeFromParent();
}

void GameUserV88::loadModifyHeadLayer() {
	userHeadsLayer = dynamic_cast<GameUserHeadV88*>(GameUserHeadFactory::createWithVersion());
	userHeadsLayer->setGameUserDelegate(this); 
	//userHeadsLayer->setTopMenuDelegate(m_TopMenuDelegate);
	userHeadsLayer->open(this->getParent(), Vec2(WIN_SIZE.width, 0), MODULE_LAYER_ORDER_V88, 0);

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
            userHead->setClipUserHeadWithFile(StringUtils::format(PLAYER_HEAD_V88, logoID));
        }
		updateHeadCallBack();
	};
}


void GameUserV88::callbackSwitch(Ref* pSender, Control::EventType event) {
    //获取事件的传递者CCControlSwitch
    ControlSwitch* controlSwitch = (ControlSwitch*)pSender;
    bool isPersonalShow = controlSwitch->isOn();
    _vContentnode.at(0)->setVisible(!isPersonalShow);
    _vContentnode.at(1)->setVisible(isPersonalShow);

    if(isPersonalShow)
    {
        m_btnTitle->setTexture("v88/module/gerenxinxi/zhanghuanquan.png");
    }
    else{
        m_btnTitle->setTexture("v88/module/gerenxinxi/gerenxinxi.png");
    }
}

void GameUserV88::codeCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }

	int tag = (dynamic_cast<AUIButton*>(pSender))->getTag();

    auto invitedLayer = InviteCodeLayerV88::create();
    if (invitedLayer) {
		invitedLayer->setCodeType(tag);
        invitedLayer->open(this->getParent(), Vec2::ZERO, MODULE_LAYER_ORDER_V88 + 5);
    }
}

bool GameUserV88::getInvitedCodeSelector(HNSocketMessage* socketMessage) {
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
