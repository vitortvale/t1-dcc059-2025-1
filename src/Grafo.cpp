#include "Grafo.h"
#include <algorithm> // Para usar std::find_if, std::sort
#include <iostream>  // Para std::cerr, std::cout, std::endl
#include <set>       // Para o gerenciamento de memória no destrutor
#include <queue>     // Para std::priority_queue em Dijkstra
#include <stack>     // Para DFS iterativo
#include <limits>    // Para std::numeric_limits<int>::max()

Grafo::Grafo() :
    ordem(0),
    is_direcionado(false),
    is_ponderado_aresta(false),
    is_ponderado_vertice(false) {
    // Construtor: Inicializa os membros.
}

Grafo::~Grafo() {
    // Destrutor: Libera a memória alocada dinamicamente para os Nós e Arestas.
    std::set<Aresta*> arestas_para_deletar;
    for (No* no : lista_adj) {
        if (no) {
            for (Aresta* aresta : no->getArestas()) {
                arestas_para_deletar.insert(aresta);
            }
        }
    }
    for (Aresta* aresta : arestas_para_deletar) {
        delete aresta;
    }
    arestas_para_deletar.clear();

    for (No* no : lista_adj) {
        delete no;
    }

    lista_adj.clear();
    id_para_indice.clear();
    indice_para_id.clear();
}


// --- Métodos de Adição de Nós e Arestas ---

void Grafo::add_no(No *novo_no) {
    if (id_para_indice.count(novo_no->getId()) > 0) {
        std::cerr << "Erro: Nó com ID '" << novo_no->getId() << "' já existe no grafo. Não adicionado." << std::endl;
        delete novo_no;
        return;
    }

    int novo_indice = lista_adj.size();
    novo_no->setIndiceInterno(novo_indice);

    lista_adj.push_back(novo_no);
    id_para_indice[novo_no->getId()] = novo_indice;
    indice_para_id.push_back(novo_no->getId());
    ordem++;
}

void Grafo::add_aresta(char id_origem, char id_destino, int peso_aresta) {
    No* no_origem = getNo(id_origem);
    No* no_destino = getNo(id_destino);

    if (no_origem == nullptr || no_destino == nullptr) {
        std::cerr << "Erro: Tentativa de adicionar aresta com ID(s) de nó inválido(s) ("
                  << id_origem << " ou " << id_destino << " não encontrados)." << std::endl;
        return;
    }

    bool aresta_ja_existe = false;
    for(Aresta* a : no_origem->getArestas()){
        if(a->getNoAlvo()->getId() == id_destino){
            aresta_ja_existe = true;
            break;
        }
    }

    if (!aresta_ja_existe) {
        Aresta* aresta_saida = new Aresta(no_destino, peso_aresta);
        no_origem->add_aresta(aresta_saida);

        std::cout << "Aresta adicionada: " << no_origem->getId() << " -> " << no_destino->getId();
        if (is_ponderado_aresta) {
            std::cout << " (peso: " << peso_aresta << ")";
        }
        std::cout << std::endl;

        if (!is_direcionado) {
            bool aresta_inversa_ja_existe = false;
            for(Aresta* a : no_destino->getArestas()){
                if(a->getNoAlvo()->getId() == id_origem){
                    aresta_inversa_ja_existe = true;
                    break;
                }
            }
            if (!aresta_inversa_ja_existe) {
                Aresta* aresta_entrada = new Aresta(no_origem, peso_aresta);
                no_destino->add_aresta(aresta_entrada);
                std::cout << "Aresta reversa adicionada: " << no_destino->getId() << " -> " << no_origem->getId();
                if (is_ponderado_aresta) {
                    std::cout << " (peso: " << peso_aresta << ")";
                }
                std::cout << std::endl;
            }
        }
    } else {
        std::cout << "Aresta " << id_origem << " -> " << id_destino << " já existe. Não adicionada novamente." << std::endl;
    }
}


// --- Métodos Auxiliares de Mapeamento ---

int Grafo::getIndiceDoId(char id) const {
    auto it = id_para_indice.find(id);
    if (it != id_para_indice.end()) {
        return it->second;
    }
    return -1;
}

char Grafo::getIdDoIndice(int indice) const {
    if (indice >= 0 && indice < static_cast<int>(indice_para_id.size())) { // Corrigido warning
        return indice_para_id[indice];
    }
    std::cerr << "Erro: Índice de nó inválido: " << indice << std::endl;
    return '\0';
}

// --- Métodos Getters ---

No* Grafo::getNo(char id) const {
    int indice = getIndiceDoId(id);
    if (indice != -1) {
        return lista_adj[indice];
    }
    return nullptr;
}

No* Grafo::getNoPorIndice(int indice) const {
    if (indice >= 0 && indice < static_cast<int>(lista_adj.size())) { // Corrigido warning
        return lista_adj[indice];
    }
    return nullptr;
}


// --- Implementações de Funcionalidades para Fecho Transitivo ---

void Grafo::dfs_recursiva(No* no_atual, std::map<char, bool>& visitado, std::vector<char>& resultado) {
    visitado[no_atual->getId()] = true;
    resultado.push_back(no_atual->getId());

    for (Aresta* aresta : no_atual->getArestas()) {
        No* vizinho = aresta->getNoAlvo();
        if (!visitado[vizinho->getId()]) {
            dfs_recursiva(vizinho, visitado, resultado);
        }
    }
}

std::vector<char> Grafo::fecho_transitivo_direto(char id_no) {
    No* inicio = getNo(id_no);
    if (inicio == nullptr) {
        std::cerr << "Erro: Nó de início '" << id_no << "' não encontrado para fecho transitivo direto." << std::endl;
        return {};
    }

    std::map<char, bool> visitado;
    for (No* no : lista_adj) {
        visitado[no->getId()] = false;
    }

    std::vector<char> resultado;
    dfs_recursiva(inicio, visitado, resultado);

    std::sort(resultado.begin(), resultado.end());
    return resultado;
}

Grafo* Grafo::getGrafoTransposto() const {
    Grafo* grafo_transposto = new Grafo();
    grafo_transposto->setDirecionado(true); // Um grafo transposto é sempre direcionado
    grafo_transposto->setPonderadoAresta(is_ponderado_aresta);
    grafo_transposto->setPonderadoVertice(is_ponderado_vertice);

    for (No* original_no : lista_adj) {
        No* novo_no = new No(original_no->getId(), original_no->getPeso());
        grafo_transposto->add_no(novo_no);
    }

    for (No* original_no : lista_adj) {
        for (Aresta* original_aresta : original_no->getArestas()) {
            char id_origem_transposto = original_aresta->getNoAlvo()->getId();
            char id_destino_transposto = original_no->getId();
            int peso_aresta_transposto = original_aresta->getPeso();
            grafo_transposto->add_aresta(id_origem_transposto, id_destino_transposto, peso_aresta_transposto);
        }
    }
    return grafo_transposto;
}

std::vector<char> Grafo::fecho_transitivo_indireto(char id_no) {
    Grafo* grafo_transposto = getGrafoTransposto();
    std::vector<char> resultado = grafo_transposto->fecho_transitivo_direto(id_no);
    delete grafo_transposto;
    std::sort(resultado.begin(), resultado.end());
    return resultado;
}

// --- Implementação do Caminho Mínimo (Dijkstra) (c) ---
std::vector<char> Grafo::caminho_minimo_dijkstra(char id_no_a, char id_no_b) {
    if (!is_ponderado_aresta) {
        std::cerr << "Erro: Dijkstra requer um grafo ponderado nas arestas." << std::endl;
        return {};
    }

    No* inicio_no = getNo(id_no_a);
    No* fim_no = getNo(id_no_b);

    if (inicio_no == nullptr || fim_no == nullptr) {
        std::cerr << "Erro: Nó(s) de início/destino inválido(s) para Dijkstra." << std::endl;
        return {};
    }

    std::map<char, int> dist;
    std::map<char, char> predecessores;
    std::priority_queue<DistanciaNo, std::vector<DistanciaNo>, std::greater<DistanciaNo>> pq;

    for (No* no : lista_adj) {
        dist[no->getId()] = std::numeric_limits<int>::max();
        predecessores[no->getId()] = '\0';
    }
    dist[inicio_no->getId()] = 0;
    pq.push({0, inicio_no->getId()});

    while (!pq.empty()) {
        DistanciaNo atual = pq.top();
        pq.pop();

        int d_atual = atual.distancia;
        char id_u = atual.id_no;

        if (d_atual > dist[id_u]) {
            continue;
        }

        No* u_no = getNo(id_u);
        for (Aresta* aresta : u_no->getArestas()) {
            No* v_no = aresta->getNoAlvo();
            char id_v = v_no->getId();
            int peso_uv = aresta->getPeso();

            if (dist[id_u] != std::numeric_limits<int>::max() && dist[id_u] + peso_uv < dist[id_v]) {
                dist[id_v] = dist[id_u] + peso_uv;
                predecessores[id_v] = id_u;
                pq.push({dist[id_v], id_v});
            }
        }
    }

    std::vector<char> caminho;
    char no_atual_caminho = fim_no->getId();
    
    // Verifica se o nó de destino foi alcançado
    if (dist[no_atual_caminho] == std::numeric_limits<int>::max()) {
        std::cout << "Caminho de " << id_no_a << " para " << id_no_b << " nao encontrado (distancia infinita)." << std::endl;
        return {};
    }

    // Reconstroi o caminho
    while (no_atual_caminho != '\0') {
        caminho.insert(caminho.begin(), no_atual_caminho);
        if (no_atual_caminho == inicio_no->getId()) {
            break;
        }
        no_atual_caminho = predecessores[no_atual_caminho];
    }
    
    // Verificação adicional para garantir que o caminho começa no nó de origem
    if (caminho.empty() || caminho[0] != inicio_no->getId()) {
        std::cout << "Caminho de " << id_no_a << " para " << id_no_b << " nao encontrado (problema na reconstrucao ou inicio)." << std::endl;
        return {};
    }

    std::cout << "Custo do caminho de " << id_no_a << " para " << id_no_b << ": " << dist[fim_no->getId()] << std::endl;
    return caminho;
}

// --- Funções Auxiliares para Floyd-Warshall ---
std::vector<std::vector<int>> Grafo::inicializar_matriz_adjacencia_com_pesos() const {
    int num_vertices = ordem;
    std::vector<std::vector<int>> dist(num_vertices, std::vector<int>(num_vertices, std::numeric_limits<int>::max()));

    for (int i = 0; i < num_vertices; ++i) {
        dist[i][i] = 0; // Distância de um nó para ele mesmo é 0
    }

    for (No* u_no : lista_adj) {
        int u_idx = u_no->getIndiceInterno();
        for (Aresta* aresta : u_no->getArestas()) {
            No* v_no = aresta->getNoAlvo();
            int v_idx = v_no->getIndiceInterno();
            if (u_idx != -1 && v_idx != -1) { // Garante que os índices são válidos
                 dist[u_idx][v_idx] = aresta->getPeso();
            }
        }
    }
    return dist;
}


// --- Implementação do Caminho Mínimo (Floyd-Warshall) (d) ---
std::vector<char> Grafo::caminho_minimo_floyd(char id_no_a, char id_no_b) {
    if (!is_ponderado_aresta) {
        std::cerr << "Erro: Floyd requer um grafo ponderado nas arestas." << std::endl;
        return {};
    }

    int inicio_idx = getIndiceDoId(id_no_a);
    int fim_idx = getIndiceDoId(id_no_b);

    if (inicio_idx == -1 || fim_idx == -1) {
        std::cerr << "Erro: Nó(s) de início/destino inválido(s) para Floyd." << std::endl;
        return {};
    }

    int num_vertices = ordem;
    std::vector<std::vector<int>> dist = inicializar_matriz_adjacencia_com_pesos();
    
    // Matriz next_node para reconstrução do caminho: next_node[i][j] armazena o próximo nó
    // no caminho mais curto de i para j. Inicialmente, se há aresta i->j, é j.
    std::vector<std::vector<int>> next_node(num_vertices, std::vector<int>(num_vertices, -1));
    for (int i = 0; i < num_vertices; ++i) {
        for (int j = 0; j < num_vertices; ++j) {
            if (dist[i][j] != std::numeric_limits<int>::max() && i != j) {
                next_node[i][j] = j; // Se há uma aresta direta, o próximo é o próprio destino
            }
        }
    }

    // Algoritmo de Floyd-Warshall
    for (int k = 0; k < num_vertices; ++k) {
        for (int i = 0; i < num_vertices; ++i) {
            for (int j = 0; j < num_vertices; ++j) {
                // Evita overflow (INF + valor) e garante que o caminho via k é válido
                if (dist[i][k] != std::numeric_limits<int>::max() &&
                    dist[k][j] != std::numeric_limits<int>::max()) {
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        next_node[i][j] = next_node[i][k]; // O próximo de i para j é o próximo de i para k
                    }
                }
            }
        }
    }

    // Verificação de ciclos negativos (dist[i][i] < 0) - importante para a corretude
    for (int i = 0; i < num_vertices; ++i) {
        if (dist[i][i] < 0) {
            std::cerr << "Ciclo negativo detectado no grafo! Caminhos minimos podem ser indefinidos." << std::endl;
            return {}; // Retorna vazio se houver ciclo negativo
        }
    }

    // Reconstruir o caminho
    std::vector<char> caminho_final;
    if (dist[inicio_idx][fim_idx] == std::numeric_limits<int>::max()) {
        std::cout << "Caminho de " << id_no_a << " para " << id_no_b << " nao encontrado (distancia infinita)." << std::endl;
        return {};
    }

    std::cout << "Custo do caminho de " << id_no_a << " para " << id_no_b << ": " << dist[inicio_idx][fim_idx] << std::endl;

    // Reconstrução do caminho usando next_node
    int current_node_idx = inicio_idx;
    while (current_node_idx != fim_idx) {
        caminho_final.push_back(getIdDoIndice(current_node_idx));
        current_node_idx = next_node[current_node_idx][fim_idx];
        if (current_node_idx == -1) { // Caso não deveria acontecer se o caminho foi encontrado
             std::cerr << "Erro na reconstrucao do caminho Floyd (next_node inválido)." << std::endl;
             return {};
        }
    }
    caminho_final.push_back(getIdDoIndice(fim_idx)); // Adiciona o nó final

    return caminho_final;
}


// --- Outras funcionalidades (Stubs permanecem) ---
Grafo * Grafo::arvore_geradora_minima_prim(const std::vector<char>& ids_nos) {
    std::cout << "Metodo arvore_geradora_minima_prim nao implementado" << std::endl;
    return nullptr;
}

Grafo * Grafo::arvore_geradora_minima_kruskal(const std::vector<char>& ids_nos) {
    std::cout << "Metodo arvore_geradora_minima_kruskal nao implementado" << std::endl;
    return nullptr;
}

Grafo * Grafo::arvore_caminhamento_profundidade(char id_no) {
    std::cout << "Metodo arvore_caminhamento_profundidade nao implementado" << std::endl;
    return nullptr;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// LETRA H 
// PARÂMETRO --> o grafo (direcionado ou não direcionado) ponderado nas arestas
// SAÍDA --> O raio, o diâmetro, o centro e a periferia do grafo.
// RAIO
int Grafo::raio() {
    MetricasResultados res = calcular_metricas_completas();
    return res.raio;
}
// DIAMETRO
int Grafo::diametro() {
    MetricasResultados res = calcular_metricas_completas();
    return res.diametro;
}
// CENTRO
std::vector<char> Grafo::centro() {
    MetricasResultados res = calcular_metricas_completas();
    return res.centro;
}
// PERIFERIA
std::vector<char> Grafo::periferia() {
    MetricasResultados res = calcular_metricas_completas();
    return res.periferia;
}
// FUNÇÃO AUXILIAR PARA CÁLCULOS
Grafo::MetricasResultados Grafo::calcular_metricas_completas() const {
    MetricasResultados resultados; // INICIALIZA A STRUCT DEFINIDA EM PRIVATE

    const int INF = std::numeric_limits<int>::max();

    // 1. OBTER A MATRIZ DE ADJACÊNCIA 
    std::vector<std::vector<int>> dist = inicializar_matriz_adjacencia_com_pesos();

    // 2. EXECUTAR FLOYD-WARSHALL
    for (int k = 0; k < ordem; ++k) 
        for (int i = 0; i < ordem; ++i) 
            for (int j = 0; j < ordem; ++j) {
                if (dist[i][k] != INF && dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j]) 
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        
    // 3. CALCULAR EXCENTRICIDADES
    std::vector<int> excentricidades(ordem);
    bool grafo_conexo = true;
    for (int i = 0; i < ordem; ++i) {
        int max_dist = 0;
        for (int j = 0; j < ordem; ++j) {
            if (dist[i][j] == INF) {
                grafo_conexo = false;
                break;
            }
            if (dist[i][j] > max_dist) {
                max_dist = dist[i][j];
            }
        }
        if (!grafo_conexo) break;
        excentricidades[i] = max_dist;
    }

    // GRAFO NÃO É CONEXO
    if (!grafo_conexo) {
        resultados.raio = -1;
        resultados.diametro = -1;
        // centro e periferia JÁ ESTÃO VAZIOS, PODE RETORNAR
        return resultados; 
    }

    // 4. CALCULAR RAIO E DIÂMETRO
    resultados.raio = excentricidades[0];
    resultados.diametro = excentricidades[0];
    for (int i = 1; i < ordem; ++i) {
        if (excentricidades[i] < resultados.raio) resultados.raio = excentricidades[i];
        if (excentricidades[i] > resultados.diametro) resultados.diametro = excentricidades[i];
    }

    // 5. ENCONTRAR CENTRO E PERIFERIA
    for (int i = 0; i < ordem; ++i) {
        if (excentricidades[i] == resultados.raio) {
            resultados.centro.push_back(this->getIdDoIndice(i));
        }
        if (excentricidades[i] == resultados.diametro) {
            resultados.periferia.push_back(this->getIdDoIndice(i));
        }
    }

    return resultados; // RETORNA A STRUCT COMPLETA
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------



std::vector<char> Grafo::vertices_de_articulacao() {
    std::cout << "Metodo vertices_de_articulacao nao implementado" << std::endl;
    return {};
}
