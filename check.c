#include <unistd.h>
#include <stdio.h>

int main()
{
	if (isatty(0))
		printf("0 is a tty.\n");
	else
		printf("0 is NOT a tty.\n");
	if (isatty(1))
		printf("1 is a tty.\n");
	else
		printf("1 is NOT a tty.\n");
	if (isatty(2))
		printf("2 is a tty.\n");
	else
		printf("2 is NOT a tty.\n");
}
