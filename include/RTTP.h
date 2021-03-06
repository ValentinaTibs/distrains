//
//  RTTP.h
//  distrains
//
//  Created by vale on 28/01/22.
//

#ifndef RTTP_h
#define RTTP_h

#include <map>
#include <utils.h>
#include <vector>


//typedef XXH64_hash_t tdSecKey;
typedef std::string tdSecKey;
extern  XXH64_hash_t app_seed;


class tDSectionOccupation{
public:

    int occupationStart;
    int trainSequenceID;

    std::string journeyId;
    std::string trainId;
    std::string routeId;
    std::string tDSectionID;
    
    tDSectionOccupation(){ exit; };
    tDSectionOccupation(int _occupationStart,std::string _routeId,std::string _tDSectionID,std::string _journeyId, std::string _trainId);
    
    void settrainSequenceID (int _trainSequenceID);
    tdSecKey makeKey(void);
    
    bool operator==(const tDSectionOccupation& r){
        if(journeyId==r.journeyId && occupationStart==r.occupationStart)
            return true;
        return false;
    };
    
};

class RTTP_member{
public:
    std::pair<tdSecKey, tDSectionOccupation> cnt;

    bool cmp(const RTTP_member& l,const RTTP_member& r){
        return l.cnt.second.trainId < r.cnt.second.trainId;
    };
    
    bool operator==(const RTTP_member& r){
        return this->cnt.first == r.cnt.first;
    };
    
    RTTP_member(tdSecKey _tdSecKey, tDSectionOccupation _tDSectionOccupation ){
        this->cnt = std::pair<tdSecKey,tDSectionOccupation>(_tdSecKey,_tDSectionOccupation);
    };
    
    RTTP_member(){
        exit;
    };
};

class RTTP{
public:
    //*--- MEMBERS  ---*//
    //std::map<tdSecKey, tDSectionOccupation> tDSectionOccupations;
    std::vector< RTTP_member > tDSectionOccupations;

    //*--- CONSTRUCTOR  ---*//
    RTTP(std::string filename);
    RTTP(){};
    
    void addtDSectionOccupation(tDSectionOccupation _newtDSectionOccupation);
    RTTP_member* find_tDSectionOccupation(RTTP_member _tDSO);

//    //*--- SERVICE  ---*//
    void printAll(void);
    void dump(std::string);

};

RTTP perturbate_RTTP(void);
RTTP perturbate_RTTP(RTTP* _in,std::vector<std::string> train_Ids);
RTTP perturbate_RTTP(RTTP* _in,int trashold);
void print_all_trains(RTTP* _in);

#endif /* RTTP_h */

//2do
// implement here a method to check if 2 rttp are the same

// NOT IMPLEMENTED - check the integrity of an RTTP
// void checkRTTP();

//2do
// implement here a method to dump a rttp in a XML file
