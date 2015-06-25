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

    FILE* file = fopen("tiempos2-lista.txt","w+");

    int N_MAX = 1001;
    //int M = N_MAX*(N_MAX-1)/2;

    // int n, m;
    // cin >> n;
    // cin >> m;

    std::vector<double> tiempos(N_MAX);

    for(int n = 1; n < N_MAX; n++){

       Grafo G(n, listaAdyacencia());

        Nodos nodos(n, Nodo());

        for(int i = 0; i < n; i++) {
            nodos[i].numero = i;
        }


        // for (int i = 1; i <= n; i++) {
        //     for(int j = i+1; j <=n;j++){
        //         G[i-1].push_back(j-1);
        //         G[j-1].push_back(i-1);

        //         nodos[i-1].grado = nodos[i-1].grado + 1;
        //         nodos[j-1].grado = nodos[j-1].grado + 1;
        //     }
        // }   

        cout << n <<  "holis"<<endl;

        std::chrono::time_point<std::chrono::high_resolution_clock> t1 = std::chrono::high_resolution_clock::now();

        vector<int> cidm = resolver(n, G, nodos);
                                             


        std::chrono::time_point<std::chrono::high_resolution_clock> t2 = std::chrono::high_resolution_clock::now();

        double d = double(std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1).count());

        cout << n << " " <<  d << endl;

       tiempos[n] = d;         
        

    }

    for (int index = 0; index < N_MAX; index ++) {       
        fprintf(file, "%d %7.8f\n", index, tiempos[index]);
    }

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
            if(visitado[u] == false){
                for (list<int>::iterator itAdyU=G[nodos[u].numero].begin(); itAdyU != G[nodos[u].numero].end(); ++itAdyU) {
                    int j = *itAdyU;
                    visitado[j] = true;
                }
            }           
        }
    }

    return cidm;
}




