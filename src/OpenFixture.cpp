//
//  OpenFixture.cpp
//  OpenFixture
//
//  Created by Henrique on 8/18/17.
//
//

#include "OpenFixture.h"
#include "OpenFixtureDmtr.h"
#include "Utils.h"



namespace openfixture {
    UniverseRef OpenFixture::createUniverseFromSchemes( const std::vector< std::vector<std::string>>& scheme, const std::string& name ){
        
        
        UniverseRef uni = Universe::create();
        
        for(auto fix : scheme){
            
            std::string name = fix[0];
            
            fix.erase(fix.begin());
            
            auto defGlobal = ofix::Definition::getDefinitionByName( name );
            
            if( defGlobal == nullptr){
                
                std::cout << "error no definition name: " << name << std::endl;
                continue;
            }
            
            auto mFix =  ofix::Fixture::create( defGlobal );
            mFix->setMode(0);
            
            
            int channel = -1;
            
            for( auto ff : fix){
                
                auto nameValue = ofix::split( ff, '=' );
                
                std::string name = nameValue[0];
                
                if( nameValue[0] == "channel" ){
                    int value = stoi( nameValue[1] );
                    channel = value;
                }else if( nameValue[0] == "mode" ){
                    
                    int value = stoi( nameValue[1] );
                    mFix->setMode(value);
                    
                }else{
                    mFix->customProp[nameValue[0]] = nameValue[1];
                }
                
            }
            
            
            if( channel != -1 ){
                uni->setFixture(channel, mFix);
            }else{
                uni->appendFixture(mFix);
            }
        }
        
        return uni;
    }
}
