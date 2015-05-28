#include <fstream>

using namespace std;

int main(){
	FILE* file = fopen("tiempos-modified.txt","w+");
	ifstream infile;
	infile.open("tiempos.txt");
	int i;
	double value;
	infile.ignore(256,'\n'); 
	while (infile >> i >> value){
	    fprintf(file, "%d %7.8f\n", i, value/i);
	}
    fclose(file);
    return 0;
}