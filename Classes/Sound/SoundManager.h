#ifndef  _APP_SoundManager_H_
#define  _APP_SoundManager_H_

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

class SoundManager
{
public:
    DECLARE_SINGLETON(SoundManager);

public:
    void setEnabled(bool enabled);
    bool isEnabled() {return m_enabled;}
    
    void PlayEffect(const char* pszFilePath);
    
    void playBackgroundMusic(const char* pszFilePath, bool bLoop = false);
    bool isBackgroundMusicPlaying();
    void stopBackgroundMusic();
    void pauseBackgroundMusic();
    void resumeBackgroundMusic();
    
    void preloadEffect(const char* pszFilePath);
    void unloadEffect(const char* pszFilePath);
    void unloadAllEffect();
    
    void setVibrateEnabled(bool vibrateEnabled);
    bool isVibrateEnabled() {return m_vibrateEnabled;}
    
    void vibrate();
    
private:
    bool m_enabled;
    std::string m_backgroundMusicPath;
    std::vector<std::string> m_preLoadEffectVec;
    
    bool m_vibrateEnabled;
};

#endif // _APP_SoundManager_H_