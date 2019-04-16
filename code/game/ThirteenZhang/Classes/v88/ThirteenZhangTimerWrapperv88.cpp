#include "ThirteenZhangTimerWrapperv88.h"
#include "ThirteenZhangSoundHelper.h"
#include "../../TTNN/Classes/v88/BRNNResV88.h"

//////////////////////////////////////////////////////////////////////////
#define SOUND_TIMER             "v66/music/alarm_v88.mp3"


namespace ThirteenZhang
{
    unsigned int TimerWrapperV88::timerid = 0;
    std::string TimerWrapperV88::TIMER_KEY("TimerWrapperV88_Timer");
	const char* ALARM_ANIMATION_V88 = "v88/animation/common/alarm/alarm.csb";


    
    TimerWrapperV88::TimerWrapperV88()
    : _textClock(nullptr)
    , _callback(nullptr)
    , _currentTime(0)
    , _self(false)
	, _whole(false)
    {
        char buf[64];
        sprintf(buf, "TimerWrapperV88_Timer_%d", timerid++);
        
        _timerKey = buf;
    }
    
    TimerWrapperV88::~TimerWrapperV88()
    {
        
    }

    bool TimerWrapperV88::load()
    {
		_textClock = LabelAtlas::create("", FNT_YELLOW_CLOCK_V88, 32, 40, '0');
		_textClock->setPosition(0,0);
		_textClock->setAnchorPoint(Vec2(0.5, 0.5));
		//_tLabel->setScale(0.8f);
		this->addChild(_textClock);
        //_textClock->setString("");

		clockBg = find<ImageView>("Image_clock");

		clockNode = CSLoader::createNode(ALARM_ANIMATION_V88);
		addChild(clockNode);
		clockNode->setVisible(false);
		if (clockNode)
		{
			clockAction = CSLoader::createTimeline(ALARM_ANIMATION_V88);
			clockNode->runAction(clockAction);
		}

        
        return true;
    }
    
    void TimerWrapperV88::unload()
    {
        _textClock = nullptr;
        restore();
    }
    
    void TimerWrapperV88::start(int time, bool ani)
    {
	
        if (time > 0)
        {
            if (ani)
            {
                getComponent<Node>()->runAction(Sequence::create(
                                                                   Hide::create(),
                                                                   ScaleTo::create(0.0f, 0.0f),
                                                                   Show::create(),
                                                                   ScaleTo::create(0.2f, 1.0f),
                                                                   CallFunc::create([=]() { this->start(time); }),
                                                                   nullptr));
            }
            else
            {
                if (_currentTime != time)
                {
                    _currentTime = time;
                    getComponent<Node>()->unschedule(_timerKey);
                    setTime(_currentTime);
                    getComponent<Node>()->schedule(CC_CALLBACK_1(TimerWrapperV88::scheduleCountdownTimer, this), 1.0f, _timerKey);
                    setVisible(true);
                }
            }
            
        }
    }
    
    void TimerWrapperV88::kill()
    {
        restore();
    }
    
    void TimerWrapperV88::restore()
    {
        _currentTime = 0;
        getComponent<Node>()->unschedule(_timerKey);
        setVisible(false);
		HNAudioEngine::getInstance()->stopEffectWithID(miusicId);
    }
    
    void TimerWrapperV88::setTime(int time)
    {
        char buf[64];
        //sprintf(buf, "%02d", time);
       // sprintf(buf, "%d", time);
		_textClock->setString(StringUtils::format("%d", time));
    }
    
    void TimerWrapperV88::setCountdownTimerCallback(const ccTimerWrapperV88& callback)
    {
        _callback = callback;
    }
    
    void TimerWrapperV88::scheduleCountdownTimer(float dt)
    {
        do
        {
            _currentTime--;

			if (!clockAction->isPlaying())
			{
				_textClock->setVisible(true);
				clockBg->setVisible(true);
			}

			//播放动画
			if (_currentTime == 6)
			{
				clockNode->setVisible(true);
				clockAction->play("alarm", false);
				_textClock->setVisible(false);
				clockBg->setVisible(false);
			
			}

            
            if (_currentTime < 0)
            {
                restore();
                if (nullptr != _callback)
                {
                    _callback(this);
                }
                break;
            }
            
            if (_currentTime <= 5)
            {
				if ((_whole||_self)&&_currentTime==5)
                {
					miusicId = HNAudioEngine::getInstance()->playEffect(SOUND_TIMER);
                    //SoundHepler::getInstance()->playerEffects(SOUND_TIMER);
                }
                
                auto func = [&]() {
                    this->setTime(_currentTime);
                };
                
                if (_self)
                {
                    _textClock->runAction(Sequence::create(CallFunc::create(func),
                                                             ScaleTo::create(0.3f, 1.8f),
                                                             ScaleTo::create(0.3f, 1.0f), nullptr));
                }
                else
                {
                    this->setTime(_currentTime);
                }
            }
            else
            {
                setTime(_currentTime);
            }

		
        } while (0);
        
    }
}
