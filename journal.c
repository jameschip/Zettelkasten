
#include "journal.h"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <dirent.h> 
#include <unistd.h>
#include <stdbool.h> 
#include <ctype.h>



/**
 * Create a path to the numbered journal entry.
 *
 */
char * journal_file_path( uint64_t fn ) {
    char file_name[10];
    sprintf(file_name, "%i", fn);

    char * home = getenv("HOME");
    char * temp = calloc( strlen( home ) + strlen( JOURNAL_DIR ) + strlen( file_name ) + 1, sizeof( char ) );

    sprintf(temp, "%s%s%s", home, JOURNAL_DIR, file_name );
    
    return temp;

}

/**
 * Get the path of the journal folder.
 *
 */
char * get_journal_location( void ) {
    char * home = getenv("HOME");
    char * temp = malloc( strlen( home ) + strlen( JOURNAL_DIR ) + 1 );

    sprintf(temp, "%s%s", home, JOURNAL_DIR );
    
    return temp;
}

/**
 * Writes file header content out to the given file.
 * 
 * Header content is between "---" tags, each line is then a different type of content
 * as denoted by the first character.
 * 
 * '>' is the entries title.
 * '#' is a tag that the entry has.
 * '~' is a link to another article
 * 
 */
void creaate_entry_header( entry * e, FILE * fp) {
    
    fputs( "---\n", fp );
        
    char * temp_title = (char *) calloc( strlen( e->title ) + 4, sizeof( char ) );
    sprintf( temp_title, "> %s\n", e->title );
    fputs( temp_title, fp );
    // free( temp_title );

    for (int index = 0; index < e->tag_count; index++) {
        char * temp_tag = (char *) calloc( strlen(e->tags[index] ) + 4, sizeof( char ) );
        sprintf(temp_tag, "# %s\n", e->tags[index]);
        fputs( temp_tag, fp );
        // free(temp_tag);
    }

    for (int index = 0; index < e->link_count; index++) {
        char * temp = (char *) calloc( strlen( e->links[index] ) + 3, sizeof( char ) );
        sprintf(temp, "~ %s\n", e->links[index]);
        fputs( temp, fp );
        // free(temp);
    }
 
    fputs( "---\n", fp );

}

void init_journal( void ) {
    
    char * journal_dir = get_journal_location();
    
    DIR* dir = opendir(journal_dir);

    if (dir) {
        closedir(dir);
    } else if (ENOENT == errno) {
        int status = mkdir(journal_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if (!status) {
            // Success
        } else {
            printf("Failed to create the journal directory -> %s \n", journal_dir);
            free( journal_dir );
            exit(EXIT_FAILURE);
        }
    } else {
        printf("Opendir failed for -> %s \n", journal_dir);
        free( journal_dir );
        exit(EXIT_FAILURE);
    }

    free( journal_dir );

}


uint64_t journal_get_count( void ) {
    
    char * journal_dir = get_journal_location();

    struct dirent *de;
    uint64_t count = 0;
    
    DIR* dir = opendir(journal_dir);
    
    if (dir) {

        while ((de = readdir(dir)) != NULL) {

            // Ignore any files that start with a '.' as these will not be entries in the journal.
            if (de->d_name[0] == '.') { 
                continue;   
            }

            count++;

        }

        closedir(dir);

    } else if (ENOENT == errno) {
        printf("Could not open directory for -> %s \n", journal_dir);
        free( journal_dir );
        exit(EXIT_FAILURE);
    } else {
        printf("Opendir failed for -> %s \n", journal_dir);
        free( journal_dir );
        exit(EXIT_FAILURE);
    }

    free(journal_dir);
    return count;

}

int journal_create_entry( entry * f ) {
    
    char * file_dir;
    file_dir = journal_file_path( f->entry_number );

    if( access( file_dir, F_OK ) != -1 ) {
        /*
        File already exists so we increment the 
        entry number and try again.
        */
        f->entry_number++;
        return journal_create_entry(f);

    } else {
        /*
        File does not exist so we create a new file
        and write the contents of the entry to it.
        */
        FILE * fp;                      
        fp = fopen(file_dir, "w");

        if (fp == NULL)  {
            printf("Unable to create file.\n");
            exit(EXIT_FAILURE);
        }

        creaate_entry_header( f, fp);
        
        fputs(f->content, fp);

        fflush(fp);
        fclose(fp);

    }

}

int journal_update_entry( entry * f ) {
    
    char * file_dir;
    file_dir = journal_file_path( f->entry_number );

    FILE * fp;                      
    fp = fopen(file_dir, "w");
    int ret = remove( file_dir );

    if(ret == 0) {
        journal_create_entry( f );
    } else {
        printf("Error: unable to delete the file");
        exit( 1 );
    }

}

void parse_header_material( entry * e, char * l)
{
    int i = 0;
    char * processed_ln = calloc( 1 , sizeof( char ) );
    // char * tmp;
    char *entry_number_string = calloc( 1, sizeof( char) );
    int entry_number;

    // There should always be a space after the tag and before the content
    // so we copy l[2] toskip it.
    while ( isspace( l[i] ) ) i++;
    switch ( l[i] ) {
        case '#': // Tag
            processed_ln = realloc( processed_ln, strlen( l ) );
            i += 1;
            while ( isspace( l[i] ) ) i++;
            strcpy( processed_ln, &l[i] );
            if( processed_ln[ strlen(processed_ln) - 1] == '\n' )
                processed_ln[ strlen(processed_ln) - 1] = '\0';
            entry_add_tag( e, processed_ln );
            // free( processed_ln );
            break;

        case '>': // Title
            processed_ln = realloc( processed_ln, strlen( l ) );
            i += 1;
            while ( isspace( l[i] ) ) i++;
            strcpy( processed_ln, &l[i] );
            if( processed_ln[ strlen(processed_ln) - 1] == '\n' )
                processed_ln[ strlen(processed_ln) - 1] = '\0';
            entry_set_title( e, processed_ln );
            // free( processed_ln );
            break;

        case '~': // Link
            entry_number = strtol( &l[ 3 ], &entry_number_string, 10 );  // Get entry number from start of string.
            i = 0;
            while ( l[ i ] != ':' ) i++;
            while ( isspace( l[ i ] ) ) i++;
            processed_ln =malloc( strlen(l) + 1);
            strcpy( processed_ln, &l[ i + 2 ] );
            if( processed_ln[ strlen(processed_ln) - 1] == '\n' )
                processed_ln[ strlen(processed_ln) - 1] = '\0';
            entry_add_link( e, entry_number, processed_ln );
            break;
    }

    
}

void journal_read_entry( entry * e, uint64_t n ) {
    
    char * file_dir = journal_file_path( n );
    FILE * fp;
    char * line = NULL;
    char * content;
    size_t len = 0;
    size_t read;
    bool header_switch = false;

    fp = fopen(file_dir, "r");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    content = calloc(1, sizeof( char ) );
    
    while ( (read = getline(&line, &len, fp) ) != -1) {
        // Entering or leaving header material.
        if ( strcmp( line, "---\n" ) == 0 ) {
            header_switch = !header_switch;
            continue;
        }

        // Parse header materials.
        if ( header_switch ) {
            parse_header_material( e, line );
        } else {
            char * tmp = realloc( content, sizeof(char *) * (strlen(line) + strlen(content) + 1) );
            if ( tmp == NULL ) {
                printf( "Unable to assign memory for file content.\n" );
                exit(EXIT_FAILURE);
            } 
            content = tmp;
            strcat( content, line );
        }

    }

    entry_set_content( e, content );
    e->entry_number = n;
    // e->content = strdup( content );

    free( content );
    free( file_dir );
    free( line );
    
}

char ** get_all_entry_refs( const char * tag_filter, uint64_t * num ) {

    char ** entry_list = calloc( 1, sizeof(char *) );
    int e_count = journal_get_count();
    *num = 0;
    if (e_count == 0) return entry_list;
    entry * e;
    for ( int index = 0; index < e_count; index++ ) {
        e = entry_init();
        journal_read_entry( e, index );
        if ( tag_filter != NULL ) {
            if ( entry_has_tag( e, tag_filter ) == 0) {
                entry_free(e);
                continue;
            }
        } 
        char ** tmp = realloc(entry_list, (1 + *num) * sizeof(char*) );
        if ( tmp == NULL) {
            printf("FAILED ALLOCATING MEMORY FOR LIST!");
            exit(0);
        }
        entry_list = tmp;
        
        char * en = calloc(7, sizeof(char));
        sprintf(en, "{%i}", e->entry_number );
        entry_list[*num] = malloc( sizeof(char) * (strlen( en ) + strlen( e->title )) + 2 );
        sprintf( entry_list[*num], "%s %s", en, e->title );
        *num += 1;
    
        entry_free(e);
    }
    

    return entry_list;

}