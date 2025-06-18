#ifndef ARESTA_H
#define ARESTA_H

using namespace std;
class Aresta {
public:
    Aresta(char id_no_alvo);
    Aresta(char id_no_alvo, int peso);
    ~Aresta() = default;

    char id_no_alvo;
    int peso;
    
};



#endif //ARESTA_H
