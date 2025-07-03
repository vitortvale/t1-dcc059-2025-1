#ifndef NO_H
#define NO_H

#include <vector>
#include "Aresta.h" // Inclui Aresta.h para a definição completa da classe Aresta

class No {
public:
    // Construtores
    No(char id, int peso); // Para nós com peso
    No(char id);          // Para nós sem peso (peso padrão 0)

    ~No() = default; // Destrutor padrão é suficiente, pois Aresta* são gerenciados pelo Grafo

    void add_aresta(Aresta *a);

    // Getters
    char getId() const { return id; }
    int getPeso() const { return peso_vertice; }
    int getIndiceInterno() const { return indice_interno; }
    const std::vector<Aresta*>& getArestas() const { return arestas; } // Retorna uma referência constante

    // Setter para o indice_interno (chamado pelo Grafo)
    void setIndiceInterno(int idx) { indice_interno = idx; }

private:
    char id;
    int peso_vertice;     // Peso do VÉRTICE (renomeado para clareza)
    int indice_interno;   // Será setado pelo Grafo, mapeia para a posição no vector lista_adj do Grafo
    std::vector<Aresta*> arestas; // Lista de adjacência para este nó
};

#endif // NO_H