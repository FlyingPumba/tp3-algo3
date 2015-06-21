#include <vector>
#include <iostream>
#include <stack>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <ctime>
using namespace std;

#define INFINITO -1
#define CANT_TESTS 100
#define CANT_NODOS_MIN 2
#define CANT_NODOS_MAX 12

typedef vector<int> Vec;
typedef vector<Vec> Matriz;

vector<int> resolver(int n, Matriz matriz);
vector<int> resolver_aux(int n, Matriz matriz, vector<int> dom, vector<int> cidm);
int random_in_range(int min, int max);
string to_string(int value);

int random_in_range(int min, int max) {
  return min + (rand() % (max - min + 1));
}

string to_string(int value) {
    ostringstream oss;
    oss << value;
    return oss.str();
}

vector<string> get_random_filename() {
    int x = random_in_range(100, 5000);
    int y = random_in_range(100, 5000);
    int z = random_in_range(100, 5000);
    vector<string> aux(2, "");
    aux[0] += to_string(x);
    aux[0] += to_string(y);
    aux[0] += to_string(z);
    aux[0] += ".in";
    aux[1] += to_string(x);
    aux[1] += to_string(y);
    aux[1] += to_string(z);
    aux[1] += ".out";
    return aux;
}

int main() {
    srand(time(0)); // use current time as seed for random generator

    for (int c = 0; c < CANT_TESTS; c++) {
        vector<string> files = get_random_filename();
        FILE* file_in = fopen(files[0].c_str(),"w+");

        int n = random_in_range(CANT_NODOS_MIN, CANT_NODOS_MAX);
        int m = random_in_range(0, n*(n-1)/2);

        cout << "TEST: " << c << ", n: " << n << ", m: " << m << " -> " << files[1] << endl;
        fprintf(file_in, "%d %d\n", n, m);

        Matriz matriz(n, Vec(n, 0));

        for (int i = 0; i < m; i++) {
            int v, w;
            do  {
                v = random_in_range(1, n);
                w = random_in_range(1, n);
            } while (v == w || matriz[v-1][w-1] == 1);
            // cout << i << endl;
            fprintf(file_in, "%d %d\n", v, w);
            matriz[v-1][w-1] = 1;
            matriz[w-1][v-1] = 1;
        }

        fclose(file_in);
        cout << "Resolviendo" << endl;
        vector<int> cidm = resolver(n, matriz);

        FILE* file_out = fopen(files[1].c_str(),"w+");
        fprintf(file_out, "[");
        // cout << "[";
        for (int i = 0; i < cidm.size() - 1; i++) {
            fprintf(file_out, "%d,", cidm[i]+1);
            // cout << cidm[i]+1 << ",";
        }
        fprintf(file_out, "%d]", cidm[cidm.size() - 1]+1);
        // cout <<  cidm[cidm.size() - 1]+1 << "]" << endl;
        fclose(file_out);
    }

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
            // Sabemos que si Dom es dominante e independiente, entonces cualquier subconjunto de Dom es no-dominante
            // Luego, devolvemos el que tiene menor cardinal entre dom y cidm
            if (dom_size < cidm_size) {
                return dom;
            } else {
                return cidm;
            }
        }
    }

    for (int i = 0; i < dom_size; i++) {
        // copio dom y borro el i-esimo nodo de la copia (no es el nodo numero i, sino el nodo en la posicion i del vector)
        vector<int> copia(dom);
        copia.erase(copia.begin() + i);
        // chequeo si la copia es dominante
        bool copia_dominante = true;
        for (int i = 0; i < n; i++) {
            // chequeo si el nodo i está en copia o es adyacente a alguno en copia
            bool nodo = false;
            for (int j = 0; j < copia.size(); j++) {
                if (copia[j] == i) {
                    // el nodo i está en copia
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
        // Sabemos que si Copia no es dominante, entonces ningun subconjunto de Copia es dominante, por lo que ni siquiera los evaluo
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
