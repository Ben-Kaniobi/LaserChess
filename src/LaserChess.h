#ifndef LASERCHESS_H
#define LASERCHESS_H

/*****************************************************************************/
/*	o o o o      Berner Fachhochschule										 */
/*		  :...o  Technik und Informatik										 */
/*****************************************************************************/
/*  Header     : LaserChess/Mapdefinition                       Version 1.0  */
/*****************************************************************************/
/*                                                                           */
/*  Function   :                                                             */
/*                                                                           */
/*  Procedures :                                                             */
/*                                                                           */
/*  Author     : C. Stoller  												 */
/* 																			 */
/*  Email	   : stolc2@bfh.ch  						                     */
/*                                                                           */
/*  History    : 17.03.2012  File created                                    */
/*                                                                           */
/*  File       : LaserChess.h                                                */
/*                                                                           */
/*****************************************************************************/
/*  n00bSoft                                                                 */
/*****************************************************************************/

enum Affiliation {PLAYER_RED = 1, PLAYER_BLUE = 2};
enum Species {KING, MIRROR, SPLITTER, WALL, CANNON};
enum Orientation {TOP_RIGHT = 0, TOP_LEFT = 1, BOTTOM_LEFT = 2, BOTTOM_RIGHT = 3};
enum Direction {LEFT, UP, RIGHT, DOWN};
enum Angle {CCW = 1, CW = -1};
enum Spielmodus {NORMALMODE, SETMODE, EXIT};

#define NORM(a) (a<0 ? a+=4 : (a>3 ? a%=4 : a))

#define ROTATE_LEFT(a) a++; NORM(a)
#define ROTATE_RIGHT(a) a--; NORM(a)

#define ANZ_FIGURES		14

// Figurearray zuerst rot dann blau, umrechnen
#define RED_FIG(i) (i/2)
#define BLUE_FIG(i)	((i/2)+7)

// Map-Koordinatenstruct
typedef struct {
	int x;
	int y;
} location;

// Deklaration der Spielfigur
typedef struct {
	enum Affiliation PLAYER;
	enum Species TYPE;
	enum Orientation DIR;
	location Pos;
}pawn;

// Globale Variable

//pawn *map[PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX];
pawn *map[8][6];

#endif
