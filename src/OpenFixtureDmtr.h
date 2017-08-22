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

    // TODO---
    struct Scheme{
        std::string name = "";
        std::map< std::string, std::vector<std::string>>  values;
    };
    // ---
    
    inline std::vector <std::vector <std::string>> loadSchemeFromString( std::string s ){
        
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
    
   inline std::vector <std::vector <std::string>> loadScheme(std::string file_path) {
       
       
        std::vector <std::vector< std::string >> saida;
        
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

}// eof namespace

#endif /* OpenFixtureDmtr_h */
