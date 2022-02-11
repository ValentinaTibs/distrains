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
    int occupationStart;
    std::string routeId;
    std::string tDSectionID;
    std::string trainID;
    int trainSequenceID;
    int trackSequenceID;
    
    
    tDSectionOccupation(int _occupationStart,std::string _routeId,std::string _tDSectionID);
    void settrainSequenceID (int _trainSequenceID);
    void settrackSequenceID (int _trackSequenceID,std::string _trainID);
    std::string makeKey(void);
};

typedef XXH64_hash_t tdSecKey;
extern  XXH64_hash_t app_seed;

class rTTPForSingleTrain{
public:
    std::map<tdSecKey, tDSectionOccupation> tDSectionOccupations;
    std::string journeyId;
    std::string trainId;
    
    rTTPForSingleTrain(std::string _journeyId, std::string _trainId );
    void addtDSectionOccupation(tDSectionOccupation _newtDSectionOccupation);
};



class RTTP{
public:
    
    // the key is the train ID
    std::map<std::string, rTTPForSingleTrain> rTTPTrainView;
    
    void addrTTPForSingleTrain(std::string _trainId,rTTPForSingleTrain newRTTPforSingleTrain );
    void checkRTTP();
    
    RTTP(std::string filename);
    
    void printAll(void);
    //2do
    // implement here a method to check if 2 rttp are the same
    
    //2do
    // implement here a method to dump a rttp in a XML file

    
};

enum errorCode{
    MERGE_SUCCESS,
    MISMATCH,
    
};

int merge(RTTP* ANC, RTTP* input);

#endif /* RTTP_h */
