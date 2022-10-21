#include <stdio.h>


int main(){
	/*
	 * program needs to fork() as many times as needed using pipes to comm w/
	 * children
	 *	equation specified in first line of data file determines the forking
	 *	once setup is done, parent task reads data from data file and
	 *	pumps it into various pipes in parallel and results come back to
	 *	the main task using another pipe
	 * /

	return 0;
}
