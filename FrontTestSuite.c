#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buttonsAndScissorsBack.h"
#include "buttonsAndScissorsFront.h"
#include "CuTest.h"

void leerCoordenadasTest(CuTest * cuTest);
void lecturaDeTableroInicialTest(CuTest * cuTest);
void cargarPartidaTest(CuTest * cuTest);
void guardarPartidaTest(CuTest * cuTest);
static int compararTableros(Ttablero* tablero);




CuSuite *
frontSuite(void)
{
	CuSuite * cuSuite = CuSuiteNew();
	SUITE_ADD_TEST(cuSuite, leerCoordenadasTest);
	SUITE_ADD_TEST(cuSuite, lecturaDeTableroInicialTest);
    SUITE_ADD_TEST(cuSuite,cargarPartidaTest) ;
    SUITE_ADD_TEST(cuSuite,guardarPartidaTest) ;
	return cuSuite;
}

void
leerCoordenadasTest(CuTest * cuTest)
{
    Tjugada* movimiento = malloc(sizeof(Tjugada));
    char* nombre = malloc(10);
	char* s = "\n]2,3[]2,2[";
	for(int i = 0; s[i] != 0;i++)
		ungetc(s[i],stdin);




	int actual = leerComando(movimiento,nombre);
	//fputs("[0,1][1,3]\n", stdin);
	int esperado = CORRECTO;
	char * mensaje = "comando invalido";
	CuAssertIntEquals_Msg(cuTest, mensaje, esperado, actual);
}

void
lecturaDeTableroInicialTest(CuTest * cuTest)
{
    /*
    Aqui probamos si funciona la carga de una martiz de 5x5 que se encuentre en
    dicho archivo mediante la funcion de front abrirTablero.
    */

    Ttablero* tablero = malloc(sizeof(Ttablero));

    tablero->dim = 5;

    tablero->colores = 6;
    tablero->matriz = crearMatriz(tablero->dim, tablero->dim);
	int actual = abrirTablero(tablero,"5x5");
	int esperado = CORRECTO;
	char * mensaje = "tablero invalido";
	CuAssertIntEquals_Msg(cuTest, mensaje, esperado, actual);
}

void
cargarPartidaTest(CuTest * cuTest)
{   /*
    esta funcion prueba si se puede leer una partida guardada con anterioridad
    para ello consideramos si la matriz es la misma y si se mantiene el modo de juego
    en el ejemplo la partida es contra la computadora
    */

    Ttablero* tablero = malloc(sizeof(Ttablero));
    Tjugadores* jugadores = malloc(sizeof(Tjugadores));
    jugadores->cantidad = 2;
    jugadores->jugador = malloc(sizeof(Tjugador)*2);
	cargarPartidaBack("prueba",jugadores, tablero);
    //jugadores->jugador[1].com == 1 verifica que el jugador 2 sea la computadora
    int actual = (jugadores->jugador[1].com == 1) && compararTableros(tablero);
    int esperado = CORRECTO;
	char * mensaje = "archivo corrupto";
	CuAssertIntEquals_Msg(cuTest, mensaje, esperado, actual);
}

static int compararTableros(Ttablero* tablero)
{
    char muestra[] = {'A','B',0,'C','D','D','B',0,'C','D','C','D',0,'B','D','C','B','E','C','E',0,0,0,'D','A'};
    int k = 0;
    if(tablero->dim != 5)
        return ERROR;
    for(int i = 0; i < tablero->dim; i++)
    {
        for(int j = 0; j<tablero->dim; j++)
        {
            if(tablero->matriz[i][j] != muestra[k++])
                return ERROR;
        }
    }

    return CORRECTO;
}

void
guardarPartidaTest(CuTest * cuTest)
{   /*
    esta funcion prueba si se puede guardar una partida
    para ello consideramos si la matriz es la misma y si se mantiene el modo de juego
    partida contra otro jugador
    */

    Ttablero* tablero = malloc(sizeof(Ttablero));
    Tjugadores* jugadores = malloc(sizeof(Tjugadores));
    jugadores->cantidad = 2;
    jugadores->jugador = malloc(sizeof(Tjugador)*2);
    cargarPartidaBack("pruebaPVP",jugadores, tablero);
    int actual = guardarPartida("pruebaPVP",*jugadores, tablero);
	int esperado = CORRECTO;
	char * mensaje = "archivo corrupto";
	CuAssertIntEquals_Msg(cuTest, mensaje, esperado, actual);
}
