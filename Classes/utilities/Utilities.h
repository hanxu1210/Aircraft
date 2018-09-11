#ifndef Utilities_H
#define Utilities_H

#include <random>
#include <numeric>

#if COCOS2D_DEBUG
    #define DynamicCast dynamic_cast
#else
    #define DynamicCast static_cast
#endif

#define MAX_LEN     (cocos2d::kMaxLogLen + 1)

#define XAssert(cond, format, ...)      \
    if(!(cond))                           \
    {                                   \
        CCAssert(false,"");             \
    }

#define RAND_LIMIT    32767

#define BIT_SET(a,b) ((a) |= (1<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1<<(b)))
#define BIT_CHECK(a,b) ((a) & (1<<(b)))

/// Random number in range [-1,1]
inline float RandomFloat()
{
    float r = (float)(std::rand() & (RAND_LIMIT));
    r /= RAND_LIMIT;
    r = 2.0f * r - 1.0f;
    return r;
}

/// Random floating point number in range [lo, hi]
inline float RandomFloat(float lo, float hi)
{
    float r = (float)(std::rand() & (RAND_LIMIT));
    r /= RAND_LIMIT;
    r = (hi - lo) * r + lo;
    return r;
}

inline tm GetDate()
{
    struct tm* pTm;
    tm curTime;
    time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    time(&timep);
#else
    struct timeval now;
    //CCTime::gettimeofdayCocos2d(&now, NULL);
    gettimeofday(&now, NULL);
    timep = now.tv_sec;
#endif

    pTm = localtime(&timep);

    curTime.tm_year = pTm->tm_year + 1900;
    curTime.tm_mon = pTm->tm_mon + 1;
    curTime.tm_mday = pTm->tm_mday;
    curTime.tm_hour = pTm->tm_hour;
    curTime.tm_min = pTm->tm_min;
    curTime.tm_sec = pTm->tm_sec;

    return curTime;
}

inline int GetDaysOfMonth(int month)
{
    int curDaysOfYear = 0;
    for (int i = 0; i < month; ++i)
    {
        switch(i)
        {
        case 1:
            curDaysOfYear += 31;
            break;
        case 2:
            curDaysOfYear += 28;
            break;
        case 3:
            curDaysOfYear += 31;
            break;
        case 4:
            curDaysOfYear += 30;
            break;
        case 5:
            curDaysOfYear += 31;
            break;
        case 6:
            curDaysOfYear += 30;
            break;
        case 7:
            curDaysOfYear += 31;
            break;
        case 8:
            curDaysOfYear += 31;
            break;
        case 9:
            curDaysOfYear += 30;
            break;
        case 10:
            curDaysOfYear += 31;
            break;
        case 11:
            curDaysOfYear += 30;
            break;
        case 12:
            curDaysOfYear += 31;
            break;
        }
    }
    return curDaysOfYear;
}

inline int GetElapseDayNum(tm curTm, tm oldTm)
{
    int yearDay = (curTm.tm_year - oldTm.tm_year) * 365;
    int curDaysOfYear = GetDaysOfMonth(curTm.tm_mon) + curTm.tm_mday;
    int oldDaysOfYear = GetDaysOfMonth(oldTm.tm_mon) + oldTm.tm_mday;
    return yearDay - oldDaysOfYear + curDaysOfYear;
}

inline std::vector<std::string> SplitString(std::string str,std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;
    int size=str.size();

    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if((int)pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}


static std::default_random_engine g_worldGenerator(time(nullptr));

inline unsigned int Fitness_proportionate_selection(const std::vector<double>& weights)
{
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    double rand = dis(g_worldGenerator) * std::accumulate(weights.begin(), weights.end(), 0.0);
    
    for (size_t i = 0; i < weights.size(); ++i)
    {
        rand -= weights[i];
        if (rand < 0)
        {
            return i;
        }
    }
    
    return -1;
}

inline std::string IntToString(const int in)
{
    std::stringstream ss;
    ss << in;
    return ss.str();
}

#endif //Utilities_H
