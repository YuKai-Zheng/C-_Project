#include "BJLNoticeV88.h"
#include "cocostudio/CocoStudio.h"
#include "HNLobby/FontConfig.h"
#include "HNLobby/v66/PlatformResV66.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;

namespace BJL {

static const int HEIGHT_HIDE = 122;    //提示框掩体的高度
static const char* NOTICE_BG_SRC  = "v66/BJL/control.png";

BJLGameNoticeV88::BJLGameNoticeV88()
    :_bg(nullptr) {

}

BJLGameNoticeV88::~BJLGameNoticeV88() {

}

BJLGameNoticeV88* BJLGameNoticeV88::create(cocos2d::Node* parent, std::string context, cocos2d::Node* shelter, std::string title, std::string fileName) {
	BJLGameNoticeV88* ptr = new BJLGameNoticeV88();
    if (nullptr != ptr && ptr->init(parent,context, shelter ,title,fileName)) {
        ptr->autorelease();
        return ptr;
    }
    return nullptr;
}

bool BJLGameNoticeV88::init(cocos2d::Node* parent, std::string& context, cocos2d::Node* shelter, std::string& title, std::string& fileName) {
    if (!HNLayer::init()) {
        return false;
    }

    _parent = parent;
    _shelter = shelter;
    HNSprite* sp = HNSprite::create(NOTICE_BG_SRC);
    sp->setRotation(180);
    Size size = sp->getContentSize();
    this->setIgnoreAnchorPointForPosition(false);
    this->setContentSize(size);

    sp->setAnchorPoint(Vec2(0.5f, 0.5f));
    sp->setPosition(140, size.height + 15);
    sp->setIgnoreAnchorPointForPosition(false);
    addChild(sp);
    _bg = sp;
    auto contextLable = Label::createWithSystemFont(context, FONT_TTF_FZCY, 22);
    contextLable->setAnchorPoint(Vec2(0.5f, 0.5f));
    contextLable->setPosition(Vec2(size.width / 2 -320, size.height / 2 +35));
    this->addChild(contextLable);
    return true;
}

void BJLGameNoticeV88::show() {
    auto winSize = Director::getInstance()->getWinSize();

    Vec2 end_position, start_position;
    if (nullptr != _shelter) {
        auto size = Size(_shelter->getContentSize().width, _shelter->getContentSize().height - 40);
        start_position = Vec2(size.width * 0.75, WIN_SIZE.height*0.5f);
        end_position = Vec2(start_position.x , start_position.y + _bg->getContentSize().height);

        this->setAnchorPoint(Vec2(0.5f, 1));
        this->setPosition(start_position);
        _shelter->addChild(this, -1);

        auto come = EaseIn::create(MoveTo::create(0.3f, end_position), 1.5f);
        auto leave = EaseOut::create(MoveTo::create(0.3f, start_position), 1.5f);
        auto seq = Sequence::create(come, DelayTime::create(3.0f), RemoveSelf::create(true), nullptr);
        this->runAction(seq);
    } else {
        return;
    }
}

bool BJLGameNoticeV88::loadUI(std::string& context, std::string& title, std::string& fileName) {
    return true;
}


}
