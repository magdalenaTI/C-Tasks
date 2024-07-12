#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <sys/wait.h>

int main(int argc,char* argv[]){
	pid_t pid;
	int errors=0;
	int success=0;
	for(int i=1;i<argc;i++){
		pid = fork();
		if(pid == -1){
			err(2,"err forking");
		}
		if(pid == 0){
			if(execlp(argv[i],argv[i],NULL)==-1){
				err(2,"err execling");
			}
		}
		int stat;
		waitpid(pid,&stat,0);
		if(WIFEXITED(stat)){
			success++;	
		}
		else{
			errors++;
		}
	}
	printf("Errors:%d\nSUCCESS:%d\n",errors,success);
	exit(0);
}
