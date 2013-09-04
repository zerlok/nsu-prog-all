#include "mod.h"

int check_fibo(int n) {
	if (n < 1) {
		printf ("%d < 1\n", n);
	}
	if (n > 47) {
		n = 47;
		printf("Can print only first %d fibo\n", n);
	}
	if (n > 0) {
		printf("0\n");
	}
	if (n > 1) {
		printf("1\n");
	}
	if (n > 2) {
		return n - 2;
	} else return 0;
}

void print_fibo(int n) {
	int a = 0, b = 1, x = a + b;
	n = check_fibo(n);
	while (n > 0) {
		printf("%d\n", x);
		a = b;
		b = x;
		x = a + b;
		n--;
	}
}
