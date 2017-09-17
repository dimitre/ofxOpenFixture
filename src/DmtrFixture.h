ofix::ofxOpenFixture mOfxx;
ofImage image;


vector <string> fixturesWithUI;


bool isBeat = false;

//--------------------------------------------------------------
void dmtrFixturesSetup() {
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
						cout << g << endl;
					}
				}
			}
			u.uis[uiname].templateVectorString["groups"] = groups;


			//cout << defPtr->customPropreties["group"].size() << endl;
			
			
            int mode = modelGroup.second[0]->getMode();
            
            if(mode != 0){
                cout << mode << endl;
            }
            
            
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
				}
			}
			
			if( defPtr->hasCustomPropretie("colorpicker") || defPtr->hasCustomPropretie("colorPicker") ){
				u.uis[uiname].templateVectorString["colorPicker"].push_back("colorHsv	cor");
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
	
	
	
	for (auto & p : mOfxx().getFixturesWithPropertiesValue("paninvert", "1")) {
		p->panInvert = true;
	}
    
    for (auto & p : mOfxx().getFixturesWithPropertiesValue("tiltinvert", "1")) {
        p->tiltInvert = true;
    }
    
    
    
	
	
    image.allocate(20,26, OF_IMAGE_COLOR);
	image.setColor(ofColor(0, 0,0, 0));
	
//	for (int a=0; a<10; a++) {
//		uiDmx->templateVectorString["individual"].push_back("int	pan_"+ofToString(a)+"	0 255 0");
//	}

    
}



//--------------------------------------------------------------
void dmtrFixturesUpdate() {
	
	// easing
	mOfxx().update();
	
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
				int sliderAudioValue = u.uis[ui_name].pInt[channel_name + "Audio"];
				fix->setChannel(channel, sliderValue + sliderAudioValue * updown);
			}
		}
	}
}


//--------------------------------------------------------------
void dmtrFixturesDraw() {
	
	// IMAGES INSPECTOR
	
    auto universes = mOfxx().getUniverses();
    ofColor colors[5] = { ofColor(244, 0,0 ),  ofColor(244, 0,255 ), ofColor(0, 255,0 ), ofColor(244, 255,255 ), ofColor(127, 255, 0 )  };
	
	ofPushMatrix();
	ofTranslate(10,0);

	for (int a=0; a<universes.size(); a++) {
        ofSetColor ( colors[a%5] );
        auto universe = universes[a];
		renderUniverse(&image, *universe );
		image.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
		float scale = 3;
        float scaledSizeX = image.getWidth() * scale;
        float scaledSizeY = image.getHeight() * scale;
		
		image.draw( (scaledSizeX + 10 )* a  , ofGetWindowSize().y - scaledSizeY - 10, image.getWidth() * scale, image.getHeight() * scale );
        
        ofDrawBitmapString( ofToString( universes[a]->universeIndex),  (scaledSizeX + 10 )* a,  ofGetWindowSize().y - scaledSizeY - 20   );
	}
	ofPopMatrix();
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
		
	}else{

		
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
	
	
	
	{
		string & scene = u.uis["uiDmx"].pString["sceneDmx"];
		if (scene == "alternate") {
			for (auto & e : u.uis["ui_at3000"].elements) {
				if (e->tag == "on") {
					e->set(beat > .5);
				}
			}
		}
	}
	
	
	
	for (auto & c : fixturesWithUI) {
		ofxDmtrUI3 * uiC = &u.uis["uiscene_" + c];
		string & scene = u.uis["ui_" + c].pString["scene_" + c];

		
		
		if (scene == "contagem") {
			auto fixtures = mOfxx().getFixturesWithDefinitionName(c);
			auto contagem = fmod(ofGetElapsedTimef() * 10 , fixtures.size()) + uiC->pInt["c_step"];
			
            
            uiC->pFloat["contagem"] += uiC->pFloat["vel"] + audio * uiC->pFloat["velAudio"] + beat * uiC->pFloat["velBeat"] ;
            
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
				int v = ofClamp(ofMap(audio * uiC->pFloat["audio"] + beat * uiC->pFloat["beat"], 0, 1, uiC->pFloat["min"], uiC->pFloat["max"]), 0, 255);
				u.uis["ui_" + c].set(p, v);
			}
		}
		
		else if (scene == "colorPeak") {
			string p = uiC->pString["param"];
			
			if (audio > uiC->pFloat["threshold"]) {
				u.uis["ui_" + c].set("red",		uiC->pColor["corPeak"].r);
				u.uis["ui_" + c].set("green",	uiC->pColor["corPeak"].g);
				u.uis["ui_" + c].set("blue",	uiC->pColor["corPeak"].b);
				u.uis["ui_" + c].set("beamred",		uiC->pColor["corPeak"].r);
				u.uis["ui_" + c].set("beamgreen",	uiC->pColor["corPeak"].g);
				u.uis["ui_" + c].set("beamblue",	uiC->pColor["corPeak"].b);
				u.uis["ui_" + c].set("aurared",		uiC->pColor["corPeak"].r);
				u.uis["ui_" + c].set("auragreen",	uiC->pColor["corPeak"].g);
				u.uis["ui_" + c].set("aurablue",	uiC->pColor["corPeak"].b);
			}
			else {
				u.uis["ui_" + c].set("red",		uiC->pColor["cor"].r);
				u.uis["ui_" + c].set("green",	uiC->pColor["cor"].g);
				u.uis["ui_" + c].set("blue",	uiC->pColor["cor"].b);
				u.uis["ui_" + c].set("beamred",		uiC->pColor["cor"].r);
				u.uis["ui_" + c].set("beamgreen",	uiC->pColor["cor"].g);
				u.uis["ui_" + c].set("beamblue",	uiC->pColor["cor"].b);
				u.uis["ui_" + c].set("aurared",		uiC->pColor["cor"].r);
				u.uis["ui_" + c].set("auragreen",	uiC->pColor["cor"].g);
				u.uis["ui_" + c].set("aurablue",	uiC->pColor["cor"].b);
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
				//if (f->get)
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
			float fatorTempo = uiC->pFloat["fatorTempo"];
			float fatorIndex = uiC->pFloat["fatorIndex"];
//			float min = uiC->pFloat["min"];
//			float max = uiC->pFloat["max"];
			
			float valMiddle		= uiC->pFloat["val"];
			float amplitude = uiC->pFloat["amplitude"];
			
			{
				string p = uiC->pString["param"];
				
				//cout << fatorIndex << endl;
				if (p != "") {
					
					for (auto & f : mOfxx().getFixturesWithDefinitionName(c)) {
						//cout << fatorIndex * (float) f->getModelId() << endl;
						float val = ofNoise(fatorIndex * (float) f->getModelId(), ofGetElapsedTimef() * fatorTempo);
						float min = valMiddle - amplitude;
						float max = valMiddle + amplitude;
						float v = ofMap(val, 0, 1, min, max);
						v = ofClamp(v, 0, 255);
						f->setChannelByName(p, int(v));
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
	if (e.name == "Beat!") {
		isBeat = true;
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
	
	
	}
    
    
    
	else if (e.tag == "on") {
		string eventPrefix = ofSplitString( e.name, "_")[0];
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
