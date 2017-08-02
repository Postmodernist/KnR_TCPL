/* ---------------------------------------------------------------------------- */
/* Utils */
/* ---------------------------------------------------------------------------- */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "defs.h"

// do not append these tokens to item
static const int NO_VAL[] = { LPAREN,  RPAREN, LBRACKET, RBRACKET, ASTERISK, ELLIPSIS, COLON };
// do not prepend these tokens with a space
static const int NO_SPC[] = { NUMBER, COMMA, SEMICOLON, EQUALS };


/* Return number of remaining tokens. */
int ntokens() {
    return token ? toklist_end - token : 0;
}

/* Append string to item. */
void item_append(const char* s) {
    if (strlen(item) + strlen(s) + 1 > BUF_LEN) {
        printf("error: item_append: not enough space to apped '%s'\n", s);
        return;
    }
    strcat(item, s);
}

/* Move item to the end of destination string. */
void item_move(char* dest, int len) {
    if (strlen(dest) + strlen(item) + 1 > len) {
        printf("error: item_move: not enough space to apped '%s'\n", item);
        return;
    }
    strcat(dest, item);
    item[0] = '\0';
}

/* Check if token is of type t. Does NOT generate error if false. */
bool accept(tokentype_t t) {  // -> item
    if (ntokens() && token->type == t) {
        if (!inlist_i(token->type, NO_VAL, 7)) {
            if (!inlist_i(token->type, NO_SPC, 4))
                item_append(" ");
            item_append(token->val);
        }
        token++;
        return true;
    }
    return false;
}

/* Check if token is of type t. Generate error if false. */
bool expect(tokentype_t t) {  // -> item
    if (!ntokens()) {
        printf("error: expect: unexpected end of declaration\n");
        return syntax_ok = false;
    }
    if (!accept(t)) {
        printf("error: expect: unexpected token '%s'\n", token->val);
        return syntax_ok = false;
    }
    return true;
}

/* Search itm in a list (string). */
bool inlist_s(const char* itm, const char** list, int len) {
    for (int i = 0; i < len; i++)
        if (strcmp(itm, list[i]) == 0)
            return true;
    return false;
}

/* Search itm in a list (int). */
bool inlist_i(const int itm, const int* list, int len) {
    for (int i = 0; i < len; i++)
        if (itm == list[i])
            return true;
    return false;
}
