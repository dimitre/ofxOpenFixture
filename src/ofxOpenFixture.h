//
//  ofxOpenFixture.h
//  OpenFixture
//
//  Created by Henrique on 8/17/17.
//
//

#ifndef ofxOpenFixture_h
#define ofxOpenFixture_h

#include "ofMain.h"

#include "OpenFixtureDmtr.h"
#include "OpenFixture.h"

#include "ofxJSON.h"

namespace openfixture{
    
    
    inline Definition* createDefinitionFromJson(const std::string& json_string ){
        ofxJSON jsonobj( json_string );
		
        std::string defNameFiltered = filterString(jsonobj["fixtureKey"].asString());
		if (defNameFiltered == "") {
			defNameFiltered = filterString(jsonobj["name"].asString());

//			cout << "fixtureKey" << endl;
//			cout << jsonobj["fixtureKey"].asString() << endl;
//			cout << "manufacturerKey" << endl;
//			cout << jsonobj["manufacturerKey"].asString() << endl;
//			cout << "oflURL" << endl;
//			cout << jsonobj["oflURL"].asString() << endl;
		}
		cout << "createDefinitionFromJson name :: " << defNameFiltered << endl;
        auto def = Definition::createDefinition( defNameFiltered );
		
		
        for( const auto& j : jsonobj["modes"] ){
            
            std::map<string, int> channelDef;
            std::vector<std::string> channelsNames;
			
			std::vector<int> blackoutMask;
			bool dimmerExists = false;
			
            int channelIndex = 0;
            
            int index = 0;
            for( const auto& channelName : j["channels"]){
                
                std::string channelNameFiltered = filterString(channelName.asString(), true);
                channelsNames.push_back(channelNameFiltered );
                channelDef[channelNameFiltered] = index;
				
				if( channelNameFiltered == "dimmer" ){
					blackoutMask.push_back( index );
				}
				
                index++;
            }
            
            def->addMode( channelDef );
            def->setChannelNames(channelsNames );
			def->setBlackoutMask(blackoutMask);
        }
        return def;
    }
    
    
    inline void renderUniverse(ofImage* image, ofix::Universe& uni, int columns = 20) {
        
        auto dmxDatas = uni.getBuffer();
        
        for (int a=0; a<512; a++) {
            int x = a % columns;
            int y = a / columns;
            
            image->setColor(x ,y  , ofColor((int)dmxDatas[a]));
        }
        image->update();
    }
        
        
    class ofxOpenFixture{
    public:
        ofxOpenFixture(){
        }
        
        void loadFixturesDefFromFolder( std::string folder_path = "_dmx/_fixtures/" ){
			if (ofFile::doesFileExist(folder_path)) {
				ofDirectory dir;
				dir.open( folder_path );
				for( int i = 0; i < dir.listDir(); i++  ){
					
					ofFile file = dir.getFile(i);
					
					//cout << file.getExtension()  << std::endl;
					
					if( file.getExtension() == "txt" ){
						mOfix.createDefinitionFromScheme( ofix::loadSchemeFromString( ofBufferFromFile( dir.getPath(i) ).getText() ) );
					}
					
					else if( file.getExtension() == "json" ){
						Definition* def = createDefinitionFromJson( ofBufferFromFile( dir.getPath(i) ).getText() ) ;
						std::string additionalInfoFile = ofSplitString(dir.getFile(i).getFileName(), ".")[0] + ".txt";
						
						string file = folder_path + "extensions/" + additionalInfoFile;
						//cout << file << endl;
						
						ofFile extensionFile( ofToDataPath( file ) );
						
						if( extensionFile.exists()  ){
							cout << "using extensionFile ::" << file << endl;
							std::vector <std::vector <std::string>> scheme = ofix::loadSchemeFromString( ofBufferFromFile( extensionFile.path() ).getText() );
							mOfix.appendSchemeToDefinition( def, scheme );
						}
					}
				}
			}
        }
        
        
        void loadUniversesDefFromFolder( std::string folder_path = "_dmx/_universes/"  ){
			if (ofFile::doesFileExist(folder_path)) {

				ofDirectory dir;
				dir.open( folder_path );
				for( int i = 0; i < dir.listDir(); i++  ){
					ofFile file(dir.getPath(i));
					string uniname = ofSplitString(file.getFileName(), ".")[0] ;
					mOfix.createUniverseFromScheme( ofix::loadSchemeFromString( ofBufferFromFile( dir.getPath(i) ).getText() ), uniname );
				}
			}
        }
        
        
        void setUniversesProps( std::string file_path = "_dmx/ips.txt" ){
            
            std::ifstream file(  ofToDataPath(file_path) );
            
            std::string line;
            
            std::string currentIp = "";
            int currentIndex = 0;
            
            while( std::getline( file, line ) )
            {
                auto s = filterString(line);
                
                if( s[0] == '[' ){
                    // start a new fixture scheme
//                    cout << "current IP" << endl;
                    currentIp = s;
                    currentIp.pop_back();
                    currentIp.erase(0,1);
                }
                
                else if(s[0] == '#' || s == ""){
                    // ignore comments & empty lines
                    continue;
                }
                else {
                    // add properties
                    
                    if (currentIp == "" ){
                        std::cout << "error setting value without a ip address" << std::endl;
                    } else {

                        vector<string> uniId = ofix::split(line, '_');

                        int index = stoi(uniId[1]);
                        mOfix.setUniversePropreties(line, currentIp,  index );
                        
                    }
                }
            }
        }

        
        ofix::OpenFixture& operator()(){
            
            return mOfix;
        }
        
        
        std::string myComputerIp = "192.168.0.1";
        ofix::OpenFixture mOfix;

    };


}

#endif /* ofxOpenFixture_h */
