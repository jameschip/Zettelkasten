
#ifndef JOURNAL_H
#define JOURNAL_H

#include <stdint.h>
#include "entry.h"

#define JOURNAL_DIR "/Zettelkasten/"

/**
 * Check that the journal directory exists and if it does not create it.
 *
 */
void init_journal( void );

/**
 * Get the number of entries in the journal
 *
 */
uint64_t journal_get_count( void );

/**
 * create a file with the nessesary contents to be used.
 * 
 */
int journal_create_entry( entry * e );

/**
 * Removes a journal entry before creating it.
 *
 */
int journal_update_entry( entry * f );


/**
 * Read the given entry from the journal.
 * 
 */
void journal_read_entry( entry * e, uint64_t n );

/**
 * Get all of the entries in the journal that have the specific filter.
 * If filter is NULL, get all entries.
 *
 */
char ** get_all_entry_refs( const char * tag_filter, uint64_t * num );

#endif