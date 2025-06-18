#include "Leitor.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>


using namespace std;

Leitor::Leitor() {

}

void Leitor::ler(string filename) {
     ifstream arquivo(filename);
     if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << filename << endl;
        return;
    }

    int direcionado, ponderadoVertices, ponderadoArestas;
    int numVertices;

    // Lê configurações iniciais
    arquivo >> direcionado >> ponderadoVertices >> ponderadoArestas;
    arquivo >> numVertices;
    arquivo.ignore();

    cout << "Direcionado: " << direcionado << "\n";
    cout << "Ponderado nos vértices: " << ponderadoVertices << "\n";
    cout << "Ponderado nas arestas: " << ponderadoArestas << "\n";
    cout << "Número de vértices: " << numVertices << "\n\n";

    cout << "Vértices lidos:\n";
    for (int i = 0; i < numVertices; ++i) {
        string linha;
        getline(arquivo, linha);
        istringstream iss(linha);

        string id;
        int peso = 0;

        iss >> id;
        if (ponderadoVertices) {
            iss >> peso;
            cout << "  " << id << " (peso: " << peso << ")\n";
        } else {
            cout << "  " << id << "\n";
        }
    }

    cout << "\nArestas lidas:\n";
    string linha;
    while (getline(arquivo, linha)) {
        if (linha.empty()) continue;

        istringstream iss(linha);
        string origem, destino;
        int peso = 0;

        iss >> origem >> destino;
        if (ponderadoArestas) {
            iss >> peso;
            cout << "  " << origem << " -> " << destino << " (peso: " << peso << ")\n";
        } else {
            cout << "  " << origem << " -> " << destino << "\n";
        }
    };

}

