#include "Ke.h"
#include "KeRenderDevice.h"
#include "KeDirect3D11RenderDevice.h"


/*
 * Name: IKeDirect3D11Fence::Destroy
 * Desc: Handles destruction of this interface instance.
 */
void IKeDirect3D11Fence::Destroy()
{
    /* Delete this instance */
    delete this;
}