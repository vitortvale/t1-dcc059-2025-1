#ifndef GRAFO_H
#define GRAFO_H

#include "No.h" // Inclui a definição da classe No
#include <vector>
#include <map>
#include <string>
#include <set>       // Para o gerenciamento de memória no destrutor
#include <limits>    // Para std::numeric_limits<int>::max() em Dijkstra/Floyd
#include <queue>     // Para std::priority_queue em Dijkstra

class Grafo {
public:
    // Construtor
    Grafo();

    // Destrutor: Libera a memória alocada para os Nós e Arestas
    ~Grafo();

    // Métodos para adicionar nós e arestas
    void add_no(No *novo_no);
    void add_aresta(char id_origem, char id_destino, int peso_aresta = 0);

    // Métodos de funcionalidades
    std::vector<char> fecho_transitivo_direto(char id_no); // a
    std::vector<char> fecho_transitivo_indireto(char id_no); // b
    std::vector<char> caminho_minimo_dijkstra(char id_no_a, char id_no_b); // c
    std::vector<char> caminho_minimo_floyd(char id_no_a, char id_no_b); // d
    Grafo* arvore_geradora_minima_prim(const std::vector<char>& ids_nos); // e
    Grafo* arvore_geradora_minima_kruskal(const std::vector<char>& ids_nos); // f
    Grafo* arvore_caminhamento_profundidade(char id_no); // g
    int raio(); // h 1
    int diametro(); // h 2
    std::vector<char> centro(); // h 3
    std::vector<char> periferia(); // h 4
    std::vector<char> vertices_de_articulacao(); // i

    // Métodos setters para as propriedades do grafo
    void setDirecionado(bool b) { is_direcionado = b; }
    void setPonderadoAresta(bool b) { is_ponderado_aresta = b; }
    void setPonderadoVertice(bool b) { is_ponderado_vertice = b; }

    // Métodos getters para as propriedades do grafo
    bool isDirecionado() const { return is_direcionado; }
    bool isPonderadoAresta() const { return is_ponderado_aresta; }
    bool isPonderadoVertice() const { return is_ponderado_vertice; }
    int getOrdem() const { return ordem; }
    No* getNo(char id) const;
    No* getNoPorIndice(int indice) const;
    const std::vector<No*>& getListaAdj() const { return lista_adj; }

    // "STRUCT" PARA CALCULAR TODOS OS ITENS DA LETRA H
    struct MetricasResultados {
        int raio;
        int diametro;
        std::vector<char> centro;
        std::vector<char> periferia;
    };
    // FUNÇÃO RESPONSÁVEL POR CALCULAR TODOS OS ITENS DA LETRA H
    MetricasResultados calcular_metricas_completas() const;

    // Funções auxiliares para Floyd-Warshall
    // O retorno pode ser `std::vector<std::vector<long long>>` para evitar overflow com INF + valor
    std::vector<std::vector<int>> inicializar_matriz_adjacencia_com_pesos() const;

    // FUNÇÃO QUE SABE IMPRIMIR O GRAFO DA LETRA F (UTILIZA LISTA DE ADJACENCIAS)
    void imprimir_grafo_letra_f(std::ostream& out) const;

    // Funcao para imprimir o grafo retornado pelo algoritmo de prim
    void print_arvore(Grafo* arvore);

private:
    int ordem;
    bool is_direcionado;
    bool is_ponderado_aresta;
    bool is_ponderado_vertice;

    std::vector<No*> lista_adj;
    std::map<char, int> id_para_indice;
    std::vector<char> indice_para_id;

    // Funções auxiliares para buscar nós por ID (internas)
    int getIndiceDoId(char id) const;
    char getIdDoIndice(int indice) const;

    // Função auxiliar para DFS (usada para fecho transitivo)
    void dfs_recursiva(No* no_atual, std::map<char, bool>& visitado, std::vector<char>& resultado);

    // Função auxiliar para criar grafo transposto
    Grafo* getGrafoTransposto() const;

    // --- Novas adições para Dijkstra e Floyd ---
    // Estrutura para a fila de prioridade de Dijkstra
    struct DistanciaNo {
        int distancia;
        char id_no;
        bool operator>(const DistanciaNo& other) const {
            return distancia > other.distancia;
        }
    };

    // ENUM PARA CONTROLAR OS 3 ESTADOS DA VISITAÇÃO DO DFS
    enum StatusVisita { NAO_VISITADO, VISITANDO, VISITADO };

    // NOVA FUNÇÃO AUXILIAR PARA A LETRA G
    void dfs_para_arvore_com_retorno(char id_no_atual, Grafo* arvore, std::map<char, StatusVisita>& status);

    // LETRA F
    // FUNÇÃO AUXILIAR PARA VERIFICAR SE EXISTE CAMINHO ENTRE DOIS NÓS (USADO NO KRUSKAL SIMPLIFICADO)
    bool existe_caminho(char id_origem, char id_destino);
    // FUNÇÃO RECURSIVA PARA A BUSCA EM PROFUNDIDADE USADA POR 'existe_caminho'
    bool dfs_existe_caminho(char id_atual, char id_destino, std::map<char, bool>& visitados);

    


    
    

};

#endif // GRAFO_H
