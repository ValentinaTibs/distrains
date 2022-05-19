//
//  diff.cpp
//  distrains
//
//  Created by vale on 16/05/22.
//

#include "diff.h"

void RTTP_diff::add_op(op_type _op,tDSectionOccupation _new_val){
    ops.push_back(RTTP_diff_op(_op,_new_val));
};


void RTTP_diff::add_op(op_type _op,tDSectionOccupation _old_val, tDSectionOccupation _new_val){
    ops.push_back(RTTP_diff_op(_op,_old_val,_new_val));
};

RTTP_diff_op* RTTP_diff::find(RTTP_diff_op _in){
    for(auto it = this->all_ops().begin(); it != this->all_ops().end(); it++){
        if(*it == _in)
            return  &*it;
    }
    return nullptr;
}

RTTP_diff diff(RTTP* L, RTTP* R){

    RTTP_diff results;

    for(auto it = L->tDSectionOccupations.begin(); it != L->tDSectionOccupations.end();it++){
        auto found = R->find_tDSectionOccupation(*it);
        if (found != nullptr) {
            if(found->cnt.second==it->cnt.second){
                printf(" FOUND %s ", it->cnt.first.c_str());
            }
            else{
                printf(" UPDATED %s ", it->cnt.first.c_str());
                results.add_op(op_type::UPDATE,found->cnt.second,it->cnt.second);
            }
        } else {
            printf(" DELETED %s ", it->cnt.first.c_str());
            results.add_op(op_type::DEL,it->cnt.second);
        }
    }
    
    printf("\n");
    
    for(auto it = R->tDSectionOccupations.begin(); it != R->tDSectionOccupations.end();it++){
        auto found = L->find_tDSectionOccupation(*it);
        if (found != nullptr) {
            printf(" FOUND %s ", it->cnt.first.c_str());
        } else {
            printf(" ADDED %s ", it->cnt.first.c_str());
            results.add_op(op_type::ADD,it->cnt.second);
        }
    }
    return results;

};


