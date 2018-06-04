#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "buttonsAndScissorsBack.h"
#include "buttonsAndScissorsFront.h"
#define ERROR 0
#define PVP 1
#define PVCOM 2
#define LOAD 3
#define TERMINAR 4
#define CORRECTO 1
#define CANT_JUGADORES 2

int
main (void)
{
	srand(time(NULL));
	Ttablero tableroActual;
	Tjugadores jugadores;
	jugadores.cantidad = CANT_JUGADORES;
	jugadores.jugador = malloc(jugadores.cantidad * sizeof(Tjugador));
	int resp;
	do
	{
    resp = menu();
    switch (resp)
    	{
    		case PVCOM:
    			inizializarJugadores(&jugadores);
    			elegirDificultad (&jugadores);
    			if (cargarTablero(&tableroActual) == CORRECTO)		
    				jugar(&jugadores, &tableroActual);
    			break;
    			
    		case PVP:
    			inizializarJugadores(&jugadores);
    			if (cargarTablero(&tableroActual) == CORRECTO)		
    				jugar(&jugadores, &tableroActual);
    			break;
    			
    		case LOAD:	    		
    			if (cargarPartida(&jugadores, &tableroActual) == CORRECTO)
    				jugar(&jugadores, &tableroActual);
    			break;
    		default:
    			break;
    	}
    }
    while (resp != TERMINAR);
    free(jugadores.jugador);
    return 0;
}

