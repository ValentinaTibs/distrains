//
//  protocol.h
//  distrains

#ifndef protocol_h
#define protocol_h

#include "RTTP.h"
#include "hist.h"
#include "diff.h"

enum errorCode{
    MERGE_SUCCESS,
    MISMATCH,
    OVERLAP
};

errorCode merge_diff(RTTP_diff* L,RTTP_diff* R,RTTP_diff* res);
errorCode apply_diff(RTTP* LEFT, RTTP* RIGHT, RTTP* output);
errorCode apply_diff(RTTP* L, RTTP_diff the_diff, RTTP* result);

errorCode local_individual_2_local_global(std::vector<RTTP> _in,RTTP* merged);
errorCode local_global_2_global(HIST* local_hist,RTTP* local_global,RTTP* global_merge);
void print_errorCode(errorCode _err);

#endif /* protocol_h */
