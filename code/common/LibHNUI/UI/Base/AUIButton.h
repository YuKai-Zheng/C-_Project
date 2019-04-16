#ifndef __H_AUIButton_H__
#define __H_AUIButton_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocos2d::ui;

namespace HN {
class AUIButton :public cocos2d::ui::Button {
public:
    AUIButton();
    virtual ~AUIButton();
    CREATE_FUNC(AUIButton);
    static AUIButton* create(const std::string& normalImage,
                             const std::string& selectedImage = "",
                             const std::string& disableImage = "",
                             TextureResType texType = TextureResType::LOCAL);
    CC_SYNTHESIZE(bool, _drawRect, DrawRect);

public:
    virtual bool init() override;
    virtual bool init(const std::string& normalImage,
                      const std::string& selectedImage ,
                      const std::string& disableImage ,
                      TextureResType texType)override;
    virtual void onPressStateChangedToNormal()override;
    virtual void onPressStateChangedToPressed()override;

    void setTitle(const std::string &imageFileName);
    void setTitleOffset(Vec2 off);
    void setGray(bool isGray);
	void setTitleScal(float scal);

    virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags)override;

    virtual void setColor(const Color3B& color)override;

    void setCustomColor(cocos2d::Vector<cocos2d::Node*> children, const Color3B& color);
private:
    void gray();
    void recovery();
    CustomCommand _customCommand;
    void onDrawPrimatives(const cocos2d::Mat4 &transform, uint32_t flags);

private:
    //Sprite* _title;
};
}
#endif //__H_AUIButton_H__