//
//  KeBaseApplicationTemplate.hpp
//  KeTemplateMac
//
//  Created by Shogun3D on 3/6/17.
//  Copyright © 2017 Shogun3D. All rights reserved.
//

#ifndef KeBaseApplicationTemplate_hpp
#define KeBaseApplicationTemplate_hpp

#include <Ke.h>
#include <KeSystem.h>
#include <KeRenderDevice.h>
#include <KeAudioDevice.h>

/*
 * Base application template class
 */
class CKeBaseApplicationTemplate
{
public:
    CKeBaseApplicationTemplate() {};
    virtual ~CKeBaseApplicationTemplate() {};
    
public:
    virtual void Run() PURE;
    
protected:
    IKeRenderDevice*    m_pRenderDevice;
    IKeAudioDevice*     m_pAudioDevice;
};

#endif /* KeBaseApplicationTemplate_hpp */
