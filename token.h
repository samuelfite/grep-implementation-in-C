#ifndef TOKEN_H
#define TOKEN_H

// Each regex token is made up of a char to match against, and the repetition
//  behavior of the match, we use a dispatch table in token_dp.[c.h] to
//   determine which functions to use to compare/terminate matches
typedef struct Token Token;
struct Token{
    char*   match_char;    // either regex syntax or literal
    int     match_type;      // corresponding type to be used for comparison withing search

    char*   repetition;    // repetition for the match_char
    int     repetition_type; // corresponding type used to determine search termination

    Token*  next_token;  //  pointer to the next token 

};

// Types are defined by token_type_t functions
typedef int             token_type_t ( char* c );
extern token_type_t     is_repeat, is_match_char, is_literal;
extern Token*           build_token( char* regx_line, int* start_index );

extern void             print_token( Token* tk );
extern  void            print_tokens( Token** token_array, int num_tokens );

#endif // include guard
