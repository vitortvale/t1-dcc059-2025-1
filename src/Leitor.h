#ifndef LEITOR_H
#define LEITOR_H

#include <iostream>
#include "Info.cpp"

using namespace std;

class Leitor {
    public:
        Leitor();
        ~Leitor();
        Info ler();
};

#endif