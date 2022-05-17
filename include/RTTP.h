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
    
    std::string journeyId;
    std::string trainId;

    int occupationStart;
    std::string routeId;
    std::string tDSectionID;
    int trainSequenceID;
    
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

//typedef std::pair<tdSecKey, tDSectionOccupation> RTTP_member;
//static bool cmp(const RTTP_member& l,const RTTP_member& r){
//    return l.second.trainId < r.cnt.second.trainId;
//}

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
RTTP perturbate_RTTP(RTTP* in,std::vector<std::string> train_Ids);


#endif /* RTTP_h */

//2do
// implement here a method to check if 2 rttp are the same

// NOT IMPLEMENTED - check the integrity of an RTTP
// void checkRTTP();

//2do
// implement here a method to dump a rttp in a XML file
