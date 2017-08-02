/* ---------------------------------------------------------------------------- */
/* Tokenizer */
/* ---------------------------------------------------------------------------- */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"

token_t  toklist[MAX_TOKENS];       // list of tokens
token_t* toklist_end = toklist;     // pointer to place right after last token
token_t* token;                     // pointer to current token

/* Terminal symbols (except identifiers, numbers, and punctuation). */
const char* storage_class_specifiers[] = { "auto", "register", "static", "extern", "typedef" };
const char* type_specifiers[] = { "void", "char", "short", "int", "long", "float", "double",
    "signed", "unsigned" };
const char* type_qualifiers[] = { "const", "volatile" };
const char* struct_or_union[] = { "struct", "union" };

/* Read tokens from input string. */
bool tokens_read() {
    char s[INPUT_LEN], tmp[INPUT_LEN], *sp, *sp0;
    int i;

    // read input string
    if (fgets(s, INPUT_LEN, stdin) == NULL) {
        read_EOF = true;
        return false;
    }

    // tokenize
    for (token = toklist, sp = s; token - toklist < INPUT_LEN; toklist_end = ++token) {
        for (; isspace(*sp); sp++);         // skip whitespace
        if (*sp == '\n' || *sp == '\0')     // end of the input string
            break;

        if (isalpha(*sp) || *sp == '_') {                       // keyword or identifier
            for (sp0 = sp; isalpha(*sp) || isdigit(*sp) || *sp == '_'; sp++);
            i = sp - sp0;
            strncpy(tmp, sp0, i);
            tmp[i] = '\0';
            if (inlist_s(tmp, storage_class_specifiers, 5)) {     // storage-class-specifier
                token_make(STORAGE_CLASS_SPECIFIER, tmp);
                continue;
            
            } else if (inlist_s(tmp, type_specifiers, 9)) {       // type-specifier
                token_make(TYPE_SPECIFIER, tmp);
                continue;

            } else if (inlist_s(tmp, type_qualifiers, 2)) {       // type-qualifier
                token_make(TYPE_QUALIFIER, tmp);
                continue;

            } else if (inlist_s(tmp, struct_or_union, 2)) {       // struct-or-union
                token_make(STRUCT_OR_UNION, tmp);
                continue;

            } else if (strcmp(tmp, "enum") == 0) {              // enum
                token_make(ENUM, tmp);
                continue;

            } else
                token_make(IDENTIFIER, tmp);                    // identifier

        } else if (isdigit(*sp)) {                              // number
            for (sp0 = sp; isdigit(*sp); sp++);
            if (isalpha(*sp) || *sp == '_') {
                printf("error: tokens_read: illegal identifier at [%ld]\n", sp0 - s);
                return false;
            }
            i = sp - sp0;
            strncpy(tmp, sp0, i);
            tmp[i] = '\0';
            token_make(NUMBER, tmp);

        } else {                                                // punctuation
            sp0 = sp++;
            switch (*sp0) {
            case '(': token_make(LPAREN, "("); break;
            case ')': token_make(RPAREN, ")"); break;
            case '[': token_make(LBRACKET, "["); break;
            case ']': token_make(RBRACKET, "]"); break;
            case '{': token_make(LBRACE, "{"); break;
            case '}': token_make(RBRACE, "}"); break;
            case '=': token_make(EQUALS, "="); break;
            case ';': token_make(SEMICOLON, ";"); break;
            case ':': token_make(COLON, ":"); break;
            case ',': token_make(COMMA, ","); break;
            case '*': token_make(ASTERISK, "*"); break;
            case '.':
                if (sp - s < INPUT_LEN - 1 && *sp++ == '.' && *sp++ == '.') {
                    token_make(ELLIPSIS, "...");
                    break;
                }  // else fall through
            default:
                printf("error: tokens_read: unrecognized token at [%ld]\n", sp0 - s);
                return false;
            }
        }
    }

    // no tokens have been read
    if (token == toklist) {
        return false;
    }

    // maximum tokens have been read
    if (token - toklist == MAX_TOKENS)
        printf("warning: tokens_read: maximum tokens have been read\n");
    
    // success
    token = toklist;
    return true;
}

/* Make a token */
void token_make(tokentype_t type, const char* val) {
    token->type = type;
    if (val) {
        token->val = malloc(strlen(val) + 1);
        strcpy(token->val, val);
    } else
        token->val = NULL;
}

/* Deallocate tokens values. */
void tokens_free() {
    for (token = toklist; token < toklist_end; token++)
        if (token->val) {
            free(token->val);
            token->val = NULL;
        }
    toklist_end = toklist;
    token = NULL;
}

/* Print token list. */
void tokens_print() {
    for (; token < toklist_end; token++)
        printf("'%s' ", token->val);
    putchar('\n');
}
