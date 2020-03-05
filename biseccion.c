//Metodo numerico Bisección, de aproximación
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define NUM_PUNTOS 333
#define NUM_COMANDOS 11

/*Prototipos*/
double funcion(double x);
void graficar(double numero_x_1, double numero_x_2, double aproximacion);
void metodo_biseccion();

double funcion(double x){
	return 1+x*sin( x + pow(x, 2) );
}
//double derivada
void graficar(double numero_x_1, double numero_x_2, double aproximacion){

	register int a;
	register int i;

	double valoresX[NUM_PUNTOS];
	double rango_minimo = -2.00;
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
	fprintf(archivo_limites, "%lf %lf \n", numero_x_1, 0.0);
	fprintf(archivo_limites, "%lf %lf \n", numero_x_2, 0.0);

	FILE * archivo_punto_en_funcion = fopen("puntos_grafica3.dat", "w");
	fprintf(archivo_punto_en_funcion, "%lf %lf \n", numero_x_1, funcion(numero_x_1));
	fprintf(archivo_punto_en_funcion, "%lf %lf \n", numero_x_2, funcion(numero_x_2));

	FILE * archivo_puntos_aproximacion = fopen("puntos_grafica4.dat", "w");
	fprintf(archivo_puntos_aproximacion, "%lf %lf \n", aproximacion, 0.0);

	//Lista de comandos para ejecutar y configurar la visualización que tendrán
	//los puntos en la gráfica con gnuplot
	char * config_gnuplot[] = {"set title \"Gráfica de la función\"",
														"set ylabel \"Y\"","set xlabel \"X\"",
														"set yrange [-1:3]",
														"set xrange [-3:2]",
														"set zeroaxis",
														"plot \"puntos_grafica.dat\" using 1:2 with lines title \"f(x) = 1+x*sin(x+x^2) \" lt rgb \"blue\"",
														"replot \"puntos_grafica2.dat\" title \"Limites\" pt 5 lt rgb \"gold\" ",
														"replot \"puntos_grafica3.dat\" title \"Punto en la función\" pt 7 lt rgb \"red\" ",
														"replot \"puntos_grafica4.dat\" title \"Aproximación\" pt 9 lt rgb \"blue\" ",
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
	fclose(ventana_gnuplot);

}

void metodo_biseccion(){

	srand48(time(NULL));
	double  numero_x_1 = 0.0;
	double  numero_x_2 = 0.0;
	double rango_minimo = -2.00;
	double rango_maximo = 1.5;
	register int i;
	for( i = 0 ; i<100; i++){
		numero_x_1 = drand48() * (rango_maximo-(rango_minimo)) + rango_minimo;
		numero_x_2 = drand48() * (rango_maximo-(rango_minimo)) + rango_minimo;
		if( funcion(numero_x_1) < 0 && funcion(numero_x_2) > 0 ){
			if( (funcion(numero_x_1) * funcion(numero_x_2)) < 0 ){ //Se verifica que los numeros iniciales tengan signos opuestos
				break;
			}
		}
		//printf("%lf ", numero_x_1);
	}
	//printf("x1=%lf, x2=%lf \n", numero_x_1, numero_x_2);
	//printf("f(%lf), f(%lf) \n", funcion(numero_x_1), funcion(numero_x_2));//Valores electos

	/*METODO*/
	double aproximacion = 0.0;
	register int iteraciones = 0;
	while( iteraciones < 11 ){
		aproximacion = numero_x_1+(numero_x_2-numero_x_1)/2;//Aproximacion_{iteraciones}
		graficar( numero_x_1, numero_x_2, aproximacion);
		if(funcion(aproximacion) * funcion(numero_x_1) > 0){
			printf("Modif X1 %lf --- Aproximacion %lf\n",numero_x_1, aproximacion);
			numero_x_1 = aproximacion;
			printf("Modif X1 %lf --- Aproximacion %lf\n",numero_x_1, aproximacion);
		}else if(funcion(aproximacion) * funcion(numero_x_2) > 0){
			printf("Modif X2 %lf --- Aproximacion %lf\n",numero_x_2, aproximacion);
			numero_x_2 = aproximacion;
			printf("Modif X2 %lf --- Aproximacion %lf\n",numero_x_2, aproximacion);
		}
		printf("Iteracion %d\n", iteraciones);
		iteraciones++;
	}

}

int main(int argc, char const *argv[]) {

	metodo_biseccion();

	return 0;
}

//Metodo de biseccion explicado https://www.youtube.com/watch?v=F9pYhJIrlKw&list=PLZyLAwfuSwFROMLCbu1IfIppztoUkCEqv&index=3&t=0s

//https://www.youtube.com/watch?v=vp83ztQkcr0
//https://www.youtube.com/watch?v=m6uahP62olo
//https://www.geeksforgeeks.org/program-for-bisection-method/
//https://books.google.com.mx/books?id=vQtaR_pV88EC&pg=PA787&lpg=PA787&dq=Book,+Numerical+methods+(Bolzano)&source=bl&ots=0Yjd3Rvd79&sig=ACfU3U1_JhxRVteukMv9dEBHKhuPGx0uWw&hl=es-419&sa=X&ved=2ahUKEwjB2_zU1p_nAhVNaq0KHWntCAkQ6AEwAHoECAoQAQ#v=onepage&q&f=false
	//P 63-64-65
//https://sites.google.com/site/pn20111/home/metodos-cerrados/3-1-metodo-de-biseccion
//https://docs.google.com/viewer?a=v&pid=sites&srcid=ZGVmYXVsdGRvbWFpbnxwbjIwMTExfGd4OjQzNzNmMGQ2YTAyYWMxZDY
