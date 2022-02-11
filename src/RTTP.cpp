//
//  RTTP.h
//  distrains
//
//  Created by vale on 28/01/22.
//

#include "RTTP.h"
#include "tinyxml2.h"

tDSectionOccupation::tDSectionOccupation(int _occupationStart,std::string _routeId,std::string _tDSectionID){
    this->occupationStart = _occupationStart;
    this->routeId = _routeId;
    this->tDSectionID = _tDSectionID;
}
void tDSectionOccupation::settrainSequenceID (int _trainSequenceID){this->trainSequenceID = _trainSequenceID;}
void tDSectionOccupation::settrackSequenceID (int _trackSequenceID,std::string _trainID){this->trackSequenceID = _trackSequenceID; this->trainID = _trainID;}

std::string tDSectionOccupation::makeKey(void){
    return std::to_string(this->occupationStart).append(this->trainID);
}

rTTPForSingleTrain::rTTPForSingleTrain(std::string _journeyId, std::string _trainId ){
    this->journeyId = _journeyId;
    this->trainId = _trainId;
}
void rTTPForSingleTrain::addtDSectionOccupation(tDSectionOccupation _newtDSectionOccupation){
    std::string tDSectionKey = _newtDSectionOccupation.makeKey();
    tDSectionOccupations.insert(std::pair<tdSecKey, tDSectionOccupation >(hash_string(tDSectionKey.c_str(),app_seed),_newtDSectionOccupation));
}



void RTTP::addrTTPForSingleTrain(std::string _trainId,rTTPForSingleTrain newRTTPforSingleTrain ){
    this->rTTPTrainView.insert(std::pair<std::string, rTTPForSingleTrain>(_trainId,newRTTPforSingleTrain));
}
        
RTTP::RTTP(std::string filename){
   
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
            int trainSequenceID = -1;
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
            newRTTPforSingleTrain.addtDSectionOccupation(newtDSOcc);
        }
        this->addrTTPForSingleTrain(std::string(trainId),newRTTPforSingleTrain);
    }
        
    
}
void RTTP::printAll(void){
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


int merge(RTTP* ANC, RTTP* input){
    errorCode retval=MERGE_SUCCESS;
    
    // for all elements in the TrainView
    for(auto it = input->rTTPTrainView.begin(); it != input->rTTPTrainView.end(); it++){
        
        // THE ELEMENT IS NOT PRESENT IN THE ANCESTOR - search by key and add it
        if( ANC->rTTPTrainView.find(it->first) == ANC->rTTPTrainView.end() )
        {
            // can be not presence because some of the element that constitute the key have changed
            // so we must first check if something between the two keys hasn't changed
            
            ANC->addrTTPForSingleTrain(it->first,it->second);
        }
        
        
//        else{
//            for(auto fr = it->second.tDSectionOccupations.begin(); fr != input->second.tDSectionOccupations.end(); fr++){
//
//        }
    }
    
    return retval;
}

