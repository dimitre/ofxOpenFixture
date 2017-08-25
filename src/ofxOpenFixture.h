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
            
            cout << dir.getPath(i) << std::endl;
            
            mOfix.createDefinitionFromScheme( ofix::loadSchemeFromString( ofBufferFromFile( dir.getPath(i) ).getText() ) );

            
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



#endif /* ofxOpenFixture_h */
