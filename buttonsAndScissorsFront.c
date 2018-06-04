
/*buttonsAndScissorsFront.c*/
#include "buttonsAndScissorsBack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/***********************************************************************************************************/
//MACROS Y CONSTANTES SIMBOLICAS
/***********************************************************************************************************/
#define LIMPIABUFFER while(getchar() != '\n')
#define ES_COLOR(x) ((x)>= 'A' && (x) <= 'E')
#define ES_ESPACIO(x) ((x) == '0' || (x) == ' ')
#define ES_VALIDO_MENU(c)   ((c) == '1' || (c) == '2' || (c) == '3' || (c) == '4')
#define DIM_MIN 5
#define DIM_MAX 30
#define MAX_LONG 260
#define MAX_ENT  10
#define MAX_NOM_ARCHIVOS 255
#define INTENTOS 5
#define SEPARADOR '-'
#define ESPACIO ' '
#define ERROR2 11
#define COORDENADAS 1
#define SALIR 2
#define GUARDAR 3
#define GUARDAR_SALIR 13
#define CONTINUAR 4
/***********************************************************************************************************/
//PROTOTIPOS DE FUNCIONES
/***********************************************************************************************************/
int elegirNivel (void);
int abrirTablero(Ttablero* tableroActual, char* nombre);
int guardarPartida(char* nombredelarchivo, Tjugadores jugadores, Ttablero* tableroActual);
int cargarPartida( Tjugadores* jugadores, Ttablero* tableroActual);
int menu (void);
int leerComando(Tjugada * movimiento, char * nombredelarchivo);
void jugar (Tjugadores * jugadores, Ttablero * tableroActual);
int cargarTablero (Ttablero * tableroActual);
int cargarPartidaBack(char* nombredelarchivo, Tjugadores* jugadores, Ttablero* tableroActual);
/***********************************************************************************************************/
//PROTOTIPOS DE FUNCIONES AUXILIARES
/***********************************************************************************************************/
static void imprimirTablero ( Ttablero * tableroActual );
static void imprimirSeparador ( int dim );
static void imprimirIndices (int dim);
static int salida(char * entrada, char * nombredelarchivo);
static int coordenadas(char * entrada, Tjugada * movimiento);
static int guardar(char * entrada, char * nombredelarchivo);
static int confirmaGuardado (char * nombredelarchivo);
static void imprimirJuego (Tjugadores jugadores, Ttablero * tableroActual);
static void imprimirGanador (Tjugadores jugadores, int forma);
static void imprimirError (Tjugada movimiento, int error);
/***********************************************************************************************************/
//FUNCIONES
/***********************************************************************************************************/

//MODULO ARCHIVOS

int abrirTablero(Ttablero* tableroActual, char* nombre)
{/*esta funcnion se encarga de ir a buscar el archivo guardado a memoria
    salida: 1-> todo OK
            0-> hubo error*/
    int cant;
    int indice;
    int k, aux;
    int verificacion;

    FILE * archivo;
    archivo = fopen(nombre,"r");

    if(archivo ==  NULL)
    {

        printf("\nError: falla en la apertura del archivo.\n");
        return ERROR;// no se pudo leer
    }

    /*si paso este punto se establecio correctamente un vinculo entre el archivo
    y el programa.*/
    cant = fgetc(archivo) - '0';//obtenemos la catnidad de matrices que contiene el archivo

    indice = rand() % cant;

    //luego la rellenamos con los valores del archivo

    // posicionamos el lector en la matriz deseada

    if(indice != 0)
    {
        //posicionamos el lector en la matriz deseada
        int dim = tableroActual ->dim;
        int offset = (indice * dim*dim ) + (dim*indice )+ (indice * 2) + 1;
        fseek(archivo,  offset , SEEK_SET);

    }

    verificacion = fgetc(archivo);// quita el ultimo espacio antes de comenzar la martiz
    if(verificacion != '\n')
    {
        fclose(archivo);
        printf("\nError: fallo la carga del tablero.\n");
        return ERROR;
    }

    // ahora cargamos la matriz
    for(k = 0; k < tableroActual->dim; k++)
    {
        //resp = fread(tableroActual->matriz[k], sizeof(char), tableroActual->dim, archivo);
		for(int j = 0; j<tableroActual->dim; j++)
		{
			aux = fgetc(archivo);
			if(ES_COLOR(aux))
				tableroActual->matriz[k][j] = aux;//toupper(aux);
			else if(aux == ESPACIO)
				tableroActual->matriz[k][j] = 0;//cargo los espacios como ceros
			else
			{
                fclose(archivo);
	            printf("\nError: fallo la carga del tablero.\n");
	            return ERROR;
	        }

		}//fin del for interno

		fgetc(archivo);// cambio de linea en el archivo
    }


    fclose(archivo);

    return CORRECTO;

}


int guardarPartida(char* nombredelarchivo, Tjugadores jugadores, Ttablero* tableroActual)
{	/*esta funcion se encarga de generar un archivo nuevo con el nombre que ingreso el usuario
	esta d¡tendra el estado del juego en el momento en que se guardo*/
    FILE* archivo;
    int com;
    int turno;
    int dim;
    archivo = fopen(nombredelarchivo, "wtb");//abrimos en modo binario para lectura y escritura
    if(archivo ==  NULL)
    {   printf("Error: archivo invalido.\n");
        return ERROR;// no se pudo crear;
    }

    com = (jugadores.jugador[0].com || jugadores.jugador[1].com);
    turno = (jugadores.turno % jugadores.cantidad);
    turno += 1; // pues tiene que ser enter 1 y 2 no 0 y 1
    dim = tableroActual->dim;

    fwrite(&com,sizeof(int), 1, archivo);
    fwrite(&turno, sizeof(int), 1, archivo);
    fwrite(&dim, sizeof(int), 1, archivo);
    // ahora guardemos la matriz
    for(int i = 0; i < tableroActual->dim; i++)
        for(int j = 0; j < tableroActual->dim; j++)
        {
            if(ES_COLOR(tableroActual->matriz[i][j]) )
                fputc(tableroActual->matriz[i][j], archivo);
            else
                fputc('0', archivo);

        }

    fclose(archivo);
    printf("\nPartida guardada exitosamente.\n");
    return CORRECTO;

}


int cargarPartidaBack(char* nombredelarchivo, Tjugadores* jugadores, Ttablero* tableroActual)
{/*esta funcino se encarga del cargado de la partida anterior.
    el orden de lectura es:
    - int tipo de jugadores, 0 es humano 1 es comp
    - prox jugador ( si es cmp 2 es la comp)
    - int dim del tableroActual
    - secuencia de char con la matriz
            0 posicion vacia
            A-E es los valores de btns posibles*/

    FILE* archivo;
    // inicializamos las variables de jugadores
    int proxTurno = 0;
    int dim = 0;
    int tipoJug = 0;
    int i , j, aux;
    archivo = fopen(nombredelarchivo, "rb");
    if(archivo == NULL)
    {
        printf("Error: el archivo esta corrupto o no existe.\n" );
        return ERROR;
    }


    fread(&tipoJug,sizeof(int), 1, archivo);
    if(tipoJug == 0 || tipoJug == 1)
    {
        fread(&proxTurno,sizeof(int), 1, archivo );
        proxTurno -= 1;// esto es porque trabajamos con el resto del numero

        if(proxTurno   <= jugadores->cantidad )
        {
            fread(&dim,sizeof(int), 1, archivo );
        }else
		{
			printf("\nError: no se pudo hacer la carga de la partida.\n" );
			return ERROR;
		}
    }else
	{
		printf("\nError: no se pudo hacer la carga de la partida.\n" );
		return ERROR;
	}


    inizializarJugadores(jugadores);

    if (tipoJug == 1)
 	{
        (jugadores->jugador[1]).com = 1;
        elegirDificultad(jugadores);
    }
    jugadores->turno = proxTurno;
    jugadores->ganador = 0;
    tableroActual->dim = dim;
    //leemos el tableroActual
    tableroActual->matriz = crearMatriz(dim,dim);
    if(tableroActual->matriz == NULL)
        return ERROR;
    for(i = 0; i < dim; i++)
    {
        for(j = 0; j < dim; j++)
        {
            aux = fgetc(archivo);
            if(ES_COLOR(aux))
                tableroActual->matriz[i][j] = aux;
            else if(ES_ESPACIO(aux))
            {
                tableroActual->matriz[i][j] = 0;
            }else
                return ERROR;
        }
    }
    fclose(archivo);

    return CORRECTO;
}

//FIN MODULO ARCHIVOS

/***********************************************************************************************************/

//MODULO INTERFAZ

int
menu (void)
{
    int c = 0;
    printf("  ____            _                                              _____   _     _                             \n");
    printf(" | __ )    ___   | |_    ___    _ __     ___   ___     _   _    |_   _| (_)   (_)   ___   _ __    __ _   ___ \n");
    printf(" |  _ \\   / _ \\  | __|  / _ \\  | '_ \\   / _ \\ / __|   | | | |     | |   | |   | |  / _ \\ | '__|  / _` | / __|\n");
    printf(" | |_) | | (_) | | |_  | (_) | | | | | |  __/ \\__ \\   | |_| |     | |   | |   | | |  __/ | |    | (_| | \\__ \n");
    printf(" |____/   \\___/   \\__|  \\___/  |_| |_|  \\___| |___/    \\__, |     |_|   |_|  _/ |  \\___| |_|     \\__,_| |___/\n");
    printf("                                                       |___/                |__/                             \n");
    printf("\nMenu del Juego\n");
    do
    {
    	if (c != 0)
			printf("\n Opcion invalida\n");
        if (c != 0 && c != '\n')
            LIMPIABUFFER;
        printf("\n1)  Juego de dos jugadores\n");
        printf("2)  Juego contra computadora\n");
        printf("3)  Recuperar un juego grabado\n");
        printf("4)  Terminar\n");
        printf("\nIngrese la opcion: ");
    }
    while (! ((c = getchar()) && ES_VALIDO_MENU(c) && getchar() == '\n'));
    return c - '0';
}

int
elegirNivel (void)
{
	int resp = 0, leido = 0;
	char enter = 0, c = 0;
	do
	{
		if (c != 0 && (enter != '\n' || resp < DIM_MIN || resp > DIM_MAX))
			printf("\nError: Opcion invalida.\n");

		printf("Elije un nivel de tablero entre %d y %d: ", DIM_MIN, DIM_MAX);
		if ((c = getchar()) != '\n')
		{
			ungetc(c, stdin);
			leido = scanf("%3d%c", &resp, &enter);
			if (resp && enter != '\n')
				LIMPIABUFFER;
		}
		if (c != '\n' && leido != 2)
			LIMPIABUFFER;
	}
	while ((leido != 2 && enter != '\n') || resp < DIM_MIN || resp > DIM_MAX);
	return resp;
}

int cargarPartida(Tjugadores* jugadores, Ttablero* tableroActual)
{
	char nombre[MAX_NOM_ARCHIVOS];
	char fmt [MAX_ENT+8];
	int leido = 0, contador = INTENTOS;
	sprintf(fmt, "%%%d[^\n]", MAX_NOM_ARCHIVOS);

	while(contador-- && !leido )
	{
		printf("\nIngrese el nombre del archivo:");
		leido = scanf(fmt, nombre);
		LIMPIABUFFER;
		if(leido == 1)
		{
			leido = cargarPartidaBack(nombre, jugadores, tableroActual);
		}
		else
			printf("\n Error: no se pudo cargar el nombre del archivo.\n");
	}// fin while

	return leido;

}
//FIN MODULO INTERFAZ

/***********************************************************************************************************/

//MODULO LEERCOMANDO

int
leerComando(Tjugada * movimiento, char * nombredelarchivo)
/*Lee todos los posibles comandos  quit save y cordenadas de una matriz.*/
{
	char fmt [MAX_ENT+8];
	sprintf(fmt, "%%%d[^\n]", MAX_LONG);
	int flag = ERROR;
	do
	{
		char entrada[MAX_LONG+1]={0};
		printf("\nIngrese un Comando:");
		scanf(fmt, entrada);
		if(getchar()=='\n')
		{
			switch(*entrada)
			{
				case '[':
					flag = coordenadas(entrada, movimiento);
					break;
				case 'q':
					flag = salida(entrada, nombredelarchivo);
					break;
				case 's':
					flag = guardar(entrada, nombredelarchivo);
					break;
				default :
					flag = ERROR;
					break;
			}

		}
		else
			LIMPIABUFFER;
		if (flag == ERROR)
			printf("\nError: Comando invalido\n");
	}
	while (flag == ERROR || flag == ERROR2 || flag == CONTINUAR);
	return flag;
}


static int guardar(char * entrada, char * nombredelarchivo)
{
	/* Esta funcion se fija si el comando ingresado es save y guarda en nombre del archivo.*/
	if ( strncmp(entrada, "save ", 5) == 0)
		{
			if(entrada[5] )
			{
				strcpy(nombredelarchivo, entrada+5);
				return GUARDAR;
            }
			printf("\nError: Nombre invalido.");
            return ERROR2;
        }

	return ERROR;
}

static int
coordenadas(char * entrada, Tjugada * movimiento)
{
	/*Esta funcion lee 4 coordenadas que representan  4 numeros que pueden ser 2 elementos de una matriz*/
	int lecturas;
	char corch, vacio;
	int f1, c1, f2, c2;
	lecturas = sscanf(entrada,"[%d,%d][%d,%d%c%c", &f1, &c1, &f2, &c2, &corch, &vacio);
	if(lecturas == 5 && corch == ']')
	{
		movimiento->iniFil = f1;
		movimiento->iniCol = c1;
		movimiento->desFil = f2;
		movimiento->desCol = c2;
		return COORDENADAS;
	}
	return ERROR;
}

static int
salida(char * entrada, char * nombredelarchivo)
{
    /*Verifica si el comando es quit pide confirmacion y pregunta si se desea guardar.*/
    int contador1 = INTENTOS, contador2 = INTENTOS, estado;
    char  letra, enter;
    if(strcmp(entrada, "quit") == 0)
    {
        while (contador1--)
        {
            printf("\n¿Esta seguro que quiere salir (Y/N)?");
            letra=getchar();
            if(letra != '\n')
                {
                	enter=getchar();
                	if(enter == '\n')
                	{
	                	if(toupper(letra) == 'Y')
	                	{
	                		while (contador2--)
	                   		{
	                   			estado = confirmaGuardado (nombredelarchivo);
	                   			if (estado != CONTINUAR)
	                   				return  estado;
	                   		}
	                	}
	                   	if (toupper(letra) == 'N')
	                   		return CONTINUAR;
	                }
	                if (contador2 != -1 && enter != '\n')
	                {
	                	LIMPIABUFFER;
	                }
	                else if (contador2 == -1)
	                	return ERROR2;
                }

        }
        return ERROR2;
    }
 	return ERROR;
}

static int
confirmaGuardado (char * nombredelarchivo)
{
	/*Confirma el guardado y lee el nombre del archivo.*/
    char  leido, letra, enter;
    char nombreAuxiliar[MAX_NOM_ARCHIVOS];
    char fmt [MAX_ENT+8];

    sprintf(fmt, "%%%d[^\n]", MAX_NOM_ARCHIVOS);

    printf("\n¿Desea guardar la partida antes de salir (Y/N)?");
    letra = getchar();
    if(letra != '\n')
        {
        	enter=getchar();
            if(enter == '\n')
            {
	        	if(toupper(letra) == 'N')
	        		return SALIR;
	        	if(toupper(letra) == 'Y')
	        	{
	        		printf("\nIngrese el nombre del archivo:");
	        		leido  = scanf(fmt, nombreAuxiliar);
	        		LIMPIABUFFER;
	        		if(leido == 1)
	        		{
	        			strcpy(nombredelarchivo, nombreAuxiliar);
						return GUARDAR_SALIR;
	        		}
	        		printf("\nError: Nombre invalido.");
	            	return ERROR2;
	        	}
	        	return CONTINUAR;
	        }
	        else
	        	LIMPIABUFFER;
        }
     	return CONTINUAR;
}


//FIN MODULO LEERCOMANDO


/***********************************************************************************************************/

//MODULO JUGAR

void
jugar (Tjugadores * jugadores, Ttablero * tableroActual)
{
	Tjugada movimiento;
	char nombredelarchivo[MAX_NOM_ARCHIVOS];
	int resp, formaganar, turnoActual, error;
	turnoActual = jugadores->turno;

	imprimirJuego(*jugadores, tableroActual);
	do
	{
		if ((jugadores->jugador[turnoActual]).com == 0)			//si el jugador no es un computadora entonces leo el comando
			resp = leerComando(&movimiento, nombredelarchivo);
		else
		{
			resp = COORDENADAS;									//la computadora genera coordenadas
			movimiento = generarJugada(tableroActual, jugadores->dificultad);
		}
		switch(resp)
		{
			case COORDENADAS:
				if ( (error = verificarJugada(&movimiento, tableroActual)) == CORRECTO )	//valido la jugada ingresada
				{
					(jugadores->jugador[turnoActual]).puntaje += realizarJugada(movimiento, tableroActual);
					if ( (formaganar = esGanador(jugadores, tableroActual)) )				//si hay un ganador imprimo y vuelvo al menu
					{
						imprimirJuego(*jugadores, tableroActual);
						imprimirGanador(*jugadores, formaganar);
						resp = SALIR;
					}
					else																	//sino sigue el juego
					{
						jugadores->turno++;
						imprimirJuego(*jugadores, tableroActual);
						turnoActual = jugadores->turno % jugadores->cantidad;
					}
				}
				else
					imprimirError(movimiento, error);
				break;

			case GUARDAR:
			case GUARDAR_SALIR:
				guardarPartida(nombredelarchivo, *jugadores, tableroActual);
				break;
		}
	}
	while (resp == COORDENADAS || resp == GUARDAR);
	liberarMatriz (tableroActual->matriz, tableroActual->dim);
}

static void
imprimirJuego (Tjugadores jugadores, Ttablero * tableroActual)
{
	int indice = jugadores.turno % jugadores.cantidad;
	printf("\nBotones ");
	for (int i = 0; i < jugadores.cantidad; i++)						//imprime los jugadores y sus puntajes
		printf("J%d: %d%c\t", jugadores.jugador[i].identificador, jugadores.jugador[i].puntaje, (i == jugadores.cantidad-1) ? '\n' : ',');
	imprimirTablero(tableroActual);
	putchar('\n');
	if (jugadores.ganador == NO_GANADOR)
		printf("Turno jugador %d\n", jugadores.jugador[indice].identificador);
}

static void
imprimirGanador (Tjugadores jugadores, int forma)
{
	int i;
	if (forma == GANA_NO_MOV)
		printf("No hay mas movimientos posibles en el tablero.\n");
	for (i = 0; i < jugadores.cantidad ; i++)
		if (jugadores.ganador & (1<<i))
			printf("El Jugador %d, ha ganado.\n", jugadores.jugador[i].identificador);
	printf("GAME OVER\n");
}

static void
imprimirError (Tjugada movimiento, int error)
{
	switch (error)
	{
		case FUERA_DE_RANGO1:
			printf("Error: No existe la posición [%d,%d]\n", movimiento.iniFil, movimiento.iniCol);
			break;
		case FUERA_DE_RANGO2:
			printf("Error: No existe la posición [%d,%d]\n", movimiento.desFil, movimiento.desCol);
			break;
		case MISMO_BOTON:
			printf("Error: Debe ingresar dos posiciones distintas\n");
			break;
		case DIR_IMPOSIBLE:
			printf("Error: La dirección del corte es invalida\n");
			break;
		case BOT_BLANCO:
			printf("Error: Se debe ingresar dos botones\n");
			break;
		case DIF_COLOR:
			printf("Error: Los botones deben contener el mismo color\n");
			break;
		case MULTICOLOR:
			printf("Error: El corte no tiene una única variedad de botones\n");
			break;
	}
}

//FIN MODULO JUGAR


/***********************************************************************************************************/

//MODULO TABLERO

int
cargarTablero (Ttablero * tableroActual)
{
	char nombre [MAX_NOM_ARCHIVOS];
	int cargar = ERROR, contador = INTENTOS;
	while (cargar == ERROR && contador--)
	{
		tableroActual->dim = elegirNivel();
		sprintf(nombre, "%dx%d", tableroActual->dim, tableroActual->dim);
		tableroActual->matriz = crearMatriz(tableroActual->dim, tableroActual->dim);
		cargar = abrirTablero(tableroActual, nombre);
	}
	return cargar;
}

static void
imprimirTablero (Ttablero * tableroActual)
{
	//Esta función imprime el tablero de juego con el puntero al tablero
	int i, j;
	putchar('\n');
	imprimirIndices(tableroActual->dim);
	imprimirSeparador (tableroActual->dim);

	for (i = 0; i < tableroActual->dim; i++)
	{
		printf("%2d|", i);							//imprimo el indice de la fila
		for (j = 0; j < tableroActual->dim; j++)	//imprimo la fila
			printf(" %c |", (ES_COLOR(tableroActual->matriz[i][j])) ? tableroActual->matriz[i][j] : ESPACIO);
		putchar('\n');
		imprimirSeparador (tableroActual->dim);
	}
}

static void
imprimirSeparador (int dim)
{
	//Esta función es utilizada por imprimir tablero para crear un separador entre las filas
	printf("  +");
	for (int i = 0; i < dim; i++)
		printf("---+");
	putchar('\n');
}

static void
imprimirIndices (int dim)
{
	//Esta función es utilizada por imprimir tablero para imprimir los indices de las columnas del tablero
	int i;
	printf(" ");
	for (i = 0; i < dim; i++)
		printf("%4d", i);
	putchar('\n');
}

//FIN MODULO TABLERO
