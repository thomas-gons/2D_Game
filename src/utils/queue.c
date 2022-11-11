#include "util.h"


Queue *queue_init() {
    Queue *q = calloc(1, sizeof *q);
    if (!q) {
        fprintf(stderr, "[ERROR] > calloc, in func queue_init\n");
        exit(1);
    }
    *q = (Queue) {NULL, 0};
    return q;
}

void queue_enqueue(Queue *q, Point p) {
    NodeP *new = calloc(1, sizeof *new);
    if (!new) {
        fprintf(stderr, "[ERROR] > calloc, in func queue_enqueue\n");
        exit(1);
    }
    *new = (NodeP) {p, NULL};
    if (!q->head) {
        *q = (Queue) {new, 1};
        return;
    }
    NodeP *tmp = q->head;
    while (tmp->next) {
        tmp = tmp->next;
    }
    tmp->next = new;
    q->nb_points++;
}

Point queue_dequeue(Queue *q) {
    NodeP *tmp = q->head;
    Point p = tmp->p;
    q->head = q->head->next;
    free(tmp);
    q->nb_points--;
    return p;
}

void queue_free(Queue *q) {
    while (q->head) {
        queue_dequeue(q);
    }
    free(q);
}