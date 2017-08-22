//
//  OpenFixture.h
//  OpenFixture
//
//  Created by Henrique on 8/18/17.
//
//

#ifndef OpenFixture_h
#define OpenFixture_h


#endif /* OpenFixture_h */

#include "Fixture.h"
#include "Utils.h"
namespace openfixture {
    
    
    class OpenFixture{
        
        public:
        
        ofix::Definition* createDefinitionFromScheme(const std::vector< std::vector<std::string>>& s ){
            
            ofix::Definition* def = nullptr;
            for(auto r :  s){
                
                def = ofix::Definition::createDefinition(r[0]);
                
                r.erase(r.begin());
                std::map<std::string, int> chanelDef;
                
                for( auto rr : r ){
                    auto nameValue = split( rr, '=' );
                    chanelDef[ nameValue[1] ] = stoi( nameValue[0] );
                }
                
                def->addMode(chanelDef);
            }
            
            return def;
        }
        
        ofix::Universe* createUniverseFromScheme( const std::vector< std::vector<std::string>>& scheme, std::string uniName = "" ){

            std::unique_ptr<ofix::Universe> uni( new Universe() );
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
				
				// Dimitre inseriu
				mFix->customProp["model"] = name;
				//cout << name << endl;

				
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
                
                mFixtures.push_back( mFix );
            }
            
            if( uniName == "" ){
                uniName = "Universe_" + std::to_string( mUniverses.size() );
            }
            
            mUniverses[uniName] = std::move(uni);

            return &(*uni);
        }
        
        
        //
        std::vector< ofix::FixtureRef > getFixturesWithPropertiesValue( std::string name, std::string value  ){

            std::vector< ofix::FixtureRef > result;
            
            for(auto& fix : mFixtures){
                
                if( fix->customProp.find( name ) != fix->customProp.end() ){
                    
                    if( fix->customProp[name] == value){
                        result.push_back( fix );
                    }
                }
            }

            return result;
        }
        
        
        std::map< std::string, std::vector< ofix::FixtureRef > > getFixturesWithProperties( std::string name ){
            
            std::map< std::string, std::vector< ofix::FixtureRef > >  result;
            
            for(auto& fix : mFixtures){

                if( fix->customProp.find( name ) != fix->customProp.end() ){
                    
                    result[ fix->customProp[name]].push_back(fix);
                    
                }
            }
            return result;
        }
        
        
        
        
        //
        std::vector< ofix::FixtureRef > getFixturesWithDefinitionName( std::string name ){
            
            std::vector< ofix::FixtureRef > result;
            auto def = Definition::getDefinitionByName(name);
            
            return result;
        }

        
//		std::vector< ofix::Definition* > getDefinitions(){
//			
//			return Definitions::
//			
//		}
        std::vector< ofix::Universe* > getUniverses(){
            
            std::vector< ofix::Universe* > result;
            
            for( auto& uni : mUniverses ){
                
                result.push_back( &(*uni.second) );
                
            }
            
            return result;
        }
        
        Universe* getUniverseByName(const std::string& name ){
            
            return &(*mUniverses[name]);
        }
        
        
        
    private:
        
        //
        std::vector<FixtureRef> mFixtures;
        std::map<std::string, std::unique_ptr<Universe> > mUniverses;
    };
    
}
