//
//  KeUnknown.h
//  Aquatic
//
//  Created by Shogun3D on 4/19/15.
//  Copyright (c) 2015 Shogun3D. All rights reserved.
//

#ifndef _KeUnknown_h
#define _KeUnknown_h

#define KEMETHOD        virtual void
#define _KEMETHOD(T)    virtual T


/*
 * Unknown (default base) structure
 */
struct IKeUnknown
{
    virtual void Destroy() PURE;
};

#endif
