#include <stdio.h>
#include <train.h>
#include <chrono>  // for high_resolution_clock

#include "tinyxml2.h"
#include "xxhash.h"
#include "RTTP.h"

static int num_trains = 3;
XXH64_hash_t app_seed = 3;

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

//	bool not_done = true;
//    std::vector<train> trains;
//
//    /* ---- INIT all trains   ---- */
//	for (int i = 0 ; i < num_trains; i++){
//        trains.push_back(train());
//	}
//	/* ---- end of INIT all trains   ---- */
//
//
//	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
//
//	for (uint8_t i = 0 ; i < num_trains; i++){
//		trains[i].check();
//	}
//
//    trains[0].sendmsg(&trains[1]);
//    auto result = trains[1].readmsg();
//
//    //trains 1 queue must be empty
//    result.print();
    
//	while(not_done){
//
//		auto start = std::chrono::high_resolution_clock::now();
//		auto finish = std::chrono::high_resolution_clock::now();
//		std::chrono::duration<double> elapsed = finish - start;
//
//		printf( "Elapsed time:, %f s\n", elapsed.count());
//
//		not_done = false;
//	}
    
    // this conf should be put in a json
    std::string fileRTT_anc = "../../data/RTTP.3branch.xml";//,  fileRTT_A, fileRTT_B;
    RTTP RTT_anc = RTTP(fileRTT_anc);
    RTT_anc.printAll();
    
    std::string fileRTT_A = "../../data/RTTP.3branch_A.xml";//,  fileRTT_A, fileRTT_B;
    RTTP RTT_A = RTTP(fileRTT_A);
    
    merge(&RTT_anc, &RTT_A);
    RTT_anc.printAll();
    
    RTT_anc.dump("../../data/RTTP.result.xml");
    

	printf("\nexecution Done\n");
	return 0;
}
