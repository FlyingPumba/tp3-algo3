#include <vector>
#include <iostream>
#include <stack>
using namespace std;

#define INFINITO -1

void resolver(vector< pair<int, int> > ejes);

// Implementacion.
int main() {
    int n, m;
    cin >> n;
    cin >> m;

    vector< pair<int, int> > ejes(m, make_pair(INFINITO, INFINITO));

    int v, w;
    for (int i = 0; i < n; i++) {
        cin >> v;
        cin >> w;
        ejes[i] = make_pair(v,w);
    }

    resolver(ejes);

    return 0;
}

void resolver(vector< pair<int, int> > ejes) {
    // Solución estupida:
    // 1. Creo A = conjunto de conjuntos dominantes de G
    // 2. Creo B = conjunto de conjuntos independientes de G
    // 3. Creo C = interseccion(A, B)
    // 4. Busco c elemento de C con menor cardinalidad
}
