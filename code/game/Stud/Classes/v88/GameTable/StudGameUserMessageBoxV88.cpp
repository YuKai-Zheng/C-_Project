#include "StudGameUserMessageBoxV88.h"

#include <string>
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace ui;

namespace STUD {
GameUserMessageBoxV88* GameUserMessageBoxV88::createMessageBox(cocos2d::Node* parent, INT userID) {
    auto pRet = new GameUserMessageBoxV88();
    if (pRet && pRet->init(parent, userID)) {
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    pRet = nullptr;
    return pRet;
}

bool GameUserMessageBoxV88::init(cocos2d::Node* parent, INT userID) {
    if ( !HNDialogBase::init(parent)) return false;

    Size winSize = Director::getInstance()->getWinSize();

    //创建一个点击事件
    auto MyListener = EventListenerTouchOneByOne::create();
    MyListener->setSwallowTouches(true);//阻止触摸向下传递
    MyListener->onTouchBegan = [&](Touch* touch, Event* event) {
        Size s = this->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);
        if (rect.containsPoint(touch->getLocation())) { //判断触摸点是否在目标的范围内
            //if (_userDataBoxRect.containsPoint(touch->getLocation())) return true;
            hide();
            return true;
        } else
            return false;
    };
    //把点击监听添加到监听序列中,因为只创建了一个监听,一个监听只能绑定到一个对象上,所以其他对象要使用监听则克隆
    _eventDispatcher->addEventListenerWithSceneGraphPriority(MyListener, this);

    //玩家信息结构体
    const UserInfoStruct* userinfo = UserInfoModule()->findUser(userID);
    if (nullptr == userinfo) {
        return true;
    }

    auto infoNode = CSLoader::createNode("stud/player/userinfo.csb");
    auto infoOutline = (Layout*)infoNode->getChildByName("userinfo_panel");
    //auto infoOutline = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("stud/player/userinfo.json");
    addChild(infoNode);
    _userDataBoxRect = infoOutline->getBoundingBox();
    infoOutline -> setAnchorPoint(Vec2(0, 0));

    auto avata		= dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(infoOutline, "avata_img"));
    auto gender		= dynamic_cast<Button*>(ui::Helper::seekWidgetByName(infoOutline, "gender_btn"));
    auto nickname	= dynamic_cast<Text*>(ui::Helper::seekWidgetByName(infoOutline, "nickname_label"));
    auto alias		= dynamic_cast<Text*>(ui::Helper::seekWidgetByName(infoOutline, "alias_label"));
    auto wallet		= dynamic_cast<Text*>(ui::Helper::seekWidgetByName(infoOutline, "wallet_label"));
    auto bank		= dynamic_cast<Text*>(ui::Helper::seekWidgetByName(infoOutline, "bank_label"));
    auto playcount	= dynamic_cast<Text*>(ui::Helper::seekWidgetByName(infoOutline, "playcount_label"));
    auto winrate	= dynamic_cast<Text*>(ui::Helper::seekWidgetByName(infoOutline, "winrate_label"));

    // avata
    auto avatafile= userinfo->bBoy? MEN_AVATA_V88 : LADY_AVATA_V88;
    avata -> loadTexture(avatafile);

    // gender
    gender->setEnabled(false);
    gender->setBright(userinfo->bBoy);

    char buf[128];
    sprintf(buf, "昵称 %s", userinfo->nickName);
    nickname->setString(GBKToUtf8(buf));

    // alias
    sprintf(buf, "账号 %s", userinfo->szName);
    alias->setString(GBKToUtf8(buf));

    // wallet
    sprintf(buf, "钱包金币 %lld", userinfo->i64Money);
    wallet->setString(GBKToUtf8(buf));

    // bank
    sprintf(buf, "银行金币 %lld", userinfo->i64Bank);
    bank->setString(GBKToUtf8(buf));
    bank->setVisible(false);

    // playcount
    sprintf(buf, "牌局 %d", userinfo->uWinCount + userinfo->uLostCount + userinfo->uMidCount + userinfo->uCutCount);
    playcount->setString(GBKToUtf8(buf));

    // winrate
    if((userinfo->uWinCount + userinfo->uLostCount + userinfo->uMidCount + userinfo->uCutCount)>0) {
        sprintf(buf, "胜率 %.2f%%", 100.0f * userinfo->uWinCount/(userinfo->uWinCount + userinfo->uLostCount + userinfo->uMidCount + userinfo->uCutCount));
    }
    winrate->setString(GBKToUtf8(buf));

    return true;
}

}
