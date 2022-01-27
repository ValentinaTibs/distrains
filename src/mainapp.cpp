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


class RTTP{
public:
    std::map<std::string, tinyxml2::XMLDocument> rTTPTrainView;
    //std::map<std::string, tinyxml2::XMLNode> rTTPInfrastructureView;
    
    RTTP(std::string filename){
        
        tinyxml2::XMLDocument doc;
        doc.LoadFile( filename.c_str() );
        
        const char* v = "failed";
        
        tinyxml2::XMLNode* ele = doc.FirstChildElement( "rTTP" )->FirstChildElement("rTTPTrainView")->FirstChildElement();
        
        for( ; ele; ele = ele->NextSibling() ){
            tinyxml2::XMLError queryResult = ele->ToElement()->QueryStringAttribute("trainId", &v);
            printf( " %s\n",v );
            //std::pair<std::string,tinyxml2::XMLNode> gianni(,ele);
            //rTTPTrainView[std::string(v)] = ele;
            tinyxml2::XMLDocument target;
            ele->DeepClone(&target);
            //rTTPTrainView.insert(std::pair<std::string, tinyxml2::XMLDocument>(,target));
            rTTPTrainView[std::string(v)] = target;
        }

    }

    virtual ~RTTP(){
        return;
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
//    std::string fileRTT_anc = "RTTP.3branch.xml";//,  fileRTT_A, fileRTT_B;
//    RTTP RTT_anc = RTTP(fileRTT_anc);

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
