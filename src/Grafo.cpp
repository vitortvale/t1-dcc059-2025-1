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
        std::cerr << "Erro: No com ID '" << novo_no->getId() << "' ja existe no grafo. Nao adicionado." << std::endl;
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
        std::cerr << "Erro: Tentativa de adicionar aresta com ID(s) de no invalido(s) ("
                  << id_origem << " ou " << id_destino << " nao encontrados)." << std::endl;
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
        std::cout << "Aresta " << id_origem << " -> " << id_destino << " ja existe. Nao adicionada novamente." << std::endl;
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


//------------------------------------------------------------------------------
// ----------------------------------- G ---------------------------------------
//------------------------------------------------------------------------------
// GERA UMA ÁRVORE DE CAMINHAMENTO EM PROFUNDIDADE E DESTACA AS ARESTAS DE RETORNO.
// RETORNA UM NOVO GRAFO QUE REPRESENTA A ÁRVORE.
//******************************************************************************
Grafo* Grafo::arvore_caminhamento_profundidade(char id_no) {
    // CRIA O NOVO GRAFO QUE SERÁ A ÁRVORE RESULTANTE.
    Grafo* arvore_dfs = new Grafo();
    arvore_dfs->setDirecionado(this->is_direcionado);

    // VERIFICA SE O NÓ INICIAL EXISTE.
    if (getNo(id_no) == nullptr) {
        std::cout << "ERRO: O no inicial '" << id_no << "' nao existe no grafo." << std::endl;
        return arvore_dfs; // RETORNA A ÁRVORE VAZIA.
    }

    // CRIA O MAPA PARA CONTROLAR O STATUS DE VISITA DE CADA NÓ.
    // INICIALIZA TODOS OS NÓS DO GRAFO ORIGINAL COMO 'NAO_VISITADO'.
    std::map<char, StatusVisita> status;
    for (const auto& no_ptr : this->lista_adj) {
        status[no_ptr->getId()] = NAO_VISITADO;
    }

    // ADICIONA APENAS O NÓ INICIAL À ÁRVORE ANTES DE COMEÇAR A BUSCA.
    arvore_dfs->add_no(this->getNo(id_no));

    std::cout << "--- INICIANDO BUSCA EM PROFUNDIDADE A PARTIR DE '" << id_no << "' ---" << std::endl;

    // CHAMA A FUNÇÃO RECURSIVA PARA CONSTRUIR A ÁRVORE E ACHAR ARESTAS DE RETORNO.
    dfs_para_arvore_com_retorno(id_no, arvore_dfs, status);

    std::cout << "--- FIM DA BUSCA ---" << std::endl;

    return arvore_dfs;
}

//******************************************************************************
// FUNÇÃO AUXILIAR RECURSIVA QUE REALIZA O DFS, MONTA A ÁRVORE
// E IDENTIFICA AS ARESTAS DE RETORNO.
//******************************************************************************
void Grafo::dfs_para_arvore_com_retorno(char id_no_atual, Grafo* arvore, std::map<char, StatusVisita>& status) {
    // ATUALIZA O STATUS DO NÓ ATUAL PARA 'VISITANDO'.
    // ISSO SIGNIFICA QUE ELE ENTROU NA PILHA DE RECURSÃO.
    status[id_no_atual] = VISITANDO;


    No* no_original = this->getNo(id_no_atual);
    if (!no_original) return;

    // PERCORRE TODAS AS ARESTAS DE SAÍDA DO NÓ ATUAL.
    for (const auto& aresta : no_original->getArestas()) {
        char id_vizinho = aresta->getNoAlvo()->getId();
        
        if (status[id_vizinho] == NAO_VISITADO) {
            std::cout << "Aresta de arvore: " << id_no_atual << " -> " << id_vizinho << std::endl;
            
            // A LÓGICA AQUI CONTINUA A MESMA: ADICIONA O VIZINHO E DEPOIS A ARESTA.
            // ISSO GARANTE QUE O NÓ 'VIZINHO' SEJA ADICIONADO APENAS NESTE MOMENTO.
            arvore->add_no(this->getNo(id_vizinho));
            arvore->add_aresta(id_no_atual, id_vizinho, aresta->getPeso());
            
            dfs_para_arvore_com_retorno(id_vizinho, arvore, status);

        } else if (status[id_vizinho] == VISITANDO) {
            std::cout << "ARESTA DE RETORNO ENCONTRADA: " << id_no_atual << " -> " << id_vizinho << std::endl;
        }
    }

    // AO SAIR DA FUNÇÃO, TODOS OS DESCENDENTES DO NÓ ATUAL JÁ FORAM EXPLORADOS.
    // ATUALIZA SEU STATUS PARA 'VISITADO'.
    status[id_no_atual] = VISITADO;
}
//------------------------------------------------------------------------------
// ----------------------------------- H ---------------------------------------
//------------------------------------------------------------------------------
// PARÂMETRO --> O GRAFO (DIRECIONADO OU NÃO DIRECIONADO) PONDERADO NAS ARESTAS
// SAÍDA --> O RAIO, O DIÂMETRO, O CENTRO E A PERIFERIA DO GRAFO.
//******************************************************************************
// FUNÇÃO PARA OBTER O RAIO DO GRAFO.
// ELA CHAMA A FUNÇÃO AUXILIAR QUE FAZ TODO O CÁLCULO
// E APENAS RETORNA A PARTE DO RESULTADO QUE INTERESSA (O RAIO).
//******************************************************************************
int Grafo::raio() {
    MetricasResultados res = calcular_metricas_completas();
    return res.raio;
}
//******************************************************************************
// FUNÇÃO PARA OBTER O DIÂMETRO DO GRAFO.
// FUNCIONA DA MESMA FORMA QUE A FUNÇÃO 'RAIO', DELEGANDO O TRABALHO
// PARA A FUNÇÃO AUXILIAR E RETORNANDO APENAS O DIÂMETRO.
//******************************************************************************
int Grafo::diametro() {
    MetricasResultados res = calcular_metricas_completas();
    return res.diametro;
}
//******************************************************************************
// FUNÇÃO PARA OBTER O CENTRO DO GRAFO (UM VETOR DE VÉRTICES).
// O CENTRO É O CONJUNTO DE VÉRTICES COM A MENOR EXCENTRICIDADE.
// A FUNÇÃO CHAMA O MÉTODO AUXILIAR E RETORNA O VETOR DE VÉRTICES DO CENTRO.
//******************************************************************************
std::vector<char> Grafo::centro() {
    MetricasResultados res = calcular_metricas_completas();
    return res.centro;
}
//******************************************************************************
// FUNÇÃO PARA OBTER A PERIFERIA DO GRAFO (UM VETOR DE VÉRTICES).
// A PERIFERIA É O CONJUNTO DE VÉRTICES COM A MAIOR EXCENTRICIDADE.
// A FUNÇÃO CHAMA O MÉTODO AUXILIAR E RETORNA O VETOR DE VÉRTICES DA PERIFERIA.
//******************************************************************************
std::vector<char> Grafo::periferia() {
    MetricasResultados res = calcular_metricas_completas();
    return res.periferia;
}
//******************************************************************************
// FUNÇÃO AUXILIAR QUE REALIZA TODOS OS CÁLCULOS DE UMA SÓ VEZ.
// ELA É 'CONST' PORQUE NÃO ALTERA O ESTADO INTERNO DO OBJETO GRAFO.
// ELA RETORNA UMA STRUCT 'METRICASRESULTADOS' CONTENDO TODOS OS VALORES.
//******************************************************************************
Grafo::MetricasResultados Grafo::calcular_metricas_completas() const {
    // CRIA UMA INSTÂNCIA DA STRUCT QUE ARMAZENARÁ TODOS OS RESULTADOS.
    MetricasResultados resultados; 

    // DEFINE UMA CONSTANTE PARA REPRESENTAR UMA DISTÂNCIA INFINITA.
    // ISSO É USADO PARA VÉRTICES QUE NÃO SÃO ALCANÇÁVEIS ENTRE SI.
    const int INF = std::numeric_limits<int>::max();

    // PASSO 1: OBTER A MATRIZ DE ADJACÊNCIA COM OS PESOS INICIAIS.
    // CHAMA UMA FUNÇÃO AUXILIAR JÁ EXISTENTE NA CLASSE PARA MONTAR A MATRIZ
    // A PARTIR DA LISTA DE ADJACÊNCIA INTERNA DO GRAFO.
    std::vector<std::vector<int>> dist = inicializar_matriz_adjacencia_com_pesos();

    // PASSO 2: EXECUTAR O ALGORITMO DE FLOYD-WARSHALL.
    // ESTE ALGORITMO CALCULA O MENOR CAMINHO ENTRE TODOS OS PARES DE VÉRTICES.
    // AO FINAL DESTES 3 LOOPS, 'DIST[I][J]' TERÁ O MENOR CUSTO DE 'I' PARA 'J'.
    for (int k = 0; k < ordem; ++k) 
        for (int i = 0; i < ordem; ++i) 
            for (int j = 0; j < ordem; ++j) {
                // SE EXISTE UM CAMINHO DE 'I' PARA 'K' E DE 'K' PARA 'J',
                // E SE ESSE CAMINHO É MAIS CURTO QUE O CAMINHO ATUAL DE 'I' PARA 'J',
                // ATUALIZA A DISTÂNCIA.
                if (dist[i][k] != INF && dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j]) 
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        
    // PASSO 3: CALCULAR A EXCENTRICIDADE DE CADA VÉRTICE.
    // A EXCENTRICIDADE DE UM VÉRTICE É A MAIOR DISTÂNCIA DELE PARA QUALQUER OUTRO VÉRTICE.
    std::vector<int> excentricidades(ordem);
    bool grafo_conexo = true; // FLAG PARA VERIFICAR SE O GRAFO É CONEXO.
    for (int i = 0; i < ordem; ++i) {
        int max_dist = 0; // GUARDA A MAIOR DISTÂNCIA A PARTIR DO VÉRTICE 'I'.
        for (int j = 0; j < ordem; ++j) {
            // SE A DISTÂNCIA FOR INFINITA, SIGNIFICA QUE O VÉRTICE 'J' NÃO É
            // ALCANÇÁVEL A PARTIR DE 'I'. PORTANTO, O GRAFO NÃO É CONEXO.
            if (dist[i][j] == INF) {
                grafo_conexo = false;
                break; // PODE PARAR O LOOP INTERNO.
            }
            // ATUALIZA A DISTÂNCIA MÁXIMA ENCONTRADA ATÉ AGORA A PARTIR DE 'I'.
            if (dist[i][j] > max_dist) {
                max_dist = dist[i][j];
            }
        }
        // SE O GRAFO NÃO FOR CONEXO, INTERROMPE O LOOP PRINCIPAL.
        if (!grafo_conexo) break;
        // ARMAZENA A EXCENTRICIDADE DO VÉRTICE 'I'.
        excentricidades[i] = max_dist;
    }

    // VERIFICAÇÃO PARA GRAFOS NÃO CONEXOS.
    // SE O GRAFO NÃO FOR CONEXO, O RAIO E O DIÂMETRO SÃO CONSIDERADOS INFINITOS.
    // AQUI, USAMOS -1 PARA INDICAR ESSA CONDIÇÃO.
    if (!grafo_conexo) {
        resultados.raio = -1;
        resultados.diametro = -1;
        // O CENTRO E A PERIFERIA FICAM VAZIOS.
        return resultados; // RETORNA IMEDIATAMENTE.
    }

    // PASSO 4: CALCULAR O RAIO E O DIÂMETRO DO GRAFO.
    // O RAIO É A MENOR EXCENTRICIDADE DENTRE TODOS OS VÉRTICES.
    // O DIÂMETRO É A MAIOR EXCENTRICIDADE DENTRE TODOS OS VÉRTICES.
    resultados.raio = excentricidades[0];
    resultados.diametro = excentricidades[0];
    for (int i = 1; i < ordem; ++i) {
        if (excentricidades[i] < resultados.raio) resultados.raio = excentricidades[i];
        if (excentricidades[i] > resultados.diametro) resultados.diametro = excentricidades[i];
    }

    // PASSO 5: ENCONTRAR OS CONJUNTOS DE VÉRTICES DO CENTRO E DA PERIFERIA.
    for (int i = 0; i < ordem; ++i) {
        // UM VÉRTICE PERTENCE AO CENTRO SE SUA EXCENTRICIDADE É IGUAL AO RAIO.
        if (excentricidades[i] == resultados.raio) {
            // CONVERTE O ÍNDICE NUMÉRICO (I) DE VOLTA PARA O RÓTULO (CHAR) DO VÉRTICE.
            resultados.centro.push_back(this->getIdDoIndice(i));
        }
        // UM VÉRTICE PERTENCE À PERIFERIA SE SUA EXCENTRICIDADE É IGUAL AO DIÂMETRO.
        if (excentricidades[i] == resultados.diametro) {
            resultados.periferia.push_back(this->getIdDoIndice(i));
        }
    }

    // RETORNA A STRUCT PREENCHIDA COM TODOS OS RESULTADOS CALCULADOS.
    return resultados; 
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


std::vector<char> Grafo::vertices_de_articulacao() {
    std::cout << "Metodo vertices_de_articulacao nao implementado" << std::endl;
    return {};
}
