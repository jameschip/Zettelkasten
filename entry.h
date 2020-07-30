
#ifndef ENTRY_H
#define ENTRY_H

#include <stdint.h>

/**
 * This is an entry in the journal.
 * 
 * entry_number -> the number of the entry.
 * tag_count    -> the number of tags that are associated with this entry.
 * link_count   -> the number of links this entry has.
 * title        -> the title of this entry.
 * content      -> the meanninfull text this entry has.
 * tags[]       -> the tags themselves.
 * entry_links  -> the links themselves.
 */
typedef struct entry {
    uint64_t entry_number;
    uint8_t tag_count;
    uint8_t link_count;
    char * title;
    char * content;
    char * tags[50];
    char * links[50];
} entry;


/**
 * Init an empty entry in the journal.
 * 
 */
entry * entry_init( void );

/**
 * Create and add a new link to a journal entry.
 *
 */
void entry_add_link( entry * e, uint64_t n, char * l );

/**
 * Set the content of the journal entry.
 * 
 */
void entry_set_content( entry * e, char * c );

/**
 * Set the title of the Journal entry.
 */
void entry_set_title( entry * e, char * t );

/**
 * Add a tag to this entry.
 * 
 */
void entry_add_tag( entry * e, char * tag );

void entry_add_tags( entry * e, char * tags );

/**
 * Free the memory used by a journal entry.
 * 
 */
void entry_free( entry * e );

/**
 * Returns 1 if the entry has the given tag and 0 if not.
 *
 */
uint8_t entry_has_tag( entry * e, const char * tag );
#endif