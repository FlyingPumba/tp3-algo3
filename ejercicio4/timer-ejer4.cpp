#include <vector>
#include <iostream>
#include <stack>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <ctime>
#include <chrono>
#include <list>
#include <queue>
#include <algorithm>
using namespace std;

#define INFINITO -1
#define CANT_TESTS 200
#define CANT_NODOS_MIN 2
#define CANT_NODOS_MAX 50

#define VISITADO 1
#define NO_VISITADO 0
#define INCLUIDO 1
#define NO_INCLUIDO 0

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

int random_in_range(int min, int max);
string to_string(int value);

int random_in_range(int min, int max) {
  return min + (rand() % (max - min + 1));
}


int main() {
    srand(time(0)); // use current time as seed for random generator
    FILE* file_in = fopen("tiempos.txt","w+");
    fprintf(file_in, "%s %s %s %s %s %s %s %s %s %s %s\n","#" , "n", "m", "tB1", "tamB1", "tB2", "tamB2", "tG1", "tamG1", "tG2", "tamG2");
    for (int c = 0; c < CANT_TESTS; c++) {
        int n = random_in_range(CANT_NODOS_MIN, CANT_NODOS_MAX);
        int m = random_in_range(0, n*(n-1)/2);

        //cout << "TEST: " << c << ", n: " << n << ", m: " << m << " -> " << files[1] << endl;
        //fprintf(file_in, "%d %d\n", n, m);
        Grafo G(n, listaAdyacencia());

        for (int i = 0; i < m; i++) {
            int v, w;
            do  {
                v = random_in_range(1, n);
                w = random_in_range(1, n);
            } while (v == w || (find(G[v-1].begin(), G[v-1].end(), w-1) != G[v-1].end()));
            // cout << i << endl;
            G[v-1].push_back(w-1);
            G[w-1].push_back(v-1);
            //fprintf(file_in, "%d %d\n", v, w);

        }

        // iniciar timer B1
        clock_t c_start = clock();
        int inicio = 0;

        vector<int> solucionInicial = bfs_cdi(G, inicio);

        vector<int> cidm = vecindad_primer_criterio(G, solucionInicial);

        int tamB1 = 0;
        for(int u = 0; u < n; u++) {
          if (cidm[u] == INCLUIDO) {
            tamB1 ++;
          }
        }
        // terminar timer B1
        clock_t c_end = clock();
        int tB1= c_end-c_start;

        // iniciar timer B2
        c_start = clock();
        inicio = 0;

        solucionInicial = bfs_cdi(G, inicio);

        cidm = vecindad_segundo_criterio(G, solucionInicial);

        int tamB2 = 0;
        for(int u = 0; u < n; u++) {
          if (cidm[u] == INCLUIDO) {
            tamB2 ++;
          }
        }
        // terminar timer B2
        c_end = clock();
        int tB2 = c_end-c_start;

        // iniciar timer G1
        c_start = clock();

        solucionInicial = goloso(G);

        cidm = vecindad_primer_criterio(G, solucionInicial);

        int tamG1 = 0;
        for(int u = 0; u < n; u++) {
          if (cidm[u] == INCLUIDO) {
            tamG1 ++;
          }
        }
        // terminar timer G1
        c_end = clock();
        int tG1= c_end-c_start;

        // iniciar timer G2
        c_start = clock();

        solucionInicial = goloso(G);

        cidm = vecindad_segundo_criterio(G, solucionInicial);

        int tamG2 = 0;
        for(int u = 0; u < n; u++) {
          if (cidm[u] == INCLUIDO) {
            tamG2 ++;
          }
        }
        // terminar timer G2
        c_end = clock();
        int tG2 = c_end-c_start;

        fprintf(file_in, "%d %d %d %d %d %d %d %d %d %d %d\n", c, n, m, tB1, tamB1, tB2, tamB2, tG1, tamG1, tG2, tamG2);

    }

    fclose(file_in);

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
  // Criterio de Vecindad 1: Cambiamos, al  menos,  tres vectices de la solucion inicial por dos
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
