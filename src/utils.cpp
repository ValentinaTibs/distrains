//
//  utils.cpp
//  distrains
//
//  Created by vale on 09/02/22.
//

#include <stdio.h>
#include "utils.h"

XXH64_hash_t hash_string(const char* _string, XXH64_hash_t seed){
    // NULL pointers are only valid if the length is zero
    size_t length = (_string == NULL) ? 0 : strlen(_string);
    return XXH64((void*)_string, length, seed);
}

std::vector<std::string> split_string(std::string s,std::string delim){
    std::vector<std::string> words;

        auto start = 0U;
        auto end = s.find(delim);
        while (end != std::string::npos)
        {
            words.push_back(s.substr(start, end - start));
            start = end + delim.length();
            end = s.find(delim, start);
        }
    
        words.push_back(s.substr(start, end));
    return words;
}
