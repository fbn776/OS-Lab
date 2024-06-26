#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <pthread.h>

#define SHM_KEY 0x1238
#define SIZE 100

typedef struct {
	char str[SIZE];
	char from_user[SIZE];
} Message;

typedef struct {
	Message msgs[SIZE];
	int f;
	int r;
} MailBox;

void writeMsg(MailBox *Q, Message item) {
	if (Q->f == -1)
		Q->f = 0;
	(Q->r) = ((Q->r) + 1) % SIZE;
	Q->msgs[Q->f] = item;
}

Message readMsg(MailBox *Q) {
	Message item = Q->msgs[Q->f];
	Q->f = ((Q->f) + 1) % SIZE;

	if (Q->f == ((Q->r) + 1) % SIZE)
		(Q->f) = (Q->r) = -1;

	return item;
}

MailBox *mailbox;
char user_name[SIZE];

void *message_receiver(void *params) {
	while (1) {
		char str[SIZE];
		Message message;

		while(mailbox->f == -1) ;


		if(strcmp(mailbox->msgs[mailbox->f].from_user, user_name) == 0)
			continue;

		message = readMsg(mailbox);

		strcpy(str, message.str);
		printf("\nMessage: %s - from: %s\n", str, message.from_user);
		if (strcmp(str, "exit") == 0)
			pthread_exit(NULL);
	}
}


int main() {
	int shmid, shmDtCode, thd_status;
	pthread_t receiver_thd;

	shmid = shmget(SHM_KEY, sizeof(MailBox), 0666 | IPC_CREAT);

	if (shmid == -1) {
		perror("shmget");
		exit(1);
	}

	mailbox = (MailBox *) shmat(shmid, NULL, 0666);

	if (mailbox == (void *) -1) {
		perror("Shared memory attach");
		exit(1);
	}

	printf("Enter username: ");
	scanf(" %[^\n]", user_name);

	// Parent fork (for sending the message)
	mailbox->f = -1;
	mailbox->r = -1;

	thd_status = pthread_create(&receiver_thd, NULL, message_receiver, (void *) NULL);

	if(thd_status != 0) {
		perror("Thread");
		exit(1);
	}

	while (1) {
		char str[SIZE];
		Message message;

		//If messages are full, wait
		while (mailbox->f == ((mailbox->r + 1) % SIZE));

		printf("Enter message: ");
		scanf(" %[^\n]", str);

		strcpy(message.str, str);
		strcpy(message.from_user, user_name);

		writeMsg(mailbox, message);
		if (strcmp(str, "exit") == 0) {
			printf("Exiting...");
			break;
		}
	}

	thd_status = pthread_join(receiver_thd, NULL);
	if (thd_status != 0)
		perror("thread join");

	shmDtCode = shmdt(mailbox);

	if (shmDtCode == -1) {
		perror("shmdt");
		exit(1);
	}
}