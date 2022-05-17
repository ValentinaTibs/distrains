#ifndef utils_h
#define utils_h

#include <string>
#define XXH_INLINE_ALL

#include "xxhash.h"
#include "RTTP.h"

// Example for a function which hashes a null terminated string with XXH32().
XXH64_hash_t hash_string(const char* _string, XXH64_hash_t seed);
//std::vector<std::string> split(const std::string& s)
//{
//    std::stringstream ss(s);
//    std::vector<std::string> words;
//    for (std::string w; ss>>w; ) words.push_back(w);
//    return words;
//}

std::vector<std::string> split_string(std::string s,std::string delimiter);

#endif /* utils_h */

