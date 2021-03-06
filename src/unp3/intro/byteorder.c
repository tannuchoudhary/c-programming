#include <stdio.h>
#include <stdlib.h>


int
main(int argc, const char *argv[])
{
	union {
		short s;
		char c[sizeof(short)];
	} un;

	un.s = 0x0102;

#ifdef _GNU_SOURCE
	printf("%s: ", CPU_VENDOR_OS);
#endif

	if (sizeof(short) == 2)
	{
		if (un.c[0] == 1 && un.c[1] == 2)
			printf("big-endian\n");
		else if (un.c[0] == 2 && un.c[1] == 1)
			printf("little-endian\n");
		else
			printf("unknown\n");
	}
	else
		printf("sizeof(short) = %ld\n", sizeof(short));

	exit(0);
}