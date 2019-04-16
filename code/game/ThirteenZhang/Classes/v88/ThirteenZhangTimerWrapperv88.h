#ifndef __ThirteenZhang_TimerWrapper_V88_H__
#define __ThirteenZhang_TimerWrapper_V88_H__

#include "UI/Base/HNLayer.h"
#include "ThirteenZhangWrapperv88.h"

namespace ThirteenZhang
{
    class TimerWrapperV88;
    
    class TimerWrapperV88 : public IWrapperV88
    {
        static std::string TIMER_KEY;
        static unsigned int timerid;
        
        typedef std::function<void(TimerWrapperV88* timer)> ccTimerWrapperV88;
        
    public:
        TimerWrapperV88();
        virtual ~TimerWrapperV88();
     
    public:
        void start(int time, bool ani = false);
        void kill();
        
    public:
        void setCountdownTimerCallback(const ccTimerWrapperV88& callback);
        void setSelf(bool self) { _self = self; }
        bool isSelf() const { return _self; }
		void setWholeTimer(bool whole) { _whole = whole; }
		bool isWhole() const { return _whole; }
        
    protected:
        virtual bool load() override;
        virtual void unload() override;
        virtual void restore() override;
        
    protected:
        void scheduleCountdownTimer(float dt);
        
    private:
        void setTime(int time);
        
    private:
		cocos2d::LabelAtlas* _textClock;
        int _currentTime;
		int miusicId;
        ccTimerWrapperV88 _callback;

		cocostudio::timeline::ActionTimeline* clockAction;
		ImageView* clockBg;
		Node* clockNode;
        
    private:
        bool _self;
		bool _whole;


        std::string _timerKey;
    };
}



#endif // !__ThirteenZhang_ControllerLayer_H__
