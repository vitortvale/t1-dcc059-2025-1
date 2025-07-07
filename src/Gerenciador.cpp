#include "Gerenciador.h"
#include <fstream>  // Para std::ofstream
#include <limits>   // Para std::numeric_limits
#include <vector>   // Para std::vector
#include <iostream> // Para std::cout, std::endl, std::cerr
#include <algorithm> // Para std::find

void Gerenciador::comandos(Grafo* grafo) {
    while (true) {
        std::cout << "Digite uma das opcoes abaixo e pressione enter:" << std::endl << std::endl;
        std::cout << "(a) Fecho transitivo direto de um no;" << std::endl;
        std::cout << "(b) Fecho transitivo indireto de um no;" << std::endl;
        std::cout << "(c) Caminho minimo (Dijkstra);" << std::endl;
        std::cout << "(d) Caminho minimo (Floyd);" << std::endl;
        std::cout << "(e) Arvore Geradora Minima (Algoritmo de Prim);" << std::endl;
        std::cout << "(f) Arvore Geradora Minima (Algoritmo de Kruskal);" << std::endl;
        std::cout << "(g) Arvore de caminhamento em profundidade;" << std::endl;
        std::cout << "(h) Raio, diametro, centro e periferia do grafo;" << std::endl;
        std::cout << "(i) Vertices de Articulacao;" << std::endl;
        std::cout << "(0) Sair;" << std::endl << std::endl;

        char resp;
        std::cin >> resp;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (resp) {
            case 'a': {
                char id_no = get_id_entrada();
                std::vector<char> fecho_transitivo_direto = grafo->fecho_transitivo_direto(id_no);
                std::cout << "Fecho transitivo direto para " << id_no << ": ";
                if (fecho_transitivo_direto.empty()) {
                    std::cout << "Nenhum no alcancavel." << std::endl;
                } else {
                    for (char c : fecho_transitivo_direto) {
                        std::cout << c << " ";
                    }
                    std::cout << std::endl;
                }

                if (pergunta_imprimir_arquivo("fecho_trans_dir.txt")) {
                    std::ofstream outfile("fecho_trans_dir.txt");
                    if (outfile.is_open()) {
                        outfile << "Fecho transitivo direto de " << id_no << ": ";
                        for (char c : fecho_transitivo_direto) {
                            outfile << c << " ";
                        }
                        outfile << std::endl;
                        outfile.close();
                        std::cout << "Saida salva em fecho_trans_dir.txt" << std::endl;
                    } else {
                        std::cerr << "Erro ao abrir arquivo para escrita." << std::endl;
                    }
                }
                break;
            }
            case 'b': {
                char id_no = get_id_entrada();
                std::vector<char> fecho_transitivo_indireto = grafo->fecho_transitivo_indireto(id_no);
                std::cout << "Fecho transitivo indireto para " << id_no << ": ";
                if (fecho_transitivo_indireto.empty()) {
                    std::cout << "Nenhum no que alcanca este." << std::endl;
                } else {
                    for (char c : fecho_transitivo_indireto) {
                        std::cout << c << " ";
                    }
                    std::cout << std::endl;
                }

                if (pergunta_imprimir_arquivo("fecho_trans_indir.txt")) {
                     std::ofstream outfile("fecho_trans_indir.txt");
                    if (outfile.is_open()) {
                        outfile << "Fecho transitivo indireto de " << id_no << ": ";
                        for (char c : fecho_transitivo_indireto) {
                            outfile << c << " ";
                        }
                        outfile << std::endl;
                        outfile.close();
                        std::cout << "Saida salva em fecho_trans_indir.txt" << std::endl;
                    } else {
                        std::cerr << "Erro ao abrir arquivo para escrita." << std::endl;
                    }
                }
                break;
            }
            case 'c': {
                char id_no_1 = get_id_entrada();
                char id_no_2 = get_id_entrada();
                std::vector<char> caminho_minimo_dijkstra = grafo->caminho_minimo_dijkstra(id_no_1, id_no_2);
                std::cout << "Caminho minimo (Dijkstra) de " << id_no_1 << " para " << id_no_2 << ": ";
                if (caminho_minimo_dijkstra.empty()) {
                    std::cout << "Caminho nao encontrado." << std::endl;
                } else {
                    for (char c : caminho_minimo_dijkstra) {
                        std::cout << c << " ";
                    }
                    std::cout << std::endl;
                }

                if (pergunta_imprimir_arquivo("caminho_minimo_dijkstra.txt")) {
                    std::ofstream outfile("caminho_minimo_dijkstra.txt");
                    if (outfile.is_open()) {
                        outfile << "Caminho minimo (Dijkstra) de " << id_no_1 << " para " << id_no_2 << ": ";
                        for (char c : caminho_minimo_dijkstra) {
                            outfile << c << " ";
                        }
                        outfile << std::endl;
                        outfile.close();
                        std::cout << "Saida salva em caminho_minimo_dijkstra.txt" << std::endl;
                    } else {
                        std::cerr << "Erro ao abrir arquivo para escrita." << std::endl;
                    }
                }
                break;
            }
            case 'd': {
                char id_no_1 = get_id_entrada();
                char id_no_2 = get_id_entrada();
                std::vector<char> caminho_minimo_floyd = grafo->caminho_minimo_floyd(id_no_1, id_no_2);
                 std::cout << "Caminho minimo (Floyd) de " << id_no_1 << " para " << id_no_2 << ": ";
                if (caminho_minimo_floyd.empty()) {
                    std::cout << "Caminho nao encontrado." << std::endl;
                } else {
                    for (char c : caminho_minimo_floyd) {
                        std::cout << c << " ";
                    }
                    std::cout << std::endl;
                }

                if (pergunta_imprimir_arquivo("caminho_minimo_floyd.txt")) {
                     std::ofstream outfile("caminho_minimo_floyd.txt");
                    if (outfile.is_open()) {
                        outfile << "Caminho minimo (Floyd) de " << id_no_1 << " para " << id_no_2 << ": ";
                        for (char c : caminho_minimo_floyd) {
                            outfile << c << " ";
                        }
                        outfile << std::endl;
                        outfile.close();
                        std::cout << "Saida salva em caminho_minimo_floyd.txt" << std::endl;
                    } else {
                        std::cerr << "Erro ao abrir arquivo para escrita." << std::endl;
                    }
                }
                break;
            }
            case 'e': {
                int tam;
                std::cout << "Digite o tamanho do subconjunto: ";
                std::cin >> tam;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (tam > 0 && tam <= grafo->getOrdem()) {
                    std::vector<char> ids = get_conjunto_ids(grafo, tam);
                    Grafo* arvore_geradora_minima_prim = grafo->arvore_geradora_minima_prim(ids);
                    std::cout << "Arvore Geradora Minima (Prim) - Saida em tela nao implementada para Grafo*" << std::endl;

                    if (pergunta_imprimir_arquivo("agm_prim.txt")) {
                        std::ofstream outfile("agm_prim.txt");
                        if (outfile.is_open() && arvore_geradora_minima_prim) {
                            outfile << "Saida da Arvore Geradora Minima (Prim) para subconjunto: ";
                            for(char id : ids) outfile << id << " ";
                            outfile << "\nConteudo da arvore nao implementado para impressao aqui." << std::endl;
                            outfile.close();
                            std::cout << "Saida salva em agm_prim.txt" << std::endl;
                        } else {
                            std::cerr << "Erro ao abrir arquivo ou arvore nao gerada." << std::endl;
                        }
                    }
                    delete arvore_geradora_minima_prim;
                } else {
                    std::cout << "Valor invalido" << std::endl;
                }
                break;
            }
            case 'f': {
                int tam;
                std::cout << "Digite o tamanho do subconjunto: ";
                std::cin >> tam;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (tam > 0 && tam <= grafo->getOrdem()) {
                    std::vector<char> ids = get_conjunto_ids(grafo, tam);
                    Grafo* arvore_geradora_minima_kruskal = grafo->arvore_geradora_minima_kruskal(ids);
                    std::cout << "Arvore Geradora Minima (Kruskal)" << std::endl;
                    // --- IMPRESSÃO NA TELA DO CONSOLE ---
                    arvore_geradora_minima_kruskal->imprimir_grafo_letra_f(std::cout);
                       
                    if (pergunta_imprimir_arquivo("agm_kruskal.txt")) {
                        std::ofstream outfile("agm_kruskal.txt");
                        if (outfile.is_open() && arvore_geradora_minima_kruskal) {
                            outfile << "Saida da Arvore Geradora Minima (Kruskal) para subconjunto: ";
                            for(char id : ids) outfile << id << " ";
                            outfile << "\nConteudo da arvore nao implementado para impressao aqui." << std::endl;
                            outfile.close();
                            std::cout << "Saida salva em agm_kruskal.txt" << std::endl;
                        } else {
                            std::cerr << "Erro ao abrir arquivo ou arvore nao gerada." << std::endl;
                        }
                    }
                    delete arvore_geradora_minima_kruskal;
                } else {
                    std::cout << "Valor invalido" << std::endl;
                }
                break;
            }
            case 'g': {
                char id_no = get_id_entrada();
                Grafo* arvore_caminhamento_profundidade = grafo->arvore_caminhamento_profundidade(id_no);
                // ******************************************************************
                // OBSERVAÇÃO: IMPLEMENTAÇÃO DA IMPRESSÃO FEITA EM GRAFO.CPP DEVIDO A COMPLEXIDADE
                // ******************************************************************
                std::cout << "Arvore de caminhamento em profundidade" << std::endl;

                if (pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt")) {
                     std::ofstream outfile("arvore_caminhamento_profundidade.txt");
                    if (outfile.is_open() && arvore_caminhamento_profundidade) {
                        outfile << "Saida da Arvore de caminhamento em profundidade a partir de " << id_no << ":\n";
                        outfile << "Conteudo da arvore nao implementado para impressao aqui." << std::endl;
                        outfile.close();
                        std::cout << "Saida salva em arvore_caminhamento_profundidade.txt" << std::endl;
                    } else {
                        std::cerr << "Erro ao abrir arquivo ou arvore nao gerada." << std::endl;
                    }
                }
                delete arvore_caminhamento_profundidade;
                break;
            }
            case 'h': {
                int raio_val = grafo->raio();
                int diametro_val = grafo->diametro();
                std::vector<char> centro_val = grafo->centro();
                std::vector<char> periferia_val = grafo->periferia();

                std::cout << "Raio do grafo: " << raio_val << std::endl;
                std::cout << "Diametro do grafo: " << diametro_val << std::endl;
                std::cout << "Centro do grafo: ";
                for (char c : centro_val) std::cout << c << " ";
                std::cout << std::endl;
                std::cout << "Periferia do grafo: ";
                for (char c : periferia_val) std::cout << c << " ";
                std::cout << std::endl;

                if (pergunta_imprimir_arquivo("grafo_propriedades.txt")) {
                    std::ofstream outfile("grafo_propriedades.txt");
                    if (outfile.is_open()) {
                        outfile << "Raio do grafo: " << raio_val << std::endl;
                        outfile << "Diametro do grafo: " << diametro_val << std::endl;
                        outfile << "Centro do grafo: ";
                        for (char c : centro_val) outfile << c << " ";
                        outfile << std::endl;
                        outfile << "Periferia do grafo: ";
                        for (char c : periferia_val) outfile << c << " ";
                        outfile << std::endl;
                        outfile.close();
                        std::cout << "Saida salva em grafo_propriedades.txt" << std::endl;
                    } else {
                        std::cerr << "Erro ao abrir arquivo para escrita." << std::endl;
                    }
                }
                break;
            }
             case 'i': {
                std::vector<char> articulacao = grafo->vertices_de_articulacao();
                std::cout << "Vertices de articulacao: ";
                if (articulacao.empty()) {
                    std::cout << "Nenhum." << std::endl;
                } else {
                    for (char c : articulacao) {
                        std::cout << c << " ";
                    }
                    std::cout << std::endl;
                }

                if (pergunta_imprimir_arquivo("vertices_articulacao.txt")) {
                     std::ofstream outfile("vertices_articulacao.txt");
                    if (outfile.is_open()) {
                        outfile << "Vertices de articulacao: ";
                        for (char c : articulacao) {
                            outfile << c << " ";
                        }
                        outfile << std::endl;
                        outfile.close();
                        std::cout << "Saida salva em vertices_articulacao.txt" << std::endl;
                    } else {
                        std::cerr << "Erro ao abrir arquivo para escrita." << std::endl;
                    }
                }
                break;
            }
            case '0': {
                return;
            }
            default: {
                std::cout << "Opcao invalida" << std::endl;
            }
        }
        std::cout << std::endl;
    }
}

char Gerenciador::get_id_entrada() {
    std::cout << "Digite o id de um no: ";
    char id;
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << std::endl;
    return id;
}

std::vector<char> Gerenciador::get_conjunto_ids(Grafo *grafo, int tam) {
    std::vector<char> ids = {};
    while(static_cast<int>(ids.size()) < tam) { // Corrigido warning de signed/unsigned
        char id_no = get_id_entrada();
        bool existe = false;
        // CORREÇÃO: Usar o método público getNo para verificar a existência do nó
        if (grafo->getNo(id_no) != nullptr) {
            existe = true;
        }

        if(!existe){
            std::cout << "Vertice nao existe no grafo." << std::endl;
        }else{
            bool repetido = std::find(ids.begin(), ids.end(), id_no) != ids.end();
            if(repetido) {
                std::cout << "Valor repetido no subconjunto." << std::endl;
            }else {
                ids.push_back(id_no);
            }
        }
    }
    return ids;
}

bool Gerenciador::pergunta_imprimir_arquivo(std::string nome_arquivo) {
    std::cout << "Imprimir em arquivo externo? (" << nome_arquivo << ")" << std::endl;
    std::cout << "(1) Sim;" << std::endl;
    std::cout << "(2) Nao." << std::endl;
    int resp;
    std::cin >> resp;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << std::endl;

    switch (resp) {
        case 1:
            return true;
        case 2:
            return false;
        default:
            std::cout << "Resposta invalida. Por favor, digite 1 ou 2." << std::endl;
            return pergunta_imprimir_arquivo(nome_arquivo);
    }
}
