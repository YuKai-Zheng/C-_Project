//
//  iOSSameImageHelper.cpp
//  MixProject_189
//
//  Created by 黄百超 on 26/06/2017.
//
//

#include "iOSSameImageHelper.h"

void iOSSameImageHelper::sameImage(std::string url) {
    NSURL* URL = [NSURL URLWithString:[NSString stringWithFormat:@"%s", url.c_str()]];
    NSData* d = [NSData dataWithContentsOfURL:URL];
    UIImage *img = [UIImage imageWithData:d];
    if (img) {
        UIImageWriteToSavedPhotosAlbum(img, nil, nil, nil);
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@""
                                                        message:@"二维码保存成功！"
                                                       delegate:nil
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles:nil, nil];
        [alert show];
    }
}
