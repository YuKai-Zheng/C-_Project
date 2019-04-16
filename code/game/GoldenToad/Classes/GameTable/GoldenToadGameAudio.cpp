
#include "Sound/HNAudioEngine.h"
#include "audio/include/AudioEngine.h"
#include "GoldenToadGameAudio.h"

namespace GoldenToad {

const char*		GAME_AUDIO_ROOT	 =	"v66/goldenToad/sounds/effect/";

static GoldenToadGameAudio* instant = nullptr;

GoldenToadGameAudio* GoldenToadGameAudio::getInstance() {
    if (nullptr == instant) {
        instant = new GoldenToadGameAudio();
    }
    return instant;
}

GoldenToadGameAudio::GoldenToadGameAudio()
    : _isPlaying(false)
    , _smallFishDieAudioID(-1) {
}

GoldenToadGameAudio::~GoldenToadGameAudio() {
}

std::string GoldenToadGameAudio::playBackgroundMusic(int _num) {
    char str[12];
    std::string filename("v66/goldenToad/sounds/bgm/");
    int num = _num != 0 ? _num : rand()%5 + 1;
    
    sprintf(str, "bgm%d.mp3", num);
    filename.append(str);
    HN::HNAudioEngine::getInstance()->playBackgroundMusic(filename.c_str(), true);
    return filename;
}

void GoldenToadGameAudio::stopBackgroundMusic() {
    HN::HNAudioEngine::getInstance()->stopBackgroundMusic();
}

void GoldenToadGameAudio::pauseBackgroundMusic() {
    HN::HNAudioEngine::getInstance()->pauseBackgroundMusic();
}

void GoldenToadGameAudio::resumeBackgroundMusic() {
    HN::HNAudioEngine::getInstance()->resumeBackgroundMusic();
}

void GoldenToadGameAudio::playSmallFishDied(FishKind kind) {
    if (!HN::HNAudioEngine::getInstance()->getSwitchOfEffect()) {
        return;
    }
    if (!_isPlaying && HN::HNAudioEngine::getInstance()->getSwitchOfEffect()) {
        int idx = rand() % 2 + 1;
        _isPlaying = true;
        std::string filename(GAME_AUDIO_ROOT);
        char str[24];
        sprintf(str, "fish%d_%d_01.wav", kind + 2, idx);
        filename.append(str);
        _smallFishDieAudioID = cocos2d::experimental::AudioEngine::play2d(filename);
        cocos2d::experimental::AudioEngine::setFinishCallback(_smallFishDieAudioID, CC_CALLBACK_0(GoldenToadGameAudio::playAudioFinishCallBack, this));
    }
}

void GoldenToadGameAudio::playAudioFinishCallBack() {
    _isPlaying = false;
}

void GoldenToadGameAudio::playFireEffect() {
    std::string filename(GAME_AUDIO_ROOT);
    filename.append("fire.mp3");
    HN::HNAudioEngine::getInstance()->playEffect(filename.c_str());
}

void GoldenToadGameAudio::playNetEffect() {
    std::string filename(GAME_AUDIO_ROOT);
    filename.append("bgm_net.mp3");
    HN::HNAudioEngine::getInstance()->playEffect(filename.c_str());
}

void GoldenToadGameAudio::playIonFireEffect() {
    std::string filename(GAME_AUDIO_ROOT);
    filename.append("ion_fire.mp3");
    HN::HNAudioEngine::getInstance()->playEffect(filename.c_str());
}

void GoldenToadGameAudio::playBingoEffect() {
    std::string filename(GAME_AUDIO_ROOT);
    filename.append("bingo.wav");
    HN::HNAudioEngine::getInstance()->playEffect(filename.c_str());
}

void GoldenToadGameAudio::playGoldEffect() {
    std::string filename(GAME_AUDIO_ROOT);
    filename.append("gold.mp3");
    HN::HNAudioEngine::getInstance()->playEffect(filename.c_str());
}

void GoldenToadGameAudio::playWaveEffect() {
    std::string filename(GAME_AUDIO_ROOT);
    filename.append("wave.mp3");
    HN::HNAudioEngine::getInstance()->playEffect(filename.c_str());
}

void GoldenToadGameAudio::playCatchEffect() {
    std::string filename(GAME_AUDIO_ROOT);
    filename.append("catch.mp3");
    HN::HNAudioEngine::getInstance()->playEffect(filename.c_str());
}

void GoldenToadGameAudio::playBombEffect() {
    std::string filename(GAME_AUDIO_ROOT);
    filename.append("superarm.mp3");
    HN::HNAudioEngine::getInstance()->playEffect(filename.c_str());
}

void GoldenToadGameAudio::playAddEffect() {
    std::string filename(GAME_AUDIO_ROOT);
    filename.append("updownpao.mp3");
    HN::HNAudioEngine::getInstance()->playEffect(filename.c_str());
}

void GoldenToadGameAudio::resetPlayEffectTag(bool isPlaying) {
    _isPlaying = isPlaying;
}

}
