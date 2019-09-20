#include "token_dp.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256

const Match_Dp match_dp[] = {
//        - name --   - type -------    - corresponding function -
            {".",      ANY_CHAR,              any_char},          //0
            {"\\d",    ANY_DIGIT,             any_digit},         //1
            {"\\D",    ANY_NON_DIGIT,         any_non_digit},     //2
            {"\\w",    ANY_LETTER,            any_letter},        //3
            {"\\W",    ANY_NON_LETTER,        any_non_letter},    //4
            {"\\s",    ANY_WHITESPACE,        any_whitespace},    //5
            {"\\",     LITERAL_B_SLASH,       literal_b_slash},   //6
            {"",       LITERAL,               literal}            //7
    };
const int num_matches = sizeof(match_dp) / sizeof(Match_Dp);

const Repetition_Dp repetition_dp[] = {
            {"?",      ZERO_OR_ONE,           zero_or_one},      //0
            {"+",      ONE_OR_MORE,           one_or_more},      //1
            {"*",      ZERO_OR_MORE,          zero_or_more},     //2
            {"",       ONCE,                  once},             //3
};
const int num_repetitions = sizeof(repetition_dp) / sizeof(Repetition_Dp);


// takes in a given name/char and returns its corresponding type
int type_from_name( char* name )
{

    for ( int i = 0; i < num_repetitions; i++ )
    {
        if ( strcmp( name, repetition_dp[i].name ) == 0 )
        {
            return repetition_dp[i].type;
        }
    }

    for ( int i = 0; i < num_matches; i++ )
    {
        // search for match chars first
        if ( strcmp( name, match_dp[i].name ) == 0 )
        {
            return match_dp[i].type;
        }
    }

    // MUST BE A LITERAL
    return LITERAL;
}


char* literal( char* line, char* match)
{
    // just check if the char matches
//    printf( "in literal *line[%c] match[%c]\n", *line, *match );
    if ( *line == *match )
    {
        return line;
    }
    return NULL;
}

char* any_char( char* line, char* match) 
{
    if ( line == NULL )
        return NULL;
    return line;
}

char* any_digit( char* line, char* match )
{
    if ( line == NULL )
        return NULL;
    if ( isdigit( *line ) != 0 )
        return line;
    return NULL;
}

char* any_non_digit( char* line, char* match)
{
    if ( line == NULL )
        return NULL;
    if ( isdigit( *line ) == 0 )
        return line;
    return NULL;
}

char* any_letter( char* line, char* match)
{
    if ( line == NULL )
        return NULL;
    if ( isalpha( *line ) != 0)
        return line;
    return NULL;
}

char* any_non_letter( char* line, char* match)
{
    if ( line == NULL )
        return NULL;
    if ( isalpha( *line ) == 0)
        return line;
    return NULL;
}

char* any_whitespace( char* line, char* match )
{
    if ( line == NULL )
    {
//        printf( "ANY_WHITESPACE returning null\n" );
        return NULL;
    }
    if ( isspace( *line ) != 0 )
    {
//        printf( "ANY_WHITESPACE returning true\n" );
        return line;
    }
//    printf( "ANY_WHITESPACE returning diet null\n" );
    return NULL;
}

char* literal_b_slash( char* line, char* match)
{
    if ( line == NULL )
        return NULL;
    if (  *line == '\\' )
        return line;
    return NULL;
}

int once( int** min, int** max )
{
    **min = 1;
    **max = 1;
    return 1;
}

int zero_or_one( int** min, int** max )
{
    **min = 0;
    **max = 1;
    return 1;
}

int one_or_more( int** min, int** max )
{
    **min = 1;
    **max = MAX_LINE_LENGTH;
    return 1;
}

int zero_or_more( int** min, int** max )
{
    **min = 0;
    **max = MAX_LINE_LENGTH;
    return 1;
}

