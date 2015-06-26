#include <vector>
#include <iostream>
#include <stack>
#include <algorithm>
using namespace std;

#define INFINITO -1

typedef vector<int> Vec;
typedef vector<Vec> Matriz;

vector<int> resolver(int n, Matriz matriz);
vector<int> resolver_aux(int n, Matriz matriz, vector<int> dom, vector<int> cidm);

// Implementacion.
int main() {

    FILE* file = fopen("grafo-completo-exacto.dat","a+");

    int n, m;
    cin >> n;
    cin >> m;

    Matriz matriz(n, Vec(n, 0));

    int v, w;
    for (int i = 0; i < m; i++) {
        cin >> v;
        cin >> w;
        matriz[v-1][w-1] = 1;
        matriz[w-1][v-1] = 1;
    }

    std::chrono::time_point<std::chrono::high_resolution_clock> t1 = std::chrono::high_resolution_clock::now();

    vector<int> cidm = resolver(n, matriz);

    std::chrono::time_point<std::chrono::high_resolution_clock> t2 = std::chrono::high_resolution_clock::now();

    double d = double(std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1).count());

    fprintf(file, "%d %d %lu %7.8f\n",n, m, cidm.size() , d);
    

    fclose(file);

    return 0;
}

vector<int> resolver(int n, Matriz matriz) {
    // inicializo el primer conjunto a evaluar con todos los vertices del grafo
    // Este primer conjunto es dominante (pero no necesariamente independiente)
    vector <int> dom(n, 0);
    for (int i = 0; i < n; i++) {
        dom[i] = i;
    }
    return resolver_aux(n, matriz, dom, dom);
}

vector<int> resolver_aux(int n, Matriz matriz, vector<int> dom, vector<int> cidm) {
    int dom_size = dom.size();
    int cidm_size = cidm.size();
    if (cidm_size == 1) {
        return cidm;
    } else {
        // chequeo si dom es independiente
        bool indep = true;
        for (int i = 0; i < dom_size; i++) {
            for (int j = i+1; j < dom_size; j++) {
                if (matriz[dom[i]][dom[j]] == 1) {
                    indep = false;
                    break;
                }
            }
            if(!indep) {
                break;
            }
        }

        if(indep) {
            // Sabemos que si Dom es dominante e independiente,
            // entonces cualquier subconjunto de Dom es no-dominante
            // Luego, devolvemos el que tiene menor cardinal entre dom y cidm
            if (dom_size < cidm_size) {
                return dom;
            } else {
                return cidm;
            }
        }
    }

    for (int i = 0; i < dom_size; i++) {
        // copio dom y borro el i-esimo nodo de la copia 
        //(no es el nodo numero i, sino el nodo en la posicion i del vector)
        vector<int> copia(dom);
        copia.erase(copia.begin() + i);
        // chequeo si la copia es dominante
        bool copia_dominante = true;
        for (int i = 0; i < n; i++) {
            // chequeo si el nodo i esta en copia o es adyacente a alguno en copia
            bool nodo = false;
            for (int j = 0; j < copia.size(); j++) {
                if (copia[j] == i) {
                    // el nodo i esta en copia
                    nodo = true;
                    break;
                } else if (matriz[i][copia[j]] == 1) {
                    // el nodo i es adyacente a alguno en copia
                    nodo = true;
                    break;
                }
            }
            if (!nodo) {
                copia_dominante = false;
                break;
            }
        }
        // Sabemos que si Copia no es dominante, entonces ningun 
        //subconjunto de Copia es dominante,
        // por lo que ni siquiera los evaluo
        if (copia_dominante) {
            vector<int> nuevo_cidm = resolver_aux(n, matriz, copia, cidm);
            if (nuevo_cidm.size() < cidm_size) {
                cidm = nuevo_cidm;
                cidm_size = nuevo_cidm.size();
            }
        }
    }
    return cidm;
}
