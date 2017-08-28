//
//  Utils.h
//  OpenFixture
//
//  Created by Henrique on 8/18/17.
//
//

#ifndef Utils_h
#define Utils_h

#include <vector>
#include <string>
#include <map>

#include <sstream>

#include <cstdlib>


namespace openfixture {
    
    
    inline bool isInteger(const std::string & s)
    {
        if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;
        
        char * p ;
        strtol(s.c_str(), &p, 10) ;
        
        return (*p == 0) ;
    }

   inline std::vector<std::string> split(std::string str, char delimiter) {
        std::vector<std::string> internal;
        std::stringstream ss(str); // Turn the string into a stream.
        std::string tok;
        
        while(getline(ss, tok, delimiter)) {
            internal.push_back(tok);
        }
        
        return internal;
    }
    
   inline std::string filterString(const std::string& input ){
        
        std::string result = input;
        
        //std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        result.erase(remove_if(result.begin(), result.end(), ::isspace ), result.end());
        result.erase(std::remove(result.begin(), result.end(), '\t'), result.end());
   
        return result;
    }

}


#endif /* Utils_h */
