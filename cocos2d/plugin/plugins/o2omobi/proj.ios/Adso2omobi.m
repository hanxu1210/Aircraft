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

#import "Adso2omobi.h"
#import "AdsWrapper.h"
#import "BOAD.h"
#import "BOADError.h"

#define OUTPUT_LOG(...)     if (self.debug) NSLog(__VA_ARGS__);

@implementation Adso2omobi

@synthesize debug = __debug;

- (void) dealloc
{
    if (self.bannerView != nil) {
        [self.bannerView release];
        self.bannerView = nil;
    }
    
    if (_fullScreen != nil)
    {
        [_fullScreen release];
        _fullScreen = nil;
    }
    
    if (_fullScreenListener != nil)
    {
        [_fullScreenListener release];
    }
    
    if (_popup != nil)
    {
        [_popup release];
        _popup = nil;
    }
    
    if (_popupListener != nil)
    {
        [_popupListener release];
    }
    
    

    [super dealloc];
}

#pragma mark InterfaceAds impl

- (void) configDeveloperInfo: (NSMutableDictionary*) devInfo
{
    NSString* appKey = (NSString*) [devInfo objectForKey:@"o2omobiAppKey"];
    NSString* appSecret = (NSString*) [devInfo objectForKey:@"o2omobiAppSecret"];
    
    [BOAD setAppId:appKey appScrect:appSecret];
    _fullScreenListener = [[BOADInterstitialListener alloc] initWithAdsType: @"AdsTypeFullScreen" adsObject:self];
    _popupListener = [[BOADInterstitialListener alloc] initWithAdsType: @"AdsTypePopup" adsObject:self];
    
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
            if (_fullScreen != nil)
            {
                [_fullScreen release];
                _fullScreen = nil;
            }
            break;
        }
    case KTypePopup:
        {
            if (_popup != nil)
            {
                [_popup release];
                _popup = nil;
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
    OUTPUT_LOG(@"Admob not support query points!");
}

- (void) spendPoints: (int) points
{
    OUTPUT_LOG(@"points = %lu", (unsigned long)points);
    
    [AdsWrapper onAdsResult:self withRet:kPointsSpendSucceed withMsg:@"spend ponts success!"];
}

- (void) setDebugMode: (BOOL) isDebugMode
{
    self.debug = isDebugMode;
    [BOAD setLogEnabled:isDebugMode];
}

- (NSString*) getSDKVersion
{
    return @"3.0.0";
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
    
    _bannerPos = pos;
    UIViewController* viewController = [AdsWrapper getCurrentRootViewController];
    BOADAdSize adSize = BOADAdSizeSmartBannerPortrait;

    self.bannerView = [[BOADBannerView alloc] initWithAdSize:adSize origin:CGPointMake(0, 0)];
	[self.bannerView retain];
    self.bannerView.delegate = self;
    [self.bannerView setRootViewController:viewController];
    [self.bannerView loadAd];
    
}

- (void) showFullScreen
{
    if (_fullScreen != nil)
    {
        [_fullScreen release];
        _fullScreen = nil;
    }
    
    _fullScreen = [[BOADInterstitial alloc] init];
    
    _fullScreen.delegate = _fullScreenListener;
    _fullScreen.rootViewController = [AdsWrapper getCurrentRootViewController];
    [_fullScreen loadFullAd];
}

- (void) showPopup
{
    if (_popup != nil && _popup.loaded) {
        [_popup presentFloatAd];
    }
}

- (void) showOfferWall
{
    //NSDictionary *extraData = [NSDictionary dictionaryWithObjectsAndKeys:@"Parameter value", @"Parameter name", nil];
    //[BOAD setExtraData:extraData];// 打开积分墙控制器前调用
    //[BOAD openIntegralWall:[AdsWrapper getCurrentRootViewController]];
    //[BOAD awardPoints:^(NSUInteger points) {// 在这里给用户增加应用货币
        //[self spendPoints: points];
    //}];
}

- (void) preloadPopup
{
    if (_popup != nil)
    {
        [_popup release];
        _popup = nil;
    }
    
    _popup = [[BOADInterstitial alloc] init];
    
    _popup.delegate = _popupListener;
    _popup.rootViewController = [AdsWrapper getCurrentRootViewController];
    [_popup preloadFloatAd];

}

#pragma mark BOADBannerViewDelegate impl

- (void)boadBannerViewWillLoadAd:(BOADBannerView *)bannerView {
    OUTPUT_LOG(@"boadBannerViewWillLoadAd");
    
    [AdsWrapper onAdsResult:self withRet:kAdsReceived withMsg:@"AdsTypeBanner"];
}
- (void)boadBannerViewDidLoadAd:(BOADBannerView *)bannerView {
    OUTPUT_LOG(@"boadBannerViewDidLoadAd");
    if (self.bannerView != nil) {
        [AdsWrapper addAdView:self.bannerView atPos:_bannerPos];
    }
    [AdsWrapper onAdsResult:self withRet:kAdsShown withMsg:@"AdsTypeBanner"];
}
- (void)boadBannerViewDidTapAd:(BOADBannerView *)bannerView {
    OUTPUT_LOG(@"boadBannerViewDidTapAd");
    
    [AdsWrapper onAdsResult:self withRet:kAdsClicked withMsg:@"AdsTypeBanner"];
}
- (void)boadBannerView:(BOADBannerView *)banner didFailToReceiveAdWithError:(BOADError *)error {
    OUTPUT_LOG(@"Failed to receive ad with error: %@", [error localizedFailureReason]);
    
    int errorNo = kUnknownError;
    switch ([error code]) {
        case BOADErrorCodeNetworkError:
            errorNo = kNetworkError;
            break;
        default:
            break;
    }
    [AdsWrapper onAdsResult:self withRet:errorNo withMsg:@"AdsTypeBanner"];
}
- (void)boadBannerViewActionWillBegin:(BOADBannerView *)bannerView willLeaveApplication:(BOOL)willLeaveApplication {
    OUTPUT_LOG(@"boadBannerViewActionWillBegin");
    
    if (willLeaveApplication) {// 不需要额外的处理
        OUTPUT_LOG(@"will leave application");
        [AdsWrapper onAdsResult:self withRet:kAdsLeaveApplication withMsg:@"AdsTypeBanner"];
    } else {// 可以暂停一些活动
        OUTPUT_LOG(@"not leave application");
        [AdsWrapper onAdsResult:self withRet:kAdsEntenLandPage withMsg:@"AdsTypeBanner"];
    }
}
- (void)boadBannerViewActionDidFinish:(BOADBannerView *)bannerView {
   OUTPUT_LOG(@"boadBannerViewActionDidFinish");
    // 可以恢复暂停的活动
    
    [AdsWrapper onAdsResult:self withRet:kAdsExitLandPage withMsg:@"AdsTypeBanner"];
}


@end


@implementation BOADInterstitialListener

@synthesize adsType =_adsType;
@synthesize adsObject = _adsObject;
@synthesize debug = __debug;

- (id) init
{
    return [self initWithAdsType:@"AdsTypePopup" adsObject:nil];
}

- (id) initWithAdsType: (NSString*) adsType adsObject: (id) adsObject
{
    if (self = [super init])
    {
        self.adsType = adsType;
        self.adsObject = adsObject;
    }
    
    return self;
}

#pragma mark BOADInterstitialDelegate impl
- (void)boadInterstitialWillLoadAd:(BOADInterstitial *)interstitial {
    OUTPUT_LOG(@"boadInterstitialWillLoadAd");
}
- (void)boadInterstitialDidLoadAd:(BOADInterstitial *)interstitial {
    OUTPUT_LOG(@"boadInterstitialDidLoadAd");
}
- (void)boadInterstitialWillPresentScreen:(BOADInterstitial *)interstitial {
    OUTPUT_LOG(@"boadInterstitialWillPresentScreen");
}
- (void)boadInterstitialDidPresentScreen:(BOADInterstitial *)interstitial
{
    OUTPUT_LOG(@"boadInterstitialDidPresentScreen");
    
    [AdsWrapper onAdsResult:self.adsObject withRet:kAdsShown withMsg:self.adsType];
}
- (void)boadInterstitialWillDismissScreen:(BOADInterstitial *)interstitial {
    OUTPUT_LOG(@"boadInterstitialWillDismissScreen");
}
- (void)boadInterstitialDidDismissScreen:(BOADInterstitial *)interstitial
{
    OUTPUT_LOG(@"boadInterstitialDidDismissScreen");
    
    [AdsWrapper onAdsResult:self.adsObject withRet:kAdsDismissed withMsg:self.adsType];
}

- (void)boadInterstitialDidTapAd:(BOADInterstitial *)interstitial
{
    OUTPUT_LOG(@"boadInterstitialDidTapAd");
    
    [AdsWrapper onAdsResult:self.adsObject withRet:kAdsClicked withMsg:self.adsType];
}

- (void)boadInterstitial:(BOADInterstitial *)interstitial didFailToReceiveAdWithError:(BOADError *)error
{
    OUTPUT_LOG(@"Failed to receive ad with error: %@", [error localizedFailureReason]);
    
    int errorNo = kUnknownError;
    switch ([error code]) {
        case BOADErrorCodeNetworkError:
            errorNo = kNetworkError;
            break;
        default:
            break;
    }
    [AdsWrapper onAdsResult:self.adsObject withRet:errorNo withMsg:self.adsType];
}

@end
