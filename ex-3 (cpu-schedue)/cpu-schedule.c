#include <stdio.h>
#include <string.h>

#define SIZE 100

typedef struct {
	int PID;
	int arrival_t;
	int burst_t;
	int priority;
} Process;

typedef struct {
	int PID;
	int arrival_t;
	int burst_t;
	int response_t;
	int completion_t;
	int turn_around_t;
	int waiting_time;
} ProcessTime;

typedef int (*Compare) (Process p1, Process p2);
typedef int (*GetProp) (ProcessTime p);

int getResp(ProcessTime pt) {
	return pt.response_t;
}

int getTurnTime(ProcessTime pt) {
	return pt.turn_around_t;
}

int getWaitingTime(ProcessTime pt) {
	return pt.waiting_time;
}

int cmpByBurstTime(Process p1, Process p2) {
	return p1.burst_t > p2.burst_t;
}

void sort(Process p[], int n, Compare cmpr) {
	int i, j;
	Process temp;

	for (i = 0; i < n - 1; i++)
		for (j = 0; j < n - i - 1; j++)
			if (cmpr(p[j], p[j + 1])) {
				temp = p[j];
				p[j] = p[j + 1];
				p[j + 1] = temp;
			}
}

void sortTable(ProcessTime p[], int n) {
	int i, j;
	ProcessTime temp;
	for (i = 0; i < n - 1; i++)
		for (j = 0; j < n - i - 1; j++)
			if (p[j].PID > p[j + 1].PID) {
				temp = p[j];
				p[j] = p[j + 1];
				p[j + 1] = temp;
			}
}

void cpyArr(Process *src, int n, Process *dest) {
	int i = 0;
	for(i = 0; i < n; i++)
		dest[i] = src[i];
}


float getAvg(ProcessTime pt[], int n, GetProp getter) {
	int sum = 0, i = 0;
	for(i = 0; i < n; i++)
		sum += getter(pt[i]);

	return ((float) sum) / (float) n;
}

/** Queue **/
void Enqueue(Process Q[], int *front, int *rear, Process item) {
	if (*front == ((*rear) + 1) % SIZE)
		printf("Queue is full\n");
	else {
		if (*front == -1)
			*front = 0;
		(*rear) = ((*rear) + 1) % SIZE;
		Q[*rear] = item;
	}
}

Process Dequeue(Process Q[], int *front, int *rear) {
	Process item = Q[*front];
	*front = ((*front) + 1) % SIZE;

	if (*front == ((*rear) + 1) % SIZE)
		*front = (*rear) = -1;

	return item;
}

int isQueueEmpty(int front, int rear) {
	return front == -1;
}

void makeQueue(Process P[], int n, Process Q[], int *f, int *r) {
	int i = 0;

	for(i = 0; i < n; i++) {
		Enqueue(Q, f, r, P[i]);
	}
}


/** Main code starts here **/
void readProcess(Process arr[], int *n) {
	int i;
	printf("Enter n: ");
	scanf(" %d", n);

	printf("Enter arrival time, burst time, priority:\n");
	for (i = 0; i < *n; i++) {
		Process p;
		printf("%d > ", i + 1);
		scanf(" %d %d %d", &(p.arrival_t), &(p.burst_t), &(p.priority));
		p.PID = i;
		arr[i] = p;
	}
}

void drawLine(int n, char line, char ends, int eol) {
	int i;

	printf("%c", ends);
	for(i = 0; i < n - 2; i++)
		printf("%c", line);
	printf("%c", ends);

	if(eol)
		printf("\n");
}
void displayProcess(Process p[], int n, const char *title) {
	int i;
	int lineLen = 48;
	int titleLen = strlen(title);

	drawLine((lineLen - titleLen) / 2, ' ', ' ', 0);
	printf("%s", title);
	drawLine((lineLen - titleLen) / 2, ' ', ' ', 1);
	drawLine(lineLen, '-', '+', 1);

	printf("| %5s | %10s | %10s | %10s |\n", "PID", "Arrival", "Burst", "Priority");
	drawLine(lineLen, '-', '|', 1);
	for (i = 0; i < n; i++)
		printf("| %5d | %10d | %10d | %10d |\n", p[i].PID, p[i].arrival_t, p[i].burst_t, p[i].priority);
	drawLine(lineLen, '-', '+', 1);
}

void displayCalcTable(ProcessTime pt[], int n, const char *title) {
	int i;
	int lineLen = 87;
	int titleLen = strlen(title);

	printf("\n");
	drawLine((lineLen - titleLen) / 2, ' ', ' ', 0);
	printf("%s", title);
	drawLine((lineLen - titleLen) / 2, ' ', ' ', 1);
	drawLine(lineLen, '-', '+', 1);

	printf("| %5s | %10s | %10s | %10s | %10s | %10s | %10s |\n", "PID", "Arrival", "Burst", "Response", "Completion", "Turn time", "Waiting");
	drawLine(lineLen, '-', '|', 1);
	for (i = 0; i < n; i++)
		printf("| %5d | %10d | %10d | %10d | %10d | %10d | %10d |\n",
			   pt[i].PID, pt[i].arrival_t, pt[i].burst_t, pt[i].response_t, pt[i].completion_t, pt[i].turn_around_t, pt[i].waiting_time);

	drawLine(lineLen, '-', '+', 1);
}


void calcTable(Process *Q, int n, const char *title) {
	int f = 0, r = n - 1, i = 0;
	ProcessTime table[100];


	int completion_t = Q[0].arrival_t;
	int prev_completion_t = Q[0].arrival_t;

	while(!isQueueEmpty(f, r)) {
		Process item = Dequeue(Q, &f, &r);
		ProcessTime row;
		completion_t += item.burst_t;

		row.PID = item.PID;
		row.burst_t = item.burst_t;
		row.arrival_t = item.arrival_t;
		row.completion_t = completion_t;
		row.turn_around_t = row.completion_t - row.arrival_t;
		row.waiting_time = row.turn_around_t - row.burst_t;
		row.response_t = prev_completion_t - row.arrival_t;

		prev_completion_t += item.burst_t;
		table[i++] = row;
	}

	sortTable(table, i);
	displayCalcTable(table, i, title);
	float respAvg = getAvg(table, i, getResp),
			turnAvg = getAvg(table, i, getTurnTime),
			waitingAvg = getAvg(table, i, getWaitingTime);

	printf("Average response time    : %.2f\n", respAvg);
	printf("Average turn around time : %.2f\n", turnAvg);
	printf("Average waiting time     : %.2f\n", waitingAvg);
}


void fcfs(Process p[], int n) {
	Process Q[100];
	cpyArr(p, n, Q);

	calcTable(Q, n, "FCFS");

}

void sjf(Process p[], int n) {
	Process Q[100];
	cpyArr(p, n, Q);

	sort(Q, n, cmpByBurstTime);
	displayProcess(Q, n, "Shortest");

	calcTable(Q, n, "SJF");
}


int main() {
	Process process_arr[100];
	int n;

	readProcess(process_arr, &n);
	displayProcess(process_arr, n, "Initial");

	fcfs(process_arr, n);
	sjf(process_arr, n);
}