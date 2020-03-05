//Minimizacion
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_COMANDOS 11

double funcion(double x, double y);
struct punto_hypercubo{
	int indice;
	double valor_x;
	double valor_y;
	double aptitud;
};
typedef struct punto_hypercubo Estructura_Punto;

void EVOP(Estructura_Punto * puntos, double rango_minimo, double rango_maximo, int n, double e);
void grafica(Estructura_Punto * puntos, double rango_minimo, double rango_maximo, int n);

Estructura_Punto constructor_estrucutra(int id, double x, double y){
  Estructura_Punto e_p;
  e_p.indice = id;
	e_p.valor_x = x;
	e_p.valor_y = y;
	e_p.aptitud = funcion(x, y);
  return e_p;
}

int main(int argc, char const *argv[]) {

	int n;
	double error = 0.000010;
	srand48(time(NULL));
	double rango_minimo = -10.00;//Es más fácil converger al minimo global en el espacio -4:4
	double rango_maximo = 10.00;//En el espacio -10:10, hay minimos locales
	/*
	while(1){
		printf("Introduzca n -> (2^n)+1: ");
		scanf("%d", &n);
		if( n > 0){
			break;
		}
	}
	*/
	n = 2;//k = 2 (k, Numero de variables)
	int puntos = pow(2, n)+1;
	register int i;
	Estructura_Punto puntos_hypercubo[puntos];
	for( i = 0; i < puntos; i++){
		double x = drand48() * (rango_maximo-(rango_minimo)) + rango_minimo;
		double y = drand48() * (rango_maximo-(rango_minimo)) + rango_minimo;
		puntos_hypercubo[i] = constructor_estrucutra(i, x, y);
	}
	EVOP(puntos_hypercubo, rango_minimo, rango_maximo, puntos, error);
	printf("FIN DE EJECUCION\n");
	return 0;
}

//Problema de Minimizacion
void EVOP(Estructura_Punto * puntos, double rango_minimo, double rango_maximo, int n, double e){

	register int iteracion = 0;
	double bandera_aptitud_1 = 10000.0000;
	double bandera_aptitud_2 = 10000.0000;;
	register int i;
	Estructura_Punto puntos_2[n];
	// Para problemas con espacio de busqueda grandes es bueno tener 1
	// para problemas menores es mejor ir disminuyendo, para el rango -4:4, usar 0.01
	double posicion_cubo = 1.00;//0.01;//Si = 1 poner más iteraciones
	double reduccion = 0;

	while( iteracion < 200 ){
		grafica(puntos, rango_minimo, rango_maximo, n);

		int mejor_aptitud_indice;

		for( i = 0; i < n ; i++){
			if( puntos[i].aptitud <= bandera_aptitud_1){
				mejor_aptitud_indice = puntos[i].indice;
				bandera_aptitud_1 = puntos[i].aptitud;
			}
			printf("( %d <- Indice, Aptitud -> %lf ); ", puntos[i].indice, puntos[i].aptitud);
		}
		printf(" ||  Indice %d  Aptitud %lf Iteracion %d\n",mejor_aptitud_indice, puntos[mejor_aptitud_indice].aptitud, iteracion);
		if( puntos[mejor_aptitud_indice].aptitud <= e ){
			printf("Iteracion %d\n", iteracion);
			break;
		}


		if(puntos[mejor_aptitud_indice].aptitud < bandera_aptitud_2){
			puntos_2[0] = constructor_estrucutra(0, puntos[mejor_aptitud_indice].valor_x+posicion_cubo, puntos[mejor_aptitud_indice].valor_y+posicion_cubo);
			puntos_2[1] = constructor_estrucutra(1, puntos[mejor_aptitud_indice].valor_x+posicion_cubo, puntos[mejor_aptitud_indice].valor_y-posicion_cubo);
			puntos_2[2] = constructor_estrucutra(2, puntos[mejor_aptitud_indice].valor_x-posicion_cubo, puntos[mejor_aptitud_indice].valor_y-posicion_cubo);
			puntos_2[3] = constructor_estrucutra(3, puntos[mejor_aptitud_indice].valor_x-posicion_cubo, puntos[mejor_aptitud_indice].valor_y+posicion_cubo);
			puntos_2[4] = constructor_estrucutra(4, puntos[mejor_aptitud_indice].valor_x, puntos[mejor_aptitud_indice].valor_y);
			bandera_aptitud_2 = puntos[mejor_aptitud_indice].aptitud;
			bandera_aptitud_1 = bandera_aptitud_2;
		}else{
			Estructura_Punto puntos_3[n];
			reduccion = posicion_cubo - 0.01;//0.1; Entre menor sea el valor restado para la reduccion, tardara mas en converger pero sera más preciso
			printf("\nreduccion --------------------------->>>>>>>>>>>>>>>>>>>>> %lf\n", reduccion);//
			puntos_3[0] = constructor_estrucutra(0, puntos_2[4].valor_x+reduccion, puntos_2[4].valor_y+reduccion);
			puntos_3[1] = constructor_estrucutra(1, puntos_2[4].valor_x+reduccion, puntos_2[4].valor_y-reduccion);
			puntos_3[2] = constructor_estrucutra(2, puntos_2[4].valor_x-reduccion, puntos_2[4].valor_y-reduccion);
			puntos_3[3] = constructor_estrucutra(3, puntos_2[4].valor_x-reduccion, puntos_2[4].valor_y+reduccion);
			puntos_3[4] = constructor_estrucutra(4, puntos_2[4].valor_x, puntos_2[4].valor_y);
			//Se considera un optimo aceptable dado que si reducción empieza a hacerse negativo, los puntos de hypercubo incrementan, no se reducen.
			if( reduccion < 0 ){
				break;//Si se elimina esta condicion el programa no convergera.
			}
			for( i = 0; i < n; i++){
				puntos_2[i] = puntos_3[i];
			}
			bandera_aptitud_2 = puntos[mejor_aptitud_indice].aptitud;
			bandera_aptitud_1 = bandera_aptitud_2;
			posicion_cubo = reduccion;
		}
		for( i = 0; i < n; i++){
			puntos[i] = puntos_2[i];
		}

		iteracion++;
	}

}

void grafica(Estructura_Punto * puntos, double rango_minimo, double rango_maximo, int n){
	register int i;
	FILE * archivo_puntos = fopen("puntos_grafica.dat", "w");
	for (i = 0; i < n; i++ ){
		fprintf(archivo_puntos, "%lf %lf %lf\n", puntos[i].valor_x, puntos[i].valor_y, puntos[i].aptitud);
	}


	char * config_gnuplot[] = {"set title \"Gráfica\"",
	"set ylabel \"valores y\"","set xlabel \"valores x\"","set zlabel \"f(x, y)\"",
	"set zeroaxis","set isosamples 80",
	"set pm3d","set palette model CMY rgbformulae 7,5,15",
 	"splot [-10:10] [-10:10] [-1:2] sin(sqrt(x**2+y**2)) / -sqrt(x**2+y**2)+1 with lines title \"sin( sqrt( x^2 + y^2 ) )/- sqrt( x^2 + y^2 )+1 \" lt rgb \"black\"",
	"replot \"puntos_grafica.dat\" title \"Hypercubo\" with points palette pointsize 1 pointtype 7",
	"pause 1"};

	FILE * ventana_gnuplot = popen("gnuplot", "w");//popen("gnuplot -persist", "w");
	for( i = 0; i < NUM_COMANDOS; i++ ){
		fprintf(ventana_gnuplot, "%s \n", config_gnuplot[i]);
	}

	fclose(archivo_puntos);
	fclose(ventana_gnuplot);

}

double funcion(double x, double y){
	return sin( sqrt( pow(x,2)+ pow(y,2) ) ) / -sqrt( pow(x, 2) + pow(y, 2) ) + 1;
}

//El programa converge cuando se alcanza un error minimo aceptable
														//la reduccion es menor a cero, osea ya no es posible reducir el cubo
														//O cuando se alcanzo el maximo de iteraciones


























//https://www.statsmadeeasy.net/2010/03/evolutionary-operation/ EVOP Teoria
//http://gnuplot.sourceforge.net/demo/pm3dcolors.html  Graficar colores GNUplot
//
