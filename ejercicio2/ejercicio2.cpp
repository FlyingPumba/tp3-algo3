#include <vector>
#include <iostream>
#include <stack>
#include <algorithm> 
using namespace std;

#define INFINITO -1

typedef vector<int> Vec;
typedef vector<Vec> Matriz;

vector<int> resolver(int n, Matriz matriz);
vector<int> resolver_aux(int n, Matriz matriz, vector<int> actual, vector<int> cidm);

// Implementacion.
int main() {
    int n, m;
    cin >> n;
    cin >> m;

    Matriz matriz(n, Vec(n, 0));

    int v, w;
    for (int i = 0; i < m; i++) {
        cin >> v;
        cin >> w;
        matriz[v][w] = 1;
        matriz[w][v] = 1;
    }

    vector<int> cidm = resolver(n, matriz);

    cout << "[";
    for (int i = 0; i < cidm.size() - 1; i++) {
        cout << cidm[i] << ",";
    }
    cout <<  cidm[cidm.size() - 1] << "]" << endl;

    return 0;
}

vector<int> resolver(int n, Matriz matriz) {
    // inicializo el primer conjunto a evaluar con todos los vertices del grafo
    vector <int> actual(n, 0);
    for (int i = 0; i < n; i++) {
        actual[i] = i;
    }
    return resolver_aux(n, matriz, actual, actual);
}

vector<int> resolver_aux(int n, Matriz matriz, vector<int> actual, vector<int> cidm) {
    int actual_size = actual.size();
    int cidm_size = cidm.size();
    if (cidm_size == 1) {
        return cidm;
    } else {
        // chequeo si el actual es independiente
        bool indep = true;
        for (int i = 0; i < actual_size; i++) {
            for (int j = i+1; j < actual_size; j++) {
                if (matriz[actual[i]][actual[j]] == 1) {
                    indep = false;
                    break;
                }
            }
            if(!indep) {
                break;
            }
        }

        bool dom = true;
        if (indep) {
            // chequeo si el actual es dominante
            for (int i = 0; i < n; i++) {
                // el i-esimo nodo está en actual ?
                if(find(actual.begin(), actual.end(), i) != actual.end()) {
                    // el i-esimo nodo está en actual
                    // sigo chequeando los siguientes nodos
                    continue;
                } else {
                    // el i-esimo nodo NO está en actual
                    // el i-esimo nodo es adyacente a alguno en actual ?
                    bool ady = false;
                    for (int j = 0; j < actual_size; j++) {
                        if (matriz[i][actual[j]] == 1) {
                            ady = true;
                            break;
                        }
                    }
                    if(!ady) {
                        dom = false;
                        break;
                    }

                }
            }
        }

        if(indep && dom && (actual_size < cidm_size || cidm_size == n)) {
            // si el conjunto actual es dominante e independiente y su cardinal es menor que el minimo, lo guardo como nuevo minimo
            cidm = actual;
            cidm_size = actual_size;
        }
    }

    for (int i = 0; i < actual_size; i++) {
        // copio el dominante actual y borro el i-esimo nodo de la copia
        vector<int> copia(actual);
        copia.erase(copia.begin() + i);
        vector<int> nuevo_cidm = resolver_aux(n, matriz, copia, cidm);
        if (nuevo_cidm.size() < cidm_size) {
            cidm = nuevo_cidm;
            cidm_size = nuevo_cidm.size();
        }
    }
    return cidm;
}
