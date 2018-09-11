#include "CommonHeaders.h"

#include "Sound/SoundManager.h"


IMPLEMENT_SINGLETON(SoundManager);

SoundManager::SoundManager()
: m_enabled(true)
, m_vibrateEnabled(true)
{
    m_enabled = CCUserDefault::getInstance()->getBoolForKey("sound", true);
    m_vibrateEnabled = CCUserDefault::getInstance()->getBoolForKey("vibrate", true);
}

SoundManager::~SoundManager()
{

}

void SoundManager::setEnabled(bool enabled)
{
    if (m_enabled != enabled && !m_enabled)
    {
        SimpleAudioEngine::getInstance()->resumeAllEffects();
        SimpleAudioEngine::getInstance()->playBackgroundMusic(m_backgroundMusicPath.c_str());
    }
    else if(m_enabled != enabled && m_enabled)
    {
        SimpleAudioEngine::getInstance()->stopAllEffects();
        SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    }

    m_enabled = enabled;
    
    CCUserDefault::getInstance()->setBoolForKey("sound", m_enabled);
    CCUserDefault::getInstance()->flush();
}

void SoundManager::setVibrateEnabled(bool vibrateEnabled)
{
    m_vibrateEnabled = vibrateEnabled;
    
    CCUserDefault::getInstance()->setBoolForKey("vibrate", m_vibrateEnabled);
    CCUserDefault::getInstance()->flush();
}

void SoundManager::PlayEffect(const char* pszFilePath)
{
    if (m_enabled)
        SimpleAudioEngine::getInstance()->playEffect(pszFilePath);
}

void SoundManager::playBackgroundMusic(const char* pszFilePath, bool bLoop)
{
    if (m_enabled)
        SimpleAudioEngine::getInstance()->playBackgroundMusic(pszFilePath, bLoop);
    
    m_backgroundMusicPath = pszFilePath;
}

bool SoundManager::isBackgroundMusicPlaying()
{
    return SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying();
}

void SoundManager::stopBackgroundMusic()
{
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
     m_backgroundMusicPath = "";
}

void SoundManager::pauseBackgroundMusic()
{
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void SoundManager::resumeBackgroundMusic()
{
    if (m_enabled)
    {
        SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    }
}

void SoundManager::preloadEffect(const char* pszFilePath)
{
    if (m_enabled)
    {
        SimpleAudioEngine::getInstance()->preloadEffect(pszFilePath);
        m_preLoadEffectVec.push_back(pszFilePath);
    }
}

void SoundManager::unloadEffect(const char* pszFilePath)
{
    SimpleAudioEngine::getInstance()->unloadEffect(pszFilePath);
    auto iter = std::find(m_preLoadEffectVec.begin(), m_preLoadEffectVec.end(), pszFilePath);
    if (iter != m_preLoadEffectVec.end())
        m_preLoadEffectVec.erase(iter);
}

void SoundManager::unloadAllEffect()
{
    std::for_each(m_preLoadEffectVec.begin(), m_preLoadEffectVec.end(),
                  [&](const std::string& str)
                  {
                      SimpleAudioEngine::getInstance()->unloadEffect(str.c_str());
                  });
    m_preLoadEffectVec.clear();
}

void SoundManager::vibrate()
{
    if (m_vibrateEnabled)
        SimpleAudioEngine::getInstance()->vibrate();
}
