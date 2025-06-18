#ifndef INFO_H
#define INFO_h

#include <iostream>
#include "No.h"

using namespace std;

typedef struct {
    int ordem;
    bool is_direcionado;
    bool is_ponderado_aresta;
    bool is_ponderado_vertice;
    vector<No*> lista_adj;
} Info;

#endif