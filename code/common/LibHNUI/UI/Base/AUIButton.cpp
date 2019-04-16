#include "AUIButton.h"
#include "Tool/GraphicsUtils.h"
#include "Tool/WireframeOnOff.h"

namespace HN {
AUIButton::AUIButton():
    _drawRect(false) {
    //_title = nullptr;
    setDrawRect(_drawRect || BUTTON_ONOFF);
}

AUIButton::~AUIButton() {
}

AUIButton* AUIButton::create(const std::string &normalImage,
                             const std::string& selectedImage,
                             const std::string& disableImage,
                             TextureResType texType) {
    AUIButton *btn = new (std::nothrow) AUIButton;
    if (btn && btn->init(normalImage, selectedImage, disableImage, texType)) {
        btn->autorelease();
        return btn;
    }
    CC_SAFE_DELETE(btn);
    return nullptr;
}

bool AUIButton::init() {
    if (!Button::init()) {
        return false;
    }
    return true;
}

bool AUIButton::init(const std::string& normalImage,
                     const std::string& selectedImage ,
                     const std::string& disableImage ,
                     TextureResType texType ) {
    if (!Button::init(normalImage, selectedImage, disableImage, texType)) {
        return false;
    }
    return true;
}

void AUIButton::onPressStateChangedToNormal() {
    Button::onPressStateChangedToNormal();
    //this->setScale(_normalTextureScaleXInSize, _normalTextureScaleYInSize);
    //for (int i = 0; i < _children.size(); i++) {
    //    Node* node = _children.at(i);
    //    if (!(node->getTag() == 20 && isShowEffect)) {
    //        //node->setScale(_normalTextureScaleXInSize, _normalTextureScaleYInSize);
    //    }
    //}
}

void AUIButton::onPressStateChangedToPressed() {
    Button::onPressStateChangedToPressed();
    //this->setScale(_pressedTextureScaleXInSize + _zoomScale, _pressedTextureScaleYInSize + _zoomScale);
    //for (int i = 0; i < _children.size(); i++) {
    //    Node* node = _children.at(i);
    //    Action *zoomAction = ScaleTo::create(0.05f,
    //                                         _pressedTextureScaleXInSize + _zoomScale,
    //                                         _pressedTextureScaleYInSize + _zoomScale);
    //    if (!(node->getTag() == 20 && isShowEffect)) {
    //        node->runAction(zoomAction);
    //    }
    //}
}

void AUIButton::setTitleScal(float scal){
	auto titile = this->getChildByTag(99);
	if (titile) {
		titile->setScale(scal);
	}
}

void AUIButton::setGray(bool isGray) {
    if (isGray) {
        gray();
    } else {
        recovery();
    }
}

void AUIButton::gray() {
    GraphicsUtils::gray(_children);
    _buttonNormalRenderer->setState(Scale9Sprite::State::GRAY);
    _buttonClickedRenderer->setState(Scale9Sprite::State::GRAY);
    _buttonDisabledRenderer->setState(Scale9Sprite::State::GRAY);
}

void AUIButton::recovery() {
    GraphicsUtils::recovery(_children);
    _buttonNormalRenderer->setState(Scale9Sprite::State::NORMAL);
    _buttonClickedRenderer->setState(Scale9Sprite::State::NORMAL);
    _buttonDisabledRenderer->setState(Scale9Sprite::State::NORMAL);
}

void AUIButton::setTitle(const std::string &imageFileName) {
    //if (!getChildByTag(99)) {
    auto _title = Sprite::create(imageFileName);
    if (_title) {
        _title->setPosition(Vec2(getContentSize().width / 2, getContentSize().height / 2));
        _title->setTag(99);
        addChild(_title);
    }
    //} else {
    //    _title->initWithFile(imageFileName);
    //}
}

void AUIButton::setTitleOffset(Vec2 off) {
    auto _title = this->getChildByTag(99);
    if (_title) {
        _title->setPosition(_title->getPosition() + off);
    }
}

void AUIButton::draw(Renderer *renderer, const Mat4& transform, uint32_t flags) {
    if (_drawRect) {
        _customCommand.init(_globalZOrder + 1);
        _customCommand.func = CC_CALLBACK_0(AUIButton::onDrawPrimatives, this, transform, flags);
        renderer->addCommand(&_customCommand);
    }
}

void AUIButton::onDrawPrimatives(const cocos2d::Mat4 &transform, uint32_t flags) {
    GraphicsUtils::drawOutline(this->isIgnoreAnchorPointForPosition(), this->getAnchorPoint(), this->getPosition(), this->getContentSize(), this, transform);
}

void AUIButton::setColor(const Color3B& color) {
    Button::setColor(color);
    setCustomColor(_children,color);
}

void AUIButton::setCustomColor(cocos2d::Vector<cocos2d::Node*> children,const Color3B& color) {
    for (cocos2d::Node* node : children) {
        node->setColor(color);
        setCustomColor(node->getChildren(), color);
    }
}

}