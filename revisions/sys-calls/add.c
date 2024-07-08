#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	printf("Argc  %d\n", argc);

	if(argc != 3) {
		printf("Not enough args\n");
		return 1;
	}

	int a = atoi(argv[1]),
		b = atoi(argv[2]);

	printf("%d + %d = %d\n", a, b, a + b);

	return 0;
}
