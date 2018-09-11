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

#import "AdsAdmob.h"
#import "AdsWrapper.h"

#define OUTPUT_LOG(...)     if (self.debug) NSLog(__VA_ARGS__);

@implementation AdsAdmob

@synthesize debug = __debug;
@synthesize strBannerPublishID = __BannerPublishID;
@synthesize strIntersititialPublishID = __InterstitialPublishID;
@synthesize testDeviceIDs = __TestDeviceIDs;

- (void) dealloc
{
    self.bannerView.delegate = nil;
    if (self.bannerView != nil) {
        [self.bannerView release];
        self.bannerView = nil;
    }

    if (self.testDeviceIDs != nil) {
        [self.testDeviceIDs release];
        self.testDeviceIDs = nil;
    }
    [super dealloc];
}

#pragma mark InterfaceAds impl

- (void) configDeveloperInfo: (NSMutableDictionary*) devInfo
{
    self.strBannerPublishID = (NSString*) [devInfo objectForKey:@"AdmobBannerID"];
    self.strIntersititialPublishID = (NSString*) [devInfo objectForKey:@"AdmobInterstitialID"];
}

- (void) showPopup
{
    if (self.interstitial.isReady && self.interstitial != nil) {
        [self.interstitial presentFromRootViewController:[AdsWrapper getCurrentRootViewController]];
    }
}

- (void) preloadPopup
{
    self.interstitial = [[GADInterstitial alloc] init];
    self.interstitial.adUnitID = self.strIntersititialPublishID;
    self.interstitial.delegate = self;
    GADRequest* request = [GADRequest request];
    request.testDevices = [NSArray arrayWithArray:self.testDeviceIDs];
    [self.interstitial loadRequest:request];

}

- (void) preloadAds: (NSMutableDictionary*) info
{
    NSNumber* adsType = (NSNumber*) [info objectForKey:@"Param1"];
    NSNumber* adsSize = (NSNumber*) [info objectForKey:@"Param2"];
    NSNumber* adsPos = (NSNumber*) [info objectForKey:@"Param3"];
    
    int numAdsType = [adsType intValue];
    
    switch (numAdsType) {
        case kTypePopUp:
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
    case kTypePopUp:
        {
            [self showPopup];
            break;
        }
    default:
        OUTPUT_LOG(@"The value of 'AdmobType' is wrong (should be 1 or 2)");
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
            }
            break;
        }
    case kTypeFullScreen:
        {
            if (nil != self.interstitial) {
                [self.interstitial release];
                self.interstitial = nil;
            }
            break;
        }
    default:
        OUTPUT_LOG(@"The value of 'AdmobType' is wrong (should be 1 or 2)");
        break;
    }
}

- (void) queryPoints
{
    OUTPUT_LOG(@"Admob not support query points!");
}

- (void) spendPoints: (int) points
{
    OUTPUT_LOG(@"Admob not support spend points!");
}

- (void) setDebugMode: (BOOL) isDebugMode
{
    self.debug = isDebugMode;
}

- (NSString*) getSDKVersion
{
    return [GADRequest sdkVersion];
}

- (NSString*) getPluginVersion
{
    return @"0.2.0";
}

- (void) showBanner: (int) sizeEnum atPos:(int) pos
{
    GADAdSize size = kGADAdSizeSmartBannerPortrait;
    
    if (nil != self.bannerView) {
        [self.bannerView removeFromSuperview];
        [self.bannerView release];
        self.bannerView = nil;
    }
    _bannerPos = pos;
    self.bannerView = [[GADBannerView alloc] initWithAdSize:size];
    self.bannerView.adUnitID = self.strBannerPublishID;
    self.bannerView.delegate = self;
    [self.bannerView setRootViewController:[AdsWrapper getCurrentRootViewController]];
    [AdsWrapper addAdView:self.bannerView atPos:pos];
    
    GADRequest* request = [GADRequest request];
    request.testDevices = [NSArray arrayWithArray:self.testDeviceIDs];
    [self.bannerView loadRequest:request];
}

#pragma mark interface for Admob SDK

- (void) addTestDevice: (NSString*) deviceID
{
    if (nil == self.testDeviceIDs) {
        self.testDeviceIDs = [[NSMutableArray alloc] init];
        [self.testDeviceIDs addObject:@"SimulatorId"];
    }
    [self.testDeviceIDs addObject:deviceID];
}

#pragma mark GADBannerViewDelegate impl

// Since we've received an ad, let's go ahead and set the frame to display it.
- (void)adViewDidReceiveAd:(GADBannerView *)bannerView {
    OUTPUT_LOG(@"===Admob: adViewDidReceiveAd ===");
    [AdsWrapper addAdView:bannerView atPos:_bannerPos];
    [AdsWrapper onAdsResult:self withRet:kAdsShown withMsg:@"AdsTypeBanner"];
}

- (void)adView:(GADBannerView *)view didFailToReceiveAdWithError:(GADRequestError *)error {
    OUTPUT_LOG(@"===Admob: didFailToReceiveAdWithError: %@", [error localizedFailureReason]);
    int errorNo = kUnknownError;
    switch ([error code]) {
    case kGADErrorNetworkError:
        errorNo = kNetworkError;
        break;
    default:
        break;
    }
    [AdsWrapper onAdsResult:self withRet:errorNo withMsg:@"AdsTypeBanner"];
}

- (void)adViewWillPresentScreen:(GADBannerView *)bannerView
{
    OUTPUT_LOG(@"===Admob: adViewWillPresentScreen ===");
    [AdsWrapper onAdsResult:self withRet:kAdsEntenLandPage withMsg:@"AdsTypeBanner"];
}

- (void)adViewDidDismissScreen:(GADBannerView *)bannerView
{
    OUTPUT_LOG(@"===Admob: adViewDidDismissScreen ===");
    [AdsWrapper onAdsResult:self withRet:kAdsExitLandPage withMsg:@"AdsTypeBanner"];
}

- (void)adViewWillDismissScreen:(GADBannerView *)bannerView
{
    OUTPUT_LOG(@"===Admob: adViewWillDismissScreen ===");
}

- (void)adViewWillLeaveApplication:(GADBannerView *)bannerView
{
    OUTPUT_LOG(@"===Admob: adViewWillLeaveApplication ===");
    [AdsWrapper onAdsResult:self withRet:kAdsLeaveApplication withMsg:@"AdsTypeBanner"];
}

/// Called when an interstitial ad request succeeded.
- (void)interstitialDidReceiveAd:(GADInterstitial *)ad {
    OUTPUT_LOG(@"===Admob: interstitialDidReceiveAd ===");
    [AdsWrapper onAdsResult:self withRet:kAdsReceived withMsg:@"AdsTypePopup"];
}

/// Called when an interstitial ad request failed.
- (void)interstitial:(GADInterstitial *)ad didFailToReceiveAdWithError:(GADRequestError *)error {
    OUTPUT_LOG(@"===Admob: interstitialDidFailToReceiveAdWithError: %@", [error localizedDescription]);
    int errorNo = kUnknownError;
    switch ([error code]) {
        case kGADErrorNetworkError:
            errorNo = kNetworkError;
            break;
        default:
            break;
    }
    [AdsWrapper onAdsResult:self withRet:errorNo withMsg:@"AdsTypePopup"];
}

/// Called just before presenting an interstitial.
- (void)interstitialWillPresentScreen:(GADInterstitial *)ad {
    OUTPUT_LOG(@"===Admob: interstitialWillPresentScreen ===");
}

/// Called before the interstitial is to be animated off the screen.
- (void)interstitialWillDismissScreen:(GADInterstitial *)ad {
    OUTPUT_LOG(@"===Admob: interstitialWillDismissScreen ===");
}

/// Called just after dismissing an interstitial and it has animated off the screen.
- (void)interstitialDidDismissScreen:(GADInterstitial *)ad {
    OUTPUT_LOG(@"===Admob: interstitialDidDismissScreen ===");
    [AdsWrapper onAdsResult:self withRet:kAdsDismissed withMsg:@"AdsTypePopup"];
}

/// Called just before the application will background or terminate because the user clicked on an
/// ad that will launch another application (such as the App Store).
- (void)interstitialWillLeaveApplication:(GADInterstitial *)ad {
    OUTPUT_LOG(@"===Admob: interstitialWillLeaveApplication ===");
    [AdsWrapper onAdsResult:self withRet:kAdsLeaveApplication withMsg:@"AdsTypePopup"];
}

@end
