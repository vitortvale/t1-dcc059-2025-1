#include <iostream>
#include <string> // Para std::string
#include "Gerenciador.h"
#include "Leitor.h"
#include "Grafo.h" // Inclui Grafo.h para a definição da classe Grafo

// Evitar 'using namespace std;' no escopo global
// using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <nome_do_arquivo_de_entrada>" << std::endl;
        return 1; // Retorna erro se o nome do arquivo não for fornecido
    }

    // Criação dos objetos Leitor e Grafo
    Leitor* leitor = new Leitor();
    Grafo* grafo = new Grafo();

    // Lê o grafo do arquivo especificado no argumento da linha de comando
    // argv[1] é o primeiro argumento após o nome do programa
    leitor->ler(argv[1], grafo);

    // Inicia o menu de comandos do gerenciador
    Gerenciador::comandos(grafo);

    // Libera a memória alocada dinamicamente
    delete grafo;
    delete leitor;

    return 0; // Saída bem-sucedida
}