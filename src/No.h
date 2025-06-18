#ifndef NO_H
#define NO_H

#include <string>
#include <vector>
#include "Aresta.h"


using namespace std;
class No {
public:
    No(char id, int peso);
    No(char id);
    ~No() = default;
    void add_aresta(Aresta *a);

    char id;
    int peso;
    vector<Aresta*> arestas;
    
};



#endif //NO_H
