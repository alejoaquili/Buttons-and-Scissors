#ifndef _buttonsAndScissorsBack_h
#define _buttonsAndScissorsBack_h

/*buttonsAndScissorsBack.h*/
/******************************************************************************************************************************************************/
//CONSTANTES SIMBOLICAS
/******************************************************************************************************************************************************/
#define FUERA_DE_RANGO1 2
#define FUERA_DE_RANGO2 3
#define MISMO_BOTON 4
#define DIR_IMPOSIBLE 5
#define BOT_BLANCO 6
#define DIF_COLOR 7
#define MULTICOLOR 8
#define GANA_NO_MOV 1
#define GANA_PUNT 2
#define NO_GANADOR 0
#define ERROR 0
#define CORRECTO 1
#define FALSO 0
/******************************************************************************************************************************************************/
//DECLARACIONES DE NUEVOS TIPOS Y  ESTRUCTURAS
/******************************************************************************************************************************************************/

typedef struct
{
  //inicio del movimiento
	int iniFil;
	int iniCol;
  //fin del camino a cubrir
	int desFil;
	int desCol;
  //incrementos en col y fil
	int dirCol;
	int dirFil;
  // color con el que se esta trabajando
	char color;
}Tjugada;

typedef struct
{
	Tjugada * movimiento;
	int cantidad;
	int mejorcorte;
}TmejorMov;

typedef struct
{
	unsigned int puntaje;
	int com;
	int identificador;
}Tjugador;

typedef struct
{
	Tjugador * jugador;
	unsigned int turno;
	unsigned int cantidad;
	unsigned int ganador;
	unsigned int dificultad;
}Tjugadores;

typedef struct
{
	char * * matriz;
	unsigned int dim;
	unsigned int colores;
}Ttablero;

/******************************************************************************************************************************************************/
//PROTOTIPOS
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

#endif

