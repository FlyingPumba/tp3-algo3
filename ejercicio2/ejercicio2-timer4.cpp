#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <stack>
#include <ctime>
#include <chrono>

using namespace std;

#define INFINITO -1

struct Esquina{
    int sol_max;
    int left;
    int right;
    int up;
    int down;
    pair<int, int> pred;

    Esquina() : sol_max(0), left(INFINITO), right(INFINITO), up(INFINITO), down(INFINITO), pred(make_pair(INFINITO,INFINITO)){};
};

typedef vector< vector < Esquina > > Mapa;

void resolver(Mapa& map, pair<int, int> pos_actual, pair<int, int> bunker, int& count);
void imprimir_resultado(Mapa& map, pair<int, int> pos_inicial, pair<int, int> bunker);
int sobrevivientes(int soldados, int zombies);
int random_in_range(int min, int max);

// Tiempos

typedef vector< vector< vector<double> > > Tabla;

// Parametros

int n, m, s;
pair<int, int> p_inicial, p_bunker;
Mapa mapa;

// Funciones

int random_in_range(int min, int max)
{
  return min + (rand() % (max - min + 1));
}

int main() {
    FILE* file = fopen("tiempos.txt","w+");

    int muestras = 20;
    int tamano = 100;
    int s_min = 1;
    int s_max = tamano/2;
    int n = tamano;
    int m = tamano;

    vector<double> tiempos = vector<double>(s_max-s_min+1);

    for (int muestra = 1; muestra <= muestras; muestra++) {
		for (int s = 1; s < s_max; s++) {
			mapa = Mapa(n, vector< Esquina >(m, Esquina()));
			for (int i = 0; i < n; i++) {
                mapa[i][0].right = tamano-i;
                mapa[i][1].left = tamano-i;
		        for (int j = 2; j < m; j++){
		            mapa[i][j-1].right = 1;
		            mapa[i][j].left = 1;
		        }
		        if (i < n-1){
		            for (int j = 0; j < m; j++){
		                mapa[i][j].down = 1;
		                mapa[i+1][j].up = 1;
		            }
		        }
		    }

		    p_inicial.first = 0;
		    p_inicial.second = 0;
		    p_bunker.first = n-1;
		    p_bunker.second = m-1;
			mapa[p_inicial.first][p_inicial.second].sol_max = s;
			int count = 0;

			std::chrono::time_point<std::chrono::high_resolution_clock> t1 = std::chrono::high_resolution_clock::now();
	        resolver(mapa, p_inicial, p_bunker, count);
	        std::chrono::time_point<std::chrono::high_resolution_clock> t2 = std::chrono::high_resolution_clock::now();
	        
		    tiempos[s-1] += double(std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1).count());
		}
    }

    fprintf(file, "s tiempo\n");

    for (int s = 1; s < s_max; s++) {
        fprintf(file, "%d %7.8f\n", s, (double)(tiempos[s-1]/(double)muestras));
    }

    fclose(file);

    return 0;
}

void imprimir_resultado(Mapa& map, pair<int, int> pos_inicial, pair<int, int> bunker){
    cout << map[bunker.first][bunker.second].sol_max << endl;
    if (map[bunker.first][bunker.second].sol_max > 0){
        stack< pair<int,int> > esquinas;
        pair<int,int> actual = bunker;
        while (actual != pos_inicial){
            esquinas.push(actual);
            actual = map[actual.first][actual.second].pred;
        }
        esquinas.push(pos_inicial);
        while (esquinas.size() > 0){
            cout << esquinas.top().first << " " << esquinas.top().second << endl;
            esquinas.pop();
        }
    }
}

int sobrevivientes(int soldados, int zombies){
    return soldados < zombies ? max(2*soldados-zombies, 0) : soldados;
}

void resolver(Mapa& map, pair<int, int> pos_actual, pair<int, int> bunker, int& count){
	count++;
    int i = pos_actual.first;
    int j = pos_actual.second;

	// cout << "llegue " << endl;
	// cout << map.size() << " " << map[0].size() << endl;
	// cout << i << " " << j << endl;
	// cout << "llegue " << endl;
    
    Esquina actual = map[i][j];
    if (actual.left != INFINITO && sobrevivientes(actual.sol_max, actual.left) > map[i][j-1].sol_max){
        map[i][j-1].sol_max = sobrevivientes(actual.sol_max, actual.left);
        map[i][j-1].pred = make_pair(i,j);
		// cout << "left " << endl;
        resolver(map, make_pair(i,j-1), bunker, count);
    }
    if (actual.right != INFINITO && sobrevivientes(actual.sol_max, actual.right) > map[i][j+1].sol_max){
        map[i][j+1].sol_max = sobrevivientes(actual.sol_max, actual.right);
        map[i][j+1].pred = make_pair(i,j);
		// cout << "right " << endl;
        resolver(map, make_pair(i,j+1), bunker, count);
    }
    if (actual.up != INFINITO && sobrevivientes(actual.sol_max, actual.up) > map[i-1][j].sol_max){
        map[i-1][j].sol_max = sobrevivientes(actual.sol_max, actual.up);
        map[i-1][j].pred = make_pair(i,j);
		// cout << "up " << endl;
        resolver(map, make_pair(i-1,j), bunker, count);
    }
    if (actual.down != INFINITO && sobrevivientes(actual.sol_max, actual.down) > map[i+1][j].sol_max){
        map[i+1][j].sol_max = sobrevivientes(actual.sol_max, actual.down);
        map[i+1][j].pred = make_pair(i,j);
		// cout << "down " << endl;
        resolver(map, make_pair(i+1,j), bunker, count);
    }
}
