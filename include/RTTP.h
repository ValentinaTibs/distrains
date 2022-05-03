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

class tDSectionOccupation{
public:
    
    std::string journeyId;
    std::string trainId;

    int occupationStart;
    std::string routeId;
    std::string tDSectionID;
    int trainSequenceID;
    std::string trainID = "";
        
    tDSectionOccupation(int _occupationStart,std::string _routeId,std::string _tDSectionID,std::string _journeyId, std::string _trainId);
    void settrainSequenceID (int _trainSequenceID);
    std::string makeKey(void);
};

typedef XXH64_hash_t tdSecKey;
extern  XXH64_hash_t app_seed;


//class rTTPForSingleTrain{
//public:
//    std::map<tdSecKey, tDSectionOccupation> tDSectionOccupations;
//    std::string journeyId;
//    std::string trainId;
//
//    rTTPForSingleTrain(std::string _journeyId, std::string _trainId );
//    void addtDSectionOccupation(tDSectionOccupation _newtDSectionOccupation);
//};
//

class RTTP{
public:
    
    // the key is the sha btw train ID and occupation start
    //std::map<std::string, rTTPForSingleTrain> rTTPTrainView;
    std::map<tdSecKey, tDSectionOccupation> tDSectionOccupations;
    
    //void addrTTPForSingleTrain(std::string _trainId,rTTPForSingleTrain newRTTPforSingleTrain );
    void addtDSectionOccupation(tDSectionOccupation _newtDSectionOccupation);
    
    RTTP(std::string filename);
    
    void printAll(void);
    void dump(std::string);
    
    //RTTP perturbate_RTTP(void);
    //2do
    // implement here a method to check if 2 rttp are the same
    
    // NOT IMPLEMENTED - check the integrity of an RTTP
    // void checkRTTP();
    
    
    //2do
    // implement here a method to dump a rttp in a XML file
 
};

enum errorCode{
    MERGE_SUCCESS,
    MISMATCH,
    
};

class RTTP_diff{
    // here admitted operation are ADD - DEL - UPDATE
    std::string op;
    RTTP_diff();
};


int diff(RTTP* ANC, RTTP* input);
int merge(RTTP* ANC, RTTP* input);

#endif /* RTTP_h */
