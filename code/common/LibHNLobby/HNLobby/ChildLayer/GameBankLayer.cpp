#include "GameBankLayer.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/PlatformConfig.h"
#include "HNLobby/FontConfig.h"
#include "HNMarketExport.h"
#include <string>
#include "../BankGivePrompt.h"
#include "Sound/HNAudioEngine.h"
#include "HNCommon/HNConverCode.h"
#include "../globel.h"
#include "../CommonMacro.h"
#include "Tool/Tools.h"

//static const char* BANKER_JSON_PATH		= "csb/platform/bankUi/bankNode.csb";
//static const char* BANKER_TITLE_CSB = "csb/platform/bankUi/bankTitleNode.csb";
static const char* BANKER_BUTTON_CSB = "csb/platform/newbankUi/button_node.csb";
static const char* BANKER_SAVE_CSB = "csb/platform/newbankUi/save_node.csb";
static const char* BANKER_GIVE_CSB = "csb/platform/newbankUi/give_node.csb";
static const char* BANKER_MODIFICATION_CSB = "csb/platform/newbankUi/modification_node.csb";
static const char* BANKER_RECORD_CSB = "csb/platform/newbankUi/record_node.csb";
static const char* BANKER_RECORD_TEXT_CSB = "csb/platform/newbankUi/record_text.csb";

//static const char* CLOSE_N				= "platform/close_n.png";
//static const char* CLOSE_D				= "platform/close_d.png";

static const char* BANKER_BG = "platform/hallBg.png";
static const char* BANKER_TITLE = "platform/newbankUi/bank.png";
static const char* CENTER_KUANG = "platform/newbankUi/1.png";

//static const char* BANK_TITLE			= "platform/bankUi/bankTitle.png";
static const char* CLOSE_MENU_N         = "platform/lobbyUi/PlatformUi_TOP/return.png";                         //closeMenu
//static const char* SELF_FRAME	    = "platform/store/common_opaque.png";

//static const char* LEAF_LEFT	    = "platform/subViewBambooLeft.png";
//static const char* LEAF_RIGHT	    = "platform/subViewBambooRight.png";
//static const char* STAR_BIG	        = "platform/bankUi/bankUiRes/star1.png";
//static const char* STAR_SMALL	    = "platform/bankUi/bankUiRes/star2.png";

#define WINSIZE Director::getInstance()->getWinSize()
#define JSON_RESOVE(obj, key) (obj.HasMember(key) && !obj[key].IsNull())

GameBankLayer* GameBankLayer::createGameBank() {
    GameBankLayer *pRet = new GameBankLayer();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return pRet;
}

GameBankLayer::GameBankLayer()
    :_bankMoney(0)
    , _recordTag(0)
    , _walletMoney(0)
    , _btnRoot(nullptr)
    , _childViewNode(nullptr)
    , _loadingLayer(nullptr)
    , _isClose(true)
    , _isOpenSaveView(true)
    , _idxJIlu(1)
    , _curpage(0)
    , _toutleRecord(0) {

    _vecNormal.clear();
    _vecPrass.clear();
    _vecButton.clear();
    _vecRecord.clear();

    _giveView.oldGiveId = 0;
    _giveView.oldGiveMoney = 0;
    _time = 61;
    _beginPage = 0; //  遍历数据 点击翻页默认归为 0-9号 点击右键 +10 左键 -10
    _endPage = 9;

    memset(&_structBtn, 0x0, sizeof(_structBtn));
    memset(&_giveView, 0x0, sizeof(_giveView));
    memset(&_modifocationView, 0x0, sizeof(_modifocationView));
    memset(&_recordView, 0x0, sizeof(_recordView));
}

GameBankLayer::~GameBankLayer() {
    _vecNormal.clear();
    _vecPrass.clear();
    _vecButton.clear();
    _vecRecord.clear();
    PlatformLogic()->removeEventSelector(MDM_GP_BANK, ASS_GP_BANK_OPEN);
    PlatformLogic()->removeEventSelector(MDM_GP_BANK, ASS_GP_BANK_CHPWD);
    PlatformLogic()->removeEventSelector(MDM_GP_BANK, ASS_GP_BANK_CHECK);
    PlatformLogic()->removeEventSelector(MDM_GP_BANK, ASS_GP_BANK_TRANSFER);
}

void GameBankLayer::openBank(Node* parent, Vec2 vec, int zorder, int tag) {
    CCAssert(nullptr != parent, "parent is nullptr");
    this->setPosition(vec);
    parent->addChild(this, zorder, tag);
    //parent->runAction(EaseSineOut::create(MoveBy::create(0.2f, Vec2(-winSize.width, 0))));
    parent->runAction(MoveBy::create(0.3f, Vec2(-WIN_SIZE.width, 0)));
}

void GameBankLayer::closeBank() {
    if (_isClose) {
        HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
        auto winSize = Director::getInstance()->getWinSize();
        this->getParent()->runAction(Sequence::create(MoveBy::create(0.3f, Vec2(winSize.width, 0)),
        CCCallFunc::create([&]() {
            this->removeFromParent();
        }), nullptr));
        _isClose = false;
    }
}

bool GameBankLayer::init() {
    if (!HNLayer::init()) {
        return false;
    }
    setBackGroundImage(BANKER_BG);

    Size winSize = Director::getInstance()->getWinSize();

    auto title = Sprite::create(BANKER_TITLE);
    title->setPosition(winSize.width / 2, winSize.height - title->getContentSize().height / 2 +10);
    this->addChild(title);

    auto kuang = Sprite::create(CENTER_KUANG);
    auto kuangSIze = kuang->getContentSize();
    _centerKuang = kuang;
    kuang->setPosition(winSize.width / 2,winSize.height * 0.48f);
    this->addChild(kuang);

    // 这个节点做子页面的父节点管理
    _childViewNode = Node::create();
    _childViewNode->setPosition(Vec2(kuang->getPositionX() - kuangSIze.width*0.4f, kuang->getPositionY() - kuangSIze.height*0.45f));
    this->addChild(_childViewNode);

    loadButton();

    //默认子界面
    addDefaultView();
    _oldViewTag = btn_save;

    // 关闭按钮
    auto closeBtn = Button::create(CLOSE_MENU_N,CLOSE_MENU_N);
    closeBtn->setShowEffect(false);
    closeBtn->setPosition(Vec2(winSize.width * 0.95f, winSize.height * 0.92f ));
    closeBtn->setPosition(Vec2(WINSIZE.width - closeBtn->getContentSize().width*0.55, WINSIZE.height - closeBtn->getContentSize().height*0.55));

    addChild(closeBtn,2);
    closeBtn->addTouchEventListener(CC_CALLBACK_2(GameBankLayer::closeEvent,this));

    //打开银行
    PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_OPEN, nullptr, 0, HN_SOCKET_CALLBACK(GameBankLayer::bankerEventSelector, this));
    return true;

}
// 初始化按钮控件
void GameBankLayer::loadButton() {
    _btnRoot = CSLoader::createNode(BANKER_BUTTON_CSB);
    _btnRoot->setPosition(Vec2( _centerKuang->getContentSize().width*0.05,_centerKuang->getContentSize().height*0.85-5));
    _centerKuang->addChild(_btnRoot);

    auto parent = _btnRoot->getChildren();
    int i = 1000;
    int idx = 0;
    for (auto child : parent) {
        auto btn = static_cast<Button *>(child->getChildByName("btn"));
        btn->setShowEffect(false);
        btn->setTag(i++);
        btn->addTouchEventListener(CC_CALLBACK_2(GameBankLayer::buttonCallBack, this));
        _vecButton.push_back(btn);

        auto normal = static_cast<Sprite *>(child->getChildByName("normal"));
        _vecNormal.push_back(normal);

        auto prass = static_cast<Sprite *>(child->getChildByName("prass"));
        _vecPrass.push_back(prass);

    }
}
//进入银行默认子页
void GameBankLayer::addDefaultView() {
    createSaveView(_childViewNode);
}

//存取
void GameBankLayer::createSaveView(Node* parent) {
    auto root = CSLoader::createNode(BANKER_SAVE_CSB);
    root->setPositionY(-15);
    parent->addChild(root);

    _oldViewTag = btn_save;
    _saveView.isAllSaveTouch = false;
    _saveView.isAllTakeTouch = false;

    _saveView.bankMoney = static_cast<Text*>(root->getChildByName("bankMoney"));
    //_saveView.bankMoney->setString(Tools::fourSeparateMoney(PlatformLogic()->loginResult.i64Bank));
    _saveView.gameMoney = static_cast<Text*>(root->getChildByName("gameMoney"));
    //_saveView.bankMoney->setString((Tools::fourSeparateMoney(PlatformLogic()->loginResult.i64Money)));

    _saveView.allSaveSpr = static_cast<ImageView*>(root->getChildByName("allSaveSpr"));
    _saveView.allSaveSpr->setTouchEnabled(true);
    _saveView.allSaveSpr->addTouchEventListener(CC_CALLBACK_2(GameBankLayer::imageVIewCallBack, this));

    _saveView.allTakeSpr = static_cast<ImageView*>(root->getChildByName("allTakeSpr"));
    _saveView.allTakeSpr->setTouchEnabled(true);
    _saveView.allTakeSpr->addTouchEventListener(CC_CALLBACK_2(GameBankLayer::imageVIewCallBack, this));

    auto moneyField = static_cast<TextField*>(root->getChildByName("money"));
    moneyField->setVisible(false);
    _saveView.moneyField = HNEditBox::createEditBox(moneyField, this);
    _saveView.moneyField->setInput(HNEditBox::InputMode::PHONE_NUMBER);

    auto mima = static_cast<TextField*>(root->getChildByName("mima"));
    mima->setVisible(false);
    _saveView.mima = HNEditBox::createEditBox(mima, this);
    _saveView.mima->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    _saveView.mima->setInput(HNEditBox::InputMode::SINGLE_LINE);
    std::string localMima = UserDefault::getInstance()->getStringForKey("bankTransactionPassword");  //登陆第一次交易成功时记录输入的交易密码 key值为 "bankTransactionPassword"
    if (localMima != "") {
        _saveView.mima->setString(localMima);
    }

    _saveView.allButton = static_cast<CheckBox*>(root->getChildByName("CheckBox"));
    _saveView.allButton->addEventListener(CC_CALLBACK_2(GameBankLayer::chackBoxCallBack,this));

    _saveView.quButton = static_cast<CheckBox*>(root->getChildByName("CheckBoxTake"));
    _saveView.quButton->addEventListener(CC_CALLBACK_2(GameBankLayer::chackBoxCallBack, this));

    _saveView.saveButton = static_cast<Button*>(root->getChildByName("saveBtn"));
    _saveView.saveButton->setTag(btn_cun);
    _saveView.saveButton->addTouchEventListener(CC_CALLBACK_2(GameBankLayer::operatorUIEventCallBack, this));

    _saveView.takeButton = static_cast<Button*>(root->getChildByName("takeBtn"));
    _saveView.takeButton->setTag(btn_take);
    _saveView.takeButton->addTouchEventListener(CC_CALLBACK_2(GameBankLayer::operatorUIEventCallBack, this));

    PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_OPEN, nullptr, 0, HN_SOCKET_CALLBACK(GameBankLayer::bankerEventSelector, this));
    _isOpenSaveView = true;
}

void GameBankLayer::reqUserNameById(std::string id) {
    std::string URL = std::string("http://") + getWebServerUrl() + std::string("/Public/AuditConfig.ashx?");
    std::string str("action=GetUserNameByUserID&key=");
    str = str + id;

    HttpRequest* request = new (std::nothrow) HttpRequest();
    request->setUrl(URL.c_str());
    request->setRequestType(HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(GameBankLayer::setTargetUserNameById, this));
    request->setRequestData(str.c_str(), str.size());
    HttpClient::getInstance()->sendImmediate(request);

    request->release();
}

void GameBankLayer::setTargetUserNameById(network::HttpClient* client, network::HttpResponse* response) {
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

//赠送金币子界面
void GameBankLayer::createGiveView(Node* parent) {
    _oldViewTag = btn_give;

    auto root = CSLoader::createNode(BANKER_GIVE_CSB);
    root->setPositionY(-15);
    parent->addChild(root);

    _giveView.gameGlob = static_cast<Text*>(root->getChildByName("gamemoney"));
    _giveView.gameGlob->setString(Tools::fourSeparateMoney(PlatformLogic()->loginResult.i64Bank));
    _giveView.targetName = static_cast<Text*>(root->getChildByName("targetName"));
    _giveView.giveGlob = static_cast<Text*>(root->getChildByName("givemoney"));

    _giveView.modification = static_cast<Button*>(root->getChildByName("btn"));
    _giveView.modification->addTouchEventListener(CC_CALLBACK_2(GameBankLayer::forwardUIEventCallBack, this));


    auto mima = static_cast<TextField*>(root->getChildByName("mima"));
    mima->setVisible(false);
    _giveView.mima = HNEditBox::createEditBox(mima, this);
    _giveView.mima->setInput(HNEditBox::InputMode::PHONE_NUMBER);
    _giveView.mima->setInputFlag(ui::EditBox::InputFlag::PASSWORD);

    auto giveMoney = static_cast<TextField*>(root->getChildByName("money"));
    giveMoney->setVisible(false);
    _giveView.giveMoney = HNEditBox::createEditBox(giveMoney, this);
    _giveView.giveMoney->setInput(ui::EditBox::InputMode::PHONE_NUMBER);
    _giveView.giveMoney->setTag(give_money);
    editBoxEditingDidEnd(_giveView.giveMoney);

    auto userId = static_cast<TextField*>(root->getChildByName("id"));
    userId->setVisible(false);
    _giveView.userId = HNEditBox::createEditBox(userId, this);
    _giveView.userId->setInput(ui::EditBox::InputMode::SINGLE_LINE);
    _giveView.userId->setTag(give_userId);
    editBoxEditingDidEnd(_giveView.userId);

    _walletMoney = PlatformLogic()->getInstance()->loginResult.i64Bank;
}

//赠送记录查看
void GameBankLayer::createRecordView(Node* parent) {
    _idxJIlu = 1;
    _oldViewTag = btn_record;
    auto root = CSLoader::createNode(BANKER_RECORD_CSB);

    auto listview = static_cast<ListView*>(root->getChildByName("ListView"));
    auto bg = static_cast<Sprite*>(root->getChildByName("bg"));
    listview->setGravity(cocos2d::ui::ListView::Gravity::BOTTOM);
    listview->setItemsMargin(10);
    auto size = listview->getContentSize();
    root->setPosition(Vec2(0, _centerKuang->getContentSize().height*0.5f - size.height *0.5f - bg->getContentSize().height));
    _childViewNode->addChild(root);


    _recordView.leftBtn = static_cast<Button*>(root->getChildByName("leftBtn"));
    _recordView.leftBtn->setTag(page_leftBtn);
    _recordView.leftBtn->addTouchEventListener(CC_CALLBACK_2(GameBankLayer::pageTurnCallBack, this));

    _recordView.rightBtn = static_cast<Button*>(root->getChildByName("rightBtn"));
    _recordView.rightBtn->setTag(page_rightBtn);
    _recordView.rightBtn->addTouchEventListener(CC_CALLBACK_2(GameBankLayer::pageTurnCallBack, this));

    _recordView.listView = static_cast<ListView*>(root->getChildByName("ListView"));
    _recordView.listView->addTouchEventListener(CC_CALLBACK_2(GameBankLayer::listViewItemCallBack, this));

    TMSG_GP_BankTranRecordReq data;
    data.dwUserID = PlatformLogic()->loginResult.dwUserID;
    data.pageSize = 10;
    data.curPage = _idxJIlu;
    data.reqType = 0;
    PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_TRANS_RECORD, &data, sizeof(data), HN_SOCKET_CALLBACK(GameBankLayer::bankerForwardSelector, this));
    LoadingLayer::createLoading(this, gFontConfig_22, GetText("LODING_REQUEST_ROOMLIST"), "platform/loading.png");
}
//修改密码子界面
void GameBankLayer::createModificationView(Node* parent) {
    _oldViewTag = btn_modification;
    auto root = CSLoader::createNode(BANKER_MODIFICATION_CSB);
    parent->addChild(root);

    _modifocationView.primitive = static_cast<Text*>(root->getChildByName("curpassword"));
    _modifocationView.giveBtn = static_cast<Button*>(root->getChildByName("btn"));
    _modifocationView.giveBtn->addTouchEventListener(CC_CALLBACK_2(GameBankLayer::userDataUIEventCallBack, this));

    auto cur = static_cast<TextField*>(root->getChildByName("cur"));
    cur->setVisible(false);
    _modifocationView.curpass = HNEditBox::createEditBox(cur, this);
    _modifocationView.curpass->setInput(HNEditBox::InputMode::PHONE_NUMBER);

    auto pass = static_cast<TextField*>(root->getChildByName("new"));
    pass->setVisible(false);
    _modifocationView.pass = HNEditBox::createEditBox(pass, this);
    _modifocationView.pass->setPasswordEnabled(false);
    _modifocationView.pass->setInput(HNEditBox::InputMode::PHONE_NUMBER);

    auto yanZhengMa = static_cast<TextField*>(root->getChildByName("yanzheng"));
    yanZhengMa->setVisible(false);
    _modifocationView.pass->setPasswordEnabled(false);
    _modifocationView.yanZhengMa = HNEditBox::createEditBox(yanZhengMa, this);
    _modifocationView.yanZhengMa->setInput(HNEditBox::InputMode::PHONE_NUMBER);
}

void GameBankLayer::imageVIewCallBack( Ref *pSender, Widget::TouchEventType type ) {
    auto image = static_cast<ImageView*>(pSender);
    switch (type) {
    case Widget::TouchEventType::ENDED: {
        if (image->getName() == "allSaveSpr") {
            if (!_saveView.isAllSaveTouch) {
                _saveView.moneyField->setString(to_string(PlatformLogic()->loginResult.i64Money));
                _saveView.allButton->setSelected(true);
                _saveView.quButton->setSelected(false);
                _saveView.isAllSaveTouch = true;
                _saveView.isAllTakeTouch = false;
            } else {
                _saveView.moneyField->setString("");
                _saveView.allButton->setSelected(false);
                _saveView.isAllSaveTouch = false;
            }
        } else if (image->getName() == "allTakeSpr") {
            if (!_saveView.isAllTakeTouch) {
                _saveView.moneyField->setString(to_string(PlatformLogic()->loginResult.i64Bank));
                _saveView.quButton->setSelected(true);
                _saveView.allButton->setSelected(false);
                _saveView.isAllSaveTouch = false;
                _saveView.isAllTakeTouch = true;
            } else {
                _saveView.moneyField->setString("");
                _saveView.quButton->setSelected(false);
                _saveView.isAllTakeTouch = false;
            }
        }
        break;
    }
    default:
        break;
    }
}

void GameBankLayer::listViewItemCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
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
            PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_TRANS_RECORD, &data, sizeof(data), HN_SOCKET_CALLBACK(GameBankLayer::bankerForwardSelector, this));
            LoadingLayer::createLoading(this, gFontConfig_22, GetText("LODING_REQUEST_ROOMLIST"), "platform/loading.png");
        }
    }
    break;
    default:
        break;
    }
}

//点击效果
void GameBankLayer::touchEffect(UINT tag) {
    int idx = 0;
    for (auto child : _vecButton) {
        if (child->getTag() == tag) {
            _vecPrass.at(idx)->setVisible(true);
            _vecNormal.at(idx)->setVisible(false);
        } else {
            _vecPrass.at(idx)->setVisible(false);
            _vecNormal.at(idx)->setVisible(true);
        }
        idx++;
    }
}
void GameBankLayer::scheduleNameCallBack() {
    if (_giveView.userId) {
        auto str = _giveView.userId->getString();
        reqUserNameById(str);
    }
}

void GameBankLayer::chackBoxCallBack(Ref* sender, CheckBox::EventType type) {
    auto box = static_cast<CheckBox*>(sender);
    switch (type) {
    case CheckBox::EventType::SELECTED:
        if (box->getName() == "CheckBox") {
            _saveView.moneyField->setString(to_string(PlatformLogic()->loginResult.i64Money));
            _saveView.quButton->setSelected(false);
            _saveView.isAllTakeTouch = false;
        } else {
            _saveView.moneyField->setString(to_string(PlatformLogic()->loginResult.i64Bank));
            _saveView.allButton->setSelected(false);
            _saveView.isAllSaveTouch = false;
        }
        break;
    case CheckBox::EventType::UNSELECTED:
        if (box->getName() == "CheckBox") {
            _saveView.moneyField->setString("");
        } else {
            _saveView.moneyField->setString("");
        }
        break;
    default:
        break;
    }
}

void GameBankLayer::scheduleMoneyCallBack() {
    if (_giveView.gameGlob) {
        auto Myoney = atoll(_giveView.gameGlob->getString().c_str());
        auto GiveMoney = atoll(_giveView.giveMoney->getString().c_str());

        transition(_giveView.giveGlob, GiveMoney);
    }
}
// 转换字符串
void GameBankLayer::transition(Text * text ,LLONG money) {

    char str[30] = { 0 };
    if (money >= 100000000) {
        LLONG yiYu = money % 100000000;
        INT yi = money / 100000000;
        if (yiYu >= 10000) {
            LLONG wanYu = yiYu % 10000;
            UINT wan = yiYu / 10000;
            sprintf(str, "%d%s%d%s%lld", yi, GetText("TIPS23"), wan, GetText("TIPS22"), wanYu);
        } else {
            if (yiYu != 0) {
                sprintf(str, "%d%s%lld%s", yi, GetText("TIPS23"), yiYu, GetText("TIPS24"));
            } else {
                sprintf(str, "%d%s", yi, GetText("TIPS23"));
            }
        }
    } else {
        if (money >= 10000) {
            long wanYu = money % 10000;
            unsigned int wan = money / 10000;
            if (wanYu != 0) {
                sprintf(str, "%d%s%d%s", wan, GetText("TIPS22"), wanYu, GetText("TIPS24"));
            } else {
                sprintf(str, "%d%s", wan, GetText("TIPS22"));
            }
        } else {
            sprintf(str, "%d",money);
        }
    }
    if (money == 0) {
        text->setString("");
    } else {
        text->setString(str);
    }
}
//创建子界面
void GameBankLayer::buttonCallBack(Ref* pSender, Widget::TouchEventType touchtype) {
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
// 添加ListView内容
void GameBankLayer::addListViewItem(UINT begin, UINT end) {
    for (int i = begin; i <= end; i++) {
        if (_vecRecord.at(i).year != 0) {
            auto node = CSLoader::createNode(BANKER_RECORD_TEXT_CSB);
            static_cast<Text *>(node->getChildByName("meId"))->setString(to_string(PlatformLogic()->getInstance()->loginResult.dwUserID));
            static_cast<Text *>(node->getChildByName("targetId"))->setString(to_string(_vecRecord.at(i).destUserID));
            static_cast<Text *>(node->getChildByName("money"))->setString(Tools::fourSeparateMoney((_vecRecord.at(i).i64Money)));

            char time[50] = { 0 };
            sprintf(time, "%d-%d-%d %d:%d:%d", _vecRecord.at(i).year, _vecRecord.at(i).month, _vecRecord.at(i).day, _vecRecord.at(i).hour,
                    _vecRecord.at(i).minute, _vecRecord.at(i).second);
            static_cast<Text *>(node->getChildByName("time"))->setString(time);

            auto layout = Layout::create();
            layout->setContentSize(Size(815.0, 20));
            node->setPositionX(-(100 / 1280 * WINSIZE.width));
            layout->addChild(node);
            _recordView.listView->addChild(layout);
            _recordView.listView->forceDoLayout();
        }
    }
    _recordView.listView->forceDoLayout();
}

// 翻页按钮回调
void GameBankLayer::pageTurnCallBack(Ref* pSender, Widget::TouchEventType touchtype) {
    if (touchtype != Widget::TouchEventType::ENDED) return;
    auto btn = static_cast<Button *>(pSender);
    switch (btn->getTag()) {
    case page_leftBtn: {
        _beginPage -= 9;
        _endPage -= 9;
        if (_beginPage - 9 <= 0) _beginPage = 0;
        if (_endPage - 9 <= 9) _endPage = 9;
        _recordView.listView->removeAllItems();
        addListViewItem(_beginPage, _endPage);
    }
    break;
    case page_rightBtn: {
        _beginPage += 9;
        _endPage += 9;
        if (_beginPage + 9 >= _vecRecord.size() - 9 - 1) _beginPage = 0; // 最多100条信息 99最大按实际最大值遍历
        if (_endPage + 9 >= _vecRecord.size() - 1 ) _endPage =9;
        _recordView.listView->removeAllItems();
        addListViewItem(_beginPage, _endPage);
    }
    break;
    default:
        break;
    }
}

void GameBankLayer::clearCurViewDat() {
    memset(&_structBtn, 0x0, sizeof(_structBtn));
    memset(&_giveView, 0x0, sizeof(_giveView));
    memset(&_modifocationView, 0x0, sizeof(_modifocationView));
    memset(&_recordView, 0x0, sizeof(_recordView));
}

void GameBankLayer::userDataUIEventCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (Widget::TouchEventType::ENDED != type) return;
    Button* psender = (Button*) pSender;
    if (psender == _modifocationView.giveBtn) {
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

            TMSG_GP_BankChPwd BankChPwd = {0};
            BankChPwd.user_id = PlatformLogic()->loginResult.dwUserID;
            strcpy(BankChPwd.MD5Pass_old, MD5_CTX::MD5String(oldPassword).c_str());
            strcpy(BankChPwd.MD5Pass_new, MD5_CTX::MD5String(newPassword).c_str());
            PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_CHPWD, &BankChPwd, sizeof(BankChPwd),HN_SOCKET_CALLBACK(GameBankLayer::bankerEventSelector, this));
        } while (0);
    }
}

void GameBankLayer::forwardUIEventCallBack(Ref* pSender, Widget::TouchEventType type) {

    if (Widget::TouchEventType::ENDED != type) return;
    Button* psender = (Button*) pSender;
    if (psender == _giveView.modification) {
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

                if (_giveView.userId->isEnabled()) {
                    GamePromptLayer::create()->showPrompt(GetText("TIPS7"));
                    break;
                }
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
            //str.append(GetText("TIPS25"));
            //str.append(GBKToUtf8(_giveView.targetName->getString().c_str()));
            //str.append(GBKToUtf8("赠送"));
            //str.append(forwardMoney.c_str());
            //str.append(GBKToUtf8("金币吗?"));

            //HNLOG_INFO("-----------%s", __String::createWithFormat(GBKToUtf8("你确定为%s赠送%s金币吗?"), _giveView.targetName->getString().c_str(), forwardMoney.c_str())->getCString());
            ////GBKToUtf8(StringUtils::format("你确定为%s赠送%s金币吗?", _giveView.targetName->getString().c_str(), forwardMoney.c_str()).c_str());

            ////std::string str = GetText("GIVE_MONEY_SURE");
            //HNLOG_INFO("<<<<<<<<<<%s",str.c_str());

            str = StringUtils::format(GetText("GIVE_MONEY_SURE"), _giveView.targetName->getString().c_str(), forwardMoney.c_str());
            auto tishi = GamePromptLayer::create();
            tishi->showPrompt(str);
            tishi->setOkayCallBack([&]() {

                if (_giveView.oldGiveId == atoi(_giveView.userId->getString().c_str()) && atoi(_giveView.giveMoney->getString().c_str()) == _giveView.oldGiveMoney && _time <= 60) {
                    GamePromptLayer::create()->showPrompt(GetText("TIPS10"));
                    return;
                } else {
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

                        TTFConfig ttConfig("platform/fonts/simhei.ttf", 24);
                        LoadingLayer::createLoading(this, ttConfig, "请稍后", "platform/loading.png");
                    }
                    _walletMoney -= bankTransfer.i64Money;
                    if (bankTransfer.i64Money <= 0) {
                        GamePromptLayer::create()->showPrompt(GetText("TIPS11"));
                        return;
                    }
                    PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_TRANSFER, &bankTransfer, sizeof(bankTransfer),
                                              HN_SOCKET_CALLBACK(GameBankLayer::bankerForwardSelector, this));
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

void GameBankLayer::operatorUIEventCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (Widget::TouchEventType::ENDED != type) return;
    Button* psender = (Button*) pSender;

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

                TMSG_GP_BankCheck BankCheck = {0};
                BankCheck.operate_type = 2;
                BankCheck.user_id = PlatformLogic()->loginResult.dwUserID;
                BankCheck.game_id = 0;
                BankCheck.money = atoll(strMoney.c_str());
                if(BankCheck.money <= 0) {
                    GamePromptLayer::create()->showPrompt(GetText("TIPS11"));
                    break;
                }
                PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_CHECK, &BankCheck, sizeof(BankCheck),
                                          HN_SOCKET_CALLBACK(GameBankLayer::bankerEventSelector, this));
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
                TMSG_GP_BankCheck BankCheck = {0};
                BankCheck.operate_type = 1;
                BankCheck.user_id = PlatformLogic()->loginResult.dwUserID;
                BankCheck.game_id = 0;
                BankCheck.money = atoll(strMoney.c_str());
                strcpy(BankCheck.szMD5Pass, MD5_CTX::MD5String(password).c_str());
                if(BankCheck.money <= 0) {
                    GamePromptLayer::create()->showPrompt(GetText("TIPS11"));
                    break;
                }
                PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_BANK_CHECK, &BankCheck, sizeof(BankCheck),
                                          HN_SOCKET_CALLBACK(GameBankLayer::bankerEventSelector, this));
            } while (0);
        } else {
            GamePromptLayer::create()->showPrompt(GetText("TIPS13"));
        }
    }
}

void GameBankLayer::closeEvent(Ref* pSender, Widget::TouchEventType type) {
    Button* selectedBtn = (Button*)pSender;
    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::BEGAN:
        selectedBtn->setColor(Color3B(170, 170, 170));
        break;
    case cocos2d::ui::Widget::TouchEventType::ENDED:
        selectedBtn->setColor(Color3B(255, 255, 255));
        closeBank();
        break;
    case cocos2d::ui::Widget::TouchEventType::CANCELED:
        selectedBtn->setColor(Color3B(255, 255, 255));
        break;
    default:
        break;

    }
}

bool GameBankLayer::bankerForwardSelector(HNSocketMessage* socketMessage) {
    UINT bAssistantID = socketMessage->messageHead.bAssistantID;
    UINT bHandleCode = socketMessage->messageHead.bHandleCode;

    std::string errText("");
    if (ASS_GP_BANK_TRANSFER == bAssistantID) {	//
        if (HC_GP_BANK_TRANSFER_ERROR == bHandleCode) {	//1		//转帐错误
            errText = GetText("HC_GP_BANK_TRANSFER_ERROR");
        } else if (HC_GP_BANK_TRANSFER_NO_DEST == bHandleCode) {	//3		//转帐目标不存在
            errText = GetText("HC_GP_BANK_TRANSFER_NO_DEST");
        } else if (HC_GP_BANK_TRANSFER_PASSWORD_ERROR == bHandleCode) {	//4		//转帐密码错误
            errText = GetText("HC_GP_BANK_TRANSFER_PASSWORD_ERROR");
        } else if (HC_GP_BANK_TRANSFER_TOTAL_LESS == bHandleCode) {	//5		//银行总额太小不够资格
            errText = GetText("HC_GP_BANK_TRANSFER_TOTAL_LESS");
        } else if (HC_GP_BANK_TRANSFER_TOO_LESS == bHandleCode) { //6		// 单笔转帐数目太少
            errText = GetText("HC_GP_BANK_TRANSFER_TOO_LESS");
        } else if (HC_GP_BANK_TRANSFER_MULTIPLE == bHandleCode) { //7		// 单笔转帐数目必须是某数的倍数
            errText = GetText("HC_GP_BANK_TRANSFER_MULTIPLE");
        } else if (HC_GP_BANK_TRANSFER_NOT_ENOUGH == bHandleCode) { //8		// 银行金额不够本次转帐
            errText = GetText("HC_GP_BANK_TRANSFER_NOT_ENOUGH");
        } else if (HC_GP_BANK_TRANSFER_TOO_MANY_TIME == bHandleCode) {	//9	// 当天转账的次数太多了
            errText = GetText("HC_GP_BANK_TRANSFER_TOO_MANY_TIME");
        } else if (HC_GP_BANK_TRANSFER_SET_REBATE_RES == bHandleCode || HC_GP_BANK_TRANSFER_UPDATE_USER_REBATE==bHandleCode) {	//9	// 返利信息
            return false;
        } else if (HC_GP_BANK_TRANSFER_Disable_SXF == bHandleCode) {
            errText = GetText("HC_GP_BANK_NOT_TARGET");
        }
        else if (HC_GP_BANK_TRANSFER_SUC == bHandleCode) {	//2		//转帐成功
            errText = GetText("HC_GP_BANK_TRANSFER_SUC");
            LoadingLayer::removeLoading(this);
            PlatformLogic()->getInstance()->loginResult.i64Bank = _walletMoney;
            _giveView.gameGlob->setString(Tools::fourSeparateMoney(_walletMoney));

            TMSG_GP_BankTransfer* data = (TMSG_GP_BankTransfer*)(socketMessage->object);

            char month[5] = { 0 };

            if (data->month < 10) {
                sprintf(month, "0%d", data->month);
            } else {
                sprintf(month, "%d", data->month);
            }

            char day[5] = { 0 };
            if (data->day < 10) {
                sprintf(day, "0%d", data->day);
            } else {
                sprintf(day, "%d", data->day);
            }

            char second[5] = { 0 };
            if (data->second < 10) {
                sprintf(second, "0%d", data->second);
            } else {
                sprintf(second, "%d", data->second);
            }

            char minute[5] = { 0 };
            if (data->minute < 10) {
                sprintf(minute, "0%d", data->minute);
            } else {
                sprintf(minute, "%d", data->minute);
            }

            char hour[5] = { 0 };
            if (data->hour < 10) {
                sprintf(hour, "0%d", data->hour);
            } else {
                sprintf(hour, "%d", data->hour);
            }
            char time[100] = { 0 };
            sprintf(time, "%d-%s-%s %s:%s:%s", data->year, month, day, hour, minute, second);

            auto bankPrompt = BankGivePrompt::create();
            bankPrompt->updateInfo(data->szDestNickName, data->i64Money, time, data->destUserID);
            this->addChild(bankPrompt, 10000000000);								 // 这个值就设置这么大
            _giveView.oldGiveId = data->destUserID;
            _giveView.oldGiveMoney = data->i64Money;
            _giveView.userId->setString("");
            _giveView.giveMoney->setString("");
            _giveView.mima->setString("");
            _giveView.targetName->setString("");
            _giveView.giveGlob->setString("");
            if (_time > 61) {
                _time = 0;
            }
            schedule(schedule_selector(GameBankLayer::starTime), 1.0f);

            return true;
        } else {
            int i = 0;
        }
        LoadingLayer::removeLoading(this);
        GamePromptLayer::create()->showPrompt(errText);
    } else if (ASS_GP_BANK_TRANS_RECORD == bAssistantID) {	// 转账查询
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
                        LoadingLayer::removeLoading(this);
                        return false;
                    }

                    auto node = CSLoader::createNode(BANKER_RECORD_TEXT_CSB);
                    string str = data->items[i].dwUserID == 8888 ? GetText("SYSTEM") : to_string(data->items[i].dwUserID);
                    static_cast<Text *>(node->getChildByName("meId"))->setString(str);
                    static_cast<Text *>(node->getChildByName("targetId"))->setString(to_string(data->items[i].destUserID));
                    static_cast<Text *>(node->getChildByName("money"))->setString(to_string(data->items[i].i64Money));
                    static_cast<Text *>(node->getChildByName("beizhu"))->setString(ForceGBKToUtf8(data->items[i].szRemark));
                    CCLOG(" hour = %d  min = %d  second = %d", data->items[i].hour, data->items[i].minute, data->items[i].second);
                    char month[5] = { 0 };

                    if (data->items[i].month < 10) {
                        sprintf(month, "0%d", data->items[i].month);
                    } else {
                        sprintf(month, "%d", data->items[i].month);
                    }

                    char day[5] = { 0 };
                    if (data->items[i].day < 10) {
                        sprintf(day, "0%d", data->items[i].day);
                    } else {
                        sprintf(day, "%d", data->items[i].day);
                    }

                    char second[5] = { 0 };
                    if (data->items[i].second < 10) {
                        sprintf(second, "0%d", data->items[i].second);
                    } else {
                        sprintf(second, "%d", data->items[i].second);
                    }

                    char minute[5] = { 0 };
                    if (data->items[i].minute < 10) {
                        sprintf(minute, "0%d", data->items[i].minute);
                    } else {
                        sprintf(minute, "%d", data->items[i].minute);
                    }

                    char hour[5] = { 0 };
                    if (data->items[i].hour < 10) {
                        sprintf(hour, "0%d", data->items[i].hour);
                    } else {
                        sprintf(hour, "%d", data->items[i].hour);
                    }

                    string time;
                    time = month;
                    time = StringUtils::format("%d-%s-%s %s:%s:%s", data->items[i].year, month, day, hour, minute, second);

                    static_cast<Text *>(node->getChildByName("time"))->setString(time);

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
            LoadingLayer::removeLoading(this);
        }
    }
    return true;
}

void GameBankLayer::starTime(float dt) {
    _time += 1;
}

char * GameBankLayer::Gettime(unsigned long nSeconds) {
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

void GameBankLayer::editBoxEditingDidEnd(ui::EditBox * editBox) {
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



bool GameBankLayer::bankerEventSelector(HNSocketMessage* socketMessage) {
    UINT bAssistantID = socketMessage->messageHead.bAssistantID;
    UINT bHandleCode = socketMessage->messageHead.bHandleCode;
    // 打开银行
    if (ASS_GP_BANK_OPEN == bAssistantID) {
        switch (bHandleCode) {
        case HC_GP_BANK_OPEN_SUC: { //					1		//打开钱柜成功
            if (!_isOpenSaveView)  break;
            // 广播过来
            CCAssert(sizeof (TMSG_GP_BankInfo) == socketMessage->objectSize, "TMSG_GP_BankInfo size is error.");
            if (sizeof (TMSG_GP_BankInfo) != socketMessage->objectSize) return true;

            TMSG_GP_BankInfo * bankInfo = reinterpret_cast<TMSG_GP_BankInfo*>(socketMessage->object);

            _bankMoney = bankInfo->i64Bank;

            _walletMoney = bankInfo->i64Wallet;

            PlatformLogic()->getInstance()->loginResult.i64Bank = bankInfo->i64Bank;
            PlatformLogic()->getInstance()->loginResult.i64Money = bankInfo->i64Wallet;

            char buffer[128];
            sprintf(buffer, "%lld", _walletMoney);
            if (_saveView.gameMoney != nullptr) {
                _saveView.gameMoney->setString(Tools::fourSeparateMoney(_walletMoney));
            }

            sprintf(buffer, "%lld", _bankMoney);
            if (_saveView.bankMoney != nullptr) {
                _saveView.bankMoney->setString(Tools::fourSeparateMoney(_bankMoney));
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
            CCAssert(sizeof (TMSG_GP_BankCheck) == socketMessage->objectSize, "TMSG_GP_BankCheck size is error.");
            if (sizeof (TMSG_GP_BankCheck) != socketMessage->objectSize) return true;

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
                _saveView.quButton->setSelected(false);
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
                _saveView.allButton->setSelected(false);
                promptInfo = GetText("TIPS18");
            }

            char buffer[128];
            sprintf(buffer, "%lld", _walletMoney);
            if (nullptr != _saveView.gameMoney) {
                _saveView.gameMoney->setString(Tools::fourSeparateMoney(_walletMoney));
            }

            sprintf(buffer, "%lld", _bankMoney);
            if (nullptr != _saveView.bankMoney) {
                _saveView.bankMoney->setString(Tools::fourSeparateMoney(_bankMoney));
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

//请求转账按钮是否显示
//void GameBankLayer::reqIsShow()
//{
//	std::string URL = std::string("http://") + getWebServerUrl() + std::string("/Public/AuditConfig.ashx?");
//	std::string str("action=GetConfigByKey&key=transfer");
//
//	HttpRequest* request = new (std::nothrow) HttpRequest();
//	request->setUrl(URL.c_str());
//	request->setRequestType(HttpRequest::Type::POST);
//	request->setResponseCallback(CC_CALLBACK_2(GameBankLayer::onHttpResponse, this));
//
//	request->setRequestData(str.c_str(), str.size());
//	HttpClient::getInstance()->sendImmediate(request);
//
//	request->release();
//}

// 获取转账按钮状态回调
//void GameBankLayer::onHttpResponse(network::HttpClient* client, network::HttpResponse* response)
//{
//	std::vector<char> recv = *response->getResponseData();
//	std::string data;
//	data.assign(recv.begin(), recv.end());
//
//	rapidjson::Document doc;
//	doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
//
//	if (doc.HasParseError() || !doc.IsObject() || !doc.HasMember("content") || !doc["content"].IsArray())
//	{
//		return;
//	}
//
//	// 解析
//	for (auto i = 0; i < doc["content"].Size(); i++)
//	{
//		if (!doc["content"][i].HasMember("AttrKey") || !doc["content"][i].HasMember("IosOpen") || !doc["content"][i].HasMember("AdrOpen"))
//		{
//			continue;
//		}
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//		bool open = doc["content"][i]["IosOpen"].GetBool();
//#else
//		bool open = doc["content"][i]["AdrOpen"].GetBool();
//#endif
//		if (!open)
//		{
//			_publicBtn.zhuanzhangBtn->setVisible(false);
//			_publicBtn.bankBtn->setPosition(_publicBtn.dataBtn->getPosition());
//			_publicBtn.dataBtn->setPosition(_publicBtn.zhuanzhangBtn->getPosition());
//		}
//	}
//}
