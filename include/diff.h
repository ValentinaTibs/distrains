//
//  diff.h
//  distrains
//
//  Created by vale on 16/05/22.
//

#ifndef diff_h
#define diff_h

#include "RTTP.h"

enum op_type{
    ADD,
    DEL,
    UPDATE
};

class RTTP_diff_op{
public:
    // here admitted operation are ADD - DEL - UPDATE
    op_type op;
    tDSectionOccupation old_val;
    tDSectionOccupation new_val;
    RTTP_diff_op(op_type _op,tDSectionOccupation _old_val,tDSectionOccupation _new_val){
        this->op=_op;
        this->new_val = _new_val;
        this->old_val = _old_val;
    };
    
    RTTP_diff_op(op_type _op,tDSectionOccupation _new_val){
        this->op=_op;
        this->new_val = _new_val;
    };
};

class RTTP_diff{
private:
    std::vector<RTTP_diff_op> ops;
public:
    void add_op(op_type _op,tDSectionOccupation _old_val,tDSectionOccupation _new_val);
    void add_op(op_type _op,tDSectionOccupation _new_val);
    std::vector<RTTP_diff_op> all_ops(){return this->ops;}
};

RTTP_diff diff(RTTP* ANC, RTTP* input);

#endif /* diff_h */
