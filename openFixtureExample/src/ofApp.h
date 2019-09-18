#pragma once

#include "ofMain.h"
#include "ofxArtnet.h"
#include "ofxOpenFixture.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
	
	
	ofix::ofxOpenFixture openFixture;
	ofxArtnetSender artnets;
	string computerIP = "192.168.0.10";
	string artnetIP = "192.168.0.30";

	ofImage imageFixture;
	ofColor dmtrFixturesDrawColors[5] = { ofColor(244, 0,0 ),  ofColor(244, 0,255 ), ofColor(0, 255,0 ), ofColor(244, 255,255 ), ofColor(127, 255, 0 )  };

};
