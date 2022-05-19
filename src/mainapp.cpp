#include <stdio.h>
#include <train.h>
#include <chrono>  // for high_resolution_clock

#include "tinyxml2.h"
#include "utils.h"
#include "RTTP.h"
#include "protocol.h"
#include "hist.h"

static int num_trains = 3;
XXH64_hash_t app_seed = 3;
std::string UserName = "AAAAAA";

void lifecycle(void){
	std::this_thread::sleep_for(std::chrono::milliseconds(3));
	printf("/n done sleeping /n");
};

void broadcast(int idx, train* trains){
    for(auto i = 0 ; i < num_trains; i++){
        if (i != idx)
            trains[idx].sendmsg(&trains[i]);
    }
    return;
}

int main (int argc, char *argv[]) {
    
    if(argc <= 6){
        printf("Wrong USAGE");
        return 0;
    }
    // this conf should be put in a json: UserName - ancesto - history
    for(auto i=0;i<argc;i++)
        printf("%s ",argv[i]);
    
    std::string base_path = argv[1];
    int train_num =  std::stoi(argv[2]);
    
    // parse the RTTP files
    std::vector<std::string> RTTP_files;
    for(auto i=3;i<3+train_num;i++){
        std::string fileRTTP = argv[i];
        RTTP_files.push_back( base_path +  fileRTTP + ".xml");
    }
    
    // in case we need need extract neighbrhood
    std::vector<std::vector<std::string> > trains_filters;
    
    for(auto i=3+train_num;i< 3 + train_num * 2;i++){
        trains_filters.push_back(split_string(argv[i],","));
    }
 
    RTTP print_all(RTTP_files[0]);
    print_all_trains(&print_all);
    // ********** obtain a new version from local train neighborhood **********
    std::vector<RTTP> all_local_individual;
    
    for(int i=0; i<num_trains-1; i++ ){
        RTTP local_full(RTTP_files[i]);
        RTTP local_individual = perturbate_RTTP(&local_full,trains_filters[i]);
        all_local_individual.push_back(local_individual);
    }
    
    //  Merge Local Individual
    RTTP local_global;
    errorCode _error;
    
    local_global = local_individual_2_local_global(all_local_individual,_error);
    print_errorCode(_error);
    local_global.printAll();
    
    auto res = perturbate_RTTP(&local_global, 5);
    res.dump("RTTP.pert_5.xml");
    
    //  Merge 2 Global
    RTTP global_merge;
    HIST* local_hist = new HIST();
    global_merge = local_global_2_global(local_hist,&local_global,_error);
    print_errorCode(_error);
    global_merge.printAll();
  
	printf("\nexecution Done\n");
	return 0;
}
