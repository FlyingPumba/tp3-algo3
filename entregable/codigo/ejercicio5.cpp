// INCLUDES
#include <vector>
#include <iostream>
#include <algorithm>
#include <stack>
#include <list>
#include <queue>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

// DEFINES
#define VISITADO 1
#define NO_VISITADO 0
#define INCLUIDO 1
#define NO_INCLUIDO 0
#define GREEDY_RANDOM_ALPHA 0.4
#define GREEDY_RANDOM_BETA 10
#define GRASP_MAX_ITER_COUNTER 10
#define PRIMER_CRITERIO 0
#define SEGUNDO_CRITERIO 1

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
void recibir_parametros(Grafo& G);
void imprimir_resultado(vector<int>& cidm);

void grasp(Grafo& G, vector<int>& cidm, bool criterio_grasp, bool criterio_greedy, bool criterio_busqueda);
void grasp_primer_criterio(Grafo& G, vector<int>& cidm, bool criterio_greedy, bool criterio_busqueda);
void grasp_segundo_criterio(Grafo& G, vector<int>& cidm, bool criterio_greedy, bool criterio_busqueda);

void construir_greedy_random(Grafo& G, vector<int>& solucion, bool criterio_greedy);
void construir_greedy_random_primer_criterio(Grafo& G, vector<int>& solucion);
void construir_greedy_random_segundo_criterio(Grafo& G, vector<int>& solucion);

void busqueda_local(Grafo& G, vector<int>& solucionInicial, bool criterio_busqueda);
void busqueda_local_primer_criterio(Grafo& G, vector<int>& solucionInicial);
void busqueda_local_segundo_criterio(Grafo& G, vector<int>& solucionInicial);

bool solucion_posible(Grafo& G, vector<int>& solucionCambiar, int cantCambios);
int get_indice_nodo(Nodos& nodos, int v);
int random_in_range(int min, int max);

// IMPLEMENTACION
int main() {
    srand(time(0)); // configuro el seed del random generator

    Grafo G;
    vector<int> cidm;

    recibir_parametros(G);
    grasp(G, cidm, PRIMER_CRITERIO, PRIMER_CRITERIO, PRIMER_CRITERIO);
    imprimir_resultado(cidm);
    
    return 0;
}

void recibir_parametros(Grafo& G) {
    int n, m;
    cin >> n;
    cin >> m;

    G = Grafo(n, listaAdyacencia());

    int v, w;
    for (int i = 0; i < m; i++) {
        cin >> v;
        cin >> w;
        G[v-1].push_back(w-1);
        G[w-1].push_back(v-1);
    }
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

void grasp(Grafo& G, vector<int>& cidm, bool criterio_grasp, bool criterio_greedy, bool criterio_busqueda) {
    if (criterio_grasp == PRIMER_CRITERIO) {
        grasp_primer_criterio(G, cidm, criterio_greedy, criterio_busqueda);
    } else {
        grasp_segundo_criterio(G, cidm, criterio_greedy, criterio_busqueda);
    }
}

void grasp_primer_criterio(Grafo& G, vector<int>& cidm, bool criterio_greedy, bool criterio_busqueda) {
    vector<int> mejor_solucion;
    construir_greedy_random(G, mejor_solucion, criterio_greedy);
    // Criterio de parada 1: hace tantas iteraciones como nodos en el grafo.
    for (int i = 0; i < G.size(); i++) {
        // Construir Solucion Greedy Random
        vector<int> nueva_solucion;
        construir_greedy_random(G, nueva_solucion, criterio_greedy);
        // Hacer busqueda local
        busqueda_local(G, nueva_solucion, criterio_busqueda);
        // Actualizar Mejor Solucion
        if (nueva_solucion.size() < mejor_solucion.size()) {
            mejor_solucion = nueva_solucion;
        }
    }
    cidm = mejor_solucion;
}

void grasp_segundo_criterio(Grafo& G, vector<int>& cidm, bool criterio_greedy, bool criterio_busqueda) {
    vector<int> mejor_solucion;
    construir_greedy_random(G, mejor_solucion, criterio_greedy);
    // Criterio de parada 2: sigue iterando hasta que no mejore la mejor solucion durante 'GRASP_MAX_ITER_COUNTER' cantidad de ciclos.
    int counter = GRASP_MAX_ITER_COUNTER;
    while (counter > 0) {
        // Construir Solucion Greedy Random
        vector<int> nueva_solucion;
        construir_greedy_random(G, nueva_solucion, criterio_greedy);
        // Hacer busqueda local
        busqueda_local(G, nueva_solucion, criterio_busqueda);
        // Actualizar Mejor Solucion
        if (nueva_solucion.size() < mejor_solucion.size()) {
            mejor_solucion = nueva_solucion;
            counter = GRASP_MAX_ITER_COUNTER;
        } else {
            counter--;
        }
    }
    cidm = mejor_solucion;
}

void construir_greedy_random(Grafo& G, vector<int>& solucion, bool criterio_greedy) {
    if (criterio_greedy == PRIMER_CRITERIO) {
        construir_greedy_random_primer_criterio(G, solucion);
    } else {
        construir_greedy_random_segundo_criterio(G, solucion);
    }
}

void construir_greedy_random_primer_criterio(Grafo& G, vector<int>& solucion) {
    // Criterio de Restricted Candidate List 1: los nodos que cumplan la condicion
    // de que su grado es por lo menos mejor_grado (de todos los nodos) * GREEDY_RANDOM_ALPHA.
    int n = G.size();
    Nodos nodos(n, Nodo());

    for(int u = 0; u < n; u++) {
        nodos[u].numero = u;
        nodos[u].grado = G[u].size();
    }

    sort(nodos.begin(), nodos.end(), orden());

    solucion = vector<int>(n, NO_INCLUIDO);

    int nodos_visitados = 0;
    while (nodos_visitados < n) {
        int mejor_grado = nodos[0].grado;
        int window_size = 0; // el maximo indice posible, la RCL va a ser nodos[0...window_size]
        for (int i = 0; i < nodos.size(); i++) {
            if (nodos[i].grado >= mejor_grado * GREEDY_RANDOM_ALPHA) {
                window_size = i;
            } else {
                break;
            }
        }

        int indice = random_in_range(0, min(window_size, (int)nodos.size()-1));

        int nodo = nodos[indice].numero;
        solucion[nodo] = INCLUIDO;
        nodos_visitados++;
        nodos.erase(nodos.begin()+indice);

        for (list<int>::iterator itAdyU=G[nodo].begin(); itAdyU != G[nodo].end(); itAdyU++) {
            int v = *itAdyU;
            int index = get_indice_nodo(nodos, v);
            if (index != -1) {
                // v no esta en el vector de nodos
                nodos.erase(nodos.begin()+index);
                nodos_visitados++;
            }
        }
    }
}

void construir_greedy_random_segundo_criterio(Grafo& G, vector<int>& solucion) {
    // Criterio de Restricted Candidate List 2: los nodos que cumplan la condicion
    // de estar entre los 'GREEDY_RANDOM_BETA' (definido antes) mejores nodos.
    int n = G.size();
    Nodos nodos(n, Nodo());

    for(int u = 0; u < n; u++) {
        nodos[u].numero = u;
        nodos[u].grado = G[u].size();
    }

    sort(nodos.begin(), nodos.end(), orden());

    solucion = vector<int>(n, NO_INCLUIDO);

    int nodos_visitados = 0;
    while (nodos_visitados < n) {
        int mejor_grado = nodos[0].grado;
        int window_size = GREEDY_RANDOM_BETA;  // el maximo indice posible, la RCL va a ser nodos[0...window_size]
        int indice = random_in_range(0, min(window_size, (int)nodos.size()-1));

        int nodo = nodos[indice].numero;
        solucion[nodo] = INCLUIDO;
        nodos_visitados++;
        nodos.erase(nodos.begin()+indice);

        for (list<int>::iterator itAdyU=G[nodo].begin(); itAdyU != G[nodo].end(); itAdyU++) {
            int v = *itAdyU;
            int index = get_indice_nodo(nodos, v);
            if (index != -1) {
                // v no esta en el vector de nodos
                nodos.erase(nodos.begin()+index);
                nodos_visitados++;
            }
        }
    }
}

void busqueda_local(Grafo& G, vector<int>& solucionInicial, bool criterio_busqueda) {
    if (criterio_busqueda == PRIMER_CRITERIO) {
        busqueda_local_primer_criterio(G, solucionInicial);
    } else {
        busqueda_local_segundo_criterio(G, solucionInicial);
    }
}

void busqueda_local_primer_criterio(Grafo& G, vector<int>& solucionInicial) {
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
          busqueda_local_primer_criterio(G, solucionAuxiliar);
          solucionInicial = solucionAuxiliar;
          break;
        }
      }

    }
  }
}

void busqueda_local_segundo_criterio(Grafo& G, vector<int>& solucionInicial) {
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
          busqueda_local_segundo_criterio(G, solucionAuxiliar);
          solucionInicial = solucionAuxiliar;
          break;
        }
      }
    }
  }
}

bool solucion_posible(Grafo& G, vector<int>& solucionCambiar, int cantCambios) {
  int n = G.size();
  bool esSolucion = true;
  bool finCiclo = false;
  list<int> verticesCambiados;

  for (int u = 0; u < n && !finCiclo; u++) {
    // Si esta INCLUIDO, sus adyacentes no pueden estar INCLUIDOS
    if (solucionCambiar[u] == INCLUIDO && G[u].size() > 0) {
      for (list<int>::iterator itAdyU=G[u].begin(); itAdyU != G[u].end() && !finCiclo; ++itAdyU) {
        int v = *itAdyU;
        if (solucionCambiar[v] == INCLUIDO) {
          esSolucion = false;
          finCiclo = true;
        }
      }
    } else if (G[u].size() > 0) {
      // Si esta NO INCLUIDO, al menos uno de sus adyacentes tiene que estar INCLUIDO
      bool adyINCLUIDO = false;
      for (list<int>::iterator itAdyU=G[u].begin(); itAdyU != G[u].end() && !finCiclo; ++itAdyU) {
        int v = *itAdyU;
        if (solucionCambiar[v] == INCLUIDO) {
          adyINCLUIDO = true;
        }
      }
      if (!adyINCLUIDO && cantCambios == 0) {
        esSolucion = false;
        finCiclo = true;
      } else if(!adyINCLUIDO) {
        // Salvo la solucion al marcar el vertice
        solucionCambiar[u] = INCLUIDO;
        cantCambios --;
      }
    } else {
      // Si es un K1, tiene que estar INCLUIDO
      esSolucion = solucionCambiar[u] == INCLUIDO;
      finCiclo = !(solucionCambiar[u] == INCLUIDO);
    }
  }

  return esSolucion;
}

int get_indice_nodo(Nodos& nodos, int v) {
    int n = nodos.size();
    for (int i = 0; i < n; i++) {
        if (nodos[i].numero == v) {
            return i;
        }
    }
    return -1;
}

int random_in_range(int min, int max) {
  return min + (rand() % (max - min + 1));
}
