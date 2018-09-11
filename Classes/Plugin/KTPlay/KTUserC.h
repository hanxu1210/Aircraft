// KTUserC.h
//
// Copyright (c) 2014 KTplay  All rights reserved.
//

#ifndef ktplay_sdk_KTUserC_h
#define ktplay_sdk_KTUserC_h


#ifdef __cplusplus
extern "C" {
#endif
    
    class KTUserC
    {
    public:
        
        const char *userId;
        
        const char *headerUrl;
        
        const char *nickname;

        int gender;
        
        const char *city;
        
        const char *score;
        
        long long rank;
        
        const char *snsUserId;
        
        const char *loginType;
        
        const char *gameUserId;
        
        bool needPresentNickname;
        
        KTUserC();
        ~KTUserC();
        
        
    };
    
#ifdef __cplusplus
}
#endif


#endif

