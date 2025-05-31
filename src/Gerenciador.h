#ifndef GERENCIADOR_H
#define GERENCIADOR_H

#include <iostream>
#include "Grafo.h"


using namespace std;
class Gerenciador {
public:
    static void comandos(Grafo* grafo);
    static int get_id_entrada();
    static int* get_conjunto_ids(Grafo* grafo, int tam);
    static bool pergunta_imprimir_arquivo(string nome_arquivo);
};


#endif //GERENCIADOR_H
