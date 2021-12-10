#include <stdio.h>
#include <train.h>

int main (int argc, char *argv[]) { 
	
	train* pendolino = new train();
	pendolino->check();

	printf("\nexecution Done\n");
	return 0;
}
