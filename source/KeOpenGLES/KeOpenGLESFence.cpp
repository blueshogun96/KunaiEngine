#include "Ke.h"
#include "KeRenderDevice.h"
#include "KeOpenGLESRenderDevice.h"


/*
 * Name: IKeOpenGLFence::Destroy
 * Desc: Handles destruction of this interface instance.
 */
void IKeOpenGLESFence::Destroy()
{
    /* Delete this instance */
    delete this;
}