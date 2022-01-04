#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>


#define TAM 256000
#define K 10000			// iteraciones del algoritmo en caso de no llegar al umbral de confianza
#define L 128000		// maximo valor de n
#define N 500			// minimo valor de n

typedef struct {
	int vector[TAM];
	int ultimo;
} monticulo;	

void inicializarMonticulo(monticulo *m);
bool monticuloVacio(const monticulo *m);
void hundir(monticulo *m, int i);
void crearMonticulo(int v [], int n, monticulo *m);
int consultarMenor(const monticulo *m);
void quitarMenor(monticulo *m);
void ordMonticulo(int v[], int n);
double microsegundos();
void inicializar_semilla();
void aleatorio(int v [], int n);
void ascendente(int v [], int n);
void descendente(int v [], int n);
double tiempo(int size, void funcion(), void generarVector());
double tiempo2(int size , void funcion(), void generarVector());
void listar_vector(int v[], int n);
void testMonticulo();
void complejidadCrearMonticulo();
void testOrdMonticulo();
void complejidadOrdMonticulo();


int main()
{
	inicializar_semilla();
	testMonticulo();
	complejidadCrearMonticulo(); //Calentamos la maquina
	complejidadCrearMonticulo();
	complejidadCrearMonticulo();
	testOrdMonticulo();
	complejidadOrdMonticulo();
	
}


void inicializarMonticulo(monticulo *m) {
	m->ultimo = -1;
}


bool monticuloVacio(const monticulo *m) {
	return (m->ultimo == -1);
}


void hundir(monticulo *m, int i) {
	int HijoIzq;
	int HijoDer;
	int x, j;
	
	do {
		HijoIzq = 2*i+1;
		HijoDer = 2*i+2;
		j = i;
		
		if ((HijoDer <= m->ultimo) && (m->vector[HijoDer] < m->vector[i]))
			i = HijoDer;
		if ((HijoIzq <= m->ultimo) && (m->vector[HijoIzq] < m->vector[i]))
			i = HijoIzq;
		x = m->vector[j];
		m->vector[j] = m->vector[i];
		m->vector[i] = x;
	} while (j != i);		
}


void crearMonticulo(int v[], int n, monticulo *m) {
	int i;
	
	if (n < TAM) {
		for (i = 0; i < n; i++) {
			m->vector[i] = v[i];
		}
		m->ultimo = n-1;
		
		
		for (i = m->ultimo/2; i >= 0; i--) {
			hundir(m, i);
		}
	}
}


int consultarMenor(const monticulo *m) {
	if (!monticuloVacio(m)) 
		return m->vector[0];
	else return printf("Error: Monticulo Vacio");
}


void quitarMenor(monticulo *m) {
	
	if (monticuloVacio(m)) printf("Error: Monticulo Vacio");
	else {
		m->vector[0] = m->vector[m->ultimo];
		m->ultimo -= 1;
		if (m->ultimo > -1) hundir(m, 0);
	}
}


void ordMonticulo(int v[], int n) {
	monticulo m;
	int i;
	
	crearMonticulo(v, n, &m);
	for(i = 0; i < n; i++) {
		v[i] = consultarMenor(&m);
		quitarMenor(&m);
	}
}


double microsegundos() { /* obtiene la hora del sistema en microsegundos */
	struct timeval t;
	if (gettimeofday(&t, NULL) < 0 )
		return 0.0;
	return (t.tv_usec + t.tv_sec * 1000000.0);
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


double tiempo(int size , void funcion(), void generarVector()) {
	
	double ta,tb,tiempo,t3,t4;
	int j;
	int vector[size];
	monticulo m;
	
	inicializarMonticulo(&m);
	generarVector(vector, size);
	ta = microsegundos();
	funcion(vector, size, &m);
	tb = microsegundos();
	tiempo = tb - ta;
		
	if (tiempo < 500 ){ // umbral de confianza
		printf("*");
		ta = microsegundos();
		for(j = 0; j < K; j++){
			generarVector(vector,size);
			funcion(vector, size, &m);
		}
		tb = microsegundos();
		t3 = tb-ta;
		ta = microsegundos();
		for(j = 0; j< K ; j++){
			generarVector(vector,size); // se resta el tiempo de ejecución gastado en generar un vector aleatorio K veces
		}
		tb=microsegundos();
		t4 = tb -ta;
		tiempo = (t3-t4)/K;
	} else printf("%1s", "");
	
	return tiempo;
}

double tiempo2(int size , void funcion(), void generarVector()) {
	
	double ta,tb,tiempo,t3,t4;
	int j;
	int vector[size];

	
	generarVector(vector, size);
	ta = microsegundos();
	funcion(vector, size);
	tb = microsegundos();
	tiempo = tb - ta;
		
	if (tiempo < 500 ){ // umbral de confianza
		printf("*");
		ta = microsegundos();
		for(j = 0; j < K; j++){
			generarVector(vector,size);
			funcion(vector, size);
		}
		tb = microsegundos();
		t3 = tb-ta;
		ta = microsegundos();
		for(j = 0; j< K ; j++){
			generarVector(vector,size); // se resta el tiempo de ejecución gastado en generar un vector aleatorio K veces
		}
		tb=microsegundos();
		t4 = tb -ta;
		tiempo = (t3-t4)/K;
	} else printf("%1s", "");
	
	return tiempo;
}

void listar_vector(int v[], int n) {
	int i;
	
	for (i = 0; i < n; i++) printf("%4i", v[i]);
	printf("\n");
}

void testMonticulo() {
	int v[10], i;
	monticulo m;
	
	aleatorio(v, 10);
	printf("Vector aleatorio de 10 elementos\n"); 
	listar_vector(v, 10); //Generamos un vector aleatorio de 10 elementos
	crearMonticulo(v, 10, &m); //Creamos un monticulo de minimos con dicho vector
	printf("Creamos un monticulo con el vector dado\n");
	listar_vector(m.vector, 10);
	printf("Vamos sacando el menor hasta que no haya monticulo...\n");
	for (i = 0; i < 10; i++) {
		printf("%4i", consultarMenor(&m)); //Vamos sacando el minimo uno a uno para comprobar que 
		quitarMenor(&m);				   //el monticulo elimina y hunde correctamente
	}
	printf("\nVector ordenado: el monticulo se crea, elimina y hunde correctamente");
	printf("\n");
	
	
}


void complejidadCrearMonticulo() {
	
	double t;
	int i;
	
	printf("\n");
	printf("Demostracion empirica de que crearMonticulo se ejecuta en tiempo O(n)\n\n");
	printf("%11s%15s%18s%13s%15s\n","n", "t(n)", "t(n)/n^0.7 ", "t(n)/n", "t(n)/n^1.3");

	for (i = N; i <=L ; i = i*2){
		t = tiempo(i, crearMonticulo, aleatorio); //testeamos crearMonticulo con vectores aleatorios(caso medio)...			
		printf("%11d%15.6lf%15.6lf%15.6lf%14.6lf\n",i, t, t/(pow(i,0.7)), t/(pow(i,1)), t/(pow(i,1.3)));
	}
}

void testOrdMonticulo() {
	int v[10];
	
	printf("\n\n");
	printf("Funcionamiento de ordenarMonticulo\n");
	aleatorio(v, 10); listar_vector(v, 10);
	printf("NO ordenado\n");
	ordMonticulo(v, 10); listar_vector(v, 10);
	printf("ordenado ¡FUNCIONA!\n\n");
}

void complejidadOrdMonticulo() {
	double t;
	int i;
	
	printf("Ordenacion por monticulos con inicializacion ascendente\n\n");
	printf("%11s%15s%15s%19s%12s\n","n", "t(n)", "t(n)/n", "t(n)/n*log(n)", "t(n)/n^1.3");
	for (i = N; i <=L ; i = i*2){
		t = tiempo2(i, ordMonticulo, ascendente);			
		printf("%11d%15.6lf%15.6lf%15.6lf%14.6lf\n",i, t, t/(pow(i,1)), t/(i*log(i)), t/(pow(i,1.3)));
	}
	printf("\n");
	
	printf("Ordenacion por monticulos con inicializacion descendente\n\n");
	printf("%11s%15s%15s%19s%12s\n","n", "t(n)", "t(n)/n", "t(n)/n*log(n)", "t(n)/n^1.3");
	for (i = N; i <=L ; i = i*2){
		t = tiempo2(i, ordMonticulo, descendente);			
		printf("%11d%15.6lf%15.6lf%15.6lf%14.6lf\n",i, t, t/(pow(i,1)), t/(i*log(i)), t/(pow(i,1.3)));
	}
	printf("\n");
	
	printf("Ordenacion por monticulos con inicializacion aleatoria\n\n");
	printf("%11s%15s%15s%19s%12s\n","n", "t(n)", "t(n)/n", "t(n)/n*log(n)", "t(n)/n^1.3");
	for (i = N; i <=L ; i = i*2){
		t = tiempo2(i, ordMonticulo, aleatorio);			
		printf("%11d%15.6lf%15.6lf%15.6lf%14.6lf\n",i, t, t/(pow(i,1)), t/(i*log(i)), t/(pow(i,1.3)));
	}
	printf("\n");
	
}

/****************************************************************************************************

void flotar(monticulo* m, int i) {
	int x;
	
	while ((i > 0) && (m->vector[(i-1)/2] > m->vector[i])) {
		x = m->vector[i];
		m->vector[i] = m->vector[(i-1)/2];
		m->vector[(i-1)/2] = x;
		i = (i-1)/2;
	}
}

void insertarMonticulo(int x, monticulo* m){

	if (m->ultimo >= TAM) {
		printf("Error: monticulo lleno\n");
	} else {
		m->ultimo++;
		m->vector[m->ultimo] = x;
		flotar(m, m->ultimo);
	}
}

int KesimoMayor(int v[], int n, int k) {
	monticulo m;
	int i;
	crearMonticulo(v, k, &m);
	for (i = k; i < n; i++) {
		if (consultarMenor(&m) < v[i]) {
			quitarMenor(&m);
			insertarMonticulo(v[i], &m);
		}
	}
	return consultarMenor(&m);	
}

****************************************************************************************************/
