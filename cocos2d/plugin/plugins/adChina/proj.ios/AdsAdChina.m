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

#import "AdsAdChina.h"
#import "AdsWrapper.h"
#import "AdManager.h"

#define OUTPUT_LOG(...)     if (self.debug) NSLog(__VA_ARGS__);

@implementation AdsAdChina

@synthesize debug = __debug;
@synthesize popupReady = __popupReady;

- (void) dealloc
{
    if (self.bannerView != nil) {
        [self.bannerView removeFromSuperview];
        [self.bannerView release];
        self.bannerView = nil;
    }

    if (_popupListener != nil)
    {
        [_popupListener release];
    }
    
    if (_fullScreenListener != nil)
    {
        [_fullScreenListener release];
    }
    
    if (nil != self.fullView) {
        [self.fullView removeFromSuperview];
        [self.fullView release];
        self.fullView = nil;
    }

    if (nil != self.popupView) {
        [self.popupView removeFromSuperview];
        [self.popupView release];
        self.popupView = nil;
    }

    [_bannerID release];
    [_fullScreenID release];
    [_popupID release];
    [_offerwallID release];
    [super dealloc];
}

#pragma mark InterfaceAds impl

- (void) configDeveloperInfo: (NSMutableDictionary*) devInfo
{
    self.bannerID = (NSString*) [devInfo objectForKey:@"AdChinaBannerID"];
    self.fullScreenID = (NSString*) [devInfo objectForKey:@"AdChinaFullScreenID"];
    self.popupID = (NSString*) [devInfo objectForKey:@"AdChinaPopupID"];
    //self.offerwallID = (NSString*) [devInfo objectForKey:@"AdChinaOfferwallID"];
        
    [AdManager setExpandLandingPageToolBar:YES];
    self.popupReady= false;
    
    _fullScreenListener = [[AdChinaFullScreenListener alloc] initWithAdsType: @"AdsTypeFullScreen" adsObject:self];
    _popupListener = [[AdChinaInterstitialListener alloc] initWithAdsType: @"AdsTypePopup" adsObject:self];
}

- (void) preloadAds: (NSMutableDictionary*) info
{
    NSNumber* adsType = (NSNumber*) [info objectForKey:@"Param1"];
    NSNumber* adsSize = (NSNumber*) [info objectForKey:@"Param2"];
    NSNumber* adsPos = (NSNumber*) [info objectForKey:@"Param3"];
    
    int numAdsType = [adsType intValue];
    
    switch (numAdsType) {
        case KTypePopup:
            [self preloadPopup];
            break;
        default:
            OUTPUT_LOG(@"The value of 'AdsType' is wrong");
            break;
    }

}

- (void) showAds: (NSMutableDictionary*) info position:(int) pos
{
    NSString* strType = [info objectForKey:@"AdsType"];
    int type = [strType intValue];
    switch (type) {
        case kTypeBanner:
        {
            NSString* strSize = [info objectForKey:@"AdsSizeEnum"];
            int sizeEnum = [strSize intValue];
            [self showBanner:sizeEnum atPos:pos];
            break;
        }
        case kTypeFullScreen:
            [self showFullScreen];
            break;
        case KTypePopup:
            [self showPopup];
            break;
        case KTypeOfferWall:
            [self showOfferWall];
            break;
        default:
            OUTPUT_LOG(@"The value of 'AdsType' is wrong");
            break;
    }
}

- (void) hideAds: (NSMutableDictionary*) info
{
    NSString* strType = [info objectForKey:@"AdsType"];
    int type = [strType intValue];
    switch (type) {
    case kTypeBanner:
        {
            if (nil != self.bannerView) {
                [self.bannerView removeFromSuperview];
                [self.bannerView release];                
                self.bannerView = nil;
                [AdsWrapper onAdsResult:self withRet:kAdsDismissed withMsg:@"AdsTypeBanner"];
            }
            break;
        }
    case kTypeFullScreen:
        {
            if (nil != self.fullView) {
                [self.fullView removeFromSuperview];
                [self.fullView release];
                self.fullView = nil;
            }
            break;
        }
    case KTypePopup:
        {
            if (nil != self.popupView) {
                [self.popupView removeFromSuperview];
                [self.popupView release];
                self.popupView = nil;
            }
            break;
        }
    default:
        OUTPUT_LOG(@"The value of 'AdmobType' is wrong");
        break;
    }
}

- (void) queryPoints
{
    OUTPUT_LOG(@"AdAdChina not support query points!");
}

- (void) spendPoints: (int) points
{
    OUTPUT_LOG(@"points = %lu", (unsigned long)points);
    
    [AdsWrapper onAdsResult:self withRet:kPointsSpendSucceed withMsg:@"spend ponts success!"];
}

- (void) setDebugMode: (BOOL) isDebugMode
{
    self.debug = isDebugMode;
}

- (NSString*) getSDKVersion
{
    return @"3.2.2";
}

- (NSString*) getPluginVersion
{
    return @"0.2.0";
}

- (void) showBanner: (int) sizeEnum atPos:(int) pos
{
    if (nil != self.bannerView) {
        [self.bannerView removeFromSuperview];
        [self.bannerView release];
        self.bannerView = nil;
    }
    
    UIViewController* viewController = [AdsWrapper getCurrentRootViewController];
    CGSize adSize = BannerSizeDefault;
    _bannerPos = pos;
    self.bannerView = [AdChinaBannerView requestAdWithAdSpaceId:_bannerID delegate:self adSize:adSize];
    [self.bannerView retain];
    [self.bannerView setDebugMode:self.debug];
    [self.bannerView setViewControllerForBrowser:viewController];
}

- (void) showFullScreen
{
    if (nil != self.fullView) {
        [self.fullView removeFromSuperview];
        self.fullView = nil;
    }
    
    self.fullView = [AdChinaFullScreenView requestAdWithAdSpaceId:_fullScreenID delegate:_fullScreenListener];
    [self.fullView retain];
    [self.fullView setDebugMode:self.debug];
    [self.fullView setViewControllerForBrowser:[AdsWrapper getCurrentRootViewController]];
}

- (void) showPopup
{
    if (self.popupReady && self.popupView != nil)
    {
        [[AdsWrapper getCurrentRootViewController].view addSubview:self.popupView];
    }
}

- (void) showOfferWall
{
    OUTPUT_LOG(@"AdAdChina not support showOfferWall!");
}

- (void) preloadPopup
{
    if (nil != self.popupView) {
        [self.popupView removeFromSuperview];
        self.popupView = nil;
    }
    
    self.popupView = [AdChinaInterstitialView requestAdWithAdSpaceId:_popupID delegate:_popupListener];
    [self.popupView retain];
    [self.popupView setViewControllerForBrowser:[AdsWrapper getCurrentRootViewController]];
}

#pragma mark AdChinaBanner Delegate impl
- (void)didClickBanner:(AdChinaBannerView *)adView {
    OUTPUT_LOG(@"===AdChina: didClickBanner===");
    [AdsWrapper onAdsResult:self withRet:kAdsClicked withMsg:@"AdsTypeBanner"];
}
     
- (void)didReceiveAd:(AdChinaBannerView *)adView {
    OUTPUT_LOG(@"===AdChina: didGetBanner Tag:%ld===", (long)adView.tag);
    if (self.bannerView != nil)
    {
        [AdsWrapper addAdView:self.bannerView atPos:_bannerPos];
    }
    [AdsWrapper onAdsResult:self withRet:kAdsReceived withMsg:@"AdsTypeBanner"];
}

- (void)didFailedToReceiveAd:(AdChinaBannerView *)adView {
    OUTPUT_LOG(@"===AdChina: didFailToGetBanner Tag:%ld===", (long)adView.tag);
    [AdsWrapper onAdsResult:self withRet:kUnknownError withMsg:@"AdsTypeBanner"];
}
- (void)didEnterFullScreenMode {
    OUTPUT_LOG(@"===AdChina: didEnterFullScreenMode ===");
    [AdsWrapper onAdsResult:self withRet:kAdsEntenLandPage withMsg:@"AdsTypeBanner"];
}
- (void)didExitFullScreenMode {
    OUTPUT_LOG(@"===AdChina: didExitFullScreenMode ===");
    [AdsWrapper onAdsResult:self withRet:kAdsExitLandPage withMsg:@"AdsTypeBanner"];
}
@end

#pragma mark AdChinaFullScreen Delegate impl
@implementation AdChinaFullScreenListener

@synthesize adsType =_adsType;
@synthesize adsObject = _adsObject;
@synthesize debug = __debug;

- (id) init
{
    return [self initWithAdsType:@"AdsTypeFullScreen" adsObject:nil];
}

- (id) initWithAdsType: (NSString*) adsType adsObject: (id) adsObject;
{
    if (self = [super init])
    {
        self.adsType = adsType;
        self.adsObject = adsObject;
    }
    
    return self;
}

- (void)didGetFullScreenAd:(AdChinaFullScreenView *)adView {
    OUTPUT_LOG(@"===AdChina: didGetFullScreenAd===");
    [[AdsWrapper getCurrentRootViewController].view addSubview:adView];
    [AdsWrapper onAdsResult:self.adsObject withRet:kAdsReceived withMsg:self.adsType];
}

- (void)didFailToGetFullScreenAd:(AdChinaFullScreenView *)adView {
    OUTPUT_LOG(@"===AdChina: didFailToGetFullScreenAd===");
    [adView removeFromSuperview];
    [AdsWrapper onAdsResult:self.adsObject withRet:kUnknownError withMsg:self.adsType];
}

- (void)didFinishWatchingFullScreenAd:(AdChinaFullScreenView *)adView {
    OUTPUT_LOG(@"===AdChina: didClosedFullScreenAd===");
    [AdsWrapper onAdsResult:self.adsObject withRet:kAdsDismissed withMsg:self.adsType];
    
}

-(void)didClickFullScreenAd:(AdChinaFullScreenView *)adView {
    OUTPUT_LOG(@"===AdChina: didClickFullScreenAdL===");
    [AdsWrapper onAdsResult:self.adsObject withRet:kAdsClicked withMsg:self.adsType];
}

- (void)didEnterFullScreenMode {
    OUTPUT_LOG(@"=====didEnterFullScreenMode=====");
    [AdsWrapper onAdsResult:self.adsObject withRet:kAdsEntenLandPage withMsg:self.adsType];
}

- (void)didExitFullScreenMode {
    OUTPUT_LOG(@"=====didExitFullScreenMode=====");
    [AdsWrapper onAdsResult:self.adsObject withRet:kAdsExitLandPage withMsg:self.adsType];
}
@end

#pragma mark AdChinaInterstitial Delegate impl

@implementation AdChinaInterstitialListener

@synthesize adsType =_adsType;
@synthesize adsObject = _adsObject;
@synthesize debug = __debug;

- (id) init
{
    return [self initWithAdsType:@"AdsTypePopup" adsObject:nil];
}

- (id) initWithAdsType: (NSString*) adsType adsObject: (id) adsObject;
{
    if (self = [super init])
    {
        self.adsType = adsType;
        self.adsObject = adsObject;
    }
    
    return self;
}

-(void)didGetInterstitialAd:(AdChinaInterstitialView *)adView {
    OUTPUT_LOG(@"=====didGetInterstitialAd======");
    AdsAdChina* ads = (AdsAdChina*)self.adsObject;
    ads.popupReady = true;
    [AdsWrapper onAdsResult:self.adsObject withRet:kAdsReceived withMsg:self.adsType];
}

- (void)didFailToGetInterstitialAd:(AdChinaInterstitialView *)adView {
    OUTPUT_LOG(@"=====didFailToGetInterstitialAd=====");
    [adView removeFromSuperview];
    [AdsWrapper onAdsResult:self.adsObject withRet:kUnknownError withMsg:self.adsType];
}
- (void)didCloseInterstitialAd:(AdChinaInterstitialView *)adView {
    OUTPUT_LOG(@"=====didCloseInterstitialAd=====");
    [adView removeFromSuperview];
    [AdsWrapper onAdsResult:self.adsObject withRet:kAdsDismissed withMsg:self.adsType];
}

// You may use these methods to count click/watch number by yourself
- (void)didClickInterstitialView:(AdChinaInterstitialView *)adView {
    OUTPUT_LOG(@"=====didClickInterstitialView=====");
    [AdsWrapper onAdsResult:self.adsObject withRet:kAdsClicked withMsg:self.adsType];
}

- (void)didEnterFullScreenMode {
    OUTPUT_LOG(@"=====didEnterFullScreenMode=====");
    [AdsWrapper onAdsResult:self.adsObject withRet:kAdsEntenLandPage withMsg:self.adsType];
}

- (void)didExitFullScreenMode {
    OUTPUT_LOG(@"=====didExitFullScreenMode=====");
    [AdsWrapper onAdsResult:self.adsObject withRet:kAdsExitLandPage withMsg:self.adsType];
}
@end
