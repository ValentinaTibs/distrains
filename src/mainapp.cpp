#include <stdio.h>
#include <train.h>
#include <chrono>  // for high_resolution_clock

#include "tinyxml2.h"
#include "utils.h"
#include "RTTP.h"
#include "protocol.h"

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

    // this conf should be put in a json: UserName - ancesto - history
    
    // ********** load the ancestor  **********
    std::string fileRTT_anc = "../../data/base/RTTP.3branch.xml";
    RTTP RTT_anc = RTTP(fileRTT_anc);

    std::string fileHIST_anc = "../../data/base/HIST.3branch.xml";

    // ********** obtain a new version **********
    //RTTP RTT_vers = perturbate_RTTP();
    
    //RTTP_diff diff = diff(&RTT_anc, &RTT_vers);
    
    //create_version();
    
//    RTT_anc.printAll();
//
//    std::string fileRTT_A = "../../data/base/RTTP.3branch_A.xml";//,  fileRTT_A, fileRTT_B;
//    RTTP RTT_A = RTTP(fileRTT_A);
//
//    merge(&RTT_anc, &RTT_A);
    RTT_anc.printAll();

    RTT_anc.dump("../../data/base/RTTP.result.xml");


	printf("\nexecution Done\n");
	return 0;
}
