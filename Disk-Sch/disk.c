#include <stdio.h>
#include <stdlib.h>

int n, r[25], temp[25], init, size;

void fcfs() {
	int i, curr = init, seek = 0;
	for (i = 0; i < n; i++) {
		seek += abs(r[i] - curr);
		curr = r[i];
	}
	printf("FCFS seek time : %d\n", seek);
}

void scan() {
	int i, j, temp_v, curr = init, seek = 0, index;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n - i - 1; j++) {
			if (temp[j] > temp[j + 1]) {
				temp_v = temp[j];
				temp[j] = temp[j + 1];
				temp[j + 1] = temp_v;
			}
		}
	}
	for (i = 0; i < n; i++) {
		if (curr < temp[i]) {
			index = i;
			break;
		}
	}
	for (i = index; i < n; i++) {
		seek += abs(temp[i] - curr);
		curr = temp[i];
	}
	seek += abs(size - 1 - curr);
	curr = size - 1;
	for (i = index - 1; i >= 0; i--) {
		seek += abs(curr - temp[i]);
		curr = temp[i];
	}
	printf("SCAN seek time : %d\n", seek);
}

void cscan() {
	int i, j, temp_v, curr = init, seek = 0, index;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n - i - 1; j++) {
			if (temp[j] > temp[j + 1]) {
				temp_v = temp[j];
				temp[j] = temp[j + 1];
				temp[j + 1] = temp_v;
			}
		}
	}
	for (i = 0; i < n; i++) {
		if (curr < temp[i]) {
			index = i;
			break;
		}
	}
	for (i = index; i < n; i++) {
		seek += abs(temp[i] - curr);
		curr = temp[i];
	}
	seek += abs(size - 1 - curr);
	curr = size - 1;
	seek += curr;
	curr = 0;
	for (i = 0; i < index; i++) {
		seek += abs(temp[i] - curr);
		curr = temp[i];
	}
	printf("CSCAN seek time: %d\n", seek);
}


int main() {
	int i;
	printf("How many requests ? : ");
	scanf("%d", &n);
	printf("Enter the request sequence : ");

	for (i = 0; i < n; i++)
		scanf("%d", &r[i]);

	for (i = 0; i < n; i++)
		temp[i] = r[i];

	printf("Enter the total disk size : ");
	scanf("%d", &size);
	printf("Enter the init head position : ");
	scanf("%d", &init);
	fcfs();
	scan();
	cscan();
}