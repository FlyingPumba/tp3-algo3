#include <vector>
#include <iostream>
#include <algorithm> 
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
    int n, m;
    cin >> n;
    cin >> m;

    Matriz matriz(n, Vec(n, 0));
    Nodos nodos(n, Nodo());
    for(int i = 0; i < n; i++) {
        nodos[i].numero = i;
    }

    int v, w;
    for (int i = 0; i < m; i++) {
        cin >> v;
        cin >> w;
        matriz[v-1][w-1] = 1;
        matriz[w-1][v-1] = 1;
        nodos[v-1].grado = nodos[v-1].grado + 1;
        nodos[w-1].grado = nodos[w-1].grado + 1;
    }

    vector<int> cidm = resolver(n, matriz, nodos);

        cout << cidm.size() << " ";
    for (int i = 0; i < cidm.size(); i++) {
        cout << cidm[i]+1 << " ";
    }
    cout << endl;

    return 0;
}

vector<int> resolver(int n, Matriz matriz, Nodos nodos) {
    // Ordeno a los nodos segun su grado
    std::sort(nodos.begin(), nodos.end(), orden());

    bool visitado[sizeof nodos] = { false };
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




