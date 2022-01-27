#ifndef train_h
#define train_h

#include <stdio.h>
#include <thread>
#include <queue>

class msg{
public:
    std::string content;
    msg(){content = "";}
    msg(int name){ content = std::to_string(name);}
    void print(void){printf(" %s ",this->content.c_str());};
};

class train{

private: 
	int name;
    std::queue<train*> read_from;
    msg last_msg;

public:
	bool is_running ;

	train(){
		printf("Constructing base \n"); 

		// this is faux rand but is nice for debugging
		name = rand() % 100; 
		is_running = false;
        last_msg = msg(name);
	}

	virtual ~train(){ 
		printf("Destructing the train \n"); 
	}

	friend bool operator==(const train& lhs, const train& rhs){
		if (lhs.name == rhs.name)
			return true;
		return false;
	};

	int check(void);
	void lifecycle(int);
    int sendmsg(train* );
    msg readmsg();
    msg getmsg();
};

#endif
