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


// LINUX npisanti
#include <memory>
#include <algorithm>
#include <cstring>


namespace openfixture {
    
    class Fixture;
    typedef std::shared_ptr<Fixture> FixtureRef;
    
    class Definition{
    public:
        
        Definition(const std::string& name_ ) : name( name_ ){
            
        
        }
        
        // STATIC functions ------
        static Definition* createDefinition( const std::string& name_  ){
            std::unique_ptr< Definition > def ( new Definition( name_ ) );
			//std::cout << "name ::" << name_ << std::endl;
            globalDefinitions[name_] = std::move( def );
            
            return &(*globalDefinitions[name_]);
        }
        
        static Definition* getDefinitionByName(const std::string& name_ ){
            
            if( globalDefinitions.find(name_) != globalDefinitions.end() ){
                return &(*globalDefinitions[name_]);
            }else{
                return nullptr;
            }
            
        }
        
        static std::vector< Definition* > getDefinitions(){
            
            std::vector< Definition* > result;
            
            for( auto& def :  globalDefinitions ){
                
                result.push_back( &(*def.second) );
            }
            return result;
        }
        
        
        //======
        void addMode(const std::map<std::string, int>& mode){
            modes.push_back( mode );
        }
        
        void setDefaultMode(int mode){
            defaultMode = mode;
        }
        
        int getDefaultMode(){
            return defaultMode;
        }
        
        void setDefaultValue(const std::vector<int> & value){
            defaultValues.push_back( value );
        }
        
        std::vector< Fixture* > getFixtures() const {
            return mFixtures;
        };
        
        
        std::vector< std::string > getChannelNames(int mode) const {
            
            auto maskedNames = names[mode];
            
            if( mNullChannelsMask.size() != 0 )
            {

                for( auto& nullChannel : mNullChannelsMask ){
//                    std::cout << "null channel: " <<  nullChannel << std::endl;
                    
                    if( nullChannel < maskedNames.size() ){
                        maskedNames[nullChannel] = "null";
                    }
                    
                }
            }
            
            return maskedNames;
        }
        
        int getChannelIndexByName(const std::string& name, int mode = 0 ){
            
            if(  modes[mode].find(name) != modes[mode].end() ){
                return modes[mode][name];
            }else{
                return -1;
            }
        }
        
        std::string getChannelNameByIndex( int index, int mode ){

            for( auto c : modes[mode] ){
                
                if( c.second == index ){
                    return c.first;
                }
            }
            
            return "";
        }
        
        void setChannelNames(const std::vector<std::string> & value){
            names.push_back( value );
        }
        
        void setCustomPropreties(const std::map<std::string, std::vector<std::string>> & value){
            customPropreties = value;
        }
        
        std::map<std::string, std::vector<std::string>> getCustomPropreties() const {
            return customPropreties;
        }
        
        std::vector<std::string> getPropretieValues(const std::string& propName ){
            return customPropreties[propName];
        }
        
        bool hasCustomProprety( const std::string& propName  ){
            
            if( customPropreties.find( propName ) != customPropreties.end() ){
                return true;
            }else{
                return false;
            }
            
        }
        
        void setBlackoutMask(const std::vector<int>& mask ){
            
            mBlackoutMask = mask;
        }
        
        void setNullChannelsMask(const std::vector<int>& nullChannels ){
            
            mNullChannelsMask = nullChannels;
            
        }
        
        
        std::vector<int> getBlackoutMask(){
            return mBlackoutMask;
        }
        
        std::string getModelName(){
            return name;
        }
        
        void setModelName(std::string name_){
            name = name_;
        }
        
        void setIgnoreMasterFade( bool v ){
            ignoreMasterFade = v;
        }
        bool getIgnoreMasterFade(){
            return ignoreMasterFade;
        }
        
        friend Fixture;
    private:
        
        static std::map< std::string,  std::unique_ptr<Definition>> globalDefinitions;
        
        // Member ------
        uint16_t getChannelByName(Fixture* fixture,  const std::string& name);
        uint16_t getMaxChannels(Fixture* fixture);
        
        
        // Channels definitions and default values
        
        int defaultMode = 0;
        std::vector< std::map<std::string, int>> modes;
        std::vector< std::vector< std::string > > names;
        
        std::vector< std::vector<int> > defaultValues;
        
        // todo: make this a class or method
        std::map<std::string, std::vector<std::string>> customPropreties;
        
        std::vector<int> mBlackoutMask;
        std::vector<int> mNullChannelsMask;

        
        std::vector< Fixture* > mFixtures;
        std::string name;
        
        bool ignoreMasterFade = false;

    };
    

    
    class Fixture{
        
    public:
        
        Fixture(){

        }
        
        Fixture( Definition* def ) : mDefPtr(def){
            
            fixId = fixtureCount;
            fixtureCount +=1 ;
            
            mChannels.resize( mDefPtr->getMaxChannels(this) );
            def->mFixtures.push_back( this );
            
            modelId = def->mFixtures.size() - 1;
                    
            setMode( def->defaultMode );
            
        
        }
        
        static FixtureRef create( Definition* def ){
            return std::make_shared<Fixture>(def);
        }
        
        
        void setChannelByName(const std::string& name, uint8_t value );
        int getChannelValueByName(const std::string& name);
        
        std::vector< std::string > getChannelNames() {
            return  mDefPtr->getChannelNames(mode);
        }
        
        void setMode(int m , bool resetToDefault = true){
            
            if( m >= mDefPtr->modes.size() ){
                std::cout << "error, no mode " << m << "in definition " << mDefPtr->name << std::endl;
            }
            
            mode = m;
            mChannels.resize(mDefPtr->getMaxChannels(this));
			mChannelsEasy.clear();
            mChannelsEasy.resize(mDefPtr->getMaxChannels(this));
            
            if( resetToDefault == true ){
               
                setBlackoutMask( mDefPtr->getBlackoutMask() );

                
                if( mDefPtr->defaultValues.size() > 0 ){
                    for(int i = 0; i < mChannels.size(); i++ ){
                        auto defaultv = mDefPtr->defaultValues[mode][i];
                        
                        setChannel(i, defaultv);
						
						mChannelsEasy[i] = defaultv;
                    }
                }
            }
        }
        
        int getMode(){
            return mode;
        }
    
        uint16_t getMaxChannels(){
            return mChannels.size() - 1;
        }
        
        
        void setChannel(int channel, uint8_t value ){
            
            
            if( panInvert ){
                auto panChannel = mDefPtr->getChannelByName(this, "pan");
                if( panChannel == channel ){
                    value = 255 - value;
                }
            }
            
            if(tiltInvert){
                
                auto tiltChannel = mDefPtr->getChannelByName(this, "tilt");
                if( tiltChannel == channel ){
                    value = 255 - value;
                }
            }
            
            
            
             mChannels[channel] = value;
        }
        
        
        void setExternalBuffer( uint8_t* dataOutput ){
            
            for( int i = 0; i < mChannels.size(); i++ ){
                
                *dataOutput = getChannel(i);
                dataOutput++;
            }
        }
        
        
        uint8_t getChannel(int channel) const{
            
//            if( mIsBlackout && mBlackoutMask.size() == 0 ){
//                return 0;
//            }
//            
//            
//            if( mIsBlackout ){
//                
//                if( std::find(mBlackoutMask.begin(), mBlackoutMask.end(), channel) != mBlackoutMask.end() ){
//                    
//                    return 0;
//                    
//                }else{
//					// TESTE
//                    //return mChannels[channel];
//					
//					return int(mChannelsEasy[channel]);
//                }
//            }
			
			return int(mChannelsEasy[channel]);

            //return mChannels[channel];
        }
        
    
        std::vector<uint8_t> getChannels(){
    
            return mChannels;
        }
        
        void setChannels(const std::vector<uint8_t>& channels_ ){
            
            if( mChannels.size() != channels_.size()  ){
                std::cout << "error, setting channel array with wrong size array" << std::endl;
            }else{
                mChannels = channels_;
            }
            
        }
        
        const int operator[](int i) const {
            return getChannel(i);
        }
        uint8_t& operator[](int i) {
            return mChannels[i];
        }
        
        
        void setBlackoutMask(const std::vector<int>& mask ){
            
            mBlackoutMask = mask;
        }
        
        std::vector<int> getBlackoutMask(){
            return mBlackoutMask;
        }
        
        void enableBlackout( bool b ){
            mIsBlackout = b;
        }
        
        bool isOnBlackout(){
            return mIsBlackout;
        }
        
        int getId(){
            return fixId;
        }
        
        int getModelId(){
            return modelId;
        }

        Definition* getDefinitionPtr(){
            return mDefPtr;
        }
		
		// XAXA DIMITRE BABEL EASING
//		float easyIn = 10.0;
//		float easyOut = 20.0;
		float easyIn = 0.0;
		float easyOut = 0.0;
		//bool useEasing = true;
		
		void updateEasing() {
			
			for (int a=0; a<mChannels.size(); a++) {
				
				float target = 0;

				
//				if (canal tem blackout) {
//					if (canal esta setado blackou)
//						-0
//						
//					else
//						*.fade
//					} else {
//							normal
//						}
				
				
				//check if we are on black out and we have a mask
				if( mIsBlackout && !mBlackoutMask.size() ){
					target = 0;
				}
                //check if channel is on blackout mask
                else if( mIsBlackout && std::find(mBlackoutMask.begin(), mBlackoutMask.end(), a) != mBlackoutMask.end() ){
					target = 0;
				}
                
                //check if we are using master fade and blackout mask
                else if (std::find(mBlackoutMask.begin(), mBlackoutMask.end(), a) != mBlackoutMask.end() && ! mDefPtr->ignoreMasterFade) {
					target = mChannels[a] * masterFade;

				} else if ( mBlackoutMask.size() == 0 && !mDefPtr->ignoreMasterFade) {
					target = mChannels[a] * masterFade;
                    
                }else{
                    
                    target = mChannels[a];
                }
				
				
				if ( target > mChannelsEasy[a]) {
					if (easyIn > 1 ) {
						mChannelsEasy[a] += (float(target) - mChannelsEasy[a]) / easyIn;
					} else {
						mChannelsEasy[a] = target;
					}
				} else {
					if (easyOut > 1 ) {
						mChannelsEasy[a] += (float(target) - mChannelsEasy[a]) / easyOut;
					} else {
						mChannelsEasy[a] = target;
					}
				}
				
			}
		}

        friend class Definition;

        // one key can have multiple values, eg: "group=stage,lowerLeft,back"
        std::map<std::string, std::vector<std::string> > customProp;
		
		static float masterFade;
        
        bool panInvert = false;
        bool tiltInvert = false;
    protected:
        

        int mode = 0;
        
        static int fixtureCount;
        int fixId = -1;
        
        int modelId = -1;
        
        std::vector<uint8_t> mChannels;
		
		// DIMITRE
		std::vector<float> mChannelsEasy;
        
        bool mIsBlackout = false;
        std::vector<int> mBlackoutMask;
        
        Definition* mDefPtr;
        
    };
	
	
    
    class Universe{
        
    public:
        Universe(){
            
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
            lastAvailableChannel += ( fix->getMaxChannels() + 1) ;

        }
        
        
        std::array<uint8_t, 512> getBuffer(){
    
            std::array<uint8_t, 512> data;
            memset(&data[0], 0, 512);
            
            for( auto& fixAdd : fixturesMap ) {
                fixAdd.second->setExternalBuffer( &data[ fixAdd.first ] );
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
        
        std::string name = "";
        std::string ipAddress = "0.0.0.0";
        int universeIndex; 
        
    private:
        std::array<uint8_t, 512> mmChannels;
        std::map<int, FixtureRef> fixturesMap;
        
        int lastAvailableChannel = 0;
    };


} // eof namespace

namespace ofix = openfixture;

#endif /* Fixture_h */
