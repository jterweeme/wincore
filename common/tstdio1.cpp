#include "common.h"

int main()
{
    fputc('a', stdout);
    puts("Hallo puts\n");
    fputs("Hallo standard error fputs\n", stderr);
    FILE *fp = fopen("diff.cpp", "r");
    char c = fgetc(fp);
    fputc(c, stdout);
    char s[100] = {0};
    fread((char *)s, 4, 1, fp);
    puts(s);
    //printf("Hallo printf\n", 1, 1);
    fclose(fp);
    fp = fopen("nieuw.txt", "w");
    fclose(fp);
    return 0;
}


