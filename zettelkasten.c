
#include "journal.h"
#include "entry.h"
#include "zettelkasten.h"

#include <string.h>
#include <menu.h>
#include <stdio.h>
#include <stdlib.h>

// #define _DEBUG_

char * filter_tag = NULL;

int last_entry;
int link_entry;
int entry_to_read;

WINDOW *create_newwin(int height, int width, int starty, int startx)
{	
    WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
    
    #ifdef _DEBUG_
        box(local_win, 0 , 0);		/* 0, 0 gives default characters 
                                    * for the vertical and horizontal
                                    * lines			*/
        wrefresh(local_win);		/* Show that box 		*/
    #endif

	return local_win;
}

WINDOW *create_newwin_lb(int height, int width, int starty, int startx)
{	
    WINDOW *local_win;
	local_win = newwin(height, width, starty, startx);
    wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wrefresh(local_win);
	return local_win;
}

void print_text_underlined( WINDOW * win, int row, int col, char* text) {
    attron( A_UNDERLINE );
    mvwprintw( win, row, col, text );
    attroff( A_UNDERLINE );
}

int zk_do_add_link() {
    int state = SCREEN_LIST;
    last_entry = entry_to_read;
    while ( state != SCREEN_DISPLAY ) {

        switch ( state ) {

            case SCREEN_ADD:
            case SCREEN_LIST:
                state = zk_screen_list( "Select entry to link to" );
                if (state = SCREEN_DISPLAY) {
                    state = SCREEN_LINK_CONTEXT;
                }
                break;

            case SCREEN_EXIT:
                return SCREEN_DISPLAY;

            case SCREEN_FILTER_INPUT:
				state = zk_screen_tag_filter();
				break;
            
            case SCREEN_LINK_CONTEXT:
                state = zk_screen_link_context();
                entry * ent = entry_init(); 
                journal_read_entry( ent, last_entry );
                entry_add_link( ent , entry_to_read, filter_tag );
                journal_update_entry( ent );
                entry_free( ent );
                entry_to_read = last_entry;
                free(filter_tag);
                filter_tag = NULL;
                return SCREEN_DISPLAY;
                break;
        }

        clear();
    }

    return state;

}



