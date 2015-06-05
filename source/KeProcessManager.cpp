#include "KePlatform.h"
#include "KeDebug.h"
#include "KeProcess.h"


/*
 * Name: KeProcessManager::KeProcessManager
 * Desc: Clear all existing processes.
 */
KeProcessManager::~KeProcessManager()
{
	ClearAllProcesses();
}

/*
 * Name: KeProcessManager::UpdateProcess
 * Desc: Update all processes in the list.
 */
uint32_t KeProcessManager::UpdateProcesses( uint32_t delta_ms )
{
	unsigned short int success_count = 0;
	unsigned short int fail_count = 0;

	KeProcessList::iterator iterator = m_process_list.begin();
	while( iterator != m_process_list.end() )
	{
		/* Get the next process */
		KeStrongProcessPtr current_process = (*iterator);

		/* Save the iterator and increment the old one incase we need 
		   to remove this one from the process list */
		KeProcessList::iterator this_iterator = iterator;
		++iterator;

		/* Initialize any uninitialized processes */
		if( current_process->GetState() == KeProcess::UNINITIALIZED )
			current_process->OnInit();

		/* Tick any running processes */
		if( current_process->GetState() == KeProcess::RUNNING )
			current_process->OnUpdate( delta_ms );

		/* Is this a dead process? */
		if( current_process->IsDead() )
		{
			/* Run the necessary exit function */
			switch( current_process->GetState() )
			{
			case KeProcess::SUCCEEDED:
			{
				/* Call success function */
				current_process->OnSuccess();

				/* Remove and save the child process if it exists. */
				KeStrongProcessPtr child = current_process->RemoveChild();
				if (child)
					AttachProcess( child );
				else
					++success_count;	/* Only count a success when the chain of processes have been completed. */
				break;
			}

			case KeProcess::FAILED:
				/* Call failure function */
				current_process->OnFail();
				++fail_count;
				break;

			case KeProcess::ABORTED:
				/* Call abortion function */
				current_process->OnAbort();
				++fail_count;
				break;
			}

			/* Remove the process and destroy it */
			m_process_list.erase( this_iterator );
		}
	}

	return ( ( success_count << 16 ) | fail_count );
}


/*
 * Name: KeProcessManager::AttachProcess
 * Desc: Attaches a new process to the list so it can be run during the next update iteration.
 */
KeWeakProcessPtr KeProcessManager::AttachProcess( KeStrongProcessPtr process )
{
	m_process_list.push_back( process );
	return KeWeakProcessPtr( process );
}


/*
 * Name: KeProcessManager::ClearAllProcesses
 * Desc: Clear all existing processes, but doesn't call any exit functions for any of them.
 */
void KeProcessManager::ClearAllProcesses()
{
	m_process_list.clear();
}


/*
 * Name: KeProcessManager::AbortAllProcesses
 * Desc: Aborts all processes (sets them as aborted).  If the immediate flag was set,
 *		 then the process's abort function is called immediately, then removed from the list.
 */
void KeProcessManager::AbortAllProcess( bool immediate )
{
	KeProcessList::iterator iterator = m_process_list.begin();
	while( iterator != m_process_list.end() )
	{
		KeProcessList::iterator temp_iterator = iterator;
		++iterator;

		KeStrongProcessPtr process = *temp_iterator;
		if( process->IsActive() )
		{
			process->SetState( KeProcess::ABORTED );
			if( immediate )
			{
				process->OnAbort();
				m_process_list.erase( temp_iterator );
			}
		}
	}
}
