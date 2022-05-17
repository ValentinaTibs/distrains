//
//  protocol.h
//  distrains

#ifndef protocol_h
#define protocol_h

#include "RTTP.h"

enum errorCode{
    MERGE_SUCCESS,
    MISMATCH,
};


void create_version(void);
void load_version(void);
void load_history(void);

errorCode apply_diff(RTTP* LEFT, RTTP* RIGHT, RTTP* output);
errorCode local_individual_2_global(std::vector<RTTP> _in);

void print_errorCode(errorCode _err);




#endif /* protocol_h */
