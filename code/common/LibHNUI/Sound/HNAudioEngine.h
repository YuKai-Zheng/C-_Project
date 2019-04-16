#ifndef _HN_AUDIO_ENGINE_
#define _HN_AUDIO_ENGINE_

#include "cocos2d.h"

USING_NS_CC;

namespace HN {

class HNAudioEngine {
public:
    static HNAudioEngine* getInstance();

public:
    HNAudioEngine();

    void stopAllSound();

    //播放音乐
    void playBackgroundMusic(const char* pszFilePath, bool bLoop = true);
    void stopBackgroundMusic(bool bReleaseData = false);
    void pauseBackgroundMusic();
    void resumeBackgroundMusic();
    //是否有音乐在播放
    bool hasBackgroundMusic();

    //播放音效
    int playEffect(const char* pszFilePath, bool bLoop = false,float pitch = 1.0f, float pan = 0.0f, float gain = 1.0f);
    void stopEffectWithID(unsigned int audioID);
    void pauseEffect();
    void resumeEffect();
    
//    //设置音量
//    void setEffectsVolume(float volume);
//    void setBackgroundMusicVolume(float volume);
//
//    //获得音量大小
//    float getEffectsVolume() const;
//    float getBackgroundMusicVolume() const;

    //设置声音开关
    void setSwitchOfMusic(bool isOpen);
    void setSwitchOfEffect(bool isOpen);

    //获取声音开关与否
    bool getSwitchOfMusic() const;
    bool getSwitchOfEffect() const;

private:
    bool			_switchOfMusic;						//true设置音乐为开
    bool			_switchOfEffect;                    //ture设置音效为开
    std::string		_currentBackgroundName;				//当前背景音乐名称
    int             _bgMusicID;                         //背景音乐ID
    bool            _isPlayingBgMusic;					//是否正在播放背景音乐
//    //音效音量
//    float effectVolume;
    //音效id数组
    std::vector<int> effectIDs;
};

}
#endif 		//_HN_AUDIO_ENGINE_


