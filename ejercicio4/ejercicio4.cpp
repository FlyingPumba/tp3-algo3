#include <vector>
#include <iostream>
#include <stack>
#include <list>
#include <queue>
#include <algorithm>
using namespace std;

#define VISITADO 1
#define NO_VISITADO 0
#define INCLUIDO 1
#define NO_INCLUIDO 0

// Esta version tiene vecindades 2-1 o 3-2 vertices de diferencia
struct Nodo {
    int numero;
    int grado;
    Nodo() : numero(0), grado(0) {};
    Nodo(const int n, const int g) : numero(n) , grado(g) {};
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

vector<int> bfs_cdi(Grafo G, int inicio);
vector<int> goloso(Grafo G);
bool solucionPosible(Grafo G, vector<int>* solucionCambiar, int cantCambios);
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
    //Solucion Inicial 1: empiezo por el nodo 0
    int inicio = 0;
    //vector<int> solucionInicial = bfs_cdi(G, inicio);
    // Solucion Inicial 2: Heuristica golosa
    vector<int> solucionInicial = goloso(G);

    //vector<int> cidm = vecindad_primer_criterio(G, solucionInicial);

    vector<int> cidm = vecindad_segundo_criterio(G, solucionInicial);


    cout << "[";
    for (int i = 0; i < cidm.size(); i++) {
        if(cidm[i] == INCLUIDO) {
          cout << i + 1 << ",";
        }

    }
    cout << "]"<< endl;
    /*tamano = 0;
    for (int i = 0; i < solucionInicial.size(); i++) {
      if (cidm[i] == INCLUIDO) {
        tamano ++;
      }
    }
    cout << tamano << endl;*/

    return 0;
}

vector<int> bfs_cdi(Grafo G, int inicio) {
  // O(m + n)
  int n = G.size();
	vector<int> estado(n,NO_VISITADO);
  vector<int> domIndep(n,NO_INCLUIDO);
  // Primera componente conexa
  estado[inicio] = VISITADO;
  domIndep[inicio] = INCLUIDO;
  queue<int> cola;
  cola.push(inicio);

  while(!cola.empty()) {
    int v = cola.front();
    cola.pop();
    domIndep[v] = INCLUIDO;
    for (list<int>::iterator it=G[v].begin(); it != G[v].end(); ++it) {
      int w = *it;
      if (domIndep[w] == INCLUIDO) {
        domIndep[v] = NO_INCLUIDO;
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
    	domIndep[u] = INCLUIDO;
    	queue<int> cola;
    	cola.push(u);

    	while(!cola.empty()) {
    		int v = cola.front();
    		cola.pop();
    		domIndep[v] = INCLUIDO;
    		for (list<int>::iterator it=G[v].begin(); it != G[v].end(); ++it) {
    			int w = *it;
    			if (domIndep[w] == INCLUIDO) {
    				domIndep[v] = NO_INCLUIDO;
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

vector<int> goloso(Grafo G) {
  int n = G.size();
  Nodos nodos(n, Nodo());

  for(int u = 0; u < n; u++) {
    nodos[u].numero = u;
  }
  for(int u = 0; u < n; u++) {
    for (list<int>::iterator itAdyU=G[u].begin(); itAdyU != G[u].end(); ++itAdyU) {
      int v = *itAdyU;
      nodos[u].grado = nodos[u].grado + 1;
      nodos[v].grado = nodos[v].grado + 1;
    }
  }

  sort(nodos.begin(), nodos.end(), orden());

  vector<int> solucionInicial(n, NO_INCLUIDO);
  vector<bool> visitado(n, false);

  for(int u = 0; u < n ; u++){
      if(visitado[nodos[u].numero] == false){
        visitado[nodos[u].numero] = true;
        solucionInicial[nodos[u].numero] = INCLUIDO;
          for (list<int>::iterator itAdyU=G[nodos[u].numero].begin(); itAdyU != G[nodos[u].numero].end(); ++itAdyU) {
              int v = *itAdyU;
              visitado[v] = true;

          }
      }
  }

  return solucionInicial;
}


vector<int> vecindad_primer_criterio(Grafo G, vector<int> solucionInicial) {
	// Criterio de Vecindad 1: Cambiamos, al  menos,  dos vectices de la solucion inicial por uno
  int n = G.size();
  // Genero soluciones vecinas
  for (int u = 0; u < n; u++) {
    vector<int> solucionAuxiliar = solucionInicial;
    if (solucionInicial[u] == NO_INCLUIDO && G[u].size() > 1) {
      int cantINCLUIDOS = 0;
      solucionAuxiliar[u] = INCLUIDO;
      // Me fijo si el vectice NO INCLUIDO tiene al menos dos vectores adyacentes INCLUIDOS
      for (list<int>::iterator itAdyU=G[u].begin(); itAdyU != G[u].end(); ++itAdyU) {
        int v = *itAdyU;
        if (solucionInicial[v] == INCLUIDO) {
          cantINCLUIDOS ++;
          solucionAuxiliar[v] = NO_INCLUIDO;
        }
      }
      // Necesito al menos 2 INCLUIDOS
      if (cantINCLUIDOS > 1) {
        int cantCambiosPosibles = 0;
        bool esSolucion = solucionPosible(G, &solucionAuxiliar, cantCambiosPosibles);
        if (esSolucion) {
          // Encontre una solucion Vecina mejor, fin del ciclo
          solucionInicial = vecindad_primer_criterio(G, solucionAuxiliar);
          break;
        }
      }

    }
  }
	return solucionInicial;
}

vector<int> vecindad_segundo_criterio(Grafo G, vector<int> solucionInicial) {
  // Criterio de Vecindad 2: Cambiamos, al  menos,  tres vectices de la solucion inicial por dos
  int n = G.size();
  // Genero soluciones vecinas
  for (int u = 0; u < n; u++) {
    vector<int> solucionAuxiliar = solucionInicial;
    if (solucionInicial[u] == NO_INCLUIDO && G[u].size() > 1) {
      // Para los INCLUIDOS en la solucionInicial me fijo en sus adyacentes para encontrar algun adyacente que tambien esta INCLUIDO
      int cantINCLUIDOS = 0;
      solucionAuxiliar[u] = INCLUIDO;
      for (list<int>::iterator itAdyU=G[u].begin(); itAdyU != G[u].end(); itAdyU++) {
        int v = *itAdyU;
        if (solucionInicial[v] == INCLUIDO) {
          cantINCLUIDOS ++;
          solucionAuxiliar[v] = NO_INCLUIDO;
        }

      }
      // Necesito al menos 2 INCLUIDOS
      if (cantINCLUIDOS > 1) {
        int cantCambiosPosibles = cantINCLUIDOS - 2;
        bool esSolucion = solucionPosible(G, &solucionAuxiliar, cantCambiosPosibles);
        //cout <<  "Probe nodo: " << u + 1 << endl;
        if(esSolucion){
          // Cambios necesarios para que sea solucion
          //cout <<  "Exito" << endl;
          // Encontre una solucion Vecina mejor, fin del ciclo
          solucionInicial = vecindad_segundo_criterio(G, solucionAuxiliar);
          break;
        }
      }
    }
  }
	return solucionInicial;
}

bool solucionPosible(Grafo G, vector<int>* solucionCambiar, int cantCambios) {

  int n = G.size();
  bool esSolucion = true;
  bool finCiclo = false;
  list<int> verticesCambiados;
  vector<int>& solucionAuxiliar = (*solucionCambiar);

  for (int u = 0; u < n && !finCiclo; u++) {
    // Si esta INCLUIDO, sus adyacentes no pueden estar INCLUIDOS
    if (solucionAuxiliar[u] == INCLUIDO && G[u].size() > 0) {
      for (list<int>::iterator itAdyU=G[u].begin(); itAdyU != G[u].end() && !finCiclo; ++itAdyU) {
        int v = *itAdyU;
        if (solucionAuxiliar[v] == INCLUIDO) {
          esSolucion = false;
          finCiclo = true;
        }
      }
    } else if (G[u].size() > 0) {
      // Si esta NO INCLUIDO, al menos uno de sus adyacentes tiene que estar INCLUIDO
      bool adyINCLUIDO = false;
      for (list<int>::iterator itAdyU=G[u].begin(); itAdyU != G[u].end() && !finCiclo; ++itAdyU) {
        int v = *itAdyU;
        if (solucionAuxiliar[v] == INCLUIDO) {
          adyINCLUIDO = true;
        }
      }
      if (!adyINCLUIDO && cantCambios == 0) {
        esSolucion = false;
        finCiclo = true;
      } else if(!adyINCLUIDO) {
        // Salvo la solucion al marcar el vertice
        solucionAuxiliar[u] = INCLUIDO;
        cantCambios --;
      }
    } else {
      // Si es un K1, tiene que estar INCLUIDO
      esSolucion = solucionAuxiliar[u] == INCLUIDO;
      finCiclo = !(solucionAuxiliar[u] == INCLUIDO);
    }
  }

  return esSolucion;
}
