
#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "journal.h"
#include "zettelkasten.h"
#include <menu.h>

int main () {
	init_journal();
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	
	int state = SCREEN_LIST;

	while( state != SCREEN_EXIT ) {
		switch ( state ) {
			
			case SCREEN_LIST:
    			state = zk_screen_list( "Select entry to read" );
				break;

			case SCREEN_DISPLAY:
				state = zk_screen_display( entry_to_read );
				break;

			case SCREEN_FILTER_INPUT:
				state = zk_screen_tag_filter();
				break;
			
			case SCREEN_ADD:
				state = zk_screen_add_entry();
				break;

			case SCREEN_CREATE_LINK:
				state = zk_do_add_link();
				break;

			case SCREEN_TAG_LIST: 
				state = zk_screen_tag_list( entry_to_read );
				break;
				
			case SCREEN_ADD_TAG:
				state = zk_screen_add_tag();
                break;

			default:
				break;

		}

		clear();
		
	}

	endwin();

    return 0;
    
}
