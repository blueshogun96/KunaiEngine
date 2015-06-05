#pragma once

#include <ezxml.h>
#include <memory>
#include <map>


/*
 * Actor types
 */
class KeActor;
class KeActorComponent;

typedef uint32_t ActorId;
typedef uint32_t ComponentId;
typedef std::shared_ptr<KeActor> KeStrongActorPtr;
typedef std::shared_ptr<KeActorComponent> KeStrongActorComponentPtr;
typedef KeActorComponent *(*KeActorComponentCreator)(void);
typedef std::map<std::string, KeActorComponentCreator> KeActorComponentCreatorMap;


/*
 * Actor component class
 */
class KeActorComponent
{
	friend class ActorFactory;

protected:
	KeStrongActorPtr m_owner;

public:
	virtual ~KeActorComponent() {};

	/* These functions are meant to be overriden by the implementation classes of the components */
	virtual bool Init( ezxml_t* xml ) PURE;
	virtual void PostInit() {}
	virtual void Update() {}

	virtual ComponentId GetComponentId() PURE;

private:
	void SetOwner( KeStrongActorPtr owner ) { m_owner = owner; }
};


/*
 * Actor factory
 */
class KeActorFactory
{
	ActorId m_last_actor_id;

protected:
	KeActorComponentCreatorMap m_actor_component_creators;

public:
	KeActorFactory();
	KeStrongActorPtr CreateActor( const char* actor_resource );

protected:
	virtual KeStrongActorComponentPtr CreateComponent( ezxml_t* xml );

private:
	ActorId GetNextActorId() { ++m_last_actor_id; return m_last_actor_id; }
};

/*
 * Actor class
 */
class KeActor
{
	KeActor();
};