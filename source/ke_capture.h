//
//  ke_capture.h
//
//  Created by Shogun3D on 8/21/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __ke_capture__
#define __ke_capture__

#include "ke_system.h"
#include "ke_renderdevice.h"

bool ke_save_screenshot_jpg( ke_renderdevice_t* renderdevice );
bool ke_save_screenshot_bmp( ke_renderdevice_t* renderdevice );

#endif /* defined(__ke_capture__) */
