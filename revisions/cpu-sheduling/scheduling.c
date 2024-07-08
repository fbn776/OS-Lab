#include <stdio.h>

typedef struct {
	int pid; // Process ID
	int at; // Arrvial time
	int bt; // Burst time
	int prty; // Priority
	int ct; // Complete time
	int rt; // Remaining time
} Process;

#define SIZE 100

typedef struct {
	Process Q[SIZE];
	int f;
	int r;
} Queue;


void copyArr(Process src[], int n, Process dest[]) {
	int i;
	for(i = 0; i < n; i++)
		dest[i] = src[i];
}

void initQueue(Process src[], int n, Queue *procs) {
	copyArr(src, n, procs->Q);
	procs->f = 0;
	procs->r = n - 1;
}

void Enqueue(Queue *procs, Process item) {
	if ((procs->f == procs->r + 1) || (procs->f == 0 && procs->r == SIZE - 1))
		printf("\n Queue is full!! \n");
	else {
		if (procs->f == -1)
			procs->f = 0;
		procs->r = (procs->r + 1) % SIZE;
		procs->Q[procs->r] = item;
 	 }
}

Process Dequeue(Queue *procs) {
 	Process temp;
  	if (procs->f == -1) {
    	printf("\nERROR Dequeuing from an empty queue\n");
		return temp;
  	} else {
    	temp = procs->Q[procs->f];
   		if (procs->f == procs->r)
   			procs->f = procs->r = -1;
   		else
   			procs->f = (procs->f + 1) % SIZE;
   		return temp;
	}
}


void readProcesses(Process ps[], int *n) {
	printf("No of processors: ");
	scanf(" %d", n);

	printf("Enter Arrival time, Burst time, Priority: \n");

	int i;
	for(i = 0; i < *n; i++) {
		printf("%d -> ", i);
		Process temp;
		temp.pid = i;
		scanf(" %d %d %d", &temp.at, &temp.bt, &temp.prty);
		ps[i] = temp;
	}
}

void displayProcess(Process procs[], int n, const char *title) {
	int i;
	printf("\n%5s\n", title);
	printf("--------------------\n");
	printf(" PID |  AT  |  BT  | Prty |  CT  |  TAT |  WT  \n");
	printf("-----+------+------+------+------+------+------\n");
	for(i = 0; i < n; i++) {
		Process temp = procs[i];
		int tat = temp.ct - temp.at;
		printf("%4d |%5d |%5d |%5d |%5d |%5d |%5d \n", temp.pid, temp.at, temp.bt, temp.prty, temp.ct, tat, tat - temp.bt);
	}
}

void sortByAt(Process procs[], int n) {
	int i, j;
	for(i = 0; i < n - 1; i++) {
		for(j = 0; j < n - i - 1; j++) {
			if(procs[j].at > procs[j + 1].at) {
				Process temp = procs[j];
				procs[j] = procs[j + 1];
				procs[j + 1] = temp;
			}
		}
	}
}

void FCFS(Process procs[], int n) {
	Queue jobQ;
	Process result[SIZE];
	int k = 0, t = 0;

	initQueue(procs, n, &jobQ);
	sortByAt(jobQ.Q, n);

	while(jobQ.f != -1) {
		Process item = Dequeue(&jobQ);
		t += item.bt;
		item.ct = t;
		result[k++] = item;
	}

	displayProcess(result, n, "FCFS");
}

void swapProcs(Process procs[], int i, int j) {
	Process temp = procs[i];
	procs[i] = procs[j];
	procs[j] = temp;
}


void SJF(Process procs[], int n) {
	Queue jobQ, readyQ;
	Process result[SIZE];
	int k = 0, t = 0, i;

	initQueue(procs, n, &jobQ);
	readyQ.f = readyQ.r = -1;

	while(k != n) {
		// Take all the process that have come. ie their arrival time is <= total elapsed time;
		if(jobQ.f != -1) {
			for(i = jobQ.f; i <= jobQ.r; i++) {
				if(jobQ.Q[i].at <= t) {
					swapProcs(jobQ.Q, i, jobQ.f);

					Enqueue(&readyQ, Dequeue(&jobQ));
				}
			}
		}

		int min_bt_index = readyQ.f;
		for(i = readyQ.f; i <= readyQ.r; i++) {
			if(readyQ.Q[i].bt < readyQ.Q[min_bt_index].bt) {
				min_bt_index = i;
			}
		}

		swapProcs(readyQ.Q, min_bt_index, readyQ.f);
		Process temp = Dequeue(&readyQ);
		t += temp.bt;
		temp.ct = t;
		result[k++] = temp;
	}
	sortByAt(result, n);
	displayProcess(result, n, "SJF");
}

void RR_FCFS(Process procs[], int n) {
	Queue jobQ;
	Process result[SIZE];

	initQueue(procs, n, &jobQ);
}

int main() {
	int n;
	Process procs[SIZE];

	readProcesses(procs, &n);
	displayProcess(procs, n, "Initial");

	FCFS(procs, n);
	SJF(procs, n);

	return 0;
}
