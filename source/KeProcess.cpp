#include "KePlatform.h"
#include "KeDebug.h"
#include "KeProcess.h"



/*
 * Name: KeProcess::KeProcess
 * Desc: 
 */
KeProcess::KeProcess()
{
	m_state = UNINITIALIZED;
}


/*
 * Name: KeProcess::~KeProcess
 * Desc: Default deconstructor; aborts any existing child process.
 */
KeProcess::~KeProcess()
{
	if( m_child )
		m_child->OnAbort();
}


/*
 * Name: KeProcess::RemoveChild
 * Desc: Removes the child process.  This releases ownership of the child to the caller,
 *		 then removes it from the process list.
 */
KeStrongProcessPtr KeProcess::RemoveChild()
{
	if( m_child )
	{
		KeStrongProcessPtr child = m_child;	/* Prevents the child from getting destroyed when we clear it */
		m_child.reset();
		return child;
	}

	return KeStrongProcessPtr();
}