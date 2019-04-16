#ifndef __ThirteenZhang_ScoreWrapper_V88_H__
#define __ThirteenZhang_ScoreWrapper_V88_H__

//#include "cocos2d.h"
//#include "HNNetExport.h"
#include "cocostudio/CocoStudio.h"
//#include "ui/CocosGUI.h"
#include "UI/Base/HNLayer.h"
#include "HNBaseType.h"
#include <string>
#include "ThirteenZhangWrapperv88.h"

//USING_NS_CC;

namespace ThirteenZhang
{
    class ScoreWrapperV88 : public IWrapperV88
    {
    public:
        enum class ALIGNMENT : BYTE
        {
            LEFT,
            RIGHT,
        };
        
    private:
        LLONG _score;
        std::string _cardType;
        ALIGNMENT _alignment;	//对齐方式
        TextAtlas* _textScore;
        ui::ImageView* _imgCardType;
        
    public:
        ScoreWrapperV88();
        virtual ~ScoreWrapperV88();
        
    public:
        virtual bool load() override;
        virtual void unload() override;
        virtual void restore() override;
        
    public:
        void setCardType(const std::string& cardType);
        void setScore(LLONG score);
        void setAlignLeft();
        void setAlignRight();
        void showCardType();
        void showCardScore();
        
    private:
        void setAlignment(ALIGNMENT alignment);
    };
}



#endif  // !__ThirteenZhang_ScoreWrapperV88_H__
