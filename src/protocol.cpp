//
//  RTTP.h
//  distrains
//
//  Created by vale on 28/01/22.
//

#include "protocol.h"
#include "RTTP.h"
#include "diff.h"

errorCode merge_diff(RTTP_diff* L,RTTP_diff* R,RTTP_diff* res){
    errorCode retval=MERGE_SUCCESS;
    
    auto L_all_ops = L->all_ops();
    auto R_all_ops = R->all_ops();

    for(auto it = L->all_ops().begin(); it != L->all_ops().end(); it++){
        
        if(R->find(*it)){
            //2do this is where we would have to put the euristic
            continue;
        }
        else{
            res->add_op(it->op, it->new_val);
        }
    }
    
    return retval;
};

errorCode apply_diff(RTTP* L,RTTP_diff the_diff, RTTP* result){
    errorCode retval=MERGE_SUCCESS;
    
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
}

errorCode apply_diff(RTTP* L, RTTP* R, RTTP* result){
    errorCode retval=MERGE_SUCCESS;
    
    RTTP_diff the_diff = diff(L, R);

    return apply_diff(L,the_diff,result);
}

errorCode local_individual_2_local_global(std::vector<RTTP> _in,RTTP* merged){
    errorCode retval = errorCode::MERGE_SUCCESS;
    merged =&(_in[0]);
    for (auto it = _in.begin()+1; it != _in.end(); it++){
        retval = apply_diff(merged,&*it,merged);
    }
    return  retval;
};

errorCode local_global_2_global(HIST* local_hist,RTTP* local_global,RTTP* global_merge){
    
    errorCode _error = MERGE_SUCCESS;
    
    RTTP last_merge = local_hist->get_last_merge();
    RTTP current_merge = local_hist->get_current_merge();
    
    RTTP_diff anc_diff = diff(&last_merge, local_global);
    RTTP_diff pull_diff = diff(&current_merge, local_global);
    
    RTTP_diff merged_diff;
    _error = merge_diff(&anc_diff,&pull_diff,&merged_diff);
    
    //global_merge
    _error = apply_diff(local_global, merged_diff, global_merge);
    
    if(_error == MERGE_SUCCESS){
        
        local_hist->update_hist();
    }
    return _error;
}

void print_errorCode(errorCode _err){
    switch (_err) {
        case 0:
            printf("\n MERGE_SUCCESS\n");
            break;
        case 1:
            printf("\n MISMATCH\n");
            break;
        case 2:
            printf("\n OVERLAP\n");
            break;

        default:
            printf("\n unexpected value \n");
            break;
    }
    return;
};
