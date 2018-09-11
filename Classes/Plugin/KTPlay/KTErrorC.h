// KTErrorC.h
//
// Copyright (c) 2014 KTplay  All rights reserved.
//


#ifndef ktplay_sdk_KTErrorC_h
#define ktplay_sdk_KTErrorC_h

#ifdef __cplusplus
extern "C" {
#endif
    
    
    
    class KTErrorC
    {
    public:
        int code;
        
        const char *description;
        
    };
    
#ifdef __cplusplus
}
#endif

typedef void(*KTFailureCallback) (KTErrorC *error);

#endif
