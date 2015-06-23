#include <vector>
#include <iostream>
#include <stack>
#include <list>
#include <queue>
#include <utility>
using namespace std;

#define VISITADO 1
#define NO_VISITADO 0
#define MARCADO 1
#define NO_MARCADO 0

// Esta version tiene vecindades 2-1 o 3-2 vertices de diferencia
typedef list<int> listaAdyacencia;
typedef vector<listaAdyacencia> Grafo;

vector<int> bfs_cdi(Grafo G, int inicio);
pair<bool, list<int> > solucionPosible(Grafo G, vector<int> solucionCambiar, int cantCambios);
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
    vector<int> solucionInicial = bfs_cdi(G, inicio);

    cout << "[";
    for (int i = 0; i < solucionInicial.size() - 1; i++) {
        cout << solucionInicial[i] << ",";
    }
    cout <<  solucionInicial[solucionInicial.size() - 1] << "] ";
    int tamano = 0;
    for (int i = 0; i < solucionInicial.size(); i++) {
      if (solucionInicial[i] == MARCADO) {
        tamano ++;
      }
    }
    cout << tamano << endl;
    // Solucion Inicial 2: Usar heuristica golosa

    //vector<int> cidm = vecindad_primer_criterio(G, solucionInicial);

    vector<int> cidm = vecindad_segundo_criterio(G, solucionInicial);


    cout << "[";
    for (int i = 0; i < cidm.size() - 1; i++) {
        cout << cidm[i] << ",";
    }
    cout <<  cidm[cidm.size() - 1] << "] ";
    tamano = 0;
    for (int i = 0; i < solucionInicial.size(); i++) {
      if (cidm[i] == MARCADO) {
        tamano ++;
      }
    }
    cout << tamano << endl;

    return 0;
}

vector<int> bfs_cdi(Grafo G, int inicio) {
  // O(m + n)
  int n = G.size();
	vector<int> estado(n,NO_VISITADO);
  vector<int> domIndep(n,NO_MARCADO);
  // Primera componente conexa
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
	// Criterio de Vecindad 1: Cambiamos, al  menos,  dos vectices de la solucion inicial por uno
  int n = G.size();
  vector<int> solucionAuxiliar = solucionInicial;
  // Genero soluciones vecinas
  for (int u = 0; u < n; u++) {
    if (solucionInicial[u] == NO_MARCADO && G[u].size() > 1) {
      int cantMarcados = 0;
      solucionAuxiliar[u] = MARCADO;
      // Me fijo si el vectice NO MARCADO tiene al menos dos vectores adyacentes MARCADOS
      for (list<int>::iterator itAdyU=G[u].begin(); itAdyU != G[u].end(); ++itAdyU) {
        int v = *itAdyU;
        if (solucionInicial[v] == MARCADO) {
          cantMarcados ++;
          solucionAuxiliar[v] = NO_MARCADO;
        }
      }
      // Necesito al menos 2 MARCADOS
      if (cantMarcados > 1) {
        int cantCambiosPosibles = 0;
        pair<bool, list<int> >  esSolucion = solucionPosible(G, solucionAuxiliar, cantCambiosPosibles);
        if (esSolucion.first) {
          // Encontre una solucion Vecina mejor, fin del ciclo
          solucionInicial = vecindad_primer_criterio(G, solucionAuxiliar);
          break;
        } else {
          // No es solucion, reinicio los valores originales
          solucionAuxiliar[u] = NO_MARCADO;
          for (list<int>::iterator itAdyU=G[u].begin(); itAdyU != G[u].end(); ++itAdyU) {
            int v = *itAdyU;
            if (solucionInicial[v] == MARCADO) {
              solucionAuxiliar[v] = MARCADO;
            }
          }
          }
        }

      }
    }

	return solucionInicial;
}

vector<int> vecindad_segundo_criterio(Grafo G, vector<int> solucionInicial) {
  // Criterio de Vecindad 1: Cambiamos, al  menos,  tres vectices de la solucion inicial por dos
  int n = G.size();
  vector<int> solucionAuxiliar = solucionInicial;
  // Genero soluciones vecinas
  for (int u = 0; u < n; u++) {
    if (solucionInicial[u] == NO_MARCADO && G[u].size() > 1) {
      // Para los marcados en la solucionInicial me fijo en sus adyacentes para encontrar algun adyacente que tambien esta marcado
      int cantMarcados = 0;
      int w = n;
      solucionAuxiliar[u] = MARCADO;
      for (list<int>::iterator itAdyU=G[u].begin(); itAdyU != G[u].end(); ++itAdyU) {
        int v = *itAdyU;
        if (solucionInicial[v] == MARCADO) {
          cantMarcados ++;
          solucionAuxiliar[v] = NO_MARCADO;
        }

      }
      // Necesito al menos 3 MARCADOS
      if (cantMarcados > 1) {
        int cantCambiosPosibles = cantMarcados - 2;
        pair<bool, list<int> > esSolucion = solucionPosible(G, solucionAuxiliar, cantCambiosPosibles);
        cout <<  "Probe nodo: " << u + 1 << endl;
        if(esSolucion.first){
          // Cambios necesarios para que sea solucion
          cout <<  "Exito" << endl;
          for (list<int>::iterator itAdyCambiados=(esSolucion.second).begin(); itAdyCambiados != (esSolucion.second).end(); ++itAdyCambiados) {
            int v = *itAdyCambiados;
              solucionAuxiliar[v] = MARCADO;
          }
          // Encontre una solucion Vecina mejor, fin del ciclo
          solucionInicial = vecindad_segundo_criterio(G, solucionAuxiliar);
          break;
          }

        }
        // No es solucion, reinicio los valores originales
        solucionAuxiliar[u] = NO_MARCADO;
        for (list<int>::iterator itAdyU=G[u].begin(); itAdyU != G[u].end(); ++itAdyU) {
          int v = *itAdyU;
          if (solucionInicial[v] == MARCADO) {
              solucionAuxiliar[v] = MARCADO;
          }
        }

      }

    }

	return solucionInicial;
}


pair<bool, list<int> > solucionPosible(Grafo G, vector<int> solucionCambiar, int cantCambios) {

  int n = G.size();
  bool esSolucion = true;
  bool finCiclo = false;
  list<int> verticesCambiados;
  vector<int> solucionAuxiliar = solucionCambiar;

  for (int u = 0; u < n && !finCiclo; u++) {
    // Si esta MARCADO, sus adyacentes no pueden estar MARCADOS
    if (solucionAuxiliar[u] == MARCADO && G[u].size() > 0) {
      for (list<int>::iterator itAdyU=G[u].begin(); itAdyU != G[u].end() && !finCiclo; ++itAdyU) {
        int v = *itAdyU;
        if (solucionAuxiliar[v] == MARCADO) {
          esSolucion = false;
          finCiclo = true;
        }
      }
    } else if (G[u].size() > 0) {
      // Si esta NO MARCADO, al menos uno de sus adyacentes tiene que estar MARCADO
      bool adyMarcado = false;
      for (list<int>::iterator itAdyU=G[u].begin(); itAdyU != G[u].end() && !finCiclo; ++itAdyU) {
        int v = *itAdyU;
        if (solucionAuxiliar[v] == MARCADO) {
          adyMarcado = true;
        }
      }
      if (!adyMarcado && cantCambios == 0) {
        esSolucion = false;
        finCiclo = true;
      } else if(!adyMarcado) {
        // Salvo la solucion al marcar el vertice
        solucionAuxiliar[u] = MARCADO;
        verticesCambiados.push_front(u);
        cantCambios --;
      }
    } else {
      // Si es un K1, tiene que estar marcado
      esSolucion = solucionAuxiliar[u] == MARCADO;
      finCiclo = !(solucionAuxiliar[u] == MARCADO);
    }
  }

  return make_pair(esSolucion, verticesCambiados);
}
