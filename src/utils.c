#include "include/utils.h"


Db_list *list_init() {
    Db_list *list = malloc(sizeof *list);
    list->head = NULL;
    list->tail = NULL;

    return list;
}

void list_push(Db_list *list, Pos position) {
    // Create a new node to push
    Node *new = malloc(sizeof *new);
    new->prev = NULL;
    new->next = NULL;
    new->position = position;
    // Push it to head of linked list
    if (list_is_empty(list)) {
        list->head = new;
        list->tail = new;
    } else {
        new->prev = list->head;
        list->head->next = new;
    }
}

Node *list_pop(Db_list *list) {
    Node *pop = NULL;
    if (list_is_empty(list)) {
        return NULL;
    }
    pop = list->tail;
    list->tail = list->tail->next;

    return pop;
}

void list_free(Db_list *list) {
    if (!list_is_empty(list)) {
        // Free all nodes
        Node *curr = list->head;
        while (curr != NULL) {
            free(curr);
            curr = list->head->next;
        }
    }
    free(list);
}

bool list_is_empty(Db_list *list) {
    return (list->head == NULL);
}

uint16_t list_len(Db_list *list) {
    uint16_t length = 0;
    Node *tmp = NULL;
    for (tmp = list->head; tmp != NULL; tmp = tmp->next) {
        length++;
    }

    return length;
}