#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "token.h"
#include "token_dp.h"

#define MAX_LINE_LENGTH 256

/* 
 * RETURNS: 
 *          1: is a repeat char, defined here
 *          0: 0 otherwise
 */
int is_repeat( char* c )
{
    if (*c == '?' || *c == '+' || *c == '*')
        return 1;
    return 0;
}

/*
 * RETURNS length of match_char match
 *        2: \\* match
 *        1: .  match
 *        0: otherwise
 */
int is_match_char( char* c )
{
    if ( *c == '\\' )
        return 2;
    if ( *c == '.' )
        return 1;
    return 0;
}

/* 
 * RETURNS:
 *         0: if c is a 'match_char' or a 'repetition' 
 *         1: otherwise
 */
int is_literal( char* c )
{
    if ( is_match_char( c ) || is_repeat( c ) )
        return 0;
    return 1;
}

// Prints information on given token
void print_token( Token* tk )
{
    fprintf( stdout, "==================\n");
    fprintf( stdout, "TOKEN[%p]:\n", tk);
    fprintf( stdout, "MATCH CHAR[%s] MATCH TYPE[%d]\n", tk->match_char, tk->match_type );
    fprintf( stdout, "REPETITION[%s] REPETITION TYPE[%d]\n", tk->repetition, tk->repetition_type );
    fprintf( stdout, "NEXT_TOKEN[%p]\n", tk->next_token);
    fprintf( stdout, "==================\n");
}

// Calls print_token for every token in token_array of size num_tokens
void print_tokens( Token** token_array, int num_tokens )
{
    printf( "FOUND %d TOKENS\n", num_tokens );
    for ( int i = 0; i < num_tokens; i++ )
    {
        print_token( token_array[i] );
    }

}


// takes in a regex line and will BUILD A TOKEN leaving regx_line at end
//  of previous token build, similar to strtok
Token* build_token( char* regx_line, int* start_index )
{
    if ( *start_index >= ( strlen( regx_line ) - 1 ) )
        return NULL;

    Token* tk = (Token*)calloc( 1, sizeof(Token) );
    //tk->match_char = "";
    //tk->repetition = "";

    tk->match_char = (char*)calloc( 3, sizeof( char ) ); //longest match_char is 2 long
    tk->repetition = (char*)calloc( 2, sizeof( char ) ); //longest match_char is 1 long
    tk->next_token = NULL;

    //=============CHAR / LITERAL ========================
    //if we are starting a match char with "\"
    if ( is_match_char( &regx_line[*start_index] ) == 2 )
    {
        tk->match_char[0] = regx_line[*start_index];
        *start_index += 1;
        tk->match_char[1] = regx_line[*start_index];

        *start_index = *start_index + 1;
    }
    // should have either '.'
    else if ( is_match_char( &regx_line[*start_index] ) == 1 )
    {
        tk->match_char[0] = regx_line[*start_index];

        *start_index += 1;
    }
    else if ( is_literal( &regx_line[*start_index] ) )
    {
        tk->match_char[0] = regx_line[*start_index];

        *start_index += 1;
    }
    // idk how else we would start
    else
    {
        fprintf( stderr, "Started on a non valid regex %c\n", regx_line[*start_index] );
    }

    //=============REPETITION ========================
    // REPEAT CHAR
    if ( is_repeat( &regx_line[*start_index] ) )
    {
        *(tk->repetition) = regx_line[*start_index];
        
        *start_index += 1;
    }
    // REPEAT of ONE
    else
    {
        memset( tk->repetition, 0, 2*sizeof(char) );
    }
    
//    tk->match_char = match_char;
//    tk->repetition = repetition;

    tk->match_type = type_from_name( tk->match_char );
    tk->repetition_type = type_from_name( tk->repetition );

    return tk;
}
