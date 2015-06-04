#include <vector>
#include <iostream>
#include <stack>
#include <list>
#include <queue>
using namespace std;

#define VISITADO 1
#define NO_VISITADO 0
#define MARCADO 1
#define NO_MARCADO 0

// NO ESTA TERMINADO, NO LO PROBE
typedef list<int> listaAdyacencia;
typedef vector<listaAdyacencia> Grafo;

vector<int> bfs_cdi(Grafo G, int v);
vector<int> vecindad_primer_criterio(Grafo G, vector<int> solucionInicial);
vector<int> vecindad_segundo_criterio(Grafo G, vector<int> solucionInicial);

// Implementacion.
int main() {
    int n, m;
    cin >> n;
    cin >> m;

    Grafo G(n, listaAdyacencia(m));

    int v, w;
    for (int i = 0; i < m; i++) {
        cin >> v;
        cin >> w;
        G[v-1].push_back(w-1);
        G[w-1].push_back(v-1);
    }
    
    vector<int> solucionInicial = bfs_cdi(G, v);
    
    vector<int> cidm = vecindad_primer_criterio(G, solucionInicial);

    //vector<int> cidm = vecindad_segundo_criterio(G, solucionInicial);

    
    cout << "[";
    for (int i = 0; i < cidm.size() - 1; i++) {
        cout << cidm[i] << ",";
    }
    cout <<  cidm[cidm.size() - 1] << "]" << endl;
	
    return 0;
}

vector<int> bfs_cdi(Grafo G, int v) {
    // O(m + n)?
    int n = G.size();
	vector<int> estado(n,NO_VISITADO);
	vector<int> domIndep(n,NO_MARCADO);
    // Puede tener varias componentes conexas
    for (int u = 0; u < n; u++) {
    	if (estado[u] == NO_VISITADO) {
    		estado[u] = VISITADO;
    		domIndep[u] = MARCADO;
    		queue<int> cola;
    		cola.push(u);

    		while(!cola.empty()) {
    			int v = cola.front(); 
    			cola.pop();
    			domIndep[v] = MARCADO;
    			for (list<int>::iterator it=G[v].begin(); it != G[v].end(); ++it) {
    				int w = *it;
    				if (domIndep[w] == MARCADO) {
    					domIndep[v] = NO_MARCADO;
    				}
    				if (estado[w] == NO_VISITADO) {
    					estado[w] = VISITADO;
    					cola.push(w);
    				}
    			}
    		}
    	}

    }

    return domIndep;
}

vector<int> vecindad_primer_criterio(Grafo G, vector<int> solucionInicial) {
	// Criterio de Vecindad 1: 




	return solucionInicial;


}

vector<int> vecindad_segundo_criterio(Grafo G, vector<int> solucionInicial) {
	// Criterio de Vecindad 2:




	return solucionInicial;


}

