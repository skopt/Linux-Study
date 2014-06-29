#include <stdio.h>
#include "log.h"
void main()
{
    char *p = "abfasfasfsdfdsafsfsafsfdsfsafsdfsafhwefhasfhsdjfsadfsafcd\n";
    int ret = 0;
    log_init();
    ret = log_write(p,strlen(p));

    printf("ret is:%d\n",ret);
}
