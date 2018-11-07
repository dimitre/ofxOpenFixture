/*

You don't really need this file.
We use as a dirty piece of code to make it run together with a graphical interface.
it doesn't even worth to read.

*/




ofix::ofxOpenFixture mOfxx;

// 6 de outubro de 2017, babel, inserindo artnets aqui dentro
ofxArtnet artnets;
string computerIP, artnetIP;

ofImage imageFixture;

vector <string> fixturesWithUI;
bool isBeat = false;


ofPixels pixels;

// REMOVER
ofxDmtrUI3 *uiDmx = &u.uis["uiDmx"];
ofxDmtrUI3 *uiDmxC = &u.uis["uisceneDmx"];
ofFbo *fboDmxInspector = &u.mapFbos["fboDmxInspector"];;
ofFbo *fboDmxScene = &u.mapFbos["fboDmxScene"];;


//--------------------------------------------------------------
void dmtrFixturesSetup() {
	
	for (auto & l : u.textToVector("_dmx/artnet_ip.txt")) {
		vector <string> col = ofSplitString(l, "	");
//		if (col[0] == "computer") {
		if (col[0] == "computer" && ofFile::doesFileExist("/Users/"+col[2])) {
			computerIP = col[1];
			cout << "artnets setup, computer ip = " + computerIP << " :: user = " << col[2] << endl;
		}
		else if (col[0] == "artnet") {
			artnetIP = col[1];
		}
	}
	
	// ISIS Computer
//	if (!ofFile::doesFileExist("/Users/dimitre")) {
//		computerIP = "192.168.0.11";
//		cout << "artnets setup, computer ip = " + computerIP << endl;
//		cout << "isis computer" << endl;
//	} else {
//		cout << "artnets setup, computer ip = " + computerIP << endl;
//		cout << "dimi computer" << endl;
//	}
	
	
	artnets.setup(computerIP.c_str());
	
	mOfxx.loadFixturesDefFromFolder("_dmx/_fixtures/");
	mOfxx.loadUniversesDefFromFolder("_dmx/_universes/");
    mOfxx.setUniversesProps();
    
	auto palcoFixtures = mOfxx().getFixturesWithProperties("group");
	for (auto & p : palcoFixtures) {
		uiDmx->templateVectorString["grupos"].push_back(p.first);
	}
	
	auto models =  mOfxx().getFixturesWithProperties("model");
	for (auto & p : models) {
		uiDmx->templateVectorString["equipamentos"].push_back(p.first);
	}
	
	uiDmx->templateVectorString["palco"] = mOfxx().getPropertiesWithPropertiesValue("group", "palco");
	uiDmx->templateVectorString["general"] = mOfxx().getFixtureNamesWithPropertiesValue("group", "general");
	
	{
		auto models =  mOfxx().getFixturesWithProperties("model");
		for (auto & modelGroup : models) {
			
			auto defPtr = mOfxx().getDefinitionByName(modelGroup.first);
			
			if(defPtr == nullptr){
				continue;
			}
			
			if( defPtr->hasCustomPropretie( "ui" ) && defPtr->getPropretieValues("ui")[0] == "0" ){
				continue;
			}
			
			string model = modelGroup.first;


			// dimitre experimental test
//			if( defPtr->hasCustomPropretie( "ui" ) && defPtr->getPropretieValues("ui")[0] == "2" ){
//				int mode = modelGroup.second[0]->getMode();
//				vector <string> params;
//				for(  auto name : defPtr->getChannelNames(mode ) ){
//					if (name != "null") {
//						params.push_back(name);
//						if( defPtr->hasCustomPropretie("audio") ){
//							auto audioChannels = defPtr->getCustomPropreties()["audio"];
//							for( auto& ac : audioChannels ){
//								if( stoi(ac) == defPtr->getChannelIndexByName(name) ){
//									params.push_back(name + "Audio");
//								}
//							}
//						}
//					}
//				}
//				vector <string> lines;
//				lines.push_back("label	" + model);
//				for (auto & p : params) {
//					lines.push_back("int	"+p+"	0 255 127");
//					//uiDmx->createFromLine("int	"+p+"	0 255 127");
//				}
//				
//				
//				vector <string> togglesOn;
//				for (auto & p : mOfxx().getFixturesWithPropertiesValue("model", model)) {
//					togglesOn.push_back("on_" + model + "_" + std::to_string( p->getModelId()));
//				}
//				lines.push_back("togglesListNoLabel	"+ofJoinString(togglesOn, " ")+"	1");
//				
//				
//				uiDmx->createFromLines(lines);
//				
//			}


			fixturesWithUI.push_back(model);
			
			string uiname = "ui_" + model;
			
			string uiSmall = "";
			if( defPtr->hasCustomPropretie( "ui" ) ){
				uiSmall = defPtr->getPropretieValues("ui")[0];
			}
			
			if (uiSmall == "small") {
				u.templateUI["customUIs"].push_back("addUIDown	"+uiname+"	text:uiFixtureSmall");
			}
			else if (uiSmall == "smallnewcol") {
				u.templateUI["customUIs"].push_back("addUI	"+uiname+"	text:uiFixtureSmall");
			}
			
			
			else  {
				u.templateUI["customUIs"].push_back("addUI	"+uiname+"	text:uiFixture");
				u.templateUI["customUIs"].push_back("addUIDown	uiscene_"+model+"");
			}
			
			
			if( defPtr->hasCustomPropretie("description")){
				string desc = defPtr->getPropretieValues("description")[0];
				u.uis[uiname].createFromLine("label	"+ desc);
			}
			
			u.uis[uiname].templateVectorString["label"].push_back( model );
			
			
			//u.uis[uiname].templateVectorString["scene"].push_back( model );
			u.uis[uiname].templateVectorString["scene"].push_back( model );
			

			vector <string> groups;
			for (auto & fix : mOfxx().getFixturesByModel(model)) {
				for (auto & g : fix->customProp["group"]) {
					if (std::find(groups.begin(), groups.end(),g)==groups.end()) {
						groups.push_back(g);
					}
				}
			}
			u.uis[uiname].templateVectorString["groups"] = groups;


			//cout << defPtr->customPropreties["group"].size() << endl;
			
			
            int mode = modelGroup.second[0]->getMode();
            
//            if(mode != 0){
//                cout << mode << endl;
//            }
//            
            
			for(  auto name : defPtr->getChannelNames(mode ) ){
				if (name != "null") {
					u.uis[uiname].templateVectorString["channels"].push_back( name );
					if( defPtr->hasCustomPropretie("audio") ){
						auto audioChannels = defPtr->getCustomPropreties()["audio"];
						for( auto& ac : audioChannels ){
							if( stoi(ac) == defPtr->getChannelIndexByName(name) ){
								u.uis[uiname].templateVectorString["channels"].push_back( name + "Audio" );
							}
						}
					}
					if( defPtr->hasCustomPropretie("beat") ){
						auto audioChannels = defPtr->getCustomPropreties()["beat"];
						for( auto& ac : audioChannels ){
							if( stoi(ac) == defPtr->getChannelIndexByName(name) ){
								u.uis[uiname].templateVectorString["channels"].push_back( name + "Beat" );
							}
						}
					}

				
				}
			}
			
			if( defPtr->hasCustomPropretie("colorpicker") || defPtr->hasCustomPropretie("colorPicker") ){
				u.uis[uiname].templateVectorString["colorPicker"].push_back("colorHsv	cor");
				u.uis[uiname].templateVectorString["colorPicker"].push_back("bool	useWhite	1");
			}
				
            // -------
            if( defPtr->hasCustomPropretie("single") ){
                
                cout << "---- single props ------ id: " << std::endl;

                
                auto singleChannels = defPtr->getCustomPropreties()["single"];
				
				u.uis[uiname].templateVectorString["individual"].push_back("");
				u.uis[uiname].templateVectorString["individual"].push_back("label	INDIVIDUAL");
				for( auto& single : singleChannels ){
					string channelName = defPtr->getChannelNames(0)[ stoi(single) -1 ];
					u.uis[uiname].templateVectorString["individual"].push_back("bool	individual_"+channelName+"	0");
				}

                for(auto& fix :  modelGroup.second){
					for( auto& single : singleChannels ){
						
						string uiname = "ui_" + model;
						string channelName = defPtr->getChannelNames(0)[ stoi(single) -1 ];
						u.uis[uiname].templateVectorString["individual"].push_back("int	"+channelName+"_"+ofToString(fix->getModelId())+"	0 255 0");
                    }
                }
            }
			
			for (auto & p : mOfxx().getFixturesWithPropertiesValue("model", model)) {
				u.uis[uiname].templateVectorString["on"].push_back( "on_" + model + "_" + std::to_string( p->getModelId() ) );
			}
		}
	}
	
	
	// removi dia 13 de setembro
	//mOfxx().setChannelInGroup( 0, 255, "palco" );

	
	// pqp nao consegui fazer isso rodar.
//	cout << "aqui roda" << endl;
//	{
//	auto models = mOfxx().getFixturesWithProperties("ignoremasterfade");
//		for (auto & modelGroup : models) {
//			modelGroup->setIgnoreMasterFade(true);
//			auto defPtr = mOfxx().getDefinitionByName(modelGroup.first);
//		}
//	}
	

	
	for (auto & p : mOfxx().getFixturesWithPropertiesValue("paninvert", "1")) {
		p->panInvert = true;
		cout << "PANINVERT" << endl;
	}
    
    for (auto & p : mOfxx().getFixturesWithPropertiesValue("tiltinvert", "1")) {
        p->tiltInvert = true;
		cout << "TILTINVERT" << endl;
    }
    
    imageFixture.allocate(20,26, OF_IMAGE_COLOR);
	imageFixture.setColor(ofColor(0, 0,0, 0));
	imageFixture.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);

//	for (int a=0; a<10; a++) {
//		uiDmx->templateVectorString["individual"].push_back("int	pan_"+ofToString(a)+"	0 255 0");
//	}

    
}



//--------------------------------------------------------------
void dmtrFixturesUpdate() {
	//cout << "dmtrFixturesUpdate()" << endl;
	// easing
	mOfxx().update();
	
	// tentando flipar o segundo fixture de laser
//	auto defs = mOfxx().getDefinitionsWithProperties("flip");
//	for( auto& def : defs){
//		string ui_name = "ui_" + def->getModelName();
//		auto channels = def->getCustomPropreties()["flip"];
//		for(int i = 0; i < channels.size(); i++){
//			int channel = stoi(channels[i]);
//			auto channel_name = def->getChannelNameByIndex(channel, 0);
//			//cout << channel_name << endl;
//			for( auto fix : def->getFixtures() ){
//				if (channel_name == "rotate" && fix->getModelId() == 0) {
//					int sliderValue = u.uis[ui_name].pInt[channel_name] + 64;
//					fix->setChannel(channel, sliderValue );
//				}
//			}
//		}
//	}

	auto defs = mOfxx().getDefinitionsWithProperties("audio");

	// AQUI SO ATUALIZA AS FIXTURES COM AUDIO
	for( auto& def : defs){
		auto channels = def->getCustomPropreties()["audio"];
		
		string ui_name = "ui_" + def->getModelName();
		for( auto fix : def->getFixtures() ){
			for(int i = 0; i < channels.size(); i++){
				int channel = stoi(channels[i]);
				auto channel_name = def->getChannelNameByIndex(channel, 0);
				int sliderValue = u.uis[ui_name].pInt[channel_name];
				float sliderAudioValue = updown * (float)u.uis[ui_name].pInt[channel_name + "Audio"];
				float sliderBeatValue = beat * (float)u.uis[ui_name].pInt[channel_name + "Beat"];
				int valTotal = sliderValue + sliderAudioValue + sliderBeatValue;
				fix->setChannel(channel, valTotal );
			}
		}
	}
	
	
#ifdef USE_ARTNET_LEGACY
	if (u.pBool["sendArtnet"] && artnets.status == 2) {
		auto universes = mOfxx().getUniverses();
		for (int a=0; a<universes.size(); a++) {
			int universo = mOfxx().getUniverses()[a]->universeIndex - 1;
			// mandar apenas o universo size ao inves dos 512?
			artnets.sendDmx(artnetIP, 0, universo, mOfxx().getUniverses()[a]->getBuffer().data(), 512);
		}
	}
#endif
	
	
	//if (u.pBool["sendArtnet"])
	ofPixels pixels;
	{
		auto universes = mOfxx().getUniverses();
		for (int a=0; a<universes.size(); a++) {
			int universo = mOfxx().getUniverses()[a]->universeIndex - 1;
			
			// mandar apenas o universo size ao inves dos 512?
			auto dmxData = mOfxx().getUniverses()[a]->getBuffer().data();
			pixels.setFromExternalPixels(dmxData, 30, 30, 1);
			artnets.sendArtnet(pixels, universo);
			//artnets.sendDmx(artnetIP, 0, universo, mOfxx().getUniverses()[a]->getBuffer().data(), 512);
		}
	}
}



ofColor dmtrFixturesDrawColors[5] = { ofColor(244, 0,0 ),  ofColor(244, 0,255 ), ofColor(0, 255,0 ), ofColor(244, 255,255 ), ofColor(127, 255, 0 )  };

//--------------------------------------------------------------
void dmtrFixturesDraw() {
	
	// IMAGES INSPECTOR
	
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
void dmtrFixturesDraw2() {
	// IMAGES INSPECTOR
	auto universes = mOfxx().getUniverses();
	
	ofPushStyle();
	ofPushMatrix();
	//ofTranslate(10,0);
	
	float scale = 3;
	float scaledSizeX = imageFixture.getWidth() * scale;
	float scaledSizeY = imageFixture.getHeight() * scale;

	float h = fboDmxInspector->getHeight();
	for (int a=0; a<universes.size(); a++) {
		ofSetColor(255);
		//ofSetColor ( dmtrFixturesDrawColors[a%5] );
		auto universe = universes[a];
		renderUniverse(&imageFixture, *universe );
		float y = 0;
		float x = (scaledSizeX*2 + 5 ) * a;
		imageFixture.draw(x , y, imageFixture.getWidth() * scale, imageFixture.getHeight() * scale );
		imageFixture.draw(x + scaledSizeX + 10, y - h, imageFixture.getWidth() * scale, imageFixture.getHeight() * scale );
		ofDrawBitmapString( ofToString( universes[a]->universeIndex),  (scaledSizeX + 10 )* a,  y );
	}
	ofPopMatrix();
	ofPopStyle();
}


//void getChannelFromInterface(FixtureRef f, ) {
void setChannelFromInterface(string modelName, string channel, int modelId = -1) {

	ofxDmtrUI3 * ui = &u.uis["ui_" + modelName];
	int val = ui->pInt[channel];
	
//	cout << modelName << endl;
//	cout << channel << endl;
//	cout << val << endl;
//	cout << "-----" << endl;

	auto def = mOfxx().getDefinitionByName(modelName);
	
	if( def->hasCustomPropretie("audio") ){
		val += ui->pInt[channel + "Audio"] * updown;
	}
	
	

	
	if( ui->pBool["individual_" + channel] ) {
		
		auto fixtures = mOfxx().getFixturesWithDefinitionName(modelName);
		for( auto& fix :  fixtures ){
			string channelName = channel + "_" + ofToString( fix->getModelId());
			int v =  ui->pInt[ channelName ];
//			if( !ui->pBool["on_" + modelName + "_" + ofToString( fix->getModelId() ) ] ){
//				v = 0;
//			}

			fix->setChannelByName( channel, v );  // + individual ?
		}
		
	} else {

		
		auto fixtures = mOfxx().getFixturesWithDefinitionName(modelName);
		for( auto& fix :  fixtures ){
			int v = val;
			
			
			
//			if( !ui->pBool["on_" + modelName + "_" + ofToString( fix->getModelId() ) ] ){
//				v = 0;
//			}
			
			
			
			// dimitre paninvert
			
			//fix->get
			
			// XAXA TODO
//			if (channel == "pan" && fix->getDefPtr()->hasCustomPropretie( "paninvert" )) {
//				v = 255 - v;
//			}
//			if (channel == "tilt" && fix->getDefPtr()->hasCustomPropretie( "tiltinvert" )) {
//				v = 255 - v;
//			}

			
			fix->setChannelByName( channel, v );
		}
		
	
	}

}


//--------------------------------------------------------------
void dmtrFixturesScene() {
	
	//audio = beat = updown;
	
	{
		string & scene = u.uis["uiDmx"].pString["sceneDmx"];
		if (scene == "alternate") {
			
			string f1 = u.uis["uisceneDmx"].pString["fixture1"];
			string f2 = u.uis["uisceneDmx"].pString["fixture2"];
			float t = u.uis["uisceneDmx"].pFloat["threshold"];
			float audioBeat = u.uis["uisceneDmx"].pBool["audio"] ? audio : beat;
			if (f1 != "_no") {
				for (auto & e : u.uis["ui_" + f1].elements) {
					if (e->tag == "on") {
						e->set(audioBeat > t);
					}
				}
			}
			
			if (f2 != "no") {
				for (auto & e : u.uis["ui_" + f2].elements) {
					if (e->tag == "on") {
						e->set(audioBeat < t);
					}
				}
			}
		}
		
//		else if (scene == "eachsupport") {
//			vector <string> suportes = ofSplitString("macaura macviperwash at3000 robespot2500 martinrushmh2 martinrushmh3 martinrushmh7 martinviperperf", " ");
//			uiC->pFloat["contagem"] += uiC->pFloat["vel"] + audio * uiC->pFloat["velAudio"] + beat * uiC->pFloat["velBeat"];
//			auto contagem = fmod(uiC->pFloat["contagem"] , suportes.size()) + uiC->pInt["c_step"];
//			int numOfFixtures =  uiC->pFloat["quantidade"] * suportes.size();
//
//			for (auto & s : suportes) {
//				u.uis["ui_" + s].set("allOff", true);
//				
//
//			}
//			int steps = uiC->pInt["c_step"];;
//			for (auto & s : suportes) {
//			}
//			
//			for(int k = 0; k < steps; k++){
//				
//				int initialIndex = (suportes.size() / steps) * k;
//				
//				for( int i = 0; i < numOfFixtures; i++ ){
//					int index = (int(contagem) + i+ initialIndex) % fixtures.size();
//					
//					
//					bool ligado = true;
//					string name = "on_" + c + "_" + ofToString(fixtures[index]->getModelId());
//					u.uis["ui_" + c].set(name, ligado);
//					
//				}
//			}
//		}
	}
	
	
	
	for (auto & c : fixturesWithUI) {
		ofxDmtrUI3 * uiC = &u.uis["uiscene_" + c];
		string & scene = u.uis["ui_" + c].pString["scene_" + c];

		
		// 28 / 12 / 2017 tentando testar mareh
		if (scene == "colorPicker") {
			auto fixtures = mOfxx().getFixturesWithDefinitionName(c);
			ofColor cor;
			for (auto & f : fixtures) {
				int index = f->getModelId();
				//cout << f->getModelId() << endl;
				cor = pixels.getColor(index * 9, 2);

				f->setChannelByName("red", cor.r);
				f->setChannelByName("green", cor.g);
				f->setChannelByName("blue", cor.b);
			}
			
		}
		
		else if (scene == "color") {
			auto fixtures = mOfxx().getFixturesWithDefinitionName(c);
			//ofColor cor = ui->pColor["cor"];
			ofColor cor = getCor(0);
			for (auto & f : fixtures) {
				int index = f->getModelId();
				
				f->setChannelByName("red", cor.r);
				f->setChannelByName("green", cor.g);
				f->setChannelByName("blue", cor.b);
			}

		
		}
		else if (scene == "contagem") {
			auto fixtures = mOfxx().getFixturesWithDefinitionName(c);
			
			uiC->pFloat["contagem"] += uiC->pFloat["vel"] + audio * uiC->pFloat["velAudio"] + beat * uiC->pFloat["velBeat"] ;
			
			auto contagem = fmod(uiC->pFloat["contagem"] , fixtures.size()) + uiC->pInt["c_step"];
			
            
			
            int numOfFixtures =  uiC->pFloat["quantidade"] * fixtures.size();
            int lastIndex = (int(contagem) - numOfFixtures) % fixtures.size();
            
            
    
            for( int i = 0; i < fixtures.size(); i++ ){
                bool ligado = false;
                string name = "on_" + c + "_" + ofToString(fixtures[i]->getModelId());
                u.uis["ui_" + c].set(name, ligado);
            }
            
            int steps = uiC->pInt["c_step"];;
            for(int k = 0; k < steps; k++){
                
                int initialIndex = (fixtures.size() / steps) * k;
                
                for( int i = 0; i < numOfFixtures; i++ ){
                    int index = (int(contagem) + i+ initialIndex) % fixtures.size();

                    
                    bool ligado = true;
                    string name = "on_" + c + "_" + ofToString(fixtures[index]->getModelId());
                    u.uis["ui_" + c].set(name, ligado);
                    
                }
            }

		}
		
		if (scene == "audioThreshold") {
			bool ligado = audio > uiC->pFloat["threshold"];
			auto fixtures = mOfxx().getFixturesWithDefinitionName(c);
			float ligados = audio * fixtures.size();
			for (auto & f : fixtures) {
				
				bool val = ofRandom(0,1) < uiC->pFloat["liga"];
				string name = "on_" + c + "_" + ofToString(f->getModelId());
				u.uis["ui_" + c].set(name, ligado);
			}
		}

		
		else if (scene == "audioBeat") {
			string p = uiC->pString["param"];
			if (p != "") {
				
				float a = audio * uiC->pFloat["audio"];
				float b = beat * uiC->pFloat["beat"];
				if (uiC->pFloat["audioNeg"]) {
					a = 255 - (audio * uiC->pFloat["audioNeg"]);
				}
				if (uiC->pFloat["beatNeg"]) {
					b = 255 - (beat * uiC->pFloat["beatNeg"]);
				}
				int v = ofClamp(ofMap(a + b, 0, 1, uiC->pFloat["min"], uiC->pFloat["max"]), 0, 255);
				u.uis["ui_" + c].set(p, v);
			}
		}
		
		else if (scene == "colorPeak") {
			string p = uiC->pString["param"];
			
			if (audio > uiC->pFloat["threshold"]) {
				
				ofColor cor = uiC->pColor["corPeak"];
				
				u.uis["ui_" + c].set("red",		uiC->pColor["corPeak"].r);
				u.uis["ui_" + c].set("green",	uiC->pColor["corPeak"].g);
				u.uis["ui_" + c].set("blue",	uiC->pColor["corPeak"].b);
				u.uis["ui_" + c].set("beamred",		uiC->pColor["corPeak"].r);
				u.uis["ui_" + c].set("beamgreen",	uiC->pColor["corPeak"].g);
				u.uis["ui_" + c].set("beamblue",	uiC->pColor["corPeak"].b);
				u.uis["ui_" + c].set("aurared",		uiC->pColor["corPeak"].r);
				u.uis["ui_" + c].set("auragreen",	uiC->pColor["corPeak"].g);
				u.uis["ui_" + c].set("aurablue",	uiC->pColor["corPeak"].b);
				
				
				u.uis["ui_" + c].set("cyan",			255-cor.r);
				u.uis["ui_" + c].set("magenta",		255-cor.g);
				u.uis["ui_" + c].set("yellow",		255-cor.b);

			}
			else {
				
				ofColor cor = uiC->pColor["cor"];

				u.uis["ui_" + c].set("red",		uiC->pColor["cor"].r);
				u.uis["ui_" + c].set("green",	uiC->pColor["cor"].g);
				u.uis["ui_" + c].set("blue",	uiC->pColor["cor"].b);
				u.uis["ui_" + c].set("beamred",		uiC->pColor["cor"].r);
				u.uis["ui_" + c].set("beamgreen",	uiC->pColor["cor"].g);
				u.uis["ui_" + c].set("beamblue",	uiC->pColor["cor"].b);
				u.uis["ui_" + c].set("aurared",		uiC->pColor["cor"].r);
				u.uis["ui_" + c].set("auragreen",	uiC->pColor["cor"].g);
				u.uis["ui_" + c].set("aurablue",	uiC->pColor["cor"].b);
				
				
				
				u.uis["ui_" + c].set("cyan",			255-cor.r);
				u.uis["ui_" + c].set("magenta",		255-cor.g);
				u.uis["ui_" + c].set("yellow",		255-cor.b);

			}
			
			

			if (uiC->pBool["tilt"]) {
				float val = uiC->pFloat["tilt"] + beat * uiC->pFloat["tiltBeat"] ;
				u.uis["ui_" + c].set("tilt", val);
			}
		}
		
		else if (scene == "_no") {
			auto fixtures = mOfxx().getFixturesWithDefinitionName(c);
			if (uiC->pBool["allOff"] || uiC->pBool["allOn"]) {
				for (auto & f : fixtures) {
					string name = "on_" + c + "_" + ofToString(f->getModelId());
					u.uis["ui_" + c].set(name, uiC->pBool["allOn"]);
				}
			}
		}
		
		else if (scene == "beat") {
			if (isBeat) {
				auto fixtures = mOfxx().getFixturesWithDefinitionName(c);
				float ligados = audio * fixtures.size();
				for (auto & f : fixtures) {
					bool val = ofRandom(0,1) < uiC->pFloat["liga"];
					string name = "on_" + c + "_" + ofToString(f->getModelId());
					u.uis["ui_" + c].set(name, val);
				}
			}
		}
		
		else if (scene == "audioOn") {
			auto fixtures = mOfxx().getFixturesWithDefinitionName(c);
			float ligados = audio * fixtures.size();
			
			for (auto & f : fixtures) {
				bool val = false;
				if (f->getModelId() < ligados) {
					val = true;
				}
				string name = "on_" + c + "_" + ofToString(f->getModelId());
				u.uis["ui_" + c].set(name, val);
			}
		}
		

		
		else if (scene == "random") {
			float fatorTempo = uiC->pFloat["fatorTempo"];

			for (auto & f : mOfxx().getFixturesWithDefinitionName(c)) {
				bool val = ofNoise(f->getModelId(), ofGetElapsedTimef() * fatorTempo) < uiC->pFloat["liga"];
				string name = "on_" + c + "_" + ofToString(f->getModelId());
				u.uis["ui_" + c].set(name, val);
				//uiDmx->getElement(f->nome)->set(val);
			}
		}
		
		else if (scene == "noise") {
			
			for (int a=0; a<2; a++) {
				string aa = ofToString(a);
				float fatorTempo = uiC->pFloat["fatorTempo" + aa];
				float fatorIndex = uiC->pFloat["fatorIndex" + aa];
				float valMiddle		= uiC->pFloat["val" + aa];
				float amplitude = uiC->pFloat["amplitude" + aa];
				
				{
					string p = uiC->pString["param" + aa];
					//cout << fatorIndex << endl;
					if (p != "") {
						
						for (auto & f : mOfxx().getFixturesWithDefinitionName(c)) {
							//cout << c << endl;
							//cout << fatorIndex * (float) f->getModelId() << endl;
							float val = ofNoise(fatorIndex * (float) f->getModelId(), ofGetElapsedTimef() * fatorTempo);
							float min = valMiddle - amplitude;
							float max = valMiddle + amplitude;
							float v = ofMap(val, 0, 1, min, max);
							v = int(ofClamp(v, 0, 255));
//							cout << p << endl;
//							cout << v << endl;
							f->setChannelByName(p, v);
							//u.uis["ui_" + c].set(p, v);

						}
					}
				}
			}

			// vindo do updown
			string p = uiC->pString["paramAudio"];
			if (p != "") {
				float val = audio * uiC->pFloat["audio"] + beat * uiC->pFloat["beat"];
				int v = ofClamp(ofMap(val, 0, 1, uiC->pFloat["min"], uiC->pFloat["max"]), 0, 255);
				u.uis["ui_" + c].set(p, v);
			}
		}
		
		else if (scene == "rnd") {
			auto fixtures = mOfxx().getFixturesWithDefinitionName(c);
			cout << c << endl;
			for (auto & f : fixtures) {
				f->setChannelByName("dimmer", ofRandom(0,255));
			}
			// numero de fixtures, iterar elas, setar dimmer = random.
		}
		
	}
	isBeat = false;
}




vector <string> masterControl;



//--------------------------------------------------------------
void dmtrFixturesUIEvent(uiEv & e) {
	
	//cout << e.name << endl;
	if (e.name == "Beat") {
		isBeat = true;
	}
	
	if (e.uiname == "uiDmx2") {
		if (e.name == "blinders") {
			for (auto & s : { "ui_blinder4x", "ui_blinder4x-line", "ui_blinder6x1st", "ui_blinder6x2nd" }) {
				u.uis[ s].set("dimmer", e.i);
			}
		}
		else if (e.name == "strobo") {
			u.uis["ui_at3000"].set("intensity", e.i);
		}
	}
	
	if (e.tag == "batchonoff") {
		//toggle_martinrushmh7_runway
		vector<string> partes = ofSplitString(e.uiname, "_");
		string model = partes[1];
		string group = e.name;
		
		for (auto & f : mOfxx().getFixturesByModel(model)) {
			string props = ofJoinString(f->customProp["group"], ",");
			if (ofIsStringInString(props, group)) {
				u.uis["ui_" + model].set("on_"+model+"_" + ofToString(f->getModelId()), (bool)e.b);
			}
		}
	}
	
	
	// NAME
	if (e.name == "allOn") {
		for (auto & el : u.uis[e.uiname].elements) {
			if (el->tag == "on" && el->kind == TOGGLE) {
				el->set(true);
			}
		}
	}
	else if (e.name == "allOff") {
		for (auto & el : u.uis[e.uiname].elements) {
			if (el->tag == "on" && el->kind == TOGGLE) {
				el->set(false);
			}
		}
	}
	
	else if (e.name == "masterFade"){
		openfixture::Fixture::masterFade = e.f;
	}
	
	else if (e.name == "easyIn"){
		//cout << "easyIn" << std::endl;
		for( auto& fix : mOfxx().getFixtures() ){
			fix->easyIn = e.f;
		}
	}
	
	else if (e.name == "easyOut"){
		//cout << "easyOut" << std::endl;
		for( auto& fix : mOfxx().getFixtures() ){
			fix->easyOut = e.f;
		}
	}
	
	
	if (e.uiname == "uiDmx" && e.tag == "palco") {
		mOfxx().setChannelInGroup( 0, e.i, e.name );
	}
	
	
	
	
	
	
	// TAG
	if (e.tag == "color") {
		string name = e.kind == SLIDER2D ? e.name : ofSplitString(e.name, "_")[0];
		ofColor cor = u.uis[e.uiname].pColor[name];
		
		u.uis[e.uiname].set("red",			cor.r);
		u.uis[e.uiname].set("green",		cor.g);
		u.uis[e.uiname].set("blue",			cor.b);
		u.uis[e.uiname].set("beamred",		cor.r);
		u.uis[e.uiname].set("beamgreen",	cor.g);
		u.uis[e.uiname].set("beamblue",		cor.b);
		u.uis[e.uiname].set("aurared",		cor.r);
		u.uis[e.uiname].set("auragreen",	cor.g);
		u.uis[e.uiname].set("aurablue",		cor.b);

		
		u.uis[e.uiname].set("cyan",			255-cor.r);
		u.uis[e.uiname].set("magenta",		255-cor.g);
		u.uis[e.uiname].set("yellow",		255-cor.b);
		
		// aqui eh o rgbw
		if (u.uis[e.uiname].pBool["useWhite"]) {
			float h, s, b;
			cor.getHsb(h, s, b);
			float invertSat = (255 - s) / 255.0;
			float sat = (s) / 255.0;
			
			u.uis[e.uiname].set("red",			cor.r * sat);
			u.uis[e.uiname].set("green",		cor.g * sat);
			u.uis[e.uiname].set("blue",			cor.b * sat);
			u.uis[e.uiname].set("white",			255 * invertSat);
		} else {
			u.uis[e.uiname].set("white",		0);
		}
	}
    
	else if (e.tag == "on") {
		string eventPrefix = ofSplitString( e.name, "_")[0];
		//cout << e.name << endl;
		if( eventPrefix == "on") {
			auto modelNameId = ofSplitString( e.name, "_");
			std::string modelName = modelNameId[1];
			std::string modelId = modelNameId[2];
			auto fix = mOfxx().getFixtureByModelId(modelName, stoi(modelId)  );
			fix->enableBlackout( !e.b );
		}
	}
	else if (e.tag == "individual") {
		string modelName = ofSplitString(e.uiname, "_")[1];
		vector <string> partes = ofSplitString(e.name, "_");
		string channel = partes[0];
		
		setChannelFromInterface( modelName, channel );
	}
	
	else if (e.tag == "channels" ) {
		// SET CHANNELS FROM INTERFACE
		// controle individual
		string modelName = ofSplitString(e.uiname, "_")[1];
		
		//aqui fazer o paninvert ou tiltinvert
		
		setChannelFromInterface(modelName, e.name);
	}

	else if (e.tag == "masterControl" ) {
		if (e.kind == TOGGLE || e.kind == RADIOITEM) {
			masterControl.clear();
			for (auto & el : uiDmx->elements) {
				if ((el->kind == TOGGLE || el->kind == RADIOITEM )&& el->getVal()==1) {
					masterControl.push_back(el->name);
				}
			}
		}
		else  {
			for (auto & m : masterControl) {
				u.uis["ui_"+m].set(e.name, e.i);
			}
		}
	}
	

}
