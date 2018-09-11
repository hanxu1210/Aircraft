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

#import "IOSIAP.h"
#import "RMStoreAppReceiptVerificator.h"
#import "RMStoreTransactionReceiptVerificator.h"
#define OUTPUT_LOG(...)     if (self.debug) NSLog(__VA_ARGS__);

@implementation IOSIAP

@synthesize debug;
NSSet * _productIdentifiers;
NSArray *_productArray;

- (void) dealloc
{
    [[RMStore defaultStore] removeStoreObserver:self];
}

-(void) configDeveloperInfo: (NSMutableDictionary*) cpInfo{
    const BOOL iOS7OrHigher = floor(NSFoundationVersionNumber) > NSFoundationVersionNumber_iOS_6_1;
    id<RMStoreReceiptVerificator> _receiptVerificator = iOS7OrHigher ? [[RMStoreAppReceiptVerificator alloc] init] : [[RMStoreTransactionReceiptVerificator alloc] init];
    [RMStore defaultStore].receiptVerificator = _receiptVerificator;
    
    [[RMStore defaultStore] addStoreObserver:self];
}

- (void) payForProduct: (NSMutableDictionary*) cpInfo{
    NSString * pid = [cpInfo objectForKey:@"productId"];
    [[RMStore defaultStore] addPayment:pid success:^(SKPaymentTransaction *transaction) {
        OUTPUT_LOG(@"Purchased");
    } failure:^(SKPaymentTransaction *transaction, NSError *error) {
        OUTPUT_LOG(@"Something went wrong");
    }];
}
- (void) setDebugMode: (BOOL) _debug{
    self.debug = _debug;
}
- (NSString*) getSDKVersion{
    return @"1.0";
}

- (NSString*) getPluginVersion{
    return @"1.0";
}

/*------------------------IAP functions-------------------------------*/
- (void) requestProducts:(NSString*) paramMap{
    NSArray *producIdArray = [paramMap componentsSeparatedByString:@","];
    OUTPUT_LOG(@"param is %@",producIdArray);
    [[RMStore defaultStore] requestProducts:[NSSet setWithArray:producIdArray] success:^(NSArray *products, NSArray *invalidProductIdentifiers) {
        OUTPUT_LOG(@"Products loaded");
    } failure:^(NSError *error) {
        OUTPUT_LOG(@"Something went wrong");
    }];
}

- (void) restoreProducts
{
    [[RMStore defaultStore] restoreTransactionsOnSuccess:^(NSArray *transactions) {
        OUTPUT_LOG(@"Product purchased");
    } failure:^(NSError *error) {
        OUTPUT_LOG(@"Something went wrong");
    }];
    
}

- (BOOL) canMakePayments
{
    return [RMStore canMakePayments];
}

#pragma mark RMStoreObserver

- (void)storeProductsRequestFailed:(NSNotification*)notification
{
    NSError *error = notification.rm_storeError;
    OUTPUT_LOG(@"storeProductsRequestFailed %@.", error.localizedDescription);
    [IAPWrapper onRequestProduct:self withRet:RequestFail withProducts:nil withError:error.localizedFailureReason];
}

- (void)storeProductsRequestFinished:(NSNotification*)notification
{
    OUTPUT_LOG(@"storeProductsRequestFinished.");
    NSArray *products = notification.rm_products;
    NSMutableArray *productsInfo = [NSMutableArray array];

    for(SKProduct *product in products){
        SKProductInfo* productInfo = [[SKProductInfo alloc] init];
        productInfo.product = product;
        productInfo.localizedPrice = [RMStore localizedPriceOfProduct:product];
        [productsInfo addObject:productInfo];
    }
    //NSArray *invalidProductIdentifiers = notification.rm_invalidProductIdentifiers;
    
    [IAPWrapper onRequestProduct:self withRet:RequestSuccees withProducts:productsInfo withError:@""];
}

- (void)storePaymentTransactionFinished:(NSNotification*)notification
{
    OUTPUT_LOG(@"storePaymentTransactionFinished.");
    NSString *productIdentifier = notification.rm_productIdentifier;
    //SKPaymentTransaction *transaction = notification.rm_transaction;
    [IAPWrapper onPayResult:self withRet:PaymentTransactionStatePurchased withMsg:productIdentifier withError:@""];
}

- (void)storePaymentTransactionFailed:(NSNotification*)notification
{
    NSError *error = notification.rm_storeError;
    OUTPUT_LOG(@"storePaymentTransactionFailed %@.", error.localizedDescription);
    NSString *productIdentifier = notification.rm_productIdentifier;
    //SKPaymentTransaction *transaction = notification.rm_transaction;
    [IAPWrapper onPayResult:self withRet:PaymentTransactionStateFailed withMsg:productIdentifier withError:error.localizedFailureReason];
}

// iOS 8+ only
- (void)storePaymentTransactionDeferred:(NSNotification*)notification
{
    NSString *productIdentifier = notification.rm_productIdentifier;
    OUTPUT_LOG(@"storePaymentTransactionDeferred %@.", productIdentifier);
    //SKPaymentTransaction *transaction = notification.rm_transaction;
    [IAPWrapper onPayResult:self withRet:PaymentTransactionStateDefferred withMsg:productIdentifier withError:@""];
}

- (void)storeRestoreTransactionsFailed:(NSNotification*)notification;
{
    NSError *error = notification.rm_storeError;
    OUTPUT_LOG(@"storeRestoreTransactionsFailed %@.", error.localizedDescription);
    NSString *productIdentifier = notification.rm_productIdentifier;
    [IAPWrapper onPayResult:self withRet:PaymentTransactionStateRestorFailed withMsg:productIdentifier withError:error.localizedFailureReason];
}

- (void)storeRestoreTransactionsFinished:(NSNotification*)notification
{
    NSMutableString *productIdentifier = [[NSMutableString alloc] initWithString:@""];
    OUTPUT_LOG(@"storeRestoreTransactionsFinished %@.", productIdentifier);
    NSArray* transaction = notification.rm_transactions;
    for (SKPaymentTransaction * trans in transaction) {
        [productIdentifier appendString:trans.payment.productIdentifier];
        [productIdentifier appendString:@","];
    }
    //NSArray *transactions = notification.rm_transactions;
    [IAPWrapper onPayResult:self withRet:PaymentTransactionStateRestored withMsg:productIdentifier withError:@""];
}

- (void)storeRefreshReceiptFailed:(NSNotification*)notification;
{
    OUTPUT_LOG(@"storeRefreshReceiptFailed.");
    NSError *error = notification.rm_storeError;
    [IAPWrapper onPayResult:self withRet:PaymentTransactionStateRefreshFailed withMsg:@"" withError:error.localizedFailureReason];
}

- (void)storeRefreshReceiptFinished:(NSNotification*)notification
{
    OUTPUT_LOG(@"storeRefreshReceiptFinished.");
    [IAPWrapper onPayResult:self withRet:PaymentTransactionStateRefreshed withMsg:@"" withError:@""];
}

@end
