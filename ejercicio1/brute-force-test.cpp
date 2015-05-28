#include <vector>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
using namespace std;

// para indexar los arrays de costos y otras operaciones con vehiculos
#define BMX        0
#define MOTO       1
#define BUGGY      2

struct costoEtapa {

    costoEtapa() : bmx(0), moto(0), buggy(0) {}

    costoEtapa(int t1, int t2, int t3) : bmx(t1), moto(t2), buggy(t3) {}

    int costo(int vehiculo) {
        if (vehiculo == BMX) {
            return bmx;
        } else if (vehiculo == MOTO) {
            return moto;
        } else {
            return buggy;
        }
    }

    int bmx;
    int moto;
    int buggy;
};

string resolver_bruto(int n, int km, int kb, vector<costoEtapa>& costos);
vector<int> bfs_bruto(int n, int h, int km_aux, int kb_aux,
    vector<int> vehiculos, int km, int kb, vector<costoEtapa>& costos);
int tiempo_total_aux(vector<int> vehiculos, vector<costoEtapa> &costos);
string resolver_ej1(int n, int km, int kb, vector<costoEtapa>& costos);


/********************** EJERCICIO 1 (sin main) ************************************/

// celda de la Tabla
struct celda {

    celda() : tiempo(0), antecesor(-1,-1), vehiculo(-1) {}

    // antecesor son las coordenadas de la celda sobre la cual se basa esta solucion
    pair<int, int> antecesor;
    // el vehiculo que se agrega en esta celda
    int vehiculo;
    // el tiempo total de la carrera que se obtiene al poner el vehiculo en la etapa
    int tiempo;
};

typedef vector<celda> Vec;
typedef vector<Vec> Tabla;
typedef vector<Tabla> HiperTabla;

int min(int a, int b);
int min(int a, int b, int c);

string resolver_ej1(int n, int km, int kb, vector<costoEtapa>& costos) {
    // creo la tabla de celdaes parciales
    // las etapas van de 0 a n-1
    HiperTabla matriz(n, Tabla(km+1, Vec(kb+1, celda())));

    // inicializo n = 0
    for (int i = 0; i <= km; i++) {
        for (int j = 0; j <= kb; j++) {
            if (i == 0 && j == 0) {
                matriz[0][i][j].tiempo = costos[0].costo(BMX);
                matriz[0][i][j].vehiculo = BMX;
            } else if (i == 0) { // j != 0
                int aux1 = costos[0].costo(BMX);
                int aux2 = costos[0].costo(BUGGY);
                matriz[0][i][j].tiempo = min(aux1, aux2);
                // guardo el vehiculo
                if (matriz[0][i][j].tiempo == aux1) {
                    matriz[0][i][j].vehiculo = BMX;
                } else {
                    matriz[0][i][j].vehiculo = BUGGY;
                }
            } else if (j == 0) { // i == 0
                int aux1 = costos[0].costo(BMX);
                int aux2 = costos[0].costo(MOTO);
                matriz[0][i][j].tiempo = min(aux1, aux2);
                // guardo el vehiculo
                if (matriz[0][i][j].tiempo == aux1) {
                    matriz[0][i][j].vehiculo = BMX;
                } else {
                    matriz[0][i][j].vehiculo = MOTO;
                }
            } else {
                int aux1 = costos[0].costo(BMX);
                int aux2 = costos[0].costo(MOTO);
                int aux3 = costos[0].costo(BUGGY);
                matriz[0][i][j].tiempo = min(aux1, aux2, aux3);
                // guardo el vehiculo
                if (matriz[0][i][j].tiempo == aux1) {
                    matriz[0][i][j].vehiculo = BMX;
                } else if (matriz[0][i][j].tiempo == aux2) {
                    matriz[0][i][j].vehiculo = MOTO;
                } else {
                    matriz[0][i][j].vehiculo = BUGGY;
                }
            }
        }
    }

    for (int h = 1; h < n; h++) {
        for (int i = 0; i <= km; i++) {
            for (int j = 0; j <= kb; j++) {
                if (i == 0 && j == 0) {
                    matriz[h][i][j].tiempo = costos[h].costo(BMX) + matriz[h-1][i][j].tiempo;
                    matriz[h][i][j].antecesor = make_pair(i,j);
                    matriz[h][i][j].vehiculo = BMX;
                } else if (i == 0) { // j != 0
                    int aux1 = costos[h].costo(BMX) + matriz[h-1][i][j].tiempo;
                    int aux2 = costos[h].costo(BUGGY) + matriz[h-1][i][j-1].tiempo;
                    matriz[h][i][j].tiempo = min(aux1, aux2);
                    // guardo el antecesor y el vehiculo
                    if (matriz[h][i][j].tiempo == aux1) {
                        matriz[h][i][j].antecesor = make_pair(i,j);
                        matriz[h][i][j].vehiculo = BMX;
                    } else {
                        matriz[h][i][j].antecesor = make_pair(i,j-1);
                        matriz[h][i][j].vehiculo = BUGGY;
                    }
                } else if (j == 0) { // i == 0
                    int aux1 = costos[h].costo(BMX) + matriz[h-1][i][j].tiempo;
                    int aux2 = costos[h].costo(MOTO) + matriz[h-1][i-1][j].tiempo;
                    matriz[h][i][j].tiempo = min(aux1, aux2);
                    // guardo el antecesor y el vehiculo
                    if (matriz[h][i][j].tiempo == aux1) {
                        matriz[h][i][j].antecesor = make_pair(i,j);
                        matriz[h][i][j].vehiculo = BMX;
                    } else {
                        matriz[h][i][j].antecesor = make_pair(i-1,j);
                        matriz[h][i][j].vehiculo = MOTO;
                    }
                } else {
                    int aux1 = costos[h].costo(BMX) + matriz[h-1][i][j].tiempo;
                    int aux2 = costos[h].costo(MOTO) + matriz[h-1][i-1][j].tiempo;
                    int aux3 = costos[h].costo(BUGGY) + matriz[h-1][i][j-1].tiempo;
                    matriz[h][i][j].tiempo = min(aux1, aux2, aux3);
                    // guardo el antecesor y el vehiculo
                    if (matriz[h][i][j].tiempo == aux1) {
                        matriz[h][i][j].antecesor = make_pair(i,j);
                        matriz[h][i][j].vehiculo = BMX;
                    } else if (matriz[h][i][j].tiempo == aux2) {
                        matriz[h][i][j].antecesor = make_pair(i-1,j);
                        matriz[h][i][j].vehiculo = MOTO;
                    } else {
                        matriz[h][i][j].antecesor = make_pair(i,j-1);
                        matriz[h][i][j].vehiculo = BUGGY;
                    }
                }
            }
        }
    }

    // reconstruyo la solucion
    vector<int> vehiculos(n, -1);
    celda aux = matriz[n-1][km][kb];
    for (int h = n-1; h > 0; h--) {
        vehiculos[h] = aux.vehiculo;
        aux = matriz[h-1][aux.antecesor.first][aux.antecesor.second];
    }
    vehiculos[0] = aux.vehiculo;

    // muestro la solucion por stdout
    string au2 = to_string(matriz[n-1][km][kb].tiempo);
    au2 += " ";
    for(int i = 0; i < n; i++) {
        au2 += to_string(vehiculos[i]+1);
        au2 += " ";
    }
    au2 += "\n";
    return au2;
}

int min(int a, int b) {
    return a < b ? a : b;
}

int min(int a, int b, int c) {
    return min(min(a,b),c);
}

/********************** fin EJERCICIO 1 ************************************/

// Implementacion
int main() {
    srand(time(NULL));

    int n_max = 10;
    int costo_max = 10000;
    int muestras_por_n = 1000;

    for (int n = 1; n <= n_max; n++) {
        cout << n << endl;
        for (int m = 0; m < muestras_por_n; m++) {
            int km = (rand()%n)+1;
            int kb = (rand()%n)+1;
            vector<costoEtapa> costos(n, costoEtapa());
            for (int aux = 0; aux < n; aux++) {
                int bmx = (rand()%costo_max)+1;
                int moto = (rand()%costo_max)+1;
                int buggy = (rand()%costo_max)+1;
                costos[aux].bmx = bmx;
                costos[aux].moto = moto;
                costos[aux].buggy = buggy;
            }
            string resultado_bruto = resolver_bruto(n, km, kb, costos);
            string resultado_ej1 = resolver_ej1(n, km, kb, costos);
            if (resultado_bruto.compare(resultado_ej1) == 0) {
                //cout << "BIEN" << endl;
            } else {
                // Veo si de verdad difierien en los tiempos, o solamente es una diferencia de orden
                std::stringstream  stream1(resultado_bruto);
                std::string tiempo_bruto;
                stream1 >> tiempo_bruto;

                std::stringstream  stream2(resultado_ej1);
                std::string tiempo_ej1;
                stream2 >> tiempo_ej1;

                if (tiempo_bruto.compare(tiempo_ej1) != 0) {
                    cout << "\nDIFERENCIA ENCONTRADA" << endl;
                    cout << "Problema: " << n << " " << km << " " << kb << endl;
                    for(int i = 0; i < n; i++) {
                        cout << costos[i].costo(BMX) << " ";
                        cout << costos[i].costo(MOTO) << " ";
                        cout << costos[i].costo(BUGGY) << endl;
                    }
                    cout << "Bruto: " << resultado_bruto;
                    cout << "Ej1: " << resultado_ej1;
                }
            }
        }
    }

    return 0;
}

int tiempo_total_aux(vector<int> vehiculos, vector<costoEtapa> &costos) {
    int n = vehiculos.size();
    int tiempo = 0;
    for(int i = 0; i < n; i++) {
       tiempo = tiempo + costos[i].costo(vehiculos[i]);
    }
    return tiempo;
}

string resolver_bruto(int n, int km, int kb, vector<costoEtapa>& costos) {
    // solucion inicial
    vector<int> vehiculos(n, BMX);

    vector<int> sol = bfs_bruto(n, 0, 0, 0, vehiculos, km, kb, costos);

    //cout << tabla[fila][columna].tiempo << " ";
    string aux = to_string(tiempo_total_aux(sol, costos));
    aux += " ";
    for(int i = 0; i < n; i++) {
        string v = "";
        if (sol[i] == MOTO) {
            v = "Motocross";
        } else if (sol[i] == BUGGY) {
            v = "Buggy";
        } else {
            v = "BMX";
        }
        //cout << "Etapa " << i+1 << ": " << v << endl;
        //cout << vehiculos[i]+1 << " ";
        aux += to_string(sol[i]+1);
        aux += " ";
    }
    //cout << "Tiempo total: " << tabla[fila][columna].tiempo << endl;
    //cout << endl;
    aux += "\n";
    return aux;
}

vector<int> bfs_bruto(int n, int h, int km_aux, int kb_aux,
    vector<int> vehiculos, int km, int kb, vector<costoEtapa>& costos) {

    if (h >= n || (km_aux >= km && kb_aux >= kb)) {
        return vehiculos;
    }

    vector<int> sol_bmx = bfs_bruto(n, h+1, km_aux, kb_aux, vehiculos, km, kb, costos);
    vector<int> sol_moto = sol_bmx;
    if (km_aux < km) {
        vehiculos[h] = MOTO;
        sol_moto = bfs_bruto(n, h+1, km_aux + 1, kb_aux, vehiculos, km, kb, costos);
    }
    vector<int> sol_buggy = sol_bmx;
    if (kb_aux < kb) {
        vehiculos[h] = BUGGY;
        sol_buggy = bfs_bruto(n, h+1, km_aux, kb_aux + 1, vehiculos, km, kb, costos);
    }

    int tiempo_bmx = tiempo_total_aux(sol_bmx, costos);
    int tiempo_moto = tiempo_total_aux(sol_moto, costos);
    int tiempo_buggy = tiempo_total_aux(sol_buggy, costos);

    // elijo el minimo
    if (tiempo_bmx < tiempo_moto) {
        if (tiempo_bmx < tiempo_buggy) {
            return sol_bmx;
        } else {
            return sol_buggy;
        }
    } else {
        if (tiempo_moto < tiempo_buggy) {
            return sol_moto;
        } else {
            return sol_buggy;
        }
    }

}
