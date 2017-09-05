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
        
        std::string defNameFiltered = filterString(jsonobj["shortName"].asString());
        
        auto def = Definition::createDefinition( defNameFiltered );
        
        
        for( const auto& j : jsonobj["modes"] ){
            
            std::map<string, int> channelDef;
            std::vector<std::string> channelsNames;
			
			std::vector<int> blackoutMask;
			bool dimmerExists = false;
			
            int channelIndex = 0;
            
            int index = 0;
            for( const auto& channelName : j["channels"]){
                
                std::string channelNameFiltered = filterString(channelName.asString());
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
        
        void loadFixturesDefFromFolder( std::string folder_path = "_fixtures/" ){
            ofDirectory dir;
            dir.open( folder_path );
            for( int i = 0; i < dir.listDir(); i++  ){
                
                ofFile file = dir.getFile(i);
                
                //cout << file.getExtension()  << std::endl;
                
                if( file.getExtension() == "txt" ){
                    mOfix.createDefinitionFromScheme( ofix::loadSchemeFromString( ofBufferFromFile( dir.getPath(i) ).getText() ) );
                }
                
                
                else if( file.getExtension() == "json" ){
                    createDefinitionFromJson( ofBufferFromFile( dir.getPath(i) ).getText() ) ;
                }
            }
            
        }
        
        
        void loadUniversesDefFromFolder( std::string folder_path = "_universes/"  ){
            
            ofDirectory dir;
            dir.open( folder_path );
            
            for( int i = 0; i < dir.listDir(); i++  ){
                
                mOfix.createUniverseFromScheme( ofix::loadSchemeFromString( ofBufferFromFile( dir.getPath(i) ).getText() ) );
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
