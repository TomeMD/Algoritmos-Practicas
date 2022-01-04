#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#define MAX 5                   // tamaño de las secuencias dadas
#define L 32000                 // maximo valor de n
#define k 1000                  // iteraciones del algoritmo en caso de no llegar al umbral de confianza
#define N 500                   // minimo valor de n



double microsegundos();
int sumaSubMax1(int v[], int n);
int sumaSubMax2(int v[], int n);
void inicializar_semilla();
void aleatorio(int v [], int n);
void ImprimirSec(int listado[]);
void test1();
void listar_vector(int v[], int n);
void test2();
double tiempo();
void tabla1();
void tabla2();


int main() 
{
	printf("%f\n\n\n", log(2));
	inicializar_semilla();
	test1();
	test2();
	tabla1();
	tabla2();

	return 0;
}


double microsegundos() { /* obtiene la hora del sistema en microsegundos */
	struct timeval t;
	
	if (gettimeofday(&t, NULL) < 0 )
	return 0.0;
	
	return (t.tv_usec + t.tv_sec * 1000000.0);
}


int sumaSubMax1(int v[], int n) {
	int estaSuma, sumaMax = 0;
	
	for (int i = 0; i < n; i++) {
		estaSuma = 0;
		for (int j = i; j < n; j++) {
			estaSuma += v[j];
			if (estaSuma > sumaMax) {
				sumaMax = estaSuma;
			}
		}	
	}
	
	return sumaMax;
}


int sumaSubMax2(int v[], int n) {
	int estaSuma = 0, sumaMax = 0;
	
	for (int j = 0; j < n; j++) {
		estaSuma += v[j];
		if (estaSuma > sumaMax) {
			sumaMax = estaSuma;
		} else if (estaSuma < 0) {
			estaSuma = 0;
		} 
		
	}
	
	return sumaMax;
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


void ImprimirSec(int listado[]) {
	int sec[MAX];
	int j = 0, count = 0;
	
	printf("%20s%15s%15s\n", "", "sumaSubMax1", "sumaSubMax2");
	
	for (int i = 0; i < 30; i++) { 
		sec[j] = listado[i];
		if (j == 4) {                      // cada 5 elementos recorridos en el array y 
			count += 1;                    // almacenados en la secuencia, ejecutamos el algoritmo
			listar_vector(sec, MAX);
			printf("%15d%15d\n", sumaSubMax1(sec, MAX), sumaSubMax2(sec, MAX));
			j = 0;
		} else {
			j += 1;
		}
	}
	printf("\n");
}


void test1() {
	
	int listado[] = {-9, 2, -5, -4, 6, 4, 0, 9, 2, 5, -2, -1, -9, -7, 
					-1, 9, -2, 1, -7, -8, 15, -2, -5, -4, 16, 7, -5, 6, 7, -7};
					
	// array con las 6 secuencias concatenadas
	printf("Test 1\n");
	ImprimirSec(listado);
}


void listar_vector(int v[], int n) {
	
	for (int i = 0; i < n; i++) {
		printf("%3i", v[i]);
	}
}


void test2() {
	
	int a, b;
	int v[9];
	
	printf("Test 2 \n");
	printf("%33s%15s%15s\n", "", "sumaSubMax1", "sumaSubMax2");
	for (int i = 0; i < 10; i++) {
		aleatorio(v, 9);
		listar_vector(v, 9);
		a = sumaSubMax1(v, 9);
		b = sumaSubMax2(v, 9);
		printf("%15d%15d\n", a, b);
	}
	
}

double tiempo(int size, int sumaSubMax()) {
	
	double t1,t2,tiempo,t3,t4;
	int vector[size];
	
	aleatorio(vector,size);
	t1 = microsegundos();
	sumaSubMax(vector,size);
	t2 = microsegundos();
	tiempo = t2 -t1;
		
	if (tiempo < 500 ){ // umbral de confianza
		printf("*");
		
		t1 = microsegundos();
		for(int j = 0; j < k; j++){
			aleatorio(vector,size);
			sumaSubMax(vector,size);
		}	
		t2 = microsegundos();
		
		t3 = t2-t1;
		
		t1 = microsegundos();
		for(int j = 0; j< k ; j++){
			aleatorio(vector,size); // Se resta el tiempo de ejecución gastado en generar un vector aleatorio k veces
		}	
		t2=microsegundos();
		
		t4 = t2 -t1;
		tiempo = ((t3-t4)/k);
	}	
	return tiempo;
}
			
void tabla1() {
	
	double t;
	
	printf("\n");
	printf("SumaSubMax1\n\n");
	printf("%11s%15s%16s%15s%13s\n","n", "t(n)", "t(n)/n^1.8 ", "t(n)/n^2 ", "t(n)/n^2.2");
	
	for (int i = N ; i <=L ; i = i*2){
		t = tiempo(i, sumaSubMax1);	
		if (t < 500)
			printf("%11d%15.6lf%15.6lf%15.6lf%14.6lf\n",i, t, t/(pow(i,1.8)), t/(pow(i,2)), t/(pow(i,2.2)));
		else
			printf("%12d%15.6lf%15.6lf%15.6lf%14.6lf\n",i, t, t/(pow(i,1.8)), t/(pow(i,2)), t/(pow(i,2.2)));
	}
	// La cota subestimada que nos sugiere el enunciado produce dos mediciones anómalas para n=500 y n=1000
	// Hemos comprobado que cambiando la cota de n^1.8 a n se obtienen las mediciones apropiadas
}
	
void tabla2( int size) {
	
	double t;
	
	printf("\n");
	printf("SumaSubMax2\n\n");
	printf("%11s%16s%16s%14s%16s\n","n", "t(n)", "t(n)/n^0.8 ", "t(n)/n", "t(n)/n*Ln(n) ");
		
	for (int i = N ; i <= L ; i = i*2){;
		t = tiempo(i, sumaSubMax2);
		printf("%11d%15.3lf%15.6lf%15.6lf%15.6lf\n",i,t,t/(pow(i,0.8)),t/i,t/(i*log(i)));
	}
}













