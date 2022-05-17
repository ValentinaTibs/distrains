//
//  RTTP.h
//  distrains
//
//  Created by vale on 28/01/22.
//

#include "protocol.h"
#include "RTTP.h"
#include "diff.h"

void create_version(void){
    
}

void load_version(void){
    
}

void load_history(void){
    
}

errorCode apply_diff(RTTP* L, RTTP* R, RTTP* result){
    errorCode retval=MERGE_SUCCESS;
    
    RTTP_diff the_diff = diff(L, R);
    auto all_ops = the_diff.all_ops();
    
    //2do here mettere una deep copy
    // result->tDSectionOccupations = L->tDSectionOccupations;

    for(auto it = all_ops.begin(); it != all_ops.end(); it++){
        if (it->op==op_type::UPDATE)
            retval = errorCode::MISMATCH;
        else if (it->op==op_type::ADD)
            result->addtDSectionOccupation(it->new_val) ;
        else
            result->addtDSectionOccupation(it->old_val) ;;
            
    }
    return retval;
}

errorCode local_individual_2_global(std::vector<RTTP> _in){
    errorCode retval = errorCode::MERGE_SUCCESS;
    RTTP* merged =&(_in[0]);
    for (auto it = _in.begin()+1; it != _in.end(); it++){
        retval = apply_diff(merged,&*it,merged);
    }
    return  retval;
};

void print_errorCode(errorCode _err){
    switch (_err) {
        case 0:
            printf("\n MERGE_SUCCESS\n");
            break;
        case 1:
            printf("\n MISMATCH\n");
            break;

        default:
            printf("\n unexpected value \n");
            break;
    }
    return;
};


int merge_protocol(RTTP* ANC,RTTP* PULL,RTTP* L){
    
    // make version
    // load history
    //  auto this_diff = diff(&RTT_anc, &RTT_anc_B);

};

