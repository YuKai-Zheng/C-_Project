#include "HNLobby/v66/GameSetLayerV66.h"
#include "PlatformResV66.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/PlatformConfig.h"
#include "VersionInfo/VersionInfo.h"
#include "../pattern/CommandManager.h"

#include "HNLobby/v66/GameAutographV66.h"
#include "HNLobby/v66/GameMusiceEffectV66.h"
#include "HNLobby/globel.h"

//static const int COLOR_LAYER_TAG	= 10000;

//CSB
static const char* SET_SHENMING_CSB =	"csb/v66/setLayer/shenMing_view.csb";
static const char* SET_HELP_CSB =		"csb/v66/setLayer/help_view.csb";
static const char* SET_MODIFY_CSB =		"csb/v66/setLayer/modifyPassWordNode.csb";
static const char* SET_BIND_CSB = "csb/v66/setLayer/bindPhoneNode.csb";
static const char* SET_SAFE_CSB = "csb/v66/setLayer/set_safeBind_v66.csb";


static const char* SET_TITLE = "v66/set_bank/set/set.png";

//button menu
static const char* SET_MENU_BUTTON_NORMAL_BG = "v66/set_bank/button/6.png";
static const char* SET_MENU_BUTTON_SELECT_BG = "v66/set_bank/button/5.png";

static const char* SET_MENU_BUTTON_TITLE0 = "v66/set_bank/button/set/12.png";
static const char* SET_MENU_BUTTON_TITLE1 = "v66/set_bank/button/set/10.png";
static const char* SET_MENU_BUTTON_TITLE2 = "v66/set_bank/button/set/8.png";
static const char* SET_MENU_BUTTON_TITLE3 = "v66/set_bank/button/set/7.png";
static const char* SET_MENU_BUTTON_TITLE4 = "v66/set_bank/button/set/11.png";
static const char* SET_MENU_BUTTON_TITLE5 = "v66/set_bank/button/set/9.png";
static const char* SET_MENU_BUTTON_TITLE6 = "v66/set_bank/button/set/anquan.png";

//title
static const char* SET_BUTTON_GETCODE_TITLE = "v66/button/green/5.png";
static const char* SET_BUTTON_BIND_TITLE = "v66/button/green/8.png";
static const char* SET_BUTTON_UNBIND_TITLE = "v66/button/green/14.png";
static const char* SET_BUTTON_MODIFY_TITLE = "v66/button/green/4.png";
static const char* SET_BUTTON_SAFE_TITLE = "v66/button/green/tijiao.png";

static const char* SET_HELP_ITEM_PATH = "v66/shenming/%s.png";

#define WINSIZE Director::getInstance()->getWinSize()


GameSetLayerV66::GameSetLayerV66() {
	m_isBind = false;
    _oldViewTag = 0;
    _childViewNode = nullptr;
    _isClose = true;
}

GameSetLayerV66::~GameSetLayerV66() {
    _vecButton.clear();
}

bool GameSetLayerV66::init() {
    if ( !HNLayer::init())
        return false;

    auto myListener = EventListenerTouchOneByOne::create();
    myListener->setSwallowTouches(true);//阻止触摸向下传递
    myListener->onTouchBegan = [](Touch *touch, Event *unused_event){
        return true;
    };
    myListener->onTouchEnded = [&](Touch *touch, Event *unused_event){
        removeFromParent();
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(myListener, this);
    
    //setBackGroundImage(PLATFORM_BG_V66);
    
    auto bg = ImageView::create(MODULE_BG_V66);
    //bg->setPosition(Vec2(WIN_SIZE.width / 2, (WIN_SIZE.height - TOP_MENU_HEIGHT) / 2));
    bg->setPosition(WIN_SIZE/2);
    bg->setTouchEnabled(true);
    addChild(bg);
    
    auto bgBottom = Sprite::create();
    bgBottom->setName("BG_BOTTOM");
    bgBottom->setPosition(Vec2(WIN_SIZE.width/2, WIN_SIZE.height/2-241));
    addChild(bgBottom);
    
    auto title = Sprite::create(SET_TITLE);
    title->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height - 20));
    bg->addChild(title);
    

	LoginResult = PlatformLogic()->loginResult;

	// 这个节点做子页面的父节点管理
	_childViewNode = Node::create();
	_childViewNode->setPosition(Vec2(bg->getContentSize() / 2));
	bg->addChild(_childViewNode);

	loadButton();

	//ios 审核
	if (Tools::isiOSReview())
	{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		touchEffect(creatBound_Btn);
		createBoundView(_childViewNode);
		return true;
#endif
	}

//	auto isYouke = UserDefault::getInstance()->getIntegerForKey("IS_LOCAL_LAND");
//
//	if (isYouke)
//	{
//		auto flag = PlatformLogic()->getInstance()->loginResult.iIMEIUserIsBind;
//		if (flag)
//		{
//			auto y1 = _vecButton.at(0)->getPositionY();
//			auto y2 = _vecButton.at(1)->getPositionY();
//			auto offsetY = y1 - y2;
//
//			for (auto btn : _vecButton)
//			{
//				if (getOnly186Try() && btn->getTag() == creatShenMing_Btn) {
//					offsetY += offsetY;
//					continue;
//				}
//				auto btnY = btn->getPositionY();
//				btn->setPositionY(btnY + offsetY);
//			}
//
//			_vecButton.at(0)->setVisible(false);
//
//			createShenMingView(_childViewNode);
//			touchEffect(creatShenMing_Btn);
//		}
//		else
//		{
//			touchEffect(createSafe_Btn);
//			createSafeView(_childViewNode);
//
//		}
//	}
//	else
//	{
//		auto y1 = _vecButton.at(0)->getPositionY();
//		auto y2 = _vecButton.at(1)->getPositionY();
//		auto offsetY = y1 - y2;
//
//		for (auto btn : _vecButton)
//		{
//			if (getOnly186Try() && btn->getTag() == creatShenMing_Btn) {
//				offsetY += offsetY;
//				continue;
//			}
//			auto btnY = btn->getPositionY();
//			btn->setPositionY(btnY + offsetY);
//		}
//
//		_vecButton.at(0)->setVisible(false);
//
//		touchEffect(createSafe_Btn);
//		createShenMingView(_childViewNode);
//	}

    return true;
}

void GameSetLayerV66::open(Node* parent, Vec2 vec, int zorder, int tag) {
	CCAssert(nullptr != parent, "parent is nullptr");
	this->setPosition(vec);
	parent->addChild(this, zorder, tag);
	runAction(MoveBy::create(0.3f, Vec2(0, WIN_SIZE.height)));

	//防止二级界面上打开头像信息
	//UserDefault::getInstance()->setBoolForKey(TOUCH_HEAD, false);
}

void GameSetLayerV66::close() {
	if (_isClose) {
		//HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
		runAction(Sequence::create(MoveBy::create(0.3f, Vec2(0, -WIN_SIZE.height)),
			CallFunc::create([&]() {
			this->removeFromParent();
		}), nullptr));

		_isClose = false;

		UserDefault::getInstance()->setBoolForKey(TOUCH_HEAD, true);
	}
}

// 初始化按钮控件
void GameSetLayerV66::loadButton() {
	vector<const char*> vMenuTitle;
//	vMenuTitle.push_back(SET_MENU_BUTTON_TITLE6);
	vMenuTitle.push_back(SET_MENU_BUTTON_TITLE0);
    vMenuTitle.push_back(SET_MENU_BUTTON_TITLE1);
	vMenuTitle.push_back(SET_MENU_BUTTON_TITLE2);
	vMenuTitle.push_back(SET_MENU_BUTTON_TITLE3);
	//vMenuTitle.push_back(SET_MENU_BUTTON_TITLE4);
	vMenuTitle.push_back(SET_MENU_BUTTON_TITLE5);

	Vec2 startPos = Vec2(WIN_SIZE.width / 2 - 516, (WIN_SIZE.height - TOP_MENU_HEIGHT) / 2 + 160);
	float fInstanceY = 70;

	int menuCount = 0;

	for (int i = 0; i < vMenuTitle.size(); i++)
	{
		if (Tools::isiOSReview())
		{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			//申明帮助
			if (i == 1 || i == 4)
			{
				continue;
			}
#endif
		}

		auto menu = AUIButton::create(SET_MENU_BUTTON_NORMAL_BG, SET_MENU_BUTTON_SELECT_BG, SET_MENU_BUTTON_SELECT_BG);
		menu->setTitle(vMenuTitle.at(i));
		menu->setTitleOffset(Vec2(10, 2));
		menu->setTag(creatShenMing_Btn + i);
		menu->addTouchEventListener(CC_CALLBACK_2(GameSetLayerV66::menuButtonCallBack, this));
		menu->setPosition(Vec2(startPos.x, startPos.y - menuCount*fInstanceY));
		addChild(menu);

		_vecButton.push_back(menu);

		menuCount++;
	}
}

void GameSetLayerV66::menuButtonCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (Widget::TouchEventType::ENDED != type) return;
    auto btn = static_cast<Button*>(pSender);

    //处理点击相同子界面一处重新加载
    if (_oldViewTag == btn->getTag()) {
        return;
    }

    //按钮已经添加了音效，无需重复添加   by HBC
	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);

    _childViewNode->removeAllChildren();
    touchEffect(btn->getTag());

    switch (btn->getTag()) {
//    case createQianming_Btn:
//        createGeXingView(_childViewNode);
//        break;
    case creatGaiMi_Btn:
        createXiuGaiView(_childViewNode);
        break;
    case creatBound_Btn: {
        if (!Tools::isiOSReview()) {
            createBoundView(_childViewNode);
        }
        break;
    }
    case creatMusicEffect_Btn:
        createMusicView(_childViewNode);
        break;
    case creatHelp_Btn: {
        createHelpView(_childViewNode);
        break;
    }
    case creatShenMing_Btn: {
        createShenMingView(_childViewNode);
        break;
    }
//	case createSafe_Btn:{
//		createSafeView(_childViewNode);
//		break;
//	}
    default:
        break;
    }
}

//点击效果
void GameSetLayerV66::touchEffect(UINT tag) {
	for (auto child : _vecButton) {
		child->setTouchEnabled(!(tag == child->getTag()));
		child->setBright(!(tag == child->getTag()));
	}
}


void GameSetLayerV66::handleModifyBtn(Ref* pSender, Widget::TouchEventType type) {
	if (Widget::TouchEventType::ENDED != type)
		return;
	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);

	std::string newPassword = newPwd->getText();
	std::string surePassword = surePwd->getText();
	std::string oldPassword = oldPwd->getText();
	std::string MD5oldPass = MD5_CTX::MD5String(oldPassword).c_str();

	std::string tmp(PlatformLogic()->loginResult.szMD5Pass);

	do {
		if (oldPassword.empty()) {
			CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("请输入旧密码。"));
			break;
		}
		if (newPassword.empty()) {
			CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("请输入新密码。"));
			break;
		}

		if (surePassword.empty()) {
			CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("请输入确认密码。"));
			break;
		}
		if (0 != tmp.compare(MD5oldPass)) {
			CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("旧密码输入错误。"));
			break;
		}
		if (0 != newPassword.compare(surePassword)) {
			CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("新密码和确认密码不相同。"));
			break;
		}

		if (newPassword.length() < 6) {
			CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("新密码长度少于6位"));
			break;
		}
		if (newPassword.compare(oldPassword) == 0) {
			CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("新旧密码一致"));
			break;
		}

		newUserPwd = MD5_CTX::MD5String(newPassword).c_str();

		MSG_GP_S_ChPassword ChPassword;
		ChPassword.dwUserID = PlatformLogic()->loginResult.dwUserID;
		strcpy(ChPassword.szMD5OldPass, PlatformLogic()->loginResult.szMD5Pass);
		strcpy(ChPassword.szMD5NewPass, newUserPwd.c_str());
		PlatformLogic()->sendData(MDM_GP_USERINFO, ASS_GP_USERINFO_UPDATE_PWD, &ChPassword, sizeof(ChPassword));
		PlatformLogic()->addEventSelector(MDM_GP_USERINFO, ASS_GP_USERINFO_ACCEPT, HN_SOCKET_CALLBACK(GameSetLayerV66::modifyPasswordSelector, this));
		PlatformLogic()->addEventSelector(MDM_GP_USERINFO, ASS_GP_USERINFO_NOTACCEPT, HN_SOCKET_CALLBACK(GameSetLayerV66::modifyPasswordSelector, this));
	} while (0);
}

bool GameSetLayerV66::modifyPasswordSelector(HNSocketMessage* socketMessage) {
	if (ASS_GP_USERINFO_ACCEPT == socketMessage->messageHead.bAssistantID) {	// 服务端已接受
		auto userDefault = UserDefault::getInstance();
		userDefault->setStringForKey(PASSWORD_TEXT, newUserPwd);
		strcpy(PlatformLogic()->loginResult.szMD5Pass, newUserPwd.c_str());
		userDefault->flush();
		newUserPwd.clear();
		CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("密码修改成功。"));

		//
		oldPwd->setString("");
		newPwd->setString("");
		surePwd->setString("");
	}
	else if (ASS_GP_USERINFO_NOTACCEPT == socketMessage->messageHead.bAssistantID) {	// 服务端未能接受
		CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("密码修改失败。"));
	}
	else {
	}
	return true;
}

void GameSetLayerV66::handleGetBtn(Ref* pSender, Widget::TouchEventType type) {
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
	request->setResponseCallback(CC_CALLBACK_2(GameSetLayerV66::onHttpCodeResponse, this));

	request->setRequestData(str.c_str(), str.size());
	network::HttpClient::getInstance()->sendImmediate(request);
}

void GameSetLayerV66::updateBindInfo(bool isBind, bool isSuccess)
{
	vertifyCode->setText("");

	if (isBind && isSuccess)
	{
		sendBtn->removeChildByTag(99);
		sendBtn->setTitle(SET_BUTTON_UNBIND_TITLE);
		sendBtn->setTitleOffset(Vec2(0, 4));

		phone->setString(ForceGBKToUtf8(LoginResult.szMobileNo));
		phone->setTouchEnabled(false);
	}

	if (!isBind && isSuccess)
	{
		sendBtn->removeChildByTag(99);
		sendBtn->setTitle(SET_BUTTON_BIND_TITLE);
		sendBtn->setTitleOffset(Vec2(0, 4));

		phone->setTouchEnabled(true);
	}
}

void GameSetLayerV66::handleSendBtn(Ref* pSender, Widget::TouchEventType type) {
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
	request->setResponseCallback(CC_CALLBACK_2(GameSetLayerV66::onHttpBindResponse, this));

	request->setRequestData(str.c_str(), str.size());
	network::HttpClient::getInstance()->sendImmediate(request);

	request->release();
}

void GameSetLayerV66::onHttpCodeResponse(HttpClient* client, HttpResponse* response)
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

void GameSetLayerV66::onHttpBindResponse(network::HttpClient* client, network::HttpResponse* response) {
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

void GameSetLayerV66::createGeXingView(Node* parent) {
	_oldViewTag = createQianming_Btn;

	auto bound = AutoGraphV66::create();
	bound->setPositionX(bound->getPositionX() - 400);
	bound->setPositionY(bound->getPositionY() - 35);
	parent->addChild(bound);
}

void GameSetLayerV66::createXiuGaiView(Node* parent) {
	_oldViewTag = creatGaiMi_Btn;

	Node* changePwdLayer = CSLoader::createNode(SET_MODIFY_CSB);
	parent->addChild(changePwdLayer);

	auto _oldPwd = (TextField*)changePwdLayer->getChildByName("TextField_oldPass");
	_oldPwd->setPlaceHolderColor(FONT_COLOR_ORANGE_V66);
	_oldPwd->setVisible(false);
	oldPwd = HNEditBox::createEditBox(_oldPwd, this);
	oldPwd->setInput(HNEditBox::InputMode::SINGLE_LINE);

	auto _newPwd = (TextField*)changePwdLayer->getChildByName("TextField_newPass");
	_newPwd->setPlaceHolderColor(FONT_COLOR_ORANGE_V66);
	_newPwd->setVisible(false);
	newPwd = HNEditBox::createEditBox(_newPwd, this);
	newPwd->setPasswordEnabled(false);
	newPwd->setInput(HNEditBox::InputMode::SINGLE_LINE);

	auto _surePwd = (TextField*)changePwdLayer->getChildByName("TextField_surePass");
	_surePwd->setPlaceHolderColor(FONT_COLOR_ORANGE_V66);
	_surePwd->setVisible(false);
	surePwd = HNEditBox::createEditBox(_surePwd, this);
	surePwd->setPasswordEnabled(false);
	surePwd->setInput(HNEditBox::InputMode::SINGLE_LINE);

	auto tempModifyBtn = (Button*)changePwdLayer->getChildByName("Button_modify");
	tempModifyBtn->setVisible(false);
	modifyBtn = AUIButton::create(BTN_GREEN_BG_V66, BTN_GREEN_BG_V66);
	modifyBtn->setTitle(SET_BUTTON_MODIFY_TITLE);
	modifyBtn->setTitleOffset(Vec2(0, 4));
	modifyBtn->setPosition(tempModifyBtn->getPosition());
	changePwdLayer->addChild(modifyBtn);
	modifyBtn->addTouchEventListener(CC_CALLBACK_2(GameSetLayerV66::handleModifyBtn, this));
}

void GameSetLayerV66::createBoundView(Node* parent) {
    _oldViewTag = creatBound_Btn;
    
	Node* bindLayer = CSLoader::createNode(SET_BIND_CSB);
	parent->addChild(bindLayer);

	auto _phonenum = (TextField*)bindLayer->getChildByName("TextField_phone");
	_phonenum->setPlaceHolderColor(FONT_COLOR_ORANGE_V66);
	_phonenum->setVisible(false);
	phone = HNEditBox::createEditBox(_phonenum, this);
	phone->setInput(HNEditBox::InputMode::PHONE_NUMBER);

	auto _vertifyCode = (TextField*)bindLayer->getChildByName("TextField_code");
	_vertifyCode->setPlaceHolderColor(FONT_COLOR_ORANGE_V66);
	_vertifyCode->setVisible(false);
	vertifyCode = HNEditBox::createEditBox(_vertifyCode, this);
	vertifyCode->setInput(HNEditBox::InputMode::PHONE_NUMBER);

	auto tempGetBtn = (Button*)bindLayer->getChildByName("Button_getCode");
	tempGetBtn->setVisible(false);
	auto getBtn = AUIButton::create(BTN_GREEN_BG_V66, BTN_GREEN_BG_V66);
	getBtn->setTitle(SET_BUTTON_GETCODE_TITLE);
	getBtn->setTitleOffset(Vec2(0, 4));
	getBtn->setPosition(tempGetBtn->getPosition());
	bindLayer->addChild(getBtn);
	getBtn->addTouchEventListener(CC_CALLBACK_2(GameSetLayerV66::handleGetBtn, this));

	auto tempSendBtn = (Button*)bindLayer->getChildByName("Button_sure");
	tempSendBtn->setVisible(false);
	sendBtn = AUIButton::create(BTN_GREEN_BG_V66, BTN_GREEN_BG_V66);	
	sendBtn->setPosition(tempSendBtn->getPosition());
	bindLayer->addChild(sendBtn);
	sendBtn->addTouchEventListener(CC_CALLBACK_2(GameSetLayerV66::handleSendBtn, this));

	updateBindInfo(LoginResult.iBindMobile, true);
}

void GameSetLayerV66::createMusicView(Node* parent) {
    _oldViewTag = creatMusicEffect_Btn;
    auto music = MusicEffectV66::create();
	music->setPositionX(music->getPositionX()-300);
    parent->addChild(music);
}


void  GameSetLayerV66::createHelpView(Node* parent) {
    _oldViewTag = creatHelp_Btn;

	auto node = CSLoader::createNode(SET_HELP_CSB);
	parent->addChild(node);

	auto listview = dynamic_cast<ListView*>(node->getChildByName("listview"));
	listview->setScrollBarAutoHideEnabled(false);

	//item str
	vector<std::string> vItemStr;
	vItemStr.push_back("jinchan");
	vItemStr.push_back("likui");
	vItemStr.push_back("2renniuniu");
	vItemStr.push_back("4renniuniu1");
	vItemStr.push_back("4renniuniu2");
	vItemStr.push_back("tongbi");
	vItemStr.push_back("bairen");
	vItemStr.push_back("FQZS");
	vItemStr.push_back("shuiguoji");
	vItemStr.push_back("huanle3zhang");
	vItemStr.push_back("huanle5zhang");
	vItemStr.push_back("13");
	vItemStr.push_back("13");
	vItemStr.push_back("huanle30miao");

	//IOS审核版本
	if (Tools::isiOSReview())
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		vItemStr.clear();
		vItemStr.push_back("4renniuniu1");
		vItemStr.push_back("4renniuniu2");
#endif
	}

	//android 审核
	if (GD_NEED_OPEN)
	{
		//vItemStr.clear();
	}

	
	char fileName[64] = { 0 };
	for (int i = 0; i < vItemStr.size(); i++)
	{
		sprintf(fileName, SET_HELP_ITEM_PATH, vItemStr.at(i).c_str());

		auto itemSprite = Sprite::create(fileName);
		itemSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

		auto layout = Layout::create();
		layout->setContentSize(itemSprite->getContentSize());

		layout->addChild(itemSprite);
		listview->addChild(layout);
	}
}

void GameSetLayerV66::createShenMingView(Node* parent) {
    _oldViewTag = creatShenMing_Btn;

	auto node = CSLoader::createNode(SET_SHENMING_CSB);
	parent->addChild(node);

	auto listview = dynamic_cast<ListView*>(node->getChildByName("listview"));
	listview->setScrollBarAutoHideEnabled(false);
}

void GameSetLayerV66::createSafeView(Node* parent)
{
	_oldViewTag = creatShenMing_Btn;
	Node* changePwdLayer = CSLoader::createNode(SET_SAFE_CSB);
	parent->addChild(changePwdLayer);

	auto TextField_account = (TextField*)changePwdLayer->getChildByName("TextField_account");
	TextField_account->setVisible(false);
	saveNewAcc = HNEditBox::createEditBox(TextField_account, this);

	auto TextField_pass = (TextField*)changePwdLayer->getChildByName("TextField_pass");
	TextField_pass->setVisible(false);
	saveNewPass = HNEditBox::createEditBox(TextField_pass, this);

	auto TextField_passSecond = (TextField*)changePwdLayer->getChildByName("TextField_passSecond");
	TextField_passSecond->setVisible(false);
	saveNewPassSure = HNEditBox::createEditBox(TextField_passSecond, this);

	//提交按钮
	auto Button_submit = (Button*)changePwdLayer->getChildByName("Button_submit");
	Button_submit->setVisible(false);
	tijiaoBtn = AUIButton::create(BTN_GREEN_BG_V66, BTN_GREEN_BG_V66);
	tijiaoBtn->setTitle(SET_BUTTON_SAFE_TITLE);
	tijiaoBtn->setTitleOffset(Vec2(0, 4));
	tijiaoBtn->setPosition(Button_submit->getPosition());
	changePwdLayer->addChild(tijiaoBtn);
	tijiaoBtn->addTouchEventListener(CC_CALLBACK_2(GameSetLayerV66::handleSafeBtn, this));

	//底图
	auto ImageView_secondBg = (ImageView*)changePwdLayer->getChildByName("ImageView_secondBg");
	auto ImageView_passBg = (ImageView*)changePwdLayer->getChildByName("ImageView_passBg");
	auto ImageView_accountBg = (ImageView*)changePwdLayer->getChildByName("ImageView_accountBg");

	//title
	auto Sprite_title = (Sprite*)changePwdLayer->getChildByName("Sprite_title");
	auto Sprite_spr = (Sprite*)changePwdLayer->getChildByName("Sprite_spr");
}

void GameSetLayerV66::handleSafeBtn(Ref* pSender, Widget::TouchEventType type)
{
	if (Widget::TouchEventType::ENDED != type)
		return;

	do {
		// 获取输入框账号
		std::string userName = saveNewAcc->getString();

		if (userName.empty()) {
			CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("账号不能为空！"));
			break;
		}

		if (saveNewAcc->getStringLength() < 6 ||
			saveNewAcc->getStringLength() > 15 ||
			Tools::verifyChinese(userName)) {
			CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("请输入6-15位英文或数字帐号！"));
			break;
		}

		// 获取输入框密码
		std::string passWord = saveNewPass->getString();
		if (passWord.empty()) {
			CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("密码不能为空！"));
			break;
		}

		if (saveNewPass->getStringLength() < 6 ||
			saveNewPass->getStringLength() > 15 ||
			Tools::verifyChinese(passWord)) {
			CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("请输入6-15位字符密码！"));
			break;
		}

		//获取确认密码
		std::string checkPassWord = saveNewPassSure->getString();
		if (checkPassWord.empty()) {
			CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("确认密码不能为空！"));
			break;
		}

		if (passWord != checkPassWord) {
			CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("密码不一致，请检查！"));
			break;
		}

		if (m_isBind == true)
		{
			CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("已经绑定账号,不能重复绑定!"));
			break;
		}

		MSG_GP_S_IMEI_Bind data;
		strcpy(data.szMD5Pass, MD5_CTX::MD5String(checkPassWord).c_str());
		strcpy(data.szName, userName.c_str());
		auto id = PlatformLogic()->getInstance()->loginResult.dwUserID;
		data.UserID = id;
		PlatformLogic()->sendData(MDM_GP_IMEI_Bind, ASS_GP_IMEI_Bind, &data, sizeof(data), HN_SOCKET_CALLBACK(GameSetLayerV66::safeMessageCallBack, this));
	} while (0);
}

bool GameSetLayerV66::safeMessageCallBack(HNSocketMessage* socketMessage) {
	UINT bAssistantID = socketMessage->messageHead.bAssistantID;
	UINT bHandleCode = socketMessage->messageHead.bHandleCode;

	MSG_GP_S_IMEI_Bind* data = (MSG_GP_S_IMEI_Bind*)(socketMessage->object);
	if (bHandleCode == 0)
	{
		CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("绑定新账号失败!"));
		return false;
	}
	else if (bHandleCode == 1)
	{
		CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("绑定新账号成功!金币已到账!"));

		UserDefault::getInstance()->setIntegerForKey("IS_LOCAL_LAND", false);
		UserDefault::getInstance()->setIntegerForKey("IS_LOCAL_LAND_NEW_ACCBIND", true);
		m_isBind = true;
		return true;
	}
	else if (bHandleCode == 2)
	{
		CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("已经绑定账号,不能重复绑定!"));
		return false;
	}
	return false;
}


