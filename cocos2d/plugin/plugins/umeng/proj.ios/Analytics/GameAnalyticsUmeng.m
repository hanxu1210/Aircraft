#import "GameAnalyticsUmeng.h"
#import "MobClickGameAnalytics.h"

#define OUTPUT_LOG(...)     if (self.debug) NSLog(__VA_ARGS__);

@implementation AnalyticsUmeng(GameAnalytics)

- (void) pay: (NSMutableDictionary*) params
{
    OUTPUT_LOG(@"Umeng pay invoked(%@)", [params debugDescription]);
    NSNumber* cash = (NSNumber*) [params objectForKey:@"Param1"];
    NSNumber* source = (NSNumber*) [params objectForKey:@"Param2"];
    NSNumber* coin = (NSNumber*) [params objectForKey:@"Param3"];
    
    double numCash = [cash doubleValue];
    int numSource = [source intValue];
    double numCoin = [coin doubleValue];

    [MobClickGameAnalytics pay:numCash source:numSource coin:numCoin];
}

- (void) payWithItem : (NSMutableDictionary*) params
{
    OUTPUT_LOG(@"Umeng payWithItem invoked(%@)", [params debugDescription]);
    NSNumber* cash = (NSNumber*) [params objectForKey:@"Param1"];
    NSNumber* source = (NSNumber*) [params objectForKey:@"Param2"];
    NSString* item = (NSString*) [params objectForKey:@"Param3"];
    NSNumber* amount = (NSNumber*) [params objectForKey:@"Param4"];
    NSNumber* price = (NSNumber*) [params objectForKey:@"Param5"];

    double numCash = [cash doubleValue];
    int numSource = [source intValue];
    int numAmount = [amount intValue];
    double numPrice = [price doubleValue];

    [MobClickGameAnalytics pay:numCash source:numSource item:item amount:numAmount price:numPrice];
}

- (void) buy: (NSMutableDictionary*) params
{
    OUTPUT_LOG(@"Umeng buy invoked(%@)", [params debugDescription]);
    NSString* item = (NSString*) [params objectForKey:@"Param1"];
    NSNumber* amount = (NSNumber*) [params objectForKey:@"Param2"];
    NSNumber* price = (NSNumber*) [params objectForKey:@"Param3"];

    int numAmount = [amount intValue];
    double numPrice = [price doubleValue];

    [MobClickGameAnalytics buy:item amount:numAmount price:numPrice];
}

- (void) use: (NSMutableDictionary*) params
{
    OUTPUT_LOG(@"Umeng use invoked(%@)", [params debugDescription]);
    NSString* item = (NSString*) [params objectForKey:@"Param1"];
    NSNumber* amount = (NSNumber*) [params objectForKey:@"Param2"];
    NSNumber* price = (NSNumber*) [params objectForKey:@"Param3"];

    int numAmount = [amount intValue];
    double numPrice = [price doubleValue];

    [MobClickGameAnalytics use:item amount:numAmount price:numPrice];
}

- (void) startLevel: (NSString *) level
{
    OUTPUT_LOG(@"Umeng startLevel invoked(%@)", level);

    [MobClickGameAnalytics startLevel:level];
}

- (void) finishLevel: (NSString *) level
{
    OUTPUT_LOG(@"Umeng finishLevel invoked(%@)", level);

    [MobClickGameAnalytics finishLevel:level];
}

- (void) failLevel: (NSString *) level
{
    OUTPUT_LOG(@"Umeng failLevel invoked(%@)", level);

    [MobClickGameAnalytics failLevel:level];
}

- (void) bonusWithCoin: (NSMutableDictionary*) params
{
    OUTPUT_LOG(@"Umeng bonusWithCoin invoked(%@)", [params debugDescription]);
    NSNumber* coin = (NSNumber*) [params objectForKey:@"Param1"];
    NSNumber* source = (NSNumber*) [params objectForKey:@"Param2"];

    double numCoin = [coin doubleValue];
    int numSource = [source intValue];

    [MobClickGameAnalytics bonus:numCoin source:numSource];
}

- (void) bonusWithItem: (NSMutableDictionary*) params
{
    OUTPUT_LOG(@"Umeng bonusWithItem invoked(%@)", [params debugDescription]);
    NSString* item = (NSString*) [params objectForKey:@"Param1"];
    NSNumber* amount = (NSNumber*) [params objectForKey:@"Param2"];
    NSNumber* price = (NSNumber*) [params objectForKey:@"Param3"];
    NSNumber* source = (NSNumber*) [params objectForKey:@"Param4"];

    int numAmount = [amount intValue];
    double numPrice = [price doubleValue];
    int numSource = [source intValue];

    [MobClickGameAnalytics bonus:item amount:numAmount price:numPrice source:numSource];
}
@end