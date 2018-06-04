#ifndef _buttonsAndScissorsFront_h
#define _buttonsAndScissorsFront_h

/*buttonsAndScissorsFront.h*/
/******************************************************************************************************************************************************/
//PROTOTIPOS
/******************************************************************************************************************************************************/
int elegirNivel (void);
int abrirTablero(Ttablero* tableroActual, char* nombre);
int guardarPartida(char* nombredelarchivo, Tjugadores jugadores, Ttablero* tableroActual);
int cargarPartida( Tjugadores* jugadores, Ttablero* tableroActual);
int menu (void);
int leerComando(Tjugada * movimiento, char * nombredelarchivo);
void jugar (Tjugadores * jugadores, Ttablero * tableroActual);
int cargarTablero (Ttablero * tableroActual);
int cargarPartidaBack(char* nombredelarchivo, Tjugadores* jugadores, Ttablero* tableroActual);

#endif

