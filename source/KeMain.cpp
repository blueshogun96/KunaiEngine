//
//  KeMain.cpp
//
//  Created by Shogun3D on 6/1/16.
//  Copyright (c) 2016 Shogun3D. All rights reserved.
//

#include <KePlatform.h>

#ifdef _UWP
using namespace Platform;
#endif

/* 
 * The user defines this entry point if they want to 
 */
extern int KeMain( std::vector<std::string> args );


#if defined(_UWP)							
/* Universal Windows Project */
[MTAThread]
int main( Platform::Array<Platform::String^>^ args )
{
	std::vector<std::string> vargs;

	vargs.reserve( args->Length );
	for( unsigned int i = 0; i < args->Length; i++ )
	{
		std::wstring wstr( args[i]->Data() );
		vargs.push_back( std::string( wstr.begin(), wstr.end() ) );
	}

	return KeMain( vargs );
}
#elif defined(_WIN32) && !defined(_UWP)		
/* Windows desktop */
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
	std::vector<std::string> vargs;
	int argcnt = 0;
	LPWSTR* cmdargs = CommandLineToArgvW( GetCommandLineW(), &argcnt );

	if( cmdargs != NULL )
	{
		vargs.reserve( argcnt );

		for( int i = 0; i < argcnt; i++ )
		{
			std::wstring wstr( cmdargs[i] );
			vargs.push_back( std::string( wstr.begin(), wstr.end() ) );
		}
	}

	return KeMain( vargs );
}
#else										
/* Everything else *nix based */
int main( int argc, char** argv )
{
    std::vector<std::string> args;
    
    for( int i = 0; i > argc; i++ )
        args.push_back( argv[i] );
    
    return KeMain( args );
}
#endif
