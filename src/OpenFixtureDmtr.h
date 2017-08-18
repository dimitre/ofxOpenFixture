//
//  OpenFixtureDmtr.h
//  OpenFixture
//
//  Created by Henrique on 8/17/17.
//
//

#ifndef OpenFixtureDmtr_h
#define OpenFixtureDmtr_h

#include "Fixture.h"

#include "Utils.h"

#include <fstream>


namespace openfixture{

    
    
    std::vector <std::vector <std::string>> loadSchemeFromString( std::string s ){
        
        std::vector <std::vector<std::string>> saida;
        std::istringstream iss(s);
        std::string line;
        
        std::string currentName = "";
        int currentIndex = 0;
        
        while( std::getline( iss, line ) )
        {
            auto s = filterString(line);
            
            
            if( s[0] == '[' ){
                // start a new fixture scheme
                
                if( currentName != ""){
                    currentIndex += 1;
                }
                
                currentName = s;
                currentName.pop_back();
                currentName.erase(0,1);
                
                saida.push_back( std::vector<std::string>() );
                saida[currentIndex].insert(saida[currentIndex].begin(), currentName);
            }
            
            else if(s[0] == '#' || s == ""){
                // ignore comments & empty lines
                continue;
            }
            else{
                // add properties
                
                if(currentName == "" ){
                    std::cout << "error setting value without a fixture name" << std::endl;
                }else{
                    
                    // TODO: split in ','
                    saida[currentIndex].push_back(s);
                }
            }
            
        }
         
         
        return saida;
        
        
    }
    
    std::vector <std::vector <std::string>> loadScheme(std::string file_path) {
        std::vector <std::vector< std::string>> saida;
        
        std::ifstream file( file_path );
        
        std::string line;
        
        std::string currentName = "";
        int currentIndex = 0;
        
        while( std::getline( file, line ) )
        {
            auto s = filterString(line);
            
            
            if( s[0] == '[' ){
                // start a new fixture scheme
                
                if( currentName != ""){
                    currentIndex += 1;
                }
                
                currentName = s;
                currentName.pop_back();
                currentName.erase(0,1);
                
                saida.push_back( std::vector<std::string>() );
                saida[currentIndex].insert(saida[currentIndex].begin(), currentName);
            }
            
            else if(s[0] == '#' || s == ""){
                // ignore comments & empty lines
                continue;
            }
            else{
                // add properties
                
                if(currentName == "" ){
                    std::cout << "error setting value without a fixture name" << std::endl;
                }else{
                    
                    // TODO: split in ','
                    saida[currentIndex].push_back(s);
                }
            }
            
        }
        
        return saida;
    }


    ofix::Definition* createDefinitionFromScheme(const std::vector< std::vector<std::string>>& s ){
        
        ofix::Definition* def = nullptr;
        for(auto r :  s){
            
            def = ofix::Definition::createDefinition(r[0]);
            
            r.erase(r.begin()); // remove title
            std::map<std::string, int> chanelDef;
            
            for( auto rr : r ){
                auto nameValue = split( rr, '=' );
                chanelDef[ nameValue[1] ] = stoi( nameValue[0] );
            }
            
            def->modes.push_back( chanelDef );
        }
        
        return def;
    }

    
    ofix::Universe createUniverseFromScheme( const std::vector< std::vector<std::string>>& scheme ){
        
        
        Universe uni;
        
        for(auto fix : scheme){
            
            std::string name = fix[0];
            
            fix.erase(fix.begin());
            
            auto defGlobal = ofix::Definition::getDefinitionByName( name );
            
            if( defGlobal == nullptr){
                
                std::cout << "error no definition name: " << name << std::endl;
                continue;
            }
            
            auto mFix =  ofix::Fixture::create( defGlobal );
            mFix->setMode(0);
            
            
            int channel = -1;
            
            for( auto ff : fix){
                
                auto nameValue = ofix::split( ff, '=' );
                
                std::string name = nameValue[0];
                
                if( nameValue[0] == "channel" ){
                    int value = stoi( nameValue[1] );
                    channel = value;
                }else if( nameValue[0] == "mode" ){
                    
                    int value = stoi( nameValue[1] );
                    mFix->setMode(value);

                }else{
                    mFix->customProp[nameValue[0]] = nameValue[1];
                }

            }
            
            
            if( channel != -1 ){
                uni.setFixture(channel, mFix);
            }else{
                uni.appendFixture(mFix);
            }
        }
      
        return uni;
    }
    
}// eof namespace

#endif /* OpenFixtureDmtr_h */
