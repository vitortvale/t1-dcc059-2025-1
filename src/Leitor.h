#ifndef LEITOR_H
#define LEITOR_H

#include <iostream>
#include "Grafo.h"

using namespace std;

class Leitor {
    public:
        Leitor();
        ~Leitor() = default;
        void ler(string filename, Grafo *g);
};

#endif