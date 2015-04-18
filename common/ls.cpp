#include "common.h"
#include "filesys.h"

class Options
{
public:
    void parse(int argc, char **argv);
};

class AppLs
{
    static bool compare(string a, string b) { return false; }
public:
    int run(int argc, char **argv);
};

void Options::parse(int argc, char **argv)
{
}

int AppLs::run(int argc, char **argv)
{
#if 1
    vector<string> list;
    FileSystem fs;
    fs.ls(&list);
    sort(list.begin(), list.end(), compare);

    for (vector<string>::iterator it = list.begin(); it != list.end(); it++)
    {
        struct stat filestats;
        ::stat(it->c_str(), &filestats);
        cout << *it << "\n";
    }
#else
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
        throw "Error opening dir";
    }
#endif

    return 0;
}

int main(int argc, char **argv)
{
    AppLs app;
    return app.run(argc, argv);
}


