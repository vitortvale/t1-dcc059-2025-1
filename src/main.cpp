#include <iostream>
#include "Gerenciador.h"
#include "Leitor.h"



using namespace std;


int main(int argc, char *argv[])
{

    Leitor* leitor = new Leitor();
    Grafo* grafo = new Grafo();
    leitor->ler(argv[1], grafo);
    
    //Gerenciador::comandos(grafo);

    return 0;
}
