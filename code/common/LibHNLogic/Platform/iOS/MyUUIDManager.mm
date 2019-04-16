#include "MyUUIDManager.h"

#import <Foundation/Foundation.h>
#import "base/SSKeychain.h"

static NSString *kSSToolkitServiceName = @"com.g8090.gamecenter.Service";
static NSString *kSSToolkitAccountName = @"com.g8090.gamecenter.Account";

std::string MyUUIDManager::getUUID(std::string str){
    NSDictionary* infoDict = [[NSBundle mainBundle] infoDictionary];
    NSString* package_name = [infoDict objectForKey:@"CFBundleIdentifier"];
    NSLog(@"======package name%@",package_name);
    
    NSError *error= nil;
    NSString *accountID = nil;

    accountID = [SSKeychain passwordForService:kSSToolkitServiceName account:kSSToolkitAccountName error:&error];
    if (error) {
        NSLog(@"getKeyChain error:%@", error.localizedDescription);
    }
    
    if ([accountID isEqualToString:@""] || accountID == nil) {
        // 随机生成一个UUID，只需要生成一次
        CFUUIDRef uuidRef = CFUUIDCreate(kCFAllocatorDefault);
        CFStringRef stringRef = CFUUIDCreateString(kCFAllocatorDefault, uuidRef);
        accountID = (__bridge NSString *)stringRef;
        accountID = [accountID stringByReplacingOccurrencesOfString:@"-" withString:@""];
        
        [SSKeychain setPassword:accountID forService:kSSToolkitServiceName account:kSSToolkitAccountName error:&error];
        CFRelease(stringRef);
        CFRelease(uuidRef);
    }
    NSLog(@"======uuid:%@",accountID);
    return [accountID UTF8String];
}
