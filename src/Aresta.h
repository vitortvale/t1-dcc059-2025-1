#ifndef ARESTA_H
#define ARESTA_H

// Forward declaration para evitar inclusão circular e facilitar dependências
class No; // Declara que 'No' é uma classe, para podermos usar 'No*'

class Aresta {
public:
    // Construtor para arestas não ponderadas (peso padrão 0)
    // O ponteiro para o 'No' de destino é passado aqui
    Aresta(No* no_alvo_ptr);

    // Construtor para arestas ponderadas
    Aresta(No* no_alvo_ptr, int peso);

    ~Aresta() = default;

    // Métodos "getters" para acessar os dados (boa prática de encapsulamento)
    No* getNoAlvo() const { return no_alvo_ptr; }
    int getPeso() const { return peso; }

private:
    No* no_alvo_ptr; // Ponteiro para o objeto No que é o alvo desta aresta
    int peso;        // Peso da aresta (0 para não ponderadas, ou um valor específico)
};

#endif // ARESTA_H
