#ifndef utils_h
#define utils_h

#include <string>
#include "xxhash.h"


// Example for a function which hashes a null terminated string with XXH32().
XXH64_hash_t hash_string(const char* _string, XXH64_hash_t seed)
{
    // NULL pointers are only valid if the length is zero
    size_t length = (_string == NULL) ? 0 : strlen(_string);
    return XXH64((void*)_string, length, seed);
}



#endif /* utils_h */

