#include "HNAudioEngine.h"
#include "audio/include/AudioEngine.h"
#include "../../LibHNLobby/HNLobby/PlatformDefine.h"

using namespace cocos2d::experimental;

namespace HN {

    static HNAudioEngine* instant = nullptr;

    HNAudioEngine::HNAudioEngine() {
        _bgMusicID = AudioEngine::INVALID_AUDIO_ID;
        _switchOfMusic = true;
        _switchOfEffect = true;
        _isPlayingBgMusic = false;
    }

    HNAudioEngine* HNAudioEngine::getInstance() {
        if (!instant) {
            instant = new HNAudioEngine();
        }
        return instant;
    }

    //播放音乐
    void HNAudioEngine::playBackgroundMusic(const char* pszFilePath, bool bLoop) {
        if (_switchOfMusic) {
            if (nullptr != pszFilePath) {
                if (0 != _currentBackgroundName.compare(pszFilePath)) {
                    _currentBackgroundName = pszFilePath;
                    _bgMusicID = AudioEngine::play2d(pszFilePath, bLoop);
//                    AudioEngine::setVolume(_bgMusicID, 1.0f);
                    _isPlayingBgMusic = true;
                }
            }
        }
    }

    void HNAudioEngine::stopBackgroundMusic(bool bReleaseData) {
        if (_bgMusicID == AudioEngine::INVALID_AUDIO_ID) {
            return;
        }
        AudioEngine::stop(_bgMusicID);
        AudioEngine::uncache(_currentBackgroundName);
        _currentBackgroundName = "";
        _isPlayingBgMusic = false;
        _bgMusicID = AudioEngine::INVALID_AUDIO_ID;
    }

    void HNAudioEngine::pauseBackgroundMusic() {
        if (!_isPlayingBgMusic) {
            return;
        }
        if (_bgMusicID == AudioEngine::INVALID_AUDIO_ID) {
            return;
        }
        AudioEngine::pause(_bgMusicID);
        _isPlayingBgMusic = false;
    }

    void HNAudioEngine::resumeBackgroundMusic() {
        if(!_switchOfMusic) {
            return;
        }
        if (_isPlayingBgMusic) {
            return;
        }
        if (_bgMusicID == AudioEngine::INVALID_AUDIO_ID) {
            return;
        }
        AudioEngine::resume(_bgMusicID);
        _isPlayingBgMusic = true;
    }

    bool HNAudioEngine::hasBackgroundMusic() {
        return (_bgMusicID != AudioEngine::INVALID_AUDIO_ID);
    }

    //播放音效
    int HNAudioEngine::playEffect(const char* pszFilePath, bool bLoop, float pitch, float pan, float gain) {
        if (_switchOfEffect) {
            int _effectMusicID = AudioEngine::play2d(pszFilePath, bLoop);
//            AudioEngine::setVolume(_effectMusicID, 1.0f);
            effectIDs.push_back(_effectMusicID);
            AudioEngine::setFinishCallback(_effectMusicID, [&](int audioID, const std::string &filePath) {
                std::vector<int>::iterator iter;
                for (iter = effectIDs.begin(); iter != effectIDs.end(); iter++) {
                    if (*iter==audioID) {
                        effectIDs.erase(iter);
                        break;
                    }
                }
            });
            return _effectMusicID;
        }
        return AudioEngine::INVALID_AUDIO_ID;
    }
    
    void HNAudioEngine::stopEffectWithID(unsigned int audioID) {
        AudioEngine::stop(audioID);
    }

    void HNAudioEngine::pauseEffect() {
        std::vector<int>::iterator iter;
        for (iter = effectIDs.begin(); iter != effectIDs.end(); iter++) {
            AudioEngine::pause(*iter);
        }
    }

    void HNAudioEngine::resumeEffect() {
        if(_switchOfEffect) {
            std::vector<int>::iterator iter;
            for (iter = effectIDs.begin(); iter != effectIDs.end(); iter++) {
                AudioEngine::resume(*iter);
            }
        }
    }

    void HNAudioEngine::stopAllSound() {
        AudioEngine::stopAll();
        AudioEngine::end();
    }

    void HNAudioEngine::setSwitchOfMusic(bool isOpen) {
        _switchOfMusic = isOpen;
        UserDefault::getInstance()->setBoolForKey(MUSIC_SWITCH, _switchOfMusic);
        UserDefault::getInstance()->flush();
    }

    bool HNAudioEngine::getSwitchOfMusic() const {
        return _switchOfMusic;
    }

    void HNAudioEngine::setSwitchOfEffect(bool isOpen) {
        _switchOfEffect = isOpen;
        UserDefault::getInstance()->setBoolForKey(EFFECT_SWITCH, _switchOfEffect);
        UserDefault::getInstance()->flush();
    }

    bool HNAudioEngine::getSwitchOfEffect() const {
        return _switchOfEffect;
    }

////volume range from 0.0 to 1.0
//void HNAudioEngine::setEffectsVolume(float volume) {
////    effectVolume = volume;
////    if (volume <= 0) {
////        _switchOfEffect = false;
////    } else {
////        _switchOfEffect = true;
////    }
////    UserDefault::getInstance()->setBoolForKey(EFFECT_SWITCH, _switchOfEffect);
////    std::vector<int>::iterator iter;
////    for (iter = effectIDs.begin(); iter != effectIDs.end(); iter++) {
////        AudioEngine::setVolume(*iter, volume);
////    }
//}
//
//float HNAudioEngine::getEffectsVolume() const {
//    return effectVolume;
//}
//
//void HNAudioEngine::setBackgroundMusicVolume(float volume) {
////    if (volume <= 0) {
////        _switchOfMusic = false;
////    } else {
////        _switchOfMusic = true;
////    }
////    UserDefault::getInstance()->setBoolForKey(MUSIC_SWITCH, _switchOfMusic);
////    UserDefault::getInstance()->flush();
////    AudioEngine::setVolume(_bgMusicID, volume);
//}
//
//float HNAudioEngine::getBackgroundMusicVolume() const {
//    return AudioEngine::getVolume(_bgMusicID);//大厅背景音乐与游戏的背景音乐音量是一致的
//}
}
