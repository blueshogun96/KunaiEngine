//
//  KeDemoApplication.cpp
//  KeTemplateMac
//
//  Created by Shogun3D on 3/6/17.
//  Copyright © 2017 Shogun3D. All rights reserved.
//

#include "KeDemoApplication.hpp"



CKeDemoApplication::CKeDemoApplication()
{
    /*
     * Initialize Kunai Engine
     */
    KeInitialize();
    
    /*
     * Initialize a basic core OpenGL 3.x device
     */
    KeRenderDeviceDesc rddesc;
    
    ZeroMemory( &rddesc, sizeof( KeRenderDeviceDesc ) );
    rddesc.width = 640;
    rddesc.height = 480;
    rddesc.colour_bpp = 32;
    rddesc.depth_bpp = 24;
    rddesc.stencil_bpp = 8;
    rddesc.fullscreen = No;
    rddesc.buffer_count = 2;
    rddesc.device_type = KE_RENDERDEVICE_OGL3;
    
    bool ret = KeCreateWindowAndDevice( &rddesc, &m_pRenderDevice );
    if( !ret )
    {
        DISPDBG( KE_ERROR, "Error initializing render device!" );
    }
}


CKeDemoApplication::~CKeDemoApplication()
{
    KeDestroyWindowAndDevice( m_pRenderDevice );
    m_pRenderDevice = NULL;
    
    KeUninitialize();
}

void CKeDemoApplication::Run()
{
    while( !KeQuitRequested() )
    {
        KeProcessEvents();
        
        float green[4] = { 0.0f, 0.5f, 0.0f, 1.0 };
        m_pRenderDevice->SetClearColourFV( green );
        m_pRenderDevice->Clear( KE_COLOUR_BUFFER | KE_DEPTH_BUFFER | KE_STENCIL_BUFFER );
        m_pRenderDevice->Swap();
    }
}
