#include "journal.h"
#include "entry.h"
#include "zettelkasten.h"

#include <string.h>
#include <menu.h>
#include <stdio.h>
#include <stdlib.h>

int zk_screen_list( const char * message ) {

    

    int row, col;
    uint64_t  file_count = 0;
    WINDOW * _e_window;
    MENU * _e_menu;
    ITEM ** _e_items;
    int c;				
    int i;
    uint8_t action_value = SCREEN_EXIT;

    curs_set(0);

    char ** entry_refs = get_all_entry_refs( filter_tag , &file_count );
    
    _e_items = (ITEM **)calloc( file_count + 1, sizeof(ITEM *) );        
    
    getmaxyx( stdscr,row,col );
    move( 2, 0 );
    clrtoeol();

    // // Print the title
    attron( A_UNDERLINE );
    if ( filter_tag == NULL) {
        mvprintw( 2, ( col - strlen( message ) ) / 2, message );
    } else {
        char * f_msg = calloc(50, sizeof( char ) );
        sprintf( f_msg, "%s {%s}", message, filter_tag );
        mvprintw( 2, ( col - strlen( f_msg ) ) / 2, f_msg );
        free( f_msg );
    }
    
    attroff( A_UNDERLINE );
    refresh();

    // // Create the links menu and enter add the entries to it.
    _e_window = create_newwin( 10, 50, 4, (col / 2) - 25 );
    keypad( _e_window, TRUE );
    
    if ( file_count > 0 ) {
        for( i = 0; i < file_count; ++i ) {
            _e_items[i] = new_item( entry_refs[i], "" );
        }
        
    }
    _e_items[file_count] = (ITEM *)NULL;

    _e_menu = new_menu( _e_items );
    set_menu_win( _e_menu, _e_window );        
    set_menu_sub( _e_menu, derwin( _e_window, 0, 0, 0, 0 ) );
    set_menu_format(_e_menu, 10, 1);
    set_menu_mark( _e_menu, "" );
    post_menu( _e_menu ); 
    wrefresh( _e_window );

    while( c = wgetch( _e_window ) ) {       
        switch(c) {

            case 'q':
                action_value = SCREEN_EXIT;
                goto exit_loop;

            case KEY_DOWN:
                menu_driver( _e_menu, REQ_DOWN_ITEM );
                wrefresh( _e_window );
                break;

            case KEY_UP:
                menu_driver( _e_menu, REQ_UP_ITEM );
                wrefresh( _e_window );
                break;

            case 'c':
                if (filter_tag != NULL ) {
                    free( filter_tag );
                    filter_tag = NULL;
                    action_value = SCREEN_LIST;
                    goto exit_loop;
                }
                break;

            case 'f':
                action_value = SCREEN_FILTER_INPUT;
                goto exit_loop;

            case 'a':
                action_value = SCREEN_ADD;
                goto exit_loop;
                
            case 10:   {    // Return Key.
                if ( file_count > 0 ) {
                    ITEM * cur; 
                    cur= current_item(_e_menu);
                    char * name = (char *)item_name( cur );
                    char * p;
                    entry_to_read = strtol(&name[1], &p, 10);
                    action_value = SCREEN_DISPLAY;
                    goto exit_loop;
                }
                break;
            } 

        }
    }

    exit_loop:; // Using a goto init!

    unpost_menu( _e_menu );
    free_menu( _e_menu );
    for( i = 0; i < file_count; ++i ) {
        free_item( _e_items[i] );
        free(entry_refs[i]);
    }

    filter_tag = NULL;

    delwin( _e_window );
 
    return action_value;

}
