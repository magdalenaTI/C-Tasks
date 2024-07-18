#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdint.h>

int main(int argc, char* argv[]){
	if(argc!=3){
		err(1,"Invalid input");
	}

	char* SCL = argv[1];
	char* SDL = argv[2];

	struct stat s1, s2;

	if(stat(SCL, &s1) == -1 || stat(SDL,&s2)==-1){
		err(2,"Can not stat");
	}
	if(s1.st_size * 8 != (s2.st_size/2)){
		err(3,"Invalid content!");
	}

	int fd1,fd2,fd3;
	fd1=open(SCL,O_RDONLY);
	if(fd1==-1){
		err(4,"Can not open SCL");
	}
	fd2=open(SDL,O_RDONLY);
	if(fd2==-1){
		err(4,"Can not open SDL");
	}
	fd3=open("result.txt",O_CREAT|O_TRUNC|O_RDWR,0666);
	if(fd3==-1){
		err(4,"Can not open result");
	}

	int bytes;
	uint8_t buff;
	while((bytes = read(fd1,&buff,sizeof(buff))) == sizeof(buff)){
		for(int i = 7; i >= 0; i--){
			uint16_t num;
			int rs;
			if((rs = read(fd2,&num,sizeof(num)))!=sizeof(num)){
				err(5,"can not read");
			}
			if((buff >> i) & 1){
				int ws;
				if((ws=write(fd3,&num,sizeof(num)))!=sizeof(num)){
					err(6,"Can not write");
				}
			}
		}
	}
	if(bytes == -1){
		err(7,"Can not read");
	}
	close(fd1);
	close(fd2);
	close(fd3);
}
