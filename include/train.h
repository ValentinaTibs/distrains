#include <stdio.h>

class train{
	public:
		bool is_running ;
		train() { printf("Constructing base \n"); 
			is_running = false;
		}
		virtual ~train(){ printf("Destructing base \n"); }
		int check(void);
};
