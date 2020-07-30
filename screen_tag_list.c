#include "journal.h"
#include "entry.h"
#include "zettelkasten.h"

#include <string.h>
#include <menu.h>
#include <stdio.h>
#include <stdlib.h>

int zk_screen_tag_list( uint64_t ent_num ) {

    int row, col;
    WINDOW * _e_window;
    MENU * _e_menu;
    ITEM ** _e_items;
    int c;				
    int i;
    uint8_t action_value = SCREEN_EXIT;

    entry * e = entry_init( );
    journal_read_entry( e, ent_num );

    curs_set(0);

    _e_items = (ITEM **)calloc( e->tag_count + 1, sizeof( ITEM * ) );        
    
    getmaxyx( stdscr,row,col );
    move( 2, 0 );
    clrtoeol();
    char * message = calloc(50, sizeof( char ) );
    sprintf( message, "Tags for {%lu}.", e->entry_number );

    // // Print the title
    attron( A_UNDERLINE );
    mvprintw( 2, ( col - strlen( message ) ) / 2, message );
    attroff( A_UNDERLINE );
    refresh();

    // // Create the links menu and enter add the entries to it.
    _e_window = create_newwin( 10, 50, 4, (col / 2) - 25 );
    keypad( _e_window, TRUE );
    
    if ( e->tag_count > 0 ) {
        for( i = 0; i < e->tag_count; ++i ) {
            _e_items[i] = new_item( e->tags[i], "" );
        }
        
    }
    _e_items[e->tag_count] = (ITEM *)NULL;

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
                action_value = SCREEN_DISPLAY;
                goto exit_loop;

            case KEY_DOWN:
                menu_driver( _e_menu, REQ_DOWN_ITEM );
                wrefresh( _e_window );
                break;

            case KEY_UP:
                menu_driver( _e_menu, REQ_UP_ITEM );
                wrefresh( _e_window );
                break;
                
            case 10:   {    // Return Key.
                if ( e->tag_count > 0 ) {
                    ITEM * cur; 
                    cur= current_item(_e_menu);
                    char * t = (char *)item_name( cur );
                    filter_tag = calloc(50, sizeof( char ) );
                    sprintf(filter_tag, "%s", t);
                    action_value = SCREEN_LIST;
                    goto exit_loop;
                }
                break;
            } 

        }
    }

    exit_loop:; // Using a goto init!

    unpost_menu( _e_menu );
    free_menu( _e_menu );
    for( i = 0; i < e->tag_count; ++i ) {
        free_item( _e_items[i] );
    }
    
    entry_free( e );

    delwin( _e_window );
 
    return action_value;

}
