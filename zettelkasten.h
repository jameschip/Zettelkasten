#ifndef ZETTELKASTEN_H
#define ZETTELKASTEN_H

#include <ncurses.h>

enum nav_states {
    SCREEN_EXIT,
    SCREEN_DISPLAY,
    SCREEN_LIST,
    SCREEN_ADD,
    SCREEN_CLEAR_FILTER,
    SCREEN_FILTER_INPUT,
    SCREEN_CREATE_LINK,
    SCREEN_LINK_CONTEXT,
    SCREEN_TAG_LIST
};

extern char * filter_tag;

extern int last_entry;
extern int link_entry;
extern int entry_to_read;

WINDOW *create_newwin(int height, int width, int starty, int startx);
WINDOW *create_newwin_lb(int height, int width, int starty, int startx);
void print_text_underlined( WINDOW * win, int row, int col, char* text);

int zk_screen_display( uint64_t e );
int zk_screen_list( const char * message );
int zk_screen_tag_filter( void );
int zk_screen_add_entry( void );
int zk_do_add_link( void );
int zk_screen_link_context( void );
int zk_screen_tag_list( uint64_t ent_num );

#endif
