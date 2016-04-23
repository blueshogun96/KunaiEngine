#include "Ke.h"
#include "KeRenderDevice.h"
#include "KeDirect3D11RenderDevice.h"



/*
 * Debugging macros
 */
#define DISPDBG_R( a, b ) { DISPDBG( a, b ); return; }
#define DISPDBG_RB( a, b ) { DISPDBG( a, b ); return false; }
#define D3D_DISPDBG( a, b, c ) if(FAILED(c)) { DISPDBG( a, b << "\nError code: (" << c << ")" ); }
#define D3D_DISPDBG_R( a, b, c ) if(FAILED(c)) { DISPDBG( a, b << "\nError code: (" << c << ")" ); return; }
#define D3D_DISPDBG_RB( a, b, c ) if(FAILED(c)) { DISPDBG( a, b << "\nError code: (" << c << ")" ); return false; }



/*
 * Name: IKeDirect3D11Fence::Destroy
 * Desc: Handles destruction of this interface instance.
 */
void IKeDirect3D11Fence::Destroy()
{
    /* Delete this instance */
    delete this;
}

/*
 * Name: IKeDirect3D11Fence::Insert
 * Desc: Inserts GPU fence object into the execution pipeline.
 */
bool IKeDirect3D11Fence::Insert()
{
	/* Set query object */
	context->End( query );

	return true;
}


/*
 * Name: IKeDirect3D11Fence::Test
 * Desc: Returns true if this all GPU commands have been completed since
 *		 this fence was set.  If there are still GPU commands pending,
 *		 returns false.
 */
bool IKeDirect3D11Fence::Test()
{
    /* Test this query object */
	HRESULT hr = context->GetData( query, NULL, 0, 0 );
	if( FAILED( hr ) )
	{
		/* Test result */
		if( hr == S_FALSE )
		{
			/* GPU commands are still pending */
			return false;
		}
		else
		{
			/* Some other error */
			D3D_DISPDBG_RB( KE_ERROR, "Unexpected error!", hr );
		}
	}

	/* GPU commands all complete */
	return true;
}


/*
 * Name: IKeDirect3D11Fence::Block
 * Desc: Stalls the current thread until the fence has been crossed.
 */
void IKeDirect3D11Fence::Block()
{
    HRESULT hr;
	do
	{
		hr = context->GetData( query, NULL, 0, 0 );
	}
	while( hr == S_FALSE );

	if( hr != S_OK )
	{
		/* Some other error */
		D3D_DISPDBG( KE_ERROR, "Unexpected error!", hr );
	}
}


/*
 * Name: IKeDirect3D11Fence::Valid
 * Desc: Tests this fence object for a valid fence.
 */
bool IKeDirect3D11Fence::Valid()
{
    /* Was this query created already?  If so, assume it's a valid fence. */
	if( !query )
		return false;

	return true;
}