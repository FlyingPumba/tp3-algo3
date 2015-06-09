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

    vector<int> cidm = resolver(n, matriz);

    cout << "[";
    for (int i = 0; i < cidm.size() - 1; i++) {
        cout << cidm[i]+1 << ",";
    }
    cout <<  cidm[cidm.size() - 1]+1 << "]" << endl;

    return 0;
}

vector<int> resolver(int n, Matriz matriz) {
    // inicializo el primer conjunto a evaluar con todos los vertices del grafo
    // Este primer conjunto es dominante (pero no independiente. A menos que n = 1)
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

        if(indep && dom_size < cidm_size) {
            // si dom es independiente y su cardinal es menor que el minimo, lo guardo como nuevo minimo
            cidm = dom;
            cidm_size = dom_size;
        }
    }

    for (int i = 0; i < dom_size; i++) {
        // copio dom y borro el i-esimo nodo de la copia
        vector<int> copia(dom);
        copia.erase(copia.begin() + i);
        // chequeo si la copia es dominante
        bool copia_dominante = true;
        for (int i = 0; i < n; i++) {
            // el i-esimo nodo está en copia ?
            if(find(copia.begin(), copia.end(), i) != copia.end()) {
                // el i-esimo nodo está en copia
                // sigo chequeando los siguientes nodos
                continue;
            } else {
                // el i-esimo nodo NO está en copia
                // el i-esimo nodo es adyacente a alguno en copia ?
                bool ady = false;
                for (int j = 0; j < copia.size(); j++) {
                    if (matriz[i][copia[j]] == 1) {
                        ady = true;
                        break;
                    }
                }
                if(!ady) {
                    copia_dominante = false;
                    break;
                }
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
