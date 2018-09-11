#import "InterfaceSocial.h"
#include <UIKit/UIKit.h>

@interface SocialKTPlay : NSObject <InterfaceSocial>
{
}

@property BOOL debug;
@property (nonatomic, retain)NSString* appKey;
@property (nonatomic, retain)NSString* appSecret;

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
@end
