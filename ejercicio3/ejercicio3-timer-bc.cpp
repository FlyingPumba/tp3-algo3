#include <vector>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <stdlib.h>
#include <stdio.h>

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

typedef vector<int> Vec;
typedef vector<Vec> Matriz;
typedef vector<Nodo> Nodos;

vector<int> resolver(int n, Matriz matriz, Nodos nodos);

// Implementacion.
int main() {

    FILE* file = fopen("tiempos-bc.txt","w+");

    int N_MAX = 3001;
    //int M = N_MAX*(N_MAX-1)/2;

    // int n, m;
    // cin >> n;
    // cin >> m;

    std::vector<double> tiempos(N_MAX);
    std::vector<int> aristas(N_MAX);


    for(int n = 1; n < N_MAX; n++){

        int m = 0;

        Matriz matriz(n, Vec(n, 0));
        Nodos nodos(n, Nodo());

        for(int i = 0; i < n; i++) {
            nodos[i].numero = i;
        }

        // for (int i = 1; i <= n; i++) {
        //     for(int j = i+1; j <=n;j++){
        //         matriz[i-1][j-1] = 1;
        //         matriz[j-1][i-1] = 1;

        //         nodos[i-1].grado = nodos[i-1].grado + 1;
        //         nodos[j-1].grado = nodos[j-1].grado + 1;

        //         m++;
        //     }
        // }

        aristas[n] = m;

        cout << n <<  "holis"<<endl;

        std::chrono::time_point<std::chrono::high_resolution_clock> t1 = std::chrono::high_resolution_clock::now();

        vector<int> cidm = resolver(n, matriz, nodos);

        std::chrono::time_point<std::chrono::high_resolution_clock> t2 = std::chrono::high_resolution_clock::now();

        double d = double(std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1).count());

        cout << n << " " <<  d << endl;

       tiempos[n] = d;


    }

    for (int index = 0; index < N_MAX; index ++) {
        fprintf(file, "%d %7.8f  %d\n", index, tiempos[index], aristas[index]);
    }

    fclose(file);




    return 0;
}

vector<int> resolver(int n, Matriz matriz, Nodos nodos) {
    // Ordeno a los nodos segun su grado
    std::sort(nodos.begin(), nodos.end(), orden());

    vector<bool> visitado(n, false);
    vector<int> cidm;
    cidm.reserve(n);

    for(int i = 0; i < n ; i++){
        if(visitado[nodos[i].numero] == false){
            visitado[nodos[i].numero] = true;
            cidm.push_back(nodos[i].numero);
            for(int j = 0; j<n; j++){
                if(matriz[nodos[i].numero][j] == 1){
                    visitado[j] = true;
                }
            }
        }
    }

    return cidm;
}
