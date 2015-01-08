#include <stdio.h>

int main(int argc, char **argv)
{
    do
    {

        for (int i = 1; i < argc; i++)
            printf("%s ", argv[i]);
    }
    while (putchar('\n') != EOF);
    return 0;
}

