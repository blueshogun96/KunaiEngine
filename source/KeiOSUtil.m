//
//  KeiOSUtil.m
//  PolyGone
//
//  Created by Shogun3D on 12/26/16.
//  Copyright © 2016 Shogun3D. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>


#define IS_RETINA ([[UIScreen mainScreen] respondsToSelector:@selector(displayLinkWithTarget:selector:)] && ([UIScreen mainScreen].scale == 2.0))

bool KeRetinaSupportedAPPLE()
{
    return IS_RETINA ? true : false;
}
