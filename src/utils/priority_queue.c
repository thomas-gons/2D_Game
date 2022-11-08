#include "util.h"

void insert(PriorityQ *prio_q, Point p) {
    if (prio_q->len + 1 == prio_q->points_len) {
        int new_memory = sizeof(Point) * 10 + prio_q->memory_size;
        Point *tmp = (Point *) realloc(prio_q->points, new_memory);
        if (!tmp) {
            printf("Failed to realloc memory for temporary point");
            exit(-1);
        }
        *prio_q = (PriorityQ) {tmp, .memory_size = new_memory};
        prio_q->points_len += 10; 
    }
    unsigned max = prio_q->points_len + 1;
    prio_q->points[max] = p;
    for (int i = max; (compare(prio_q->points[i/2], prio_q->points[i])) && i/2; i /= 2) {
        Point tmp = prio_q->points[i/2];
        prio_q->points[i/2] = prio_q->points[i];
        prio_q->points[i] = tmp;
    }
    prio_q->len += 1;
}

Point pop(PriorityQ *prio_q) {
    Point p = prio_q->points[1];
    if (prio_q->len == 1) {
        prio_q->len -= 1;
        return p;    
    }

    prio_q->points[1] = prio_q->points[prio_q->len];
    prio_q->len -= 1;

    heapify(prio_q, 1);
    return p;
}

void heapify(PriorityQ *prio_q, unsigned index) {
	Point* parent = getPoint(prio_q, index);
	if (!parent)
        return;	
	
	Point *leftChild = getPoint(prio_q, index * 2);
	if (leftChild && compare(*parent, *leftChild) == 1) {
		swap(parent, leftChild);
		heapify(prio_q, (index * 2));
		heapify(prio_q, index);
        return;
	}
	
	
	Point *rightChild = getPoint(prio_q, index * 2 + 1);
	if (rightChild && compare(*parent, *rightChild) == 1) {
		swap(parent, rightChild);
		heapify(prio_q, index * 2 + 1);
		heapify(prio_q, index);
        return;
	}
}

void priorityQ_free(PriorityQ *prio_q) {
    if (prio_q)
        free(prio_q->points);
    
    free(prio_q);
}