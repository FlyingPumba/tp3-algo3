#include <vector>
#include <iostream>
using namespace std;

// Structs y Typedefs
// para indexar los arrays de costos y otras operaciones con vehiculos
#define BMX        0
#define MOTO       1
#define BUGGY      2

// struct que contiene el costo de una etapa con diferentes vehiculos
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

// celda de la Tabla
struct celda {
    celda() : tiempo(0), antecesor(-1,-1), vehiculo(-1) {}

    pair<int, int> antecesor;
    int vehiculo;
    int tiempo;
};

typedef vector<celda> Vec;
typedef vector<Vec> Tabla;
typedef vector<Tabla> HiperTabla;

// Prototipado de funciones
void resolver(int n, int km, int kb, vector<costoEtapa>& costos);
int min(int a, int b);
int min(int a, int b, int c);

// Implementacion.
int main() {
    // cantidad de etapas
    int n;
    cin >> n;
    // cantidad de veces que puedo usar la moto
    int km;
    cin >> km;
    // cantidad de veces que puedo usar el buggy
    int kb;
    cin >> kb;

    // limito la cantidad de motos a la cantidad de etapas
    if(km > n) {
        km = n;
    }
    // limito la cantidad de buggys a la cantidad de etapas
    if(kb > n) {
        kb = n;
    }

    // cargo los costos
    vector<costoEtapa> costos(n, costoEtapa());
    int bmx, moto, buggy;
    for(int i = 0; i < n; i++) {
        cin >> bmx;
        cin >> moto;
        cin >> buggy;
        costoEtapa costo = costoEtapa(bmx, moto, buggy);
        costos[i] = costo;
    }

    resolver(n, km, kb, costos);
}

void resolver(int n, int km, int kb, vector<costoEtapa>& costos) {
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
                    matriz[h][i][j].tiempo = costos[h].costo(BMX) +
                                    matriz[h-1][i][j].tiempo;
                    matriz[h][i][j].antecesor = make_pair(i,j);
                    matriz[h][i][j].vehiculo = BMX;
                } else if (i == 0) { // j != 0
                    int aux1 = costos[h].costo(BMX) +
                                    matriz[h-1][i][j].tiempo;
                    int aux2 = costos[h].costo(BUGGY) +
                                    matriz[h-1][i][j-1].tiempo;
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
                    int aux1 = costos[h].costo(BMX) +
                                    matriz[h-1][i][j].tiempo;
                    int aux2 = costos[h].costo(MOTO) +
                                    matriz[h-1][i-1][j].tiempo;
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
                    int aux1 = costos[h].costo(BMX) +
                                        matriz[h-1][i][j].tiempo;
                    int aux2 = costos[h].costo(MOTO) +
                                        matriz[h-1][i-1][j].tiempo;
                    int aux3 = costos[h].costo(BUGGY) +
                                        matriz[h-1][i][j-1].tiempo;
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
    cout << matriz[n-1][km][kb].tiempo << " ";
    for(int i = 0; i < n; i++) {
        cout << vehiculos[i]+1 << " ";
    }
    cout << endl;
}

int min(int a, int b) {
    return a < b ? a : b;
}

int min(int a, int b, int c) {
    return min(min(a,b),c);
}
