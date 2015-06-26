#include <vector>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <stdlib.h>
#include <stdio.h>
#include <stack>
#include <list>
#include <queue>

using namespace std;



struct Nodo {
    int numero;
    int grado;
    Nodo() : numero(0), grado(0) {};
    Nodo(const int n,const bool v,const int g) : numero(n) , grado(g) {};
};

struct orden
{
    inline bool operator() (const Nodo& n1, const Nodo& n2)
    {
        return (n1.grado > n2.grado);
    }
};

typedef vector<Nodo> Nodos;
typedef list<int> listaAdyacencia;
typedef vector<listaAdyacencia> Grafo;

vector<int> resolver(int n, Grafo G, Nodos nodos);

// Implementacion.
int main() {

    FILE* file = fopen("grafo-ciclo-goloso.dat","a+");


    int n, m;
    cin >> n;
    cin >> m;


       Grafo G(n, listaAdyacencia());

        Nodos nodos(n, Nodo());

        for(int i = 0; i < n; i++) {
            nodos[i].numero = i;
        }


    int v, w;
    for (int i = 0; i < m; i++) {
        cin >> v;
        cin >> w;
        G[v-1].push_back(w-1);
        G[w-1].push_back(v-1);
        nodos[v-1].grado = nodos[v-1].grado + 1;
        nodos[w-1].grado = nodos[w-1].grado + 1;
    }

    std::chrono::time_point<std::chrono::high_resolution_clock> t1 = std::chrono::high_resolution_clock::now();

    vector<int> cidm = resolver(n, G, nodos);

    std::chrono::time_point<std::chrono::high_resolution_clock> t2 = std::chrono::high_resolution_clock::now();

    double d = double(std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1).count());

    fprintf(file, "%d %d %lu %7.8f\n",n, m, cidm.size() , d);


    fclose(file);


    return 0;
}

vector<int> resolver(int n, Grafo G, Nodos nodos) {
    // Ordeno a los nodos segun su grado
    std::sort(nodos.begin(), nodos.end(), orden());

    vector<bool> visitado(n, false);
    vector<int> cidm;
    cidm.reserve(n);

    for(int u = 0; u < n ; u++){
        if(visitado[nodos[u].numero] == false){
            visitado[nodos[u].numero] = true;
            cidm.push_back(nodos[u].numero);
            for (list<int>::iterator itAdyU=G[nodos[u].numero].begin(); itAdyU != G[nodos[u].numero].end(); ++itAdyU) {
                int j = *itAdyU;
                cout << j << endl ;
                visitado[j] = true;
            }
        }
    }

    return cidm;
}
