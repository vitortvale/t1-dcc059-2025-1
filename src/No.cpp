#include "No.h"

No::No(char id, int peso) : id(id), peso_vertice(peso), indice_interno(-1) {
    // -1 indica que o índice ainda não foi definido pelo Grafo
}

No::No(char id) : id(id), peso_vertice(0), indice_interno(-1) {
    // Peso padrão 0 para nós não ponderados
}

void No::add_aresta(Aresta *a) {
    this->arestas.push_back(a);
}
