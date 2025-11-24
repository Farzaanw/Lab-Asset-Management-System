// Steps to start up CLI interface //
// 1. Enter this folder /cli
// 2. Run the command: g++ main.cpp ../SystemController.cpp -o TestExecutable.exe
// 3. Run the command: ./TestExecutable.exe

#include <iostream>
#include "../SystemController.h"

int main(){
    std::cout << "Welcome to the Lab Asset Management System!" << std::endl;

    SystemController system;
    system.run();

    return 0; 
}
