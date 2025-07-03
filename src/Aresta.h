#ifndef ARESTA_H
#define ARESTA_H

class No; // Forward declaration

class Aresta {
public:
    // Construtor para arestas não ponderadas (peso padrão 0)
    Aresta(No* no_alvo_ptr);

    // Construtor para arestas ponderadas
    Aresta(No* no_alvo_ptr, int peso);

    ~Aresta() = default; // O compilador gera um destrutor padrão eficiente

    // Métodos "getters"
    No* getNoAlvo() const { return no_alvo_ptr; }
    int getPeso() const { return peso; }

private:
    No* no_alvo_ptr; // Ponteiro para o objeto No que é o alvo desta aresta
    int peso;        // Peso da aresta (0 para não ponderadas, ou um valor específico)
};

#endif // ARESTA_H
