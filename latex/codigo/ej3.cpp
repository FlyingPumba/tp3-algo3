#include <vector>
#include <iostream>
#include <queue>
using namespace std;

class DisjointSet    {
    int *id, cantidadDeArboles, *rank;
    
public:
    
    DisjointSet(int N)   {
        cantidadDeArboles = N;

        id = new int[N];
        rank = new int[N];

        for(int i=0; i<N; i++)  {
            id[i] = i;
            rank[i] = 1;
        }
    }
    ~DisjointSet()   {
    delete [] id;
    delete [] rank;
    }

    // Devuelve el id de la componente a la que pertenece p
    int buscar(int p) {
        int raiz = p;
        while (raiz != id[raiz])
            raiz = id[raiz];
        while (p != raiz) {
            int newp = id[p];
            id[p] = raiz;
            p = newp;
        }
        return raiz;
    }
    // mergea dos sets
    void unir(int x, int y)    {
        int i = buscar(x);
        int j = buscar(y);
        if (i == j) return;
        
        //hace que la raiz del menor apunte a la del mayor
        if(rank[i] < rank[j]){
            id[i] = j;
        }else{
            id[j] = i;
            if(rank[i] == rank[j]){
                rank[i]++;
            }   
        }
        cantidadDeArboles--;
    }

    //devuelve true si dos pozos estan en el mismo arbol
    bool conectados(int x, int y)    {
        return buscar(x) == buscar(y);
    }

    // Devuelve la cantidad de arboles
    int count() {
        return cantidadDeArboles;
    }

};



struct Tuberia {
    Tuberia(): desde(0), hasta(0), costo(0){}
    Tuberia(const int d,const int h,const int c) : desde(d), hasta(h), costo(c) {};

    // desde que pozo va la tuberia
    int desde;

    //hasta que pozo va la tuberia
    int hasta;

    //costo de la construccion
    int costo;
};

struct compararPorCostos
{
  bool operator()(const Tuberia& izq, const Tuberia& der) const
  {
    return izq.costo > der.costo;
  }
};

typedef vector<Tuberia> Vec;

// Prototipado de funciones


// Implementacion. Contiene el cargado de input mas la resolucion del ejercicio.
int main() {
    // cantidad de pozos
    int n;
    cin >> n;

    // cantidad de pares de pozos en los cuales se puede construir una tuberia
    int m;
    cin >> m;

    // costo de construir una refineria
    int c;
    cin >> c;

    // cargo las potenciales tuberias en una cola de prioridad y guardo en un vector si puedo 
    //acceder por tuberia o necesito refineria si o si.
    std::priority_queue<Tuberia, std::vector<Tuberia>, compararPorCostos> tuberias;

    //defino el costo total como el costo de poner una refineria por pozo
    int costoTotal = n*c;

    //O(m*log(m))
    for(int i = 0; i < m; i++) {
        int desde, hasta, costo;
        cin >> desde;
        cin >> hasta;
        cin >> costo;

        //solo agrego las tuberias que tengan costo menor a la refineria ya que
        //solo es conveniente por costos hacerlo si es asi

        if(costo < c){
            tuberias.push(Tuberia(desde,hasta,costo));
        }
    }

    DisjointSet componentes(n);

    //en este vector se guardaran las tuberias que se vayan construyendo, tiene como maximo
    //el tamano de tuberias que agregue a tuberias, es decir, la cantidad de tuberias con costo menor a c
    vector<Tuberia> tuberiasConstruidas;
    tuberiasConstruidas.reserve(tuberias.size());

    cout << endl;
    while(!tuberias.empty()){
        Tuberia t = tuberias.top();
        tuberias.pop();

        //busco con -1 para que encuentre el indice correcto en el UF
        if(!componentes.conectados(t.desde - 1,t.hasta - 1)){

            //uno las componentes en el disjoint set
            componentes.unir(t.desde - 1,t.hasta - 1);

            //agrego la tuberia a tuberias construidas
            tuberiasConstruidas.push_back(t);

            //bajo el costo total
            costoTotal = costoTotal - c + t.costo;
        }
    }

    cout << costoTotal << " " <<  componentes.count() << " " << tuberiasConstruidas.size() << endl;

    //imprimo los pozos que tienen refineria
    //defino que un pozo tiene refineria si es la raiz de su componente
    for (int i = 0; i < n; i++){
        if(componentes.buscar(i) == i){
            cout << i + 1 << " ";
        }
    }

    //imprimo las tuberias que se construyeron
    cout << endl;
    for(int i = 0; i < tuberiasConstruidas.size();i++){
        cout << tuberiasConstruidas[i].desde << " " << tuberiasConstruidas[i].hasta << endl;
    }

}


