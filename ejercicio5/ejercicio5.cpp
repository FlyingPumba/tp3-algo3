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

// ESTRUCTURAS
struct Nodo {
    int numero; //indica el indice con el que fue creado
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

// TYPEDEF
typedef vector<Nodo> Nodos;
typedef list<int> listaAdyacencia;
typedef vector<listaAdyacencia> Grafo;

// FUNCIONES
void recibir_parametros(Grafo& G);
void imprimir_resultado(vector<int>& cidm);
vector<int> grasp(Grafo& G);
vector<int> construir_greedy_random(Grafo& G);
void busqueda_local(Grafo& G, vector<int>& solucionInicial);
bool solucion_posible(Grafo& G, vector<int>& solucionCambiar, int cantCambios);
int get_indice_nodo(Nodos& nodos, int v);
int random_in_range(int min, int max);

// IMPLEMENTACION
int main() {
    srand(time(0)); // use current time as seed for random generator

    Grafo G;
    recibir_parametros(G);
    vector<int> cidm = grasp(G);
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
    cout << "[";
    for (int i = 0; i < cidm.size()-1; i++) {
        if(cidm[i] == INCLUIDO) {
          cout << i + 1 << ",";
        }
    }
    if (cidm[cidm.size() - 1] == INCLUIDO) {
        cout <<  cidm.size() << "]" << endl;
    }
}

vector<int> grasp(Grafo& G) {
    vector<int> mejor_solucion = construir_greedy_random(G);
    // Criterio de parada: hace tantas iteraciones como nodos en el grafo.
    for (int i = 0; i < G.size(); i++) {
        //Construir Solucion Greedy Random
        vector<int> nueva_solucion = construir_greedy_random(G);
        //Hacer busqueda local
        busqueda_local(G, nueva_solucion);
        //Actualizar Mejor Solucion
        if (nueva_solucion.size() < mejor_solucion.size()) {
            mejor_solucion = nueva_solucion;
        }
    }
    return mejor_solucion;
}

vector<int> construir_greedy_random(Grafo& G) {
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

    vector<int> solucion(n, NO_INCLUIDO);

    int nodos_agregados = 0;
    while (nodos_agregados < n) {
        int window_size = 5; // definir en porcentaje del grado que tiene el optimo (indice 0 de no visitados)
        int indice = random_in_range(0, min(window_size, (int)nodos.size()-1));

        int nodo = nodos[indice].numero;
        solucion[nodo] = INCLUIDO;
        nodos_agregados++;
        nodos.erase(nodos.begin()+indice);

        for (list<int>::iterator itAdyU=G[nodo].begin(); itAdyU != G[nodo].end(); itAdyU++) {
            int v = *itAdyU;
            int index = get_indice_nodo(nodos, v);
            nodos.erase(nodos.begin()+index);
            nodos_agregados++;
        }
    }
    return solucion;
}

void busqueda_local(Grafo& G, vector<int>& solucionInicial) {
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
        bool esSolucion = solucion_posible(G, solucionAuxiliar, cantCambiosPosibles);
        //cout <<  "Probe nodo: " << u + 1 << endl;
        if(esSolucion){
          // Cambios necesarios para que sea solucion
          //cout <<  "Exito" << endl;
          // Encontre una solucion Vecina mejor, fin del ciclo
          busqueda_local(G, solucionAuxiliar);
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
