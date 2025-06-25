#ifndef NO_H
#define NO_H

#include <vector>
#include "Aresta.h"

class No {
public:
    // Construtores
    No(char id, int peso); // Para nós com peso
    No(char id);          // Para nós sem peso (peso padrão 0)

    ~No() = default;

    void add_aresta(Aresta *a);

    // Getters
    char getId() const { return id; }
    int getPeso() const { return peso_vertice; }
    int getIndiceInterno() const { return indice_interno; } // Agora tem getter
    const std::vector<Aresta*>& getArestas() const { return arestas; }

    // Setter para o indice_interno (chamado pelo Grafo)
    void setIndiceInterno(int idx) { indice_interno = idx; }

private:
    char id;
    int peso_vertice; // Renomeado para clareza (peso do VÉRTICE)
    int indice_interno; // Será setado pelo Grafo
    std::vector<Aresta*> arestas; // Lista de adjacência
};

#endif //NO_H
