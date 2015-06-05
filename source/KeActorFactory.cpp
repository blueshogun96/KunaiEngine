#if 0
#include "KePlatform.h"
#include "KeDebug.h"
#include "KeActor.h"


/*
 * Name: KeActorFactory::KeActorFactory
 * Desc: 
 */
KeActorFactory::KeActorFactory()
{

}


/*
 * Name: KeActorFactory::CreateActor
 * Desc: 
 */
KeStrongActorPtr KeActorFactory::CreateActor( const char* actor_resource )
{
	/* Parse the XML data */
	ezxml_t xml = ezxml_parse_str( (char*) actor_resource, strlen( actor_resource ) );
	if( !xml )
	{
		return KeStrongActorPtr();
	}

	/* Create a new actor instance */
	KeStrongActorPtr actor( new KeActor( GetNextActorId() ) );
	if( !actor->Init(xml) )
	{
	}
}
#endif