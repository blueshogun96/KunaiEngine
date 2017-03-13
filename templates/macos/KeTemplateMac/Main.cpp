//
//  Main.cpp
//  KeTemplateMac
//
//  Created by Shogun3D on 3/5/17.
//  Copyright © 2017 Shogun3D. All rights reserved.
//

#include "KeDemoApplication.hpp"


int KeMain( std::vector<std::string> args )
{
    std::unique_ptr<CKeDemoApplication> App( new CKeDemoApplication );
    App->Run();
    
    return 0;
}
