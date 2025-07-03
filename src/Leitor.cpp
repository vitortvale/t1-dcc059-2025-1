#include "Leitor.h"
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "No.h"
#include "Grafo.h"

Leitor::Leitor() {
}

void Leitor::ler(std::string filename, Grafo *g) {
    std::ifstream arquivo(filename);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
        return;
    }

    int direcionado_val, ponderadoVertices_val, ponderadoArestas_val;
    int numVertices;

    arquivo >> direcionado_val >> ponderadoVertices_val >> ponderadoArestas_val;
    arquivo >> numVertices;
    arquivo.ignore();

    std::cout << "Lendo informacoes do grafo..." << std::endl;

    // CORREÇÃO: Usar '==' para comparação e setters
    g->setDirecionado(direcionado_val == 1);
    std::cout << "Grafo " << (g->isDirecionado() ? "direcionado" : "nao direcionado") << std::endl;

    g->setPonderadoVertice(ponderadoVertices_val == 1);
    std::cout << "Grafo " << (g->isPonderadoVertice() ? "ponderado nos vertices" : "nao ponderado nos vertices") << std::endl;

    g->setPonderadoAresta(ponderadoArestas_val == 1);
    std::cout << "Grafo " << (g->isPonderadoAresta() ? "ponderado nas arestas" : "nao ponderado nas arestas") << std::endl;

    std::cout << "Numero de vertices: " << numVertices << "\n\n";

    std::cout << "Vertices lidos:\n";
    for (int i = 0; i < numVertices; ++i) {
        std::string linha;
        std::getline(arquivo, linha);
        std::istringstream iss(linha);

        char id;
        int peso = 0;

        iss >> id;

        if (g->isPonderadoVertice()) {
            iss >> peso;
            No *n = new No(id, peso);
            std::cout << "No criado! id: " << id << " (peso: " << peso << ")\n";
            g->add_no(n);
        } else {
            No *n = new No(id);
            std::cout << "No criado! id: " << id << "\n";
            g->add_no(n);
        }
    }

    std::cout << "\nArestas lidas:\n";
    std::string linha_aresta;
    while (std::getline(arquivo, linha_aresta)) {
        if (linha_aresta.empty()) continue;

        std::istringstream iss_aresta(linha_aresta);
        char origem, destino;
        int peso = 0;

        iss_aresta >> origem >> destino;
        if (g->isPonderadoAresta()) {
            iss_aresta >> peso;
            std::cout << "   " << origem << " -> " << destino << " (peso: " << peso << ")\n";
            g->add_aresta(origem, destino, peso);
        } else {
            std::cout << "   " << origem << " -> " << destino << "\n";
            g->add_aresta(origem, destino);
        }
    }
    arquivo.close();
}
