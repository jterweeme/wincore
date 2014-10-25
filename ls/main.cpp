#include <dirent.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(".")) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
            printf("%s\n", ent->d_name);

        closedir (dir);
    }
    else
    {
        perror ("");
        return -1;
    }
    return 0;
}


