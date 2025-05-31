#ifndef NO_H
#define NO_H

#include <string>
#include "Aresta.h"


using namespace std;
class No {
public:
    No();
    ~No();

    int id;
    double peso;
    Aresta* primeira_aresta;
};



#endif //NO_H
