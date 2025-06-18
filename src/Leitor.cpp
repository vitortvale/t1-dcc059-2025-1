#include "Leitor.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "No.h"
#include "Grafo.h"

using namespace std;

Leitor::Leitor() {

}

void Leitor::ler(string filename,Grafo *g) {
     ifstream arquivo(filename);
     if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << filename << endl;
        return;
    }

    int direcionado, ponderadoVertices, ponderadoArestas;
    int numVertices;

    arquivo >> direcionado >> ponderadoVertices >> ponderadoArestas;
    arquivo >> numVertices;
    arquivo.ignore();

    cout << "Lendo informacoes do grafo..." << endl;    
    if(direcionado = 1) {
        g->is_direcionado = true; 
        cout << "Grafo direcionado" << endl;
    }
    else {
        g->is_direcionado = false;
        cout << "Grafo nao direcionado" << endl;

    }
     if(direcionado = 1) {
         g->is_ponderado_vertice = true; 
        cout << "Grafo ponderado nos vertices" << endl;
    }
    else {
        g->is_ponderado_vertice = false;
        cout << "Grafo nao ponderado nos vertices" << endl;

    }
     if(direcionado = 1) {
        g->is_ponderado_aresta = true;
        cout << "Grafo ponderado nas arestas" << endl;
    }
    else {
        g->is_ponderado_aresta = false;
        cout << "Grafo nao ponderado nas arestas" << endl;

    }
    cout << "Número de vértices: " << numVertices << "\n\n";

    cout << "Vértices lidos:\n";
    for (int i = 0; i < numVertices; ++i) {
        string linha;
        getline(arquivo, linha);
        istringstream iss(linha);

        char id;
        int peso = 0;
        iss >> id;

        if (ponderadoVertices) {
            iss >> peso;
            No *n = new No(id, peso);
            cout << "No criado! id: " << id << " (peso: " << peso << ")\n";
            g->add_no(n);

        } else {
            No *n = new No(id);
            cout << "No criado! id: " << id << "\n";
            g->add_no(n);
        }
    }

    cout << "\nArestas lidas:\n";
    string linha;
    while (getline(arquivo, linha)) {
        if (linha.empty()) continue;

        istringstream iss(linha);
        char origem, destino;
        int peso = 0;

        iss >> origem >> destino;
        if (ponderadoArestas) {
            iss >> peso;
            cout << "  " << origem << " -> " << destino << " (peso: " << peso << ")\n";
            if (direcionado) {
                //Adiciona a aresta apenas no No origem
            }
            else {
                //Adiciona a aresta para ambos os nos
            }
        } else {
            cout << "  " << origem << " -> " << destino << "\n";
            if (direcionado) {
                //Adiciona a aresta apenas no No origem
            }
            else {
               //Adiciona a aresta para ambos os nos
            }
        }
    };

}

