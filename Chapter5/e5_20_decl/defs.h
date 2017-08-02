#ifndef DEFS
#define DEFS

#define OUTPUT_LEN      2200
#define INPUT_LEN       500
#define MAX_TOKENS      500
#define BUF_LEN         500
#define SPECIFIERS_LEN  1000
#define DECLARATOR_LEN  1000
#define NAME_LEN        100

/* Types of tokens */
typedef enum { IDENTIFIER, NUMBER, LPAREN, RPAREN, LBRACKET, RBRACKET, LBRACE, RBRACE, EQUALS,
    SEMICOLON, COLON, COMMA, ASTERISK, ELLIPSIS, STORAGE_CLASS_SPECIFIER, TYPE_SPECIFIER,
    TYPE_QUALIFIER, STRUCT_OR_UNION, ENUM } tokentype_t;

/* Token */
typedef struct {
    tokentype_t type;
    char*       val;
} token_t;

/* ---------------------------------------------------------------------------- */
/* main */

extern char out[];
extern bool read_EOF;

/* ---------------------------------------------------------------------------- */
/* tokenizer */

extern token_t* toklist_end;
extern token_t* token; 

bool tokens_read(void);
void token_make(tokentype_t, const char*);
void tokens_free(void);
void tokens_print(void);

/* ---------------------------------------------------------------------------- */
/* parser */

extern char item[];
extern bool syntax_ok;

bool declaration(void);
bool declaration_specifiers(char*, int);
bool storage_class_specifier(void);
bool type_specifier(void);
bool type_qualifier(void);
bool struct_or_union_specifier(void);
bool struct_declaration_list(void);
bool struct_declaration(void);
bool specifier_qualifier_list(char*, int);
bool struct_declarator(char*, int, char*);
bool enum_specifier(void);
bool enumerator_list(void);
bool enumerator(void);
bool declarator(char*, int, char*);
bool direct_declarator(char*, int, char*);
bool pointer(void);
bool type_qualifier_list(void);
bool parameter_type_list(void);
bool parameter_list(void);
bool parameter_declaration(void);
bool identifier_list(void);
bool abstract_declarator(char*, int);
bool direct_abstract_declarator(char*, int);

/* ---------------------------------------------------------------------------- */
/* utils */
int  ntokens(void);
void item_append(const char*);
void item_move(char*, int);
bool accept(tokentype_t);
bool expect(tokentype_t);
bool inlist_s(const char*, const char**, int);
bool inlist_i(const int, const int*, int);

#endif
