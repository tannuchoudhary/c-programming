/*
extern: extern simply tells us that the variable is defined elsewhere and not within the same block
where it is used. Basically, the value is assigned to it in a different block and this can be overwritten/changed
in a different block as well.

The main purpose of using extern variables is that they can be accessed between different files which are part
of a large program.
*/

#include <stdio.h>

extern void
print_hello(char *);


void
print_hello(char *s)
{
    printf("hello %s\n", s);
}

int
main(void)
{
    print_hello("world");
    return 0;
}

/*
If "void print_hello" in multi c files, gcc gains an error as follow:

    /tmp/ccZuMDlf.o: In function `print_hello':
    extern_func.c:(.text+0x0): multiple definition of `print_hello'
    /tmp/ccpXQPU1.o:extern_main.c:(.text+0x0): first defined here

*/