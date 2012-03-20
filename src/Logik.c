/*****************************************************************************/
/*	o o o o      Berner Fachhochschule										 */
/*		  :...o  Technik und Informatik										 */
/*****************************************************************************/
/*  Module     : LaserChess/Logik                               Version 1.0  */
/*****************************************************************************/
/*                                                                           */
/*  Function   :                                                             */
/*                                                                           */
/*  Procedures : laser()  spiel()  some other helper tools                   */
/*                                                                           */
/*  Author     : C. Stoller                                                  */
/* 																			 */
/*  Email	   : stolc2@bfh.ch  						                     */
/*                                                                           */
/*  History    : 17.03.2012  File created                                    */
/*                                                                           */
/*  File       : Logik.c                                                     */
/*                                                                           */
/*****************************************************************************/
/*  n00bSoft                                                                 */
/*****************************************************************************/

#include "Grafik.h"
#include "Logik.h"
#include "LaserChess.h"
#include "window.h"


/*****************************************************************************/
/*  Function   : laser                                          Version 1.0  */
/*****************************************************************************/
/*                                                                           */
/*  Function   :                                                             */
/*                                                                           */
/*  Input Para :                                                             */
/*                                                                           */
/*  Output     : if a wall was hit: 0. If a king was hit: -1 for player_red, */
/*               -2 for player_blue. If a mirror was hit: +1 for player_red, */
/*               +2 for player_blue                                          */
/*                                                                           */
/*  Author     : C. Stoller                                                  */
/*                                                                           */
/*  Email      : stolc2@bfh.ch                                               */
/*                                                                           */
/*****************************************************************************/

int laser(location pos, enum Direction dir)
{
    location next_pos = pos;

    switch(dir)
    {
        case RIGHT:
            next_pos.x++;
            break;

        case UP:
            next_pos.y++;
            break;

        case LEFT:
            next_pos.x--;
            break;

        case DOWN:
            next_pos.y--;
            break;
    }


    if(!is_inside_map(next_pos))
    {
        // wenn nicht mehr im spielfeld, in eine wand gefahren. -> return 0
        return 0;
    }
    else
    {
        pawn *next_pawn = map[next_pos.x][next_pos.y];
        int return_value, reflection;

        if(!is_figure(next_pawn))
        {
            // Leeres Feld: Linie zeichnen, sich selbst ausf�hren, linie wieder l�schen
            draw_laser(next_pos, dir);
            return_value = laser(next_pos, dir);
            draw_empty_field(next_pos);
            return return_value;
        }
        else
        {
            // wenn eine Figur: was f�r eine?
            switch(next_pawn->TYPE)
            {
                case WALL:
                    // Mauer getroffen: aufh�ren, wie bei is_inside_map = 0
                    return 0;
                
                case KING:
                    // K�nig getroffen: Player negativ zur�ckgeben
                    draw_king_destroyed(next_pawn);
                    // SLEEP ca 2sek!
                    WaitMs(2000);
                    return -(next_pawn->PLAYER);

                case MIRROR:
                    // Spiegel getroffen: reflektion?
                    reflection = dir - next_pawn->DIR;
                    NORM(reflection);

                    switch(reflection)
                    {
                        case 0:
                        case 1:
                            // zerst�rung: Spiegel positiv zur�ckgeben
                            draw_mirror_destroyed(next_pawn);
                            // SLEEP ca 2sek!
                            WaitMs(2000);
                            return next_pawn->PLAYER;

                        case 2:
                            // Reflektion um 90� nach rechts (CW)
                            ROTATE_RIGHT(dir);
                            // Linie zeichnen, angle = -1 (CW)
                            draw_angled_laser(next_pos, dir, -1);

                            // sich selbst ausf�hren und danach linie wieder l�schen
                            return_value = laser(next_pos, dir);
                            draw_figure(next_pawn);
                            return return_value;

                        case 3:
                            // Reflektion um 90� nach links (CCW)
                            ROTATE_LEFT(dir);
                            // Linie zeichnen, angle = +1 (CCW)
                            draw_angled_laser(next_pos, dir, 1);

                            // sich selbst ausf�hren und danach linie wieder l�schen
                            return_value = laser(next_pos, dir);
                            draw_empty_field(next_pos);
                            draw_figure(next_pawn);
                            return return_value;
                    }
                    break;
                
                case SPLITTER:
                    // Splitter getroffen: welche Reflektion? (keine zerst�rung m�glich)
                    reflection = dir - next_pawn->DIR;
                    NORM(reflection);

                    switch(reflection)
                    {
                        case 0:
                        case 2:
                            // Reflektion um 90� nach rechts (CW)
                            ROTATE_RIGHT(dir);
                            // Linie zeichnen, angle = -1 (CW)
                            draw_angled_laser(next_pos, dir, -1);

                            // sich selbst ausf�hren und danach linie wieder l�schen
                            return_value = laser(next_pos, dir);
                            draw_figure(next_pawn);
                            return return_value;

                        case 1:
                        case 3:
                            // Reflektion um 90� nach links (CCW)
                            ROTATE_LEFT(dir);
                            // Linie zeichnen, angle = +1 (CCW)
                            draw_angled_laser(next_pos, dir, 1);

                            // sich selbst ausf�hren und danach linie wieder l�schen
                            return_value = laser(next_pos, dir);
                            draw_empty_field(next_pos);
                            draw_figure(next_pawn);
                            return return_value;
                    }
                    break;
            }
        }
    }

    return 0;
}


/*****************************************************************************/
/*  Function   : is_inside_map                                  Version 1.0  */
/*****************************************************************************/
/*                                                                           */
/*  Function   : checks if the given coordinates are inside the array        */
/*                                                                           */
/*  Input Para : given coordinates                                           */
/*                                                                           */
/*  Output     : if inside map (means, inside the range [0 - 7][0 - 5], then */
/*               it returns 1. Otherwise it returns 0.                       */
/*                                                                           */
/*  Author     : C. Stoller                                                  */
/*                                                                           */
/*  Email      : stolc2@bfh.ch                                               */
/*                                                                           */
/*****************************************************************************/

int is_inside_map(location pos)
{
    // wenn innerhalb der definierten Array-grenzen:
    if(pos.x < PLAYGROUND_X_MAX && pos.x > 0
       pos.y < PLAYGROUND_Y_MAX && pos.y > 0)
    {
        // true zur�ckgeben
        return 1;
    }
    else
    {
        // false zur�ckgeben
        return 0;
    }
    
    // Python:
    // return 1 if (0 < pos.x < PLAYGROUND_X_MAX) and (0 < pos.y < PLAYGROUND_Y_MAX) else 0
    // # - just sayin'
}


/*****************************************************************************/
/*  Function   : is_figure                                      Version 1.0  */
/*****************************************************************************/
/*                                                                           */
/*  Function   : checks if the given coordinates contains a figure           */
/*                                                                           */
/*  Input Para : given coordinates                                           */
/*                                                                           */
/*  Output     : if there is a figure, returns 1, if its an empty field,     */
/*               returns 0. (a wall is treatened as a figure)                */
/*                                                                           */
/*  Author     : C. Stoller                                                  */
/*                                                                           */
/*  Email      : stolc2@bfh.ch                                               */
/*                                                                           */
/*****************************************************************************/

int is_figure(location pos)
{
    // wenn map dort einen NULL pointer enth�lt (= keine figure):
    if(map[pos.x][pos-y] == NULL)
    {
        // false zur�ckgeben
        return 0;
    }
    else
    {
        // true zur�ckgeben
        return 1;
    }
}


/*****************************************************************************/
/*  Function   : move_figure                                    Version 1.0  */
/*****************************************************************************/
/*                                                                           */
/*  Function   : moves a figure to the given location                        */
/*                                                                           */
/*  Input Para :                                                             */
/*                                                                           */
/*  Output     : none                                                        */
/*                                                                           */
/*  Author     : C. Stoller                                                  */
/*                                                                           */
/*  Email      : stolc2@bfh.ch                                               */
/*                                                                           */
/*****************************************************************************/

void move_figure(*pawn figure, location new_pos)
{
    // clearing the new field:
    draw_empty_field(new_pos);
    
    // changing the new location in the figure struct
    figure->POS = new_pos;
    
    // drawing the figure there:
    draw_figure(figure);
}