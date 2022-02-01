//
//  RTTP.h
//  distrains
//
//  Created by vale on 28/01/22.
//

#ifndef RTTP_h
#define RTTP_h

#include <map>


class tDSectionOccupation{
public:
    int occupationStart;
    std::string routeId;
    std::string tDSectionID;
    std::string trainID;
    int trainSequenceID;
    int trackSequenceID;
    
    
    tDSectionOccupation(int _occupationStart,std::string _routeId,std::string _tDSectionID){
        this->occupationStart = _occupationStart;
        this->routeId = _routeId;
        this->tDSectionID = _tDSectionID;
    }
    void settrainSequenceID (int _trainSequenceID){this->trainSequenceID = _trainSequenceID;}
    void settrackSequenceID (int _trackSequenceID,std::string _trainID){this->trackSequenceID = _trackSequenceID; this->trainID = _trainID;}
};

class rTTPForSingleTrain{
public:
    std::map<std::string, tDSectionOccupation> tDSectionOccupations;
    std::string journeyId;
    std::string trainId;
    
    rTTPForSingleTrain(std::string _journeyId, std::string _trainId ){
        this->journeyId = _journeyId;
        this->trainId = _trainId;
    }
    void addtDSectionOccupation(std::string tDSectionKey, tDSectionOccupation _newtDSectionOccupation){
        tDSectionOccupations.insert(std::pair<std::string, tDSectionOccupation >(std::string(tDSectionKey),_newtDSectionOccupation));
    }
};

class rTTPForSingleTDSection{
public:
    std::map<std::string, tDSectionOccupation> tDSectionOccupations;
    std::string tDSectionId;
    
    rTTPForSingleTDSection(std::string _tDSectionId){
        this->tDSectionId = _tDSectionId;
    }
    void addtDSectionOccupation(std::string tDSectionKey,tDSectionOccupation _newtDSectionOccupation){
        tDSectionOccupations.insert(std::pair<std::string, tDSectionOccupation >(std::string(tDSectionKey),_newtDSectionOccupation));
    }
};

class RTTP{
public:
    
    // the key is the train ID
    std::map<std::string, rTTPForSingleTrain> rTTPTrainView;
    // the key is the train ID trackSequenceID
    std::map<std::string, rTTPForSingleTDSection > rTTPInfrastructureView;
    
    
    RTTP(std::string filename){
       
        // 2do metterci un check per vedere se il file esiste
        tinyxml2::XMLDocument doc;
        doc.LoadFile( filename.c_str() );

        tinyxml2::XMLNode* ele = doc.FirstChildElement( "rTTP" )->FirstChildElement("rTTPTrainView")->FirstChildElement();
        
        for( ; ele; ele = ele->NextSibling() ){
            const char* trainId = "failed";
            const char* journeyId = "failed";
            
            tinyxml2::XMLError queryResult1 = ele->ToElement()->QueryStringAttribute("trainId", &trainId);
            tinyxml2::XMLError queryResult2 = ele->ToElement()->QueryStringAttribute("journeyId", &journeyId);
            
            rTTPForSingleTrain newRTTPforSingleTrain( (std::string(journeyId)), (std::string(trainId)) );
            
            tinyxml2::XMLNode* single_ele = ele->FirstChildElement();
            for( ; single_ele; single_ele = single_ele->NextSibling() ){
                int occupationStart;
                int trainSequenceID;
                const char* routeId = "failed";
                const char* tDSectionID = "failed";
                
                //2do put some safety check here on the queryResults values
                tinyxml2::XMLError queryResult1 = single_ele->ToElement()->QueryIntAttribute("occupationStart", &(occupationStart));
                tinyxml2::XMLError queryResult2 = single_ele->ToElement()->QueryIntAttribute("trainSequenceID", &(trainSequenceID));
                tinyxml2::XMLError queryResult3 = single_ele->ToElement()->QueryStringAttribute("routeId", &(routeId));
                tinyxml2::XMLError queryResult4 = single_ele->ToElement()->QueryStringAttribute("tDSectionID", &(tDSectionID));
                
                tDSectionOccupation newtDSOcc(occupationStart,std::string(routeId),std::string(tDSectionID));
                newtDSOcc.settrainSequenceID(trainSequenceID);
                // we assume here which is thhe key for a TDSectionOccupation from the train point of view
                newRTTPforSingleTrain.addtDSectionOccupation(std::string(tDSectionID), newtDSOcc);
            }
            rTTPTrainView.insert(std::pair<std::string, rTTPForSingleTrain>(std::string(trainId),newRTTPforSingleTrain));
        }
            
        ele = doc.FirstChildElement( "rTTP" )->FirstChildElement("rTTPInfrastructureView")->FirstChildElement();
        
        for( ; ele; ele = ele->NextSibling() ){
            const char* tDSectionId = "failed";
            
            tinyxml2::XMLError queryResult1 = ele->ToElement()->QueryStringAttribute("tDSectionId", &tDSectionId);
            rTTPForSingleTDSection newrTTPForSingleTDSection(tDSectionId);
            
            std::vector<tDSectionOccupation> tDSectionOccupations;
            
            
            tinyxml2::XMLNode* single_ele = ele->FirstChildElement();
            for( ; single_ele; single_ele = single_ele->NextSibling() ){
                int occupationStart;
                int trackSequenceID;
                const char* routeId = "failed";
                const char* tDSectionID = "failed";
                const char* trainID = "failed";
                
                
                //2do put some safety check here on the queryResults values
                tinyxml2::XMLError queryResult1 = single_ele->ToElement()->QueryIntAttribute("occupationStart", &(occupationStart));
                tinyxml2::XMLError queryResult2 = single_ele->ToElement()->QueryIntAttribute("trackSequenceID", &(trackSequenceID));
                tinyxml2::XMLError queryResult3 = single_ele->ToElement()->QueryStringAttribute("routeId", &(routeId));
                tinyxml2::XMLError queryResult4 = single_ele->ToElement()->QueryStringAttribute("tDSectionID", &(tDSectionID));
                tinyxml2::XMLError queryResult5 = single_ele->ToElement()->QueryStringAttribute("trainID", &(trainID));
                
                
                tDSectionOccupation newtDSOcc(occupationStart,std::string(routeId),std::string(tDSectionID));
                newtDSOcc.settrackSequenceID(trackSequenceID,std::string(trainID));
                newrTTPForSingleTDSection.addtDSectionOccupation(std::string(trainID), newtDSOcc);
            }
            
            rTTPInfrastructureView.insert(std::pair<std::string, rTTPForSingleTDSection >(std::string(tDSectionId),newrTTPForSingleTDSection));
        }
    }

    virtual ~RTTP(){
        return;
    }
    
    void printAll(void){
        for (auto it = rTTPTrainView.begin(); it != rTTPTrainView.end(); it++)
        {
            printf("\n\n TRAIN --> %s \n",it->first.c_str());
            for (auto fr = it->second.tDSectionOccupations.begin(); fr != it->second.tDSectionOccupations.end();fr++){
                printf("occupationStart=%d routeId=%s tDSectionID=%s trainSequenceID=%d \n",fr->second.occupationStart,fr->second.routeId.c_str(),fr->second.tDSectionID.c_str(),fr->second.trainSequenceID);
            }
        }
        for (auto it = rTTPInfrastructureView.begin(); it != rTTPInfrastructureView.end(); it++)
        {
            printf("\n TRACK --> %s \n",it->first.c_str());
            for (auto fr = it->second.tDSectionOccupations.begin();
                 fr != it->second.tDSectionOccupations.end();fr++){
                printf("occupationStart=%d routeId=%s tDSectionID=%s trackSequenceID=%d trainID=%s \n",fr->second.occupationStart,fr->second.routeId.c_str(),fr->second.tDSectionID.c_str(),fr->second.trackSequenceID,fr->second.trainID.c_str());
            }
        }
        
        printf("=================== \n");
    }
    
    //2do
    // implement here a method to check if 2 rttp are the same
    
    //2do
    // implement here a method to dump a rttp in a XML file

    
    enum errorCode{
        MERGE_SUCCESS,
    };
    
    int merge(RTTP* input){
        errorCode retval=MERGE_SUCCESS;
        
        
        // for all elements in the
        for(auto it = input->rTTPTrainView.begin(); it != input->rTTPTrainView.end(); it++){
            
            if( this->rTTPTrainView.find(it->first) == this->rTTPTrainView.end() )
            {
                this->rTTPTrainView.insert(std::pair<std::string, rTTPForSingleTrain>(std::string(it->first),it->second));
            }            
        }
        
        for(auto it = input->rTTPInfrastructureView.begin(); it != input->rTTPInfrastructureView.end(); it++){
            
            if( this->rTTPInfrastructureView.find(it->first) == this->rTTPInfrastructureView.end() )
            {
                // 2do - this INSERT is not what it should be done - but instead we have to think on a merge data structure - in this case data presents in the ANC but not in the INPUT might be overwritten - or maybe this is just needed in cases of key that is found but has a different content
                this->rTTPInfrastructureView.insert(std::pair<std::string,
                    rTTPForSingleTDSection>(std::string(it->first),it->second));
            }
        }
        return retval;
    }
};

#endif /* RTTP_h */
