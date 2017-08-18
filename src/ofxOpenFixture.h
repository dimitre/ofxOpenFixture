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
#include "Fixture.h"
#include "OpenFixtureDmtr.h"


namespace openfixture {
    
    void renderUniverse(ofImage* image, Universe& uni, int columns = 20) {
        
        auto dmxDatas = uni.getBuffer();
        
        for (int a=0; a<512; a++) {
            int x = a % columns;
            int y = a / columns;
            
            image->setColor(x ,y  , ofColor((int)dmxDatas[a]));
        }
        image->update();
    }
}



#endif /* ofxOpenFixture_h */
