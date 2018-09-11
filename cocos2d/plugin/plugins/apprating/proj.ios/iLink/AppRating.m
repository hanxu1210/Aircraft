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

#import "AppRating.h"
#import "SocialWrapper.h"

#define OUTPUT_LOG(...)     if (self.debug) NSLog(__VA_ARGS__);

@implementation AppRating

@synthesize debug;

-(void) configDeveloperInfo: (NSMutableDictionary*) cpInfo{
    OUTPUT_LOG(@"configDeveloperInfo invoked (%@)", [cpInfo description]);
    
    NSString* appId = (NSString*) [cpInfo objectForKey:@"AppStoreId"];
    NSString* eventsUntilPrompt = (NSString*) [cpInfo objectForKey:@"EventsUntilPrompt"];
    NSString* usesUntilPrompt = (NSString*) [cpInfo objectForKey:@"UsesUntilPrompt"];
    NSString* daysUntilPrompt = (NSString*) [cpInfo objectForKey:@"DaysUntilPrompt"];
    NSUInteger numEvents = [eventsUntilPrompt integerValue];
    NSUInteger numUses = [usesUntilPrompt integerValue];
    double numDays = [daysUntilPrompt doubleValue];
    
    [Appirater setAppId:appId];
    [Appirater setDaysUntilPrompt:numDays];
    [Appirater setUsesUntilPrompt:numUses];
    [Appirater setSignificantEventsUntilPrompt:numEvents];
    [Appirater setTimeBeforeReminding:2];
    [Appirater setDelegate:self];
    [Appirater appLaunched:YES];
}

- (void) setDebugMode: (BOOL) _debug{
    self.debug = _debug;
    [Appirater setDebug:_debug];
}
- (NSString*) getSDKVersion{
    return @"iLink 1.0.1 && Appirate 2.0.4";
}

- (NSString*) getPluginVersion{
    return @"1.0";
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

- (void) appEnteredForeground
{
    [Appirater appEnteredForeground:YES];
}

- (void) logEvent
{
    [Appirater userDidSignificantEvent:YES];
}

- (void) appiraterDidDeclineToRate
{
    [SocialWrapper onSocialResult:self withRet:kUserDidDeclineToRateApp withMsg:@""];
}

- (void) appiraterDidOptToRemindLater
{
    [SocialWrapper onSocialResult:self withRet:kUserDidRequestReminderToRateApp withMsg:@""];
}

- (void) appiraterDidOptToRate
{
    [SocialWrapper onSocialResult:self withRet:kUserDidAttemptToRateApp withMsg:@""];
}

- (void) iLinkDidOpenAppStore
{
    [SocialWrapper onSocialResult:self withRet:kOpenAppStoreSuccess withMsg:@""];
}

- (void) iLinkCouldNotConnectToAppStore:(NSError *)error
{
    [SocialWrapper onSocialResult:self withRet:kOpenAppStoreFailed withMsg:[[error userInfo] objectForKey:NSLocalizedDescriptionKey]];
}

@end
