#include "ofApp.h"

ofPixels pixels;


//--------------------------------------------------------------
void ofApp::setup(){
	artnets.setup(artnetIP.c_str());
	
	openFixture.loadFixturesDefFromFolder("_dmx/_fixtures/");
	openFixture.loadUniversesDefFromFolder("_dmx/_universes/");
	openFixture.setUniversesProps();
	
	
	imageFixture.allocate(20,26, OF_IMAGE_COLOR);
	imageFixture.setColor(ofColor(0, 0,0, 0));
	imageFixture.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);

	// get all definitions
	
	for (auto & def : openFixture().getDefinitions()) {
		cout << def->getModelName() << endl;
		// esta função precisa passar o modo. deveria trabalhar com modo default também.
		//def->getChannelNames(0);
		for (auto & c : def->getChannelNames(0)) {
			cout << "-- " << c << endl;
		}
		cout << "---------------------" << endl;
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	auto universes = openFixture().getUniverses();
	for (int a=0; a<universes.size(); a++) {
		int universo = openFixture().getUniverses()[a]->universeIndex - 1;
		auto dmxData = openFixture().getUniverses()[a]->getBuffer().data();

		ofxArtnetMessage m;
		m.setData(openFixture().getUniverses()[a]->getBuffer().data(), 512);
		artnets.sendArtnet(m);
	}
}


//--------------------------------------------------------------
void ofApp::draw(){
	auto def = openFixture().getDefinitionByName("skypanel-s30c");
	for( auto fix : def->getFixtures() ){
		fix->setChannelByName("red", ofRandom(0,255));
		fix->setChannelByName("green", ofRandom(0,255));
		fix->setChannelByName("blue", ofRandom(0,255));
	}


	openFixture().update();

	// DRAW Inspectors
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(10,0);
	auto universes = openFixture().getUniverses();
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
