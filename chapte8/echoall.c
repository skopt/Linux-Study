#include <stdio.h>

int main(int argc, char *argv[])
{
    int i = 0;
    char **ptr = NULL;
    extern char **environ;

    for(i = 0; i < argc; i++)
    {
        printf("argv[%d]=%s\n", i, argv[i]);
    }

    for(ptr = environ; *ptr != NULL; ptr++)
    {
        printf("%s\n", *ptr);
    }

    return 0;
}
