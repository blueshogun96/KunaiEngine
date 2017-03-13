//
//  KeDemoApplication.hpp
//  KeTemplateMac
//
//  Created by Shogun3D on 3/6/17.
//  Copyright © 2017 Shogun3D. All rights reserved.
//

#ifndef KeDemoApplication_hpp
#define KeDemoApplication_hpp

#include "../../src/KeBaseApplicationTemplate.hpp"

class CKeDemoApplication : public CKeBaseApplicationTemplate
{
public:
    CKeDemoApplication();
    virtual ~CKeDemoApplication();
    
public:
    virtual void Run();
};

#endif /* KeDemoApplication_hpp */
