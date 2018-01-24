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
	std::string dxvs = 
		"float4 vs_main( float4 Pos : POSITION ) : SV_POSITION\n"
		"{\n"
		"    return Pos;\n"
		"}";
	std::string dxps = 
		"float4 ps_main( float4 Pos : SV_POSITION ) : SV_Target\n"
		"{\n"
		"    return float4( 1.0f, 1.0f, 0.0f, 1.0f );\n"
		"}";

	std::string glvs = 
		"#version 150\n"
		"in  vec3 in_pos;\n"
		"out vec4 out_colour;\n"

		"void main( void )\n"
		"{\n"
		"    gl_Position = vec4( in_pos.xyz, 1.0 );\n"
		"    out_colour = vec4( 1, 1, 1, 1 );\n"
		"}";
	std::string glfs = 
		"#version 150\n"
		"out vec4 colour;\n"
		"in vec4 out_colour;\n"

		"void main(void)\n"
		"{\n"
			"colour = out_colour;\n"
		"}";

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
    rddesc.buffer_count = 1;
    rddesc.device_type = KE_RENDERDEVICE_D3D11;
	rddesc.refresh_rate = 60;
    
    bool ret = KeCreateWindowAndDevice( &rddesc, &m_pRenderDevice );
    if( !ret )
    {
        DISPDBG( KE_ERROR, "Error initializing render device!" );
    }

	/*
	 * Initialize GPU program and geometry buffer
	 */
	KeVertexAttribute va[] = 
	{
		{ KE_VA_POSITION, 3, KE_FLOAT, No, sizeof(float)*3, 0 },
		{ -1, 0, 0, 0, 0 }
	};

	nv::vec3f vd[] = 
	{
		nv::vec3f( 1.0f, -1.0f, 0.0f ),
		nv::vec3f( -1.0f, -1.0f, 0.0f ),
		nv::vec3f( 0.0f, 1.0f, 0.0f ),
	};

	if( rddesc.device_type == KE_RENDERDEVICE_D3D11 )
		m_pRenderDevice->CreateProgram( dxvs.c_str(), dxps.c_str(), NULL, NULL, va, &m_pProgram );
	else
		m_pRenderDevice->CreateProgram( glvs.c_str(), glfs.c_str(), NULL, NULL, va, &m_pProgram );
	m_pRenderDevice->CreateGeometryBuffer( &vd, sizeof(nv::vec3f)*3, NULL, 0, 0, KE_USAGE_STATIC_WRITE, va, &m_pGB );

	if( m_pRenderDevice->CreateCommandList( &m_pCmdList ) )
	{
		if( m_pRenderDevice->BeginCommandList( m_pCmdList ) )
		{
			m_pRenderDevice->SetProgram( m_pProgram );
			m_pRenderDevice->SetGeometryBuffer( m_pGB );
			m_pRenderDevice->SetTexture( 0, NULL );

			float green[4] = { 0.0f, 0.5f, 0.0f, 1.0 };
			m_pRenderDevice->SetClearColourFV( green );
			m_pRenderDevice->SetClearDepth( 1.0f );
			m_pRenderDevice->SetClearStencil(0);
			m_pRenderDevice->Clear( KE_COLOUR_BUFFER | KE_DEPTH_BUFFER /*| KE_STENCIL_BUFFER*/ );
			m_pRenderDevice->DrawVertices( KE_TRIANGLES, sizeof(nv::vec3f), 0, 3 );

			m_pRenderDevice->EndCommandList( &m_pCmdList, Yes );
		}
	}
}


CKeDemoApplication::~CKeDemoApplication()
{
	if( m_pCmdList )
		m_pCmdList->Destroy();

	if( m_pGB )
		m_pGB->Destroy();

	if( m_pProgram )
		m_pProgram->Destroy();

    KeDestroyWindowAndDevice( m_pRenderDevice );
    m_pRenderDevice = NULL;
    
    KeUninitialize();
}

void CKeDemoApplication::Run()
{
	/*m_pRenderDevice->SetProgram( m_pProgram );
	m_pRenderDevice->SetGeometryBuffer( m_pGB );
	m_pRenderDevice->SetTexture( 0, NULL );*/

    while( !KeQuitRequested() )
    {
        KeProcessEvents();
        
#if 0
        float green[4] = { 0.0f, 0.5f, 0.0f, 1.0 };
        m_pRenderDevice->SetClearColourFV( green );
		m_pRenderDevice->SetClearDepth( 1.0f );
		m_pRenderDevice->SetClearStencil(0);
        m_pRenderDevice->Clear( KE_COLOUR_BUFFER | KE_DEPTH_BUFFER /*| KE_STENCIL_BUFFER*/ );
		m_pRenderDevice->DrawVertices( KE_TRIANGLES, sizeof(nv::vec3f), 0, 3 );
#endif
		//m_pRenderDevice->Clear( KE_COLOUR_BUFFER | KE_DEPTH_BUFFER /*| KE_STENCIL_BUFFER*/ );
		m_pRenderDevice->ExecuteCommandList( m_pCmdList, Yes );
        m_pRenderDevice->Swap();
    }
}
