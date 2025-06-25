#ifndef GRAFO_H
#define GRAFO_H

#include "No.h" // Inclui a definição da classe No
#include <iostream>
#include <vector>
#include <map>    // Para o mapeamento de char ID para int índice
#include <string> // Pode ser útil se IDs complexos forem usados, mas char está ok por enquanto

// Evitar 'using namespace std;' em arquivos .h
// class Grafo { ... };

class Grafo {
public:
    // Construtor: Inicializa flags e mapeamentos
    Grafo();

    // Destrutor: Responsável por liberar a memória alocada para os Nós e Arestas
    ~Grafo();

    // Métodos para adicionar nós e arestas
    void add_no(No *novo_no);
    // Adiciona uma aresta entre dois nós. O peso é opcional e default para 0.
    void add_aresta(char id_origem, char id_destino, int peso_aresta = 0);

    // Métodos de funcionalidades (assinaturas atualizadas para usar char id)
    // O retorno pode ser ajustado para incluir mais informações se necessário (ex: caminho, custo)
    std::vector<char> fecho_transitivo_direto(char id_no); // a
    std::vector<char> fecho_transitivo_indireto(char id_no); // b
    std::vector<char> caminho_minimo_dijkstra(char id_no_a, char id_no_b); // c
    std::vector<char> caminho_minimo_floyd(char id_no_a, char id_no_b); // d - corrigido id_no_b
    Grafo* arvore_geradora_minima_prim(std::vector<char> ids_nos); // e
    Grafo* arvore_geradora_minima_kruskal(std::vector<char> ids_nos); // f
    Grafo* arvore_caminhamento_profundidade(char id_no); // g
    int raio(); // h 1
    int diametro(); // h 2
    std::vector<char> centro(); // h 3
    std::vector<char> periferia(); // h 4
    std::vector<char> vertices_de_articulacao(); // i (Adicionado, mas não pedido na lista inicial do trabalho)

    // Métodos getters para as propriedades do grafo (Leitor pode usá-las para configurar)
    bool isDirecionado() const { return is_direcionado; }
    bool isPonderadoAresta() const { return is_ponderado_aresta; }
    bool isPonderadoVertice() const { return is_ponderado_vertice; }
    int getOrdem() const { return ordem; }
    No* getNo(char id); // Retorna o ponteiro para um No dado seu ID
    No* getNoPorIndice(int indice); // Retorna o ponteiro para um No dado seu índice

    // Atributos do grafo
    int ordem; // Número de vértices do grafo
    bool is_direcionado;
    bool is_ponderado_aresta;
    bool is_ponderado_vertice;

private:
    std::vector<No*> lista_adj; // Lista de adjacência principal: vector de ponteiros para Nós
    std::map<char, int> id_para_indice; // Mapeia o ID char do vértice para seu índice no lista_adj
    std::vector<char> indice_para_id; // Mapeia o índice para o ID char (útil para resultados)

    // Funções auxiliares para buscar nós por ID (internas)
    int getIndiceDoId(char id);
    char getIdDoIndice(int indice);
};

#endif // GRAFO_H
