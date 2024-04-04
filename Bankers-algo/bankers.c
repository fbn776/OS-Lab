#include <stdio.h>
#define SIZE 100

int isArrayLess(int A[], int B[], int n) {
	int i;
	for(i = 0; i < n; i++)
		if(A[i] > B[i])
			return 0;

	return 1;
}

int main() {
	int Seq[SIZE];
	int i, j, k, index = 0, isSafe = 1;
	// n -> No of a process; m -> No of type of process
	int n, m;
	// size -> 0 to m-1
	int Avail[SIZE], Work[SIZE];
	// size -> 0 to n-1
	int Finish[SIZE];
	// size -> n by m
	int Max[SIZE][SIZE], Alloc[SIZE][SIZE], Need[SIZE][SIZE];

	printf("Enter No of process: ");
	scanf(" %d", &n);

	printf("Enter No of type of resources: ");
	scanf(" %d", &m);

	// Read Available resources
	printf("Enter available: ");
	for(i = 0; i < m; i++) {
		scanf(" %d", Avail + i);
		Work[i] = Avail[i];
	}

	// Read Max demand of a process
	printf("Max demand");
	for(i = 0; i < n; i++) {
		printf("P%d: ", i);
		for (j = 0; j < m; j++) {
			scanf(" %d", &Max[i][j]);
		}
	}

	// Read current allocation
	printf("Currently allocated:\n");
	for(i = 0; i < n; i++) {
		printf("P%d: ", i);
		for (j = 0; j < m; j++) {
			scanf(" %d", &Alloc[i][j]);

			// Find out how much more the process needs
			Need[i][j] = Max[i][j] - Alloc[i][j];
		}
	}
	// Initially set to all 0
	for (i = 0; i < n; i++)
		Finish[i] = 0;

	for(k = 0; k < m; k++)
		for (i = 0; i < n; i++)
			if (!Finish[i] && isArrayLess(Need[i], Work, m)) {
				Seq[index++] = i;

				for (j = 0; j < m; j++)
					Work[j] += Alloc[i][j];
				Finish[i] = 1;
			}

	for(i = 0; i < n; i++)
		if(!Finish[i]) {
			isSafe = 0;
			break;
		}

	printf("\n\nThe process is %s\n", isSafe ? "Safe" : "Not Safe");


	printf("Sequence: ");
	for(i = 0; i < index; i++) {
		printf("P%d ", Seq[i]);
	}
}