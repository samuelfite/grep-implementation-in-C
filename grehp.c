#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "token_dp.h"
#include "token.h"

#define MAX_LINE_LENGTH 256

/*
 * REGEX MATCHING RULES
 *
 * .  match any character
 * \d match any digit[0-9]
 * \D match any non-digit
 * \w match any letter[a-zA-Z]
 * \W match any non-letter
 * \s match any whitespace character (space or tab '\t')
 * \\ match a single literal backslack character '\'
 *
 * ?  match 0 or 1 instances
 * +  match 1 or more instances
 * *  match 0 or more instances
 */

int get_possibile_starts( char* input_line, Token* tk, int is_first )
{
    // populate min/max bounds
    int** min = (int**)calloc( 1, sizeof(int*) );
    *min = (int*)calloc( 1, sizeof(int) );
    int** max = (int**)calloc( 1, sizeof(int*) );
    *max= (int*)calloc( 1, sizeof(int) );

    repetition_dp[tk->repetition_type].function( min, max );

    int starts_found = 0;
    char** possible_starts = (char**)calloc( MAX_LINE_LENGTH, sizeof(char*) );
    char** original = possible_starts;
    int len = strlen( input_line );
    // on first token, populate all possible starting points
    for ( int j = 0; j < len; j++ )
    {
        char* new = "";
        new = match_dp[tk->match_type].function( input_line, tk->match_char );
        if ( new != NULL && ( **max > starts_found || is_first ) )
        {
            if ( is_first || (tk->repetition_type == ONE_OR_MORE ) || (tk->repetition_type == ZERO_OR_MORE) )
            {
                possible_starts[starts_found] = ++input_line;
                starts_found += 1;
            }
            //only allow one starting point after first
            else 
            {
                possible_starts[0] = ++input_line;
                starts_found = 1;
            }
        }
        else if ( is_first )
        {
            input_line++;
        }
        else
        {
//           colou?r
            if ( **min == 0 )
            {
                possible_starts[starts_found] = input_line;
                starts_found += 1;
            }
                break;
        }
    }

    free( *min ); free( min ); free( *max ); free( max );

    int return_arr = 0;
    for ( int i = 0; i < starts_found; i++ )
    {
        //BASE CASE: Reached end of regex successfully
        if ( tk->next_token == NULL )
        {
            free( original );
            return 1;
        }
        else
        {
            return_arr = get_possibile_starts( possible_starts[i], tk->next_token, 0 );
            if ( return_arr == 1 )
            {
                free( original );
                return 1;
            }
        }
    }
    //BASE CASE: No potential starting points found, i.e no matches
    free( original );
    return 0;
}



int main( int argc, char** argv )
{
    setvbuf( stdout, NULL, _IONBF, 0 );

    if (argc != 3)
    {
        fprintf(stderr, "ERROR: Invalid arguments\nUSAGE: a.out <regex-file> <input-file>\n");
        return EXIT_FAILURE;
    }

    /*
     * =====ARGV[] FILE PARSING===========================
     */
    char* input_filename = (char*)calloc( ( strlen(argv[2]) + 1 ), sizeof(char) );
    strcpy( input_filename, argv[2] );
#ifdef DEBUG
    fprintf( stderr, "INPUT_FILENAME: %s \n", input_filename );
#endif
    FILE* input_fp;
    input_fp = fopen( input_filename, "rb" );
    if ( input_fp == NULL ) 
    {
        free( input_filename );
        perror( "ERROR: fopen() failed during opening of argv[2]" );
        return EXIT_FAILURE;
    }

    //--Read input file--
    fseek( input_fp, 0, SEEK_END ); //set file pointer to eof
    long input_size =  ftell( input_fp ); //get distance from 0-eof
    fseek( input_fp, 0, SEEK_SET ); //set file pointer back to start

    char* input_string = (char*)calloc( ( input_size + 1 ), sizeof(char) );
    if ( input_string == NULL )
    {
        perror( "ERROR: calloc failed" );
        return EXIT_FAILURE;
    }

    int bytes_read = fread( input_string, 1, input_size, input_fp ); 
    if ( bytes_read == 0 )
    {
        perror( "ERROR: fread() failed" );
        return EXIT_FAILURE;
    }

    fclose( input_fp );
    input_string[input_size] = 0; //null terminate string

#ifdef DEBUG
    printf( "INPUT FILE CONTENTS: \n%s", input_string );
#endif


    char* regex_filename = (char*)calloc( ( strlen(argv[1]) + 1 ), sizeof(char) );
    strcpy( regex_filename, argv[1] );
#ifdef DEBUG
    fprintf( stderr, "REGEX_FILENAME: %s \n", regex_filename );
#endif
    FILE* regex_fp; 
    regex_fp = fopen( regex_filename, "rb" );
    if ( regex_fp == NULL ) 
    {
        free( input_fp );
        free( input_string );
        free( regex_filename );
        perror( "ERROR: fopen() failed during opening of argv[1]" );
        return EXIT_FAILURE;
    }

    //--Read regex file--
    fseek( regex_fp, 0, SEEK_END ); //set file pointer to eof
    long regex_size =  ftell( regex_fp ); //get distance from 0-eof
    fseek( regex_fp, 0, SEEK_SET ); //set file pointer back to start

    char* regex_string = calloc( (regex_size + 1), sizeof(char) );
    if ( regex_string == NULL )
    {
        perror( "ERROR: calloc failed" );
        return EXIT_FAILURE;
    }

    bytes_read = fread( regex_string, 1, regex_size, regex_fp ); 
    if ( bytes_read == 0 )
    {
        perror( "ERROR: fread() failed" );
        return EXIT_FAILURE;
    }

    fclose( regex_fp );
    regex_string[regex_size] = 0; //null terminate string

#ifdef DEBUG
    printf( "REGEX FILE CONTENTS: \n%s", regex_string );
#endif
    /*
     * =====END ARGV[] FILE PARSING===========================
     */

    //"regex_string" and "input_string" are both char arrays that 
    //  will be split on '\n' and will be treated as our lines of input


    //===REGEX INPUT(array) LOOP=============================
    char* given_regex_line = strtok( regex_string, "\n" );
    while ( (given_regex_line != NULL) && (strlen(given_regex_line) > 1 ) )
    {
        // TOKEN ARRAY: at most MAX_LINE_LENGTH tokens in a given regex
        Token** token_array = (Token**)calloc( MAX_LINE_LENGTH, sizeof( Token* ) );
        int* start_index = (int*)calloc( 1, sizeof(int) ); //build_token increments start_index
        *start_index = 0;

        //BUILD TOKENS from given_regex_line
        int num_tokens = 0;
        token_array[num_tokens] = build_token( given_regex_line, start_index );
        while( token_array[num_tokens]  != NULL ) //will be null when start_index == len(given_regex_line)-1
        {
            num_tokens++;
            token_array[num_tokens] = build_token( given_regex_line, start_index );
            if ( token_array[num_tokens] != NULL )
                token_array[num_tokens-1]->next_token = token_array[num_tokens];
        }
        free( start_index );
        
        //===INPUT FILE(array) LOOP =================================
        char* input_line = strtok( input_string, "\n" );
        //char* input_line_copy = (char*)calloc( MAX_LINE_LENGTH, sizeof(char)  );
        char* input_line_copy = (char*)calloc( MAX_LINE_LENGTH, sizeof(char)  );
        int found = 0;
        while ( input_line != NULL ) 
        {
            // save unmodified line for printing
            strcpy( input_line_copy, input_line );
            // LOOP OVER regex TOKENS ---

            // start recursive find for first token
            if ( ( found = get_possibile_starts( input_line_copy, token_array[0], 1 )) != 0 )
            {
                printf( "%s\n", input_line_copy );

            }
            //---bottom of input file loop
            input_line = strtok( NULL, "\n" );
        }

        free( input_line_copy );
        
        // ---bottom of regex file loop
        given_regex_line = strtok( NULL, "\n" );
        for( int i = 0; i < num_tokens; i++ )
        {
            free( token_array[i]->match_char );
            free( token_array[i]->repetition );
            free( token_array[i] );
        }
        free( token_array );
    }

    //---FREE VARS
    free( regex_string );
    free( input_string );
    free( input_filename );
    free( regex_filename );


    return EXIT_SUCCESS;
}
