#include <vector>
#include <iostream>
#include <fstream>
#include <stack>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sstream>
using namespace std;

#define INFINITO -1

typedef vector<int> Vec;
typedef vector<Vec> Matriz;

void medir_tiempos(ofstream &data_file, int n, int m, Matriz matriz);
void start_timer();
double stop_timer();
vector<int> resolver(int n, Matriz matriz);
vector<int> resolver_aux(int n, Matriz matriz, vector<int> dom, vector<int> cidm);

static bool poda_A = false;
static bool poda_B = false;
static chrono::time_point<chrono::high_resolution_clock> start_time;

// Implementacion.
int main() {
    char* path = "../tests/";

    // obtengo los nombres de los tests
    DIR* dirFile = opendir(path);
    vector<string> tests;
    if (dirFile) {
       struct dirent* hFile;
       errno = 0;
       while (( hFile = readdir( dirFile )) != NULL )
       {
          if (!strcmp( hFile->d_name, ".")) continue;
          if (!strcmp( hFile->d_name, "..")) continue;
          if (hFile->d_name[0] == '.') continue;

          if (strstr( hFile->d_name, ".in")) {
              string str(hFile->d_name);
              tests.push_back(str);
          }
       }
       closedir(dirFile);
    }

    // abro archivo de output
    ofstream data_file;
    data_file.open("datos_ej2.dat");
    data_file << "n m sin_podas poda_A poda_B ambas_podas\n";

    for (int i = 0; i < tests.size(); i++) {
        cout << tests[i] << endl;
        string test_file(path);
        test_file += tests[i];

        ifstream infile(test_file);

        string line;
        getline(infile, line);
        istringstream iss(line);

        int n, m;
        iss >> n;
        iss >> m;

        Matriz matriz(n, Vec(n, 0));

        int v, w;
        for (int i = 0; i < m; i++) {
            getline(infile, line);
            istringstream iss(line);

            iss >> v;
            iss >> w;
            matriz[v-1][w-1] = 1;
            matriz[w-1][v-1] = 1;
        }

        cout << n << " " << m << endl;
        medir_tiempos(data_file, n, m, matriz);
        infile.close();
        infile.clear();
    }

    data_file.close();
    return 0;
}

void medir_tiempos(ofstream &data_file, int n, int m, Matriz matriz) {
    poda_A = false;
    poda_B = false;
    start_timer();
    resolver(n, matriz);
    double time_sin_podas = stop_timer();

    poda_A = true;
    poda_B = false;
    start_timer();
    resolver(n, matriz);
    double time_poda_A = stop_timer();

    poda_A = false;
    poda_B = true;
    start_timer();
    resolver(n, matriz);
    double time_poda_B = stop_timer();

    poda_A = true;
    poda_B = true;
    start_timer();
    resolver(n, matriz);
    double time_ambas_podas = stop_timer();

    data_file << n << " " << m << " " << time_sin_podas << " " << time_poda_A << " " << time_poda_B << " " << time_ambas_podas << "\n";
}


void start_timer() {
    start_time = chrono::high_resolution_clock::now();
}

double stop_timer() {
    chrono::time_point<chrono::high_resolution_clock> end_time = chrono::high_resolution_clock::now();
    return double(chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count());
}

vector<int> resolver(int n, Matriz matriz) {
    // inicializo el primer conjunto a evaluar con todos los vertices del grafo
    // Este primer conjunto es dominante (pero no necesariamente independiente)
    vector <int> dom(n, 0);
    for (int i = 0; i < n; i++) {
        dom[i] = i;
    }
    return resolver_aux(n, matriz, dom, dom);
}

vector<int> resolver_aux(int n, Matriz matriz, vector<int> dom, vector<int> cidm) {
    int dom_size = dom.size();
    int cidm_size = cidm.size();
    if (cidm_size == 1) {
        return cidm;
    } else {
        // chequeo si dom es independiente
        bool indep = true;
        for (int i = 0; i < dom_size; i++) {
            for (int j = i+1; j < dom_size; j++) {
                if (matriz[dom[i]][dom[j]] == 1) {
                    indep = false;
                    break;
                }
            }
            if(!indep) {
                break;
            }
        }

        if(indep) {
            if (poda_A == true) {
                // Sabemos que si Dom es dominante e independiente, entonces cualquier subconjunto de Dom es no-dominante
                // Luego, devolvemos el que tiene menor cardinal entre dom y cidm
                if (dom_size < cidm_size) {
                    return dom;
                } else {
                    return cidm;
                }
            } else {
                // sin aplicar la poda
                if (dom_size < cidm_size) {
                    cidm = dom;
                    cidm_size = dom_size;
                }
            }
        }
    }

    for (int i = 0; i < dom_size; i++) {
        // copio dom y borro el i-esimo nodo de la copia (no es el nodo numero i, sino el nodo en la posicion i del vector)
        vector<int> copia(dom);
        copia.erase(copia.begin() + i);
        // chequeo si la copia es dominante
        bool copia_dominante = true;
        for (int i = 0; i < n; i++) {
            // chequeo si el nodo i está en copia o es adyacente a alguno en copia
            bool nodo = false;
            for (int j = 0; j < copia.size(); j++) {
                if (copia[j] == i) {
                    // el nodo i está en copia
                    nodo = true;
                    break;
                } else if (matriz[i][copia[j]] == 1) {
                    // el nodo i es adyacente a alguno en copia
                    nodo = true;
                    break;
                }
            }
            if (!nodo) {
                copia_dominante = false;
                break;
            }
        }
        if (poda_B == true) {
            // Sabemos que si Copia no es dominante, entonces ningun subconjunto de Copia es dominante, por lo que ni siquiera los evaluo
            if (copia_dominante) {
                vector<int> nuevo_cidm = resolver_aux(n, matriz, copia, cidm);
                if (nuevo_cidm.size() < cidm_size) {
                    cidm = nuevo_cidm;
                    cidm_size = nuevo_cidm.size();
                }
            }
        } else {
            vector<int> nuevo_cidm = resolver_aux(n, matriz, copia, cidm);
            if (nuevo_cidm.size() < cidm_size) {
                cidm = nuevo_cidm;
                cidm_size = nuevo_cidm.size();
            }
        }
    }
    return cidm;
}
