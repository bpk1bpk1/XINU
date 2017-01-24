#include <xinu.h>
#include <string.h>
#include <stdio.h>

shellcmd xsh_hello(int nargs, char *args[]) {
	if (nargs > 2) {
		printf("%s: Too many arguments\n", args[0]);
		return 1;
	}
	
	if (nargs < 2) {
		printf("Too few arguments \n");
	}

	if (nargs == 2) {
		printf("Hello %s, Welcome to the world of Xinu!!\n", args[1]);
	}
	
	return 0;
}
