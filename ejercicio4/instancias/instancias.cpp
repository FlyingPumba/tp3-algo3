#include <vector>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main() {

	for (int i = 1; i <= 10; i++) {
    	char buffer[32];
    	snprintf(buffer, sizeof(char) * 32, "file%i.in", i);
		FILE* file = fopen(buffer,"a+");
		int n = rand() % 20 + 1; // 0 < n < 10001
		int m = n;
		fprintf(file, "%d\n", n);
        	while (m > 0) { // genero la instancia
			int u = rand() % n + 1; // -1 < n < 10001
			int v = rand() % n + 1;
			fprintf(file, "%d %d\n", u, v);
			m --;	
		}
		fclose(file);
	}



    	return 0;
}
