#include <fstream>

using namespace std;

int main(){
	FILE* file = fopen("tiempos-modified.txt","w+");
	ifstream infile;
	infile.open("tiempos.txt");
	int s, n, m;
	double value;
	while (infile >> s >> n >> m >> value){
	    fprintf(file, "%d %d %d %7.8f\n", s, n, m, value/s);
	}
    fclose(file);
    return 0;
}