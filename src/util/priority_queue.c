#include "util.h"


PQueue *pqueue_init() {
    PQueue *q = calloc(1, sizeof (*q));
    if (!q) {
        fprintf(stderr, "\x1b[1m%s:\x1b[0m In function '%s':\033[31m%s:%i: error: calloc failed\033[0m\n",
            __FILE__, __func__, __FILE__, ((__LINE__) - 3));
        exit(2);
    }
    *q = (PQueue) {NULL, 0};
    return q;
}

void pqueue_enqueue(PQueue *q, Point p) {
    PQNode *new = calloc(1, sizeof (*new));
    if (!new) {
        fprintf(stderr, "\x1b[1m%s:\x1b[0m In function '%s':\033[31m%s:%i: error: calloc failed\033[0m\n",
            __FILE__, __func__, __FILE__, ((__LINE__) - 3));
        exit(2);
    }
    *new = (PQNode) {p, NULL};
    q->nb_points++;
    if (!q->head) {
        q->head = new;
        return;
    }
    PQNode *tmp = q->head;
    if (tmp->p.f > new->p.f) {
        new->next = tmp;
        q->head = new;
        return;
    }
    while (tmp->next && new->p.f >= tmp->next->p.f) {
        tmp = tmp->next;
    }
    new->next = tmp->next;
    tmp->next = new;
}

Point pqueue_dequeue(PQueue *q) {
    PQNode *tmp = q->head;
    Point p = tmp->p;
    q->head = q->head->next;
    free(tmp);
    q->nb_points--;
    return p;
}

void pqueue_free(PQueue *q) {
    while (q->head) {
        pqueue_dequeue(q);
    }
    free(q);
}

void pqueue_display(PQueue *q) {
    for (PQNode *tmp = q->head; tmp; tmp = tmp->next) {
        printf("(%hhu, %hhu, %u, %u) ", tmp->p.pos.l, tmp->p.pos.c, tmp->p.f, tmp->p.g);
    }
    printf("\n");
}