#include "Ke.h"
#include "KeRenderDevice.h"
#include "KeOpenGLRenderDevice.h"
#include "KeOpenGLFence.h"



/*
 * Name: IKeOpenGLFence::Destroy
 * Desc: Handles destruction of this interface instance.
 */
void IKeOpenGLFence::Destroy()
{
    /* Destroy the fence */
    KeOpenGLDeleteFence[vendor]( this );
    
    /* Delete this instance */
    delete this;
}

/*
 * Name: IKeOpenGLFence::Insert
 * Desc: Inserts GPU fence object into the execution pipeline.
 */
bool IKeOpenGLFence::Insert()
{
    /* Sanity check */
    if( !fence )
        return false;
    
    IKeOpenGLFence* f = this;
    
    return KeOpenGLInsertFence[vendor]( (IKeOpenGLFence**) &f );
}


/*
 * Name: IKeOpenGLFence::Test
 * Desc: Returns true if this all GPU commands have been completed since
 *		 this fence was set.  If there are still GPU commands pending,
 *		 returns false.
 */
bool IKeOpenGLFence::Test()
{
    if( !fence )
        return false;
    
    return KeOpenGLTestFence[vendor]( this );
}


/*
 * Name: IKeOpenGLFence::Block
 * Desc: Stalls the current thread until the fence has been crossed.
 */
void IKeOpenGLFence::Block()
{
    KeOpenGLBlockOnFence[vendor]( this );
}


/*
 * Name: IKeOpenGLFence::Valid
 * Desc: Tests this fence object for a valid fence.
 */
bool IKeOpenGLFence::Valid()
{
    if( !fence )
        return false;
    
    return KeOpenGLIsFence[vendor]( this );
}