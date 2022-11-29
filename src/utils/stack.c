#include "util.h"


Stack *stack_init() {
    Stack *stack = calloc(1, sizeof *stack);
    if (!stack) {
        fprintf(stderr, "[ERROR] > calloc, in func stack_init\n");
        exit(2);
    }
    stack->head = NULL;
    stack->tail = NULL;

    return stack;
}

void stack_push(Stack *stack, Position pos) {
    // Create a new node to push
    Node *new = calloc(1, sizeof *new);
    if (!new) {
        fprintf(stderr, "[ERROR] > calloc, in func stack_push\n");
        exit(2);
    }
    new->next = NULL;
    new->pos = pos;
    // Push it to stack
    if (stack_is_empty(stack)) {
        stack->head = new;
        stack->tail = new;
    } else {
        new->next = stack->head;
        stack->head = new;
    }
}

void stack_pop(Stack *stack) {
    if (!stack_is_empty(stack)) {
        Node *pop = stack->head;
        stack->head = stack->head->next;
        pop->next = NULL;
        free(pop);
    }
}

Position stack_remove(Stack *stack, uint16_t index) {
    Position ret = {0, 0};
    if (!stack_is_empty(stack)) {
        Node *tmp = NULL;
        Node *curr = stack->head;
        for (uint16_t count = 0; count < index; count++) {
            tmp = curr;
            curr = curr->next;
        }
        tmp->next = curr->next;
        ret = curr->pos;
        free(curr);
        return ret;
    }
    return ret;
}

void stack_free(Stack *stack) {
    while (!stack_is_empty(stack)) {
        stack_pop(stack);
    }
    free(stack);
}

void stack_display(Stack *stack) {
    Node *tmp = stack->head;
    while (tmp != NULL) {
        printf("(l:%d, c:%d)->", tmp->pos.l, tmp->pos.c);
        tmp = tmp->next;
    }
    if (tmp == NULL) {
        printf("NULL\n");
    }
    printf("\n");
}

bool stack_is_empty(Stack *stack) {
    return (stack->head == NULL);
}

uint16_t stack_len(Stack *stack) {
    uint16_t length = 0;
    Node *tmp = NULL;
    for (tmp = stack->head; tmp != NULL; tmp = tmp->next) {
        length++;
    }
    return length;
}