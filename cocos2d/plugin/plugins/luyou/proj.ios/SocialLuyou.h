#import "InterfaceSocial.h"
#import "LuYouSDK.h"
#include <UIKit/UIKit.h>

@interface SocialLuyou : NSObject <InterfaceSocial, LuYouSDKDelegate>
{
}

@property BOOL debug;
@property (nonatomic, retain)NSString* appKey;

- (void)dealloc;
- (UIViewController*) getViewController;
/**
 interfaces of protocol : InterfaceSocial
 */
- (void) configDeveloperInfo : (NSMutableDictionary*) cpInfo;
- (void) submitScore: (NSString*) leaderboardID withScore: (long) score;
- (void) showLeaderboard: (NSString*) leaderboardID;
- (void) unlockAchievement: (NSMutableDictionary*) achInfo;
- (void) showAchievements;
- (void) setDebugMode: (BOOL) debug;
- (NSString*) getSDKVersion;
- (NSString*) getPluginVersion;

- (bool) isSupported;
@end
