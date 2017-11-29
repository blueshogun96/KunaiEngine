#ifndef _FACTORY_H_
#define _FACTORY_H_

#include <map>
#include <Star.h>

#pragma warning( disable : 4786 )

#define DECLARE_FACTORY( IDType, ClassType ) \
\
	private : \
	typedef std::map< IDType, Star< ClassType > > InstanceMap; \
	static InstanceMap smInstanceMap; \
	static IDType		smLastID; \
\
	static IDType GenerateID(){ return ++smLastID; } \
\
    ClassType( void ); \
	ClassType( const ClassType& aClassType ); \
	ClassType& operator=( const ClassType& aClassType ); \
	bool ClassType::operator==( const ClassType& aClassType ) const { return mID == aClassType.mID; } \
	bool ClassType::operator<( const ClassType& aClassType ) const { return mID < aClassType.mID; } \
\
	public : \
	\
	static Star< ClassType > Create( void ) \
	{  \
		ClassType* pClassType = new ClassType; \
		std::pair< InstanceMap::iterator, bool > aPair = smInstanceMap.insert( std::pair< IDType, Star< ClassType > >( GenerateID(), pClassType ) );  \
		pClassType->mID = (*(aPair.first)).first; \
		return pClassType; \
	} \
	static Star< ClassType > Create( const IDType& anID ) \
	{  \
		ClassType* pClassType = new ClassType; \
		std::pair< InstanceMap::iterator, bool > aPair = smInstanceMap.insert( std::pair< IDType, Star< ClassType > >( anID, pClassType ) );  \
		pClassType->mID = anID; \
		return pClassType; \
	} \
\
	static Star< ClassType > Clone( const ClassType& aClassType ) \
	{  \
		ClassType* pClassType = new ClassType( aClassType ); \
		std::pair< InstanceMap::iterator, bool > aPair = smInstanceMap.insert( std::pair< IDType, Star< ClassType > >( GenerateID(), pClassType ) );  \
		pClassType->mID = GenerateID(); \
		return pClassType; \
	} \
\
	static Star< ClassType > Find( const IDType& anID ) \
	{  \
		InstanceMap::iterator iter = smInstanceMap.find( anID ); \
		if ( iter == smInstanceMap.end() ) \
		{ \
			return 0; \
		} \
		return ( (*iter).second );  \
} \
\
	static bool Destroy( const IDType& anID ) \
	{  \
		Star< ClassType > pClassType = 0; \
		InstanceMap::iterator iter = smInstanceMap.find( anID ); \
		if ( iter == smInstanceMap.end() ) \
		{ \
			return false; \
		} \
		delete ( (*iter).second ).AsPtr(); \
		smInstanceMap.erase( iter ); \
		return true; \
	} \
\
IDType ClassType::GetID() const { return mID; }

#define IMPLEMENT_FACTORY( IDType, ClassType, DefaultID ) \
    /*static*/ ClassType::InstanceMap ClassType::smInstanceMap; \
	/*static*/ IDType ClassType::smLastID = DefaultID;


#endif