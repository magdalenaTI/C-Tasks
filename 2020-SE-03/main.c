#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAXP 8
#define MAXN 8

typedef struct{
	char fname[MAXN];
	uint32_t off;
	uint32_t len;
}row_t;

int main(int argc,char* argv[]){
	if(argc!=2){
		errx(1,"Invalid arguments!");
	}
	row_t arr[MAXP];
	int fd=0;
	if((fd=open(argv[1],O_RDONLY))==-1){
		err(2,"Can not open file");
	}

	struct stat st;
	if(fstat(fd,&st)==-1){
		err(3,"Can not stat %s",argv[1]);
	}
	if(((long unsigned int)st.st_size>sizeof(arr))||
			(st.st_size % sizeof(row_t)!=0)){
		err(4,"Inconsistent %s",argv[1]);
	}

	size_t nrows = st.st_size / sizeof(row_t);

	if(read(fd,arr,nrows * sizeof(row_t))!=(ssize_t)(nrows*sizeof(row_t))){
		err(5,"Can not read rows from %s",argv[1]);
	}
	close(fd);
	
	for(size_t i=0;i < nrows;i++){
		if(arr[i].fname[MAXN-1]!=0x00){
			errx(6,"Wrong file content!");
		}
	}

	int pf[2];
	if(pipe(pf)==-1){
		err(7,"Pipe failed");
	}

	pid_t pid=0;
	size_t i=0;

	for(i=0; i < nrows;i++){
		if((pid=fork())==-1){
			err(1,"Can not fork");
		}
		if(pid==0){
			close(pf[0]);
			break; // not special logic
			//just not too many if-s in one  for
		}
	}
	//the program remembers how much is i
	if(pid==0){
		if((fd=open(arr[i].fname,O_RDONLY))==-1){
			err(8,"Can not open");
		}
		if(fstat(fd,&st)==-1){
			err(9,"Can not fstat");
		}

		if(((arr[i].off + arr[i].len)*sizeof(uint16_t)) > 
				(long unsigned int)st.st_size){
			err(10,"wrong size of %s based on %s", arr[i].fname,argv[1]);
		}

		if(lseek(fd,arr[i].off*sizeof(uint16_t),SEEK_SET)==-1){
			err(11,"Lseek failed");
		}
		uint16_t result=0x0000;
		uint16_t e = 0x0000;

		for(uint32_t j=0; j < arr[i].len;j++){
			if(read(fd, &e, sizeof(e))!=sizeof(e)){
				err(12,"Can not read");
			}
			result^=e;
		}
		if(write(pf[1],&result,sizeof(result))!=sizeof(result)){
			err(13,"Can not write to pipe!");
		}

		close(fd);
		close(pf[1]);
		exit(0);
	}

	close(pf[1]);
	uint16_t res = 0x0000;
	uint16_t rd = 0x0000;

	while(read(pf[0],&rd, sizeof(rd))==sizeof(rd)){
		res^=rd;
	}
	dprintf(1,"result: %04X\n",res);
	return 0;
}




