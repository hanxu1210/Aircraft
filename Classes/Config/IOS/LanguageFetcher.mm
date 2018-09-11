#import <Foundation/Foundation.h>
#import "LanguageFetcher.h"
#import <string>

extern "C"
{
	const char* FetchCurrentLanguage()
	{
		std::string lang = [[[NSLocale preferredLanguages] objectAtIndex:0] UTF8String];
		return lang.c_str();
	}
}