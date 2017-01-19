//
//  KeTimer.h
//
//  Created by Shogun3D on 1/16/17.
//  Copyright © 2017 Shogun3D. All rights reserved.
//

#ifndef KeTimer_h
#define KeTimer_h

/*
 * Timer flags
 */
#define KE_TIMER_PERIODIC   1

/*
 * Timer handle
 */
struct KeTimer;


bool KeInitializeTimer( const char* timer_name, uint32_t flags, KeTimer* timer );

#endif /* KeTimer_h */
