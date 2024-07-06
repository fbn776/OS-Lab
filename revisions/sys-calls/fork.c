#include <stdio.h>
#include <unistd.h>

int main() {

	printf("This is from the parent process\n");

	printf("Fork starting..\n");
	int pid = fork();

	if(pid == -1) {
		perror("Forking failed\n");
		return 1;
	}

	printf("Fork sucess\n");

	if(pid == 0) {
		printf("This is from child process with PID: %d\n", getpid());
	} else {
		printf("This is from the paretn process with PID: %d\n", getpid());
	}

	printf("END\n");
}
