#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

int main(int argc,char* argv[]){
	if(argc!=4){
		errx(2,"err param");
	}
	int fd=open(argv[3],O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	if(fd<0){
		err(1,"can not open file");
	}
	pid_t pd1=fork();
	if(pd1==-1){
		err(3,"err forking");
	}
	if(pd1==0){
		if(execlp(argv[1],argv[1],NULL)==-1){
			err(4,"err exec");
		}
	}
	int stat;
	waitpid(pd1,&stat,0);
	if(WIFEXITED(stat)){
		write(fd,argv[1],strlen(argv[1]));
		write(fd,"\n",1);
		close(fd);
	}
	else{

		err(5,"err process terminating");
	}
	int fd2=open(argv[3],O_WRONLY|O_APPEND);
	if(fd2<0){
		err(1,"can not open file");
	}

	pid_t pd2=fork();
	if(pd2==-1){
		err(3,"err forking");
	}
	if(pd2==0){
		if(execlp(argv[2],argv[2],NULL)==-1){
			err(4,"err exec");
		}
	}
	int st;
	waitpid(pd2,&st,0);
	if(WIFEXITED(st)){
		write(fd2,argv[2],strlen(argv[2]));
		write(fd2,"\n",1);
		close(fd2);
	}
	else{

		err(5,"err process terminating");
	}
	exit(0);

}
