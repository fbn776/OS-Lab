#include <stdio.h>
#include <unistd.h>

int main() {
	int pid = fork();

	if(pid == -1) {
		perror("fork");
		return 1;
	}

	if(pid == 0) {
		printf("Hello from the child side | PID = %d\n", getpid());
		int ppid = execlp("./add", "add", "10", "20");
		/**
		ececlp replaces the calling process with the specified process.
		This wont replace the PID, just the process datas and stuff
		*/
		printf("This shouldn't print");
	} else{
		printf("Hello from the parent side | PID = %d\n", getpid());
	}

	return 0;
}
