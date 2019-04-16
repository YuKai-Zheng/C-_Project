//
//  SnapshotHelper.h
//  MixProject_VIP
//
//  Created by 黄百超 on 30/06/2017.
//
//

#import <Foundation/Foundation.h>

@interface SnapshotHelper : NSObject

+(NSData*)getSnapshotData;
+(NSData*)getSnapshotWithQRCodeData:(NSData *)code;
+(UIImage*)compressImage:(UIImage *)image
                  toByte:(NSUInteger)maxLength;

@end
