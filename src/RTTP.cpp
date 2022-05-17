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

void tDSectionOccupation::settrainSequenceID (int _trainSequenceID){
    this->trainSequenceID = _trainSequenceID;
}

tdSecKey tDSectionOccupation::makeKey(void){
    //(hash_string(tDSectionKey.c_str(),app_seed)
    return this->trainId + this->tDSectionID;
}

void RTTP::addtDSectionOccupation(tDSectionOccupation _newtDSectionOccupation){
    tdSecKey tDSectionKey = _newtDSectionOccupation.makeKey();
    RTTP_member new_RTTP_member(tDSectionKey,  _newtDSectionOccupation);
    tDSectionOccupations.push_back(new_RTTP_member);
}

// 2do HERE a better find done throught a better data structure could boost performances
RTTP_member* RTTP::find_tDSectionOccupation(RTTP_member _tDSO){
    for(auto it = tDSectionOccupations.begin(); it != tDSectionOccupations.end();it++){
        if( *it == _tDSO ){
            return &(*it);
        }
    }
    return nullptr ;
}

RTTP::RTTP(std::string filename){

    // 2do metterci un check per vedere se il file esiste
    tinyxml2::XMLDocument doc;
    doc.LoadFile( filename.c_str() );

    // 2do metterci un check che si assicuri l'esistenza sia di rTTP che di rTTPTrainView
    // e che altrimenti ritorni un errore di file malformato
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

        printf("\n\n TRAIN --> %s journeyID ---> %s \n",it->cnt.second.trainId.c_str(),it->cnt.second.journeyId.c_str());
        
        printf("KEY=%s occupationStart=%d routeId=%s tDSectionID=%s trainSequenceID=%d \n",it->cnt.first.c_str(), it->cnt.second.occupationStart,it->cnt.second.routeId.c_str(),it->cnt.second.tDSectionID.c_str(),it->cnt.second.trainSequenceID);
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

    //  2do WE MUST SORT tDSectionOccupations at first
    for (auto it = tDSectionOccupations.begin(); it != tDSectionOccupations.end(); it++)
    {
            // se non c'è l'elemento con chiave fr->second.tDSectionID aggiungerlo
            auto found_key = rTTPInfrastructureView.find(it->cnt.second.tDSectionID);
            if(found_key == rTTPInfrastructureView.end()){
                std::string new_key = it->cnt.second.tDSectionID;
                rTTPInfrastructureView.insert(std::pair<std::string, std::vector<tDSectionOccupation> > ( new_key,std::vector<tDSectionOccupation>()));
            }
            tDSectionOccupation new_one_elem(it->cnt.second.occupationStart,it->cnt.second.routeId,it->cnt.second.tDSectionID,it->cnt.second.journeyId,it->cnt.second.trainId);
            new_one_elem.trainId = it->cnt.second.trainId;
            rTTPInfrastructureView.at(it->cnt.second.tDSectionID).push_back(new_one_elem);
    }

    fprintf(fp,"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n");

    tinyxml2::XMLPrinter printer( fp );
    printer.OpenElement( "rTTP" );
    printer.OpenElement( "rTTPTrainView" );

    std::string last_train_id = "";
    
    for (auto it = tDSectionOccupations.begin(); it != tDSectionOccupations.end(); it++)
    {
        // if this tdSection is referring to something we havent' seen before
        if(it->cnt.second.trainId != last_train_id)
        {
            // if we are not in the first loop
            if(last_train_id != ""){
                //close rTTPForSingleTrains
                printer.CloseElement();
            }
            
            // open rTTPForSingleTrain
            printer.OpenElement( "rTTPForSingleTrain" );
            printer.PushAttribute( "journeyId", it->cnt.second.journeyId.c_str() );
            printer.PushAttribute( "trainId", it->cnt.second.trainId.c_str() );
            
            last_train_id = it->cnt.second.trainId;
        }
        
        int count = 0;
        
        printer.OpenElement( "tDSectionOccupation" );
        printer.PushAttribute( "occupationStart", std::to_string(it->cnt.second.occupationStart).c_str() );
        printer.PushAttribute( "routeId", it->cnt.second.routeId.c_str() );
        printer.PushAttribute( "tDSectionID", it->cnt.second.tDSectionID.c_str() );
        printer.PushAttribute( "trainID", it->cnt.second.trainId.c_str() );
        printer.PushAttribute( "trainSequenceID", std::to_string(it->cnt.second.trainSequenceID).c_str());
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

// data una RTTP estrae una sotto parte che riguarda solo un tot di treni
RTTP perturbate_RTTP(void){
    std::string fileRTTP = "../../data/base/RTTP.3branch_A.xml";
    RTTP RTTP_pert = RTTP(fileRTTP);
    return RTTP_pert;
}
#include <algorithm>

RTTP perturbate_RTTP(RTTP* _in,std::vector<std::string> train_Ids){
    RTTP RTTP_pert;
    
    for(auto it = _in->tDSectionOccupations.begin(); it != _in->tDSectionOccupations.end();it++){
            if(std::find(train_Ids.begin(), train_Ids.end(), it->cnt.second.trainId)!= train_Ids.end())
                RTTP_pert.addtDSectionOccupation(it->cnt.second);
    }
    return RTTP_pert;
}

void print_all_trains(RTTP* _in){
    std::vector<std::string> all_trains;
    for(auto it = _in->tDSectionOccupations.begin(); it != _in->tDSectionOccupations.end();it++){
        if (std::find(all_trains.begin(), all_trains.end(), it->cnt.second.trainId) == all_trains.end()) {
            all_trains.push_back( it->cnt.second.trainId);
            printf(" \n -> train %s ", it->cnt.second.trainId.c_str());
        }
    }
    printf(" \n ");
}



//int merge(RTTP* L, RTTP* R){
//
//    errorCode retval=MERGE_SUCCESS;
//
//    for(auto it = L->tDSectionOccupations.begin(); it != L->tDSectionOccupations.end();it++){
//        auto found = R->find_tDSectionOccupation(*it);
//        if (found != nullptr) {
//            if(found->cnt.second==it->cnt.second){
//                printf(" FOUND %s ", it->cnt.first.c_str());
//            }
//            else{
//                printf(" UPDATED %s ", it->cnt.first.c_str());
//                results.add_op(op_type::UPDATE,found->cnt.second,it->cnt.second);
//            }
//        } else {
//            printf(" DELETED %s ", it->cnt.first.c_str());
//            results.add_op(op_type::DEL,it->cnt.second);
//        }
//    }
//
//    printf("\n");
//
//    for(auto it = R->tDSectionOccupations.begin(); it != R->tDSectionOccupations.end();it++){
//        auto found = L->find_tDSectionOccupation(*it);
//        if (found != nullptr) {
//            printf(" FOUND %s ", it->cnt.first.c_str());
//        } else {
//            printf(" ADDED %s ", it->cnt.first.c_str());
//            results.add_op(op_type::ADD,found->cnt.second);
//        }
//    }
//
////
////    // for all elements in the TrainView
////    for(auto it = input->tDSectionOccupations.begin(); it != input->tDSectionOccupations.end(); it++){
////
////        // THE ELEMENT IS NOT PRESENT IN THE ANCESTOR - search by key and add it
////        if( L->tDSectionOccupations.find(it->first) == ANC->tDSectionOccupations.end() )
////        {
////            // can be not presence because some of the element that constitute the key have changed
////            // so we must first check if something between the two keys hasn't changed
////            ANC->addtDSectionOccupation(it->second);
////        }
////
////
//////        else{
//////            for(auto fr = it->second.tDSectionOccupations.begin(); fr != input->second.tDSectionOccupations.end(); fr++){
//////
//////        }
////    }
//
//    return retval;
//}



//int diff(RTTP* ANC, RTTP* input){
//    errorCode retval=MERGE_SUCCESS;
//    return retval;
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
