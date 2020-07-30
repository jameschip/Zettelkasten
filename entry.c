
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "entry.h"
#include "string_helpers.h"

entry * entry_init( void ) 
{
    entry * e = malloc( sizeof( entry ) );
 
    e->entry_number = 0;
    e->tag_count = 0;
    e->link_count = 0;

    e->title = malloc( 1 );
    e->title[0] = '\0';
    e->content = malloc( 1 );
    e->content[0] = '\0';

    return e;
}

void entry_add_link( entry * e, uint64_t n, char * l ) {
    char * tmp = calloc(100,  sizeof( char ) );
    sprintf( tmp, "{%lu} : %s", n, l );
    e->links[e->link_count++] = strdup( tmp );
    free( tmp );
}

void entry_add_tags( entry * e, char * tags ) {
    int index, ch_p = 0; 
    char * tag = calloc( 50, sizeof( char ) );

    for (index = 0; index < strlen( tags ); index++ ) {
        if ( tags[index] == ' ' || tags[index] == '\0' ) {
            //add tag here
            if (ch_p == 0) continue;
            tag[ch_p] = '\0';
            entry_add_tag( e, tag );
            ch_p = 0;   
        } else {
            tag[ch_p++] = tags[index];
        }
    }
}

void entry_set_title( entry * e, char * t )
{
    remove_trailing_white( t );
    free(e->title);
    e->title = strdup( t );
}

void entry_set_content( entry *  e, char * c )
{    
    remove_trailing_white( c );
    free(e->content);
    e->content = strdup( c );
}

void entry_add_tag( entry * e, char * c )
{
    remove_trailing_white( c );
    e->tags[e->tag_count] = strdup(c);
    e->tag_count++;
}

void entry_free( entry * e ) 
{

    free(e->title);
    free(e->content);
    int i;
    
    if ( e->tag_count > 0 ) {
        for ( i = 0; i < e->tag_count; i++ )
        {
            free(e->tags[i]);
        }
    }

    if ( e->link_count > 0 ) {
        for ( i = 0; i < e->link_count; i++ )
        {
            free( e->links[i] );
        }
    }
    
    free(e);

}

uint8_t entry_has_tag( entry * e, const char * tag ) {
    for ( int i = 0; i < e->tag_count; i++ ) {
        if ( strcmp( e->tags[i], tag ) == 0 ) {
            return 1;
        }
    }

    return 0;
}
