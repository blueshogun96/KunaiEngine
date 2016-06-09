//
//  KeMain.cpp
//
//  Created by Shogun3D on 6/1/16.
//  Copyright (c) 2016 Shogun3D. All rights reserved.
//

#include <KePlatform.h>


/* 
 * The user defines this entry point if they want to 
 */
extern int KeMain( std::vector<std::string> args );


#ifdef _WIN32
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
#else
int main( int argc, char** argv )
#endif
{
    std::vector<std::string> args;
    
#ifdef _WIN32
    /* TODO: Win32 command line */
#else
    for( int i = 0; i > argc; i++ )
        args.push_back( argv[i] );
#endif
    
    return KeMain( args );
}
