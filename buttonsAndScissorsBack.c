
/*buttonsAndScissorsBack.c*/
#include "buttonsAndScissorsBack.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/******************************************************************************************************************************************************/
//MACROS Y CONSTANTES SIMBOLICAS
/******************************************************************************************************************************************************/
#define ESTA_DENTRO(n, dim) ((n) >= 0 && (n) <= (dim))
#define parESTA_DENTRO(ini, des, dim) ((ini) >= 0 && (des) >= 0 && (ini) < (dim) && (des) < (dim))
#define DIR_POSIBLES 8
#define BLANCO 0
#define FACIL 1
#define DIFICIL 2
#define PVCOM 1
#define COM 1
#define MODOS 2
#define MAX_NOM_ARCHIVOS 255
#define BLOQUE_MOV 5
/******************************************************************************************************************************************************/
//PROTOTIPOS DE FUNCIONES
/******************************************************************************************************************************************************/
int realizarJugada(Tjugada movimiento, Ttablero * tableroActual);
int verificarJugada(Tjugada * movimiento, Ttablero * tableroActual);
int esGanador(Tjugadores * jugadores, Ttablero * tableroActual);
Tjugada generarJugada (Ttablero * tableroActual, int dificultad);
void inizializarJugadores(Tjugadores * jugadores);
void ordenarJugadores (Tjugadores * jugadores);
void elegirDificultad (Tjugadores * jugadores);
char ** crearMatriz (int fil, int col);
void liberarMatriz (char ** matriz, unsigned int dim);
/******************************************************************************************************************************************************/
//PROTOTIPOS DE FUNCIONES AUXILIARES
/******************************************************************************************************************************************************/
static int matrizVacia (Ttablero * tableroActual);
static int buscarGanador (Tjugadores * jugadores);
static int hayMovimientos (Ttablero * tableroActual);
static int encuentraMov (Ttablero* tableroActual, Tjugada * movimiento, int(*criterio)(int,int), TmejorMov * mejoresMov);
static int caminoLibre (Tjugada movimiento, Ttablero * tableroActual );
static int dirPosible (Tjugada * movimiento);
static int existeCamino (Tjugada * movimiento, Ttablero * tableroActual, int(*criterio)(int,int), TmejorMov * mejoresMov);
static void guardarMov (Tjugada * movimiento, TmejorMov * mejoresMov, int corte, int(*criterio)(int,int));
static int menorCortes (int corte1, int corte2);
static int mayorCortes (int corte1, int corte2);
/******************************************************************************************************************************************************/
//FUNCIONES
/******************************************************************************************************************************************************/

//MODULO JUGAR

int
realizarJugada (Tjugada movimiento, Ttablero* tableroActual)
{
	int i, j, cortes = 1;
	for(i = movimiento.iniFil, j = movimiento.iniCol; i != movimiento.desFil || j != movimiento.desCol; i +=movimiento.dirFil, j += movimiento.dirCol)
	{
		if(tableroActual->matriz[i][j] == movimiento.color)
		{
    		tableroActual->matriz[i][j] = BLANCO;
			cortes++;
		}
	}
	tableroActual->matriz[i][j] = BLANCO;
	return cortes;
}

int
verificarJugada (Tjugada * movimiento, Ttablero * tableroActual)
{

// aca verificamos que sean movimientos dentro del tablero
	if (!parESTA_DENTRO(movimiento->iniFil, movimiento->iniCol, tableroActual->dim) )
		return FUERA_DE_RANGO1;

	if (!parESTA_DENTRO(movimiento->desFil, movimiento->desCol, tableroActual->dim))
		return FUERA_DE_RANGO2;

	movimiento->color = tableroActual->matriz[movimiento->iniFil][movimiento->iniCol];

//aca verifico que no sean la misma posicion
	if (movimiento->iniFil == movimiento->desFil && movimiento->iniCol == movimiento->desCol)
		return MISMO_BOTON;

	// veamos que sea un dir posible y asignemosla en caso de ser asi
	if (!dirPosible(movimiento))
		return DIR_IMPOSIBLE;

	//aca verifico que no sean blancos
	if (tableroActual->matriz[movimiento->desFil][movimiento->desCol] == BLANCO || tableroActual->matriz[movimiento->iniFil][movimiento->iniCol] == BLANCO)
		return BOT_BLANCO;

//aca verifico que tengan el mismo color
	if (tableroActual->matriz[movimiento->desFil][movimiento->desCol] != movimiento->color)
		return DIF_COLOR;

	// verificamos que no hay fichas en el medio
	if (!caminoLibre(*movimiento, tableroActual))
		return MULTICOLOR;

	return CORRECTO;                                                             
}//fin verificarJugada

int
esGanador (Tjugadores * jugadores, Ttablero * tableroActual)
{
	if(matrizVacia(tableroActual))							//verifico si el tablero no tiene mas botones
	{
		jugadores->ganador = buscarGanador(jugadores);		//busco los jugadores que tengan el mayor puntaje
		return GANA_PUNT;
	}
	else if (!hayMovimientos(tableroActual))				//verifico si hay movimientos posibles en el tablero
    {
    	jugadores->ganador = (1<<(jugadores->turno % jugadores->cantidad));
		return GANA_NO_MOV;
	}

    return NO_GANADOR;
}

static int
hayMovimientos (Ttablero * tableroActual)
{
	int i, j;
	Tjugada movimiento;
	for (i = 0; i < tableroActual->dim; i++)
	{
		for (j = 0; j < tableroActual->dim; j++)
		{
			if (tableroActual->matriz[i][j] != BLANCO)		//verifico que el boton que voy a revisar no sea un blanco
			{
				movimiento.iniFil = i;						//cargo el punto inicial dentro de la estructura movimiento y luego busco si se puede generar la jugada con otro punto
  				movimiento.iniCol = j;
				movimiento.color = tableroActual->matriz[i][j];
				if (encuentraMov(tableroActual, &movimiento, NULL, NULL))	// criterio = NULL y puntero a la estructura de TmejoresMov = NULL porque reutilizo la funcion
					return CORRECTO;
			}
		}
	}
	return FALSO;
}

static int
matrizVacia (Ttablero * tableroActual)
{
	int i, j;
	for (i = 0; i < tableroActual->dim; i++)
		for (j = 0; j < tableroActual->dim; j++)
			if (tableroActual->matriz[i][j] != BLANCO)		//si encuentro un boton => la matriz no esta vacia
				return FALSO;
	return CORRECTO;
}

static int
buscarGanador (Tjugadores * jugadores)
{
	//esta función deja un numero con 1 en la posición de los ganadores
	int i, ganadores = 0, max = 0;
	for (i = 0; i < jugadores->cantidad; i++)
	{
		if ( (jugadores->jugador[i]).puntaje > max )
		{
			max = (jugadores->jugador[i]).puntaje;
			ganadores = (1<<i);
		}
		else if ( (jugadores->jugador[i]).puntaje == max )
			ganadores = ganadores | (1<<i);
	}
	return ganadores;
}

Tjugada
generarJugada (Ttablero * tableroActual, int dificultad)
{
	int i, j, indice;
	Tjugada movimiento;
	TmejorMov mejoresMov;									//inicializo la estructura que contiene las posibles jugadas
	mejoresMov.cantidad = 0;
	mejoresMov.mejorcorte = 0;
	mejoresMov.movimiento = NULL;
	
	int(*criterio)(int,int);
	if(dificultad == FACIL)									//inicializo la dificultad
		criterio = mayorCortes;
	else
		criterio = menorCortes;		
	
	for (i = 0; i < tableroActual->dim; i++)
	{
		for (j = 0; j < tableroActual->dim; j++)
		{
			if (tableroActual->matriz[i][j] != BLANCO)		//verifico que el boton que voy a revisar no sea un blanco
			{
				movimiento.iniFil = i;						//cargo los valores dentro de la estructura movimiento
  				movimiento.iniCol = j;
				movimiento.color = tableroActual->matriz[i][j];
				encuentraMov(tableroActual, &movimiento, criterio, &mejoresMov);
			}
		}
	}
	indice = rand() % mejoresMov.cantidad;
	movimiento = mejoresMov.movimiento[indice];				//devuelvo uno de los mejores movimientos que halla encontrado
	free(mejoresMov.movimiento);
	return movimiento;
}

static int
mayorCortes (int corte1, int corte2)
{
	return corte1 > corte2;
}

static int
menorCortes (int corte1, int corte2)
{
	if (corte2 == 0)
		return corte1;
	return corte1 < corte2;
}


static int
encuentraMov (Ttablero* tableroActual, Tjugada * movimiento, int(*criterio)(int,int), TmejorMov * mejoresMov)
{
	int k, cortes;
	static int dir[][2] = {{1,0},{0,1},{1,1},{1,-1}};			// matriz de direcciones
	for (k = 0; k < DIR_POSIBLES/2; k++)
	{
		movimiento->dirFil = dir[k][0];					//cargo la dirección en que voy a buscar en el movimiento
        movimiento->dirCol = dir[k][1];
		cortes = existeCamino(movimiento, tableroActual, criterio, mejoresMov);
		if (criterio == NULL && cortes != 0)			//(para hayMovimiento)
			return cortes;								//se logro validar la dirección asignada al movimiento
    }													//sino recorro todo para conseguir todos los movimientos posibles
	return cortes;										
}

static int
existeCamino (Tjugada * movimiento, Ttablero * tableroActual, int(*criterio)(int,int), TmejorMov * mejoresMov)
{
//En esta función buscamos un punto de destino para la jugada, desde el punto inicial, en la dirección cargada anteriormente

	int i, j, cortes = 0;
	for (i = movimiento->iniFil+movimiento->dirFil, j = movimiento->iniCol+movimiento->dirCol; parESTA_DENTRO(i, j, tableroActual->dim); i += movimiento->dirFil, j += movimiento->dirCol)
	{
		if (tableroActual->matriz[i][j] == movimiento->color)
    	{
        	movimiento->desFil = i;						//cargo los posibles puntos de destino en el movimiento
        	movimiento->desCol = j;
        	cortes = caminoLibre(*movimiento, tableroActual);
        	if (criterio == NULL)						//apenas encuentro una direccion invalida la dejo de seguir (es para hayMovimiento)
				return cortes;
			else if (cortes != 0)						//si encuentro un corte, este es un posible movimiento y lo guardardo
				guardarMov(movimiento, mejoresMov, cortes, criterio);
    	}
    	else if (tableroActual->matriz[i][j] != BLANCO)
			return cortes;								//si se bloquea una dirección salgo de esa dirección
	}
	return cortes;
}

static int
caminoLibre (Tjugada movimiento, Ttablero * tableroActual)
{
/*
  En esta funcion vemos que el camino conformado entre ini y des este libre de fichas que sean de otro color
  para eso, vemos que cada posicion del camino sea vacia o del color que se pide
*/
	int i, j, cortes = 1;								// cortes = 1 por el ultimo boton que no cuento
	for (i = movimiento.iniFil, j = movimiento.iniCol; i != movimiento.desFil || j != movimiento.desCol;i += movimiento.dirFil, j += movimiento.dirCol ) //!!faltaban cosas
	{
		if (tableroActual->matriz[i][j] != BLANCO && tableroActual->matriz[i][j] != movimiento.color)
			return 0;
		else if(tableroActual->matriz[i][j] == movimiento.color)
			cortes++;									// devolvemos la cantidad de botones a cortar
	}
	return cortes;										// si llego hasta aca es porque todos eran o blancos o del color
}

static void
guardarMov (Tjugada * movimiento, TmejorMov * mejoresMov, int corte, int(*criterio)(int,int))
{
	if (criterio(corte, mejoresMov->mejorcorte))			//guardo los movimientos que sirven en el vector de jugadas
	{														//según el criterio de dificultad
		mejoresMov->cantidad = 0;
		free(mejoresMov->movimiento);						//libero el vector de movimientos porque encontre uno mejor
		mejoresMov->movimiento = malloc(BLOQUE_MOV * sizeof(Tjugada));
		mejoresMov->mejorcorte = corte;
		mejoresMov->movimiento[mejoresMov->cantidad++] = *movimiento;
	}
	else if (corte == mejoresMov->mejorcorte)
	{
		if ((mejoresMov->cantidad % BLOQUE_MOV) == 0)			//si el movimiento hace la misma cantidad de cortes lo guardo
			mejoresMov->movimiento = realloc(mejoresMov->movimiento, (mejoresMov->cantidad + BLOQUE_MOV) * sizeof(Tjugada));
		mejoresMov->movimiento[mejoresMov->cantidad++] = *movimiento;
	}
}

static int
dirPosible (Tjugada* movimiento)
{
	int encontrada = 0;                                                   
	int dy = movimiento->desFil - movimiento->iniFil;		//para calcular la pendiente m = (Y-Yo)/(X-Xo)
	int dx = movimiento->desCol - movimiento->iniCol;		//solo tengo 8 pendientes validas
	if (dx == 0)
	{
    	encontrada = 1;										
    	movimiento->dirCol = 0;
    	if (dy > 0)
    	    movimiento->dirFil = 1;
    	else                                        
    	    movimiento->dirFil = -1;
  	}
  	else if (dy == 0)	
  	{
  		encontrada = 1;
    	movimiento->dirFil = 0;
    	if (dx > 0)
    	    movimiento->dirCol = 1;
    	else
    	    movimiento->dirCol = -1;
  	}
  	else if (abs(dx)== abs(dy))
  	{
  		encontrada = 1;
    	if (dx > 0)
    	    movimiento->dirCol = 1;
    	else
    	    movimiento->dirCol = -1;
    	if (dy > 0)
    	    movimiento->dirFil = 1;
    	else
    	    movimiento->dirFil = -1;
  	}
  	return encontrada;
}

//FIN MODULO JUGAR

/******************************************************************************************************************************************************/

//MODULO JUGADORES

void
inizializarJugadores(Tjugadores * jugadores)
{
	jugadores->turno = rand() % jugadores->cantidad;
	jugadores->ganador = NO_GANADOR;
	for (int i = 0; i < jugadores->cantidad; i++)
	{
		jugadores->jugador[i].puntaje = 0;
		jugadores->jugador[i].com = 0;
		jugadores->jugador[i].identificador = i+1;
	}
}

void
elegirDificultad (Tjugadores * jugadores)
{
	int resp = rand() % MODOS;
	jugadores->dificultad = resp;
	(jugadores->jugador[PVCOM]).com = COM;
}

/*			
//EN CASO DE MAS JUGADORES SE DEBERIA REORDENAR EL VECTOR DE JUGADORES PARA LOGRAR UNA SECUENCIA ALEATORIA
void
ordenarJugadores (Tjugadores * jugadores)
{
	reordena los jugadores para que no vayan en el orden en que se ingresaron
	int i;
	Tjugador aux;
	for(i = 0; i < jugadores->cantidad - 1; i++)		//el ultimo no lo intercambio con si mismo                  
	{                                                  
		int indice = rand() % (jugadores->cantidad - i) + i;
		aux = jugadores->jugador[i];
		jugadores->jugador[i] = jugadores->jugador[indice];
        jugadores->jugador[indice] = aux;
	}
}
*/

//FIN MODULO JUGADORES

/******************************************************************************************************************************************************/

//MODULO TABLERO

char ** 
crearMatriz (int fil, int col)
{
	//Esta función crea una matriz dinamica 
	char **matriz = NULL;
	int i, j;
	int mem = 1;        // flag para indicar que hay memoria para todas las filas 
	matriz = malloc(fil * sizeof(char *));
 
    // se reserva memoria para la tabla de punteros intermedia 
	if (matriz != NULL)
	{
		for (i = 0; i < fil && mem; i++)
			if ((matriz[i] = malloc(col * sizeof(char)) ) == NULL)
				mem = 0;
		if (!mem) 
		{ 
			for(j = 0; j < i-1; j++)
				free (matriz[j]);		//libera el espacio ya reservado
 
			free (matriz);   
			matriz = NULL;     // si no hay memoria se devuelve NULL 
		}
	}
	return matriz;		//devuelve el puntero a la matriz
}

void
liberarMatriz (char ** matriz, unsigned int dim)
{
	int i;
	for (i = 0; i < dim; i++)
		free (matriz[i]);
	free (matriz);
}

//FIN MODULO TABLERO
