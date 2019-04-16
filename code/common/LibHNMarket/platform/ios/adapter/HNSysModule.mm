#import "HNSysModule.h"
#import "../../external/json/json.h"
#import "HNUtility.h"
#include <arpa/inet.h>
#include <ifaddrs.h>

#import <Foundation/Foundation.h>
#import "base/SSKeychain.h"
//#import "base/SFHFKeychainUtils.h"

static NSString *kSSToolkitUUIDServiceName = @"com.g8090.gamecenter.OPENUUID.Service";
static NSString *kSSToolkitUUIDAccountName = @"com.g8090.gamecenter.OPENUUID.Account";

#define MODULE_NAME "sysmodule"

////////////////////////////////////////////////////////////////////////////////////////////////////
namespace HN {
create_impl(SysModule)

    SysModule::SysModule()
    : Module(MODULE_NAME) {
  resisterMethod("getbuildversion", HN_SELECTOR(SysModule::getBuildVersion));
  resisterMethod("getversion", HN_SELECTOR(SysModule::getVersion));
  resisterMethod("getosversion", HN_SELECTOR(SysModule::getOSVersion));
  resisterMethod("getdevice", HN_SELECTOR(SysModule::getDevice));
  resisterMethod("getimei", HN_SELECTOR(SysModule::getIMEI));
  resisterMethod("dealPhone", HN_SELECTOR(SysModule::dealPhone));
  resisterMethod("dealMsg", HN_SELECTOR(SysModule::dealMsg));
  resisterMethod("GetSerialNumber", HN_SELECTOR(SysModule::GetSerialNumber));
  resisterMethod("installApp", HN_SELECTOR(SysModule::installApp));
  resisterMethod("getNetIp", HN_SELECTOR(SysModule::getIPAddress));
  resisterMethod("getNetType", HN_SELECTOR(SysModule::getNetType));
  resisterMethod("getAPPUDID", HN_SELECTOR(SysModule::getAPPUDID));
}

SysModule::~SysModule() {}

std::string SysModule::getVersion(const std::string &args) {
  NSString *gameVersion = [HNUtility getVersion];
  return [gameVersion UTF8String];
}

std::string SysModule::getBuildVersion(const std::string &args) {
  NSString *gameVersion = [HNUtility getBuildVersion];
  return [gameVersion UTF8String];
}

std::string SysModule::getOSVersion(const std::string &args) {
  NSString *osVersion = [HNUtility getOSVersion];
  return [osVersion UTF8String];
}

std::string SysModule::getDevice(const std::string &args) {
  NSString *device = [HNUtility getDevice];
  return [device UTF8String];
}
    
std::string SysModule::getIMEI(const std::string &args) {
//    [SFHFKeychainUtils purgeItemsForServiceName:kSSToolkitUUIDServiceName error:nil];
    
//    [SSKeychain deletePasswordForService:kSSToolkitUUIDServiceName account:kSSToolkitUUIDAccountName];
//    NSString *openUUID = @"";
    
//    NSError *error= nil;
    NSString *openUUID = nil;
//
//    openUUID = [SSKeychain passwordForService:kSSToolkitUUIDServiceName account:kSSToolkitUUIDAccountName error:&error];
////    openUUID = [SFHFKeychainUtils storeUsername:<#(NSString *)#> andPassword:<#(NSString *)#> forServiceName:kSSToolkitUUIDServiceName updateExisting:true error:&error];
//    openUUID = [SFHFKeychainUtils getPasswordForUsername:kSSToolkitUUIDAccountName andServiceName:kSSToolkitUUIDServiceName error:&error];
//    if (error) {
//        NSLog(@"getKeyChain error:%@", error.localizedDescription);
//    }
//    
//    if ([openUUID isEqualToString:@""] || openUUID == nil) {
//        // 随机生成一个UUID，只需要生成一次
        openUUID = [HNUtility getIMEI];
////        [SSKeychain setPassword:openUUID forService:kSSToolkitUUIDServiceName account:kSSToolkitUUIDAccountName error:&error];
//        [SFHFKeychainUtils storeUsername:kSSToolkitUUIDAccountName andPassword:openUUID forServiceName:kSSToolkitUUIDServiceName updateExisting:true error:&error];
//    }
    return [openUUID UTF8String];
}

std::string SysModule::GetSerialNumber(const std::string &args) {
  return getIMEI(args);
}

std::string SysModule::dealPhone(const std::string &args) {
  Json::Reader jsReader;
  Json::Value JsonRoot;
  if (jsReader.parse(args, JsonRoot)) {
    std::string number = JsonRoot["number"].asString();
    [HNUtility dealPhone:[NSString stringWithUTF8String:number.c_str()]];
  }
  return "1";
}

std::string SysModule::dealMsg(const std::string &args) {
  Json::Reader jsReader;
  Json::Value JsonRoot;
  if (jsReader.parse(args, JsonRoot)) {
    std::string number = JsonRoot["number"].asString();
    [HNUtility dealMsg:[NSString stringWithUTF8String:number.c_str()]];
  }
  return "1";
}

std::string SysModule::installApp(const std::string &args) {
  // Application::getInstance()->openURL(args);
  return "ios-installApp";
}

std::string SysModule::getAPPUDID(const std::string &args) {
  NSUUID *identifierForVendor = [[UIDevice currentDevice] identifierForVendor];
  NSString *deviceId = [identifierForVendor UUIDString];
  //  SystemInfo::setUUID(std::string([deviceId UTF8String]));
  return std::string([deviceId UTF8String]);
}

std::string SysModule::getNetIp(const std::string &args) {
  NSString *address = @"error";
  struct ifaddrs *interfaces = nullptr;
  struct ifaddrs *temp_addr = nullptr;
  int success = 0;

  // retrieve the current interfaces - returns 0 on success
  success = getifaddrs(&interfaces);
  if (success == 0) {
    // Loop through linked list of interfaces
    temp_addr = interfaces;
    while (temp_addr != nullptr) {
      if (temp_addr->ifa_addr->sa_family == AF_INET) {
        // Check if interface is en0 which is the wifi connection on the iPhone
        if ([[NSString stringWithUTF8String:temp_addr->ifa_name]
                isEqualToString:@"en0"]) {
          // Get NSString from C String
          address =
              [NSString stringWithUTF8String:inet_ntoa(((struct sockaddr_in *)
                                                            temp_addr->ifa_addr)
                                                           ->sin_addr)];
        }
      }

      temp_addr = temp_addr->ifa_next;
    }
  }

  // Free memory
  freeifaddrs(interfaces);

  return [address UTF8String];
}

std::string SysModule::getIPAddress(const std::string &args) {
  return [[HNUtility getIPAdress] UTF8String];
}

std::string SysModule::getNetType(const std::string &args) {
  return [[HNUtility GetNetWorkType] UTF8String];
}
}
