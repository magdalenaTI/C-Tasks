#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){

	if(argc != 3){
		errx(2,"err in param number");
	}

	pid_t pid;
	pid=fork();
	if(pid==-1){
		err(2,"err forking");
	}
	if(pid==0){
		if(execlp(argv[1],argv[1],NULL)==-1){
			err(3,"err execling");
		}
	}

	int st;
	waitpid(pid,&st,0);

	if(WIFEXITED(st)){
		if(execlp(argv[2],argv[2],NULL)==-1){
			err(3,"err execlong");
		}		
	}else{
		exit(42);
	}


}
