#ifndef LEITOR_H
#define LEITOR_H

#include <iostream>
#include <string> // Para std::string
#include "Grafo.h" // Inclui Grafo.h

// Evitar 'using namespace std;' em arquivos .h
// class Leitor { ... };

class Leitor {
public:
    Leitor();
    ~Leitor() = default;
    void ler(std::string filename, Grafo *g);
};

#endif // LEITOR_H