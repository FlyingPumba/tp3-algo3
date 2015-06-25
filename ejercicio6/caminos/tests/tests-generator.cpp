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

         int m = n-1;



        cout << "TEST: " << c << ", n: " << c << ", m: " << m  << endl;

        fprintf(file_in, "%d %d\n",n , m );

         
         //PARA GRAFO COMPLETO   
         for (int i = 1; i < n; i++) {
                fprintf(file_in, "%d %d\n", i, i+1);
         } 

        fclose(file_in);

        FILE* file_out = fopen(files[1].c_str(),"w+");
        if(n%2 == 0){
            fprintf(file_out, "%d ", n/2);
        }else{
            fprintf(file_out, "%d ", (n-1)/2);
        }
        fclose(file_out);
    }

    return 0;
}