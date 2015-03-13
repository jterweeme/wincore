#include "common.h"

class OptionsDate
{
public:
    void parse(int argc, char **argv);
};

class AppDate
{
public:
    int run(int argc, char **argv);
};

int AppDate::run(int argc, char **argv)
{
    char onzin[900] = {0};
    tm cal;
    cal.tm_year = 0;
    strftime(onzin, 900, "%Y", &cal);
    cout << onzin << "\n";
    //cout << "Thu Jan  1 00:00:00 UTC 1970\n";
    return 0;
}

int main(int argc, char **argv)
{
    AppDate app;
    return app.run(argc, argv);
}


