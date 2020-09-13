#include <iostream>

int main(int argc, char *argv[])
{
    //temporary for lazyness
    argc = 4;
    argv = new char *[4];

    if (argc != 4)
    {
        throw std::invalid_argument("Usage: main.exe [input file] [algorithm index] [verbose level]");
    }

    return 0;
}