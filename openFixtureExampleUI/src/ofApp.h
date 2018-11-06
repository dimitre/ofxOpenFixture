#pragma once

#include "ofMain.h"
#include "ofxDmtrUI3.h"

#include "ofxArtnet.h"
#include "ofxOpenFixture.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
	
	void uiEvents(uiEv & e);
	
	ofxDmtrUI3 u;
	ofxDmtrUI3 *ui = &u.uis["ui"];
	ofxDmtrUI3 *uiC  = &u.uis["uiscene"];
	
	ofFbo *fbo = &u.mapFbos["fbo"];
	string *cena = &ui->pString["scene"];
	
	
	
//	ofix::ofxOpenFixture mOfxx;
//	ofxArtnet artnets;
//	string computerIP = "192.168.0.10";
//	string artnetIP = "192.168.0.30";
//	ofImage imageFixture;
//	
//	ofColor dmtrFixturesDrawColors[5] = { ofColor(244, 0,0 ),  ofColor(244, 0,255 ), ofColor(0, 255,0 ), ofColor(244, 255,255 ), ofColor(127, 255, 0 )  };

	
	// Placeholder variables and functions
	float updown, beat, audio;
	ofColor getCor(float p) { return ofColor(255); }
	
#include "DmtrFixture.h"

};
