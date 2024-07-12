#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <stdio.h>

int main (int argc, char* argv[]){
	if(argc != 2){
		errx(1,"err");
	}
	pid_t pid = fork();
	if(pid < 0){
		errx(2,"err forking");
	}
	if(pid==0){
		if(execlp(argv[1], argv[1],NULL) == -1){
			err(2, "err execling");
		}
	}
	pid_t wait_code = waitpid(pid,NULL,0);
	
	if (wait_code == -1) {
		err(1, "Could not wait for child.");
	}

	if (!WIFEXITED(child_status)) {
		err(1, "Child did not terminate normally.");
	}

	if (WEXITSTATUS(child_status) != 0) {
		err(1, "Child exit code not 0, file should be already closed.");
	}
	else{
		printf("%s\n", argv[1]);
	}
	exit(0);
}
