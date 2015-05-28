#include <vector>
#include <iostream>
#include <stack>
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

void resolver(Mapa& mapa, pair<int, int> pos_actual, pair<int, int> bunker);
void imprimir_resultado(Mapa& mapa, pair<int, int> pos_inicial, pair<int, int> bunker);
void mostrar(Mapa& m); //debug
int sobrevivientes(int soldados, int zombies);

// Implementacion. Contiene el cargado de input mas la resolucion del ejercicio.
int main() {
    int n, m, s;
    cin >> n;
    cin >> m;
    cin >> s;

    pair<int, int> pos_inicial, bunker;
    cin >> pos_inicial.first;
    cin >> pos_inicial.second;
    cin >> bunker.first;
    cin >> bunker.second;

    Mapa mapa = Mapa(n, vector< Esquina >(m, Esquina()));

    for (int i = 0; i < n; i++) {
        for (int j = 1; j < m; j++){
            int c;
            cin >> c;
            mapa[i][j-1].right = c;
            mapa[i][j].left = c;
        }
        if (i < n-1){
            for (int j = 0; j < m; j++){
                int c;
                cin >> c;
                mapa[i][j].down = c;
                mapa[i+1][j].up = c;
            }
        }
    }

    mapa[pos_inicial.first][pos_inicial.second].sol_max = s;

    resolver(mapa, pos_inicial, bunker);
    
    imprimir_resultado(mapa, pos_inicial, bunker);
    cout << endl;   

    return 0;
}

void imprimir_resultado(Mapa& mapa, pair<int, int> pos_inicial, pair<int, int> bunker){
    cout << mapa[bunker.first][bunker.second].sol_max << endl;
    if (mapa[bunker.first][bunker.second].sol_max > 0){
        stack< pair<int,int> > esquinas;
        pair<int,int> actual = bunker;
        while (actual != pos_inicial){
            esquinas.push(actual);
            actual = mapa[actual.first][actual.second].pred;
        }
        esquinas.push(pos_inicial);
        while (esquinas.size() > 0){
            cout << esquinas.top().first << " " << esquinas.top().second << endl;
            esquinas.pop();
        }
    }
}

void mostrar(Mapa& m) {
    cout << endl;
    int n = m.size();
    for(int i = 0; i < n; ++i) {
        int t = m[i].size();
        for (int j = 0; j < t; j++) {
            if (m[i][j].sol_max == 0)
                cout << "X ";
            else
                cout << m[i][j].sol_max << " ";
        }
        cout << endl;
    }
}

int sobrevivientes(int soldados, int zombies){
    return soldados < zombies ? max(2*soldados-zombies, 0) : soldados;
}

void resolver(Mapa& mapa, pair<int, int> pos_actual, pair<int, int> bunker){
    if (pos_actual != bunker){
        int i = pos_actual.first;
        int j = pos_actual.second;
        Esquina actual = mapa[i][j];
        if (actual.left != INFINITO && sobrevivientes(actual.sol_max, actual.left) > mapa[i][j-1].sol_max){
            mapa[i][j-1].sol_max = sobrevivientes(actual.sol_max, actual.left);
            mapa[i][j-1].pred = make_pair(i,j);
            resolver(mapa, make_pair(i,j-1), bunker);
        }
        if (actual.right != INFINITO && sobrevivientes(actual.sol_max, actual.right) > mapa[i][j+1].sol_max){
            mapa[i][j+1].sol_max = sobrevivientes(actual.sol_max, actual.right);
            mapa[i][j+1].pred = make_pair(i,j);
            resolver(mapa, make_pair(i,j+1), bunker);
        }
        if (actual.up != INFINITO && sobrevivientes(actual.sol_max, actual.up) > mapa[i-1][j].sol_max){
            mapa[i-1][j].sol_max = sobrevivientes(actual.sol_max, actual.up);
            mapa[i-1][j].pred = make_pair(i,j);
            resolver(mapa, make_pair(i-1,j), bunker);
        }
        if (actual.down != INFINITO && sobrevivientes(actual.sol_max, actual.down) > mapa[i+1][j].sol_max){
            mapa[i+1][j].sol_max = sobrevivientes(actual.sol_max, actual.down);
            mapa[i+1][j].pred = make_pair(i,j);
            resolver(mapa, make_pair(i+1,j), bunker);
        }
    }
}
