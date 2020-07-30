#include "journal.h"
#include "entry.h"
#include "zettelkasten.h"

#include <string.h>
#include <menu.h>
#include <stdio.h>
#include <stdlib.h>

int zk_screen_tag_filter( void ) {
    
    int row, col;
    char * title = "Filter by tag:";
    int c;
    int tag_len = 0;
    WINDOW * _t_window;

    curs_set(1);

    getmaxyx( stdscr,row,col );
    
    // Print the title
    attron( A_UNDERLINE );
    mvprintw( ( row / 4 ) - 1, ( col - strlen( title ) ) / 2, title );
    attroff( A_UNDERLINE );
    refresh();
    
    _t_window = create_newwin( 1, 50, ( row / 4 ) + 1, (col / 2) - 25 );
    keypad( _t_window, TRUE );
    wrefresh( _t_window );

    
    while(( c = wgetch( _t_window ) ) != 10 ) {       
        switch (c) {

            case KEY_LEFT:
                getyx( _t_window, row, col );
                if ( col > 0 ) {
                    wmove( _t_window, row, col - 1 );
                }
                break;
            
            case KEY_RIGHT:
                getyx( _t_window, row, col );
                if ( col < tag_len ) {
                    wmove( _t_window, row, col + 1 );
                }
                break;
            
            case KEY_DC:
                wdelch( _t_window );
                getyx( _t_window, row, col );
                if ( col < tag_len ) {
                    tag_len--;
                }
                break;
            
            case KEY_BACKSPACE:
            case 127:
                getyx( _t_window, row, col );
                if ( col > 0) {
                    wmove( _t_window, row, col - 1 );
                    wdelch( _t_window );
                    tag_len--;
                }
                break;
            
            default:
                tag_len++;
                waddch( _t_window, c );
                break;
        }
	}
    
    wmove( _t_window, 0, 0 );
    char * tmp = realloc( filter_tag, tag_len + 1);
    if ( tmp != NULL ) {
        filter_tag = tmp;
    }
    winnstr( _t_window, filter_tag, tag_len + 1 );
    filter_tag[tag_len] = '\0';
    delwin( _t_window );

    return SCREEN_LIST;
}