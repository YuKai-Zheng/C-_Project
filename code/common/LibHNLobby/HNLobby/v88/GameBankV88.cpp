#include "GameBankV88.h"
#include "PlatformResV88.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/PlatformConfig.h"
#include "HNLobby/FontConfig.h"
#include "HNMarketExport.h"
#include <string>
#include "../BankGivePrompt.h"
#include "Sound/HNAudioEngine.h"
#include "../globel.h"
#include "HNCommon/HNConverCode.h"
#include "../pattern/CommandManager.h"

//子界面csb
static const char* BANKER_SAVE_CSB = "csb/v88/bankLayer/save_node.csb";
static const char* BANKER_GIVE_CSB = "csb/v88/bankLayer/give_node.csb";
static const char* BANKER_MODIFICATION_CSB = "csb/v88/bankLayer/modification_node.csb";
static const char* BANKER_RECORD_CSB = "csb/v88/bankLayer/record_node.csb";
static const char* BANKER_RECORD_TEXT_CSB = "csb/v88/bankLayer/record_text.csb";

static const char* BANKER_TITLE = "v88/module/bank/bank.png";

//button menu
static const char* BANK_MENU_BUTTON_NORMAL_BG0 = "v88/module/button/1.png";
static const char* BANK_MENU_BUTTON_NORMAL_BG1 = "v88/module/button/2.png";
static const char* BANK_MENU_BUTTON_NORMAL_BG2 = "v88/module/button/3.png";
static const char* BANK_MENU_BUTTON_SELECT_BG0 = "v88/module/button/5.png";
static const char* BANK_MENU_BUTTON_SELECT_BG1 = "v88/module/button/6.png";
static const char* BANK_MENU_BUTTON_SELECT_BG2 = "v88/module/button/4.png";
static const char* BANK_MENU_BUTTON_DISABLED_BG0 = "v88/module/button/5.png";
static const char* BANK_MENU_BUTTON_DISABLED_BG1 = "v88/module/button/6.png";
static const char* BANK_MENU_BUTTON_DISABLED_BG2 = "v88/module/button/4.png";

static const char* BANK_MENU_BUTTON_TITLE0 = "v88/module/button/bank/14.png";
static const char* BANK_MENU_BUTTON_TITLE1 = "v88/module/button/bank/15.png";
static const char* BANK_MENU_BUTTON_TITLE2 = "v88/module/button/bank/16.png";
static const char* BANK_MENU_BUTTON_TITLE3 = "v88/module/button/bank/13.png";
static const char* BANK_MENU_BUTTON_NORMAL_TITLE0 = "v88/module/button/bank/2.png";
static const char* BANK_MENU_BUTTON_NORMAL_TITLE1 = "v88/module/button/bank/3.png";
static const char* BANK_MENU_BUTTON_NORMAL_TITLE2 = "v88/module/button/bank/4.png";
static const char* BANK_MENU_BUTTON_NORMAL_TITLE3 = "v88/module/button/bank/1.png";
//title
static const char*	BANK_BUTTON_TAKE_TITLE = "v88/button/yellow1/quchu.png";
static const char*	BANK_BUTTON_SAVE_TITLE = "v88/button/yellow1/cunru.png";
static const char*	BANK_BUTTON_GIVE_TITLE = "v88/button/yellow1/zengsong.png";
static const char*	BANK_BUTTON_MODIFY_TITLE = "v88/button/yellow1/xiugai.png";

static const int	BANK_PASSWORD_LIMIT_MIN_LENGTH = 6;

#define WINSIZE Director::getInstance()->getWinSize()
#define JSON_RESOVE(obj, key) (obj.HasMember(key) && !obj[key].IsNull())

GameBankV88::GameBankV88()
:_bankMoney(0)
, _walletMoney(0)
, _childViewNode(nullptr)
, _isClose(true)
, _isOpenSaveView(true)
, _idxJIlu(1)
, _curpage(0)
, _toutleRecord(0) {

	_vecButton.clear();
	_vecRecord.clear();

	_giveView.oldGiveId = 0;
	_giveView.oldGiveMoney = 0;
	_time = 61;

	memset(&_giveView, 0x0, sizeof(_giveView));
	memset(&_modifocationView, 0x0, sizeof(_modifocationView));
	memset(&_recordView, 0x0, sizeof(_recordView));
}

GameBankV88::~GameBankV88() {
	_vecButton.clear();
	_vecRecord.clear();
	PlatformLogic()->removeEventSelector(MDM_GP_BANK, ASS_GP_BANK_OPEN);
	PlatformLogic()->removeEventSelector(MDM_GP_BANK, ASS_GP_BANK_CHPWD);
	PlatformLogic()->removeEventSelector(MDM_GP_BANK, ASS_GP_BANK_CHECK);
	PlatformLogic()->removeEventSelector(MDM_GP_BANK, ASS_GP_BANK_TRANSFER);
}

bool GameBankV88::init() {
	if (!HNLayer::init()) {
		return false;
	}

	auto myListener = EventListenerTouchOneByOne::create();
	myListener->setSwallowTouches(true);//阻止触摸向下传递
	myListener->onTouchBegan = [](Touch *touch, Event *unused_event){
		return true;
	};
    myListener->onTouchEnded = [&](Touch *touch, Event *unused_event){
        close();
    };
	_eventDispatcher->addEventListenerWithSceneGraphPriority(myListener, this);

	auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 100));
	addChild(colorLayer);
	//setBackGroundImage(PLATFORM_BG_V88);

 	auto bg = ImageView::create(MODULE_BG_V88);
 	//bg->setPosition(Vec2(WIN_SIZE.width / 2, (WIN_SIZE.height - TOP_MENU_HEIGHT) / 2));
    bg->setPosition(WIN_SIZE/2);
    bg->setTouchEnabled(true);
 	addChild(bg);
    
    auto bgBottom = Sprite::create("v88/common/tanchuang/bg1.png");
    bgBottom->setName("BG_BOTTOM");
    bgBottom->setPosition(Vec2(WIN_SIZE.width/2, WIN_SIZE.height/2-241));
    addChild(bgBottom);
    
 	auto title = Sprite::create(BANKER_TITLE);
 	title->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height - 44));
 	bg->addChild(title);
 
 	// 这个节点做子页面的父节点管理
 	_childViewNode = Node::create();
 	_childViewNode->setPosition(bg->getPosition());
 	addChild(_childViewNode);

	loadButton(bg);

	//默认子界面
	addDefaultView();

	//打开银行
	//要加个loading
	PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_OPEN, nullptr, 0, HN_SOCKET_CALLBACK(GameBankV88::bankerEventSelector, this));
	return true;

}

void GameBankV88::open(Node* parent, Vec2 vec, int zorder, int tag) {
    CCAssert(nullptr != parent, "parent is nullptr");
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setPosition(vec);
    parent->addChild(this, zorder, tag);
    
    //setScale(0.2);
    //runAction(ScaleTo::create(0.2f, 1.0f));
}

void GameBankV88::close() {
    //runAction(Sequence::create(ScaleTo::create(0.2f, 0.2f), RemoveSelf::create(), nullptr));
	removeFromParent();
}

// 初始化按钮控件
void GameBankV88::loadButton(ImageView* parent) {
	_vMenuTitle.clear();
	_vMenuTitle.push_back(BANK_MENU_BUTTON_NORMAL_TITLE0);
	_vMenuTitle.push_back(BANK_MENU_BUTTON_NORMAL_TITLE1);
	_vMenuTitle.push_back(BANK_MENU_BUTTON_NORMAL_TITLE2);
	_vMenuTitle.push_back(BANK_MENU_BUTTON_NORMAL_TITLE3);

	_vMenuTitle.push_back(BANK_MENU_BUTTON_TITLE0);
	_vMenuTitle.push_back(BANK_MENU_BUTTON_TITLE1);
	_vMenuTitle.push_back(BANK_MENU_BUTTON_TITLE2);
	_vMenuTitle.push_back(BANK_MENU_BUTTON_TITLE3);
    
    vector<const char*> vMenuImg;
    vMenuImg.push_back(BANK_MENU_BUTTON_NORMAL_BG0);
    vMenuImg.push_back(BANK_MENU_BUTTON_NORMAL_BG1);
    vMenuImg.push_back(BANK_MENU_BUTTON_NORMAL_BG1);
    vMenuImg.push_back(BANK_MENU_BUTTON_NORMAL_BG2);
    
    vMenuImg.push_back(BANK_MENU_BUTTON_SELECT_BG0);
    vMenuImg.push_back(BANK_MENU_BUTTON_SELECT_BG1);
    vMenuImg.push_back(BANK_MENU_BUTTON_SELECT_BG1);
    vMenuImg.push_back(BANK_MENU_BUTTON_SELECT_BG2);

	vMenuImg.push_back(BANK_MENU_BUTTON_DISABLED_BG0);
	vMenuImg.push_back(BANK_MENU_BUTTON_DISABLED_BG1);
	vMenuImg.push_back(BANK_MENU_BUTTON_DISABLED_BG1);
	vMenuImg.push_back(BANK_MENU_BUTTON_DISABLED_BG2);
    
    Size parentSize = parent->getContentSize();
    float fInstanceX = 198;
	Vec2 startPos = Vec2(parentSize.width/2 - fInstanceX*1.5, parentSize.height/2 + 170);

	for (int i = 0; i < 4; i++)
	{
		auto menu = AUIButton::create(vMenuImg.at(i), vMenuImg.at(i + 4), vMenuImg.at(i + 8));
        menu->setPressedActionEnabled(false);
		menu->setTitle(_vMenuTitle.at(i));
		//menu->setTitleOffset(Vec2(10, 2));
		menu->setTag(btn_save + i);
		menu->addTouchEventListener(CC_CALLBACK_2(GameBankV88::menuButtonCallBack, this));
		menu->setPosition(Vec2(startPos.x + fInstanceX*i, startPos.y));
		parent->addChild(menu);

		_vecButton.push_back(menu);
	}
}

//创建子界面
void GameBankV88::menuButtonCallBack(Ref* pSender, Widget::TouchEventType touchtype) {
	if (touchtype != Widget::TouchEventType::ENDED)		return;
	auto btn = (Button*)pSender;
	//处理点击相同子界面一处重新加载
	if (_oldViewTag == btn->getTag()) {
		return;
	}
	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
	_curpage = 0;
	_toutleRecord = 0;
	_vecRecord.clear();
	_childViewNode->removeAllChildren();
	clearCurViewDat();
	touchEffect(btn->getTag());

	switch (btn->getTag()) {
	case btn_save: {
					   createSaveView(_childViewNode);
					   break;
	}
	case btn_give: {
					   createGiveView(_childViewNode);
					   _isOpenSaveView = false;
					   break;
	}
	case btn_record: {
						 createRecordView(_childViewNode);
						 _isOpenSaveView = false;
						 break;
	}
	case btn_modification: {
							   createModificationView(_childViewNode);
							   _isOpenSaveView = false;
							   break;
	}
	default:
		break;
	}
}

//点击效果
void GameBankV88::touchEffect(UINT tag) {
	for (auto child : _vecButton) {
		child->setTouchEnabled(!(tag == child->getTag()));
		child->setBright(!(tag == child->getTag()));

		int nIndex = child->getTag() - 1000;
		string btnTitle = (tag == child->getTag()) ? _vMenuTitle.at(nIndex + 4) : _vMenuTitle.at(nIndex);
		AUIButton* btn = (AUIButton*)child;
		if (btn->getChildByTag(99))
		{
			btn->getChildByTag(99)->removeFromParent();
		}
		btn->setTitle(btnTitle);
	}
}

//进入银行默认子页
void GameBankV88::addDefaultView() {
	_oldViewTag = btn_save;
	touchEffect(btn_save);
	createSaveView(_childViewNode);
}

void GameBankV88::reqUserNameById(std::string id) {
	std::string URL = std::string("http://") + getWebServerUrl() + std::string("/Public/AuditConfig.ashx?");
	std::string str("action=GetUserNameByUserID&key=");
	str = str + id;

	HttpRequest* request = new (std::nothrow) HttpRequest();
	request->setUrl(URL.c_str());
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(GameBankV88::setTargetUserNameById, this));
	request->setRequestData(str.c_str(), str.size());
	HttpClient::getInstance()->sendImmediate(request);

	request->release();
}

void GameBankV88::setTargetUserNameById(network::HttpClient* client, network::HttpResponse* response) {
	std::vector<char> recv = *response->getResponseData();
	std::string data;
	data.assign(recv.begin(), recv.end());

	rapidjson::Document doc;
	doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
	if (doc.HasParseError() || !doc.IsObject() || !doc.HasMember("content") || !doc["content"].IsArray()) {
		if (_giveView.targetName) {
			_giveView.targetName->setString("");
		}
		// 处理无当前用户
		return;
	}

	// 解析
	for (auto i = 0; i < doc["content"].Size(); i++) {
		if (!doc["content"][i].HasMember("NickName")) {
			continue;
		}
		std::string name = doc["content"][i]["NickName"].GetString();
		if (_giveView.targetName) {
			_giveView.targetName->setString(name); //通过ID获取姓名 更新 文本内容
		}

	}
}

//存取
void GameBankV88::createSaveView(Node* parent) {
	auto root = CSLoader::createNode(BANKER_SAVE_CSB);
	parent->addChild(root);
    
    getChildByName("BG_BOTTOM")->setVisible(true);
    
	_oldViewTag = btn_save;
	_saveView.isAllSaveTouch = false;
	_saveView.isAllTakeTouch = false;

	_saveView.bagMoneyText = dynamic_cast<Text*>(root->getChildByName("Text_bagMoney"));
	_saveView.bagMoneyText->setColor(FONT_CLOLOR_YELLOW_V88);

	_saveView.bankMoneyText = dynamic_cast<Text*>(root->getChildByName("Text_bankMoney"));
	_saveView.bankMoneyText->setColor(FONT_CLOLOR_YELLOW_V88);

	auto moneyField = dynamic_cast<TextField*>(root->getChildByName("TextField_money"));
	moneyField->setVisible(false);
    moneyField->setColor(FONT_CLOLOR_YELLOW_V88);
	_saveView.moneyField = HNEditBox::createEditBox(moneyField, this);
	_saveView.moneyField->setInput(HNEditBox::InputMode::PHONE_NUMBER);

	auto mima = dynamic_cast<TextField*>(root->getChildByName("TextField_password"));
	mima->setPlaceHolder(GBKToUtf8("初始密码为123456"));
	mima->setVisible(false);
    mima->setColor(FONT_CLOLOR_YELLOW_V88);
	_saveView.mima = HNEditBox::createEditBox(mima, this);
	_saveView.mima->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
	_saveView.mima->setInput(HNEditBox::InputMode::SINGLE_LINE);
	//登陆第一次交易成功时记录输入的交易密码 key值为 "bankTransactionPassword"
	std::string localMima = UserDefault::getInstance()->getStringForKey("bankTransactionPassword");
	if (localMima != "") {
		_saveView.mima->setString(localMima);
	}

	_saveView.saveWordCheckBox = dynamic_cast<CheckBox*>(root->getChildByName("CheckBox_saveWord"));
	_saveView.saveWordCheckBox->setTag(save_word);
	_saveView.saveWordCheckBox->addEventListener(CC_CALLBACK_2(GameBankV88::checkBoxCallBack, this));
	_saveView.saveImageCheckBox = dynamic_cast<CheckBox*>(root->getChildByName("CheckBox_saveImage"));
	_saveView.saveImageCheckBox->setTag(save_image);
	_saveView.saveImageCheckBox->addEventListener(CC_CALLBACK_2(GameBankV88::checkBoxCallBack, this));

	_saveView.takeWordCheckBox = dynamic_cast<CheckBox*>(root->getChildByName("CheckBox_takeWord"));
	_saveView.takeWordCheckBox->setTag(take_word);
	_saveView.takeWordCheckBox->addEventListener(CC_CALLBACK_2(GameBankV88::checkBoxCallBack, this));
	_saveView.takeImageCheckBox = dynamic_cast<CheckBox*>(root->getChildByName("CheckBox_takeImage"));
	_saveView.takeImageCheckBox->setTag(take_iamge);
	_saveView.takeImageCheckBox->addEventListener(CC_CALLBACK_2(GameBankV88::checkBoxCallBack, this));

	auto saveBtn = dynamic_cast<Button*>(root->getChildByName("Button_save"));
	saveBtn->setVisible(false);
	_saveView.saveButton = AUIButton::create(BTN_YELLOW1_NORMAL_BG_V88, BTN_YELLOW1_SELECT_BG_V88, BTN_YELLOW1_ENABLED_BG_V88);
	_saveView.saveButton->setTitle(BANK_BUTTON_SAVE_TITLE);
	//_saveView.saveButton->setTitleOffset(Vec2(0, 4));
	_saveView.saveButton->setPosition(saveBtn->getPosition());
	root->addChild(_saveView.saveButton);
	_saveView.saveButton->setTag(btn_cun);
	_saveView.saveButton->addTouchEventListener(CC_CALLBACK_2(GameBankV88::saveTakeMoneyCallBack, this));

	auto takeBtn = dynamic_cast<Button*>(root->getChildByName("Button_take"));
	takeBtn->setVisible(false);
	_saveView.takeButton = AUIButton::create(BTN_YELLOW1_NORMAL_BG_V88, BTN_YELLOW1_SELECT_BG_V88, BTN_YELLOW1_ENABLED_BG_V88);
	_saveView.takeButton->setTitle(BANK_BUTTON_TAKE_TITLE);
	//_saveView.takeButton->setTitleOffset(Vec2(0, 4));
	_saveView.takeButton->setPosition(takeBtn->getPosition());
	root->addChild(_saveView.takeButton);
	_saveView.takeButton->setTag(btn_take);
	_saveView.takeButton->addTouchEventListener(CC_CALLBACK_2(GameBankV88::saveTakeMoneyCallBack, this));

	PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_OPEN, nullptr, 0, HN_SOCKET_CALLBACK(GameBankV88::bankerEventSelector, this));
	_isOpenSaveView = true;
}

void GameBankV88::checkBoxCallBack(Ref* sender, CheckBox::EventType type) {
	auto checkBox = dynamic_cast<CheckBox*>(sender);
	int tagType = checkBox->getTag();

	switch (type) {
	case CheckBox::EventType::SELECTED:
		if (tagType == save_word || tagType == save_image)
		{
			_saveView.moneyField->setString(to_string(PlatformLogic()->loginResult.i64Money));

			_saveView.saveWordCheckBox->setSelected(true);
			_saveView.saveImageCheckBox->setSelected(true);

			_saveView.takeWordCheckBox->setSelected(false);
			_saveView.takeImageCheckBox->setSelected(false);

			_saveView.saveButton->setTouchEnabled(true);
			_saveView.saveButton->setBright(true);
			_saveView.takeButton->setTouchEnabled(false);
			_saveView.takeButton->setBright(false);

			_saveView.isAllTakeTouch = false;
			_saveView.isAllSaveTouch = true;
		}
		else
		{
			_saveView.moneyField->setString(to_string(PlatformLogic()->loginResult.i64Bank));

			_saveView.saveWordCheckBox->setSelected(false);
			_saveView.saveImageCheckBox->setSelected(false);

			_saveView.takeWordCheckBox->setSelected(true);
			_saveView.takeImageCheckBox->setSelected(true);

			_saveView.saveButton->setTouchEnabled(false);
			_saveView.saveButton->setBright(false);
			_saveView.takeButton->setTouchEnabled(true);
			_saveView.takeButton->setBright(true);

			_saveView.isAllTakeTouch = true;
			_saveView.isAllSaveTouch = false;
		}
		break;
	case CheckBox::EventType::UNSELECTED:
		_saveView.moneyField->setString("");
		if (tagType == save_word || tagType == save_image)
		{
			_saveView.saveWordCheckBox->setSelected(false);
			_saveView.saveImageCheckBox->setSelected(false);
			_saveView.takeButton->setTouchEnabled(true);
			_saveView.takeButton->setBright(true);
			_saveView.isAllSaveTouch = false;
		}
		else
		{
			_saveView.takeWordCheckBox->setSelected(false);
			_saveView.takeImageCheckBox->setSelected(false);
			_saveView.saveButton->setTouchEnabled(true);
			_saveView.saveButton->setBright(true);
			_saveView.isAllTakeTouch = false;
		}
		break;
	default:
		break;
	}
}

void GameBankV88::saveTakeMoneyCallBack(Ref* pSender, Widget::TouchEventType type) {
	if (Widget::TouchEventType::ENDED != type) return;
	Button* psender = (Button*)pSender;

	if (psender == _saveView.saveButton || psender == _saveView.takeButton) {
		HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
		// 银行打开失败，不做任何处理
		if (!_bankOpen) return;
		// 存款
		if (psender->getTag() == btn_cun) {
			std::string strMoney = _saveView.moneyField->getString();
			do {
				if (strMoney.empty()) {
					GamePromptLayer::create()->showPrompt(GetText("TIPS14"));
					break;
				}

				TMSG_GP_BankCheck BankCheck = { 0 };
				BankCheck.operate_type = 2;
				BankCheck.user_id = PlatformLogic()->loginResult.dwUserID;
				BankCheck.game_id = 0;
				BankCheck.money = atoll(strMoney.c_str());
				if (BankCheck.money <= 0) {
					GamePromptLayer::create()->showPrompt(GetText("TIPS11"));
					break;
				}
				PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_CHECK, &BankCheck, sizeof(BankCheck),
					HN_SOCKET_CALLBACK(GameBankV88::bankerEventSelector, this));
			} while (0);
		}
		// 取款
		else if (psender->getTag() == btn_take) {
			std::string strMoney = _saveView.moneyField->getString();
			std::string password = _saveView.mima->getString();
			do {
				if (strMoney.empty()) {
					GamePromptLayer::create()->showPrompt(GetText("TIPS12"));
					break;
				}

				if (password.empty()) {
					GamePromptLayer::create()->showPrompt(GetText("TIPS9"));
					break;
				}
				TMSG_GP_BankCheck BankCheck = { 0 };
				BankCheck.operate_type = 1;
				BankCheck.user_id = PlatformLogic()->loginResult.dwUserID;
				BankCheck.game_id = 0;
				BankCheck.money = atoll(strMoney.c_str());
				strcpy(BankCheck.szMD5Pass, MD5_CTX::MD5String(password).c_str());
				if (BankCheck.money <= 0) {
					GamePromptLayer::create()->showPrompt(GetText("TIPS11"));
					break;
				}
				PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_CHECK, &BankCheck, sizeof(BankCheck),
					HN_SOCKET_CALLBACK(GameBankV88::bankerEventSelector, this));
			} while (0);
		}
		else {
			GamePromptLayer::create()->showPrompt(GetText("TIPS13"));
		}
	}
}

//赠送金币子界面
void GameBankV88::createGiveView(Node* parent) {
	_oldViewTag = btn_give;

	auto root = CSLoader::createNode(BANKER_GIVE_CSB);
	parent->addChild(root);

    getChildByName("BG_BOTTOM")->setVisible(true);
    
	_giveView.targetName = dynamic_cast<Text*>(root->getChildByName("Text_targetName"));
	_giveView.targetName->setColor(FONT_CLOLOR_YELLOW_V88);
	_giveView.bankMoney = dynamic_cast<Text*>(root->getChildByName("Text_bankMoney"));
	_giveView.bankMoney->setColor(FONT_CLOLOR_YELLOW_V88);
	_giveView.bankMoney->setString(Tools::fourSeparateMoney(PlatformLogic()->getInstance()->loginResult.i64Bank));

	auto giveBtn = dynamic_cast<Button*>(root->getChildByName("Button_give"));
	giveBtn->setVisible(false);
	_giveView.giveBtn = AUIButton::create(BTN_YELLOW1_NORMAL_BG_V88, BTN_YELLOW1_SELECT_BG_V88, BTN_YELLOW1_ENABLED_BG_V88);
	_giveView.giveBtn->setTitle(BANK_BUTTON_GIVE_TITLE);
	//_giveView.giveBtn->setTitleOffset(Vec2(0, 4));
	_giveView.giveBtn->setPosition(giveBtn->getPosition());
	root->addChild(_giveView.giveBtn);
	_giveView.giveBtn->addTouchEventListener(CC_CALLBACK_2(GameBankV88::giveMoneyCallBack, this));

	auto password = dynamic_cast<TextField*>(root->getChildByName("TextField_password"));
	password->setPlaceHolder(GBKToUtf8("初始密码为123456"));
	password->setPlaceHolderColor(FONT_CLOLOR_YELLOW_V88);
	password->setVisible(false);
	_giveView.mima = HNEditBox::createEditBox(password, this);
	_giveView.mima->setInput(HNEditBox::InputMode::SINGLE_LINE);
	_giveView.mima->setInputFlag(ui::EditBox::InputFlag::PASSWORD);

	auto giveMoney = dynamic_cast<TextField*>(root->getChildByName("TextField_money"));
	giveMoney->setPlaceHolderColor(FONT_CLOLOR_YELLOW_V88);
	giveMoney->setVisible(false);
	_giveView.giveMoney = HNEditBox::createEditBox(giveMoney, this);
	_giveView.giveMoney->setInput(ui::EditBox::InputMode::PHONE_NUMBER);
	_giveView.giveMoney->setTag(give_money);
	editBoxEditingDidEnd(_giveView.giveMoney);

	auto userId = dynamic_cast<TextField*>(root->getChildByName("TextField_id"));
	userId->setPlaceHolderColor(FONT_CLOLOR_YELLOW_V88);
	userId->setVisible(false);
	_giveView.userId = HNEditBox::createEditBox(userId, this);
	_giveView.userId->setInput(ui::EditBox::InputMode::PHONE_NUMBER);
	_giveView.userId->setTag(give_userId);

	//是否有转账 有 读取上一次记录ID 没有默认
	auto lastGiveId = UserDefault::getInstance()->getStringForKey("BANK_LASTGIVE_MONEY_USERID");
	auto lastUserid = UserDefault::getInstance()->getStringForKey("BANK_LAST_USERID");
	auto curUserId = UserDefault::getInstance()->getStringForKey(USERNAME_TEXT);
	if (lastGiveId == "")
	{
		;
	}
	else
	{
		if (lastUserid == curUserId)
			_giveView.userId->setString(lastGiveId);
	}
	UserDefault::getInstance()->setStringForKey("BANK_LAST_USERID", curUserId);

	editBoxEditingDidEnd(_giveView.userId);

	_walletMoney = PlatformLogic()->getInstance()->loginResult.i64Bank;
}

void GameBankV88::giveMoneyCallBack(Ref* pSender, Widget::TouchEventType type) {

	if (Widget::TouchEventType::ENDED != type) return;
	Button* psender = (Button*)pSender;
	if (psender == _giveView.giveBtn) {
		HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
		std::string userText = _giveView.userId->getString();
		std::string forwardMoney = _giveView.giveMoney->getString();
		std::string bankPassword = _giveView.mima->getString();

		//userText = "123456";
		//forwardMoney = "500000";
		//bankPassword = "123456";

		do {
			if (atoll(userText.c_str()) == PlatformLogic()->loginResult.dwUserID) {
				GamePromptLayer::create()->showPrompt(GetText("TIPS5"));
				break;
			}
			if (userText.empty()) {
				if (_giveView.userId->isEnabled()) {
					GamePromptLayer::create()->showPrompt(GetText("TIPS6"));
					break;
				}

// 				if (_giveView.userId->isEnabled()) {
// 					GamePromptLayer::create()->showPrompt(GetText("TIPS7"));
// 					break;
// 				}
			}

			if (forwardMoney.empty()) {
				GamePromptLayer::create()->showPrompt(GetText("TIPS8"));
				break;
			}

			if (bankPassword.empty()) {
				GamePromptLayer::create()->showPrompt(GetText("TIPS9"));
				break;
			}

			std::string str;
			str.append(GetText("TIPS25"));
			str.append(_giveView.targetName->getString().c_str());
			str.append(GBKToUtf8("赠送"));
			str.append(forwardMoney.c_str());
			str.append(GBKToUtf8("金币吗?"));

			HNLOG_INFO("-----------%s", __String::createWithFormat(GBKToUtf8("你确定为%s赠送%s金币吗?"), _giveView.targetName->getString().c_str(), forwardMoney.c_str())->getCString());
			//GBKToUtf8(StringUtils::format("你确定为%s赠送%s金币吗?", _giveView.targetName->getString().c_str(), forwardMoney.c_str()).c_str());

			//std::string str = GetText("GIVE_MONEY_SURE");
			HNLOG_INFO("<<<<<<<<<<%s", str.c_str());
			auto tishi = GamePromptLayer::create();
			tishi->showPrompt(str);
			tishi->setOkayCallBack([&]() {

				if (_giveView.oldGiveId == atoi(_giveView.userId->getString().c_str()) && atoi(_giveView.giveMoney->getString().c_str()) == _giveView.oldGiveMoney && _time <= 60) {
					GamePromptLayer::create()->showPrompt(GetText("TIPS10"));
					return;
				}
				else {
					MSG_GP_R_LogonResult& LogonResult = PlatformLogic()->loginResult;
					TMSG_GP_BankTransfer bankTransfer;
					if (_giveView.userId->isEnabled()) {
						bankTransfer.UserID = LogonResult.dwUserID;
						strcpy(bankTransfer.szNickName, LogonResult.nickName);

						bankTransfer.destUserID = atol(_giveView.userId->getString().c_str());
						bankTransfer.i64Money = atoll(_giveView.giveMoney->getString().c_str());
						bankTransfer.bUseDestID = true;
						strcpy(bankTransfer.szDestNickName, Utf8ToGB(_giveView.targetName->getString().c_str()));
						strcpy(bankTransfer.szMD5Pass, MD5_CTX::MD5String(_giveView.mima->getString()).c_str());
					}
					_walletMoney -= bankTransfer.i64Money;
					if (bankTransfer.i64Money <= 0) {
						GamePromptLayer::create()->showPrompt(GetText("TIPS11"));
						return;
					}
					CommandManager::getInstance()->addLoaderCommand("");
					PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_TRANSFER, &bankTransfer, sizeof(bankTransfer),
						HN_SOCKET_CALLBACK(GameBankV88::bankerForwardSelector, this));
				}
				//if (_giveView.userId->isEnabled())
				//{
				//	bankTransfer.UserID = LogonResult.dwUserID;
				//	strcpy(bankTransfer.szNickName, LogonResult.nickName);

				//	strcpy(bankTransfer.szDestNickName, userText.c_str());
				//	bankTransfer.i64Money = atol(forwardMoney.c_str());
				//	bankTransfer.bUseDestID = false;
				//	strcpy(bankTransfer.szMD5Pass, MD5_CTX::MD5String(bankPassword).c_str());
				//}
			});

		} while (0);
	}
}

//赠送记录查看
void GameBankV88::createRecordView(Node* parent) {
	_idxJIlu = 1;
	_oldViewTag = btn_record;

	auto root = CSLoader::createNode(BANKER_RECORD_CSB);
	_childViewNode->addChild(root);

    getChildByName("BG_BOTTOM")->setVisible(false);
    
	_recordView.listView = dynamic_cast<ListView*>(root->getChildByName("ListView"));
	_recordView.listView->setScrollBarAutoHideEnabled(false);
	_recordView.listView->addTouchEventListener(CC_CALLBACK_2(GameBankV88::recordlistViewCallBack, this));

	////////////
	TMSG_GP_BankTranRecordReq data;
	data.dwUserID = PlatformLogic()->loginResult.dwUserID;
	data.pageSize = 10;
	data.curPage = _idxJIlu;
	data.reqType = 0;
	PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_TRANS_RECORD, &data, sizeof(data), HN_SOCKET_CALLBACK(GameBankV88::bankerForwardSelector, this));
	/////////////
	CommandManager::getInstance()->addLoaderCommand("");
}

void GameBankV88::recordlistViewCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
	switch (type) {

	case cocos2d::ui::Widget::TouchEventType::ENDED:
	case cocos2d::ui::Widget::TouchEventType::CANCELED: {
															if (_recordView.listView->getInnerContainer()->getPositionY() > 5) {
																if (_toutleRecord == _vecRecord.size())
																	return;

																TMSG_GP_BankTranRecordReq data;
																data.dwUserID = PlatformLogic()->loginResult.dwUserID;
																data.pageSize = 10;
																data.curPage = _idxJIlu;
																data.reqType = 0;
																PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_TRANS_RECORD, &data, sizeof(data), HN_SOCKET_CALLBACK(GameBankV88::bankerForwardSelector, this));
																CommandManager::getInstance()->addLoaderCommand("");
															}
	}
		break;
	default:
		break;
	}
}

//修改密码子界面
void GameBankV88::createModificationView(Node* parent) {
	_oldViewTag = btn_modification;
	auto root = CSLoader::createNode(BANKER_MODIFICATION_CSB);
	parent->addChild(root);

    getChildByName("BG_BOTTOM")->setVisible(true);
    
	_modifocationView.primitive = dynamic_cast<Text*>(root->getChildByName("Text_Tip"));

	auto oldPassword = dynamic_cast<TextField*>(root->getChildByName("TextField_old"));
	oldPassword->setPlaceHolderColor(FONT_CLOLOR_YELLOW_V88);
	oldPassword->setVisible(false);
	_modifocationView.curpass = HNEditBox::createEditBox(oldPassword, this);
	_modifocationView.curpass->setInput(HNEditBox::InputMode::SINGLE_LINE);

	auto newPassword = dynamic_cast<TextField*>(root->getChildByName("TextField_new"));
	newPassword->setPlaceHolderColor(FONT_CLOLOR_YELLOW_V88);
	newPassword->setVisible(false);
	_modifocationView.pass = HNEditBox::createEditBox(newPassword, this);
	_modifocationView.pass->setPasswordEnabled(false);
	_modifocationView.pass->setInput(HNEditBox::InputMode::SINGLE_LINE);

	auto checkPassword = dynamic_cast<TextField*>(root->getChildByName("TextField_check"));
	checkPassword->setPlaceHolderColor(FONT_CLOLOR_YELLOW_V88);
	checkPassword->setVisible(false);
	_modifocationView.pass->setPasswordEnabled(false);
	_modifocationView.yanZhengMa = HNEditBox::createEditBox(checkPassword, this);
	_modifocationView.yanZhengMa->setInput(HNEditBox::InputMode::SINGLE_LINE);

	auto modifyBtn = dynamic_cast<Button*>(root->getChildByName("Button_modify"));
	modifyBtn->setVisible(false);
	_modifocationView.modifyBtn = AUIButton::create(BTN_YELLOW1_NORMAL_BG_V88, BTN_YELLOW1_SELECT_BG_V88, BTN_YELLOW1_ENABLED_BG_V88);
	_modifocationView.modifyBtn->setTitle(BANK_BUTTON_MODIFY_TITLE);
	//_modifocationView.modifyBtn->setTitleOffset(Vec2(0, 4));
	_modifocationView.modifyBtn->setPosition(modifyBtn->getPosition());
	root->addChild(_modifocationView.modifyBtn);
	_modifocationView.modifyBtn->addTouchEventListener(CC_CALLBACK_2(GameBankV88::modifyPasswordCallBack, this));
}

void GameBankV88::modifyPasswordCallBack(Ref* pSender, Widget::TouchEventType type) {
	if (Widget::TouchEventType::ENDED != type) return;

	Button* psender = (Button*)pSender;

	if (psender == _modifocationView.modifyBtn) {
		HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
		// 银行打开失败，不做任何处理
		if (!_bankOpen) return;

		bool canModify = false;
		std::string oldPassword = _modifocationView.curpass->getString();
		std::string newPassword = _modifocationView.pass->getString();
		std::string surePassword = _modifocationView.yanZhengMa->getString();
		do {
			std::string errorText("");
			if (oldPassword.empty()) {
				GamePromptLayer::create()->showPrompt(GetText("TIPS1"));
				break;
			}

			if (newPassword.empty()) {
				GamePromptLayer::create()->showPrompt(GetText("TIPS2"));
				break;
			}

			if (surePassword.empty()) {
				GamePromptLayer::create()->showPrompt(GetText("TIPS3"));
				break;
			}

			if (surePassword.compare(newPassword)) {
				GamePromptLayer::create()->showPrompt(GetText("TIPS4"));
				break;
			}

			//不能少于6位
			if (newPassword.length() < BANK_PASSWORD_LIMIT_MIN_LENGTH)
			{
				GamePromptLayer::create()->showPrompt(GetText("TIPS27"));
				break;
			}

			TMSG_GP_BankChPwd BankChPwd = { 0 };
			BankChPwd.user_id = PlatformLogic()->loginResult.dwUserID;
			strcpy(BankChPwd.MD5Pass_old, MD5_CTX::MD5String(oldPassword).c_str());
			strcpy(BankChPwd.MD5Pass_new, MD5_CTX::MD5String(newPassword).c_str());
			PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_CHPWD, &BankChPwd, sizeof(BankChPwd),
				HN_SOCKET_CALLBACK(GameBankV88::bankerEventSelector, this));
		} while (0);
	}
}

void GameBankV88::scheduleNameCallBack() {
	if (_giveView.userId) {
		auto str = _giveView.userId->getString();
		reqUserNameById(str);
	}
}

void GameBankV88::scheduleMoneyCallBack() {
// 	if (_giveView.gameGlob) {
// 		auto Myoney = atoll(_giveView.gameGlob->getString().c_str());
// 		auto GiveMoney = atoll(_giveView.giveMoney->getString().c_str());
// 
// 		transition(_giveView.giveGlob, GiveMoney);
// 	}
}
// 转换字符串
void GameBankV88::transition(Text * text, LLONG money) {

	char str[30] = { 0 };
	if (money >= 100000000) {
		LLONG yiYu = money % 100000000;
		INT yi = money / 100000000;
		if (yiYu >= 10000) {
			LLONG wanYu = yiYu % 10000;
			UINT wan = yiYu / 10000;
			sprintf(str, "%d%s%d%s%lld", yi, GetText("TIPS23"), wan, GetText("TIPS22"), wanYu);
		}
		else {
			if (yiYu != 0) {
				sprintf(str, "%d%s%lld%s", yi, GetText("TIPS23"), yiYu, GetText("TIPS24"));
			}
			else {
				sprintf(str, "%d%s", yi, GetText("TIPS23"));
			}
		}
	}
	else {
		if (money >= 10000) {
			long wanYu = money % 10000;
			unsigned int wan = money / 10000;
			if (wanYu != 0) {
				sprintf(str, "%u%s%d%s", wan, GetText("TIPS22"), wanYu, GetText("TIPS24"));
			}
			else {
				sprintf(str, "%d%s", wan, GetText("TIPS22"));
			}
		}
		else {
			sprintf(str, "%lld", money);
		}
	}
	if (money == 0) {
		text->setString("");
	}
	else {
		text->setString(str);
	}
}

void GameBankV88::clearCurViewDat() {
	memset(&_giveView, 0x0, sizeof(_giveView));
	memset(&_modifocationView, 0x0, sizeof(_modifocationView));
	memset(&_recordView, 0x0, sizeof(_recordView));
}

bool GameBankV88::bankerForwardSelector(HNSocketMessage* socketMessage) {
	UINT bAssistantID = socketMessage->messageHead.bAssistantID;
	UINT bHandleCode = socketMessage->messageHead.bHandleCode;

	std::string errText("");
	if (ASS_GP_BANK_TRANSFER == bAssistantID) {	//
		if (HC_GP_BANK_TRANSFER_ERROR == bHandleCode) {	//1		//转帐错误
			errText = GetText("HC_GP_BANK_TRANSFER_ERROR");
		}
		else if (HC_GP_BANK_TRANSFER_NO_DEST == bHandleCode) {	//3		//转帐目标不存在
			errText = GetText("HC_GP_BANK_TRANSFER_NO_DEST");
		}
		else if (HC_GP_BANK_TRANSFER_PASSWORD_ERROR == bHandleCode) {	//4		//转帐密码错误
			errText = GetText("HC_GP_BANK_TRANSFER_PASSWORD_ERROR");
		}
		else if (HC_GP_BANK_TRANSFER_TOTAL_LESS == bHandleCode) {	//5		//银行总额太小不够资格
			errText = GetText("HC_GP_BANK_TRANSFER_TOTAL_LESS");
		}
		else if (HC_GP_BANK_TRANSFER_TOO_LESS == bHandleCode) { //6		// 单笔转帐数目太少
			errText = GetText("HC_GP_BANK_TRANSFER_TOO_LESS");
		}
		else if (HC_GP_BANK_TRANSFER_MULTIPLE == bHandleCode) { //7		// 单笔转帐数目必须是某数的倍数
			errText = GetText("HC_GP_BANK_TRANSFER_MULTIPLE");
		}
		else if (HC_GP_BANK_TRANSFER_NOT_ENOUGH == bHandleCode) { //8		// 银行金额不够本次转帐
			errText = GetText("HC_GP_BANK_TRANSFER_NOT_ENOUGH");
		}
		else if (HC_GP_BANK_TRANSFER_TOO_MANY_TIME == bHandleCode) {	//9	// 当天转账的次数太多了
			errText = GetText("HC_GP_BANK_TRANSFER_TOO_MANY_TIME");
		}
		else if (HC_GP_BANK_TRANSFER_SET_REBATE_RES == bHandleCode || HC_GP_BANK_TRANSFER_UPDATE_USER_REBATE == bHandleCode) {	//9	// 返利信息
			return false;
		}
		else if (HC_GP_BANK_TRANSFER_Disable_SXF == bHandleCode) {
			errText = GetText("HC_GP_BANK_NOT_TARGET");
		}
		else if (HC_GP_BANK_TRANSFER_SUC == bHandleCode) {	//2		//转帐成功
			errText = GetText("HC_GP_BANK_TRANSFER_SUC");
			CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
			auto i64Money = atoll(_giveView.giveMoney->getString().c_str());
			auto loginResult = PlatformLogic()->getInstance()->loginResult;
			PlatformLogic()->getInstance()->loginResult.i64Bank = loginResult.i64Bank - i64Money;
			_walletMoney = PlatformLogic()->getInstance()->loginResult.i64Bank;
			_giveView.bankMoney->setString(Tools::addComma(_walletMoney));

			TMSG_GP_BankTransfer* data = (TMSG_GP_BankTransfer*)(socketMessage->object);

			char month[5] = { 0 };

			if (data->month < 10) {
				sprintf(month, "0%d", data->month);
			}
			else {
				sprintf(month, "%d", data->month);
			}

			char day[5] = { 0 };
			if (data->day < 10) {
				sprintf(day, "0%d", data->day);
			}
			else {
				sprintf(day, "%d", data->day);
			}

			char second[5] = { 0 };
			if (data->second < 10) {
				sprintf(second, "0%d", data->second);
			}
			else {
				sprintf(second, "%d", data->second);
			}

			char minute[5] = { 0 };
			if (data->minute < 10) {
				sprintf(minute, "0%d", data->minute);
			}
			else {
				sprintf(minute, "%d", data->minute);
			}

			char hour[5] = { 0 };
			if (data->hour < 10) {
				sprintf(hour, "0%d", data->hour);
			}
			else {
				sprintf(hour, "%d", data->hour);
			}
			char time[100] = { 0 };
			sprintf(time, "%d-%s-%s %s:%s:%s", data->year, month, day, hour, minute, second);
			//转账成功记录ID
			UserDefault::getInstance()->setStringForKey("BANK_LASTGIVE_MONEY_USERID", _giveView.userId->getString());
			auto bankPrompt = BankGivePrompt::create();
			bankPrompt->updateInfo(data->szDestNickName, data->i64Money, time, data->destUserID);
			this->addChild(bankPrompt, 1000000);// 这个值就设置这么大
			_giveView.oldGiveId = data->destUserID;
			_giveView.oldGiveMoney = data->i64Money;
			_giveView.giveMoney->setString("");
			_giveView.mima->setString("");
			if (_time > 61) {
				_time = 0;
			}
			schedule(schedule_selector(GameBankV88::starTime), 1.0f);

			return true;
		}
		else {
			int i = 0;
		}
		CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
		GamePromptLayer::create()->showPrompt(errText);
	}
	else if (ASS_GP_BANK_TRANS_RECORD == bAssistantID) {	// 转账查询
		if (HC_GP_BANK_TRANS_RECORD_SUC == bHandleCode) {
			//if (sizeof(TMSG_GP_BankTranRecord) != socketMessage->objectSize); return true;
			TMSG_GP_BankTranRecord_Result* data = (TMSG_GP_BankTranRecord_Result*)(socketMessage->object);
			CCLOG("size = %d", socketMessage->objectSize);
			//判断是否在查看记录界面 如果不在 不做处理 （回包有延迟）
			if (_recordView.listView) {
				for (int i = 0; i < 10; i++) {
					if (data->items[i].day == 0) {
						continue;
					}

					if (_curpage == data->items[i].curPage) {
						CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
						return false;
					}

					auto node = CSLoader::createNode(BANKER_RECORD_TEXT_CSB);

					string str = data->items[i].dwUserID == 8888 ? GetText("SYSTEM") : to_string(data->items[i].dwUserID);

                    Color3B color = Color3B(213, 228, 255);
                    
					auto meIDText = dynamic_cast<Text *>(node->getChildByName("Text_id"));
					meIDText->setColor(color);
					meIDText->setString(str);

					auto targetIdText = dynamic_cast<Text *>(node->getChildByName("Text_targetId"));
					targetIdText->setColor(color);
					targetIdText->setString(to_string(data->items[i].destUserID));

					auto moneyText = dynamic_cast<Text *>(node->getChildByName("Text_money"));
					moneyText->setColor(color);
					moneyText->setString(to_string(data->items[i].i64Money));
					
					auto beizhuText = dynamic_cast<Text *>(node->getChildByName("Text_beizhu"));
					beizhuText->setColor(color);
					beizhuText->setString(ForceGBKToUtf8(data->items[i].szRemark));

					CCLOG(" hour = %d  min = %d  second = %d", data->items[i].hour, data->items[i].minute, data->items[i].second);
					char month[5] = { 0 };

					if (data->items[i].month < 10) {
						sprintf(month, "0%d", data->items[i].month);
					}
					else {
						sprintf(month, "%d", data->items[i].month);
					}

					char day[5] = { 0 };
					if (data->items[i].day < 10) {
						sprintf(day, "0%d", data->items[i].day);
					}
					else {
						sprintf(day, "%d", data->items[i].day);
					}

					char second[5] = { 0 };
					if (data->items[i].second < 10) {
						sprintf(second, "0%d", data->items[i].second);
					}
					else {
						sprintf(second, "%d", data->items[i].second);
					}

					char minute[5] = { 0 };
					if (data->items[i].minute < 10) {
						sprintf(minute, "0%d", data->items[i].minute);
					}
					else {
						sprintf(minute, "%d", data->items[i].minute);
					}

					char hour[5] = { 0 };
					if (data->items[i].hour < 10) {
						sprintf(hour, "0%d", data->items[i].hour);
					}
					else {
						sprintf(hour, "%d", data->items[i].hour);
					}

					string time;
					time = month;
					time = StringUtils::format("%d-%s-%s %s:%s:%s", data->items[i].year, month, day, hour, minute, second);

					auto timeText = static_cast<Text *>(node->getChildByName("Text_time"));
					timeText->setColor(color);
					timeText->setString(time);

					auto layout = Layout::create();
					layout->setContentSize(Size(815.0, 25));
					node->setPositionY(1);
					layout->addChild(node);
					_recordView.listView->addChild(layout);
					_vecRecord.push_back(data->items[i]);
					_toutleRecord = data->items[i].totalRecord;
					_recordView.listView->forceDoLayout();
				}
				_curpage = _idxJIlu;
				_idxJIlu++;

			}
			CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
		}
	}
	return true;
}

void GameBankV88::starTime(float dt) {
	_time += 1;
}

char * GameBankV88::Gettime(unsigned long nSeconds) {
	unsigned long hour = 0, min = 0, MicroSecond = 0, Millisecond = 0;

	Millisecond = nSeconds * 1000;
	MicroSecond = nSeconds * 1000000;

	hour = nSeconds / 3600;
	if (24 <= hour) {
		hour %= 24;
	}

	min = nSeconds % 3600 / 60;
	nSeconds = nSeconds % 3600 % 60;
	// printf("%d:%d:%d:%d",hour,min,Seconds,Millisecond);//这一行去掉
	char *pTime = new char[9];
	memset(pTime, 0, 9);
	sprintf(pTime, "%lu:%lu:%lu", hour, min, nSeconds);
	return pTime;
}

void GameBankV88::editBoxTextChanged(ui::EditBox* editBox)
{
	if (editBox == nullptr) return;
}

void GameBankV88::editBoxEditingDidEnd(ui::EditBox * editBox) {
	if (editBox == nullptr) return;

	switch (editBox->getTag()) {
	case give_money: {
						 scheduleMoneyCallBack();
						 break;
	}
	case give_userId: {
						  scheduleNameCallBack();
						  break;
	}
	default:
		break;
	}
}



bool GameBankV88::bankerEventSelector(HNSocketMessage* socketMessage) {
	UINT bAssistantID = socketMessage->messageHead.bAssistantID;
	UINT bHandleCode = socketMessage->messageHead.bHandleCode;
	// 打开银行
	if (ASS_GP_BANK_OPEN == bAssistantID) {
		switch (bHandleCode) {
		case HC_GP_BANK_OPEN_SUC: { //					1		//打开钱柜成功
									  if (!_isOpenSaveView)  break;
									  // 广播过来
									  CCAssert(sizeof(TMSG_GP_BankInfo) == socketMessage->objectSize, "TMSG_GP_BankInfo size is error.");
									  if (sizeof(TMSG_GP_BankInfo) != socketMessage->objectSize) return true;

									  TMSG_GP_BankInfo * bankInfo = reinterpret_cast<TMSG_GP_BankInfo*>(socketMessage->object);

									  _bankMoney = bankInfo->i64Bank;

									  _walletMoney = bankInfo->i64Wallet;

									  PlatformLogic()->getInstance()->loginResult.i64Bank = bankInfo->i64Bank;
									  PlatformLogic()->getInstance()->loginResult.i64Money = bankInfo->i64Wallet;

									  char buffer[128];
									  sprintf(buffer, "%lld", _walletMoney);
									  if (_saveView.bagMoneyText != nullptr) {
										  _saveView.bagMoneyText->setString(Tools::goldDisplayByType(_walletMoney));
									  }

									  sprintf(buffer, "%lld", _bankMoney);
									  if (_saveView.bankMoneyText != nullptr) {
										  _saveView.bankMoneyText->setString(Tools::goldDisplayByType(_bankMoney));
									  }

									  _bankOpen = true;

		}
			break;
		case HC_GP_BANK_OPEN_ERR: { //					2		//打开钱柜错误
									  _bankOpen = false;
		}
			break;
		case HC_GP_BANK_OPEN_ERR_PSW: { //				3		//打开钱框错误
										  _bankOpen = false;
		}
			break;
		default:
			break;
		}
	}
	// 修改密码
	else if (ASS_GP_BANK_CHPWD == bAssistantID) {
		switch (bHandleCode) {
		case HC_GP_BANK_CHPWD_SUC: { //					1			//修改成功
									   GamePromptLayer::create()->showPrompt(GetText("TIPS15"));
									   _modifocationView.curpass->setText("");
									   _modifocationView.pass->setText("");
									   _modifocationView.yanZhengMa->setText("");
		}
			break;
		case HC_GP_BANK_CHPWD_ERR_PWD: { //					2		//修改失败，密码错误
										   GamePromptLayer::create()->showPrompt(GetText("TIPS16"));
		}
			break;
		default:
			break;
		}
	}
	// 存取钱
	else if (ASS_GP_BANK_CHECK == bAssistantID) {
		switch (bHandleCode) {
		case HC_GP_BANK_CHECK_SUC: { //					0			//操作成功
									   // 广播过来
									   CCAssert(sizeof(TMSG_GP_BankCheck) == socketMessage->objectSize, "TMSG_GP_BankCheck size is error.");
									   if (sizeof(TMSG_GP_BankCheck) != socketMessage->objectSize) return true;

									   TMSG_GP_BankCheck *BankCheck = reinterpret_cast<TMSG_GP_BankCheck*>(socketMessage->object);

									   std::string promptInfo;
									   // 取钱
									   if (1 == BankCheck->operate_type) {

										   _bankMoney -= BankCheck->money;
										   _walletMoney += BankCheck->money;
										   PlatformLogic()->loginResult.i64Bank = _bankMoney;
										   PlatformLogic()->loginResult.i64Money = _walletMoney;
										   if (_delegate) {
											   _delegate->walletChanged(_walletMoney);
											   _delegate->bankChanged(_bankMoney);
										   }
										   //  _saveView.mima->setString("");
										   promptInfo = GetText("TIPS17");
										   UserDefault::getInstance()->setStringForKey("bankTransactionPassword", _saveView.mima->getString());
										   //_saveView.quButton->setSelected(false);
										   _saveView.takeButton->setEnabled(true);
									   }

									   // 存钱
									   if (2 == BankCheck->operate_type) {
										   _bankMoney += BankCheck->money;
										   _walletMoney -= BankCheck->money;
										   PlatformLogic()->loginResult.i64Bank = _bankMoney;
										   PlatformLogic()->loginResult.i64Money = _walletMoney;
										   if (_delegate) {
											   _delegate->walletChanged(_walletMoney);
											   _delegate->bankChanged(_bankMoney);
										   }
										   // _saveView.mima->setString("");
										   UserDefault::getInstance()->setStringForKey("bankTransactionPassword", _saveView.mima->getString());
										   //_saveView.allButton->setSelected(false);
										   _saveView.saveButton->setEnabled(true);
										   promptInfo = GetText("TIPS18");
									   }

									   char buffer[128];
									   sprintf(buffer, "%lld", _walletMoney);
									   if (nullptr != _saveView.bagMoneyText) {
										   _saveView.bagMoneyText->setString(Tools::fourSeparateMoney(_walletMoney));
									   }

									   sprintf(buffer, "%lld", _bankMoney);
									   if (nullptr != _saveView.bankMoneyText) {
										   _saveView.bankMoneyText->setString(Tools::fourSeparateMoney(_bankMoney));
									   }

									   _saveView.moneyField->setText("");
									   GamePromptLayer::create()->showPrompt(promptInfo.c_str());
		}
			break;
		case HC_GP_BANK_CHECK_NO_USER: { //				1			//没有找到用户
										   GamePromptLayer::create()->showPrompt(GetText("TIPS19"));
		}
			break;
		case HC_GP_BANK_CHECK_ERR_PWD: { //				2			//错误密码
										   GamePromptLayer::create()->showPrompt(GetText("TIPS20"));
		}
			break;
		case HC_GP_BANK_CHECK_NOT_MONEY: { //				4			//余额不足
											 GamePromptLayer::create()->showPrompt(GetText("TIPS21"));
		}
			break;
		default:
			break;
		}
	}
	return true;
}
