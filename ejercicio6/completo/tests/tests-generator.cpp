#include <vector>
#include <iostream>
#include <stack>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <ctime>
using namespace std;

#define INFINITO -1
#define CANT_TESTS 100

typedef vector<int> Vec;
typedef vector<Vec> Matriz;

string to_string_custom(int value) {
    ostringstream oss;
    oss << value;
    return oss.str();
}

vector<string> get_random_filename(int c) {
    vector<string> aux(2, "");
    aux[0] += to_string_custom(c);
    aux[0] += ".in";
    aux[1] += to_string_custom(c);
    aux[1] += ".out";
    return aux;
}

int main() {
    srand(time(0)); // use current time as seed for random generator

    for (int c = 1; c <= CANT_TESTS; c++) {


        vector<string> files = get_random_filename(c);
        FILE* file_in = fopen(files[0].c_str(),"w+");

         
         int n = c;

         //PARA GRAFO COMPLETO
         int m = c*(c-1)/2;

         //PARA EL COMPLEMENTO DEL COMPLETO
         //int m = 0;

        cout << "TEST: " << c << ", n: " << c << ", m: " << m << " -> 1"  << endl;

        fprintf(file_in, "%d %d\n",n , m );

         
         //PARA GRAFO COMPLETO   
         for (int i = 1; i <= n; i++) {
             for(int j = i+1; j <=n;j++){
                fprintf(file_in, "%d %d\n", i, j);
             }
         } 

        fclose(file_in);

        FILE* file_out = fopen(files[1].c_str(),"w+");
        fprintf(file_out, "%d ", 1);
        fclose(file_out);
    }

    return 0;
}