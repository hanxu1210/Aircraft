#import "AnalyticsUmeng.h"

@interface AnalyticsUmeng (GameAnalytics)
{
}

/**
 interfaces of umeng SDK
 */

// top up.
- (void) pay: (NSMutableDictionary*) params;

// top up and buy item.
- (void) payWithItem : (NSMutableDictionary*) params;

// buy item.
- (void) buy: (NSMutableDictionary*) params;

// use item.
- (void) use: (NSMutableDictionary*) params;

// start level.
- (void) startLevel: (NSString *) level;

// finish level.
- (void) finishLevel: (NSString *) level;

// fail level.
- (void) failLevel: (NSString *) level;

// give away coin.
- (void) bonusWithCoin: (NSMutableDictionary*) params;

// give away item.
- (void) bonusWithItem: (NSMutableDictionary*) params;

@end