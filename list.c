// ============================================================================
// list.c -- YOUR WORK for Lab 1.
//
// Implement every function below so it matches its contract in list.h. Each has
// a TODO describing what to do; the header has the ownership rule and Big-O.
//
// The placeholder bodies let the project compile and stay leak-free BEFORE you
// start, so the whole workflow (make, make valgrind, make format) works from
// minute one. As you implement each function, real behaviour appears.
//
// The `(void)x;` lines just tell the compiler "I know x is unused for now" so
// -Werror does not stop you. Delete each one once you use that variable.
// ============================================================================

#include "list.h"

#include <stdlib.h> // you will need malloc and free

TokenList list_create(void) {
    // GIVEN: an empty list is all-null with length 0. (This one is done for you
    // as an example of a designated initializer -- study it, then build the
    // rest below.)
    return (TokenList){.head = nullptr, .tail = nullptr, .length = 0};
}

void list_destroy(TokenList *list) {
    TokenNode *current = list->head;

    while (current != nullptr) {
        TokenNode *next = current->next; // save next FIRST
        token_free(&current->token);     // free the lexeme
        free(current);                   // free the node
        current = next;
    }

    // Reset the list to empty
    *list = list_create();
}
bool list_push_front(TokenList *list, Token token) {
    TokenNode *node = malloc(sizeof(TokenNode));
    if (node == nullptr) {
        token_free(&token);
        return false;
    }

    node->token = token;
    node->next  = list->head; // point to the old head

    list->head = node;

    if (list->tail == nullptr) {
        // List was empty, so this node is also the tail
        list->tail = node;
    }

    list->length++;
    return true;
}

bool list_push_back(TokenList *list, Token token) {
    TokenNode *node = malloc(sizeof(TokenNode));
    if (node == nullptr) {
        token_free(&token);
        return false;
    }

    node->token = token;
    node->next  = nullptr;

    if (list->tail == nullptr) {
        // List is empty
        list->head = node;
        list->tail = node;
    } else {
        // List already has nodes
        list->tail->next = node;
        list->tail       = node;
    }

    list->length++;
    return true;
}

bool list_insert_after(TokenList *list, TokenNode *node, Token token) {
    if (node == nullptr) {
        token_free(&token);
        return false;
    }

    TokenNode *new_node = malloc(sizeof(TokenNode));
    if (new_node == nullptr) {
        token_free(&token);
        return false;
    }

    new_node->token = token;
    new_node->next  = node->next; // new node points to what used to be after `node`
    node->next      = new_node;   // `node` now points to the new node

    if (list->tail == node) {
        // We inserted after the old tail, so update tail
        list->tail = new_node;
    }

    list->length++;
    return true;
}

TokenNode *list_find_first(const TokenList *list, TokenKind kind) {
    for (TokenNode *cur = list->head; cur != nullptr; cur = cur->next) {
        if (cur->token.kind == kind) {
            return cur;
        }
    }
    return nullptr;
}

bool list_remove_first(TokenList *list, TokenKind kind) {
    TokenNode *prev = nullptr;
    TokenNode *cur  = list->head;

    while (cur != nullptr) {
        if (cur->token.kind == kind) {
            // Found the node to remove

            if (prev == nullptr) {
                // Removing the head
                list->head = cur->next;
            } else {
                prev->next = cur->next;
            }

            if (list->tail == cur) {
                // Removing the tail
                list->tail = prev;
            }

            token_free(&cur->token);
            free(cur);
            list->length--;
            return true;
        }

        prev = cur;
        cur  = cur->next;
    }

    return false; // not found
}
size_t list_length(const TokenList *list) {
    return list->length;
}
void list_print(const TokenList *list, FILE *out) {
    for (TokenNode *cur = list->head; cur != nullptr; cur = cur->next) {
        fprintf(out, "  %-9s '%s' (%d:%d)\n", token_kind_name(cur->token.kind), cur->token.lexeme,
                cur->token.line, cur->token.col);
    }
}
void list_to_dot(const TokenList *list, FILE *out) {
    fprintf(out, "digraph tokens {\n");
    fprintf(out, "  rankdir=LR;\n");
    fprintf(out, "  node [shape=box];\n");

    size_t i = 0;
    for (TokenNode *cur = list->head; cur != nullptr; cur = cur->next, i++) {
        fprintf(out, "  n%zu [label=\"%s\\n%s\"];\n", i, token_kind_name(cur->token.kind),
                cur->token.lexeme);
    }

    for (size_t j = 0; j + 1 < i; j++) {
        fprintf(out, "  n%zu -> n%zu;\n", j, j + 1);
    }

    if (i > 0) {
        fprintf(out, "  n%zu -> nnull;\n", i - 1);
    }

    fprintf(out, "  nnull [label=\"nullptr\"];\n");
    fprintf(out, "}\n");
}

bool list_check_invariant(const TokenList *list) {
    size_t count    = 0;
    TokenNode *cur  = list->head;
    TokenNode *last = nullptr;

    while (cur != nullptr) {
        count++;
        last = cur;
        cur  = cur->next;
    }

    // Check length
    if (count != list->length) {
        return false;
    }

    // Check tail
    if (list->length == 0) {
        return list->head == nullptr && list->tail == nullptr;
    } else {
        return list->tail == last;
    }
}