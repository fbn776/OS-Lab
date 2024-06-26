#include <stdio.h>
#include <string.h>
#define SIZE 100

typedef struct {
	int PID;
	int arrival_t;
	int burst_t;
	int priority;
	int completion_t;
	int rem_time;
	int turn_around_t;
	int waiting_t;
} Process;

// Function pointers
typedef int (*GetProp)(Process p);
typedef Process (*GetNextProcess)(Process p[], int *f, int *r);

void swapInQueue(Process Q[], int i, int j) {
	Process temp = Q[i];
	Q[i] = Q[j];
	Q[j] = temp;
}

void sort(Process p[], int n) {
	int i, j;

	for (i = 0; i < n - 1; i++)
		for (j = 0; j < n - i - 1; j++)
			if (p[j].arrival_t > p[j + 1].arrival_t)
				swapInQueue(p, j, j + 1);
}

void cpyArr(Process *src, int n, Process *dest) {
	int i;
	for (i = 0; i < n; i++) {
		dest[i] = src[i];
		dest[i].rem_time = src[i].burst_t;
	}
}

int getBurstTime(Process p) {
	return p.burst_t;
}

int getRemTime(Process p) {
	return p.rem_time;
}

int getTurnTime(Process pt) {
	return pt.turn_around_t;
}

int getWaitingTime(Process pt) {
	return pt.waiting_t;
}

float getAvg(Process pt[], int n, GetProp getter) {
	int sum = 0, i = 0;
	for (i = 0; i < n; i++)
		sum += getter(pt[i]);

	return ((float) sum) / (float) n;
}

void drawLine(int n, char line, char ends, int eol) {
	int i;

	printf("%c", ends);
	for (i = 0; i < n - 2; i++)
		printf("%c", line);
	printf("%c", ends);

	if (eol)
		printf("\n");
}

void displayAsTable(Process pt[], int n, const char *title, int displayFull) {
	int i;
	int lineLen = displayFull ? 74 : 48;
	int titleLen = strlen(title);

	printf("\n");
	drawLine((lineLen - titleLen) / 2, ' ', ' ', 0);
	printf("%s", title);
	drawLine((lineLen - titleLen) / 2, ' ', ' ', 1);
	drawLine(lineLen, '-', '+', 1);

	displayFull ?
	printf("| %5s | %10s | %10s | %10s | %10s | %10s |\n",
		   "PID", "Arrival", "Burst", "Completion", "Turn time", "Waiting") :
	printf("| %5s | %10s | %10s | %10s |\n",
		   "PID", "Arrival", "Burst", "Priority");

	drawLine(lineLen, '-', '|', 1);
	for (i = 0; i < n; i++)
		displayFull ?
		printf("| %5d | %10d | %10d | %10d | %10d | %10d |\n",
			   pt[i].PID, pt[i].arrival_t, pt[i].burst_t, pt[i].completion_t, pt[i].turn_around_t, pt[i].waiting_t) :
		printf("| %5d | %10d | %10d | %10d |\n", pt[i].PID, pt[i].arrival_t, pt[i].burst_t, pt[i].priority);
	drawLine(lineLen, '-', '+', 1);
}

void showResult(Process *table, int n, const char *title) {
	sort(table, n);
	displayAsTable(table, n, title, 1);
	float turnAvg = getAvg(table, n, getTurnTime),
			waitingAvg = getAvg(table, n, getWaitingTime);

	printf("Average turn around time : %.2f\n", turnAvg);
	printf("Average waiting time     : %.2f\n", waitingAvg);
}

/** Queue **/
void Enqueue(Process Q[], int *front, int *rear, Process item) {
	if (*front == -1)
		*front = 0;
	(*rear) = ((*rear) + 1) % SIZE;
	Q[*rear] = item;
}

Process Dequeue(Process Q[], int *front, int *rear) {
	Process item = Q[*front];
	*front = ((*front) + 1) % SIZE;

	if (*front == ((*rear) + 1) % SIZE)
		(*front) = (*rear) = -1;

	return item;
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
		p.PID = i + 1;
		arr[i] = p;
	}
}

Process ShortestBTDequeue(Process Q[], int *f, int *r, GetProp func) {
	int i, min = 99999, pos, AT = 0;
	for (i = *f; i <= *r; i++)
		if (func(Q[i]) < min || func(Q[i]) == min && Q[i].arrival_t < AT) {
			min = func(Q[i]);
			pos = i;
			AT = Q[i].arrival_t;
		}

	swapInQueue(Q, *f, pos);
	return Dequeue(Q, f, r);
}

Process getShortestBT(Process Q[], int *f, int *r) {
	return ShortestBTDequeue(Q, f, r, getBurstTime);
}

Process PriorityDequeue(Process Q[], int *f, int *r) {
	int i, max = -9999, pos, AT = 0;
	for (i = *f; i <= *r; i++)
		if (Q[i].priority > max || Q[i].priority == max && Q[i].arrival_t < AT) {
			max = Q[i].priority;
			pos = i;
			AT = Q[i].arrival_t;
		}

	swapInQueue(Q, *f, pos);

	return Dequeue(Q, f, r);
}

void nonPreemptiveScheduling(const char *title, Process p[], int n, GetNextProcess nextProcess) {
	Process table[SIZE];

	int t, i, completion_t, tn = 0;

	Process Q[SIZE], JobQ[SIZE];
	int qf = 0, qr = n - 1,
			jf = -1, jr = -1;

	cpyArr(p, n, Q);

	t = completion_t = Q[qf].arrival_t;
	while (qf != -1) {
		for (i = 0; i <= t; i++)
			if (Q[qf].arrival_t <= t && qf != -1)
				Enqueue(JobQ, &jf, &jr, Dequeue(Q, &qf, &qr));

		while (jf != -1) {
			Process item = nextProcess(JobQ, &jf, &jr);
			Process row;
			completion_t += item.burst_t;
			row.PID = item.PID;
			row.burst_t = item.burst_t;
			row.arrival_t = item.arrival_t;
			row.completion_t = completion_t;
			row.turn_around_t = row.completion_t - row.arrival_t;
			row.waiting_t = row.turn_around_t - row.burst_t;
			t += item.burst_t;
			table[tn++] = row;
		}
		t++;
	}
	showResult(table, tn, title);
}

void roundRobinScheduling(Process p[], int n) {
	const int quantum = 2;
	Process table[SIZE];
	int t = 0, tn = 0, j, cmpt_t = 0;

	int qf = 0, qr = n - 1,
			jf = -1, jr = -1;

	Process Q[SIZE], JobQ[SIZE];

	cpyArr(p, n, Q);

	while (qf != -1) {
		for (j = qf; j <= qr; j++)
			if (Q[j].arrival_t <= t && qf != -1) {
				swapInQueue(Q, qf, j);
				Enqueue(JobQ, &jf, &jr, Dequeue(Q, &qf, &qr));
			}

		if (jf != -1) {
			Process item = ShortestBTDequeue(JobQ, &jf, &jr, getRemTime);
			item.rem_time -= quantum;
			cmpt_t += quantum;
			if (item.rem_time <= 0) {
				// If item.rem_tim is less than time quantum, then adjust future cmpt time.
				cmpt_t = item.rem_time < 0 ? cmpt_t - item.rem_time : cmpt_t;
				Process row;
				row.PID = item.PID;
				row.burst_t = item.burst_t;
				row.arrival_t = item.arrival_t;
				row.completion_t = cmpt_t;
				row.turn_around_t = row.completion_t - row.arrival_t;
				row.waiting_t = row.turn_around_t - row.burst_t;
				table[tn++] = row;
			} else
				Enqueue(Q, &qf, &qr, item);

			while (jf != -1)
				Enqueue(Q, &qf, &qr, Dequeue(JobQ, &jf, &jr));
		}
		t += 1;
	}
	showResult(table, tn, "Round robin");
}

int main() {
	Process process_arr[100];
	int n;

	readProcess(process_arr, &n);
	displayAsTable(process_arr, n, "Input", 0);
	sort(process_arr, n);

	nonPreemptiveScheduling("FCFS", process_arr, n, Dequeue);
	nonPreemptiveScheduling("SJF", process_arr, n, getShortestBT);
	nonPreemptiveScheduling("Priority", process_arr, n, PriorityDequeue);
	roundRobinScheduling(process_arr, n);
}
