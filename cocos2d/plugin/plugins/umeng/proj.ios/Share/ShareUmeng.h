#import "InterfaceShare.h"
#import "UMSocialControllerService.h"
#import "UMSocialShakeService.h"

@interface ShareUmeng : NSObject <InterfaceShare, UMSocialUIDelegate, UMSocialShakeDelegate >
{
	NSString* appKey;
    NSMutableArray* platfoms;
}

@property BOOL debug;
@property (nonatomic, retain)NSString* shareUrl;

- (id)init;
- (void)dealloc;
- (UIViewController*) getViewController;
- (void) onShareResult:(UMSocialResponseEntity *)response;
/**
 interfaces of protocol : InterfaceShare
 */
 
- (void) configDeveloperInfo: (NSMutableDictionary*) cpInfo;
- (void) share: (NSMutableDictionary*) shareInfo;
- (void) setDebugMode: (BOOL) debug;
- (NSString*) getSDKVersion;
- (NSString*) getPluginVersion;

/**
 interfaces of protocol : Interface of umeng share sdk
 */
- (void) setTargetUrl: (NSString*) targetUrl;
- (void) setShakeThreshold: (NSNumber*) threshold;
- (void) shakeToShare: (NSString*) shareText;
- (void) didFinishShareInShakeView:(UMSocialResponseEntity *)response;
- (void) didCloseShakeView;
- (UMSocialShakeConfig)didShakeWithShakeConfig;

- (void) supportWeiXinPlatform: (NSMutableDictionary*) info;
- (void) supportQQPlatform: (NSMutableDictionary*) info;
- (void) supportSinaWeiboPlatform;
- (void) supportTencentWeiboPlatform: (NSString*) redirectUrl;
- (void) supportRenrenPlatform: (NSMutableDictionary*) info;
- (void) supportDoubanPlatform;
- (void) supportSmsPlatform;
- (void) supportEmailPlatform;

@end
