/****************************************************************************
Copyright (c) 2014 cocos2d-x.org

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

#import "IAPWrapper.h"
#include "PluginUtilsIOS.h"
#include "ProtocolIAP.h"
#import "ParseUtils.h"

using namespace cocos2d::plugin;

@implementation SKProductInfo

@synthesize product = _product;
@synthesize localizedPrice = _localizedPrice;

@end

@implementation IAPWrapper

+ (void) onPayResult:(id) obj withRet:(IAPResult) ret withMsg:(NSString*) msg withError:(NSString*)error
{
    PluginProtocol* plugin = PluginUtilsIOS::getPluginPtr(obj);
    ProtocolIAP* iapPlugin = dynamic_cast<ProtocolIAP*>(plugin);
    ProtocolIAP::ProtocolIAPCallback callback = iapPlugin->getCallback();
    const char* chMsg = [msg UTF8String];
    const char* chError = [error UTF8String];
    PayResultCode cRet = (PayResultCode) ret;
    if (iapPlugin) {
        iapPlugin->onPayResult(cRet, chMsg, chError);
    }else if(callback){
        std::string stdmsg(chMsg);
        std::string stderror(chError);
        callback(cRet,stdmsg,stderror);
    } else {
        PluginUtilsIOS::outputLog("Can't find the C++ object of the IAP plugin");
    }
}
+(void) onRequestProduct:(id)obj withRet:(ProductRequest) ret withProducts:(NSArray *)products withError:(NSString*)error{
    PluginProtocol* plugin = PluginUtilsIOS::getPluginPtr(obj);
    ProtocolIAP* iapPlugin = dynamic_cast<ProtocolIAP*>(plugin);
    PayResultListener *listener = iapPlugin->getResultListener();
    ProtocolIAP:: ProtocolIAPCallback callback = iapPlugin->getCallback();
    const char* charError = [error UTF8String];
    //NSString *productInfo =  [ParseUtils NSDictionaryToNSString:products];
    const char *charProductInfo = nullptr;
    /*if(productInfo !=nil){
        charProductInfo =[productInfo UTF8String];
    }else{
        charProductInfo = "parse productInfo fail";
    }*/
    if (iapPlugin) {
        if(listener){
            TProductList pdlist;
            if (products) {
                for(SKProductInfo *productInfo in products){
                    TProductInfo info;
                    SKProduct* product = productInfo.product;
                    if (product.productIdentifier != nil)
                    {
                        info.insert(std::make_pair("productId", std::string([product.productIdentifier UTF8String])));
                    }
                    if (product.localizedTitle != nil)
                    {
                        info.insert(std::make_pair("productName", std::string([product.localizedTitle UTF8String])));
                    }
                    if (product.price != nil)
                    {
                        
                        info.insert(std::make_pair("productPrice", std::string([productInfo.localizedPrice UTF8String])));
                    }
                    if (product.localizedDescription != nil)
                    {
                        info.insert(std::make_pair("productDesc", std::string([product.localizedDescription UTF8String])));
                    }
                    pdlist.push_back(info);
                }
            }
            listener->onRequestProductsResult((IAPProductRequest )ret,pdlist,charError);
        }else if(callback){  
            std::string stdstr(charProductInfo);
            std::string stderror(charError);
            callback((IAPProductRequest )ret,stdstr,stderror);
        }
    } else {
        PluginUtilsIOS::outputLog("Can't find the C++ object of the IAP plugin");
    }
}
@end
