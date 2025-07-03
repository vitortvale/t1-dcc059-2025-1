#include "No.h"

// Construtor para nós com peso
No::No(char id, int peso) : id(id), peso_vertice(peso), indice_interno(-1) {
    // -1 indica que o índice ainda não foi definido pelo Grafo
}

// Construtor para nós sem peso (peso padrão 0)
No::No(char id) : id(id), peso_vertice(0), indice_interno(-1) {
    // Peso padrão 0 para nós não ponderados
}

// Adiciona uma aresta à lista de adjacência deste nó
void No::add_aresta(Aresta *a) {
    this->arestas.push_back(a);
}