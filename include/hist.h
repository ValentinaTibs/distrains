//
//  hist.h
//  distrains
//

#ifndef hist_h
#define hist_h

#include "RTTP.h"

typedef XXH64_hash_t vs_name;

class HIST{
public:
    // here admitted operation are ADD - DEL - UPDATE
    vs_name last_merge, current_merge;
    std::vector<vs_name> full_hist;
    
    RTTP get_last_merge();
    RTTP get_current_merge();
    
    void update_hist();
    
    HIST();
};

#endif /* hist_h */
