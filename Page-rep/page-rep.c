#include<stdio.h>

typedef struct {
	int n;
	int last;
	int freq;
} Frame;

void display(Frame pages[30], int n) {
	int i;
	printf("Frames\n");
	for (i = 0; i < n; i++)
		printf("%d ", pages[i].n);

	printf("\n");
}

void FIFS(Frame *Frames, int size, int Pages[30], int n) {
	int i, j;
	printf("FIFS\n");
	int pageFault = 0, index = -1;
	for (i = 0; i < size; i++)
		(Frames + i)->n = -1;

	display(Frames, size);

	for (i = 0; i < n; i++) {
		int pageFound = 0;
		for (j = 0; j < size; j++) {
			if ((Frames + j)->n == Pages[i]) {
				pageFound = 1;
			}
		}
		if (!pageFound) {
			pageFault++;
			index = (index + 1) % size;
			(Frames + index)->n = Pages[i];
		}
		display(Frames, size);
	}
	printf("No of page faults = %d\n", pageFault);
	printf("PageFault Ratio = %.2f\n", ((float) pageFault / n));
	printf("No of page hits = %3d\n", n - pageFault);
	printf("Page hit ratio = %.2f\n", ((float) (n - pageFault) / n));
}

void LRU(Frame *Frames, int Frame_Size, int Pages[30], int n) {
	int i, j;
	printf("\n\t\t LRU\n");
	int pageFault = 0;
	for (i = 0; i < Frame_Size; i++) {
		(Frames + i)->n = -1;
		(Frames + i)->last = 0;
	}
	display(Frames, Frame_Size);
	int z = 1;
	for (i = 0; i < n; i++) {
		int pageFound = 0;
		for (j = 0; j < Frame_Size; j++) {
			if ((Frames + j)->n == Pages[i]) {
				pageFound = 1;
				(Frames + j)->last = z;
				z++;
				break;
			}
		}
		if (!pageFound) {
			pageFault++;
			int min = 0;
			for (j = 1; j < Frame_Size; j++) {
				if ((Frames + j)->last < (Frames + min)->last) {
					min = j;
				}
			}
			(Frames + min)->n = Pages[i];
			(Frames + min)->last = z;
			z++;
		}
		display(Frames, Frame_Size);
	}
	printf("\nNo of PageFaults =%3d ", pageFault);
	printf("\t PageFault Ratio = %.2f", ((float) pageFault / n));
	printf("\nNo of PageHits =%3d ", n - pageFault);
	printf("\t PageHit Ratio = %.2f\n\n", ((float) (n - pageFault) / n));

}

void LFU(Frame *Frames, int Frame_Size, int Pages[30], int n) {
	int i, j;
	printf("\n\t\t LFU\n");
	int pageFault = 0;
	for (i = 0; i < Frame_Size; i++) {
		(Frames + i)->n = -1;
		(Frames + i)->freq = 0;
	}
	display(Frames, Frame_Size);
	int index = 0;
	for (i = 0; i < n; i++) {
		int pageFound = 0;
		for (j = 0; j < Frame_Size; j++) {
			if ((Frames + j)->n == Pages[i]) {
				pageFound = 1;
				(Frames + j)->freq += 1;
				break;
			}
		}
		if (!pageFound) {
			pageFault++;
			int min = index;
			for (j = 0; j < Frame_Size; j++) {
				if ((Frames + index)->freq < (Frames + min)->freq) {
					min = index;
				}
				index = (index + 1) % Frame_Size;
			}
			(Frames + min)->n = Pages[i];
			(Frames + min)->freq = 1;
			index = (min + 1) % Frame_Size;
		}
		display(Frames, Frame_Size);
	}
	printf("\nNo of PageFaults =%3d ", pageFault);
	printf("\t PageFault Ratio = %.2f", ((float) pageFault / n));
	printf("\nNo of PageHits =%3d ", n - pageFault);
	printf("\t PageHit Ratio = %.2f\n\n", ((float) (n - pageFault) / n));

}

int main() {
	int Frame_Size, n, i;
	printf("Enter the size of Frame: ");
	scanf("%d", &Frame_Size);
	Frame Frames[Frame_Size];
	printf("Enter the Number of pages: ");
	scanf("%d", &n);
	int Pages[n];
	printf("Enter the Page Numbers: ");
	for (i = 0; i < n; i++)
		scanf("%d", &Pages[i]);

	printf("\nAvailable Pages are: ");
	for (i = 0; i < n; i++)
		printf("%3d", Pages[i]);

	FIFS(Frames, Frame_Size, Pages, n);
	LRU(Frames, Frame_Size, Pages, n);
	LFU(Frames, Frame_Size, Pages, n);
	return 0;
}