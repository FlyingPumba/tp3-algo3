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


typedef list<int> listaAdyacencia;
typedef vector<listaAdyacencia> Grafo;

int mayor_grado(Grafo G);
vector<int> bfs_cdi(Grafo G, int inicio);
vector<int> vecindad_primer_criterio(Grafo G, vector<int> solucionInicial);
vector<int> vecindad_segundo_criterio(Grafo G, vector<int> solucionInicial);

// Implementacion.
int main() {
    int n, m;
    cin >> n;
    cin >> m;

    Grafo G(n, listaAdyacencia());

    int v, w;
    for (int i = 0; i < m; i++) {
        cin >> v;
        cin >> w;
        G[v-1].push_back(w-1);
        G[w-1].push_back(v-1);
    }
    // Solucion Inicial 1: empiezo por el nodo 0
    //int inicio = 0;
    //vector<int> solucionInicial = bfs_cdi(G, inicio);

    // Solucion Inicial 2: empiezo por el nodo con mayor grado
    int inicio = mayor_grado(G);
    vector<int> solucionInicial = bfs_cdi(G, inicio);

    //vector<int> cidm = vecindad_primer_criterio(G, solucionInicial);

    vector<int> cidm = vecindad_segundo_criterio(G, solucionInicial);


    cout << "[";
    for (int i = 0; i < cidm.size() - 1; i++) {
        cout << cidm[i] << ",";
    }
    cout <<  cidm[cidm.size() - 1] << "]" << endl;

    return 0;
}

int mayor_grado(Grafo G) {
  int n = G.size();
  int mayorGrado = 0;
  for (int u = 0; u < n; u++) {
    if (G[mayorGrado].size() < G[u].size()) {
      mayorGrado = u;
    }
  }

  return mayorGrado;
}

vector<int> bfs_cdi(Grafo G, int inicio) {
  // O(m + n)?
  int n = G.size();
	vector<int> estado(n,NO_VISITADO);
  // n+1 porque guardo en la posicion n la cantidad de nodos marcados
  vector<int> domIndep(n+1,NO_MARCADO);
  // Primera componete conexa
  estado[inicio] = VISITADO;
  domIndep[inicio] = MARCADO;
  queue<int> cola;
  cola.push(inicio);

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
  // Resto de las componentes conexas
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
  // Calculo la cardinalidad del subconjunto de nodos marcados y lo guardo en la posicion n
  int nodosMarcados = 0;
  for (int u = 0; u < n; u++) {
    nodosMarcados += domIndep[u];
  }
  domIndep[n] = nodosMarcados;

  return domIndep;
}

vector<int> vecindad_primer_criterio(Grafo G, vector<int> solucionInicial) {
	// Criterio de Vecindad 1: Al menos 1 vertice de diferencia
  //Las soluciones de la vecindad parten de marcar los nodos no marcados en la solucionInicial
  int n = G.size();
  // n+1 porque guardo en la posicion n la cantidad de nodos marcados
  vector<int> mejorVecino(n+1, MARCADO);
  mejorVecino[n] = n;
  // Genero soluciones vecinas y me quedo con la mejor
  for (int u = 0; u < n; u++) {
    if (solucionInicial[u] == NO_MARCADO) {
      vector<int> solucionVecina = bfs_cdi(G, u);
      if (solucionVecina[n] < mejorVecino[n]) {
        mejorVecino = solucionVecina;
      }
    }
  }
  // Si la solucion vecina es mejor a la inicial, entonces hago otra iteracion
  if (mejorVecino[n] < solucionInicial[n]) {
    solucionInicial = vecindad_primer_criterio(G, mejorVecino);
  }

	return solucionInicial;
}

vector<int> vecindad_segundo_criterio(Grafo G, vector<int> solucionInicial) {
	// Criterio de Vecindad 2: Al menos 2 vertices de diferencia
  // Busco los vertices marcados en la solucionInicial que comparten un adyacente
  int n = G.size();
  // n+1 porque guardo en la posicion n la cantidad de nodos marcados
  vector<int> mejorVecino(n+1, MARCADO);
  mejorVecino[n] = n;
  // Genero soluciones vecinas y me quedo con la mejor
  for (int u = 0; u < n; u++) {
    if (solucionInicial[u] == MARCADO && G[u].size() > 0) {
      // Para los marcados en la solucionInicial me fijo en sus adyacentes para encontrar algun adyacente que tambien esta marcado
      for (list<int>::iterator it1=G[u].begin(); it1 != G[u].end(); ++it1) {
        int v = *it1;
        for (list<int>::iterator it2=G[v].begin(); it2 != G[v].end(); ++it2) {
          int w = *it2;
          if (u < w && solucionInicial[w] == MARCADO) {
            // La solucionVecina va a diferir en al menos dos vertices con la original (u y w)
            vector<int> solucionVecina = bfs_cdi(G, v);
            if (solucionVecina[n] < mejorVecino[n]) {
              mejorVecino = solucionVecina;
            } 
          }
        }
      }
    }
  }
  // Si la solucion vecina es mejor a la inicial, entonces hago otra iteracion
  if (mejorVecino[n] < solucionInicial[n]) {
    solucionInicial = vecindad_segundo_criterio(G, mejorVecino);
  }

  return solucionInicial;

}