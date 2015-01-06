#include <stdio.h>
int main()
{
    char onzin[255] = {0};
    fread(onzin, 10, 1, stdin);
    int count = ftell(stdin);
    printf("%d\n", count);
    return 0;
}


