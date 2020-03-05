//Metodo numerico Bisección, de aproximación
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define NUM_PUNTOS 2000
#define NUM_COMANDOS 11 //+1 Para graficar la recta tangente de la funcion

/*Prototipos*/
double funcion(double x);
void graficar(double numero_x, double aproximacion);
void metodo_newton_raphson();

double funcion(double x){
	return 1+x*sin( x + pow(x, 2) );
}
double derivada_de_funcion(double x){
	return sin( pow(x, 2 )+x) + x*((2*x)+1) * cos( pow(x,2) + x );
}
void graficar(double numero_x, double aproximacion){

	register int a;
	register int i;

	double valoresX[NUM_PUNTOS];
	double rango_minimo = -10.00;
	for( a = 0; a <= NUM_PUNTOS-1; a++){
		valoresX[a] = rango_minimo;
		rango_minimo += 0.01;
	}
	double valoresY[NUM_PUNTOS];

	for( a = 0; a <= NUM_PUNTOS-1; a++ ){
		valoresY[a] = funcion(valoresX[a]);
		//printf("value of f(x): x=%lf\n", funcion(valoresX[a]));
	}
	//Se crea y se abre el archivo puntos_grafica.dat en modo escritura
	//para almacenar los valores de x y y que están declarados en los arreglos
	//valoresX y valoresY
	FILE * archivo_puntos = fopen("puntos_grafica.dat", "w");
	//Guarda los puntos x, y en el archivo .dat creado y abierto previamente
	for (i = 0; i < NUM_PUNTOS; i++ ){
		fprintf(archivo_puntos, "%lf %lf \n", valoresX[i], valoresY[i]);
	}

	FILE * archivo_limites = fopen("puntos_grafica2.dat", "w");
	fprintf(archivo_limites, "%lf %lf \n", numero_x, 0.0);

	FILE * archivo_punto_en_funcion = fopen("puntos_grafica3.dat", "w");
	fprintf(archivo_punto_en_funcion, "%lf %lf \n", numero_x, funcion(numero_x));

	FILE * archivo_puntos_aproximacion = fopen("puntos_grafica4.dat", "w");
	fprintf(archivo_puntos_aproximacion, "%lf %lf \n", aproximacion, 0.0);

	/*
	FILE * recta = fopen("recta.dat", "w");
	fprintf(recta, "%lf %lf \n", numero_x, funcion(numero_x));
	fprintf(recta, "%lf %lf \n", aproximacion, 0.0);
	*/

	//Lista de comandos para ejecutar y configurar la visualización que tendrán
	//los puntos en la gráfica con gnuplot
	char * config_gnuplot[] = {"set title \"Gráfica de la función\"",
														"set ylabel \"Y\"","set xlabel \"X\"",
														"set yrange [-10:10]",
														"set xrange [-10:10]",
														"set zeroaxis",
														"plot \"puntos_grafica.dat\" using 1:2 with lines title \"f(x) = 1+x*sin(x+x^2) \" lt rgb \"blue\"",
														"replot \"puntos_grafica2.dat\" title \"Valor de f(x), en y=0\" pt 5 lt rgb \"gold\" ",
														"replot \"puntos_grafica3.dat\" title \"Punto en la función\" pt 7 lt rgb \"red\" ",
														"replot \"puntos_grafica4.dat\" title \"Aproximación\" pt 9 lt rgb \"blue\" ",
														//"replot \"recta.dat\" using 1:2 with lines title \"Recta tangente a la grafica\" lt rgb \"red\" ",
														"pause 1"};
	/*Se crea una archivo de tipo popen, es una tebería IPC que se usa, para
	* ejecutar gnuplot y enviarle el archivo a graficar*/
	FILE * ventana_gnuplot = popen("gnuplot", "w");//popen("gnuplot -persist", "w");
	//Ejecuta los comandos de GNUplot uno por uno
	for( i = 0; i < NUM_COMANDOS; i++ ){
		fprintf(ventana_gnuplot, "%s \n", config_gnuplot[i]);
	}

	fclose(archivo_puntos);
	fclose(archivo_limites);
	fclose(archivo_punto_en_funcion);
	fclose(archivo_puntos_aproximacion);
	//fclose(recta);
	fclose(ventana_gnuplot);

}

void metodo_newton_raphson(){

	srand48(time(NULL));
	double  numero_x = 0.0;
	double rango_minimo = -5.00;
	double rango_maximo = 5.00;
	register int i;
	for( i = 0 ; i<100; i++){
		numero_x = drand48() * (rango_maximo-(rango_minimo)) + rango_minimo;
		if( funcion(numero_x) > 0 ){
			break;
		}
	}

	/*METODO*/
	double aproximacion = 0.0;
	register int iteraciones = 0;
	while( iteraciones < 11 ){
		if( derivada_de_funcion(numero_x) != 0){
			aproximacion = numero_x - (funcion(numero_x)/derivada_de_funcion(numero_x));
		}else{
			break;
		}
		graficar(numero_x, aproximacion);
		if(funcion(aproximacion) != derivada_de_funcion(numero_x)){
			printf("Modif X1 %lf --- Aproximacion %lf\n",numero_x, aproximacion);
			numero_x = aproximacion;
			printf("Modif X1 %lf --- Aproximacion %lf\n",numero_x, aproximacion);
		}
		printf("Iteracion %d\n", iteraciones);
		iteraciones++;
	}

}

int main(int argc, char const *argv[]) {

	metodo_newton_raphson();

	return 0;
}



//https://www.youtube.com/watch?v=tX9ecFstUUk&list=PLZyLAwfuSwFROMLCbu1IfIppztoUkCEqv&index=1













//
