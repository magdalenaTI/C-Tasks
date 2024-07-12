#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
	if(argc != 1){
		err(1,"parameter err");
	}

	pid_t pid;
	pid = fork();
	if(pid == -1){
		err(2,"err forking");
	}

	if(pid==0){
		if(execlp("touch", "touch","fooFile",0)==-1){
			err(2,"err execling");
		}
	}
	int stat;
	pid_t cpid = waitpid(pid,&stat,0);
	
	if(cpid ==-1){
		err(3,"Can not wait");
	}
	int i=0;
	while(i<10){
	pid_t pd1;
	pid_t pd2;

	pd1=fork();
	
	if(pd1 == -1){
		err(2,"err forking");
	}

	int st1;
	waitpid(pd1,&st1,0);
	int fd=open("fooFile",O_RDWR|O_APPEND);
	if(fd<0){
		err(2,"err opening file");
	}
	
	if(pd1==0){
		write(fd,"foo\n",4);
		exit(0);
	}
	close(fd);

	int st2;
	pd2=fork();
	waitpid(pd2,&st2,0);

	if(pd2 == -1){
		err(2,"err forking");
	}
	int fd1=open("fooFile",O_RDWR|O_APPEND);
	
	if(pd2==0){
		write(fd1,"bar\n",4);
		exit(0);
	}
	waitpid(pd2,&st2,0);
	close(fd1);
	i++;
	}
	exit(0);
}





