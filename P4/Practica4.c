/*Julian Barcia Facal
  Tome Maseda Dorado
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>


#define K 10000			    // iteraciones del algoritmo en caso de no llegar al umbral de confianza
#define TAM_MAX 1000		// maximo valor de n
#define N 25 				// minimo valor de n
			

typedef int ** matriz;

double microsegundos();
void inicializar_semilla();
matriz crearMatriz(int n);
void listar_matriz(matriz m,int tam);
void dijkstra(matriz grafo, matriz distancias, int tam);
matriz crearMatriz(int n);
void iniMatriz(matriz m, int n);
void liberarMatriz(matriz m, int n);
void figura2test(void);
void matriz1 (matriz m);
void matriz2 (matriz m);
void figura3test(void);
double tiempo(int size);
void testcomplejidad();


int main()
{
	inicializar_semilla();
	figura2test();
	figura3test();
	testcomplejidad();
	testcomplejidad();
	testcomplejidad();
	
	
}
void figura2test(void){
	
	matriz distancias,grafo;;
	
	grafo =crearMatriz(5);
	matriz1(grafo);
	printf("\n FIGURA 2 \n");
	printf("********************************************************** \n");
	printf("Matriz adyacente\n");
	listar_matriz(grafo,5);
	distancias = crearMatriz(5);
	dijkstra(grafo,distancias,5);
	printf("\n");
	printf("Distancias\n");
	listar_matriz(distancias,5);
	liberarMatriz(distancias,5);
	liberarMatriz(grafo,5);
	
	
	
	
}

void figura3test(void){
	
	matriz distancias,grafo;;
	
	grafo =crearMatriz(4);
	matriz2(grafo);
	printf("\n FIGURA 3 \n");
	printf("********************************************************** \n");
	printf("Matriz adyacente\n");
	listar_matriz(grafo,4);
	distancias = crearMatriz(4);
	dijkstra(grafo,distancias,4);
	printf("\n");
	printf("Distancias\n");
	listar_matriz(distancias,4);
	liberarMatriz(distancias,4);
	liberarMatriz(grafo,4);


}


void matriz1 (matriz m){
	
	m[0][0]=0;
	m[0][1]=1;
	m[0][2]=8;
	m[0][3]=4;
	m[0][4]=7;
	m[1][0]=1;
	m[1][1]=0;
	m[1][2]=2;
	m[1][3]=6;
	m[1][4]=5;
	m[2][0]=8;
	m[2][1]=2;
	m[2][2]=0;
	m[2][3]=9;
	m[2][4]=5;
	m[3][0]=4;
	m[3][1]=6;
	m[3][2]=9;
	m[3][3]=0;
	m[3][4]=3;
	m[4][0]=7;
	m[4][1]=5;
	m[4][2]=5;
	m[4][3]=3;
	m[4][4]=0;

	
}

void matriz2 (matriz m){
	
	m[0][0]=0;
	m[0][1]=1;
	m[0][2]=4;
	m[0][3]=7;
	m[1][0]=1;
	m[1][1]=0;
	m[1][2]=2;
	m[1][3]=8;
	m[2][0]=4;
	m[2][1]=2;
	m[2][2]=0;
	m[2][3]=3;
	m[3][0]=7;
	m[3][1]=8;
	m[3][2]=3;
	m[3][3]=0;
	
}

void dijkstra(matriz grafo, matriz distancias, int tam) {
	int m, i, w, n,v,min,j;
	int *noVisitados = malloc(tam*sizeof(int));
	for (m=0; m<tam; m++) {
		for (i=0; i<tam; i++) {
			noVisitados[i] = 1;
			distancias[m][i] = grafo[m][i];
		} 
		j=0;
		noVisitados[m] = 0;
		while (j < tam-2) {
			v =0;
			min = TAM_MAX;
			for (n=0; n<tam; n++){
				if (noVisitados[n] == 1){
					if (distancias[m][n] < min){
						min = distancias[m][n];
						v = n;
					}	
				}
			}
			noVisitados[v]=0;
			for (w=0; w<tam ; w++){
				if (noVisitados[w] ==1){
					if (distancias [m][w] > (distancias [m][v]+ grafo [v][w]))
						distancias [m][w] = distancias [m][v] + grafo[v][w];
				}
			}
			j++;
		}
	}
	free(noVisitados);

}
	
matriz crearMatriz(int n) {
	int i;
	matriz aux;
	if ((aux = malloc(n*sizeof(int *))) == NULL)
		return NULL;
	for (i=0; i<n; i++)
		if ((aux[i] = malloc(n*sizeof(int))) == NULL)
			return NULL;
	return aux;
}

/* Inicializacion pseudoaleatoria [1..TAM_MAX] de un grafo completono dirigido con n nodos, representado por su matriz de adayencia  */
void iniMatriz(matriz m, int n) {
	int i, j;
	for (i=0; i<n; i++)
		for (j=i+1; j<n; j++)
			m[i][j] = rand() % 10 + 1;
	for (i=0; i<n; i++)
		for (j=0; j<=i; j++)
			if (i==j)
				m[i][j] = 0;
			else 
				m[i][j] = m[j][i];
			
}

void liberarMatriz(matriz m, int n) {
	int i;
	for (i=0; i<n; i++)
		free(m[i]);
	free(m);
}

void listar_matriz(matriz m,int tam) {
	int i,j;
	for(i=0;i<tam;i++){
		for(j=0;j<tam;j++){
		printf("%3d ", m[i][j]);
		}
		putchar('\n');
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

void testcomplejidad() {
	double t;
	int i;
	
	printf("\n");
	printf("Comprobacion empirica de la complejidad del algoritmo de calculo de distancias minimas \n");
	printf("\n %11s%16s%16s%14s%16s%14s\n","n", "t(n)", "t(n)/n^2.5 ", "t(n)/n^3", "t(n)/n^2.9" , "t(n)/n^3.5 ");

	for (i = N; i <=TAM_MAX; i = i*2){
		t = tiempo(i);		
		printf("%11d%16.6lf%15.6lf%15.6lf%14.6lf%14.6lf\n",i, t, t/(pow(i,2.5)), t/(pow(i,3)), t/(pow(i,2.9)) ,t/(pow(i,3.5)));
	}
}

double tiempo(int size) {
	
	double ta,tb,tiempo,t3,t4;
	int j;
	matriz m,d;
	d = crearMatriz(size);
	m = crearMatriz(size);
	iniMatriz(m,size);
	ta=microsegundos();
	dijkstra(m,d,size);
	tb=microsegundos();
	tiempo = tb-ta;
	
	if (tiempo < 500 ){ // umbral de confianza
		printf("*");
		ta = microsegundos();
		for(j = 0; j < K; j++){
			iniMatriz(m,size);
			dijkstra(m, d, size);
		}
		tb = microsegundos();
		t3 = tb-ta;
		ta = microsegundos();
		for(j = 0; j< K ; j++){
			iniMatriz(m,size); // se resta el tiempo de ejecución gastado en generar un vector aleatorio K veces
		}
		tb=microsegundos();
		t4 = tb -ta;
		tiempo = (t3-t4)/K;
	} else printf("%1s", "");
	
	liberarMatriz(m, size);
	liberarMatriz(d, size);
	
	return tiempo;
}
	





