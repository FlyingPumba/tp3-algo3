#include <vector>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <chrono>
#include <thread>
#include <list>
#include <utility>
using namespace std;

clock_t startTime;
void startTimer() {
    startTime = clock();
}

void endTimer(FILE* file) {
    double tiempo = double( clock() - startTime ) / (double)CLOCKS_PER_SEC;
    fprintf(file, "%7.8f\n", tiempo);
}

double endTimer() {
    double tiempo = double( clock() - startTime ) / (double)CLOCKS_PER_SEC;
    return tiempo;
}

// Implementacion
int main() {
    FILE* file = fopen("tiempos.txt","a+");
    srand(time(NULL));
    // "echo '3 0' | ./caballos"
    const char *echo_base_0 = "echo '%d %d'";
    const char *echo_base_k = "echo '%d %d ";
    const char *extra_base = "%d %d ";
    const char *program_base_0 = " | ./caballos";
    const char *program_base_k = "' | ./caballos";

    //VARIABLES
    int muestras = 100;
    int n = 5;
    int k_inicial = 0;
    int k_final = n*n;
    long long int tiempos[(n*n)+1];

    for (int i = 0; i <= n*n; i++) {
        tiempos[i] = 0;
    }

    //MATRIZ DE CONTROL
    bool checker[n][n];

    for (int m = 0; m < muestras; m++) {

        list< pair<int,int> > coordenadas;
        cout << m << endl;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                checker[i][j] = false;
        }

        for (int i = 0; i < k_inicial; i++) {
            int f = rand()%n;
            int c = rand()%n;
            while (checker[f][c] == true){
                f = rand()%n;
                c = rand()%n;
            }
            checker[f][c] = true;
            coordenadas.push_back(make_pair(f,c));
        }

        for (int k = k_inicial; k <= k_final; k++) {

            if (m == 0 || k != 0){

                char program[512];
                //cout << n << " " << k << endl;

                if (k > 0) {
                    int f = rand()%n;
                    int c = rand()%n;
                    while (checker[f][c] == true){
                        f = rand()%n;
                        c = rand()%n;
                    }
                    checker[f][c] = true;
                    coordenadas.push_back(make_pair(f,c));
                }

                if (k == 0) {
                    sprintf(program, echo_base_0, n, k);
                    strcat(program, program_base_0);
                } else {
                    sprintf(program, echo_base_k, n, k);
                    for(list< pair<int,int> >::iterator iter = coordenadas.begin();
                    		iter != coordenadas.end(); iter++) {
                        char saux[10];
                        sprintf(saux, extra_base, iter->first, iter->second);
                        strcat(program, saux);
                    }
                    strcat(program, program_base_k);
                }
                std::chrono::time_point<std::chrono::high_resolution_clock> t1 =
                	std::chrono::high_resolution_clock::now();
                system(program);
                std::chrono::time_point<std::chrono::high_resolution_clock> t2 =
                	std::chrono::high_resolution_clock::now();
                tiempos[(n*n)-k] += std::chrono::duration_cast
                	<std::chrono::nanoseconds>(t2-t1).count();
            }

        }
    }

    for (int i = n*n-k_final; i <= n*n-k_inicial; i++) {
        if (i == n*n)
            fprintf(file, "%d %lld\n", i, tiempos[i]);
        else
            fprintf(file, "%d %lld\n", i, tiempos[i]/muestras);       
    }

    fclose(file);

    return 0;
}
