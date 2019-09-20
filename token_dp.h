#ifndef TOKEN_DP_H
#define TOKEN_DP_H 

#include "token.h"
// correspond to indexes in dispatch tables
#define ANY_CHAR         0
#define ANY_DIGIT        1
#define ANY_NON_DIGIT    2
#define ANY_LETTER       3
#define ANY_NON_LETTER   4
#define ANY_WHITESPACE   5
#define LITERAL_B_SLASH  6
#define LITERAL          7

#define ZERO_OR_ONE      0
#define ONE_OR_MORE      1
#define ZERO_OR_MORE     2
#define ONCE             3


typedef int repetition_t( int** min, int** max );

typedef struct{
    char*        name;
    int          type;
    repetition_t* function;
} Repetition_Dp;

extern repetition_t zero_or_one, one_or_more, zero_or_more, once;

extern const Repetition_Dp repetition_dp[];

//returns int pointer to array with range of valid starting positions for next 
// token
typedef char* match_t( char*, char* );

typedef struct{
    char*        name;
    int          type;
    match_t* function;
} Match_Dp;

//Functions in dispatch table
extern match_t any_char, any_digit, any_non_digit, any_letter, any_non_letter,
                        any_whitespace, literal_b_slash, literal;

//Dispatch table
extern const Match_Dp match_dp[];

// helper functions ============================================
extern int is_repetition( char* repitition);
extern int is_literal( char* repitition);

extern int type_from_name( char* name );

//extern char* find_match( char* line, Token* tk, int first, char*** possible_starts );

extern const int        num_matches;
extern const int        num_repetitions;

#endif //include guard
