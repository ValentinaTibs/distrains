//
//  RTTP.h
//  distrains
//
//  Created by vale on 28/01/22.
//

#include "RTTP.h"
#include "tinyxml2.h"
#include <vector>

tDSectionOccupation::tDSectionOccupation(int _occupationStart,std::string _routeId,std::string _tDSectionID){
    this->occupationStart = _occupationStart;
    this->routeId = _routeId;
    this->tDSectionID = _tDSectionID;
}
void tDSectionOccupation::settrainSequenceID (int _trainSequenceID){this->trainSequenceID = _trainSequenceID;}

std::string tDSectionOccupation::makeKey(void){
    return std::to_string(this->occupationStart);
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
    
    printf("=================== \n");
}

void RTTP::dump(std::string filename){
    FILE *fp;
    
    if((fp=fopen(filename.c_str(), "w+"))==NULL) {
        printf("Error in opening file");
        return;
    }
    
    std::map<std::string,std::vector<tDSectionOccupation> > rTTPInfrastructureView;
    
    
    for (auto it = rTTPTrainView.begin(); it != rTTPTrainView.end(); it++)
    {
        for (auto fr = it->second.tDSectionOccupations.begin(); fr != it->second.tDSectionOccupations.end();fr++)
        {
            
            // se non c'è l'elemento con chiave fr->second.tDSectionID aggiungerlo
            auto found_key = rTTPInfrastructureView.find(fr->second.tDSectionID);
            if(found_key == rTTPInfrastructureView.end()){
                std::string new_key = fr->second.tDSectionID;
                rTTPInfrastructureView.insert(std::pair<std::string, std::vector<tDSectionOccupation> > ( new_key,std::vector<tDSectionOccupation>()));
            }
            tDSectionOccupation new_one_elem(fr->second.occupationStart,fr->second.routeId,fr->second.tDSectionID);
            new_one_elem.trainID = it->second.trainId;
            rTTPInfrastructureView.at(fr->second.tDSectionID).push_back(new_one_elem);
        }
    }
    
    
    fprintf(fp,"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n");

    tinyxml2::XMLPrinter printer( fp );
    printer.OpenElement( "rTTP" );
    printer.OpenElement( "rTTPTrainView" );

    for (auto it = rTTPTrainView.begin(); it != rTTPTrainView.end(); it++)
    {

        printer.OpenElement( "rTTPForSingleTrain" );
        printer.PushAttribute( "journeyId", it->second.journeyId.c_str() );
        printer.PushAttribute( "trainId", it->second.trainId.c_str() );

        int count = 0;
        for (auto fr = it->second.tDSectionOccupations.begin(); fr != it->second.tDSectionOccupations.end();fr++){

            printer.OpenElement( "tDSectionOccupation" );
            printer.PushAttribute( "occupationStart", std::to_string(fr->second.occupationStart).c_str() );
            printer.PushAttribute( "routeId", fr->second.routeId.c_str() );
            printer.PushAttribute( "tDSectionID", fr->second.tDSectionID.c_str() );
            printer.PushAttribute( "trainID", it->second.trainId.c_str() );
            printer.PushAttribute( "trainSequenceID", std::to_string(count).c_str());
            count++;
            //rTTPForSingleTrain
            printer.CloseElement();
        }

        //rTTPForSingleTrain
        printer.CloseElement();
    }
    
    //rTTPTrainView
    printer.CloseElement();
    fprintf(fp,"\n");

    
    printer.OpenElement( "rTTPInfrastructureView" );
    for (auto it = rTTPInfrastructureView.begin(); it != rTTPInfrastructureView.end(); it++)
    {
        printer.OpenElement( "rTTPForSingleTDSection" );
        printer.PushAttribute( "tDSectionId", it->first.c_str() );

        int count = 0;
        for (auto fr = it->second.begin(); fr != it->second.end();fr++){
            printer.OpenElement( "tDSectionOccupation" );
            printer.PushAttribute( "occupationStart", std::to_string(fr->occupationStart).c_str());
            printer.PushAttribute( "routeId", fr->routeId.c_str());
            printer.PushAttribute( "tDSectionID", fr->tDSectionID.c_str());
            printer.PushAttribute( "trackSequenceID", std::to_string(count).c_str());
            printer.PushAttribute( "trainID", fr->trainID.c_str());

            count++;

            printer.CloseElement();
        }
        
        //rTTPForSingleTDSection
        printer.CloseElement();
    }
    
    //rTTPInfrastructureView
    printer.CloseElement();

    //rTTP
    printer.CloseElement();
    
    fclose(fp);
};

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

