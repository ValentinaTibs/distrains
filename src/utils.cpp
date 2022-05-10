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


