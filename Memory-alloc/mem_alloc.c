#include <stdio.h>

#define SIZE 100

typedef struct {
	int size;
	int occupied;
} Block;

typedef struct {
	Block blocks[SIZE];
	int tSize;
	int noOfBlocks;
} Memory;

void drawLine(int n, char line, char ends, int eol) {
	int i;

	printf("%c", ends);
	for (i = 0; i < n - 2; i++)
		printf("%c", line);
	printf("%c", ends);

	if (eol)
		printf("\n");
}

void readMemory(Memory *mem) {
	int i;

	printf("Enter no of blocks: ");
	scanf(" %d", &(mem->noOfBlocks));

	mem->tSize = 0;

	printf("Enter block sizes: ");
	for (i = 0; i < mem->noOfBlocks; i++) {
		scanf(" %d", &(mem->blocks[i].size));
		mem->blocks[i].occupied = 0;
		mem->tSize += mem->blocks[i].size;
	}
}

void displayMem(Memory mem, char title[]) {
	int t_frag = 0;
	int t_occ = 0;

	int i;
	printf("\n");
	drawLine(24, '-', '+', 1);
	printf("|  %10s          |\n", title);
	drawLine(24, '-', '+', 1);
	printf("|  i | tot | occ | frg |\n");
	drawLine(24, '=', '+', 1);
	for (i = 0; i < mem.noOfBlocks; i++) {
		int curr_frag = mem.blocks[i].size - mem.blocks[i].occupied;
		printf("| %2d | %3d | %3d | %3d |\n", i, mem.blocks[i].size, mem.blocks[i].occupied, curr_frag);
		t_frag += curr_frag;
		t_occ += mem.blocks[i].occupied;
		drawLine(24, '-', '+', 1);
	}

	printf("| %2s | %3d | %3d | %3d |\n", "t", mem.tSize, t_occ, t_frag);
	drawLine(24, '-', '+', 1);

	printf("\n");
}

void mem_cpy(Memory src, Memory *dest) {
	int i;
	dest->tSize = src.tSize;
	dest->noOfBlocks = src.noOfBlocks;

	for (i = 0; i < src.noOfBlocks; i++) {
		dest->blocks[i] = src.blocks[i];
	}
}

void first_fit(Memory inp_mem, int input[], int n) {
	Memory mem;
	mem_cpy(inp_mem, &mem);

	int unused[SIZE], unSize = 0, i = 0;

	while (i < n) {
		int j = 0;
		int isUsed = 0;

		while (j < mem.noOfBlocks) {
			Block curr = mem.blocks[j];

			// Checks if a block is unoccupied
			if (curr.occupied > 0) {
				j++;
				continue;
			}

			// Checks if a block has enough space
			if (input[i] <= curr.size) {
				mem.blocks[j].occupied = input[i];
				isUsed = 1;
				break;
			}

			j++;
		}

		if (isUsed == 0)
			unused[unSize++] = input[i];

		i++;
	}

	displayMem(mem, "First");

	if (unSize > 0) {
		printf("There are unused blocks: ");
		for (i = 0; i < unSize; i++)
			printf("%d ", unused[i]);
	}
}

void best_fit(Memory inp_mem, int input[], int n) {
	Memory mem;
	mem_cpy(inp_mem, &mem);

	int unused[SIZE], unSize = 0, i = 0, j;

	while (i < n) {
		int possible_index[SIZE], size = 0;
		j = 0;

		while (j < mem.noOfBlocks) {
			Block curr = mem.blocks[j];

			// Checks if a block is occupied
			if (curr.occupied > 0) {
				j++;
				continue;
			}

			// Check if block has enough space
			if (input[i] <= curr.size)
				possible_index[size++] = j;

			j++;
		}

		if (size == 0) {
			unused[unSize++] = input[i];
		} else {
			int k, min = possible_index[0];
			for (k = 1; k < size; k++) {
				int index = possible_index[k];

				if (mem.blocks[index].size < mem.blocks[min].size)
					min = index;
			}
			mem.blocks[min].occupied = input[i];
		}
		i++;
	}

	displayMem(mem, "Best");

	if (unSize > 0) {
		printf("There are unused blocks: ");
		for (i = 0; i < unSize; i++)
			printf("%d ", unused[i]);
	}
}

void worst_fit(Memory inp_mem, int input[], int n) {
	Memory mem;
	mem_cpy(inp_mem, &mem);

	int unused[SIZE], unSize = 0, i = 0, j;

	while (i < n) {
		int possible_index[SIZE], size = 0;
		j = 0;

		while (j < mem.noOfBlocks) {
			Block curr = mem.blocks[j];

			// Checks if a block is occupied
			if (curr.occupied > 0) {
				j++;
				continue;
			}

			// Check if block has enough space
			if (input[i] <= curr.size)
				possible_index[size++] = j;

			j++;
		}

		if (size == 0) {
			unused[unSize++] = input[i];
		} else {
			int k, max = possible_index[0];
			for (k = 1; k < size; k++) {
				int index = possible_index[k];

				if (mem.blocks[index].size > mem.blocks[max].size)
					max = index;
			}
			mem.blocks[max].occupied = input[i];
		}
		i++;
	}

	displayMem(mem, "Worst");

	if (unSize > 0) {
		printf("There are unused blocks: ");
		for (i = 0; i < unSize; i++)
			printf("%d ", unused[i]);
	}
}

int main() {
	Memory mem;
	int i, n, input[SIZE];

	readMemory(&mem);
	displayMem(mem, "Initial");

	printf("Enter no of blocks: ");
	scanf(" %d", &n);
	printf("Enter block sizes to alloc: ");
	for (i = 0; i < n; i++)
		scanf(" %d", &input[i]);

	first_fit(mem, input, n);
	best_fit(mem, input, n);
	worst_fit(mem, input, n);
}