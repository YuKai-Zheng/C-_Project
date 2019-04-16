
#ifndef GoldenToadGameAudio_h
#define GoldenToadGameAudio_h
//#include <string>
#include "GoldenToadMessageHead.h"


namespace GoldenToad
{

	class GoldenToadGameAudio// : public HNGameUIBase
	{
    public:
        
        GoldenToadGameAudio();
        virtual ~GoldenToadGameAudio();
        
    private:
        bool _isPlaying;
        int _smallFishDieAudioID;
        
	public:
        static GoldenToadGameAudio* getInstance();
        
        static std::string playBackgroundMusic(int num = 0);
		static void stopBackgroundMusic();
		static void pauseBackgroundMusic();
		static void resumeBackgroundMusic();

		static void playFireEffect();
		static void playNetEffect();
		static void playIonFireEffect();
		static void playBingoEffect();
		static void playGoldEffect();
		static void playWaveEffect();
		static void playCatchEffect();
		static void playBombEffect();
		static void playAddEffect();

    public:
        void playSmallFishDied(FishKind kind);
        void resetPlayEffectTag(bool isPlaying = false);
        
    private:
        void playAudioFinishCallBack();
        
	};
}


#endif
