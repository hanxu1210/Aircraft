#import "SocialKTPlay.h"
#import "KTPlay.h"

#define OUTPUT_LOG(...)     if (self.debug) NSLog(__VA_ARGS__);

@implementation SocialKTPlay

@synthesize debug = __debug;

- (void)dealloc
{
    [self.appKey release];
    [self.appSecret release];
    
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

    self.appKey = (NSString*) [cpInfo objectForKey:@"KTPlayAppKey"];
    self.appSecret = (NSString*) [cpInfo objectForKey:@"KTPlayAppSecret"];
    
    [KTPlay startWithAppKey:self.appKey appSecret:self.appSecret];
    [KTPlay setKTParentView:[self getViewController].view];
}

- (void) setDebugMode: (BOOL) debug
{
    self.debug = debug;
}

- (NSString*) getSDKVersion
{
    return @"ktplay cocos2dx sdk 1.5.2";
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

 @end