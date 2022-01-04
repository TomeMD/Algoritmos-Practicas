#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>


#define K 10000			// iteraciones del algoritmo en caso de no llegar al umbral de confianza
#define L 128000	// maximo valor de n
#define N 500			// minimo valor de n

double microsegundos();
double tiempo( int size , void ord(), void generarVector());
void cabecera(char* stringFuncion, char* tipoVector, char* cotas[]);
void tabla(void ord(), void generarVector(), int i, double cotas[]);
void testInsercion();
void testShell();
void ord_ins (int v [], int n);
void ord_shell (int v [], int n);
void inicializar_semilla();
void aleatorio(int v [], int n);
void listar_vector(int v[], int n);
bool ordenado(int v [], int n);
void ascendente(int v [], int n);
void descendente(int v [], int n);
void testAleatorio(int v [], int n, void ord(), char* stringFuncion);
void testDescendente(int v [], int n, void ord(), char* stringFuncion);
void testAscendente(int v [], int n, void ord(), char* stringFuncion);
void test1();



int main()
{
	inicializar_semilla();
	test1();
	testInsercion(); // calentamos la maquina inicialmente
	testInsercion();
	testShell();

	return 0;
}



double microsegundos() { /* obtiene la hora del sistema en microsegundos */
struct timeval t;
if (gettimeofday(&t, NULL) < 0 )
return 0.0;
return (t.tv_usec + t.tv_sec * 1000000.0);
}


double tiempo( int size , void ord(), void generarVector()) {
	
	double ta,tb,tiempo,t3,t4;
	int vector[size];
	
	generarVector(vector,size);
	ta = microsegundos();
	ord(vector,size);
	tb = microsegundos();
	tiempo = tb - ta;
		
	if (tiempo < 500 ){ // umbral de confianza
		printf("*");
		ta = microsegundos();
		for(int j = 0; j < K; j++){
			generarVector(vector,size);
			ord(vector,size);
		}
		tb = microsegundos();
		t3 = tb-ta;
		ta = microsegundos();
		for(int j = 0; j< K ; j++){
			generarVector(vector,size); // se resta el tiempo de ejecución gastado en generar un vector aleatorio K veces
		}
		tb=microsegundos();
		t4 = tb -ta;
		tiempo = (t3-t4)/K;
	}
	
	return tiempo;
}

void cabecera(char* stringFuncion, char* tipoVector, char* cotas[]) {
	
	printf("\n");
	printf("Ordenacion por %s con inicializacion %s\n", stringFuncion, tipoVector);
	printf("\n");
	printf("%11s","n");
	
	// ajustamos el formato de la tabla a cada caso
	
	if (strcmp(tipoVector, "ascendente") == 0) {
			printf("%16s","t(n)");
	} else {printf("%14s","t(n)");} 

	
	if ((strcmp(stringFuncion, "Insercion") == 0) && (strcmp(tipoVector, "ascendente") == 0)) {
		printf("%13s%s","t(n)/", cotas[0]);
	} else if ((strcmp(stringFuncion, "Shell") == 0) && (strcmp(tipoVector, "aleatoria") == 0)){
		printf("%16s%s","t(n)/", cotas[0]);
	} else {printf("%14s%s","t(n)/", cotas[0]);}
	
	printf("%11s%s","t(n)/", cotas[1]);
	    
	if (strcmp(stringFuncion, "Shell") == 0) {
			printf("%8s%s","t(n)/", cotas[2]);		
	} else if (strcmp(tipoVector, "ascendente") == 0) {
		printf("%11s%s","t(n)/", cotas[2]);
	} else {printf("%10s%s","t(n)/", cotas[2]);}
	
	printf("\n");

}

void tabla(void ord(), void generarVector(), int i, double cotas[]) {
	
	if(cotas[0] < 500) {printf("%10d",i);} else {printf("%11d",i);}
	printf("%17.4lf",cotas[0]);
	printf("%15.6lf",cotas[0]/cotas[1]);
	printf("%15.6lf",cotas[0]/cotas[2]);
	printf("%14.7lf\n",cotas[0]/cotas[3]);
}

void testInsercion() {
	
	int i; double t; double cotas[3]; char * sCotas[3];
	
	/*insercion aleatoria y descendente se estudian hasta n = 64000
		ya que no es un algoritmo muy rápido y no es su mejor caso(ascendente)*/ 
	
	sCotas[0] = "n^1.8"; sCotas[1] = "n^2"; sCotas[2] = "n^2.2";
	cabecera("Insercion", "aleatoria", sCotas);
	for (i = N ; i < L ; i = i*2){
		t = tiempo(i, ord_ins, aleatorio);
		cotas[0] = t;cotas[1] = pow(i, 1.8);cotas[2] = pow(i, 2);cotas[3] = pow(i, 2.2);
		tabla(ord_ins, aleatorio, i, cotas);	
	}
	
	sCotas[0] = "n^1.8"; sCotas[1] = "n^2"; sCotas[2] = "n^2.2";
	cabecera("Insercion", "descendente", sCotas);
	for (i = N ; i < L ; i = i*2){
		t = tiempo(i, ord_ins, descendente);
		cotas[0] = t;cotas[1] = pow(i, 1.8);cotas[2] = pow(i, 2);cotas[3] = pow(i, 2.2);
		tabla(ord_ins, descendente, i, cotas);	
	}
	
	sCotas[0] = "n^0.5"; sCotas[1] = "n"; sCotas[2] = "n^1.2";
	cabecera("Insercion", "ascendente", sCotas);
	for (i = N ; i <=L ; i = i*2){
		t = tiempo(i, ord_ins, ascendente);
		cotas[0] = t;cotas[1] = pow(i, 0.5);cotas[2] = pow(i, 1);cotas[3] = pow(i, 1.2);
		tabla(ord_ins, ascendente, i, cotas);	
	}	
}

void testShell() {
	
	int i; double t; double cotas[3]; char * sCotas[3];
	
	sCotas[0] = "n"; sCotas[1] = "n^1.2"; sCotas[2] = "n^1.4";
	cabecera("Shell", "aleatoria", sCotas);
	for (i = N ; i <=L ; i = i*2){
		t = tiempo(i, ord_shell, aleatorio);
		cotas[0] = t;cotas[1] = pow(i,1);cotas[2] = pow(i,1.2);cotas[3] = pow(i, 1.4);
		tabla(ord_shell, aleatorio, i, cotas);	
	}
	
	sCotas[0] = "n^0.8"; sCotas[1] = "n^1.1"; sCotas[2] = "n^1.6";
	cabecera("Shell", "descendente", sCotas);
	for (i = N ; i <=L ; i = i*2){
		t = tiempo(i, ord_shell, descendente);
		cotas[0] = t;cotas[1] = pow(i, 0.8);cotas[2] = pow(i, 1.1);cotas[3] = pow(i, 1.6);
		tabla(ord_shell, descendente, i, cotas);	
	}
	
	sCotas[0] = "n"; sCotas[1] = "n^1.13"; sCotas[2] = "n^1.5";
	cabecera("Shell", "ascendente", sCotas);
	for (i = N ; i <=L ; i = i*2){
		t = tiempo(i, ord_shell, ascendente);
		cotas[0] = t;cotas[1] = pow(i, 1);cotas[2] = pow(i,1.13);cotas[3] = pow(i, 1.5);
		tabla(ord_shell, ascendente, i, cotas);	
	}

}

void ord_ins(int v [], int n) {

	int x, j, i;
	
	for (i = 1; i < n; i++) {
		x = v[i];
		j = i-1;
		while ((j >= 0) && (v[j] > x)) {
			v[j+1] = v[j];
			j = j-1;
		}
		v[j+1] = x;
	}	
}

void ord_shell(int v [], int n) {
	
	int incremento = n, tmp, j, i;
	bool seguir;
	div_t division;
	
	do {
		division = div(incremento, 2);
		incremento = division.quot;
		for (i = incremento+1; i < n; i++) {
			tmp = v[i];
			j = i;
			seguir = true;
			while ((j - incremento >= 0) && seguir) {
				if (tmp < v[j -incremento]) {
					v[j] = v[j-incremento];
					j = j-incremento;
				} else {seguir = false;}
			}
			v[j] = tmp;
		}
	} while (incremento != 1);
}

void listar_vector(int v[], int n) {
	int i;
	
	for (i = 0; i < n; i++) printf("%4i", v[i]);
	printf("\n");
}

void inicializar_semilla() {
	srand(time(NULL));
	/* se establece la semilla de una nueva serie de enteros pseudo-aleatorios */
}

void aleatorio(int v [], int n) {
	int i, m=2*n+1;
	
	for (i=0; i < n; i++)
		v[i] = (rand() % m) - n;
	/* se generan números pseudoaleatorio entre -n y +n */
}

bool ordenado(int v [], int n) {
	
	int x, i, j;
	bool estaOrdenado = true;
	
	for (i = 0; i < n; i++) {
		x = v[i];
		for (j = i+1; j < n; j++) {
			if (x > v[j]) {estaOrdenado = false;}
		}
	}
	
	return estaOrdenado;
}

void ascendente(int v [], int n) {
	int i;
	for (i=0; i < n; i++) v[i] = i + 1;
}

void descendente(int v [], int n) {
	int i, j = 0;
	for (i=n; (i >= 0) && (j < n); i--) {
		v[j] = i;
		j++;
	}
}

void testAleatorio(int v [], int n, void ord(), char* stringFuncion) {
	
	printf("Inicializacion aleatoria\n");
	aleatorio(v, n);
	listar_vector(v, n);
	printf("ordenado? %i\n",ordenado(v, n));
	printf("Ordenacion por %s\n", stringFuncion);
	ord(v, n);
	listar_vector(v, n);
	printf("ordenado? %i\n",ordenado(v, n));
	
	printf("\n\n");
}

void testDescendente(int v [], int n, void ord(), char* stringFuncion) {
	
	printf("Inicializacion descendente\n");
	descendente(v, n);
	listar_vector(v, n);
	printf("ordenado? %i\n",ordenado(v, n));
	printf("Ordenacion por %s\n", stringFuncion);
	ord(v, n);
	listar_vector(v, n);
	printf("ordenado? %i\n",ordenado(v, n));
	
	printf("\n\n");
}

void testAscendente(int v [], int n, void ord(), char* stringFuncion) {
	
	printf("Inicializacion ascendente\n");
	ascendente(v, n);
	listar_vector(v, n);
	printf("ordenado? %i\n",ordenado(v, n));
	printf("Ordenacion por %s\n", stringFuncion);
	ord(v, n);
	listar_vector(v, n);
	printf("ordenado? %i\n",ordenado(v, n));
	
	printf("\n\n");
}

void test1() {
	
	int v[17];
	int	ascDesc[10];
	
	testAleatorio(v, 17, ord_ins, "Insercion");
	testDescendente(ascDesc, 10, ord_ins, "Insercion");
	testAscendente(ascDesc, 10, ord_ins, "Insercion");
	
	testAleatorio(v, 17, ord_shell, "Shell");
	testDescendente(ascDesc, 10, ord_shell, "Shell");
	testAscendente(ascDesc, 10, ord_shell, "Shell");
}

