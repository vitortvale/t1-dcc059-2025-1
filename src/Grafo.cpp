#include "Grafo.h"
#include <algorithm> // Para usar std::find_if ou outros algoritmos se necessário

Grafo::Grafo() :
    ordem(0), // Inicializa a ordem como 0
    is_direcionado(false),
    is_ponderado_aresta(false),
    is_ponderado_vertice(false) {
    // Construtor: Inicializa os membros.
    // As flags serão setadas pelo Leitor.
}

Grafo::~Grafo() {
    // Destrutor: Libera a memória alocada dinamicamente para os Nós e Arestas.
    // É CRUCIAL para evitar vazamentos de memória.

    // Primeiro, liberar as Arestas
    for (No* no : lista_adj) {
        if (no) { // Verifica se o ponteiro não é nulo
            // Itera sobre as arestas de cada nó e as deleta
            // CUIDADO: Se o grafo não é direcionado, cada aresta será criada duas vezes
            // (uma para cada sentido). Precisamos garantir que cada objeto Aresta
            // seja deletado APENAS UMA VEZ.
            // Uma abordagem é guardar ponteiros para as arestas em um conjunto global no Grafo,
            // ou garantir que a Aresta não direcionada seja uma única instância.
            // Para simplicidade inicial, vamos assumir que cada 'new Aresta'
            // precisa ser deletado, o que implica que Arestas bidirecionais
            // criam dois objetos Aresta distintos (A->B e B->A).
            // Se você quiser otimizar isso, pode usar smart pointers ou um pool de arestas.

            // ABORDAGEM SIMPLES (considera objetos Aresta distintos para A->B e B->A):
            for (Aresta* aresta : no->getArestas()) {
                delete aresta; // Libera o objeto Aresta
            }
            // Limpa o vetor de arestas do nó (opcional, mas boa prática)
            // No::getArestas() retorna const&, então precisamos de um método não-const para clear
            // OU No::arestas deve ser acessível aqui (o que é contra encapsulamento)
            // Se getArestas() for const &, o destrutor precisaria de um método para limpar.
            // Alternativa: no->arestas (se public) ou um no->clear_arestas();
        }
    }

    // Segundo, liberar os Nós
    for (No* no : lista_adj) {
        delete no; // Libera o objeto No
    }

    // Opcional: Limpar os vetores e mapas após liberar a memória
    lista_adj.clear();
    id_para_indice.clear();
    indice_para_id.clear();
}


// --- Métodos de Adição de Nós e Arestas ---

void Grafo::add_no(No *novo_no) {
    // Verifica se o nó já existe no grafo pelo ID
    if (id_para_indice.count(novo_no->getId()) > 0) {
        std::cerr << "Erro: Nó com ID '" << novo_no->getId() << "' já existe no grafo." << std::endl;
        delete novo_no; // Evita vazamento de memória se o nó já existe
        return;
    }

    // Atribui o índice interno ao nó
    int novo_indice = lista_adj.size();
    // novo_no->setIndiceInterno(novo_indice); // Se você adicionar um setter para indice_interno em No
    // No construtor de No, já passamos o indice_interno.
    // Apenas confirme que o No foi criado com o indice correto antes de chamar add_no.
    // Se o No foi criado com o índice, não é necessário setar novamente aqui.

    lista_adj.push_back(novo_no); // Adiciona o ponteiro do nó ao vector principal
    id_para_indice[novo_no->getId()] = novo_indice; // Mapeia ID para índice
    indice_para_id.push_back(novo_no->getId()); // Mapeia índice para ID
    ordem++; // Incrementa a ordem do grafo
}

void Grafo::add_aresta(char id_origem, char id_destino, int peso_aresta) {
    // 1. Obter os nós de origem e destino usando os IDs
    int indice_origem = getIndiceDoId(id_origem);
    int indice_destino = getIndiceDoId(id_destino);

    if (indice_origem == -1 || indice_destino == -1) {
        std::cerr << "Erro: Tentativa de adicionar aresta com ID(s) de nó inválido(s)." << std::endl;
        return;
    }

    No* no_origem = lista_adj[indice_origem];
    No* no_destino = lista_adj[indice_destino];

    // 2. Criar e adicionar a aresta (No_Origem -> No_Destino)
    // A Aresta agora recebe um ponteiro para o No alvo
    Aresta* aresta_saida = new Aresta(no_destino, peso_aresta);
    no_origem->add_aresta(aresta_saida);
    std::cout << "Aresta adicionada: " << no_origem->getId() << " -> " << no_destino->getId();
    if (is_ponderado_aresta) {
        std::cout << " (peso: " << peso_aresta << ")";
    }
    std::cout << std::endl;


    // 3. Se o grafo não for direcionado, adicionar a aresta inversa (No_Destino -> No_Origem)
    if (!is_direcionado) {
        Aresta* aresta_entrada = new Aresta(no_origem, peso_aresta);
        no_destino->add_aresta(aresta_entrada);
        std::cout << "Aresta reversa adicionada: " << no_destino->getId() << " -> " << no_origem->getId();
        if (is_ponderado_aresta) {
            std::cout << " (peso: " << peso_aresta << ")";
        }
        std::cout << std::endl;
    }
}

// --- Métodos Auxiliares de Mapeamento ---

int Grafo::getIndiceDoId(char id) {
    auto it = id_para_indice.find(id);
    if (it != id_para_indice.end()) {
        return it->second;
    }
    return -1; // Retorna -1 se o ID não for encontrado
}

char Grafo::getIdDoIndice(int indice) {
    if (indice >= 0 && indice < indice_para_id.size()) {
        return indice_para_id[indice];
    }
    // Lidar com erro ou retornar um valor padrão, como '\0' ou lançar exceção
    std::cerr << "Erro: Índice de nó inválido: " << indice << std::endl;
    return '\0'; // Retorna um caractere nulo para indicar erro
}

// --- Métodos Getters ---

No* Grafo::getNo(char id) {
    int indice = getIndiceDoId(id);
    if (indice != -1) {
        return lista_adj[indice];
    }
    return nullptr; // Retorna nullptr se o nó não for encontrado
}

No* Grafo::getNoPorIndice(int indice) {
    if (indice >= 0 && indice < lista_adj.size()) {
        return lista_adj[indice];
    }
    return nullptr; // Retorna nullptr se o índice for inválido
}

// --- Implementações de Funcionalidades (Stubs permanecem por enquanto) ---

std::vector<char> Grafo::fecho_transitivo_direto(char id_no) {
    std::cout << "Metodo fecho_transitivo_direto nao implementado" << std::endl;
    return {};
}

std::vector<char> Grafo::fecho_transitivo_indireto(char id_no) {
    std::cout << "Metodo fecho_transitivo_indireto nao implementado" << std::endl;
    return {};
}

std::vector<char> Grafo::caminho_minimo_dijkstra(char id_no_a, char id_no_b) {
    std::cout << "Metodo caminho_minimo_dijkstra nao implementado" << std::endl;
    return {};
}

std::vector<char> Grafo::caminho_minimo_floyd(char id_no_a, char id_no_b) {
    std::cout << "Metodo caminho_minimo_floyd nao implementado" << std::endl;
    return {};
}

Grafo * Grafo::arvore_geradora_minima_prim(std::vector<char> ids_nos) {
    std::cout << "Metodo arvore_geradora_minima_prim nao implementado" << std::endl;
    return nullptr;
}

Grafo * Grafo::arvore_geradora_minima_kruskal(std::vector<char> ids_nos) {
    std::cout << "Metodo arvore_geradora_minima_kruskal nao implementado" << std::endl;
    return nullptr;
}

Grafo * Grafo::arvore_caminhamento_profundidade(char id_no) {
    std::cout << "Metodo arvore_caminhamento_profundidade nao implementado" << std::endl;
    return nullptr;
}

int Grafo::raio() {
    std::cout << "Metodo raio nao implementado" << std::endl;
    return 0;
}

int Grafo::diametro() {
    std::cout << "Metodo diametro nao implementado" << std::endl;
    return 0;
}

std::vector<char> Grafo::centro() {
    std::cout << "Metodo centro nao implementado" << std::endl;
    return {};
}

std::vector<char> Grafo::periferia() {
    std::cout << "Metodo periferia nao implementado" << std::endl;
    return {};
}

std::vector<char> Grafo::vertices_de_articulacao() {
    std::cout << "Metodo vertices_de_articulacao nao implementado" << std::endl;
    return {};
}
