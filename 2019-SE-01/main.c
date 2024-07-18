#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char* argv[]){
	if(argc<3){
		err(1,"Wrong number of arguments!");
	}
	int fd=open("run.log",O_CREAT|O_TRUNC|O_RDWR,0666);
	if(fd==-1){
		err(2,"Can not open file!");
	}
	int time_needed = argv[1][0]-'0';
	int counter=0;
	while(counter<10){
		int pf[2];
		if(pipe(pf)==-1){
			err(3,"Pipe failure");
		}
		long finish;
		long start;
		int ex_status;
		pid_t pid=fork();
		if(pid==-1){
			err(4,"Can not fork");
		}
		if(pid==0){
			start = time(NULL);
			close(pf[0]);
			if(write(pf[1],&start,sizeof(start))!=sizeof(start)){
				err(5,"Writing error");
			}
			
			if(execvp(argv[2],argv+2)==-1){
				err(6,"Fork err");
			}
		}
		close(pf[1]);
		int status;
		wait(&status);
		finish=time(NULL);
		ex_status=WEXITSTATUS(status);

		long st;
		if(read(pf[0],&st,sizeof(st))!= sizeof(st)){
			err(7,"reading err");
		}

		if(write(fd,&st,sizeof(st))!=sizeof(st)){
			err(4,"writing err");
		}
  		if(write(fd,&finish,sizeof(finish))!=sizeof(finish)){
			err(4,"writing err");
		}

		if(write(fd,&ex_status,sizeof(ex_status))!=sizeof(ex_status)){
			err(4,"writing err");
		}
	//	dprintf(1,"%d %d %d\n",st,finish,ex_status);
		if(ex_status != 0 && finish - st < time_needed){
			exit(129);
		}
		close(pf[0]);
		counter++;
	}


}
