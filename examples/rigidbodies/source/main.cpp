//
//  main.cpp
//  rigidbodies
//
//  Created by Shogun3D on 8/12/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "rigidbodydemo.h"


int main( int argc, char** argv )
{
    /* Create a new instance of the demo */
    rigidbodydemo_t* demo = new rigidbodydemo_t();
    if( !demo->initialize() )
    {
        delete demo;
        return 0;
    }
    
    /* Execute our main loop */
    return demo->main_loop();
}