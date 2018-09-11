// KTLeaderboardC.h
//
// Copyright (c) 2014 KTplay  All rights reserved.
//



#ifndef ktplay_sdk_KTLeaderboardC_h
#define ktplay_sdk_KTLeaderboardC_h
#include "KTErrorC.h"
#include "KTLeaderboardPaginatorC.h"
#ifdef __cplusplus
extern "C" {
#endif
    
    typedef void(*KTLeaderboardCallBack) (bool isSuccess,const char *leaderboardId ,KTLeaderboardPaginatorC *leaderboard,KTErrorC *error);
    
    typedef void(*KTReportScoreCallBack) (bool isSuccess,const char *leaderboardId,long long score,KTErrorC *error);
    

    class KTLeaderboardC
    {
    public:
        static void friendsLeaderboard(const char *leaderboardId ,int startIndex,int count,KTLeaderboardCallBack callback);

        static void globalLeaderboard(const char *leaderboardId ,int startIndex,int count,KTLeaderboardCallBack callback);
        
        static void lastFriendsLeaderboard(const char *leaderboardId ,int startIndex,int count,KTLeaderboardCallBack callback);
        
        static void lastGlobalLeaderboard(const char *leaderboardId ,int startIndex,int count,KTLeaderboardCallBack callback);
        
        static void reportScore(long long score ,const char *leaderboardId,KTReportScoreCallBack callback);
        
    };
    
#ifdef __cplusplus
}
#endif

#endif
