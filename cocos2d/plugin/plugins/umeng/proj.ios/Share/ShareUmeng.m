#import "ShareUmeng.h"
#import "ShareWrapper.h"
#import "UMSocial.h"
#import "UMSocialWechatHandler.h"
#import "UMSocialQQHandler.h"
#import "UMSocialSinaHandler.h"
#import "UMSocialScreenShoter.h"

#define OUTPUT_LOG(...)     if (self.debug) NSLog(__VA_ARGS__);

@implementation ShareUmeng

@synthesize debug = __debug;
@synthesize shareUrl = _shareUrl;

- (id)init
{
    if (self = [super init]) {
        platfoms = [NSMutableArray arrayWithCapacity:6];
        [platfoms retain];
    }
    return self;
}

- (void)dealloc
{
    [appKey release];
    [platfoms release];
    [[self shareUrl] release];
    [UMSocialShakeService unShakeToSns];
    
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


- (void) onShareResult:(UMSocialResponseEntity *)response
{
    //得到分享到的微博平台名
    if ([response.data count] > 0) {
        OUTPUT_LOG(@"share to sns name is %@",[[response.data allKeys] objectAtIndex:0]);
    }
    
    switch (response.responseCode)
	{
		case UMSResponseCodeSuccess:
			[ShareWrapper onShareResult:self withRet:kShareSuccess withMsg:[response message]];
			break;
		case UMSResponseCodeCancel:
			[ShareWrapper onShareResult:self withRet:kShareCancel withMsg:[response message]];
			break;
		default:
			[ShareWrapper onShareResult:self withRet:kShareFail withMsg:[response message]];
			break;
	}
}

#pragma mark - InterfaceShare 

- (void) configDeveloperInfo: (NSMutableDictionary*) cpInfo
{
    OUTPUT_LOG(@"configDeveloperInfo invoked (%@)", [cpInfo description]);    

    appKey = (NSString*) [cpInfo objectForKey:@"UmengAppKey"];
    [appKey retain];
    NSString* offcialSinaWeiboUserId = [cpInfo objectForKey:@"OffcialSinaWeiboUserId"];
    NSString* officalTencentWeiboUserId = [cpInfo objectForKey:@"OffcialTencentWeiboUserId"];
    
    [UMSocialData setAppKey:appKey];
    [[UMSocialDataService defaultDataService] requestAddFollow:UMShareToSina followedUsid:@[offcialSinaWeiboUserId] completion:nil];
    [[UMSocialDataService defaultDataService] requestAddFollow:UMShareToTencent followedUsid:@[officalTencentWeiboUserId] completion:nil];
    
}

- (void) share: (NSMutableDictionary*) shareInfo
{
    OUTPUT_LOG(@"share invoked (%@)", [shareInfo description]);

    NSString* strText = (NSString*) [shareInfo objectForKey:@"shareText"];
    NSString* strImg = (NSString*) [shareInfo objectForKey:@"shareImage"];
    UIImage* shareImage = [UIImage imageNamed:strImg];
    
    [UMSocialSnsService presentSnsIconSheetView:[self getViewController]
                                    appKey:appKey
                                    shareText:[strText stringByAppendingString:self.shareUrl]
                                    shareImage:shareImage
                                    shareToSnsNames:platfoms
                                    delegate:self];
}

- (void) setDebugMode: (BOOL) debug
{
    self.debug = debug;
	
	if(debug)
		[UMSocialData openLog:YES];
	else
		[UMSocialData openLog:NO];
}

- (NSString*) getSDKVersion
{
    return @"4.2.1";
}

- (NSString*) getPluginVersion
{
    return @"0.2.0";
}

#pragma mark - UMSocialUIDelegate

- (void) didFinishGetUMSocialDataInViewController:(UMSocialResponseEntity *)response
{
	OUTPUT_LOG(@"didFinishGetUMSocialDataInViewController with response is %@", response);
	
	[self onShareResult:response];
}

#pragma mark - Interface of umeng share sdk

- (void) setTargetUrl: (NSString*) targetUrl
 {
    OUTPUT_LOG(@"setTargetUrl invoked (%@)", [targetUrl description]);
    self.shareUrl = targetUrl;
    [UMSocialData defaultData].extConfig.wechatSessionData.url = targetUrl;
    [UMSocialData defaultData].extConfig.wechatTimelineData.url = targetUrl;
    [UMSocialData defaultData].extConfig.wechatFavoriteData.url = targetUrl;
    [UMSocialData defaultData].extConfig.qqData.url = targetUrl;
    [UMSocialData defaultData].extConfig.qzoneData.url = targetUrl;
 }

- (void) setShakeThreshold: (NSNumber*) threshold
{
    OUTPUT_LOG(@"setShakeThreshold invoked (%@)", [threshold description]);
    
    double numThreshold = [threshold doubleValue];
    
    [UMSocialShakeService setShakeThreshold:numThreshold];
}

- (void) shakeToShare: (NSString*) shareText
{
    OUTPUT_LOG(@"shakeToShare invoked (%@)", [shareText description]);
    
    [self setShakeThreshold: [NSNumber numberWithDouble:1.6]];
    UIViewController* viewController = [self getViewController];
    [UMSocialShakeService setShakeToShareWithTypes:nil shareText:[shareText stringByAppendingString:self.shareUrl] screenShoter:[UMSocialScreenShoterCocos2d screenShoterFromEaglView:viewController.view] inViewController:viewController delegate: self];
}

- (void) didFinishShareInShakeView:(UMSocialResponseEntity *)response
{
    OUTPUT_LOG(@"didFinishGetUMSocialDataInViewController with response is %@",response);
    
    [self onShareResult:response];
}

- (void) didCloseShakeView
{
    OUTPUT_LOG(@"didCloseShakeView");
    
    [ShareWrapper onShakeActionComplete:self];
}

-(UMSocialShakeConfig)didShakeWithShakeConfig
{
    OUTPUT_LOG(@"didShakeWithShakeConfig");
    
    [ShareWrapper onShakeClosed:self];
    return UMSocialShakeConfigDefault;
}

- (void) supportWeiXinPlatform: (NSMutableDictionary*) info
{
    OUTPUT_LOG(@"supportWeiXinPlatform invoked (%@)",[info description]);
    
    NSString* appID = (NSString*) [info objectForKey:@"Param1"];
    NSString* appSecret = (NSString*) [info objectForKey:@"Param2"];
    NSString* title = (NSString*) [info objectForKey:@"Param3"];
    
    [UMSocialWechatHandler setWXAppId:appID appSecret:appSecret url:self.shareUrl];
    [UMSocialData defaultData].extConfig.wechatSessionData.title = title;
    [UMSocialData defaultData].extConfig.wechatTimelineData.title = title;
    [platfoms addObject:UMShareToWechatSession];
    [platfoms addObject:UMShareToWechatTimeline];
}

- (void) supportQQPlatform: (NSMutableDictionary*) info
{
    OUTPUT_LOG(@"supportQQPlatform invoked (%@)",[info description]);
    
    NSString* appID = (NSString*) [info objectForKey:@"Param1"];
    NSString* appKey = (NSString*) [info objectForKey:@"Param2"];
    
    [UMSocialQQHandler setQQWithAppId:appID appKey:appKey url:self.shareUrl];
    [UMSocialQQHandler setSupportWebView:YES];
    [platfoms addObject:UMShareToQQ];
    [platfoms addObject:UMShareToQzone];
    }

- (void) supportSinaWeiboPlatform
{
    OUTPUT_LOG(@"supportSinaWeiboPlatform invoked");
    
    [UMSocialSinaHandler openSSOWithRedirectURL:nil];
    [platfoms addObject:UMShareToSina];
}

- (void) supportTencentWeiboPlatform: (NSString*) redirectUrl
{
    OUTPUT_LOG(@"supportTencentWeiboPlatform invoked (%@)", redirectUrl);
    
    //[UMSocialTencentWeiboHandler openSSOWithRedirectUrl:redirectUrl];
    [platfoms addObject:UMShareToTencent];
}

- (void) supportRenrenPlatform: (NSMutableDictionary*) info
{
    OUTPUT_LOG(@"supportRenrenPlatform invoked (%@)",[info description]);
    
    //[UMSocialRenrenHandler openSSO];
    [platfoms addObject:UMShareToRenren];
}

- (void) supportDoubanPlatform
{
    OUTPUT_LOG(@"supportDoubanPlatform invoked");
    
    [platfoms addObject:UMShareToDouban];
}

- (void) supportSmsPlatform
{
    OUTPUT_LOG(@"supportSmsPlatform invoked");
    
    [platfoms addObject:UMShareToSms];
}

- (void) supportEmailPlatform
{
    OUTPUT_LOG(@"supportEmailPlatform invoked");
    
    [platfoms addObject:UMShareToEmail];
}

 @end