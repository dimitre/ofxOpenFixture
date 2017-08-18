//
//  Fixture.h
//  OpenFixture
//
//  Created by Henrique on 8/7/17.
//
//

#ifndef Fixture_h
#define Fixture_h

#include <vector>
#include <map>
#include <bitset>
#include <array>

#include <iostream>


namespace openfixture {
    
    class Fixture;
    
    class Definition{
    public:
        
        Definition(const std::string& name_ ) : name( name_ ){
            
        
        }
        
        // STATIC functions ------
        static Definition* createDefinition( const std::string& name_  ){
            
            std::unique_ptr< Definition > def ( new Definition( name_ ) );
            globalDefinitions[name_] = std::move( def );
            
            return &(*globalDefinitions[name_]);
        }
        
        
        static Definition* getDefinitionByName(const std::string& name_ ){
            
            return &(*globalDefinitions[name_]);
        }
        
        // name, def map
        static std::map< std::string,  std::unique_ptr<Definition>> globalDefinitions;
        
        
        // Member ------
        uint16_t getChannelByName(Fixture* fixture,  const std::string& name);
        uint16_t getMaxChannels(Fixture* fixture);
        
        
        std::vector< std::map<std::string, int>> modes;
        std::vector< Fixture* > fixtures;
        std::string name;
    };
    

    typedef std::shared_ptr<Fixture> FixtureRef;
    class Fixture{
        
    public:
        
        Fixture(){

        }
        
        Fixture( Definition* def ) : mDefPtr(def){
            mChannels.resize( mDefPtr->getMaxChannels(this) );
            def->fixtures.push_back( this );
        }
        
        static FixtureRef create( Definition* def ){
            return std::make_shared<Fixture>(def);
        }
        
        
        void setChannelByName(const std::string& name, uint8_t value );
        int getChannelValueByName(const std::string& name);
        
        void setMode(int m ){
            
            if( m >= mDefPtr->modes.size() ){
                std::cout << "error, no mode " << m << "in definition " << mDefPtr->name << std::endl;
            }
            
            mode = m;
            mChannels.resize(mDefPtr->getMaxChannels(this));
        }
    
        uint16_t getMaxChannels(){
            return mChannels.size() - 1;
        }
        
        
        void setChannel(int channel, uint8_t value ){
             mChannels[channel] = value;
        }
        
        
        void setBuffer( uint8_t* data ){
            
            for( int i = 0; i < mChannels.size(); i++ ){
                
                *data = mChannels[i];
                data++;
            }
            
        }
        
        //
        uint8_t getChannel(int channel){
            return mChannels[channel];
        }
        
        const int operator[](int i) const {
            return mChannels[i];
        }
        uint8_t& operator[](int i) {
            return mChannels[i];
        }

        friend class Definition;

        std::map<std::string, std::string> customProp;
        
    protected:
        
        int mode = 0;
        
        std::vector<uint8_t> mChannels;
        Definition* mDefPtr;
        
    };
    
    
    
    typedef std::shared_ptr< class Universe> UniverseRef;
    class Universe{
        
    public:
        Universe(){
            
        }
        
        static UniverseRef create(){

            return std::make_shared<Universe>();
            
        }
        
        
        void setFixture(int atChannel, FixtureRef fix ){
            
            // TODO: check availability
            fixturesMap[ atChannel ] = fix;
            
            if(  lastAvailableChannel < atChannel ){
                lastAvailableChannel = atChannel + fix->getMaxChannels();
            }
        }
        
        void appendFixture( FixtureRef fix ){
            
            fixturesMap[ lastAvailableChannel ] = fix;
            lastAvailableChannel += fix->getMaxChannels();

        }
        
        
        std::array<uint8_t, 512> getBuffer(){
    
            std::array<uint8_t, 512> data;
            memset(&data[0], 0, 512);
            
            for( auto& fixAdd : fixturesMap ) {
                fixAdd.second->setBuffer( &data[ fixAdd.first ] );
            }
            return data;
            
        }
        
        // checks if it's ok to add a fixture to the end of the buffer
        bool checkMaxChannels(int atIndex, FixtureRef fix){
            
            if( atIndex + fix->getMaxChannels() > 512 ){
                return false;
            }else{
                return true;
            }
        }
        
        std::vector<FixtureRef> getFixtures(){
            
            
            std::vector<FixtureRef> fixtures;
            
            for(auto fix : fixturesMap){
                fixtures.push_back( fix.second );
            }
            
            
            return fixtures;
        }
        
        
        
    private:
        std::array<uint8_t, 512> mDmxData;
        std::map<int, FixtureRef> fixturesMap;
        
        int lastAvailableChannel = 0;
    };


} // eof namespace

namespace ofix = openfixture;

#endif /* Fixture_h */
