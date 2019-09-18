#include "ofApp.h"

ofPixels pixels;


//--------------------------------------------------------------
void ofApp::setup(){
	
	dmtrFixturesSetup();
	artnet.setup(artnetIP.c_str());
	u.createSoftwareFromText("u.txt");
	
	//u.setFbo(*fbo);
	
	for (auto & thisui : u.allUIs) {
		ofAddListener(thisui->settings.uiEvent,this, &ofApp::uiEvents);
	}
	u.loadMaster();

}

//--------------------------------------------------------------
void ofApp::update(){
	
	dmtrFixturesUpdate();
	//dmtrFixturesScene();
}


//--------------------------------------------------------------
void ofApp::draw(){
	dmtrFixturesDraw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}




//--------------------------------------------------------------
void ofApp::uiEvents(uiEv & e) {
	dmtrFixturesUIEvent(e);
}
