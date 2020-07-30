#include "journal.h"
#include "entry.h"
#include "zettelkasten.h"

#include <string.h>
#include <menu.h>
#include <stdio.h>
#include <stdlib.h>

int zk_screen_display( uint64_t e ) {

    int row,col;
    entry * ent = entry_init();
    WINDOW * content_win;
    WINDOW * link_window;
    MENU * link_menu;
    ITEM **link_items;
    int c;				
    int i;
    uint8_t return_value = SCREEN_EXIT;

    curs_set(0);

    getmaxyx( stdscr,row,col );
    
    journal_read_entry( ent, e );

    // Print the title
    attron( A_UNDERLINE );
    char en[10];
    sprintf( en, "%i", ent->entry_number );
    mvwprintw( stdscr, 2, ( col - (strlen( ent->title ) + strlen( en ) + 3) ) / 2, "{%i} %s", ent->entry_number, ent->title );
    attroff( A_UNDERLINE );
    refresh();

    // Create the window for the content of the note
    // and put the contents in it.
    content_win = create_newwin(10, 50, 4, (col / 2) - 25 );
    mvwprintw( content_win, 0, 0,"%s", ent->content );
    wrefresh( content_win );
    
    // Create the links menu and enter add the entries to it.
    link_window = create_newwin( 10, 50,15, (col / 2) - 25 );
    keypad( link_window, TRUE );

    link_items = (ITEM **)calloc( ent->link_count + 1, sizeof(ITEM *) );        
    
    for( i = 0; i < ent->link_count; ++i ) {
        link_items[i] = new_item(ent->links[i], " ");
    }

    link_items[ent->link_count] = (ITEM *)NULL;

    link_menu = new_menu( link_items );
    set_menu_win( link_menu, link_window );        
    set_menu_sub( link_menu, derwin( link_window, 0, 0, 0, 0 ) );
    set_menu_format(link_menu, 10, 1);
    set_menu_mark( link_menu, "" );
    post_menu( link_menu ); 
    wrefresh( link_window );

    while( c = wgetch( link_window ) ) {       
        switch(c) {	
            
            case KEY_DOWN:
				menu_driver( link_menu, REQ_DOWN_ITEM );
                wrefresh( link_window );
				break;
			
            case KEY_UP:
				menu_driver( link_menu, REQ_UP_ITEM );
                wrefresh( link_window );
				break;
            
            case 'q':
                return_value = SCREEN_LIST;
                goto exit_loop;
                break;

            case 'L':
                return_value = SCREEN_CREATE_LINK;
                goto exit_loop;
            
            case 'T':
                return_value = SCREEN_TAG_LIST;
                goto exit_loop;
                
            case 10: {
                if ( ent->link_count == 0 ) continue;
                ITEM * cur; 
                cur = current_item(link_menu);
                char * name = (char *)item_name( cur );
                char * p;
                entry_to_read = strtol(&name[1], &p, 10);
                return_value = SCREEN_DISPLAY;
                goto exit_loop;
                break;
            }   
		}
	}

    exit_loop:;

    unpost_menu( link_menu );
    free_menu( link_menu );
    
    for( i = 0; i < ent->link_count; ++i ) {
        free_item( link_items[i] );
    }
    
    delwin( link_window );
    delwin( content_win );

    entry_free( ent );
    
    return return_value;

}