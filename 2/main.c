#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <stdio.h>

int main(int argc, char* argv[]){
	if(argc != 4 ){
		errx(1,"err in arguments");
	}
	pid_t pid;
	int i=1;

	while(i<4){
		pid = fork();
		if(pid == -1){
			errx(2,"err forking");
		}

	if(pid == 0){
		if(execlp(argv[i],argv[i],NULL)==-1){
			err(2,"err execling");
		}
	}

	int stat;
	pid_t cpid = waitpid(pid, &stat,0);
	if(WIFEXITED(stat)){
		printf("Child %d terminated with status: %d\n",cpid, WEXITSTATUS(stat));
	}
	i++;
	}

	exit(0);
}

