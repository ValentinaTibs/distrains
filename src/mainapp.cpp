#include <stdio.h>
#include <train.h>
#include <chrono>  // for high_resolution_clock
#include "tinyxml2.h"
#include <map>


static int num_trains = 3;

void lifecycle(void){
	std::this_thread::sleep_for(std::chrono::milliseconds(3));
	printf("/n done sleeping /n");
};

void broadcast(int idx, train* trains){
    for(auto i = 0 ; i < num_trains; i++){
        if (i != idx)
            trains[idx].sendmsg(&trains[i]);
    }
    return;
}


class tDSectionOccupation{
public:
    int occupationStart;
    std::string routeId;
    std::string tDSectionID;
    int trainSequenceID;
    int trackSequenceID;
    
    tDSectionOccupation(int _occupationStart,std::string _routeId,std::string _tDSectionID){
        this->occupationStart = _occupationStart;
        this->routeId = _routeId;
        this->tDSectionID = _tDSectionID;
    }
    void settrainSequenceID (int _trainSequenceID){this->trainSequenceID = _trainSequenceID;}
    void settrackSequenceID (int _trackSequenceID){this->trackSequenceID = _trackSequenceID;}
};

class rTTPForSingleTrain{
public:
    std::vector<tDSectionOccupation> tDSectionOccupations;
    std::string journeyId;
    std::string trainId;
    
    rTTPForSingleTrain(std::string _journeyId, std::string _trainId ){
        this->journeyId = _journeyId;
        this->trainId = _trainId;
    }
    void addtDSectionOccupation(tDSectionOccupation _newtDSectionOccupation){
        tDSectionOccupations.push_back(_newtDSectionOccupation);
    }
};


class RTTP{
public:
    
    // the key is the train ID
    std::map<std::string, rTTPForSingleTrain> rTTPTrainView;
    // the key is the train ID trackSequenceID
    std::map<std::string, std::vector<tDSectionOccupation> > rTTPInfrastructureView;
    
    
    
    RTTP(std::string filename){
       
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
                
                tinyxml2::XMLError queryResult1 = single_ele->ToElement()->QueryIntAttribute("occupationStart", &(occupationStart));
                tinyxml2::XMLError queryResult2 = single_ele->ToElement()->QueryIntAttribute("trainSequenceID", &(trainSequenceID));
                tinyxml2::XMLError queryResult3 = single_ele->ToElement()->QueryStringAttribute("routeId", &(routeId));
                tinyxml2::XMLError queryResult4 = single_ele->ToElement()->QueryStringAttribute("tDSectionID", &(tDSectionID));
                
                tDSectionOccupation newtDSOcc(occupationStart,std::string(routeId),std::string(tDSectionID));
                newtDSOcc.settrainSequenceID(trainSequenceID);
                
                newRTTPforSingleTrain.tDSectionOccupations.push_back(newtDSOcc);
            }
            rTTPTrainView.insert(std::pair<std::string, rTTPForSingleTrain>(std::string(trainId),newRTTPforSingleTrain));
        }
        
        ele = doc.FirstChildElement( "rTTP" )->FirstChildElement("rTTPInfrastructureView")->FirstChildElement();
        
        for( ; ele; ele = ele->NextSibling() ){
            const char* tDSectionId = "failed";
            tinyxml2::XMLError queryResult1 = ele->ToElement()->QueryStringAttribute("tDSectionId", &tDSectionId);
            
            std::vector<tDSectionOccupation> tDSectionOccupations;
            
            
            tinyxml2::XMLNode* single_ele = ele->FirstChildElement();
            for( ; single_ele; single_ele = single_ele->NextSibling() ){
                int occupationStart;
                int trackSequenceID;
                const char* routeId = "failed";
                const char* tDSectionID = "failed";
                
                tinyxml2::XMLError queryResult1 = single_ele->ToElement()->QueryIntAttribute("occupationStart", &(occupationStart));
                tinyxml2::XMLError queryResult2 = single_ele->ToElement()->QueryIntAttribute("trackSequenceID", &(trackSequenceID));
                tinyxml2::XMLError queryResult3 = single_ele->ToElement()->QueryStringAttribute("routeId", &(routeId));
                tinyxml2::XMLError queryResult4 = single_ele->ToElement()->QueryStringAttribute("tDSectionID", &(tDSectionID));
                
                tDSectionOccupation newtDSOcc(occupationStart,std::string(routeId),std::string(tDSectionID));
                newtDSOcc.settrackSequenceID(trackSequenceID);
                
                tDSectionOccupations.push_back(newtDSOcc);
            }
            rTTPInfrastructureView.insert(std::pair<std::string, std::vector<tDSectionOccupation> >(std::string(tDSectionId),tDSectionOccupations));
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
                printf("occupationStart=%d routeId=%s tDSectionID=%s trainSequenceID=%d \n",fr->occupationStart,fr->routeId.c_str(),fr->tDSectionID.c_str(),fr->trainSequenceID);
            }
        }
        for (auto it = rTTPInfrastructureView.begin(); it != rTTPInfrastructureView.end(); it++)
        {
            printf("\n\n TRACK --> %s \n",it->first.c_str());
            for (auto fr = it->second.begin(); fr != it->second.end();fr++){
                printf("occupationStart=%d routeId=%s tDSectionID=%s trackSequenceID=%d \n",fr->occupationStart,fr->routeId.c_str(),fr->tDSectionID.c_str(),fr->trackSequenceID);
            }
        }
    }
};



int main (int argc, char *argv[]) { 

//	bool not_done = true;
//    std::vector<train> trains;
//
//    /* ---- INIT all trains   ---- */
//	for (int i = 0 ; i < num_trains; i++){
//        trains.push_back(train());
//	}
//	/* ---- end of INIT all trains   ---- */
//
//
//	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
//
//	for (uint8_t i = 0 ; i < num_trains; i++){
//		trains[i].check();
//	}
//
//    trains[0].sendmsg(&trains[1]);
//    auto result = trains[1].readmsg();
//
//    //trains 1 queue must be empty
//    result.print();
    
//	while(not_done){
//
//		auto start = std::chrono::high_resolution_clock::now();
//		auto finish = std::chrono::high_resolution_clock::now();
//		std::chrono::duration<double> elapsed = finish - start;
//
//		printf( "Elapsed time:, %f s\n", elapsed.count());
//
//		not_done = false;
//	}
    
    // this conf should be put in a json
    std::string fileRTT_anc = "RTTP.3branch.xml";//,  fileRTT_A, fileRTT_B;
    RTTP RTT_anc = RTTP(fileRTT_anc);
    RTT_anc.printAll();

//    RTT* RTT_anc = load_RTT(fileRTT_anc);
//    RTT* RTT_A = load_RTT(fileRTT_A);
//    RTT* RTT_B = load_RTT(fileRTT_B);
    
//    tinyxml2::XMLDocument doc;
//    doc.LoadFile( "RTTP.3branch.xml" );
//    printf("\nLoading of file: result %d\n",doc.ErrorID());
//
//
//    const char* v = "failed";
//
//    for( tinyxml2::XMLNode* ele = doc.FirstChildElement( "PLAY" )->FirstChildElement("TITLE"); ele; ele = ele->NextSibling() ){
//        const tinyxml2::XMLAttribute* attrib = ele->ToElement()->FindAttribute("attr0");
//        attrib->
//        XMLAttribute >Value()
//        printf( " %s\n",v );
//    }

    

	printf("\nexecution Done\n");
	return 0;
}
