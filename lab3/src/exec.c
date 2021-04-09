#include <stdio.h>
#include <unistd.h>

int main( void ) {
	char *argv[4] = {"sequential_min_max", "1", "10",NULL};

	int pid = fork();

	if ( pid == 0 ) {
        execv("sequential_min_max",  argv);
	}

	wait(0);

	

	return 0;
}