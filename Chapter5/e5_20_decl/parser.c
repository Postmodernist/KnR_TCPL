/* ---------------------------------------------------------------------------- */
/* Parser */
/* ---------------------------------------------------------------------------- */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"

char item[BUF_LEN];
bool syntax_ok = true;  // false: parser encountered syntax error

#define have(x) inlist_s(x, (const char**)type_spec, ntype_spec)

/*
declaration:
    declaration-specifiers declarator ;
*/
bool declaration() {  // -> out
    char spec[SPECIFIERS_LEN] = {};
    char decl[DECLARATOR_LEN] = {};
    char name[NAME_LEN] = {};

    item[0] = '\0';
    syntax_ok = true;

    if (declaration_specifiers(spec, SPECIFIERS_LEN)) {
        if (declarator(decl, DECLARATOR_LEN, name)) {
            if (expect(SEMICOLON)) {
                sprintf(out, " %s:%s%s", name, decl, spec);
                return true;
            }
        } else if (syntax_ok)
            printf("error: declaration: expected identifier\n");
    } else if (syntax_ok) 
        printf("error: declaration: expected declaration specifiers\n");

    return false;
}

/* ---------------------------------------------------------------------------- */
/* Declaration specifiers */

/*
declaration-specifiers:
    storage-class-specifier
    type-specifier
    type-qualifier
    storage-class-specifier declaration-specifiers
    type-specifier declaration-specifiers
    type-qualifier declaration-specifiers

- at most one storage-class-specifier may be given in a declaration
- at most one of the 'long' or 'short' may be specified together with 'int'; the meaning is
  the same if 'int' is not mentioned
- 'long' may be specified with 'double'
- at most one of 'signed' or 'unsigned' may be specified with 'int' or any of its 'short' or
  'long' varieties, or with 'char'. Either may appear alone, in which case 'int' is understood
- otherwise, at most one type-specifier may be given in a declaration
- type-qualifiers may appear with any type-specifier
*/
bool declaration_specifiers(char* spec, int slen) {  // -> spec
    bool stor_class_spec = false, type_spec_ok = false;
    char* type_spec[3];  // keep track of type specifiers
    int ntype_spec = 0, i;
    
    while (1) {
        if (syntax_ok && storage_class_specifier()) {   // storage-class-specifier
            if (stor_class_spec) {
                printf("error: declaration_specifiers: multiple storage class specifiers\n");
                return syntax_ok = false;
            } else {
                stor_class_spec = true;
                item_move(spec, slen);
            }
        
        } else if (syntax_ok && type_specifier()) {     // type-specifier
            if (ntype_spec == 3) {
                printf("error: declaration_specifiers: too many type specifiers\n");
                return syntax_ok = false;
            } else {
                type_spec[ntype_spec] = malloc(strlen(item) + 1);
                strcpy(type_spec[ntype_spec++], item + 1);
                item_move(spec, slen);
            }
        
        } else if (syntax_ok && type_qualifier()) {     // type-qualifier
            item_move(spec, slen);
        
        } else
            break;
    }

    if (!syntax_ok) {
        for (i = 0; i < ntype_spec; i++)
            free(type_spec[i]);
        return false;
    }

    // is the type specifiers combination legal?
    if (ntype_spec == 0) {
        if (strlen(spec)) {
            printf("error: declaration_specifiers: type is not specified\n");
            return syntax_ok = false;
        }
        return false;  // no type specifiers given

    } else if (ntype_spec == 1) {
        type_spec_ok = true;

    } else if (ntype_spec == 2) {
        if ((have("int") && (have("short") || have("long") || have("signed") || have("unsigned")))
           || (have("double") && have("long"))
           || (have("char") && (have("signed") || have("unsigned")))
           || (have("short") && (have("signed") || have("unsigned")))
           || (have("long") && (have("signed") || have("unsigned"))))
            type_spec_ok = true;

    } else if (ntype_spec == 3) {
        if (have("int") && (have("short") || have("long")) && (have("signed") || have("unsigned")))
            type_spec_ok = true;
    }

    for (i = 0; i < ntype_spec; i++)
        free(type_spec[i]);

    if (type_spec_ok)
        return true;

    printf("error: declaration_specifiers: illegal combination of type specifiers\n");
    return syntax_ok = false;
}

/*
storage-class-specifier:
    auto register static extern typedef
*/
bool storage_class_specifier() {  // -> item
    if (accept(STORAGE_CLASS_SPECIFIER))
        return true;
    return false;
}

/*
type-specifier:
    void char short int long float double signed unsigned
    struct-or-union-specifier
    enum-specifier
    typedef-name  // not used since it needs context
*/
bool type_specifier() {  // -> item
    if (accept(TYPE_SPECIFIER) || struct_or_union_specifier() || enum_specifier())
        return true;
    return false;
}

/*
type-qualifier:
    const
    volatile
*/
bool type_qualifier() {  // -> item
    if (accept(TYPE_QUALIFIER))
        return true;
    return false;
}

/*
struct-or-union-specifier:
    struct-or-union identifier { struct-declaration-list }
    struct-or-union { struct-declaration-list }
    struct-or-union identifier
*/
bool struct_or_union_specifier() {  // -> item
    char tmp[BUF_LEN] = {};
    bool ident = false, braces = false;

    if (accept(STRUCT_OR_UNION)) {
        item_move(tmp, BUF_LEN);

        if ((ident = accept(IDENTIFIER))) {
            item_move(tmp, BUF_LEN);
            if (!ntokens() || token->type != LBRACE) {
                item_append(tmp);
                return true;
            }
        }

        if ((braces = accept(LBRACE))) {
            item_move(tmp, BUF_LEN);

            if (struct_declaration_list())
                item_move(tmp, BUF_LEN);
            else {
                printf("error: struct_or_union_specifier: invalid struct/union declaration\n");
                return syntax_ok = false;
            }
            
            if (expect(RBRACE)) {
                item_move(tmp, BUF_LEN);
                item_append(tmp);
                return true;
            }
        }

        if (!ident && !braces) {
            // found struct/union but no identifier and no braces
            printf("error: struct_or_union_specifier: stray struct/union specifier\n");
            return syntax_ok = false;
        }
    }

    return false;
}

/*
struct-declaration-list:
    struct-declaration
    struct-declaration struct-declaration-list
*/
bool struct_declaration_list() {  // -> item
    char tmp[BUF_LEN] = {};

    if (struct_declaration()) {
        item_move(tmp, BUF_LEN);
        while (struct_declaration())
            item_move(tmp, BUF_LEN);
        if (syntax_ok) {
            item_append(tmp);
            return true;
        }
    }

    return false;
}

/*
struct-declaration:
    specifier-qualifier-list struct-declarator ;
*/
bool struct_declaration() {  // -> item
    char spec[BUF_LEN] = {};
    char decl[BUF_LEN] = {};
    char name[NAME_LEN] = {};

    if (specifier_qualifier_list(spec, BUF_LEN) && struct_declarator(decl, BUF_LEN, name)) {
        if (strlen(name) + strlen(spec) +  strlen(decl) + 2 > BUF_LEN)
            printf("error: struct_declaration: not enough space in buffer\n");
        if (strlen(name))
            sprintf(item, "%s:%s%s", name, decl, spec);
        else
            sprintf(item, "%s%s", decl, spec);
        return expect(SEMICOLON);
    }
    return false;
}

/*
specifier-qualifier-list:
    type-specifier
    type-qualifier
    type-specifier specifier-qualifier-list
    type-qualifier specifier-qualifier-list
*/
bool specifier_qualifier_list(char* spec, int slen) {  // -> spec
    if (type_specifier() || type_qualifier()) {
        item_move(spec, slen);
        while (type_specifier() || type_qualifier())
            item_move(spec, slen);
        return syntax_ok;
    }
    return false;
}

/*
struct-declarator:
    declarator
    declarator : constant-expression
    : constant-expression
*/
bool struct_declarator(char* decl, int dlen, char* name) {  // -> decl, name
    if (declarator(decl, dlen, name)) {
        if (!ntokens() || token->type != COLON)
            return true;
    } else if (!syntax_ok)
        return false;

    if (accept(COLON)) {
        if (strlen(decl)) {  // expected identifier only
            printf("error: struct_declarator: bitfield '%s' has invalid type\n", name + 1);
            return syntax_ok = false;
        }
        item_append(" bitfield[");
        item_move(decl, dlen);
        if (expect(NUMBER)) {
            item_append("] of");
            item_move(decl, dlen);
            return true;
        } else
            return false;
    }

    return false;
}

/*
enum-specifier:
    enum identifier { enumerator-list }
    enum { enumerator-list }
    enum identifier
*/
bool enum_specifier() {  // -> item
    bool iden = false, braces = false;

    if (accept(ENUM)) {
        if ((iden = accept(IDENTIFIER)))
            if (!ntokens() || token->type != LBRACE)
                return true;

        if ((braces = accept(LBRACE))) {
            if (enumerator_list())
                return expect(RBRACE);
            else {
                printf("error: enum_specifier: invalid enumerator list\n");
                return syntax_ok = false;
            }
        }

        if (!iden && !braces) {
            // found enum but no identifier and no braces
            printf("error: enum_specifier: stray enum specifier\n");
            return syntax_ok = false;
        }
    }
    return false;
}

/*
enumerator-list:
    enumerator
    enumerator , enumerator-list
*/
bool enumerator_list() {  // -> item
    if (enumerator()) {
        while (accept(COMMA))
            if (!enumerator()) {
                printf("error: enumerator_list: expected enumerator after ','\n");
                return syntax_ok = false;
            }
        return true;
    }
    return false;
}

/*
enumerator:
    identifier
    identifier = constant-expression
*/
bool enumerator() {  // -> item
    if (accept(IDENTIFIER)) {
        if (accept(EQUALS))
            return expect(NUMBER);
        return true;
    }
    return false;
}

/* ---------------------------------------------------------------------------- */
/* Declarator */

/*
declarator:
    direct-declarator
    pointer direct-declarator
*/
bool declarator(char* decl, int dlen, char* name) {  // -> decl, name
    char tmp[BUF_LEN] = {};
    bool ret;

    if (!syntax_ok)
        return false;

    if (pointer())          // save pointers to tmp
        item_move(tmp, BUF_LEN);

    ret = direct_declarator(decl, dlen, name);
    
    if (!syntax_ok)
        return false;

    if (strlen(tmp)) {      // append pointers to declarator
        item_append(tmp);
        item_move(decl, dlen);
    }

    return ret;
}

/*
direct-declarator:
    identifier
    ( declarator )
    direct-declarator []
    direct-declarator [ constant-expression ]
    direct-declarator ()
    direct-declarator ( parameter-type-list )
    direct-declarator ( identifier-list )
*/
bool direct_declarator(char* decl, int dlen, char* name) {  // -> decl, name
    char tmp[BUF_LEN] = {};

    if (!syntax_ok)
        return false;

    if (accept(IDENTIFIER))
        item_move(name, NAME_LEN);
    else if (accept(LPAREN) && declarator(decl, dlen, name) && expect(RPAREN))
        ;
    else
        return false;

    while (1) {
        tmp[0] = '\0';
        
        if (accept(LBRACKET)) {                 // [ constant-expression ]
            item_append(" array[");
            item_move(tmp, BUF_LEN);
            
            if (accept(NUMBER))
                item_move(tmp, BUF_LEN);
            
            if (expect(RBRACKET)) {
                strcpy(item, tmp);
                item_append("] of");
                item_move(decl, dlen);
            } else
                return false;

        } else if (accept(LPAREN)) {            // ( parameter-type-list / identifier-list )
            item_append(" function(");
            item_move(tmp, BUF_LEN);
            
            if (parameter_type_list())
                item_move(tmp, BUF_LEN);
            else if (syntax_ok && identifier_list())
                item_move(tmp, BUF_LEN);
            else if (syntax_ok) {
                item_append(" void");
                item_move(tmp, BUF_LEN);
            } else
                return false;

            if (expect(RPAREN)) {
                item_append(" ) returning");
                item_move(tmp, BUF_LEN);
                item_append(tmp);
                item_move(decl, dlen);
            } else
                return false;

        } else
            break;
    }

    return true;
}

/*
pointer:
    *
    * pointer
    * type-qualifier-list
    * type-qualifier-list pointer
*/
bool pointer() {  // -> item
    if (accept(ASTERISK)) {
        type_qualifier_list();
        item_append(" pointer to");
    } else
        return false;
    pointer();
    return true;
}

/*
type-qualifier-list:
    type-qualifier
    type-qualifier type-qualifier-list
*/
bool type_qualifier_list() {  // -> item
    while (type_qualifier());
    return true;
}

/*
parameter-type-list:
    parameter-list
    parameter-list , ...
*/
bool parameter_type_list() {  // -> item
    if (parameter_list()) {
        if (accept(COMMA)) {   // true only if parameter_list detected ellipsis after comma
            expect(ELLIPSIS);  // so this is guaranteed, but let's be defensive
            item_append(" variable number of arguments");
        }
        return true;
    }
    return false;
}

/*
parameter-list:
    parameter-declaration
    parameter-declaration , parameter-list
*/
bool parameter_list() {  // -> item
    char tmp[BUF_LEN] = {};

    if (parameter_declaration()) {
        item_move(tmp, BUF_LEN);
        while (ntokens() > 1 && token->type == COMMA && (token + 1)->type != ELLIPSIS) {
            accept(COMMA);
            item_move(tmp, BUF_LEN);
            if (parameter_declaration())
                item_move(tmp, BUF_LEN);
            else {
                printf("error: parameter_list: expected parameter after ','\n");
                return syntax_ok = false;
            }
        }
        item_append(tmp);
        return true;
    }
    return false;
}

/*
parameter-declaration:
    declaration-specifiers
    declaration-specifiers declarator
    declaration-specifiers abstract-declarator
*/
bool parameter_declaration() {  // -> item
    char spec[BUF_LEN] = {};
    char decl[BUF_LEN] = {};
    char name[NAME_LEN] = {};
    token_t* token_save;
    bool syntax_ok0;

    if (declaration_specifiers(spec, BUF_LEN)) {
        token_save = token;                     // save token position
        if (declarator(decl, BUF_LEN, name))    // try declarator
            sprintf(item, "%s:%s%s", name, decl, spec);
        else {
            syntax_ok0 = syntax_ok;             // remember if declarator found error
            syntax_ok = true;                   // reset syntax flag
            decl[0] = '\0';                     // reset decl
            token = token_save;                 // restore token
            if (abstract_declarator(decl, BUF_LEN))  // try abstract declarator
                sprintf(item, "%s%s", decl, spec);
            else if (syntax_ok0 && syntax_ok)   // both declarators returned empty, no error
                sprintf(item, "%s", spec);
            else
                return syntax_ok = false;       // declarators found error, but none returned true
        }
        return true;
    }
    return false;
}

/*
identifier-list:
    identifier
    identifier , identifier-list
*/
bool identifier_list() {  // -> item
    if (accept(IDENTIFIER)) {
        while (accept(COMMA) && expect(IDENTIFIER));
        return syntax_ok;
    }
    return false;
}

/*
abstract-declarator:
    pointer
    pointer direct-abstract-declarator
    direct-abstract-declarator
*/
bool abstract_declarator(char* decl, int dlen) {  // -> decl
    char tmp[BUF_LEN] = {};

    if (!syntax_ok)
        return false;

    if (pointer())          // save pointers to tmp
        item_move(tmp, BUF_LEN);

    direct_abstract_declarator(decl, dlen);

    if (!syntax_ok)
        return false;

    if (strlen(tmp)) {      // append pointers to declarator
        item_append(tmp);
        item_move(decl, dlen);
    }

    if (strlen(decl))
        return true;
    return false;
}

/*
direct-abstract-declarator:
    ( abstract-declarator )
    ()
    []
    [ constant-expression ]
    direct-abstract-declarator []
    direct-abstract-declarator [ constant-expression ]
    direct-abstract-declarator ()
    direct-abstract-declarator ( parameter-type-list )
*/
bool direct_abstract_declarator(char* decl, int dlen) {  // -> decl
    char tmp[BUF_LEN] = {};

    if (!syntax_ok)
        return false;

    if (accept(LPAREN)) {
        if (abstract_declarator(decl, dlen)) {        // ( abstract-declarator )
            if (!expect(RPAREN))
                return false;
        } else if (expect(RPAREN)) {            // ()
            item_append(" function( void ) returning");
            item_move(decl, dlen);
        } else
            return false;

    } else if (ntokens() && token->type == LBRACKET)
        ;
    else
        return false;

    while (1) {
        tmp[0] = '\0';
        
        if (accept(LBRACKET)) {                 // [ constant-expression ]
            item_append(" array[");
            item_move(tmp, BUF_LEN);
            
            if (accept(NUMBER))
                item_move(tmp, BUF_LEN);
            
            if (expect(RBRACKET)) {
                strcpy(item, tmp);
                item_append("] of");
                item_move(decl, dlen);
            } else
                return false;

        } else if (accept(LPAREN)) {            // ( parameter-type-list )
            item_append(" function(");
            item_move(tmp, BUF_LEN);
            
            if (parameter_type_list())
                item_move(tmp, BUF_LEN);
            else if (syntax_ok) {
                item_append(" void");
                item_move(tmp, BUF_LEN);
            } else
                return false;

            if (expect(RPAREN)) {
                item_append(" ) returning");
                item_move(tmp, BUF_LEN);
                item_append(tmp);
                item_move(decl, dlen);
            } else
                return false;

        } else
            break;
    }

    return true;
}
