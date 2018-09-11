/****************************************************************************
Copyright (c) 2012+2013 cocos2d+x.org

http://www.cocos2d+x.org

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

#import "ShareWrapper.h"
#include "PluginUtilsIOS.h"
#include "ProtocolShare.h"

using namespace cocos2d::plugin;

@implementation ShareWrapper

+ (void) onShareResult:(id) obj withRet:(ShareResult) ret withMsg:(NSString*) msg
{
    PluginProtocol* pPlugin = PluginUtilsIOS::getPluginPtr(obj);
    ProtocolShare* pShare = dynamic_cast<ProtocolShare*>(pPlugin);
    if (pShare) {
        const char* chMsg = [msg UTF8String];
        ShareResultCode cRet = (ShareResultCode) ret;
        pShare->onShareResult(cRet, chMsg);
    } else {
        PluginUtilsIOS::outputLog("Can't find the C++ object of the Share plugin");
    }
}

+ (void) onShakeActionComplete:(id) obj
{
    PluginProtocol* pPlugin = PluginUtilsIOS::getPluginPtr(obj);
    ProtocolShare* pShare = dynamic_cast<ProtocolShare*>(pPlugin);
    if (pShare) {
        pShare->onShakeActionComplete();
    } else {
        PluginUtilsIOS::outputLog("Can't find the C++ object of the Share plugin");
    }
}

+ (void) onShakeClosed:(id) obj
{
    PluginProtocol* pPlugin = PluginUtilsIOS::getPluginPtr(obj);
    ProtocolShare* pShare = dynamic_cast<ProtocolShare*>(pPlugin);
    if (pShare) {
        pShare->onShakeClosed();
    } else {
        PluginUtilsIOS::outputLog("Can't find the C++ object of the Share plugin");
    }
}

@end