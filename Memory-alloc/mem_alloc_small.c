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

	printf("%s\n", title);
	printf("|  i | tot | occ | frg |\n");
	for (i = 0; i < mem.noOfBlocks; i++) {
		int curr_frag = mem.blocks[i].size - mem.blocks[i].occupied;
		printf("| %2d | %3d | %3d | %3d |\n", i, mem.blocks[i].size, mem.blocks[i].occupied, curr_frag);
		t_frag += curr_frag;
		t_occ += mem.blocks[i].occupied;
	}

	printf("| %2s | %3d | %3d | %3d |\n", "t", mem.tSize, t_occ, t_frag);
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

			if (curr.occupied > 0) {
				j++;
				continue;
			}

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

void dynamic_fit(Memory inp_mem, int input[], int n, char *title, int isLess) {
	Memory mem;
	mem_cpy(inp_mem, &mem);

	int unused[SIZE], unSize = 0, i = 0, j;

	while (i < n) {
		int possible_index[SIZE], size = 0;
		j = 0;

		while (j < mem.noOfBlocks) {
			Block curr = mem.blocks[j];

			if (curr.occupied > 0) {
				j++;
				continue;
			}

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

				if (isLess ? mem.blocks[index].size < mem.blocks[min].size : mem.blocks[index].size > mem.blocks[min].size)
					min = index;
			}
			mem.blocks[min].occupied = input[i];
		}
		i++;
	}

	displayMem(mem, title);

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
	dynamic_fit(mem, input, n, "Best", 1);
	dynamic_fit(mem, input, n, "Worst", 0);
}