#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buttonsAndScissorsBack.h"
#include "buttonsAndScissorsFront.h"
#include "CuTest.h"
#define CANT_CORTES 2

void movimientosValidosTest(CuTest * cuTest);
void movimientosInvalidosTest(CuTest * cuTest);
void cortesTest(CuTest * cuTest);
void ganadorTest(CuTest * cuTest);
static Ttablero* crearTablero(void);

CuSuite *
backSuite(void)
{
	CuSuite * cuSuite = CuSuiteNew();
	SUITE_ADD_TEST(cuSuite, movimientosValidosTest);
    SUITE_ADD_TEST(cuSuite, movimientosInvalidosTest);
	SUITE_ADD_TEST(cuSuite, ganadorTest);
    SUITE_ADD_TEST(cuSuite, cortesTest);
	return cuSuite;
}

void
ganadorTest(CuTest * cuTest)
{
	Tjugadores* jugadores = malloc(sizeof(Tjugadores));
	jugadores->jugador = malloc(2*sizeof(Tjugador));
	jugadores->jugador[0].puntaje = 0;
	jugadores->jugador[1].puntaje = 0;
	jugadores->cantidad = 2;
	jugadores->turno = 1;
	Ttablero* tablero = crearTablero();
	int actual = esGanador(jugadores, tablero);
	int esperado = NO_GANADOR;
	char * mensaje = "comando invalido";
	CuAssertIntEquals_Msg(cuTest, mensaje, esperado, actual);
}

void
movimientosValidosTest(CuTest * cuTest)
{

    Tjugada* movimiento = malloc(sizeof(Tjugada));
    movimiento->iniFil = 2;
    movimiento->iniCol = 0;
    movimiento->desFil = 3;
    movimiento->desCol = 0;

    //levantamos un tablero
    Ttablero* tablero = crearTablero();
	int actual = verificarJugada(movimiento,tablero);
	int esperado = CORRECTO;
	char * mensaje = "movimiento invalido";
	CuAssertIntEquals_Msg(cuTest, mensaje, esperado, actual);
}

void
movimientosInvalidosTest(CuTest * cuTest)
{


    Tjugada* movimiento = malloc(sizeof(Tjugada));
    movimiento->iniFil = -1;
    movimiento->iniCol = 0;
    movimiento->desFil = 3;
    movimiento->desCol = 0;

    //levantamos un tablero
    Ttablero* tablero = crearTablero();
	int actual = verificarJugada(movimiento,tablero);
	int esperado = FUERA_DE_RANGO1;
	char * mensaje = "movimiento invalido";
	CuAssertIntEquals_Msg(cuTest, mensaje, esperado, actual);
}

void
cortesTest(CuTest * cuTest)
{

    Tjugada* movimiento = malloc(sizeof(Tjugada));
    movimiento->iniFil = 2;
    movimiento->iniCol = 0;
    movimiento->desFil = 3;
    movimiento->desCol = 0;

    //levantamos un tablero
    Ttablero* tablero = crearTablero();
    verificarJugada(movimiento,tablero);
	int actual = realizarJugada(*movimiento,tablero);
	int esperado = CANT_CORTES;
	char * mensaje = "cortes  invalido";
	CuAssertIntEquals_Msg(cuTest, mensaje, esperado, actual);
}



static Ttablero* crearTablero(void)
{
    int k = 0;
    Ttablero* tablero = malloc(sizeof(Ttablero));
    tablero->dim = 5;
    tablero->colores = 6;
    tablero->matriz = crearMatriz(tablero->dim, tablero->dim);
    char muestra[] = {'A','B',0,'C','D','D','B',0,'C','D','C','D',0,'B','D','C','B','E','C','E',0,0,0,'D','A'};
    for(int i = 0; i < tablero->dim;i++)
    {
        for(int j = 0; j < tablero->dim; j++)
            tablero->matriz[i][j] = muestra[k++];
    }
    return tablero;
}
