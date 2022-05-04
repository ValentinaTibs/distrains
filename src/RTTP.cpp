//
//  RTTP.h
//  distrains
//
//  Created by vale on 28/01/22.
//

#include "RTTP.h"
#include "tinyxml2.h"
#include <vector>

tDSectionOccupation::tDSectionOccupation(int _occupationStart,std::string _routeId,std::string _tDSectionID, std::string _journeyId, std::string _trainId){
    this->occupationStart = _occupationStart;
    this->routeId = _routeId;
    this->tDSectionID = _tDSectionID;
    this->journeyId = _journeyId;
    this->trainId = _trainId;
}

void tDSectionOccupation::settrainSequenceID (int _trainSequenceID){this->trainSequenceID = _trainSequenceID;}

std::string tDSectionOccupation::makeKey(void){
    return this->trainId + std::to_string(this->occupationStart);}

void RTTP::addtDSectionOccupation(tDSectionOccupation _newtDSectionOccupation){
    std::string tDSectionKey = _newtDSectionOccupation.makeKey();
    tDSectionOccupations.insert(std::pair<tdSecKey, tDSectionOccupation >(hash_string(tDSectionKey.c_str(),app_seed),_newtDSectionOccupation));
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

            tDSectionOccupation newtDSOcc(occupationStart,std::string(routeId),std::string(tDSectionID),std::string(journeyId),std::string(trainId) );

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

                tDSectionOccupation newtDSOcc(occupationStart,std::string(routeId),std::string(tDSectionID),std::string(journeyId),std::string(trainId) );
            }
            newtDSOcc.settrainSequenceID(trainSequenceID);
            // we assume here which is thhe key for a TDSectionOccupation from the train point of view
            this->addtDSectionOccupation(newtDSOcc);
        }
    }
    return;
}

void RTTP::printAll(void){
    for (auto it = tDSectionOccupations.begin(); it != tDSectionOccupations.end(); it++){

        printf("\n\n TRAIN --> %s journeyID ---> %s \n",it->second.trainId.c_str(),it->second.journeyId.c_str());
        
        printf("KEY=%d occupationStart=%d routeId=%s tDSectionID=%s trainSequenceID=%d \n",it->first, it->second.occupationStart,it->second.routeId.c_str(),it->second.tDSectionID.c_str(),it->second.trainSequenceID);
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

    for (auto it = tDSectionOccupations.begin(); it != tDSectionOccupations.end(); it++)
    {
            // se non c'è l'elemento con chiave fr->second.tDSectionID aggiungerlo
            auto found_key = rTTPInfrastructureView.find(it->second.tDSectionID);
            if(found_key == rTTPInfrastructureView.end()){
                std::string new_key = it->second.tDSectionID;
                rTTPInfrastructureView.insert(std::pair<std::string, std::vector<tDSectionOccupation> > ( new_key,std::vector<tDSectionOccupation>()));
            }
            tDSectionOccupation new_one_elem(it->second.occupationStart,it->second.routeId,it->second.tDSectionID,it->second.journeyId,it->second.trainId);
            new_one_elem.trainId = it->second.trainId;
            rTTPInfrastructureView.at(it->second.tDSectionID).push_back(new_one_elem);

    }

    fprintf(fp,"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n");

    tinyxml2::XMLPrinter printer( fp );
    printer.OpenElement( "rTTP" );
    printer.OpenElement( "rTTPTrainView" );

    std::string last_train_id = "";
    
    for (auto it = tDSectionOccupations.begin(); it != tDSectionOccupations.end(); it++)
    {
        // if this tdSection is referring to something we havent' seen before
        if(it->second.trainId != last_train_id)
        {
            // if we are not in the first loop
            if(last_train_id != ""){
                //close rTTPForSingleTrains
                printer.CloseElement();
            }
            
            // open rTTPForSingleTrain
            printer.OpenElement( "rTTPForSingleTrain" );
            printer.PushAttribute( "journeyId", it->second.journeyId.c_str() );
            printer.PushAttribute( "trainId", it->second.trainId.c_str() );
            
            last_train_id = it->second.trainId;
        }

        int count = 0;
        
        printer.OpenElement( "tDSectionOccupation" );
        printer.PushAttribute( "occupationStart", std::to_string(it->second.occupationStart).c_str() );
        printer.PushAttribute( "routeId", it->second.routeId.c_str() );
        printer.PushAttribute( "tDSectionID", it->second.tDSectionID.c_str() );
        printer.PushAttribute( "trainID", it->second.trainId.c_str() );
        printer.PushAttribute( "trainSequenceID", std::to_string(count).c_str());
        count++;

        //close tDSectionOccupation
        printer.CloseElement();
    }

    //close last rTTPForSingleTrains
    printer.CloseElement();

    
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
            printer.PushAttribute( "trainID", fr->trainId.c_str());
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

RTTP_diff::RTTP_diff(){
    this->op = "stocazzo";
};

//int merge(RTTP* ANC, RTTP* input){
//    errorCode retval=MERGE_SUCCESS;
//
//    // for all elements in the TrainView
//    for(auto it = input->tDSectionOccupations.begin(); it != input->tDSectionOccupations.end(); it++){
//
//        // THE ELEMENT IS NOT PRESENT IN THE ANCESTOR - search by key and add it
//        if( ANC->tDSectionOccupations.find(it->first) == ANC->tDSectionOccupations.end() )
//        {
//            // can be not presence because some of the element that constitute the key have changed
//            // so we must first check if something between the two keys hasn't changed
//            ANC->addtDSectionOccupation(it->second);
//        }
//
//
////        else{
////            for(auto fr = it->second.tDSectionOccupations.begin(); fr != input->second.tDSectionOccupations.end(); fr++){
////
////        }
//    }
//
//    return retval;
//}
//
//int diff(RTTP* ANC, RTTP* input){
//    errorCode retval=MERGE_SUCCESS;
//    return retval;
//}



//RTTP perturbate_RTTP(void){
//    std::string fileRTTP = "../../data/base/RTTP.3branch_A.xml";
//    RTTP RTTP_pert = RTTP(fileRTTP);
//    return RTTP_pert;
//}

//rTTPForSingleTrain::rTTPForSingleTrain(std::string _journeyId, std::string _trainId ){
//    this->journeyId = _journeyId;
//    this->trainId = _trainId;
//}


//void RTTP::addrTTPForSingleTrain(std::string _trainId,rTTPForSingleTrain newRTTPforSingleTrain ){
//    this->rTTPTrainView.insert(std::pair<std::string, rTTPForSingleTrain>(_trainId,newRTTPforSingleTrain));
//}
        
//RTTP_diff diff(RTTP* ANC, RTTP* input){
//    errorCode retval=MERGE_SUCCESS;
//
////    for(auto it = input->rTTPTrainView.begin(); it != input->rTTPTrainView.end(); it++){
////
////            // THE ELEMENT IS NOT PRESENT IN THE ANCESTOR - search by key and add it
////        if( ANC->rTTPTrainView.find(it->first) == ANC->rTTPTrainView.end() )
////        {
////            // can be not presence because some of the element that constitute the key have changed
////            // so we must first check if something between the two keys hasn't changed
////
////            ANC->addrTTPForSingleTrain(it->first,it->second);
////        }
////
////
//////        else{
//////            for(auto fr = it->second.tDSectionOccupations.begin(); fr != input->second.tDSectionOccupations.end(); fr++){
//////
//////        }
////    }
////
////    return retval;
//    //return NULL;
//}
