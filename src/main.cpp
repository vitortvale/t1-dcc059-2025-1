#include <iostream>
#include "Gerenciador.h"
#include "Leitor.h"



using namespace std;


int main(int argc, char *argv[])
{

    Leitor* leitor = new Leitor();
    leitor->ler(argv[1]);
    //Grafo* grafo = new Grafo(info);

    //Gerenciador::comandos(grafo);

    return 0;
}
