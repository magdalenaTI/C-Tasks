#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <err.h>
#include <sys/stat.h>
#include <stdlib.h>

typedef struct{
	uint16_t magic;
	uint16_t filetype;
	uint32_t count;
}header;

typedef struct{
	uint16_t offset_data; //in data
	uint16_t offset_out; // in out
}list;

int main(int argc,char* argv[]){
	if(argc!=4){
		errx(1,"Invalid arguments!");
	}
	
	int fd1=open(argv[1],O_RDONLY);
	if(fd1==-1){
		err(2,"Can not open %s",argv[1]);
	}
	int fd2=open(argv[2],O_RDONLY);
	if(fd2==-1){
		err(2,"Can not open %s", argv[2]);
	}
	int fd3=open(argv[3],O_CREAT | O_TRUNC | O_RDWR, 0666);
	if(fd3==-1){
		err(2,"Can not open %s", argv[3]);
	}

	header h1;
	int rs = read(fd1,&h1, sizeof(h1));
	if(rs!= sizeof(h1)){
		err(3,"Error in reading from %s", argv[1]);
	}
	if(h1.magic != 0x5A4D || h1.filetype != 1){
		errx(4,"Invalid type or magic key!");
	}

	header h2;
	int rs2 = read(fd2,&h2, sizeof(h2));
	if(rs2!= sizeof(h2)){
		err(3,"Error in reading from %s", argv[2]);
	}
	if(h2.magic != 0x5A4D || h2.filetype != 2){
		errx(4,"Invalid type or magic key!");
	}

	struct stat st;
	if(stat(argv[3],&st)==-1){
		err(5,"Stat failure!");
	}
	if(st.st_size==0){
		header h;
		h.magic=0x5A4D;
		h.filetype=3;
		int ws = write(fd3,&h,sizeof(h));
		if(ws!=sizeof(h)){
			err(6,"Error writing in output file!");
		}
	}
	else{
		header h3;
		int rs3 = read(fd3,&h3, sizeof(h3));
		if(rs3!= sizeof(h3)){
			err(3,"Error in reading from %s", argv[3]);
		}
		if(h3.magic != 0x5A4D || h1.filetype != 3){
			errx(4,"Invalid type or magic key!");
		}
	}

	list list_element;
	uint32_t data_element;
	int read_list;

	while((read_list = read(fd1,&list_element,sizeof(list_element)))==sizeof(list_element)){
		if(lseek(fd2,list_element.offset_data,SEEK_SET)==-1){
				err(6,"Error in lseeking");
		}
		int read_data = read(fd2,&data_element,sizeof(data_element));
		if(read_data == -1){
			err(3,"Can not read from %s", argv[2]);
		}

 		if(lseek(fd3,list_element.offset_out,SEEK_SET)==-1){
			err(7,"Error in lseeking");
		}
		if(write(fd3,&read_data,sizeof(read_data))!= sizeof(read_data)){
			err(6,"Can not write to %s", argv[3]);
		}

	}
	if(read_list == -1){
		err(3,"Can not read from %s", argv[1]);
	}
	
	close(fd1);
	close(fd2);
	close(fd3);
	exit(0);

}
