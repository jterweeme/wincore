class MyNode
{
public:
    MyNode() { }
};

class MyDocument
{
    MyNode *_root;
public:
    MyDocument() { }
    void import(istream &is);
};

class App
{
public:
    App() { }
    int run();
};

void MyDocument::import(istream &is)
{
    
}

int App::run()
{
    MyDocument doc;
    return 0;
}

int main()
{
    App app;
    return app.run();
}


