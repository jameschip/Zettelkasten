#include "journal.h"
#include "entry.h"
#include "zettelkasten.h"
#include "string_helpers.h"

#include <string.h>
#include <menu.h>
#include <stdio.h>
#include <stdlib.h>


enum win_focus {
    FOCUS_TITLE,
    FOCUS_CONTENT,
    FOCUS_TAG,
    FOCUS_OK,
    FOCUS_CANCEL
};


void home_cursor_in_window( WINDOW * win ) {
    wmove( win, 0, 0 );
}

char * read_content_string( WINDOW * win ) {
    char * content = calloc(1000, sizeof(char) );
    char * str = calloc(1000, sizeof(char) );
    for ( int x = 0; x < 11; x++ ) {
        int num = mvwinstr( win, x, 0, str );
        strcat( content, str );
    }
    free( str );
    return content;
}

int zk_screen_add_entry( void ) {
    
    WINDOW * _title_win;
    WINDOW * _content_win;
    WINDOW * _tags_win;
    WINDOW * _create_win;
    WINDOW * _cancel_win;
    WINDOW * focusesd_win;

    int row,col;
    int c;
    int return_value = SCREEN_LIST;

    int focused_window_id = FOCUS_TITLE;
    
    curs_set(1);
    
    getmaxyx( stdscr,row,col );

    print_text_underlined( stdscr, 2, (col / 2) - 32, "Title:");

    print_text_underlined( stdscr, 4, (col / 2) - 40, "Entry content:");
    
    print_text_underlined( stdscr, 15, (col / 2) - 31, "Tags:");

    refresh();

    _title_win = create_newwin_lb(1, 50, 2, (col / 2) - 25 );
    _content_win = create_newwin_lb(10, 50, 4, (col / 2) - 25 );
    _tags_win = create_newwin_lb(1, 50, 15, (col / 2) - 25 );
    _cancel_win = create_newwin(1, 10, 17, (col / 2) - 25 );
    _create_win = create_newwin(1, 10, 17, (col / 2) );
    
    print_text_underlined( _cancel_win, 0, 0, "Cancel" );
    print_text_underlined( _create_win, 0, 0, "Ok" );

    keypad( _title_win, TRUE );
    keypad( _content_win, TRUE );
    keypad( _tags_win, TRUE );
    keypad( _cancel_win, TRUE );
    keypad( _create_win, TRUE );

    wrefresh( _title_win );
    wrefresh( _content_win );
    wrefresh( _tags_win );
    wrefresh( _cancel_win );
    wrefresh( _create_win );

    focusesd_win = _title_win;
    home_cursor_in_window( focusesd_win );
    // refresh();
    
    while( c = wgetch( focusesd_win ) ) {       
        switch(c) {	

            case '\t': {
                if (focused_window_id == FOCUS_TITLE ) {
                    focused_window_id = FOCUS_CONTENT;
                    focusesd_win = _content_win;
                } else if ( focused_window_id == FOCUS_CONTENT ) {
                    focused_window_id = FOCUS_TAG;
                    focusesd_win = _tags_win;
                } else if ( focused_window_id == FOCUS_TAG ) {
                    focused_window_id = FOCUS_CANCEL;
                    focusesd_win = _cancel_win;
                } else if ( focused_window_id == FOCUS_CANCEL ) {
                    focused_window_id = FOCUS_OK;
                    focusesd_win = _create_win;
                } else if ( focused_window_id == FOCUS_OK ) {
                    focused_window_id = FOCUS_TITLE;
                    focusesd_win = _title_win;
                } 
                home_cursor_in_window( focusesd_win );
                refresh();
            }

            case KEY_LEFT:
                getyx( focusesd_win, row, col );
                if ( col > 0)
                    wmove( focusesd_win, row, col - 1 );
                break;

            case KEY_RIGHT:
                getyx( focusesd_win, row, col );
                if ( col < 50)
                    wmove( focusesd_win, row, col + 1 );
                break;

            case KEY_UP:
                if ( focused_window_id == FOCUS_CONTENT ) {
                    getyx( focusesd_win, row, col );
                    if ( row > 0)
                        wmove( focusesd_win, row - 1, col );
                    break;
                }

            case KEY_DOWN:
                if ( focused_window_id == FOCUS_CONTENT ) {
                    getyx( focusesd_win, row, col );
                    if ( row < 12)
                        wmove( focusesd_win, row + 1, col);
                    break;
                }

            case KEY_DC:
                wdelch( focusesd_win );
                getyx( focusesd_win, row, col );
                break;

            case KEY_BACKSPACE:
            case 127:
                getyx( focusesd_win, row, col );
                if ( col > 0) {
                    wmove( focusesd_win, row, col - 1 );
                    wdelch( focusesd_win );
                }
                break;

            case 10: {
                if ( focused_window_id == FOCUS_OK ) {
                    entry * new_entry = entry_init();
                    new_entry->entry_number = journal_get_count();
                    int num;
                    char * str = malloc( 1 );
                    str[0] = '\0';
                    num = mvwinstr( _title_win, 0, 0, str );
                    entry_set_title( new_entry, str );
                    char * cont = read_content_string( _content_win );
                    entry_set_content( new_entry, cont );
                    char * tags = calloc( 51, sizeof( char ) );
                    mvwinstr( _tags_win, 0, 0, tags );
                    entry_add_tags( new_entry, tags );
                    journal_create_entry( new_entry );
                    entry_free( new_entry );
                    goto exit_loop;
                } else if ( focused_window_id == FOCUS_CANCEL ) {
                    // Cancel
                    goto exit_loop;
                } else if ( focused_window_id == FOCUS_TAG || focused_window_id == FOCUS_TITLE ) {
                    continue;
                }
            }

            default:
                if ( focused_window_id != FOCUS_OK && focused_window_id != FOCUS_CANCEL )
                    waddch( focusesd_win, c );
                break;
		}
	}

    exit_loop:;

    delwin( _title_win );
    delwin( _content_win );
    delwin( _tags_win );
    delwin( _cancel_win );
    delwin( _create_win );
    return return_value;
}

