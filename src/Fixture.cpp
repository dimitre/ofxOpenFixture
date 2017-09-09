//
//  Fixture.cpp
//  OpenFixture
//
//  Created by Henrique on 8/7/17.
//
//

#include "Fixture.h"
using namespace std;

namespace openfixture {
    
    // Definition --------
    uint16_t Definition::getChannelByName(openfixture::Fixture *fixture, const std::string &name){
        
        int mode = fixture->mode;
        
        if ( modes[mode].find(name) == modes[mode].end() ) {
            
            std::cout << "no channel with name: " << name  << " in fixture: " << fixture->getDefinitionPtr()->getModelName()   << std::endl;
            return -1;
        } else {
            return modes[mode][name];
        }
    }
    
    uint16_t Definition::getMaxChannels(openfixture::Fixture *fixture)
    {
        int mode = fixture->mode;
        return modes[mode].size();
    }
    
    std::map<std::string, std::unique_ptr<Definition>> Definition::globalDefinitions{};

 
    
    
    // Fixtures --------
    int Fixture::fixtureCount = 0;
	float Fixture::masterFade = 1.0f;

    void Fixture::setChannelByName(const std::string& name, uint8_t value){
        
        int channel = mDefPtr->getChannelByName(this, name);
        mChannels[channel] = value;
        
    }
    
    
    int Fixture::getChannelValueByName(const std::string& name){
        
        int channel = mDefPtr->getChannelByName(this, name);
        return mChannels[channel];
    }
    
}
