#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define SHM_KEY 0x1235
#define SIZE 100

struct Message {
	char str[SIZE][SIZE];
	int f;
	int r;
};


void addMsg(struct Message *Q, char item[]) {
	if (Q->f == -1)
		Q->f = 0;
	(Q->r) = ((Q->r) + 1) % SIZE;
	strcpy(Q->str[Q->r], item);
}

char *getMsg(struct Message *Q) {
	char *item = Q->str[Q->f];
	Q->f = ((Q->f) + 1) % SIZE;

	if (Q->f == ((Q->r) + 1) % SIZE)
		(Q->f) = (Q->r) = -1;

	return item;
}


int main() {
	int shmid, shmDtCode;
	struct Message
			*mailbox;

	shmid = shmget(SHM_KEY, sizeof(struct Message), 0666 | IPC_CREAT);

	if (shmid == -1) {
		perror("shmget");
		exit(1);
	}

	mailbox = (struct Message*) shmat(shmid, NULL, 0666);

	if (mailbox == (void *) -1) {
		perror("Shared memory attach");
		return 1;
	}

	while(1) {
		char str[SIZE];

		while(mailbox->f == -1);

		strcpy(str, getMsg(mailbox));
		printf("Message: %s\n", str);

		if(strcmp(str, "exit") == 0) {
			break;
		}
	}

	shmDtCode = shmdt(mailbox);

	if (shmDtCode == -1) {
		perror("shmdt");
		exit(1);
	}
}