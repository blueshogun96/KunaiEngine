#pragma once

#include <memory>
#include <list>


class KeProcess;
typedef std::shared_ptr<KeProcess> KeStrongProcessPtr;
typedef std::weak_ptr<KeProcess> KeWeakProcessPtr;


/*
 * Process class
 */
class KeProcess
{
	friend class KeProcessManager;

public:
	enum State
	{
		UNINITIALIZED = 0,
		REMOVED,
		RUNNING,
		PAUSED,
		SUCCEEDED,
		FAILED,
		ABORTED,
	};

private:
	State m_state;
	KeStrongProcessPtr m_child;

public:
	KeProcess();
	virtual ~KeProcess();

protected:
	virtual void OnInit() { m_state = RUNNING; }
	virtual void OnRender( uint32_t delta_miliseconds ) PURE;
	virtual void OnUpdate( uint32_t delta_miliseconds ) PURE;
	virtual void OnSuccess() {}
	virtual void OnFail() {}
	virtual void OnAbort() {}

public:
	inline void Succeed();
	inline void Fail();

	inline void Pause();
	inline void Unpause();

	State GetState() const { return m_state; }
	bool IsActive() const { return ( m_state == RUNNING || m_state == PAUSED ); }
	bool IsDead() const { return ( m_state == SUCCEEDED || m_state == FAILED || m_state == ABORTED ); }
	bool IsRemoved() const { return ( m_state == REMOVED ); }
	bool IsPaused() const { return m_state == PAUSED; }

	inline void AttachChild( KeStrongProcessPtr child );
	KeStrongProcessPtr RemoveChild();
	KeStrongProcessPtr PeekChild() { return m_child; }

private:
	void SetState( State new_state ) { m_state = new_state; }
};


/*
 * Process manager class
 */
class KeProcessManager
{
	typedef std::list<KeStrongProcessPtr> KeProcessList;
	KeProcessList m_process_list;

public:
	~KeProcessManager();

	uint32_t UpdateProcesses( uint32_t delta_ms );
	KeWeakProcessPtr AttachProcess( KeStrongProcessPtr process );
	void AbortAllProcess( bool immediate );

	uint32_t GetProcessCount() const { return m_process_list.size(); }

private:
	void ClearAllProcesses();
};
 