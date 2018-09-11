/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "InterfaceAds.h"
#import "AdChinaBannerView.h"
#import "AdChinaFullScreenView.h"
#import "AdChinaInterstitialView.h"

typedef enum {
    kTypeBanner = 1,
    kTypeFullScreen,
    KTypePopup,
    KTypeOfferWall
} KugouType;

@interface AdChinaInterstitialListener : NSObject <AdChinaInterstitialViewDelegate>
{
    
}

@property (copy, nonatomic) NSString* adsType;
@property (assign, nonatomic) id adsObject;
@property BOOL debug;
- (id) init;
- (id) initWithAdsType: (NSString*) adsType adsObject: (id) adsObject;
@end

@interface AdChinaFullScreenListener : NSObject <AdChinaFullScreenViewDelegate>
{
    
}

@property (copy, nonatomic) NSString* adsType;
@property (assign, nonatomic) id adsObject;
@property BOOL debug;
- (id) init;
- (id) initWithAdsType: (NSString*) adsType adsObject: (id) adsObject;
@end

@interface AdsAdChina : NSObject <InterfaceAds, AdChinaBannerViewDelegate>
{
    AdChinaInterstitialListener* _popupListener;
    AdChinaFullScreenListener* _fullScreenListener;
    int _bannerPos;
}

@property BOOL debug;
@property BOOL popupReady;
@property (nonatomic, assign) AdChinaBannerView* bannerView;
@property (nonatomic, assign) AdChinaFullScreenView *fullView;
@property (nonatomic, assign) AdChinaInterstitialView *popupView;
@property (nonatomic, retain) NSString* bannerID;
@property (nonatomic, retain) NSString *fullScreenID;
@property (nonatomic, retain) NSString *popupID;
@property (nonatomic, retain) NSString *offerwallID;

- (void) showBanner: (int) sizeEnum atPos:(int) pos;
- (void) showFullScreen;
- (void) showPopup;
- (void) showOfferWall;
- (void) preloadPopup;
- (void) preloadAds: (NSMutableDictionary*) info;
/**
 interfaces from InterfaceAds
 */
- (void) configDeveloperInfo: (NSMutableDictionary*) devInfo;
- (void) showAds: (NSMutableDictionary*) info position:(int) pos;
- (void) hideAds: (NSMutableDictionary*) info;
- (void) queryPoints;
- (void) spendPoints: (int) points;
- (void) setDebugMode: (BOOL) isDebugMode;
- (NSString*) getSDKVersion;
- (NSString*) getPluginVersion;


@end
