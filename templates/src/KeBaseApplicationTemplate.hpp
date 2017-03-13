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

	virtual void OnResize( int width, int height )
	{
		nv::matrix4f projection;

		nv::perspective( projection, 45.0f, float( width/height ), 0.1f, 1000.0f );

		m_pRenderDevice->SetProjectionMatrix( &projection );
		m_pRenderDevice->SetViewport( 0, 0, width, height );
	}
    
protected:
    IKeRenderDevice*    m_pRenderDevice;
    IKeAudioDevice*     m_pAudioDevice;
};

#endif /* KeBaseApplicationTemplate_hpp */
