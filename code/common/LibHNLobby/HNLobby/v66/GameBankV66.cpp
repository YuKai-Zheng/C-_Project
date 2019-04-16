#include "GameBankV66.h"
#include "PlatformResV66.h"
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
#include "BindPhone.hpp"
#include "../GameData.h"
#include "GameBankEnsureV66.hpp"
#include "../pattern/factory/gameBank/GameBankFactory.h"

//子界面csb
static const char* BANKER_SAVE_CSB = "csb/newplatform/bank/node_qukuan.csb";
static const char* BANKER_GIVE_CSB = "csb/newplatform/bank/node_zengsong.csb";
static const char* BANKER_MODIFICATION_CSB = "csb/newplatform/bank/node_gaimi.csb";
static const char* BANKER_RECORD_CSB = "csb/newplatform/bank/node_jilu.csb";
static const char* BANKER_RECORD_TEXT_CSB = "csb/newplatform/bank/record_text.csb";

static const int	BANK_PASSWORD_LIMIT_MIN_LENGTH = 6;

#define WINSIZE Director::getInstance()->getWinSize()
#define JSON_RESOVE(obj, key) (obj.HasMember(key) && !obj[key].IsNull())

GameBankV66::GameBankV66()
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

GameBankV66::~GameBankV66() {
	_vecButton.clear();
	_vecRecord.clear();
	PlatformLogic()->removeEventSelector(MDM_GP_BANK, ASS_GP_BANK_OPEN);
	PlatformLogic()->removeEventSelector(MDM_GP_BANK, ASS_GP_BANK_CHPWD);
	PlatformLogic()->removeEventSelector(MDM_GP_BANK, ASS_GP_BANK_CHECK);
	PlatformLogic()->removeEventSelector(MDM_GP_BANK, ASS_GP_BANK_TRANSFER);
}

bool GameBankV66::init() {
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

	Node* _rootNode = CSLoader::createNode("csb/newplatform/bank/node_bank.csb");
	addChild(_rootNode);
	_rootNode->setPosition(WIN_SIZE / 2);

	auto closeBtn = dynamic_cast<Button*>(_rootNode->getChildByName("Button_exit"));
	closeBtn->setAlphaTouchEnable(true);
	closeBtn->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type){
		if (type != Widget::TouchEventType::ENDED) return;
		close();
	});

 	// 这个节点做子页面的父节点管理
	_childViewNode = _rootNode->getChildByName("bg")->getChildByName("detail");
	auto buttons = dynamic_cast<Layout*>(_rootNode->getChildByName("bg")->getChildByName("buttons"));
	auto btn_qukuan = dynamic_cast<Button*>(buttons->getChildByName("btn_qukuan"));
	auto btn_zengsong = dynamic_cast<Button*>(buttons->getChildByName("btn_zengsong"));
	auto btn_jilu = dynamic_cast<Button*>(buttons->getChildByName("btn_jilu"));
	auto btn_gaimi = dynamic_cast<Button*>(buttons->getChildByName("btn_gaimi"));
	btn_qukuan->addTouchEventListener(CC_CALLBACK_2(GameBankV66::menuButtonCallBack, this));
	btn_zengsong->addTouchEventListener(CC_CALLBACK_2(GameBankV66::menuButtonCallBack, this));
	btn_jilu->addTouchEventListener(CC_CALLBACK_2(GameBankV66::menuButtonCallBack, this));
	btn_gaimi->addTouchEventListener(CC_CALLBACK_2(GameBankV66::menuButtonCallBack, this));
	btn_qukuan->setTag(btn_save + 0);
	btn_zengsong->setTag(btn_save + 1);
	btn_jilu->setTag(btn_save + 2);
	btn_gaimi->setTag(btn_save + 3);
	_vMenuTitle.clear();

	_vecButton.push_back(btn_qukuan);
	btn_zengsong->setOpacity(0);
	_vecButton.push_back(btn_zengsong);
	btn_jilu->setOpacity(0);
	_vecButton.push_back(btn_jilu);
	btn_gaimi->setOpacity(0);
	_vecButton.push_back(btn_gaimi);

	//MSG_GP_R_LogonResult &LogonResult = PlatformLogic()->loginResult;
	//auto txtMoney = dynamic_cast<Text*>(_rootNode->getChildByName("bg")->getChildByName("coin")->getChildByName("Text_money"));
	//if (txtMoney) {
		//txtMoney->setString(Tools::addComma(LogonResult.i64Money));
	//}
	
	//默认子界面
	addDefaultView();

	//打开银行

	//适配处理，调整按钮位置及缩放背景图
	//auto bg = (Layout*)_rootNode->getChildByName("bg");
	//bg->setContentSize(Size(WIN_SIZE.width, bg->getContentSize().height));
	//bg->getChildByName("titlebg")->setScaleX(WIN_SIZE.width / 1280);
	//bg->getChildByName("titlebg")->setPositionX(WIN_SIZE.width / 2);
	//((Button*)bg->getChildByName("Button_exit"))->setPositionX(0);
	//((ImageView*)bg->getChildByName("coin"))->setPositionX(WIN_SIZE.width-40);
	//((ImageView*)bg->getChildByName("Image_1"))->setPositionX(WIN_SIZE.width/2);
	//((Layout*)bg->getChildByName("buttons"))->setPositionX(WIN_SIZE.width / 2-295.61);
	//((Node*)bg->getChildByName("detail"))->setPositionX(WIN_SIZE.width / 2 +80);

	return true;

}

void GameBankV66::open(Node* parent, Vec2 vec, int zorder, int tag) {
    CCAssert(nullptr != parent, "parent is nullptr");
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setPosition(vec);
    parent->addChild(this, zorder, tag);
    
}

void GameBankV66::close() {
    //runAction(Sequence::create(ScaleTo::create(0.2f, 0.2f), RemoveSelf::create(), nullptr));
	removeFromParent();
}

//创建子界面
void GameBankV66::menuButtonCallBack(Ref* pSender, Widget::TouchEventType touchtype) {
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
	//touchEffect(btn->getTag());

	for (int i = 0; i < 4; i++)
	{
		//char str[64] = { 0 };
		//sprintf(str, "csb/newplatform/bank/img/%d.png", i );
		auto btn = _vecButton[i];
		btn->setOpacity(0);
		//dynamic_cast<Sprite*>(btn->getChildByName("btn_bg"))->setTexture("csb/newplatform/bank/img/btn_huang.png");
		//dynamic_cast<Sprite*>(btn->getChildByName("btn_txt"))->setTexture(str);
	}

	switch (btn->getTag()) {
	case btn_save: {
		createSaveView(_childViewNode);
		btn->setOpacity(255);
		//dynamic_cast<Sprite*>(_vecButton[0]->getChildByName("btn_bg"))->setTexture("csb/newplatform/bank/img/btn_yh_lan.png");
		//dynamic_cast<Sprite*>(_vecButton[0]->getChildByName("btn_txt"))->setTexture("csb/newplatform/bank/img/10.png");
		break;
	}
	case btn_give: {
		//dynamic_cast<Sprite*>(_vecButton[1]->getChildByName("btn_bg"))->setTexture("csb/newplatform/bank/img/btn_yh_lan.png");
		//dynamic_cast<Sprite*>(_vecButton[1]->getChildByName("btn_txt"))->setTexture("csb/newplatform/bank/img/11.png");
		btn->setOpacity(255);
		createGiveView(_childViewNode);
		_isOpenSaveView = false;
		break;
	}
	case btn_record: {
		//dynamic_cast<Sprite*>(_vecButton[2]->getChildByName("btn_bg"))->setTexture("csb/newplatform/bank/img/btn_yh_lan.png");
		//dynamic_cast<Sprite*>(_vecButton[2]->getChildByName("btn_txt"))->setTexture("csb/newplatform/bank/img/12.png");
		btn->setOpacity(255);
		createRecordView(_childViewNode);
		_isOpenSaveView = false;
		break;
	}
	case btn_modification: {
		//dynamic_cast<Sprite*>(_vecButton[3]->getChildByName("btn_bg"))->setTexture("csb/newplatform/bank/img/btn_yh_lan.png");
		//dynamic_cast<Sprite*>(_vecButton[3]->getChildByName("btn_txt"))->setTexture("csb/newplatform/bank/img/13.png");
		btn->setOpacity(255);
		createModificationView(_childViewNode);
		_isOpenSaveView = false;
		break;
	}
	default:
		break;
	}
}

//点击效果
void GameBankV66::touchEffect(UINT tag) {
	for (auto child : _vecButton) {
		child->setTouchEnabled(!(tag == child->getTag()));
		child->setBright(!(tag == child->getTag()));

	}
}

//进入银行默认子页
void GameBankV66::addDefaultView() {
	_oldViewTag = btn_save;
	//touchEffect(btn_save);
	createSaveView(_childViewNode);
}

void GameBankV66::reqUserNameById(std::string id) {
	std::string URL = std::string("http://") + getWebServerUrl() + std::string("/Public/AuditConfig.ashx?");
	std::string str("action=GetUserNameByUserID&key=");
	str = str + id;

	HttpRequest* request = new (std::nothrow) HttpRequest();
	request->setUrl(URL.c_str());
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(GameBankV66::setTargetUserNameById, this));
	request->setRequestData(str.c_str(), str.size());
	HttpClient::getInstance()->sendImmediate(request);

	request->release();
}

void GameBankV66::setTargetUserNameById(network::HttpClient* client, network::HttpResponse* response) {
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
void GameBankV66::createSaveView(Node* parent) {
	auto root = CSLoader::createNode(BANKER_SAVE_CSB);	
	auto v = Vec2(90, 100);
    root->setPosition(v);
	_oldViewTag = btn_save;
	_saveView.isAllSaveTouch = false;
	_saveView.isAllTakeTouch = false;

	_saveView.bagMoneyText = dynamic_cast<Text*>(root->getChildByName("bg")->getChildByName("txt_bagcoin"));
	_saveView.bankMoneyText = dynamic_cast<Text*>(root->getChildByName("bg")->getChildByName("txt_bankcoin"));

	auto moneyField = dynamic_cast<TextField*>(root->getChildByName("bg")->getChildByName("txt_money")->getChildByName("TextField_1"));
	_saveView.moneyField = HNEditBox::createEditBox(moneyField, this);
	_saveView.moneyField->setInput(HNEditBox::InputMode::PHONE_NUMBER);
	_saveView.moneyField->setTag(input_money);
	_saveView.moneyField->setDelegate(this);
	editBoxEditingDidEnd(_saveView.moneyField);

	std::string localMima = UserDefault::getInstance()->getStringForKey("bankTransactionPassword");
	_saveView.saveImageCheckBox = dynamic_cast<CheckBox*>(root->getChildByName("bg")->getChildByName("CheckBox_saveImage"));
	_saveView.saveImageCheckBox->setTag(save_image);
	_saveView.saveImageCheckBox->addEventListener(CC_CALLBACK_2(GameBankV66::checkBoxCallBack, this));

	_saveView.takeImageCheckBox = dynamic_cast<CheckBox*>(root->getChildByName("bg")->getChildByName("CheckBox_takeImage"));
	_saveView.takeImageCheckBox->setTag(take_iamge);
	_saveView.takeImageCheckBox->addEventListener(CC_CALLBACK_2(GameBankV66::checkBoxCallBack, this));
	_saveView.saveImageCheckBox->setSelected(false);
	_saveView.takeImageCheckBox->setSelected(false);

	_saveView.saveButton = dynamic_cast<Button*>(root->getChildByName("bg")->getChildByName("btn_cunru"));
	_saveView.saveButton->setTag(btn_cun);
	_saveView.saveButton->addTouchEventListener(CC_CALLBACK_2(GameBankV66::saveTakeMoneyCallBack, this));

	_saveView.takeButton = dynamic_cast<Button*>(root->getChildByName("bg")->getChildByName("btn_quchu"));
	_saveView.takeButton->setTag(btn_take);
	_saveView.takeButton->addTouchEventListener(CC_CALLBACK_2(GameBankV66::saveTakeMoneyCallBack, this));
    
    char szMD5PassLocal[50];
    sprintf(szMD5PassLocal,"%s",GameData::getInstance()->getUserBankPassWordMD5().c_str());
    TMSG_GP_BankOpenReq pdata;
    memcpy(pdata.szMD5Pass,szMD5PassLocal,sizeof(szMD5PassLocal));
    PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_OPEN,&pdata,sizeof(pdata), HN_SOCKET_CALLBACK(GameBankV66::bankerEventSelector, this));
    
	parent->addChild(root);
	_isOpenSaveView = true;
}

void GameBankV66::checkBoxCallBack(Ref* sender, CheckBox::EventType type) {
	auto checkBox = dynamic_cast<CheckBox*>(sender);
	int tagType = checkBox->getTag();

	switch (type) {
	case CheckBox::EventType::SELECTED:
		if (tagType == save_word || tagType == save_image)
		{
			_saveView.llMoneyAmount = PlatformLogic()->loginResult.i64Money;
			std::string strMoney = Tools::goldDisplayByType(_saveView.llMoneyAmount);
			_saveView.moneyField->setString(strMoney);
			_saveView.saveImageCheckBox->setSelected(true);
			_saveView.takeImageCheckBox->setSelected(false);
			_saveView.saveButton->setTouchEnabled(true);
			_saveView.takeButton->setTouchEnabled(false);
			_saveView.isAllTakeTouch = false;
			_saveView.isAllSaveTouch = true;
		}
		else
		{
			_saveView.llMoneyAmount = PlatformLogic()->loginResult.i64Bank;
			std::string strMoney = Tools::goldDisplayByType(_saveView.llMoneyAmount);
			_saveView.moneyField->setString(strMoney);
			_saveView.saveImageCheckBox->setSelected(false);
			_saveView.takeImageCheckBox->setSelected(true);
			_saveView.saveButton->setTouchEnabled(false);
			_saveView.takeButton->setTouchEnabled(true);
			_saveView.isAllTakeTouch = true;
			_saveView.isAllSaveTouch = false;
		}
		break;
	case CheckBox::EventType::UNSELECTED:
		_saveView.moneyField->setString("");
		_saveView.llMoneyAmount = 0;
		if (tagType == save_word || tagType == save_image)
		{
			//_saveView.saveWordCheckBox->setSelected(false);
			_saveView.saveImageCheckBox->setSelected(false);
			_saveView.takeButton->setTouchEnabled(true);
			//_saveView.takeButton->setBright(false);
			_saveView.isAllSaveTouch = false;
            
		}
		else
		{
			//_saveView.takeWordCheckBox->setSelected(false);
			_saveView.takeImageCheckBox->setSelected(false);
			_saveView.saveButton->setTouchEnabled(true);
			//_saveView.saveButton->setBright(false);
			_saveView.isAllTakeTouch = false;
		}
		break;
	default:
		break;
	}
}

void GameBankV66::saveTakeMoneyCallBack(Ref* pSender, Widget::TouchEventType type) {
	Button* psender = (Button*)pSender;
	if (Widget::TouchEventType::ENDED != type) {
		if (type == Widget::TouchEventType::BEGAN)
		{
			psender->setScale(0.8f);
		}
		else if (type == Widget::TouchEventType::CANCELED)
		{
			psender->setScale(1.0f);
		}
		return;
	}
	
	psender->setScale(1.0f);
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
				BankCheck.money = _saveView.llMoneyAmount;
				if (BankCheck.money <= 0) {
					GamePromptLayer::create()->showPrompt(GetText("TIPS11"));
					break;
				}
				PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_CHECK, &BankCheck, sizeof(BankCheck),
					HN_SOCKET_CALLBACK(GameBankV66::bankerEventSelector, this));
			} while (0);
		}
		// 取款
		else if (psender->getTag() == btn_take) {
			std::string strMoney = _saveView.moneyField->getString();
            std::string password = GameData::getInstance()->getUserBankPassWord();
			do {
				if (strMoney.empty()) {
					GamePromptLayer::create()->showPrompt(GetText("TIPS12"));
					break;
				}

				if (password.empty()) {
					GamePromptLayer::create()->showPrompt(GetText("TIPS9"));
					break;
				}
                
                std::string savePassWord = GameData::getInstance()->getUserBankPassWord();
				TMSG_GP_BankCheck BankCheck = { 0 };
				BankCheck.operate_type = 1;
				BankCheck.user_id = PlatformLogic()->loginResult.dwUserID;
				BankCheck.game_id = 0;
				BankCheck.money = _saveView.llMoneyAmount;
				strcpy(BankCheck.szMD5Pass, MD5_CTX::MD5String(savePassWord).c_str());
				if (BankCheck.money <= 0) {
					GamePromptLayer::create()->showPrompt(GetText("TIPS11"));
					break;
				}
				PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_CHECK, &BankCheck, sizeof(BankCheck),
					HN_SOCKET_CALLBACK(GameBankV66::bankerEventSelector, this));
			} while (0);
		}
		else {
			GamePromptLayer::create()->showPrompt(GetText("TIPS13"));
		}
	}
}

//赠送金币子界面
void GameBankV66::createGiveView(Node* parent) {
	_oldViewTag = btn_give;

	auto root = CSLoader::createNode(BANKER_GIVE_CSB);
	parent->addChild(root);
    root->setPosition(Vec2(90,100));
   // getChildByName("BG_BOTTOM")->setVisible(true);
    
	_giveView.targetName = dynamic_cast<Text*>(root->getChildByName("Text_targetName"));
	//_giveView.targetName->setColor(FONT_CLOLOR_YELLOW_V66);
	_giveView.bankMoney = dynamic_cast<Text*>(root->getChildByName("bg")->getChildByName("txt_bankcoin"));
	//_giveView.bankMoney->setColor(FONT_CLOLOR_YELLOW_V66);
	_giveView.bankMoney->setString(Tools::goldDisplayByType(PlatformLogic()->getInstance()->loginResult.i64Bank));

	_giveView.giveBtn = dynamic_cast<Button*>(root->getChildByName("bg")->getChildByName("btn_zengsong"));
	//giveBtn->setVisible(false);
	//_giveView.giveBtn = AUIButton::create("v66/module/bank/22.png", "v66/module/bank/22_1.png", "v66/module/bank/22_1.png");
	////_giveView.giveBtn->setTitle(BANK_BUTTON_GIVE_TITLE);
	////_giveView.giveBtn->setTitleOffset(Vec2(0, 4));
	//_giveView.giveBtn->setPosition(giveBtn->getPosition());
	//root->addChild(_giveView.giveBtn);
	_giveView.giveBtn->addTouchEventListener(CC_CALLBACK_2(GameBankV66::giveMoneyCallBack, this));
   
    _giveView.max = dynamic_cast<Button*>(root->getChildByName("bg")->getChildByName("btn_max"));
    if (_giveView.max) {
        _giveView.max->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type){
            if (type != Widget::TouchEventType::ENDED) {
                return;
            }
            if (_giveView.giveMoney) {
                //std::string maxMoney = StringUtils::format("%lld",PlatformLogic()->getInstance()->loginResult.i64Bank);
				std::string maxMoney = Tools::goldDisplayByType(PlatformLogic()->getInstance()->loginResult.i64Bank);
                _giveView.giveMoney->setString(maxMoney);
				_giveView.llGiveMoney = PlatformLogic()->getInstance()->loginResult.i64Bank;
            }
            if (_giveView.sliderBar) {
                _giveView.sliderBar->setPercent(100);
            }
        });
    }
    
    _giveView.sliderBar = dynamic_cast<Slider*>(root->getChildByName("bg")->getChildByName("Slider_coin"));
	_giveView.sliderBar->setPercent(0);
    if (_giveView.sliderBar) {
        _giveView.sliderBar->addEventListener([=](cocos2d::Ref * ref, Slider::EventType type){
            switch (type)
            {
                case cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED:
                {
                    int percent = _giveView.sliderBar->getPercent();
                    if (_giveView.giveMoney) {
                        long long curMoney = PlatformLogic()->getInstance()->loginResult.i64Bank;
                        //std::string maxMoney = StringUtils::format("%lld",curMoney * percent / 100);
						_giveView.llGiveMoney = curMoney * percent / 100;
						std::string maxMoney = Tools::goldDisplayByType(_giveView.llGiveMoney);
                        _giveView.giveMoney->setString(maxMoney);
                    }
                }
                    break;
                default:
                    break;
            }

        });
    }
    
	//auto password = dynamic_cast<TextField*>(root->getChildByName("TextField_password"));
	//password->setPlaceHolder(GBKToUtf8("初始密码为123456"));
	//password->setPlaceHolderColor(FONT_CLOLOR_YELLOW_V66);
	//password->setVisible(false);
	//_giveView.mima = HNEditBox::createEditBox(password, this);
	//_giveView.mima->setInput(HNEditBox::InputMode::SINGLE_LINE);
	//_giveView.mima->setInputFlag(ui::EditBox::InputFlag::PASSWORD);

	auto giveMoney = dynamic_cast<TextField*>(root->getChildByName("bg")->getChildByName("txt_money")->getChildByName("TextField_1"));
	//giveMoney->setPlaceHolderColor(FONT_CLOLOR_YELLOW_V66);
	giveMoney->setVisible(false);
	_giveView.giveMoney = HNEditBox::createEditBox(giveMoney, this);
	_giveView.giveMoney->setInput(ui::EditBox::InputMode::PHONE_NUMBER);
	_giveView.giveMoney->setTag(give_money);
	editBoxEditingDidEnd(_giveView.giveMoney);

	auto userId = dynamic_cast<TextField*>(root->getChildByName("bg")->getChildByName("txt_id")->getChildByName("TextField_1"));
	//userId->setPlaceHolderColor(FONT_CLOLOR_YELLOW_V66);
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

void GameBankV66::giveMoneyCallBack(Ref* pSender, Widget::TouchEventType type) {

	Button* psender = (Button*)pSender;
	if (Widget::TouchEventType::ENDED != type) {
		if (type == Widget::TouchEventType::BEGAN)
		{
			psender->setScale(0.8f);
		}
		else if (type == Widget::TouchEventType::CANCELED)
		{
			psender->setScale(1.0f);
		}
		return;
	}

	psender->setScale(1.0f);
	if (psender == _giveView.giveBtn) {
		HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
		std::string userText = _giveView.userId->getString();
		std::string forwardMoney = _giveView.giveMoney->getString();
        std::string bankPassword = GameData::getInstance()->getUserBankPassWord();

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

				if (_giveView.oldGiveId == atoi(_giveView.userId->getString().c_str()) && _giveView.llGiveMoney == _giveView.oldGiveMoney && _time <= 60) {
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
						bankTransfer.i64Money = _giveView.llGiveMoney;
						bankTransfer.bUseDestID = true;
						strcpy(bankTransfer.szDestNickName, Utf8ToGB(_giveView.targetName->getString().c_str()));
                        strcpy(bankTransfer.szMD5Pass, MD5_CTX::MD5String(GameData::getInstance()->getUserBankPassWord()).c_str());
					}
					_walletMoney -= bankTransfer.i64ActualTransfer;
					if (bankTransfer.i64Money <= 0) {
						GamePromptLayer::create()->showPrompt(GetText("TIPS11"));
						return;
					}
					CommandManager::getInstance()->addLoaderCommand("");
					PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_TRANSFER, &bankTransfer, sizeof(bankTransfer),
						HN_SOCKET_CALLBACK(GameBankV66::bankerForwardSelector, this));
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
void GameBankV66::createRecordView(Node* parent) {
	_idxJIlu = 1;
	_oldViewTag = btn_record;

	auto root = CSLoader::createNode(BANKER_RECORD_CSB);
	_childViewNode->addChild(root);
    root->setPosition(root->getPosition() + Vec2(110,110));
    //getChildByName("BG_BOTTOM")->setVisible(false);
    
	_recordView.listView = dynamic_cast<ListView*>(root->getChildByName("bg")->getChildByName("ListView"));
	_recordView.listView->setScrollBarAutoHideEnabled(false);
	_recordView.listView->addTouchEventListener(CC_CALLBACK_2(GameBankV66::recordlistViewCallBack, this));

	////////////
	TMSG_GP_BankTranRecordReq data;
	data.dwUserID = PlatformLogic()->loginResult.dwUserID;
	data.pageSize = 10;
	data.curPage = _idxJIlu;
	data.reqType = 0;
	PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_TRANS_RECORD, &data, sizeof(data), HN_SOCKET_CALLBACK(GameBankV66::bankerForwardSelector, this));
	/////////////
	CommandManager::getInstance()->addLoaderCommand("");
}

void GameBankV66::recordlistViewCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
	switch (type) {

	case cocos2d::ui::Widget::TouchEventType::ENDED:
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
	{
		if (_recordView.listView->getInnerContainer()->getPositionY() > 5) {
			if (_toutleRecord == _vecRecord.size())
				return;

			TMSG_GP_BankTranRecordReq data;
			data.dwUserID = PlatformLogic()->loginResult.dwUserID;
			data.pageSize = 10;
			data.curPage = _idxJIlu;
			data.reqType = 0;
			PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_TRANS_RECORD, &data, sizeof(data), HN_SOCKET_CALLBACK(GameBankV66::bankerForwardSelector, this));
			CommandManager::getInstance()->addLoaderCommand("");
		}
	}
		break;
	default:
		break;
	}
}

//修改密码子界面
void GameBankV66::createModificationView(Node* parent) {
	_oldViewTag = btn_modification;
	auto root = CSLoader::createNode(BANKER_MODIFICATION_CSB);
	parent->addChild(root);
    root->setPosition(Vec2(90,100));
    //getChildByName("BG_BOTTOM")->setVisible(true);
    
	_modifocationView.primitive = dynamic_cast<Text*>(root->getChildByName("Text_Tip"));

	auto oldPassword = dynamic_cast<TextField*>(root->getChildByName("bg")->getChildByName("txt_old")->getChildByName("TF"));
	//oldPassword->setPlaceHolderColor(FONT_CLOLOR_YELLOW_V66);
	oldPassword->setVisible(false);
	_modifocationView.curpass = HNEditBox::createEditBox(oldPassword, this);
	_modifocationView.curpass->setInput(HNEditBox::InputMode::SINGLE_LINE);

	auto newPassword = dynamic_cast<TextField*>(root->getChildByName("bg")->getChildByName("txt_new")->getChildByName("TF"));
	//newPassword->setPlaceHolderColor(FONT_CLOLOR_YELLOW_V66);
	newPassword->setVisible(false);
	_modifocationView.pass = HNEditBox::createEditBox(newPassword, this);
	_modifocationView.pass->setPasswordEnabled(false);
	_modifocationView.pass->setInput(HNEditBox::InputMode::SINGLE_LINE);

	auto checkPassword = dynamic_cast<TextField*>(root->getChildByName("bg")->getChildByName("txt_check")->getChildByName("TF"));
	//checkPassword->setPlaceHolderColor(FONT_CLOLOR_YELLOW_V66);
	checkPassword->setVisible(false);
	_modifocationView.pass->setPasswordEnabled(false);
	_modifocationView.yanZhengMa = HNEditBox::createEditBox(checkPassword, this);
	_modifocationView.yanZhengMa->setInput(HNEditBox::InputMode::SINGLE_LINE);

	_modifocationView.modifyBtn = dynamic_cast<Button*>(root->getChildByName("bg")->getChildByName("btn_modify"));
	//modifyBtn->setVisible(false);
	//_modifocationView.modifyBtn = AUIButton::create("v66/module/bank/23.png", "v66/module/bank/23_1.png", "v66/module/bank/23_1.png");
	////_modifocationView.modifyBtn->setTitle(BANK_BUTTON_MODIFY_TITLE);
	////_modifocationView.modifyBtn->setTitleOffset(Vec2(0, 4));
	//_modifocationView.modifyBtn->setPosition(modifyBtn->getPosition());
	//root->addChild(_modifocationView.modifyBtn);
	_modifocationView.modifyBtn->addTouchEventListener(CC_CALLBACK_2(GameBankV66::modifyPasswordCallBack, this));
}

void GameBankV66::modifyPasswordCallBack(Ref* pSender, Widget::TouchEventType type) {
	Button* psender = (Button*)pSender;
	if (Widget::TouchEventType::ENDED != type)
	{
		if (type == Widget::TouchEventType::BEGAN)
		{
			psender->setScale(0.8f);
		}
		else if (type == Widget::TouchEventType::CANCELED)
		{
			psender->setScale(1.0f);
		}

		return;
	}
		
	psender->setScale(1.0f);
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
				HN_SOCKET_CALLBACK(GameBankV66::bankerEventSelector, this));
		} while (0);
	}
}

void GameBankV66::scheduleNameCallBack() {
	if (_giveView.userId) {
		auto str = _giveView.userId->getString();
		//reqUserNameById(str);modify by zengwc
	}
}

void GameBankV66::scheduleMoneyCallBack() {
// 	if (_giveView.gameGlob) {
// 		auto Myoney = atoll(_giveView.gameGlob->getString().c_str());
// 		auto GiveMoney = atoll(_giveView.giveMoney->getString().c_str());
// 
// 		transition(_giveView.giveGlob, GiveMoney);
// 	}
    if (_giveView.giveMoney) {
        //std::string inputStr = _giveView.giveMoney->getString();
        if (_giveView.llGiveMoney > 0) {
            //long long inputMoney = atoll(inputStr.c_str());
            LLONG bankMoney  = PlatformLogic()->getInstance()->loginResult.i64Bank;
            
            auto percent = (float)_giveView.llGiveMoney / bankMoney * 100;
            int m_percent = percent > 100 ? 100 : percent;
            if (_giveView.sliderBar) {
                _giveView.sliderBar->setPercent(m_percent);
            }
        }
       
    }
}
// 转换字符串
void GameBankV66::transition(Text * text, LLONG money) {

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

void GameBankV66::clearCurViewDat() {
	memset(&_giveView, 0x0, sizeof(_giveView));
	memset(&_modifocationView, 0x0, sizeof(_modifocationView));
	memset(&_recordView, 0x0, sizeof(_recordView));
}

bool GameBankV66::bankerForwardSelector(HNSocketMessage* socketMessage) {
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
        else if (HC_GP_BANK_TRANSFER_BIND_PHONE == bHandleCode)
        {
            errText = GetText("HC_GP_BANK_NOT_PHONE");
        }
		else if (HC_GP_BANK_TRANSFER_SUC == bHandleCode) {	//2		//转帐成功
			errText = GetText("HC_GP_BANK_TRANSFER_SUC");
			CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
			auto i64Money = _giveView.llGiveMoney;
			auto loginResult = PlatformLogic()->getInstance()->loginResult;
			PlatformLogic()->getInstance()->loginResult.i64Bank = loginResult.i64Bank - i64Money;
			_walletMoney = PlatformLogic()->getInstance()->loginResult.i64Bank;
			_giveView.bankMoney->setString(Tools::goldDisplayByType(_walletMoney));

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
			_giveView.llGiveMoney = 0.0f;
			_giveView.giveMoney->setUserData(&_giveView.llGiveMoney);
			//_giveView.mima->setString("");
            if (_giveView.sliderBar) {
                _giveView.sliderBar->setPercent(0);
            }
			if (_time > 61) {
				_time = 0;
			}
			schedule(schedule_selector(GameBankV66::starTime), 1.0f);

			return true;
		}
		else {
			int i = 0;
		}
        
		CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
        auto gamePrompt = GamePromptLayer::create();
        gamePrompt->showPrompt(errText);
        if (HC_GP_BANK_TRANSFER_BIND_PHONE == bHandleCode) {
            gamePrompt->setOkayCallBack([this](){
                auto bindPhone = BindPhoneGui::create();
                bindPhone->SetOpType(BindPhoneOpType::VerifyCode, true, PlatformLogic()->loginResult.dwUserID);
                bindPhone->Open(this, "绑定手机", WIN_SIZE/2, 100, 0);
            });
        }
        else if (HC_GP_BANK_TRANSFER_PASSWORD_ERROR == bHandleCode)
        {
            gamePrompt->setOkayCallBack([this](){
                if (_delegate) {
                    _delegate->setOpenBankFunc();
                    close();
                }
            });
        }
        
        
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
					string str = data->items[i].dwUserID == 6666 ? GetText("SYSTEM") : to_string(data->items[i].dwUserID);

                    Color3B color = FONT_CLOLOR_YELLOW_V66;
                    
					auto meIDText = dynamic_cast<Text *>(node->getChildByName("Text_id"));
					//meIDText->setColor(color);
					meIDText->setString(str);

					auto targetIdText = dynamic_cast<Text *>(node->getChildByName("Text_targetId"));
					//targetIdText->setColor(color);
					targetIdText->setString(to_string(data->items[i].destUserID));

					auto moneyText = dynamic_cast<Text *>(node->getChildByName("Text_money"));
					//moneyText->setColor(color);
					moneyText->setString(Tools::goldDisplayByType(data->items[i].i64Money));
					
					auto beizhuText = dynamic_cast<Text *>(node->getChildByName("Text_beizhu"));
					//beizhuText->setColor(color);
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
					//timeText->setColor(color);
					timeText->setString(time);

					auto layout = Layout::create();
					layout->setContentSize(Size(820.0, 52));
                    layout->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                    node->setAnchorPoint(Vec2(0.5,0));
                    node->setPosition(layout->getContentSize()/2);
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

void GameBankV66::starTime(float dt) {
	_time += 1;
}

char * GameBankV66::Gettime(unsigned long nSeconds) {
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

void GameBankV66::editBoxTextChanged(ui::EditBox* editBox)
{
	if (editBox == nullptr) return;
}

void GameBankV66::editBoxEditingDidEnd(ui::EditBox * editBox)
{
	if (editBox == nullptr) return;

	switch (editBox->getTag())
	{
	case give_money: 
	{
		scheduleMoneyCallBack();
	}
	break;
	case give_userId:
	{
		scheduleNameCallBack();
	}
	break;
	case input_money:
	{
		if (editBox == _saveView.moneyField) 
		{
			_saveView.llMoneyAmount = atof(_saveView.moneyField->getString().c_str()) * PlatformConfig::getInstance()->getGoldDisplayMultiple();
		}
	}
	break;
	default:
		break;
	}
}

bool GameBankV66::bankerEventSelector(HNSocketMessage* socketMessage) {
	UINT bAssistantID = socketMessage->messageHead.bAssistantID;
	UINT bHandleCode = socketMessage->messageHead.bHandleCode;
	// 打开银行
	if (ASS_GP_BANK_OPEN == bAssistantID) 
	{
		switch (bHandleCode) 
		{
		case HC_GP_BANK_OPEN_SUC: 
		{							  
			//1打开钱柜成功
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
		case HC_GP_BANK_OPEN_ERR:
		{ //2//打开钱柜错误
			 _bankOpen = false;
		}
		break;
		case HC_GP_BANK_OPEN_ERR_PSW:
		{
			//3//打开钱框错误
		   _bankOpen = false;
		}
			break;
		default:
			break;
		}
	}
	// 修改密码
	else if (ASS_GP_BANK_CHPWD == bAssistantID) 
	{
		switch (bHandleCode) {
		case HC_GP_BANK_CHPWD_SUC: { //					1			//修改成功
									   GamePromptLayer::create()->showPrompt(GetText("TIPS15"));
									   _modifocationView.curpass->setText("");
									   _modifocationView.pass->setText("");
									   _modifocationView.yanZhengMa->setText("");
            
            UserDefault::getInstance()->setBoolForKey("IS_OPEN_BANK",false);
            if (_delegate) {
                _delegate->setOpenBankFunc();
                close();
            }
		}
			break;
		case HC_GP_BANK_CHPWD_ERR_PWD: { 
			//2	//修改失败，密码错误
			GamePromptLayer::create()->showPrompt(GetText("TIPS16"));
		}
			break;
		default:
			break;
		}
	}
	// 存取钱
	else if (ASS_GP_BANK_CHECK == bAssistantID)
	{
		switch (bHandleCode) 
		{
		case HC_GP_BANK_CHECK_SUC:
		{ 
			//					0			//操作成功
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
				//UserDefault::getInstance()->setStringForKey("bankTransactionPassword", _saveView.mima->getString());
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
				//UserDefault::getInstance()->setStringForKey("bankTransactionPassword", _saveView.mima->getString());
				//_saveView.allButton->setSelected(false);
				_saveView.saveButton->setEnabled(true);
				promptInfo = GetText("TIPS18");
			}

			char buffer[128];
			sprintf(buffer, "%lld", _walletMoney);
			if (nullptr != _saveView.bagMoneyText) {
				_saveView.bagMoneyText->setString(Tools::goldDisplayByType(_walletMoney));
			}

			sprintf(buffer, "%lld", _bankMoney);
			if (nullptr != _saveView.bankMoneyText) {
				_saveView.bankMoneyText->setString(Tools::goldDisplayByType(_bankMoney));
			}

			_saveView.moneyField->setText("");
			_saveView.llMoneyAmount = 0;
			GamePromptLayer::create()->showPrompt(promptInfo.c_str());

			_saveView.saveButton->setTouchEnabled(true);
			_saveView.takeButton->setTouchEnabled(true);
            
		}
			break;
		case HC_GP_BANK_CHECK_NO_USER: 
		{ 
			//				1			//没有找到用户
			GamePromptLayer::create()->showPrompt(GetText("TIPS19"));
		}
			break;
		case HC_GP_BANK_CHECK_ERR_PWD:
		{ 
			//				2			//错误密码
			GamePromptLayer::create()->showPrompt(GetText("TIPS20"));
            if (_delegate) {
                _delegate->setOpenBankFunc();
                close();
            }
		}
			break;
		case HC_GP_BANK_CHECK_NOT_MONEY: 
		{ 
			//				4			//余额不足
			GamePromptLayer::create()->showPrompt(GetText("TIPS21"));
		}
			break;
		default:
			break;
		}
	}
	return true;
}
