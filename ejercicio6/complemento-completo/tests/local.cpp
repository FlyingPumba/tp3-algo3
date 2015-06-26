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

// Esta version tiene vecindades k-1 o k-k-1 vertices de diferencia
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

void bfs_cdi(Grafo& G, vector<int>& solucionInicial, int inicio);
void goloso(Grafo& G, vector<int>& solucionInicial);
bool solucion_posible(Grafo& G, vector<int>& solucionAuxiliar, int cantCambios);
void vecindad_primer_criterio(Grafo& G, vector<int>& solucionInicial);
void vecindad_segundo_criterio(Grafo& G, vector<int>& solucionInicial);
void imprimir_resultado(vector<int>& cidm);

// Implementacion.
int main() {

    FILE* file = fopen("grafo-complemento-local.dat","a+");

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

    std::chrono::time_point<std::chrono::high_resolution_clock> t1 = std::chrono::high_resolution_clock::now();


    vector<int> solucionInicial(n, NO_INCLUIDO);
    //Solucion Inicial 1: empiezo por el nodo 0
    //int inicio = 0;
    //bfs_cdi(G, solucionInicial, inicio);
    // Solucion Inicial 2: Heuristica golosa
    goloso(G, solucionInicial);
    //vecindad_primer_criterio(G, solucionInicial);

    vecindad_segundo_criterio(G, solucionInicial);

    vector<int> solucion;
    solucion.reserve(solucionInicial.size());
    for (int i = 0; i < solucionInicial.size(); i++) {
        if(solucionInicial[i] == INCLUIDO) {
            solucion.push_back(i+1);
        }
    }

    std::chrono::time_point<std::chrono::high_resolution_clock> t2 = std::chrono::high_resolution_clock::now();

    double d = double(std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1).count());

    fprintf(file, "%d %d %lu %7.8f\n",n, m, solucion.size() , d);

    //imprimir_resultado(solucionInicial);

    return 0;
}

void bfs_cdi(Grafo& G, vector<int>& solucionInicial, int inicio) {
  // O(m + n)
  int n = G.size();
	vector<int> estado(n,NO_VISITADO);
  // Primera componente conexa
  estado[inicio] = VISITADO;
  solucionInicial[inicio] = INCLUIDO;
  queue<int> cola;
  cola.push(inicio);

  while(!cola.empty()) {
    int v = cola.front();
    cola.pop();
    solucionInicial[v] = INCLUIDO;
    for (list<int>::iterator it=G[v].begin(); it != G[v].end(); ++it) {
      int w = *it;
      if (solucionInicial[w] == INCLUIDO) {
        solucionInicial[v] = NO_INCLUIDO;
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
    	solucionInicial[u] = INCLUIDO;
    	queue<int> cola;
    	cola.push(u);

    	while(!cola.empty()) {
    		int v = cola.front();
    		cola.pop();
    		solucionInicial[v] = INCLUIDO;
    		for (list<int>::iterator it=G[v].begin(); it != G[v].end(); ++it) {
    			int w = *it;
    			if (solucionInicial[w] == INCLUIDO) {
    				solucionInicial[v] = NO_INCLUIDO;
    			}
    			if (estado[w] == NO_VISITADO) {
    				estado[w] = VISITADO;
    				cola.push(w);
    			}
    		}
    	}
    }

  }
}

void goloso(Grafo& G, vector<int>& solucionInicial) {
  int n = G.size();
  Nodos nodos(n, Nodo());

  for(int u = 0; u < n; u++) {
      nodos[u].numero = u;
      nodos[u].grado = G[u].size();
  }

  sort(nodos.begin(), nodos.end(), orden());

  vector<bool> visitado(n, false);

  for(int u = 0; u < n ; u++){
      if(visitado[nodos[u].numero] == false){
        visitado[nodos[u].numero] = true;
        solucionInicial[nodos[u].numero] = INCLUIDO;
          for (list<int>::iterator itAdyU=G[nodos[u].numero].begin();
           itAdyU != G[nodos[u].numero].end(); ++itAdyU) {
              int v = *itAdyU;
              visitado[v] = true;

          }
      }
  }

}


void vecindad_primer_criterio(Grafo& G, vector<int>& solucionInicial) {
	// Criterio de Vecindad 1: Cambiamos k vertices por 1 vertice, donde k > 1
  int n = G.size();
  // Genero soluciones vecinas
  for (int u = 0; u < n; u++) {
    vector<int> solucionAuxiliar = solucionInicial;
    if (solucionInicial[u] == NO_INCLUIDO && G[u].size() > 1) {
      int cantINCLUIDOS = 0;
      solucionAuxiliar[u] = INCLUIDO;
      // Me fijo si el vectice NO INCLUIDO tiene al menos
      // dos vectores adyacentes INCLUIDOS
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
        bool esSolucion = solucion_posible(G, solucionAuxiliar, cantCambiosPosibles);
        if (esSolucion) {
          // Encontre una solucion Vecina mejor, fin del ciclo
          vecindad_primer_criterio(G, solucionAuxiliar);
          solucionInicial = solucionAuxiliar;
          break;
        }
      }

    }
  }
}

void vecindad_segundo_criterio(Grafo& G, vector<int>& solucionInicial) {
  // Criterio de Vecindad 2: Cambiamos k vertices por, a lo sumo,
  // k-1 vertices, , donde k > 1
  int n = G.size();
  // Genero soluciones vecinas
  for (int u = 0; u < n; u++) {
    vector<int> solucionAuxiliar = solucionInicial;
    if (solucionInicial[u] == NO_INCLUIDO && G[u].size() > 1) {
      // Para los INCLUIDOS en la solucionInicial me fijo en sus adyacentes
      // para encontrar algun adyacente que tambien esta INCLUIDO
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
        bool esSolucion = solucion_posible(G, solucionAuxiliar, cantCambiosPosibles);
        if(esSolucion){
          // Encontre una solucion Vecina mejor, fin del ciclo
          vecindad_segundo_criterio(G, solucionAuxiliar);
          solucionInicial = solucionAuxiliar;
          break;
        }
      }
    }
  }
}

bool solucion_posible(Grafo& G, vector<int>& solucionAuxiliar, int cantCambios) {

  int n = G.size();
  bool esSolucion = true;
  bool finCiclo = false;

  for (int u = 0; u < n && !finCiclo; u++) {
    // Si esta INCLUIDO, sus adyacentes no pueden estar INCLUIDOS
    if (solucionAuxiliar[u] == INCLUIDO && G[u].size() > 0) {
      for (list<int>::iterator itAdyU=G[u].begin(); itAdyU != G[u].end()
       && !finCiclo; ++itAdyU) {
        int v = *itAdyU;
        if (solucionAuxiliar[v] == INCLUIDO) {
          esSolucion = false;
          finCiclo = true;
        }
      }
    } else if (G[u].size() > 0) {
      // Si esta NO INCLUIDO, al menos uno de sus adyacentes
      // tiene que estar INCLUIDO
      bool adyINCLUIDO = false;
      for (list<int>::iterator itAdyU=G[u].begin(); itAdyU != G[u].end()
       && !finCiclo; ++itAdyU) {
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

void imprimir_resultado(vector<int>& cidm) {
    vector<int> solucion;
    solucion.reserve(cidm.size());
    for (int i = 0; i < cidm.size(); i++) {
        if(cidm[i] == INCLUIDO) {
            solucion.push_back(i+1);
        }
    }

    cout << solucion.size() << " ";
    for (int i = 0; i < solucion.size(); i++) {
        cout << solucion[i] << " ";
    }
    cout << endl;
}
