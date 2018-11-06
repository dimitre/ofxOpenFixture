#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	artnets.setup(computerIP.c_str());
	mOfxx.loadFixturesDefFromFolder("_dmx/_fixtures/");
	mOfxx.loadUniversesDefFromFolder("_dmx/_universes/");
	mOfxx.setUniversesProps();
	
	
	imageFixture.allocate(20,26, OF_IMAGE_COLOR);
	imageFixture.setColor(ofColor(0, 0,0, 0));
	imageFixture.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);

}

//--------------------------------------------------------------
void ofApp::update(){
	if (artnets.status == 2)
	{
		auto universes = mOfxx().getUniverses();
		for (int a=0; a<universes.size(); a++) {
			int universo = mOfxx().getUniverses()[a]->universeIndex - 1;
			artnets.sendDmx(artnetIP, 0, universo, mOfxx().getUniverses()[a]->getBuffer().data(), 512);
		}
	}
}


//--------------------------------------------------------------
void ofApp::draw(){
	
	auto universes = mOfxx().getUniverses();
	
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(10,0);
	
	for (int a=0; a<universes.size(); a++) {
		ofSetColor ( dmtrFixturesDrawColors[a%5] );
		auto universe = universes[a];
		renderUniverse(&imageFixture, *universe );
		float scale = 3;
		float scaledSizeX = imageFixture.getWidth() * scale;
		float scaledSizeY = imageFixture.getHeight() * scale;
		
		imageFixture.draw( (scaledSizeX + 10 )* a  , ofGetWindowSize().y - scaledSizeY - 10, imageFixture.getWidth() * scale, imageFixture.getHeight() * scale );
		
		ofDrawBitmapString( ofToString( universes[a]->universeIndex),  (scaledSizeX + 10 )* a,  ofGetWindowSize().y - scaledSizeY - 20   );
	}
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

