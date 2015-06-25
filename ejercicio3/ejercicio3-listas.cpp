#include <vector>
#include <iostream>
#include <algorithm>
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

    vector<int> cidm = resolver(n, G, nodos);

        cout << cidm.size() << " ";
    for (int i = 0; i < cidm.size(); i++) {
        cout << cidm[i]+1 << " ";
    }
    cout << endl;


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
