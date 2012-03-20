/*****************************************************************************/
/*  o o o o      Berner Fachhochschule                                       */
/*        :...o  Technik und Informatik                                      */
/*****************************************************************************/
/*  Module     : LaserChess/Grafik                              Version 1.0  */
/*****************************************************************************/
/*                                                                           */
/*  Desc       : graphics functions                                          */
/*                                                                           */
/*  Procedures :                                                             */
/*                                                                           */
/*  Author     : C. Stoller                                                  */
/*                                                                           */
/*  Email      : stolc2@bfh.ch                                               */
/*                                                                           */
/*  Creation   : 17.03.2012                                                  */
/*                                                                           */
/*  File       : Grafik.c                                                    */
/*                                                                           */
/*****************************************************************************/
/*  n00bSoft                                                                 */
/*****************************************************************************/

#include "Grafik.h"

/*Umrechnung Windowskoord. zu Mapposition*/
location pixel_to_map(location Mapkoordinaten)	//bekommt windowskoordinaten gibt mapkoordinaten zur�ck
{
	Mapkoordinaten.x = (Mapkoordinaten.x-(Mapkoordinaten.x%FIELD_SIZE))/FIELD_SIZE;	//(x-(x%100)/100) nimmt Werte zwischen 0 und 7 an (da x von 0 bis 800)
	Mapkoordinaten.y = (PLAYGROUND_Y_MAX-1)-((Mapkoordinaten.y-(Mapkoordinaten.y%FIELD_SIZE))/FIELD_SIZE);	//(5-(y-(y%100)/100) nimmt Werte zwischen 0 und 5 an (da y von 0 bis 600)

	if(is_inside_map(Mapkoordinaten))	//errorhandling (wenn x genau 800 oder y genau 600)
	{
		return Mapkoordinaten;
	}
	else
	{
		Mapkoordinaten.x = -1;
		Mapkoordinaten.y = -1;
		return Mapkoordinaten;			//!!! B�rtschi - need to know oder so :p !!!
	}

}

/*Umrechnung Mapkoord. zu Windowskoord. ECKE LINKS UNTEN!*/
location map_to_pixel(location Windowskoordinaten)	//bekommt mapkoordinaten gibt windowskoordinaten zur�ck
{
	Windowskoordinaten.x *= FIELD_SIZE;	//0=>0; 1=>100; 2=>200; 3=>300; 4=>400; 5=>500; 6=>600; 7=>700
	Windowskoordinaten.y = (PLAYGROUND_Y_MAX*FIELD_SIZE)-(Windowskoordinaten.y*FIELD_SIZE);	//(600-(y*100)) 0=>600; 1=>500; 2=>400; 3=>300; 4=>200; 5=>100
	return Windowskoordinaten;
}

/*Spielfeld zeichnen*/
void draw_playground()
{
	int i;		//Anzahl verschobene Felder
	InitGraphic(PLAYGROUND_X_MAX*FIELD_SIZE, PLAYGROUND_Y_MAX*FIELD_SIZE);	//initialisiert und �ffnet ein 800*600 Grafikfenster
	DrawFilledRectangle(0, 0, PLAYGROUND_X_MAX*FIELD_SIZE, PLAYGROUND_Y_MAX*FIELD_SIZE,COL_BLACK, FIELD_BORDER_WIDTH);	//zeichnet das schwarze Spielfeld
	DrawEmptyRectangle(0, 0, PLAYGROUND_X_MAX*FIELD_SIZE, PLAYGROUND_Y_MAX*FIELD_SIZE,COL_GREY, FIELD_BORDER_WIDTH);	//zeichnet die Spielfeldumrandung

	for(i = 1; i < PLAYGROUND_X_MAX; i++)
	{
		DrawLine(0, FIELD_SIZE*i, PLAYGROUND_X_MAX*FIELD_SIZE, FIELD_SIZE*i, COL_GREY, FIELD_LINE_WIDTH);	//zeichnet die 5 grauen Horizontal-Linien
	}

	for(i = 1; i < PLAYGROUND_Y_MAX; i++)
	{
		DrawLine(FIELD_SIZE*i, 0, FIELD_SIZE*i, PLAYGROUND_Y_MAX, COL_GREY, FIELD_LINE_WIDTH);				//zeichnet die 7 grauen Vertikal-Linien
	}
}

/*Gr�ner Focus ("Umrandung") zeichnen*/
void draw_focus(location Field) //bekommt Mapkoordinaten und schreibt sie ins struct location Field
{
	location Punkt;					//initialisieren: struct location Punkt
	Punkt = map_to_pixel(Field);	//Umwandlung der Mapkoordinaten in Windowskoordinaten (Punkt links unten des ausgew. Feldes)
	DrawEmptyRectangle(Punkt.x+FOCUS_IDENT, Punkt.y-FOCUS_IDENT, PLAYGROUND_X_MAX-2*FOCUS_IDENT, PLAYGROUND_Y_MAX-2*FOCUS_IDENT, COL_GREEN, 2*FOCUS_IDENT);	//um 5 Pixel einr�cken (x-5,y-5,90,90,Gr�n,5)
}

