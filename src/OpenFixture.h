//
//  OpenFixture.h
//  OpenFixture
//
//  Created by Henrique on 8/18/17.
//
//

#ifndef OpenFixture_h
#define OpenFixture_h


#include "Fixture.h"
#include <string>
#include <map>
#include <vector>

namespace openfixture {

    class OpenFixture{
        
    public:
        OpenFixture(){
            
        }
        
        ~OpenFixture(){
            
        }
        
        
        ofix::UniverseRef createUniverseFromSchemes( const std::vector< std::vector<std::string>>& scheme, const std::string& universeName = "" );
        
        
        std::map<std::string, UniverseRef> mUniverses;
        std::vector< FixtureRef > mFixtures;
    };

} // eof namespace
#endif /* OpenFixture_h */
