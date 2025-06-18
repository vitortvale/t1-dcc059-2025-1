#include "No.h"

No::No(char id, int peso) {
    this->id = id;
    this->peso = peso;
}

No::No(char id) {
    this->id = id;
}


void No::add_aresta(Aresta *a) {
    this->arestas.push_back(a);
}