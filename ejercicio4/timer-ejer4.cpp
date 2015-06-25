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
#define CANT_TESTS 500
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

void bfs_cdi(Grafo& G, vector<int>& solucionInicial, int inicio);
void goloso(Grafo& G, vector<int>& solucionInicial);
bool solucion_posible(Grafo& G, vector<int>& solucionAuxiliar, int cantCambios);
void vecindad_primer_criterio(Grafo& G, vector<int>& solucionInicial);
void vecindad_segundo_criterio(Grafo& G, vector<int>& solucionInicial);
void imprimir_resultado(vector<int>& cidm);

int random_in_range(int min, int max);
string to_string(int value);

int random_in_range(int min, int max) {
  return min + (rand() % (max - min + 1));
}


int main() {
    srand(time(0)); // use current time as seed for random generator
    FILE* file_in = fopen("tiempos.txt","w+");
    fprintf(file_in, "%s %s %s %s %s %s %s %s %s %s %s %s\n","#" , "n", "m", "n+m","tB1", "tamB1", "tB2", "tamB2", "tG1", "tamG1", "tG2", "tamG2");
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

        vector<int> solucionInicialB1(n, NO_INCLUIDO);
        bfs_cdi(G, solucionInicialB1, inicio);

        vecindad_primer_criterio(G, solucionInicialB1);

        int tamB1 = 0;
        for(int u = 0; u < n; u++) {
          if (solucionInicialB1[u] == INCLUIDO) {
            tamB1 ++;
          }
        }
        // terminar timer B1
        clock_t c_end = clock();
        int tB1= c_end-c_start;

        // iniciar timer B2
        c_start = clock();
        inicio = 0;

        vector<int> solucionInicialB2(n, NO_INCLUIDO);

        bfs_cdi(G, solucionInicialB2, inicio);

        vecindad_segundo_criterio(G, solucionInicialB2);

        int tamB2 = 0;
        for(int u = 0; u < n; u++) {
          if (solucionInicialB2[u] == INCLUIDO) {
            tamB2 ++;
          }
        }
        // terminar timer B2
        c_end = clock();
        int tB2 = c_end-c_start;

        // iniciar timer G1
        c_start = clock();

        vector<int> solucionInicialG1(n, NO_INCLUIDO);

        goloso(G, solucionInicialG1);

        vecindad_primer_criterio(G, solucionInicialG1);

        int tamG1 = 0;
        for(int u = 0; u < n; u++) {
          if (solucionInicialG1[u] == INCLUIDO) {
            tamG1 ++;
          }
        }
        // terminar timer G1
        c_end = clock();
        int tG1= c_end-c_start;

        // iniciar timer G2
        c_start = clock();

        vector<int> solucionInicialG2(n, NO_INCLUIDO);

        goloso(G, solucionInicialG2);

        vecindad_segundo_criterio(G, solucionInicialG2);

        int tamG2 = 0;
        for(int u = 0; u < n; u++) {
          if (solucionInicialG2[u] == INCLUIDO) {
            tamG2 ++;
          }
        }
        // terminar timer G2
        c_end = clock();
        int tG2 = c_end-c_start;
        int s = n+m;
        fprintf(file_in, "%d %d %d %d %d %d %d %d %d %d %d %d\n", c, n, m, s, tB1, tamB1, tB2, tamB2, tG1, tamG1, tG2, tamG2);

    }

    fclose(file_in);

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
          for (list<int>::iterator itAdyU=G[nodos[u].numero].begin(); itAdyU != G[nodos[u].numero].end(); ++itAdyU) {
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
  // Criterio de Vecindad 2: Cambiamos k vertices por, a lo sumo, k-1 vertices, , donde k > 1
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
