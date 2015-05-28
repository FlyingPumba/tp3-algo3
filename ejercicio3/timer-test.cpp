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


int random_in_range(int min, int max)
{
  return min + (rand() % (max - min + 1));
}


// Implementacion. Contiene el cargado de input más la resolución del ejercicio.
int main() {

FILE* file = fopen("tiempos.txt","w+");

    int MUESTRAS = 20;
    int N_MAX = 1000;
    int C_MAX = 1000;

    // cargo las potenciales tuberias en una cola de prioridad y guardo en un vector si puedo 
    //acceder por tuberia o necesito refineria si o si.
    std::priority_queue<Tuberia, std::vector<Tuberia>, compararPorCostos> tuberias;

    for(int n = 2; n < N_MAX + 2; n ++){
        double sum = 0;
        for(int muestra = 0; muestra < MUESTRAS; muestra++){
            int m = random_in_range(1,(n*(n-1)/2));
            int c = random_in_range(1,C_MAX);

            for(int tu = 0; tu < m; tu++){
                 int desde = random_in_range(1,n);
                 int hasta = random_in_range(1,n);
                    //para que no tenga origen y destino en el mismo pozo
                if(desde == hasta){
                        if(hasta < n){
                            hasta++;
                        }else{
                            hasta--;
                        }
                    }

                int costo = (rand()%C_MAX)+1;
                if(costo < c){
                     tuberias.push(Tuberia(desde,hasta,costo));
                }
            }


            std::chrono::time_point<std::chrono::high_resolution_clock> t1 = std::chrono::high_resolution_clock::now();
            int costoTotal = n*c;
            DisjointSet componentes(n);

            vector<Tuberia> tuberiasConstruidas;
            tuberiasConstruidas.reserve(tuberias.size());

            while(!tuberias.empty()){
                Tuberia t = tuberias.top();
                tuberias.pop();
                // cout << t.desde << " " << t.hasta  << " " << t.costo << endl;

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
            std::chrono::time_point<std::chrono::high_resolution_clock> t2 = std::chrono::high_resolution_clock::now();
            sum += double(std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1).count());
            cout << n << " " << muestra << endl;
        }
        double prom = sum / double(MUESTRAS);
        fprintf(file, "%d %7.8f\n", n, prom);
    }

}


