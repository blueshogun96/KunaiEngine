#include "Ke.h"
#include "KeRenderDevice.h"
#include "KeOpenGLRenderDevice.h"


/*
 * Name: IKeOpenGLFence::Destroy
 * Desc: Handles destruction of this interface instance.
 */
void IKeOpenGLFence::Destroy()
{
    /* Delete this instance */
    delete this;
}