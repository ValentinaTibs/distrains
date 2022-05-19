//
//  hist.cpp
//  distrains
//

#include "hist.h"
#include "utils.h"

HIST::HIST(){};

//2do this is here due to demo issues
RTTP HIST::get_last_merge(){
//    FILE *fp;
//
//    if((fp=fopen("../../data/base/ZIO.xml", "w+"))==NULL) {
//        printf("Error in opening file");
//        return;
//    }
    return RTTP("../../data/base/RTTP.pert_5.xml");
};

RTTP HIST::get_current_merge(){
    return RTTP("../../data/base/RTTP.3branch_current_merge.xml");
};

#include <ctime>

void HIST::update_hist(){
    
    // get current time as a string
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);
    
    vs_name vs_name = hash_string(buffer,app_seed);
    full_hist.push_back(vs_name);
    
    // maybe here we can update the current version if needed
};



