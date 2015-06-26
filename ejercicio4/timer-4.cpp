// INCLUDES
#include <vector>
#include <iostream>
#include <algorithm>
#include <stack>
#include <list>
#include <queue>
#include <stdlib.h>
#include <stdio.h>

#include <ctime>
#include <chrono>
#include <dirent.h>
#include <string.h>
#include <sstream>
using namespace std;

// DEFINES
#define VISITADO 1
#define NO_VISITADO 0
#define INCLUIDO 1
#define NO_INCLUIDO 0

// ESTRUCTURAS
struct Nodo {
    int numero; //indica el indice con el que fue creado
    int grado;
    Nodo() : numero(0), grado(0) {};
    Nodo(const int n, const int g) : numero(n) , grado(g) {};
};

struct orden {
    inline bool operator() (const Nodo& n1, const Nodo& n2) {
        return (n1.grado > n2.grado);
    }
};

// TYPEDEF
typedef vector<Nodo> Nodos;
typedef list<int> listaAdyacencia;
typedef vector<listaAdyacencia> Grafo;

// FUNCIONES
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

// Experimentacion

typedef vector<int> Vec;
typedef vector<Vec> Matriz;

void medir_tiempos(ofstream &data_file, int n, int m, Matriz matriz);
void start_timer();
double stop_timer();

static chrono::time_point<chrono::high_resolution_clock> start_time;

// Implementacion.
int main() {

    FILE* file_in_n_m = fopen("tiemposnm.txt","w+");
    FILE* file_in_n = fopen("tiemposn.txt","w+");
    FILE* file_in_m = fopen("tiemposm.txt","w+");

    fprintf(file_in_n_m, "nm t_B1 t_G1 t_B2 t_G2\n"); // n_v111 n_v112 n_v121 n_v122 n_v211 n_v212 n_v221 n_v222\n");
    fprintf(file_in_n, "n t_B1 t_G1 t_B2 t_G2\n"); // n_v111 n_v112 n_v121 n_v122 n_v211 n_v212 n_v221 n_v222\n");
    fprintf(file_in_m, "m t_B1 t_G1 t_B2 t_G2\n"); // n_v111 n_v112 n_v121 n_v122 n_v211 n_v212 n_v221 n_v222\n");

    int nodos_start = 5;
    int nodos_end = 30;
    int muestras = 10;

    vector< vector<double> > tiempos_n_m(4, vector<double>(1+nodos_end+(nodos_end*(nodos_end-1))/2, 0));
    vector< vector<double> > tiempos_n(4, vector<double>(1+nodos_end, 0));
    vector< vector<double> > tiempos_m(4, vector<double>(1+(nodos_end*(nodos_end-1))/2, 0));

    vector<double> veces_n_m(1+nodos_end+(nodos_end*(nodos_end-1))/2, 0);
    vector<double> veces_n(1+nodos_end, 0);
    vector<double> veces_m(1+(nodos_end*(nodos_end-1))/2, 0);

    for (int nodos = nodos_start; nodos <= nodos_end; nodos++) {
        cout << nodos << endl;
        for (int ejes = 0; ejes <= (nodos*(nodos-1))/2; ejes++) {
            for (int muestra = 0; muestra < muestras; muestra++) {

                // Construir grafo
                Grafo G(nodos, listaAdyacencia());
                for (int i = 0; i < ejes; i++) {
                    int v, w;
                    do  {
                        v = random_in_range(1, nodos);
                        w = random_in_range(1, nodos);
                    } while (v == w || (find(G[v-1].begin(), G[v-1].end(), w-1) != G[v-1].end()));
                    G[v-1].push_back(w-1);
                    G[w-1].push_back(v-1);
                }

                // Medir tiempos
                vector<int> B1(nodos,NO_INCLUIDO);
                double tiempo;
                int inicio = 0;

                start_timer();
                bfs_cdi(G, B1, inicio);
                vecindad_primer_criterio(G, B1);
                tiempo = stop_timer();

                tiempos_n_m[0][nodos+ejes] += tiempo;
                tiempos_n[0][nodos] += tiempo;
                tiempos_m[0][ejes] += tiempo;

                vector<int> B2(nodos,NO_INCLUIDO);
                start_timer();
                bfs_cdi(G, B2, inicio);
                vecindad_segundo_criterio(G, B2);
                tiempo = stop_timer();

                tiempos_n_m[2][nodos+ejes] += tiempo;
                tiempos_n[2][nodos] += tiempo;
                tiempos_m[2][ejes] += tiempo;

                vector<int> G1(nodos,NO_INCLUIDO);
                start_timer();
                goloso(G, G1);
                vecindad_primer_criterio(G, G1);
                tiempo = stop_timer();

                tiempos_n_m[1][nodos+ejes] += tiempo;
                tiempos_n[1][nodos] += tiempo;
                tiempos_m[1][ejes] += tiempo;

                vector<int> G2(nodos,NO_INCLUIDO);
                start_timer();
                goloso(G, G2);
                vecindad_segundo_criterio(G, G2);
                tiempo = stop_timer();

                tiempos_n_m[3][nodos+ejes] += tiempo;
                tiempos_n[3][nodos] += tiempo;
                tiempos_m[3][ejes] += tiempo;

                veces_n_m[nodos+ejes] = veces_n_m[nodos+ejes]+1;
                veces_n[nodos] = veces_n[nodos]+1;
                veces_m[ejes] = veces_m[ejes]+1;

            }
        }
    }

    for (int index = 0; index <= nodos_end + (nodos_end*(nodos_end-1))/2; index++) {
        if (veces_n_m[index] > 0){
            fprintf(file_in_n_m, "%d %.0f %.0f %.0f %.0f\n", index,
                tiempos_n_m[0][index]/veces_n_m[index], tiempos_n_m[1][index]/veces_n_m[index],
                tiempos_n_m[2][index]/veces_n_m[index], tiempos_n_m[3][index]/veces_n_m[index]);
        }
    }

    for (int index = 0; index <= nodos_end; index++) {
        if (veces_n[index] > 0){
            fprintf(file_in_n, "%d %.0f %.0f %.0f %.0f\n", index,
                tiempos_n[0][index]/veces_n[index], tiempos_n[1][index]/veces_n[index],
                tiempos_n[2][index]/veces_n[index], tiempos_n[3][index]/veces_n[index]);
        }
    }

    for (int index = 0; index <= (nodos_end*(nodos_end-1))/2; index++) {
        if (veces_m[index] > 0){
            fprintf(file_in_m, "%d %.0f %.0f %.0f %.0f\n", index,
                tiempos_m[0][index]/veces_m[index], tiempos_m[1][index]/veces_m[index],
                tiempos_m[2][index]/veces_m[index], tiempos_m[3][index]/veces_m[index]);
        }
    }

    fclose(file_in_n_m);
    fclose(file_in_n);
    fclose(file_in_m);

    return 0;
}

void start_timer() {
    start_time = chrono::high_resolution_clock::now();
}

double stop_timer() {
    chrono::time_point<chrono::high_resolution_clock> end_time = chrono::high_resolution_clock::now();
    return double(chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count());
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
