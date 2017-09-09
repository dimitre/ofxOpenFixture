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
        
        ofix::Definition* createDefinitionFromScheme(const std::vector< std::vector<std::string>>& schema, bool channelStartsAtOne = true ){
            
            ofix::Definition* def = nullptr;
            for(auto defString :  schema){
                
                std::string defName = defString[0];
                
                def = ofix::Definition::createDefinition(defName);

                defString.erase(defString.begin());
                
                std::map<std::string, int> chanelDef;
                std::vector<int> blackoutMask;
				bool dimmerExists = false;
                std::map<std::string, std::vector<std::string>> propsDef;
                
                
                std::vector<std::string> channelsNames;
                std::vector<int> defaultValues;
                
                
                for( auto channelString : defString ){
                    
                    auto nameValue = split( channelString, '=' );

                    if(  isInteger( nameValue[0] ) ){
                        // add a channel definitiion
                        auto values = split( nameValue[1], ',' );

                        if( values.size() > 1 ){
                            // set a default value
                            int v = stoi( values[1] );
                            defaultValues.push_back( v );
                            
                        }else{
                            //if there is no default value, set it to 0
                            defaultValues.push_back( 0 );
                        }
                        
                        int nameValueInt = stoi( nameValue[0] );
                        
                            if( channelStartsAtOne ){
                                     nameValueInt -= 1;
                                
                                if( nameValueInt < 0 ){
                                    nameValueInt = 0;
                                    cout << "error, fixture channel starting at 0 and channelStartsAtOne equals true: " << defName << std::endl;
                                }
                            }
						
                        chanelDef[ values[0] ] = nameValueInt;
                        channelsNames.push_back( values[0] );

						if( values[0] == "dimmer" ){
							cout << "channelDef:  " << defName <<  values[0] << " NVI " << nameValueInt << std::endl;
							blackoutMask.push_back( nameValueInt );
							dimmerExists = true;
						}
						
                        
                    }
					
					else if( nameValue[0] == "blackout" ){
						
						if( dimmerExists == false ){

								auto values = split( nameValue[1], ',' );
								for(auto& v : values){
									
									if( isInteger( v ) ){
										blackoutMask.push_back( stoi(v) -1 );
									}
									
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
        
        
        void appendSchemeToDefinition(ofix::Definition* def,  const std::vector< std::vector<std::string>>& schema, bool channelStartsAtOne = true ){

            for(auto defString :  schema){
             
                std::string defName = defString[0];
                defString.erase(defString.begin());

                
                std::map<std::string, int> chanelDef;
                std::vector<int> blackoutMask;
                bool dimmerExists = false;
                std::map<std::string, std::vector<std::string>> propsDef;
                
                std::vector<std::string> channelsNames;
                std::vector<int> defaultValues;
                
                
                for( auto channelString : defString ){
                    
                    auto nameValue = split( channelString, '=' );
                    
                    if(  isInteger( nameValue[0] ) ){
                        // add a channel definitiion
                        auto values = split( nameValue[1], ',' );
                        
                        if( values.size() > 1 ){
                            // set a default value
                            int v = stoi( values[1] );
                            defaultValues.push_back( v );
                            
                        }else{
                            //if there is no default value, set it to 0
                            defaultValues.push_back( 0 );
                        }
                        
                        int nameValueInt = stoi( nameValue[0] );
                        
                        if( channelStartsAtOne ){
                            nameValueInt -= 1;
                            
                            if( nameValueInt < 0 ){
                                nameValueInt = 0;
                                cout << "error, fixture channel starting at 0 and channelStartsAtOne equals true: " << defName << std::endl;
                            }
                        }
                        
                        chanelDef[ values[0] ] = nameValueInt;
                        channelsNames.push_back( values[0] );
                        
                        if( values[0] == "dimmer" ){
                            cout << "channelDef:  " << defName <<  values[0] << " NVI " << nameValueInt << std::endl;
                            blackoutMask.push_back( nameValueInt );
                            dimmerExists = true;
                        }
                        
                        
                    }//eof channel props
                    
                    else if( nameValue[0] == "blackout" ){
                        
                        if( dimmerExists == false ){
                            
                            auto values = split( nameValue[1], ',' );
                            for(auto& v : values){
                                
                                if( isInteger( v ) ){
                                    blackoutMask.push_back( stoi(v) -1 );
                                }
                                
                            }
                            
                        }
                        
                    }else{
                        //if nameValue[0] is no a digit or blackout mask, it's a custom propriety
                        auto values = split( nameValue[1], ',' );
                        propsDef[ nameValue[0] ] = values;
                    }
                    
                    
                }
                
                if(chanelDef.size())
                    def->addMode(chanelDef);
                
                if(channelsNames.size())
                    def->setChannelNames( channelsNames );
                
                if( defaultValues.size() )
                    def->setDefaultValue( defaultValues );
                
                if(propsDef.size())
                    def->setCustomPropreties(propsDef);
                
                if( blackoutMask.size())
                    def->setBlackoutMask(blackoutMask);

            }
        }
        
        
        
        ofix::Universe* createUniverseFromScheme( const std::vector< std::vector<std::string>>& scheme, std::string uniName = "", bool channelStartsAtOne = true ){

            std::unique_ptr<ofix::Universe> uni( new Universe() );
            for(auto fix : scheme){
                
                std::string name = fix[0];
                
                //std::cout << name << std::endl;
                
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

                        int value = stoi( nameValue[1] );
                        
                        if( channelStartsAtOne == true){
                            value -= 1;
                            
                            if( value < 0 ){
                                value = 0;
                                cout << "error, universe channel starting at 0 and channelStartsAtOne equals true" << std::endl;
                            }
                            
                        }
                        
                        channel = value;
                    }else if( nameValue[0] == "mode" ){
                        
                        int value = stoi( nameValue[1] );
                        mFix->setMode(value);
                        
                    }else{
                        
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
        
        Fixture* getFixtureByModelId(string modelName, int id_){
            
            
            auto def = getDefinitionByName(modelName);
            
            
            auto fixtures = def->getFixtures();
            
            
            for(auto& f : fixtures ){
                
                if( f->getModelId() == id_ ){
                    return f;
                }
                
            }
            
        }
        
        
		
		// DIMITRE, experimental - 04/09/2017
		// Funciona mas isso pode ser um lookup table. ponteiros ou algo.
		// os cruzamentos ja sao computados no carregar do software.
		int getNumFixturesByModel(string model) {
			int count = 0;
			for(auto& fix : mFixtures){
				string name = "model";
				if( fix->customProp.find( name ) != fix->customProp.end() ){
					auto exists = std::find( fix->customProp[name].begin(), fix->customProp[name].end(), model );
					if( exists != fix->customProp[name].end() ){
						count ++;
					}
				}
			}
			return count;
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
        std::vector< ofix::Fixture* > getFixturesWithDefinitionName( std::string name ){
            
            std::vector< ofix::Fixture* > result;
            auto def = Definition::getDefinitionByName(name);
			result = def->getFixtures();
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
		
		
		void update() {
			for (auto & fix : mFixtures) {
				fix->updateEasing();
			}
		}
        
		
		std::vector<FixtureRef> getFixtures(){
			return mFixtures;
		}
		
		
		
    private:
        
        //
        std::vector<FixtureRef> mFixtures;
        std::map<std::string, std::unique_ptr<Universe> > mUniverses;
    };
    
}
