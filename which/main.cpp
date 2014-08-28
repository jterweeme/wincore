#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <sys/stat.h>
#include <algorithm>


class MyVector : public std::vector<std::string>
{
public:
    MyVector(const char *env);
    MyVector(std::string env);
    void constructor(std::string env);
    std::string toString();
};

class App
{
public:
    int run(int argc, char **argv);
};

MyVector::MyVector(const char *env)
{
    constructor(std::string(env));
}

MyVector::MyVector(std::string env)
{
    constructor(env);
}

std::string MyVector::toString()
{
    std::string s;

    for (iterator it = begin(); it != end(); ++it)
    {
	s += *it;
	s += "\r\n";
    }

    return s;
}

void MyVector::constructor(std::string env)
{
    std::string s;

    for (int i = 0; i < env.length(); i++)
    {
	if (env[i] == ';')
	{
	    push_back(s);
	    s.clear();
	}
	else
	{
            s += env[i];
	}
    }

    push_back(s);
}

int App::run(int argc, char **argv)
{
    if (argc != 2)
        return -1;
       
    char *env = getenv("PATH");
    MyVector path(env);
    struct stat foo;

    for (MyVector::iterator it = path.begin(); it != path.end(); it++)
    {
	std::string pad = std::string(*it);
	std::replace(pad.begin(), pad.end(), '\\', '/');
	pad += '/';
	pad += argv[1];
	std::string com = pad + ".com";
	std::string exe = pad + ".exe";
	std::string bat = pad + ".bat";

	if (::stat(com.c_str(), &foo) == 0)
	    std::cout << com.c_str() << std::endl;
	if (::stat(exe.c_str(), &foo) == 0)
	    std::cout << exe.c_str() << std::endl;
	if (::stat(bat.c_str(), &foo) == 0)
	    std::cout << bat.c_str() << std::endl;
    }
    return 0;
}

int main(int argc, char **argv)
{
    App app;
    return app.run(argc, argv);
}


