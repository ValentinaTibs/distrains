#include <stdio.h>
#include <chrono>
#include <thread>
#include "train.h"

int train::check(void){
	
	printf("Everything is fine for %d \n",this->name);
	return 0;
};
	

int train::sendmsg(train* receiver){
    receiver->read_from.push(this);
    return 0;
}

msg train::getmsg(){
    return this->last_msg;
}

msg train::readmsg(){
    msg result;
    train* target_train = this->read_from.front();
    this->read_from.pop();
    // manage here the empty msg
    if(target_train != nullptr)
        return target_train->getmsg();
    
}
