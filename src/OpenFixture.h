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
        
        class Scheme{
            
            
            
        };
        
        public:
        
        ofix::Definition* createDefinitionFromScheme(const std::vector< std::vector<std::string>>& schema ){
            
            ofix::Definition* def = nullptr;
            for(auto defString :  schema){
                
                std::string defName = defString[0];
                
                def = ofix::Definition::createDefinition(defName);

                defString.erase(defString.begin());
                
                std::map<std::string, int> chanelDef;
                std::vector<int> blackoutMask;
                std::map<std::string, std::vector<std::string>> propsDef;
                
                
                std::vector<std::string> channelsNames;
                std::vector<int> defaultValues;
                
                for( auto channelString : defString ){
                    
                    auto nameValue = split( channelString, '=' );

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
                
                std::cout << name << std::endl;
                
                fix.erase(fix.begin());

                auto defGlobal = ofix::Definition::getDefinitionByName( name );
                
                if( defGlobal == nullptr){
                    
                    std::cout << "error no definition name: " << name << std::endl;
                    continue;
                }
                
                //std::cout << "ok..." << name << std::endl;
                
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
                        int value = stoi( nameValue[1] ) - 1;
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
		
		
		// DIMITRE
		void setFixturesChannelByModel(string modelName, string channelName, int val) {
			for(auto & fix : mFixtures){
				string name = "model";
				if( fix->customProp.find( name ) != fix->customProp.end() ){
					auto exists = std::find( fix->customProp[name].begin(), fix->customProp[name].end(), modelName );
					if( exists != fix->customProp[name].end() ){
                        
                        
						fix->setChannelByName(channelName, val);
					}
				}
			}
		}
		
        void setFixturesChannelByModelId( string modelName, int id_,  int val ){
            
            
            
            
        }
        
		
		// DIMITRE
		vector <string> getPropertiesWithPropertiesValue(string name, string value) {
			vector <string> result;
			for(auto& fix : mFixtures){
				if( fix->customProp.find( name ) != fix->customProp.end() ){
					auto exists = std::find( fix->customProp[name].begin(), fix->customProp[name].end(), value );
					if( exists != fix->customProp[name].end() ){
						for (auto & c : fix->customProp[name]) {
							if (c != value && std::find( result.begin(), result.end(), c) == result.end()) {
								result.push_back(c);
							}
						}
					}
				}
			}
			return result;
		}
		
		// DIMITRE
		vector <string> getFixtureNamesWithPropertiesValue(string name, string value) {
			cout << "===== BEGIN" << endl;
			vector <string> result;
			for(auto& fix : mFixtures){
				//cout << fix->getDefinitionPtr()->getModelName() << endl;;
				if( fix->customProp.find( name ) != fix->customProp.end() ){
					auto exists = std::find( fix->customProp[name].begin(), fix->customProp[name].end(), value );
					if( exists != fix->customProp[name].end() ){
						string c = fix->getDefinitionPtr()->getModelName();
						//if (c != value && std::find( result.begin(), result.end(), c) == result.end())
						{
							result.push_back(c);
						}
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
        
        
        std::vector< ofix::Definition* > getDefinitionsWithProperties( std::string name ){
            
            std::vector< ofix::Definition*> result;
            
            auto definitions = Definition::getDefinitions() ;
            
            for(auto& def  :  definitions){
                
                if( def->hasCustomPropretie( name )  ){
                    result.push_back( def );
                }
                
            }

            return result;
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
        
        
        void setChannelInGroup( int channel, int value, std::string groupName, std::string groupTag = "group" ){
            
            for( auto& fix : getFixturesWithPropertiesValue(groupTag, groupName)){
                fix->setChannel(channel, value);
            }

        }
        
        
    private:
        
        //
        std::vector<FixtureRef> mFixtures;
        std::map<std::string, std::unique_ptr<Universe> > mUniverses;
    };
    
}
