#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    // creating news graphs.csv each time
    system("rm graphs.csv ; touch graphs.csv");

    string command = "mpic++ -std=c++11 main.cpp final2.cpp";
    system(command.c_str());

    cout << "\n\n ::::::::: Running on 8 processes :::::::::\n\n"
         << endl;
    command = "mpirun -np 8 ./a.out 8";
    system(command.c_str());

    cout << "\n\n ::::::::: Running on 10 processes :::::::::\n\n"
         << endl;
    command = "mpirun -np 10 ./a.out 10";
    system(command.c_str());

    cout << "\n\n ::::::::: Running on 12 processes :::::::::\n\n"
         << endl;
    command = "mpirun -np 12 ./a.out 12";
    system(command.c_str());

    cout << "\n\n ::::::::: Running on 14 processes :::::::::\n\n"
         << endl;
    command = "mpirun -np 14 ./a.out 14";
    system(command.c_str());

    cout << "\n\n ::::::::: Running on 16 processes :::::::::\n\n"
         << endl;
    command = "mpirun -np 16 ./a.out 16";
    system(command.c_str());

    system("exit");
    return 0;
}
