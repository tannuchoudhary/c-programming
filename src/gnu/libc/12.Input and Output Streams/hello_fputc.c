#include <stdio.h>

// Function: int fputc (int c, FILE *stream)

// The fputc function converts the character c to type unsigned char, and
// writes it to the stream. EOF is returned if a write error occurs; otherwise
// the character c is returned.


void
fputc_usage(void)
{
    char c = 'A';

    fputc(c, stdout);
}

int
main(void)
{
    fputc_usage();
    return 0;
}


// https://www.gnu.org/software/libc/manual/html_node/Simple-Output.html#Simple-Output
// http://man7.org/linux/man-pages/man3/fputc.3.html