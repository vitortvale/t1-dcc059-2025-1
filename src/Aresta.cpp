#include "Aresta.h"
#include "No.h" 

// Construtor para arestas não ponderadas
Aresta::Aresta(No* no_alvo_ptr) : no_alvo_ptr(no_alvo_ptr), peso(0) {
    // Peso padrão 0, indicando não ponderado ou peso neutro
}

// Construtor para arestas ponderadas
Aresta::Aresta(No* no_alvo_ptr, int peso) : no_alvo_ptr(no_alvo_ptr), peso(peso) {
    // Membros inicializados na lista de inicialização
}
