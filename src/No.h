#ifndef NO_H
#define NO_H

#include <string>
#include <vector>
#include "Aresta.h"


using namespace std;
class No {
public:
    No();
    ~No();

    char id;
    int peso;
    vector<Aresta*> arestas;
    // implementar add aresta(push no vector)
};



#endif //NO_H
