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
#include <math.h>
using namespace std;

#define INFINITO -1
#define MAX_N 9

typedef vector<int> Vec;
typedef vector<Vec> Matriz;

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
    data_file.open("datos-ej2.dat");
    data_file << "n sin-podas poda-A poda-B ambas-podas ";
    data_file << "sin-podas-div-2n poda-A-div-2n poda-B-div-2n ambas-podas-div-2n ";
    data_file << "sin-podas-div-2nn3 poda-A-div-2nn3 poda-B-div-2nn3 ambas-podas-div-2nn3";
    data_file << "\n";

    vector<int> cant_tests_n(MAX_N+1, 0);
    vector<double> tiempos_sin_podas(MAX_N+1, 0);
    vector<double> tiempos_poda_A(MAX_N+1, 0);
    vector<double> tiempos_poda_B(MAX_N+1, 0);
    vector<double> tiempos_ambas_podas(MAX_N+1, 0);

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
        if (n <= MAX_N) {
            // medir tiempos:
            poda_A = false;
            poda_B = false;
            start_timer();
            resolver(n, matriz);
            tiempos_sin_podas[n] += stop_timer();

            poda_A = true;
            poda_B = false;
            start_timer();
            resolver(n, matriz);
            tiempos_poda_A[n] += stop_timer();

            poda_A = false;
            poda_B = true;
            start_timer();
            resolver(n, matriz);
            tiempos_poda_B[n] += stop_timer();

            poda_A = true;
            poda_B = true;
            start_timer();
            resolver(n, matriz);
            tiempos_ambas_podas[n] += stop_timer();

            cant_tests_n[n] += 1;
        }
        infile.close();
        infile.clear();
    }

    for (int i = 0; i <= MAX_N; i++) {
        if (cant_tests_n[i] != 0) {
            double time_sin_podas = tiempos_sin_podas[i]/(double)cant_tests_n[i];
            double time_poda_A = tiempos_poda_A[i]/(double)cant_tests_n[i];
            double time_poda_B = tiempos_poda_B[i]/(double)cant_tests_n[i];
            double time_ambas_podas = tiempos_ambas_podas[i]/(double)cant_tests_n[i];
            data_file << i << " " << time_sin_podas << " " << time_poda_A << " " << time_poda_B << " " << time_ambas_podas << " ";

            double exponencial = pow(4,i);
            time_sin_podas = time_sin_podas/exponencial;
            time_poda_A = time_poda_A/exponencial;
            time_poda_B = time_poda_B/exponencial;
            time_ambas_podas = time_ambas_podas/exponencial;
            data_file << time_sin_podas << " " << time_poda_A << " " << time_poda_B << " " << time_ambas_podas << " ";

            double cubica = pow(i,3);
            time_sin_podas = time_sin_podas/cubica;
            time_poda_A = time_poda_A/cubica;
            time_poda_B = time_poda_B/cubica;
            time_ambas_podas = time_ambas_podas/cubica;
            data_file << time_sin_podas << " " << time_poda_A << " " << time_poda_B << " " << time_ambas_podas << "\n";
        }
    }

    data_file.close();
    return 0;
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
            bool dom_dominante = true;
            if (poda_B == false) {
                // no se todavia si Dom es dominante o no, lo chequeo:
                for (int i = 0; i < n; i++) {
                    // chequeo si el nodo i está en copia o es adyacente a alguno en copia
                    bool nodo = false;
                    for (int j = 0; j < dom.size(); j++) {
                        if (dom[j] == i) {
                            // el nodo i está en dom
                            nodo = true;
                            break;
                        } else if (matriz[i][dom[j]] == 1) {
                            // el nodo i es adyacente a alguno en dom
                            nodo = true;
                            break;
                        }
                    }
                    if (!nodo) {
                        dom_dominante = false;
                        break;
                    }
                }
            }

            if (dom_dominante) {
                if (poda_A == true) {
                    // Sabemos que si Dom es dominante e independiente, entonces cualquier subconjunto de Dom es no-dominante
                    // Luego, devolvemos el que tiene menor cardinal entre dom y cidm
                    if (dom_size < cidm_size) {
                        return dom;
                    } else {
                        return cidm;
                    }
                } else {
                    // sin aplicar la poda A
                    if (dom_size < cidm_size) {
                        cidm = dom;
                        cidm_size = dom_size;
                    }
                }
            }
        }
    }

    for (int i = 0; i < dom_size; i++) {
        // copio dom y borro el i-esimo nodo de la copia (no es el nodo numero i, sino el nodo en la posicion i del vector)
        vector<int> copia(dom);
        copia.erase(copia.begin() + i);
        if (poda_B == true) {
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
