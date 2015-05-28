#include <vector>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <chrono>
using namespace std;

typedef pair<int, int> Coord;

auto startTime = chrono::high_resolution_clock::now();
void startTimer() {
    startTime = chrono::high_resolution_clock::now();
}

double endTimer() {
    auto finish = chrono::high_resolution_clock::now();
    double tiempo = chrono::duration_cast<chrono::nanoseconds>(finish-startTime).count();
    return tiempo;
}

// Implementacion
int main() {
    FILE* file = fopen("datos1-ej1.dat","w+");
    fprintf(file, "n km kb nanosegundos km*kb n*km*kb div-(km*kb) div-(n*km*kb)\n");
    srand(time(NULL));

    int n_max = 100;
    int costo_max_bmx = 1000;
    int muestras_por_n = 10;
    int muestras_por_km_kb = 10;

    for (int n = 1; n <= n_max; n++) {
        for (int m = 0; m < muestras_por_n; m++) {
            double sum = 0;
            int km = (rand()%n)+1;
            int kb = (rand()%n)+1;
            for (int k = 0; k < muestras_por_km_kb; k++) {
                string program = "echo '";
                program += to_string(n);
                program += " ";
                program += to_string(km);
                program += " ";
                program += to_string(kb);
                program += " ";
                for (int aux = 0; aux < n; aux++) {
                    int bmx = (rand()%costo_max_bmx)+1;
                    int moto = rand()%bmx;
                    int buggy = rand()%bmx;
                    program += to_string(bmx);
                    program += " ";
                    program += to_string(moto);
                    program += " ";
                    program += to_string(buggy);
                    program += " ";
                }
                program += "' | ./ejercicio1";
                startTimer();
                //cout << program << endl;
                system(program.c_str());
                sum = sum + endTimer();
            }
            double prom = sum / double(muestras_por_km_kb);
            double aux1 = prom/double(km*kb);
            double aux1 = prom/double(n*km*kb);
            fprintf(file, "%d %d %d %7.8f %d %d %7.8f %7.8f\n", n, km, kb, prom, km*kb, n*km*kb, aux1, aux2);
        }
    }

    fclose(file);
    return 0;
}
