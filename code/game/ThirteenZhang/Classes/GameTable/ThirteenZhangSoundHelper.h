#ifndef __ThirteenZhang_SoundHepler_h__
#define __ThirteenZhang_SoundHepler_h__

#include <string>
#include "HNLobby/PlatformDefine.h"
#include "Sound/HNAudioEngine.h"

namespace ThirteenZhang {
class SoundHepler final {
//    bool _musicSwitch;
//    bool _effectsSwitch;

//    int  _musicValue;
//    int  _effectsValue;

public:
    static SoundHepler* getInstance();
    explicit SoundHepler();
    ~SoundHepler();

public:
    void setMusicSwitch(bool val);
    void setEffectSwitch(bool val);

    void setMusicValue(int val);
    void setEffectValue(int val);

    void playButtonSoundEffect() const {
        HN::HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
    };

//    bool getMusicSwitch() const {
//        return _musicSwitch;
//    }
//    bool getEffectSwitch() const {
//        return _effectsSwitch;
//    }
//
//    int getMusicValue() const {
//        return _musicValue;
//    }
//    int getEffectValue() const {
//        return _effectsValue;
//    }

public:
    void playerMusic(const std::string& music);
    void playerEffects(const std::string& effects);

public:
    void stopMusic();
    void pauseMusic();
    void resumeMusic();
};

}

#endif // __ThirteenZhang_SoundHepler_h__
