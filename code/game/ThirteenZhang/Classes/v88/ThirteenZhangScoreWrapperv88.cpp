#include "ThirteenZhangScoreWrapperv88.h"
#include "ThirteenZhangGameLogic.h"

//////////////////////////////////////////////////////////////////////////
namespace ThirteenZhang
{
    ScoreWrapperV88::ScoreWrapperV88()
    : _textScore(nullptr)
    , _imgCardType(nullptr)
    , _alignment(ALIGNMENT::LEFT)
    {
    }
    
    ScoreWrapperV88::~ScoreWrapperV88()
    {
        
    }
    
    bool ScoreWrapperV88::load()
    {
        _imgCardType = find<ImageView>("Image_CardType");
		_textScore = find<TextAtlas>("BitmapFontLabel_Score");
        setAlignLeft();
        
        return true;
    }
    
    void ScoreWrapperV88::unload()
    {
        _textScore = nullptr;
        _imgCardType = nullptr;
    }
    
    void ScoreWrapperV88::restore()
    {
        
    }
    
    void ScoreWrapperV88::setCardType(const std::string& cardType)
    {
        if (_cardType != cardType)
        {
            _cardType = cardType;
			if (_imgCardType==nullptr){
				_imgCardType = ImageView::create();
			}
            _imgCardType->loadTexture(cardType);
            _imgCardType->setContentSize(_imgCardType->getVirtualRendererSize());
        }
    }
    
    void ScoreWrapperV88::setScore(LLONG score)
    {
        if (_score != score)
        {
            if (_textScore == nullptr) {
                return;
            }
            _score = score;
            
            char buf[128];
            sprintf(buf, (score > 0) ? "+%lld" : "%lld", score);
            if (score > 0)
            {
				_textScore->setProperty(buf, "Games/12100004/13zhang/9.png", 21, 33, "(");
              
            }
            else
            {
                //_textScore->setTextColor(Color4B::GREEN);
				_textScore->setProperty(buf, "Games/12100004/13zhang/8.png", 21, 33, "(");
            }
            _textScore->setString(buf);
        }
    }
    
    void ScoreWrapperV88::setAlignLeft()
    {
        setAlignment(ALIGNMENT::LEFT);
    }
    
    void ScoreWrapperV88::setAlignRight()
    {
        setAlignment(ALIGNMENT::RIGHT);
    }
    
    void ScoreWrapperV88::showCardType()
    {
        if (_textScore) {
            _textScore->setVisible(false);
        }
        if (_imgCardType) {
            _imgCardType->setVisible(true);
        }
    }
    
    void ScoreWrapperV88::showCardScore()
    {
        if (_textScore) {
            _textScore->setVisible(true);
        }
    }
    
    void ScoreWrapperV88::setAlignment(ALIGNMENT alignment)
    {
        if (alignment != _alignment)
        {
            _alignment = alignment;
            
            switch (_alignment)
            {
                case ALIGNMENT::LEFT: //牌型-得分
                {
                    _imgCardType->setAnchorPoint(Vec2(1.0f, 0.5f));
                    _textScore->setAnchorPoint(Vec2(0.0f, 0.5f));
                    
                    _imgCardType->setPosition(Vec2(-5, 0.0f));
                    _textScore->setPosition(Vec2(5, 0.0f));
                } break;
                case ALIGNMENT::RIGHT://得分-牌型
                {
                    _imgCardType->setAnchorPoint(Vec2(0.0, 0.5f));
                    _textScore->setAnchorPoint(Vec2(1.0f, 0.5f));
                    
                    _imgCardType->setPosition(Vec2(5, 0.0f));
                    _textScore->setPosition(Vec2(-5, 0.0f));
                } break;
                default: break;
            }
            
        }
    }
}
