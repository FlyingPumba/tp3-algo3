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

// Esta version tiene vecindades de exactamente 2 o 3 vertices de diferencia
typedef list<int> listaAdyacencia;
typedef vector<listaAdyacencia> Grafo;

int mayor_grado(Grafo G);
vector<int> bfs_cdi(Grafo G, int inicio);
bool esSolucion(Grafo G, vector<int> solucionPosible);
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
    // Usar golosa?
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
  vector<int> domIndep(n,NO_MARCADO);
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

  return domIndep;
}

vector<int> vecindad_primer_criterio(Grafo G, vector<int> solucionInicial) {
	// Criterio de Vecindad 1: Exactamente dos nodos de diferencia
  //Las soluciones de la vecindad parten de marcar los nodos no marcados en la solucionInicial
  int n = G.size();
  // n+1 porque guardo en la posicion n la cantidad de nodos marcados
  vector<int> solucionAuxiliar = solucionInicial;
  // Genero soluciones vecinas y me quedo con la mejor
  bool finCiclo = false;
  for (int u = 0; u < n && !finCiclo; u++) {
    if (solucionInicial[u] == MARCADO && G[u].size() > 0) {
      // Para los marcados en la solucionInicial me fijo en sus adyacentes para encontrar algun adyacente que tambien esta marcado
      for (list<int>::iterator itAdyU=G[u].begin(); itAdyU != G[u].end() && !finCiclo; ++itAdyU) {
        int v = *itAdyU;
        for (list<int>::iterator itAdyV=G[v].begin(); itAdyV != G[v].end() && !finCiclo; ++itAdyV) {
          int w = *itAdyV;
          if (u < w && solucionInicial[w] == MARCADO) {
            // La solucionVecina va a diferir en al menos dos vertices con la original (u y w)
            solucionAuxiliar[u], solucionAuxiliar[w] = NO_MARCADO;
            solucionAuxiliar[v] = MARCADO;
            if (esSolucion(G, solucionAuxiliar)) {
              solucionInicial = vecindad_primer_criterio(G, solucionAuxiliar);
              finCiclo = true;
            } 
          }
        }
      }
    }
  }

	return solucionInicial;
}

vector<int> vecindad_segundo_criterio(Grafo G, vector<int> solucionInicial) {
  // Criterio de Vecindad 1: Exactamente dos nodos de diferencia
  //Las soluciones de la vecindad parten de marcar los nodos no marcados en la solucionInicial
  int n = G.size();
  // n+1 porque guardo en la posicion n la cantidad de nodos marcados
  vector<int> solucionAuxiliar = solucionInicial;
  // Genero soluciones vecinas y me quedo con la mejor
  bool finCiclo = false;
  for (int u = 0; u < n && !finCiclo; u++) {
    if (solucionInicial[u] == MARCADO && G[u].size() > 0) {
      // Para los marcados en la solucionInicial me fijo en sus adyacentes para encontrar algun adyacente que tambien esta marcado
      for (list<int>::iterator itAdyU=G[u].begin(); itAdyU != G[u].end() && !finCiclo; ++itAdyU) {
        int v = *itAdyU;
        int cantMarcados = 0;
        int z;
        for (list<int>::iterator itAdyV=G[v].begin(); itAdyV != G[v].end() && !finCiclo; ++itAdyV) {
          int w = *itAdyV;
          if (u < w && solucionInicial[w] == MARCADO) {
            if (cantMarcados == 0) {
              z = w;
              cantMarcados = 1;
            } else {
              // La solucionVecina va a diferir en al menos dos vertices con la original (u y w y z)
              solucionAuxiliar[u] = NO_MARCADO;
              solucionAuxiliar[w] = NO_MARCADO;
              solucionAuxiliar[z] = NO_MARCADO;
              solucionAuxiliar[v] = MARCADO;
              if (esSolucion(G, solucionAuxiliar)) {
                solucionInicial = vecindad_segundo_criterio(G, solucionAuxiliar);
                finCiclo = true;
              } 
            }

          }
        }
      }
    }
  }

  return solucionInicial;
}

bool esSolucion(Grafo G, vector<int> solucionPosible) {
  
  int n = G.size();
  bool esSolucion = true;
  bool finCiclo = false;
  
  for (int u = 0; u < n && !finCiclo; u++) {
    // Si esta MARCADO, sus adyacentes no pueden estar MARCADOS
    if (solucionPosible[u] == MARCADO && G[u].size() > 0) {
      for (list<int>::iterator itAdyU=G[u].begin(); itAdyU != G[u].end() && !finCiclo; ++itAdyU) {
        int v = *itAdyU;
        if (solucionPosible[v] == MARCADO) {
          esSolucion = false;
          finCiclo = true;
        }
      }
    } else if (G[u].size() > 0) {
      // Si esta NO MARCADO, al menos uno de sus adyacentes tiene que estar MARCADO
      bool adyMarcado = false;
      for (list<int>::iterator itAdyU=G[u].begin(); itAdyU != G[u].end() && !finCiclo; ++itAdyU) {
        int v = *itAdyU;
        if (solucionPosible[v] == MARCADO) {
          adyMarcado = true;
        }
      }
      if (!adyMarcado) {
        esSolucion = false;
        finCiclo = true;
      }
    } else {
      // Si es un K1, tiene que estar marcado
      esSolucion = solucionPosible[u] == MARCADO;
      finCiclo = !(solucionPosible[u] == MARCADO);
    }
  }

  return esSolucion;
}