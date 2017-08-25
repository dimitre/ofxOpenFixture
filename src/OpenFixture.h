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
                
                std::string defName = r[0];
                
                def = ofix::Definition::createDefinition(defName);

                r.erase(r.begin());
                
                std::map<std::string, int> chanelDef;
                std::vector<int> blackoutMask;
                std::map<std::string, std::vector<std::string>> propsDef;
                
                
                std::vector<std::string> channelsNames;
                std::vector<int> defaultValues;
                
                for( auto rr : r ){
                    
//                    cout << "rr: " << rr << std::endl;
                    
                    auto nameValue = split( rr, '=' );
                    

                    
                    
                    if(  isInteger( nameValue[0] ) ){
                        // add a channel definitiion
                    
                        
                        auto values = split( nameValue[1], ',' );
                        
                        if( values.size() > 1 ){
                            defaultValues.push_back( stoi( values[1] ) );
                        }else{
                            defaultValues.push_back( 0 );
                        }
                        
                        chanelDef[ values[0] ] = stoi( nameValue[0] );
                        channelsNames.push_back( values[0] );
                        
                    }else if( nameValue[0] == "blackout" ){
                        
                        
                        auto values = split( nameValue[1], ',' );
                        
                        
                        
                        for(auto& v : values){
                            
                            if( isInteger( v ) ){
                                blackoutMask.push_back( stoi(v) );
                            }
                            
                        }
                        
                        

                    }else{
                        //if nameValue[0] is no a digit or blackout mask, it's a custom propriety
                        auto values = split( nameValue[1], ',' );
                        propsDef[ nameValue[0] ] = values;
                    }
                    

                }
                
                
                def->addMode(chanelDef);
                def->setChannelNames( channelsNames );
                def->setDefaultValue( defaultValues );
                def->setCustomPropreties(propsDef);
                def->setBlackoutMask(blackoutMask);
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
                std::vector<std::string> propValues{ name };
                mFix->customProp["model"] = propValues;

				
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

//                        cout << "prop name: " << nameValue[0] << std::endl;
//                        cout << "prop value: " << nameValue[1] << std::endl;
                        
                        auto values = split(nameValue[1], ',');
                        
                        mFix->customProp[nameValue[0]] = values;
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
                    
                    
                    auto exists = std::find( fix->customProp[name].begin(), fix->customProp[name].end(), value );
                    
                    if( exists != fix->customProp[name].end() ){
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
                    
                    for( auto& v : fix->customProp[name] ){
                        result[v].push_back( fix );
                    }
                }
            }
            return result;
        }
        
        
        FixtureRef getFixtureById( int id_ ){
            
            for(int i = 0; i < mFixtures.size(); i++){
                if( mFixtures[i]->getId() == id_ ){
                    return mFixtures[i];
                }
            }
            
            return nullptr;
        }
        
        //
        std::vector< ofix::FixtureRef > getFixturesWithDefinitionName( std::string name ){
            
            std::vector< ofix::FixtureRef > result;
            auto def = Definition::getDefinitionByName(name);
            
            return result;
        }
        
        
        Definition* getDefinitionByName( std::string name ){
            return Definition::getDefinitionByName(name);
        }

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
