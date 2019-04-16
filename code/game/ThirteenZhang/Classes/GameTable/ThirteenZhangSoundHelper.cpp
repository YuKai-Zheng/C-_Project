#include "ThirteenZhangSoundHelper.h"
//#include "HNUIExport.h"

//#include "cocos2d.h"
//
//USING_NS_CC;

#define MUSIC_SWITCH_KEY		"ThirteenZhang_CheckBox_Misic"
#define EFFECTS_SWITCH_KEY		"ThirteenZhang_CheckBox_Effects"

//#define MUSIC_VALUE_KEY			"ThirteenZhang_Slider_Misic"
//#define EFFECTS_VALUE_KEY		"ThirteenZhang_Slider_Effects"

namespace ThirteenZhang {
static SoundHepler* gSoundHelper = nullptr;

SoundHepler* SoundHepler::getInstance() {
    if (gSoundHelper == nullptr) {
        gSoundHelper = new SoundHepler;
    }

    return gSoundHelper;
}
SoundHepler::SoundHepler() {
//    auto userDefuault = UserDefault::getInstance();
//    this->_musicSwitch = userDefuault->getBoolForKey(MUSIC_SWITCH_KEY, true);
//    this->_effectsSwitch = userDefuault->getBoolForKey(EFFECTS_SWITCH_KEY, true);

//    _musicValue = userDefuault->getIntegerForKey(MUSIC_VALUE_KEY, 50);
//    _effectsValue = userDefuault->getIntegerForKey(EFFECTS_VALUE_KEY, 50);
}

SoundHepler::~SoundHepler() {
}

void SoundHepler::setMusicSwitch(bool val) {
//    if (_musicSwitch != val) {
//        _musicSwitch = val;
//        auto userDefuault = UserDefault::getInstance();
//        userDefuault->setBoolForKey(MUSIC_SWITCH_KEY, _musicSwitch);
//    }
}

void SoundHepler::setEffectSwitch(bool val) {
//    if (_effectsSwitch != val) {
//        _effectsSwitch = val;
//        auto userDefuault = UserDefault::getInstance();
//        userDefuault->setBoolForKey(EFFECTS_SWITCH_KEY, _effectsSwitch);
//    }
}

void SoundHepler::setMusicValue(int val) {
//    if (_musicValue != val) {
//        _musicValue = val;
//        auto userDefuault = UserDefault::getInstance();
//        userDefuault->setIntegerForKey(MUSIC_VALUE_KEY, _musicValue);
//        HN::HNAudioEngine::getInstance()->setBackgroundMusicVolume(_musicValue);
//    }
}

void SoundHepler::setEffectValue(int val) {
//    if (_effectsValue != val) {
//        _effectsValue = val;
//        auto userDefuault = UserDefault::getInstance();
//        userDefuault->setIntegerForKey(EFFECTS_VALUE_KEY, _effectsValue);
//        HN::HNAudioEngine::getInstance()->setEffectsVolume(_effectsValue);
//    }
}

void SoundHepler::playerMusic(const std::string& music) {
//    if (_musicSwitch) {
    HN::HNAudioEngine::getInstance()->playBackgroundMusic(music.c_str(), true);
//    } else {
//        HN::HNAudioEngine::getInstance()->pauseBackgroundMusic();
//    }
}

void SoundHepler::playerEffects(const std::string& effects) {
//    if (_effectsSwitch) {
    HN::HNAudioEngine::getInstance()->playEffect(effects.c_str());
//    }
}

void SoundHepler::stopMusic() {
    HN::HNAudioEngine::getInstance()->stopBackgroundMusic();
}

void SoundHepler::pauseMusic() {
    HN::HNAudioEngine::getInstance()->pauseBackgroundMusic();
}

void SoundHepler::resumeMusic() {
    HN::HNAudioEngine::getInstance()->resumeBackgroundMusic();
}
}
