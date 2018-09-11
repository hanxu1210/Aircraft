#import "SocialLuyou.h"

#define OUTPUT_LOG(...)     if (self.debug) NSLog(__VA_ARGS__);

@implementation SocialLuyou

@synthesize debug = __debug;

- (void)dealloc
{
    [self.appKey release];
    
    [super dealloc];
}

- (UIViewController*) getViewController
{
    UIViewController* ctrol = nil;
    if ([[UIDevice currentDevice].systemVersion floatValue] < 6.0) {
        // warning:addSubview doesn't work on greater than or equal to ios6
        NSArray* array = [[UIApplication sharedApplication]windows];
        UIWindow* win = [array objectAtIndex:0];
        
        UIView* ui = [[win subviews] objectAtIndex:0];
        ctrol = (UIViewController*)[ui nextResponder];
    }
    else
    {
        // use this method on greater than or equal to ios6
        ctrol = [UIApplication sharedApplication].keyWindow.rootViewController;
    }
    return ctrol;
}

#pragma mark - InterfaceSocial

- (void) configDeveloperInfo: (NSMutableDictionary*) cpInfo
{
    OUTPUT_LOG(@"configDeveloperInfo invoked (%@)", [cpInfo description]);    

    self.appKey = (NSString*) [cpInfo objectForKey:@"LuyouAppKey"];
    
    [LuYouSDK setDeveloperKey:self.appKey];
    [LuYouSDK shareLuYouSDK].delegate = self;
    [[LuYouSDK shareLuYouSDK] showLuYouSDKFloatViewWithPresentController:[self getViewController]];
}

- (void) setDebugMode: (BOOL) debug
{
    self.debug = debug;
}

- (NSString*) getSDKVersion
{
    return @"luyou ios sdk 1.1.8";
}

- (NSString*) getPluginVersion
{
    return @"0.2.0";
}

- (void) submitScore: (NSString*) leaderboardID withScore: (long) score
{
    
}

- (void) showLeaderboard: (NSString*) leaderboardID
{
    
}

- (void) unlockAchievement: (NSMutableDictionary*) achInfo
{
    
}

- (void) showAchievements
{
    
}

- (void) enterLuyousdkUI
{
    
}

- (void) ternOutLuyousdkUI
{
    
}

- (bool) isSupported
{
    return true;
}
 @end