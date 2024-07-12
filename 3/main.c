#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char*argv[]){

	if(argc != 2){
		err(1,"Wrong args");
	}

	int status;
	int fd = open(argv[1],O_WRONLY| O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);

	if(fd<0){
		err(1,"can not open file");
	}

	pid_t pid = fork();
	if(pid == -1){
		err(1,"Filed fork");
	}
	if(pid==0){
		write(fd,"foobar\n", 7);
	}
	else{
		pid_t cpid = wait(&status);
		if(cpid < 0){ err(1,"Cannot wait for child");
		}
		char c[2];
		ssize_t read_size;
		int fd2 = open(argv[1],O_RDWR);
		while((read_size = read(fd2,&c, sizeof(c)))>0){
			if(write(1,&c,read_size)!= read_size){
				err(1,"err writing");
			}
			write(1, " ", 1);
		}
}
	

	close(fd);
	exit(0);
}

