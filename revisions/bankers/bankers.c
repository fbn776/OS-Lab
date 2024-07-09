#include <stdio.h>

int isArrLess(int prim[20], int sec[20], int n) {
	int i = 0;
	for(i = 0; i < n; i++) {
		if(prim[i] > sec[i])
			return 0;
	}
	return 1;
}


void bankers(int n, int m, int Alloc[][20], int Req[][20], int Avail[]) {
	int Work[20], Finish[20], i, j, k, isSafe = 1;

	// Work = Avail
	for(i = 0; i < m; i++)
		Work[i] = Avail[i];
	// Finish[i] = true for all i
	for(i = 0; i < n; i++)
		Finish[i] = 0;

	for(j = 0; j < n; j++) {
		for(i = 0; i < n; i++) {
			if(Finish[i] == 0 && isArrLess(Req[i], Work, m)) {
				for(k = 0; k < m; k++) {
					Work[k] += Alloc[i][k];
				}

				Finish[i] = 1;
			}
		}
	}

	for(i = 0; i < n; i++) {
		if(!Finish[i]) {
			isSafe = 0;
			break;
		}
	}

	if(isSafe)
		printf("The seq is safe\n");
	else
		printf("The seq is not safe\n");
}

int main() {
	int n, m, i, j;
	int Alloc[20][20], Req[20][20], Avail[20];

	printf("Enter no of process: ");
	scanf(" %d", &n);

	printf("Enter no of resources: ");
	scanf(" %d", &m);

	printf("Enter allocation: ");
	for(i = 0; i < n; i++) {
		for(j = 0; j < m; j++) {
			scanf(" %d", &Alloc[i][j]);
		}
	}
	printf("Enter req: ");
	for(i = 0; i < n; i++) {
		for(j = 0; j < m; j++) {
			scanf(" %d", &Req[i][j]);
		}
	}

	printf("Avail: ");
	for(i = 0; i < n; i++) {
		scanf(" %d", &Avail[i]);
	}

	bankers(n, m, Alloc, Req, Avail);

	return 0;
}
